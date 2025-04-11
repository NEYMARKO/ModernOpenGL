#include "MeshLoader.h"

MeshLoader::MeshLoader(const char* filePath)
{
    std::ifstream file;
    file.open(filePath);
    if (!file.fail())
    {
        std::vector<glm::vec3> normalVectors;
        ReadFile(file, normalVectors);
        CalculateScalingFactor();
    }
    else
    {
        std::cout << "There was a problem with opening the file";
    }
}

void MeshLoader::ReadFile(std::ifstream& file, std::vector<glm::vec3>& normalVectors)
{
    std::string line;
    while (std::getline(file, line))
    {
        SplitLine(line, normalVectors);
    }
    return;
}


void MeshLoader::SplitLine(std::string& line, std::vector<glm::vec3>& normalVectors)
{
    std::string state = line.substr(0, 2);
    if (state == "vn") line = line.substr(3, line.length());
    else line = line.substr(2, line.length());

    std::stringstream lineStream(line);
    std::string word;

    if (state == "v " || state == "vn")
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
                if (state == "v " && coordinate < minExtremes[0])
                {
                    minExtremes[0] = coordinate;
                }
                else if (state == "v " && coordinate > maxExtremes[0])
                {
                    maxExtremes[0] = coordinate;
                }
                break;
            case 1:
                vec3.y = coordinate;
                if (state == "v " && coordinate < minExtremes[1])
                {
                    minExtremes[1] = coordinate;
                }
                else if (state == "v " && coordinate > maxExtremes[1])
                {
                    maxExtremes[1] = coordinate;
                }
                break;
            case 2:
                vec3.z = coordinate;
                if (state == "v " && coordinate < minExtremes[2])
                {
                    minExtremes[2] = coordinate;
                }
                else if (state == "v " && coordinate > maxExtremes[2])
                {
                    maxExtremes[2] = coordinate;
                }
                break;
            default:
                break;
            }
            pos++;
        }
        if (state == "v ")
        {
            v.position = vec3;
            this->vertices.push_back(v);
        }
        else
        {
            normalVectors.push_back(vec3);
        }
        return;
    }
    else if (state == "f ")
    {
        while (std::getline(lineStream, word, ' '))
        {
            size_t delimiterPos = word.find("//");
            std::string indexString = word.substr(0, delimiterPos);
            std::string normalIndexString = word.substr(delimiterPos + 2, word.length());

            //std::cout << "INDEX STRING: " << indexString << "NORMAL INDEX: " << normalIndexString << std::endl;
            // std::cout << "WORD: " << word << std::endl;
            // std::cout << "Index: " << indexString << std::endl;
            // std::cout << "Normal index: " << normalIndexString << std::endl;
            // std::cout << std::endl;

            unsigned int index = std::stoul(indexString) - 1;
            unsigned int normalPos = std::stoul(normalIndexString) - 1;
            this->vertices[index].normal = normalVectors[normalPos];
            this->indices.push_back(index);
        }
        return;
    }
    return;
}

void MeshLoader::CalculateScalingFactor()
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

MeshLoader::~MeshLoader()
{
    //std::cout << "Deleted loader" << std::endl;
}