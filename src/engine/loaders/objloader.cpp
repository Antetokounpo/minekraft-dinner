#include "objloader.hpp"

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<vector>

OBJLoader::OBJLoader(){}

OBJLoader::~OBJLoader() {}

std::vector<std::string> OBJLoader::split(const std::string& s, char delim)
{
    std::vector<std::string> word_list;
    std::istringstream iss(s);
    std::string word;

    while(std::getline(iss, word, delim))
        word_list.push_back(word);

    return word_list;
}

void OBJLoader::store_float_in_vector(std::vector<float>& v, std::vector<std::string>& data)
{
    for(std::string& i: data)
        v.push_back(std::stof(i));
}

void OBJLoader::load_file(const std::string& filename, std::vector<float>& vertices, std::vector<float>& uvs, std::vector<float>& normals, std::vector<unsigned int>& indices)
{
    std::vector<float> temp_uvs;
    std::vector<float> temp_normals;
    std::vector<float> temp_vertices;

    std::ifstream myfile(filename);
    std::string line;
    if(myfile.is_open())
    {
        while(std::getline(myfile, line))
        {
            if(std::string("#os").find(line.front()) != std::string::npos) // Ignore lines which starts with #, o, s
                continue;

            std::vector<std::string> line_split = split(line, ' ');
            std::string data_type = line_split[0];
            line_split.erase(line_split.begin()); // Keep only numerical data

            if(data_type == "v")
            {
                store_float_in_vector(temp_vertices, line_split);
            }
            else if(data_type == "vt")
            {
                store_float_in_vector(temp_uvs, line_split);
            }
            else if(data_type == "vn")
            {
                store_float_in_vector(temp_normals, line_split);
            }

            if(data_type == "f")
            {
                for(std::string& vertex: line_split)
                {
                    std::vector<std::string> data_split = split(vertex, '/');
                    int v = std::stoi(data_split[0]);
                    int vt = std::stoi(data_split[1]);
                    int vn = std::stoi(data_split[2]);

                    for(int i = 0; i<3; i++)
                        vertices.push_back(temp_vertices[(v-1)*3+i]);
                    uvs.push_back(temp_uvs[(vt-1)*2]); // Blender ¯\_(ツ)_/¯
                    uvs.push_back(1.0f - temp_uvs[(vt-1)*2+1]);
                    for(int i = 0; i<3; i++)
                        normals.push_back(temp_normals[(vn-1)*3+i]);
                }
            }
        }
        myfile.close();

        for(unsigned int i = 0; i<vertices.size()/3; i++)
            indices.push_back(i);
    }
    else
        std::cout << "Can't open: " << filename << std::endl;
}
