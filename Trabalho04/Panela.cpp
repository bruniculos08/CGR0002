#include <iostream>
#include <vector>

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
        bool StartWith(std::string line, const char* text);
        void AddVertexData(int v1, int v2, int v3, int v4);
};