#pragma once

#include<string>
#include<sstream>
#include<vector>

class OBJLoader
{
    public:
        OBJLoader();
        ~OBJLoader();

        static void load_file(const std::string& filename, std::vector<float>& vertices, std::vector<float>& uvs, std::vector<float>& normals, std::vector<int>& indices);
    private:
        static std::vector<std::string> split(const std::string& s, char delim);
        static void store_float_in_vector(std::vector<float>& v, std::vector<std::string>& data);
};
