#include "MeshLoader.h"
#include "Mesh.h"

MeshLoader::MeshLoader(const char* filePath, Mesh* mesh)
{
    std::ifstream file;
    file.open(filePath);
    if (!file.fail())
    {
        ReadFile(file, mesh);
        NormalizeObject(mesh);
    }
    else
    {
        std::cout << "There was a problem with opening the file";
    }
}

void MeshLoader::ReadFile(std::ifstream& file, Mesh* mesh)
{
    std::string line;
    while (std::getline(file, line))
    {
        FillVertexInfo(line, mesh);
    }
    return;
}

void MeshLoader::FillVertexInfo(std::string line, Mesh* mesh)
{
    if (line[0] == 'v')
    {
        line = line.substr(2, line.length());
        SplitLine(line, 'v', mesh);
    }
    else if (line[0] == 'f')
    {
        line = line.substr(2, line.length());
        SplitLine(line, 'f', mesh);
    }
    return;
}

void MeshLoader::SplitLine(std::string line, char state, Mesh* mesh)
{
    std::stringstream lineStream(line);
    std::string word;

    if (state == 'v')
    {
        Vertex v;
        glm::vec3 vec3(1.0f, 1.0f, 1.0f);
        int pos = 0;
        while (std::getline(lineStream, word, ' '))
        {
            float coordinate = std::stof(word);
            switch (pos)
            {
            case 0:
                vec3.x = coordinate;
                if (coordinate < minExtremes[0])
                {
                    minExtremes[0] = coordinate;
                }
                else if (coordinate > maxExtremes[0])
                {
                    maxExtremes[0] = coordinate;
                }
                break;
            case 1:
                vec3.y = coordinate;
                if (coordinate < minExtremes[1])
                {
                    minExtremes[1] = coordinate;
                }
                else if (coordinate > maxExtremes[1])
                {
                    maxExtremes[1] = coordinate;
                }
                break;
            case 2:
                vec3.z = coordinate;
                if (coordinate < minExtremes[2])
                {
                    minExtremes[2] = coordinate;
                }
                else if (coordinate > maxExtremes[2])
                {
                    maxExtremes[2] = coordinate;
                }
                break;
            default:
                break;
            }
            pos++;
        }
        v.position = vec3;
        mesh->vertices.push_back(v);
        return;
    }
    else if (state == 'f')
    {
        while (std::getline(lineStream, word, ' '))
        {
            unsigned int index = std::stoul(word) - 1;
            mesh->indices.push_back(index);
        }
        return;
    }
    return;
}

void MeshLoader::NormalizeObject(Mesh* mesh)
{
    float xmin = minExtremes[0];
    float ymin = minExtremes[1];
    float zmin = minExtremes[2];
    float xmax = maxExtremes[0];
    float ymax = maxExtremes[1];
    float zmax = maxExtremes[2];
    float firstComparison = std::max(1 / (xmax - xmin), 1 / (ymax - ymin));
    this->scalingFactor = std::max(firstComparison, 1 / (zmax - zmin));
}