#include <iostream>
#include "SDLtemplate.hpp"

// Compile: g++ main.cpp -lSDL2 -o main && ./main

int main(void){
    // (1) Definindo tamanho e altura da imagem:
    int width = 800;
    int height = 400;
    // (2) Printando o cabeçalho do arquivo da imagem (este output será obtido pelo código que...
    // ... irá gerar a imagem):
    std::cout << "P3\n" << width << " " << height <<  "\n255\n";
    sdltemplate::sdl("Ray Tracer", width, height);
    // (3) Loop para andar pelos pixels da imagem:
    for(int y = height - 1; y >= 0; y--){
        for(int x = 0; x < width; x++){
            float r = float(x) / float(width);
            float g = float(y) / float(height);
            float b = 0.2;
            int ir = int(255.99*r);
            int ig = int(255.99*g);
            int ib = int(255.99*b);
            std::cout << ir << " " << ig << " " << ib << "\n";
            sdltemplate::setDrawColor(sdltemplate::createColor(ir, ig, ib, 255));
            sdltemplate::drawPoint(x, height-y);
        }
    }
    while(sdltemplate::running){
        sdltemplate::loop();
    }
    
}