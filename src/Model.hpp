#pragma once

#include <fstream>
#include <functional>

#include "glm.hpp"

#include "Shader.hpp"

class Model
{
public:

	Model(const char* path, Shader* shader) :
        m_Shader(shader)
	{
		std::fstream file(path);

		if (!file.is_open()) return;

        std::string in;
        while (file >> in)
        {
            if (in == "v")
            {
                glm::fvec3 v;
                file >> v.x;
                file >> v.y;
                file >> v.z;

                m_Vertices.push_back(v);
            }
            else if (in == "f")
            {
                glm::ivec3 v;
                file >> in;
                v.x = std::atoi(in.substr(0, in.find('/')).c_str()) - 1;
                file >> in;
                v.y = std::atoi(in.substr(0, in.find('/')).c_str()) - 1;
                file >> in;
                v.z = std::atoi(in.substr(0, in.find('/')).c_str()) - 1;

                m_Faces.push_back(v);
            }
        }
	}

    int32_t vertices() const { return m_Vertices.size(); }
    int32_t faces() const { return m_Faces.size(); }

    glm::vec3 vertice(int32_t i) const { return m_Vertices[i]; }
    glm::ivec3 face(int32_t i) const { return m_Faces[i]; }

    Shader* getShader() const { return m_Shader; }
    void setShader(Shader* s) const { m_Shader = s; }

private:

	std::vector<glm::vec3> m_Vertices;
	std::vector<glm::ivec3> m_Faces;
    mutable Shader* m_Shader;
};