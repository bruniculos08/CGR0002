#include "OBJModel.h"
#include <iostream> 
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

OBJModel::OBJModel(){

}

OBJModel::~OBJModel(){
    
}

void OBJModel::LoadFromFile(const char* fileName){
    std::vector<Position> vertices;    
    std::vector<Texture> textures;    
    std::vector<Normal> normals;

    std::ifstream file(fileName);
    if(file){
        char currentMtlName[100];
        std::string line;
        while(std::getline(file, line)){
            if(StartWith(line, "mtllib")){
                char mtlFileName[100];
                (void)sscanf(line.c_str(), "mtllib %s", mtlFileName, sizeof(mtlFileName));
                LoadMaterialFile(mtlFileName);
            }
            if(StartWith(line, "v")){
                Position pos;
                sscanf(line.c_str(), "v %f %f %f", &pos.x, &pos.y, &pos.z);
                vertices.push_back(pos);
            }
            if(StartWith(line, "vt")){
                Texture tex;
                sscanf(line.c_str(), "vt %f %f", &tex.x, &tex.y);
                textures.push_back(tex);
            }
            if(StartWith(line, "vn")){
                Normal n;
                sscanf(line.c_str(), "vn %f %f %f", &n.x, &n.y, &n.z);
                normals.push_back(n);
            }
            if(StartWith(line, "usemtl")){
                (void)sscanf(line.c_str(), "usemtl %s", currentMtlName, sizeof(currentMtlName));
            }
            if(StartWith(line, "f")){
                int v1, v2, v3;
                int t1, t2, t3;
                int n1, n2, n3;
                // formato da linha de face f v/vt/vn:
                (void)sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", 
                            &v1, &t1, &n1, &v2, &t2, &n2, &v3, &t3, &n3);

                AddVertexData(v1, t1, n1, currentMtlName, vertices, textures, normals);
                AddVertexData(v2, t2, n2, currentMtlName, vertices, textures, normals);
                AddVertexData(v3, t3, n3, currentMtlName, vertices, textures, normals);
            }
        }
    }
    else{
        std::cout << "OBJ file loading failed" << std::endl;
    } 
}

// Como Vertice é um retorno da função é vértice está definido dentro da classe OBJModel deve-se...
// ... explicitar isso (o que não seria nessário caso houvesse Vertice em argumento da função...
// ... visto que a função é de um item da classe OBJModel);
std::vector<Vertice> OBJModel::GetVertexData(){
    return mVertexData;
}

int OBJModel::GetVertexCount(){
    return mVertexData.size();
}

void OBJModel::LoadMaterialFile(const char* fileName){
    // Gambiarra temporária
    std::cout << "material fileName: " << fileName << std::endl;
    std::ifstream file("Panela.mtl");
    if(file){
        std::string line;
        while(std::getline(file, line)){
            char mtlName[100];
            if(StartWith(line, "newmtl")){
                // A função "sscanf_s" é da Microsoft e a sua função equivalente é a "sscanf":
                (void)sscanf(line.c_str(), "newmtl %s", mtlName, sizeof(mtlName));
                mMaterialMap[mtlName] = Color();
            }
            if(StartWith(line, "Kd")){
                // A função "sscanf_s" é da Microsoft e a sua função equivalente é a "sscanf":
                Color& color = mMaterialMap[mtlName];
                sscanf(line.c_str(), "Kd %f %f %f", &color.r, &color.g, &color.b);
            }
        }
    }
    else{
        std::cout << "Material file loading failed" << std::endl;
    }
}

bool OBJModel::StartWith(std::string& line, const char* text){
    size_t textLen = strlen(text);
    if(line.size() < textLen){
        return false;
    }
    for(size_t i = 0; i < textLen; i++){
        if(line[i] == text[i]) continue;
        else return false;
    }
    return true;
}

void OBJModel::AddVertexData(int vIndex, int tIndex, int nIndex,
                            const char *mtl,
                            std::vector<Position>& vertices, std::vector<Texture>& textures,
                            std::vector<Normal>& normals){

    Color c = mMaterialMap[mtl];
    Position p = vertices[vIndex - 1];
    Texture t = textures[tIndex - 1];
    Normal n = normals[nIndex - 1];

    Vertice newVertice;
    newVertice.c = c;
    newVertice.p = p;
    newVertice.t = t;
    newVertice.n = n;
    mVertexData.push_back(newVertice);

    // mVertexData.push_back(p.x);
    // mVertexData.push_back(p.y);
    // mVertexData.push_back(p.z);
    // mVertexData.push_back(t.x);
    // mVertexData.push_back(t.y);
    // mVertexData.push_back(c.r);
    // mVertexData.push_back(c.g);
    // mVertexData.push_back(c.b);
    // mVertexData.push_back(n.x);
    // mVertexData.push_back(n.y);
    // mVertexData.push_back(n.z);


}