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
            if(StartWith(line, "vn")){
                Normal n;
                sscanf(line.c_str(), "vn %f %f %f", &n.x, &n.y, &n.z);
                normals.push_back(n);
            }
            if(StartWith(line, "usemtl")){
                (void)sscanf(line.c_str(), "usemtl %s", currentMtlName, sizeof(currentMtlName));
            }
            if(StartWith(line, "f")){
                int v1, v2, v3, v4;
                int t1, t2, t3, t4;
                int n1, n2, n3, n4;
                (void)sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &v1, )
            }
        }
    }
    else{
        std::cout << "OBJ file loading failed" << std::endl;
    } 
}

std::vector<float> OBJModel::GetVertexData(){

}

int OBJModel::GetVertexCount(){

}

void OBJModel::LoadMaterialFile(const char* fileName){
    std::ifstream file(fileName);
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

void OBJModel::AddVertexData(int v1, int v2, int v3, int v4,
                            const char *mt1, int n1, int n2, int n3, int n4,
                            std::vector<Position>& vertices, std::vector<Normal>& normals){

}