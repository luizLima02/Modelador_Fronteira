#pragma once

#include<glad/glad.h>
#include<half_edge.h>


typedef struct Ponto{
    float px;
    float py;
    float pz;
};

class Mesh
{
private:
    std::vector<Ponto> pontos;
    int qntDeVertices;
    //buffers
    GLuint VAO;
    GLuint VBO;
    void initBuffers();
public:
    Mesh(std::vector<glm::vec3> vertices, int qntDeVertices);
    ~Mesh();
    void render();
};

Mesh::Mesh(std::vector<glm::vec3> vertices, int qntDeVertices)
{
    for(int i = 0; i < vertices.size(); i++){
        Ponto ponto = {vertices[i].x, vertices[i].y, vertices[i].z};
        this->pontos.push_back(ponto);
    }
    this->qntDeVertices = qntDeVertices;
}

Mesh::~Mesh()
{
}

void Mesh::initBuffers()
{
}

void Mesh::render()
{
    std::cout << "Renderizando\n";
    for(int i = 0; i < qntDeVertices; i++){
        std::cout << pontos[i].px
                  << " " 
                  << pontos[i].py
                  << " " 
                  << pontos[i].pz
                  << "\n"; 
    }
}
