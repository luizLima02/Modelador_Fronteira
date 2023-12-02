#pragma once

#include<glad/glad.h>
#include<half_edge.h>
//glm
#include<glm/vec3.hpp>
#include<glm/mat4x4.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<new>
//render
#include<Shader.h>

struct Point{
    float px;
    float py;
    float pz;

    Point(glm::vec3 pos)
    {
        px = pos.x;
        py = pos.y;
        pz = pos.z;
    }

     Point(glm::vec4 pos)
    {
        px = pos.x;
        py = pos.y;
        pz = pos.z;
    }

    Point(float px, float py, float pz):px(px), py(py), pz(pz)
    {
    }

    Point()
    {
    }
};

class Mesh
{
private:
    glm::mat4 transform;
    Point* pontos;
    int qntDeVertices;
    //buffers
    GLuint VAO;
    GLuint VBO;
    void initBuffers(){
        glCreateVertexArrays(1, &this->VAO);
        glBindVertexArray(this->VAO);

        glGenBuffers(1, &this->VBO);
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, this->qntDeVertices * sizeof(Point), this->pontos, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 
                              3,
                              GL_FLOAT,
                              false, 
                              3*sizeof(float), 
                              (void*)(0*sizeof(float)));
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    }
public:

    Mesh(std::vector<Point> points, int qntDePontos){
       this->pontos         = new(std::nothrow)Point[qntDePontos];
       this->qntDeVertices  = qntDePontos;
       for(int i = 0; i < qntDePontos; i++)
            this->pontos[i] = points[i];
       
       glm::mat4 transform = glm::mat4(1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1, 0, 0, 0, 0,  1);
       this->transform = transform;
       initBuffers();
    }

    Mesh(std::vector<glm::vec3> points, int qntDePontos){
       this->pontos         = new(std::nothrow)Point[qntDePontos];
       this->qntDeVertices  = qntDePontos;
       for(int i = 0; i < qntDePontos; i++)
            this->pontos[i] = Point(points[i]);
       
       glm::mat4 transform = glm::mat4(1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1, 0, 0, 0, 0,  1);
       this->transform = transform;
       initBuffers();
    }

    Mesh(std::vector<glm::vec4> points, int qntDePontos){
       this->pontos         = new(std::nothrow)Point[qntDePontos];
       this->qntDeVertices  = qntDePontos;
       for(int i = 0; i < qntDePontos; i++)
            this->pontos[i] = Point(points[i]);
       
       glm::mat4 transform = glm::mat4(1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1, 0, 0, 0, 0,  1);
       this->transform = transform;
       initBuffers();
    }

    ~Mesh(){
        glDeleteVertexArrays(1, &this->VAO);
        glDeleteBuffers(1, &this->VBO);
    }
    
    void render(int k){
        for(int i = 0; i < this->qntDeVertices; i++){
            std::cout << "( "
                  << this->pontos[i].px
                  << " "
                  << this->pontos[i].py
                  << " "
                  << this->pontos[i].pz
                  << " )\n";
        }
    }
    
    void render(Shader *shader){
        glBindVertexArray(this->VAO);
        //ativa shader
        shader->use();
        shader->setMat4("model", this->transform);
        //RENDER
		glDrawArrays(GL_LINE_STRIP,0, this->qntDeVertices);
        glBindVertexArray(0);
    }

};

class Modelo{
    private:
        std::vector<Mesh*> arests;
    public:

    Modelo(Solid *s){
        Face      *f;
        Loop      *l;
        Half_edge *half;
        f = s->s_faces;
        while(f){
            int numVertices = 0;
            std::vector<glm::vec4> faceAtual;
            //std::cout << "Face " << f->face_num << ":\n";
            l = f->f_loops;
            while(l){
                std::cout << "Loop:\n";
                half = l->l_edg;
                do{
                    numVertices++;
                    faceAtual.push_back(half->vtx->vcood);
                }while((half = half->nextH) != l->l_edg);
                l = l->nextL;
            }
            Mesh* ar = new Mesh(faceAtual, numVertices);
            this->arests.push_back(ar);
            f = f->nextF;
        }
    }

    void Render(Shader* shader){
        int meshs = 0;
        for(auto& i : this->arests)
            meshs++;
            //i->render(shader);
        
        std::cout << meshs << "\n";
    }

    ~Modelo(){
        
    }
};
