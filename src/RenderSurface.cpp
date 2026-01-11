#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

#include "RenderSurface.hpp"
#include "Model.hpp"

RenderSurface::RenderSurface(int32_t width, int32_t height, Col backgroundColor) :
    m_PixelGrid(width, height, backgroundColor),
    m_ZBuffer(width, height) {}

void RenderSurface::setViewport(int32_t x, int32_t y, int32_t w, int32_t h)
{
    m_Viewport = {
        {w / 2.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, h / 2.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {x + w / 2.0f, y + h / 2.0f, 0.0f, 1.0f}
    };
}

void RenderSurface::setPerspective(float f)
{
    m_Perspective = {
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, -1.0f / f, 1.0f}
    };
}

void RenderSurface::setLookat(glm::fvec3 eye, glm::fvec3 center, glm::fvec3 up)
{
    glm::fvec3 n = glm::normalize(eye - center);
    glm::fvec3 l = glm::normalize(glm::cross(up, n));
    glm::fvec3 m = glm::normalize(glm::cross(n, l));

    m_ModelView = glm::mat4({
        {l.x, m.x, n.x, 0.0f},
        {l.y, m.y, n.y, 0.0f},
        {l.z, m.z, n.z, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}
    }) * glm::mat4({
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {-center.x, -center.y, -center.z, 1.0f}
    });
}

void RenderSurface::line(glm::vec2 a, glm::vec2 b, Col color)
{
    bool steep = std::abs(a.x - b.x) < std::abs(a.y - b.y);
    if (steep)
    {
        std::swap(a.x, a.y);
        std::swap(b.x, b.y);
    }

    if (a.x > b.x) std::swap(a, b);

    int32_t y = a.y;
    int32_t error = 0;
    for (int32_t x = a.x; x <= b.x; ++x)
    {
        if (steep) m_PixelGrid.put(y, x, color);
        else m_PixelGrid.put(x, y, color);

        error += 2 * std::abs(b.y - a.y);
        if (error > b.x - a.x)
        {
            y += b.y > a.y ? 1 : -1;
            error -= 2 * (b.x - a.x);
        }
    }
}

void RenderSurface::rastorize(glm::vec4 v[3], Col c[3])
{
    glm::vec4 normalized[3]{
        v[0] / v[0].w,
        v[1] / v[1].w,
        v[2] / v[2].w
    };
    glm::vec2 screen[3]{
        glm::vec2(m_Viewport * normalized[0]),
        glm::vec2(m_Viewport * normalized[1]),
        glm::vec2(m_Viewport * normalized[2])
    };

    auto [left, right] = std::minmax({screen[0].x, screen[1].x, screen[2].x});
    auto [top, bottom] = std::minmax({screen[0].y, screen[1].y, screen[2].y});
    
    glm::mat3 ABC({
        {screen[0].x, screen[1].x, screen[2].x},
        {screen[0].y, screen[1].y, screen[2].y},
        {1.0f, 1.0f, 1.0f}
    });
    if (glm::determinant(ABC) < 1.0f) return;// That's culling

    for (int32_t y = top; y <= bottom; ++y) for (int32_t x = left; x <= right; ++x)
    {
        const glm::fvec3 barycentric = glm::transpose(glm::inverse(ABC)) * glm::fvec3(x, y, 1.0f);
        if (barycentric.x < 0 || barycentric.y < 0 || barycentric.z < 0) continue;

        float depth = glm::dot(barycentric, glm::fvec3(normalized[0].z, normalized[1].z, normalized[2].z));
        if (!m_ZBuffer.depthTest(x, y, depth)) continue;

        m_PixelGrid.put(
            x, y,
            Colors::Purple
        );
    }
}

void RenderSurface::renderModel(const Model& model)
{
    const size_t faces = model.faces();
    for (size_t i = 0; i < faces; ++i)
    {
        auto face = model.face(i);
        glm::vec4 vertices[3]{
            m_Perspective * m_ModelView * glm::vec4(model.vertice(face.x), 1),
            m_Perspective * m_ModelView * glm::vec4(model.vertice(face.y), 1),
            m_Perspective * m_ModelView * glm::vec4(model.vertice(face.z), 1)
        };
        Col colors[3]{
            Colors::Red,
            Colors::Green,
            Colors::Blue
        };

        rastorize(vertices, colors);
    }
}