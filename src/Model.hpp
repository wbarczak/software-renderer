#pragma once

#include <fstream>

#include "Core.hpp"

class Model
{
public:

	Model(const char* path)
	{
		std::fstream file(path);

		if (!file.is_open()) return;

        std::string in;
        while (file >> in)
        {
            if (in == "v")
            {
                Vec3f v;
                file >> v.x();
                file >> v.y();
                file >> v.z();

                m_Vertices.push_back(v);
            }
            else if (in == "f")
            {
                Vec3i v;
                file >> in;
                v.x() = std::atoi(in.substr(0, in.find('/')).c_str()) - 1;
                file >> in;
                v.y() = std::atoi(in.substr(0, in.find('/')).c_str()) - 1;
                file >> in;
                v.z() = std::atoi(in.substr(0, in.find('/')).c_str()) - 1;

                m_Faces.push_back(v);
            }
        }
	}

    int32_t vertices() const { return m_Vertices.size(); }
    int32_t faces() const { return m_Faces.size(); }

    Vec3f vertice(int32_t i) const { return m_Vertices[i]; }
    Vec3i face(int32_t i) const { return m_Faces[i]; }

private:

	std::vector<Vec3f> m_Vertices;
	std::vector<Vec3i> m_Faces;
};