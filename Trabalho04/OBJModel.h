#include <iostream>
#include <vector>
#include <map>
#include <string>

struct Position{
            float x, y, z;
        };
        struct Texture{
            float x, y;
        };
        struct Color{
            float r, g, b;
        };
        struct Normal
        {
            float x, y, z;
        };

        struct Vertice{
            Color c;
            Texture t;
            Position p;
            Normal n;
        };

class OBJModel{
    public:
        OBJModel();
        ~OBJModel();

        void LoadFromFile(const char* fileName);
        std::vector<Vertice> GetVertexData();
        int GetVertexCount();
    
    private:
        
        void LoadMaterialFile(const char* fileName);
        bool StartWith(std::string& line, const char* text);
        void AddVertexData(int vIndex, int tIndex, int nIndex,
                        const char *mt1,
                        std::vector<Position>& vertices, std::vector<Texture>& textures,
                        std::vector<Normal>& normals);
        
        std::map<std::string, Color> mMaterialMap;
        std::vector<Vertice> mVertexData;
        
};