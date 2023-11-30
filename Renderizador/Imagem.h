#pragma once
#include<glad/glad.h>
#include<string>
#define STB_IMAGE_IMPLEMENTATION
#include<stb_image.h>

using std::string;

enum IMG_FORMAT{
    PNG,
    JPG
};

class Imagem
{
private:
    unsigned char* img_data;
    int width;
    int height;
    int nComponent;
    IMG_FORMAT formato;
public:
    Imagem(string filename, IMG_FORMAT formato){
        if(formato == PNG){
            this->img_data = stbi_load(filename.c_str(), &this->width, &this->height, &this->nComponent, 4);
        }else if(formato == JPG){
            this->img_data = stbi_load(filename.c_str(), &this->width, &this->height, &this->nComponent, 3);
        }else{
            return;
        }
        if (this->img_data == NULL) {
            return;
        }
        this->formato = formato;
    }
    //getters
    int getWidth(){return this->width;}
    int getHeight(){return this->height;}
    int getNComp(){return this->nComponent;}
    IMG_FORMAT getFormat(){return this->formato;}

    unsigned char* getData(){return this->img_data;}

    ~Imagem(){stbi_image_free(this->img_data); };

};