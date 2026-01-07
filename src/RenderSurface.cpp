#include <fstream>
#include <vector>
#include <string>

#include "RenderSurface.hpp"
#include "Model.hpp"

void RenderSurface::line(Vec2i a, Vec2i b, Col color)
{
    // That's Brasenham's line drawing algorithm

    // First we check if we'll be moving on the x or y axis
    // If y then we swap x with y on both vectors and later swap them back when drawing in the step where we check for steep again
    // Effectively it means that if the angle is steep our y is x and x is y
    bool steep = std::abs(a.x() - b.x()) < std::abs(a.y() - b.y());
    if (steep)
    {
        std::swap(a.x(), a.y());
        std::swap(b.x(), b.y());
    }

    // We ensure that we're always drawing from left to right
    if (a.x() > b.x()) std::swap(a, b);

    // We set the starting position to the smaller y of the vectors and init error to 0
    int32_t y = a.y();
    int32_t error = 0;
    for (int32_t x = a.x(); x <= b.x(); ++x)
    {
        // Here we step pixel by pixel and fill them in, the branching is explained above
        if (steep) m_PixelGrid.put(y, x, color);
        else m_PixelGrid.put(x, y, color);

        // The error value is here to check if after a move in x we should move in y and if yes then it executes the move
        error += 2 * std::abs(b.y() - a.y());
        if (error > b.x() - a.x())
        {
            y += b.y() > a.y() ? 1 : -1;
            error -= 2 * (b.x() - a.x());
        }
    }
}

void RenderSurface::triangleFrame(Vec2i a, Vec2i b, Vec2i c, Col color)
{
    line(a, b, color);
    line(b, c, color);
    line(c, a, color);
}

void RenderSurface::triangleScanline(Vec2i a, Vec2i b, Vec2i c, Col color)
{
    if (a.y() > b.y()) std::swap(a, b);
    if (b.y() > c.y()) std::swap(b, c);
    if (a.y() > b.y()) std::swap(a, b);

    int32_t totalHeight = c.y() - a.y();

    if (a.y() != b.y())
    {
        int32_t segmentHeight = b.y() - a.y();
        for (int32_t y = a.y(); y < b.y(); ++y)
        {
            int32_t x1 = a.x() + ((c.x() - a.x()) * (y - a.y())) / totalHeight;
            int32_t x2 = a.x() + ((b.x() - a.x()) * (y - a.y())) / segmentHeight;
            m_PixelGrid.putHorizontalLine(y, std::min(x1, x2), std::max(x1, x2), color);
        }
    }

    if (b.y() != c.y())
    {
        int32_t segmentHeight = c.y() - b.y();
        for (int32_t y = b.y(); y <= c.y(); ++y)
        {
            int32_t x1 = a.x() + ((c.x() - a.x()) * (y - a.y())) / totalHeight;
            int32_t x2 = b.x() + ((c.x() - b.x()) * (y - b.y())) / segmentHeight;
            m_PixelGrid.putHorizontalLine(y, std::min(x1, x2), std::max(x1, x2), color);
        }
    }
}

static float triangleArea(Vec2i a, Vec2i b, Vec2i c)
{
    return ((b.y() - a.y()) * (b.x() + a.x()) + (c.y() - b.y()) * (c.x() + b.x()) + (a.y() - c.y()) * (a.x() + c.x())) / 2.0f;
}

void RenderSurface::trianglePixelCheck(Vec3i a, Vec3i b, Vec3i c, Col cA, Col cB, Col cC)
{
    const int32_t top = std::max(0, std::min(a.y(), std::min(b.y(), c.y())));
    const int32_t left = std::max(0, std::min(a.x(), std::min(b.x(), c.x())));
    const int32_t bottom = std::min(m_PixelGrid.height() - 1, std::max(a.y(), std::max(b.y(), c.y())));
    const int32_t right = std::min(m_PixelGrid.width() - 1, std::max(a.x(), std::max(b.x(), c.x())));

    const Vec2i iA(a);
    const Vec2i iB(b);
    const Vec2i iC(c);
    const float totalArea = triangleArea(iA, iB, iC);
    if (totalArea < 1.0f) return;// That's culling

    for (int32_t y = top; y <= bottom; ++y) for (int32_t x = left; x <= right; ++x)
    {
        const Vec2i point(x, y);
        const float areaA = triangleArea(point, iB, iC) / totalArea;
        const float areaB = triangleArea(point, iC, iA) / totalArea;
        const float areaC = triangleArea(point, iA, iB) / totalArea;

        if (areaA < 0 || areaB < 0 || areaC < 0) continue;

        const uint8_t depth = a.z() * areaA + b.z() * areaB + c.z() * areaC;
        if (depth < m_ZBuffer.get(point).r) continue;

        m_ZBuffer.put(
            point,
            Col(
                (a.z() * areaA + b.z() * areaB + c.z() * areaC),
                (a.z() * areaA + b.z() * areaB + c.z() * areaC),
                (a.z() * areaA + b.z() * areaB + c.z() * areaC)
            )
        );

        m_PixelGrid.put(
            point,
            Col(
                (cA.r * areaA + cB.r * areaB + cC.r * areaC),
                (cA.g * areaA + cB.g * areaB + cC.g * areaC),
                (cA.b * areaA + cB.b * areaB + cC.b * areaC)
            )
        );
    }
}

Vec3f RenderSurface::project(Vec3f vertice)
{
    return Vec3f(
        (vertice.x() + 1.0f) * m_PixelGrid.width() / 2.0f,
        (vertice.y() + 1.0f) * m_PixelGrid.height() / 2.0f,
        (vertice.z() + 1.0f) * 255.0f / 2.0f
    );
}

static Vec3f rotate(Vec3f vertice, float angle = 0)
{;
    Mat3 rot{
        {{std::cos(angle), 0, std::sin(angle)},
        {0,1,0},
        {-std::sin(angle), 0, std::cos(angle)}}
    };
    return rot * vertice;
}

void RenderSurface::renderModel(const Model& model, float angle)
{
    Random rD;

    const int32_t faces = model.faces();
    for (int32_t i = 0; i < faces; ++i)
    {
        auto face = model.face(i);
        trianglePixelCheck(
            Vec3i(project(rotate(model.vertice(face.x()), angle))),
            Vec3i(project(rotate(model.vertice(face.y()), angle))),
            Vec3i(project(rotate(model.vertice(face.z()), angle))),
            Colors::Red,
            Colors::Green,
            Colors::Blue
        );
    }
}