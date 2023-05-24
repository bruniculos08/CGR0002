#include <iostream>
#include <vector>
#include <map>
#include <string>

class OBJModel{
    public:
        OBJModel();
        ~OBJModel();
    
        void LoadFromFile(const char* fileName);
        std::vector<float> GetVertexData();
        int GetVertexCount();

    private:
        struct Position{
            float x, y, z;
        };
        struct Color{
            float r, g, b;
        };
        struct Normal
        {
            float x, y, z;
        };
        
        void LoadMaterialFile(const char* fileName);
        bool StartWith(std::string& line, const char* text);
        void AddVertexData(int v1, int v2, int v3, int v4,
            const char* mtl, int n1, int n2, int n3, int n4,
            std::vector<Position>& vertices, std::vector<Normal>& normals);
        
        std::map<std::string, Color> mMaterialMap;
        std::vector<float> mVertexData;
        
};