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

class Mesh_Aresta
{
private:
    glm::mat4 transform;
    Point* pontos;
    int qntDeVertices = 2;
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

    Mesh_Aresta(Point pi, Point pf){
       this->pontos = new(std::nothrow)Point[2];
       this->pontos[1] = pf;
       this->pontos[0] = pi;
       glm::mat4 transform = glm::mat4(1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1, 0, 0, 0, 0,  1);
       this->transform = transform;
       initBuffers();
    }

    Mesh_Aresta(glm::vec3 pi, glm::vec3 pf){
       this->pontos = new(std::nothrow)Point[2];
       this->pontos[1] = Point(pf);
       this->pontos[0] = Point(pi);
       glm::mat4 transform = glm::mat4(1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1, 0, 0, 0, 0,  1);
       this->transform = transform;
       initBuffers();
    }

    Mesh_Aresta(glm::vec4 pi, glm::vec4 pf){
       this->pontos = new(std::nothrow)Point[2];
       this->pontos[1] = Point(pf);
       this->pontos[0] = Point(pi);
       glm::mat4 transform = glm::mat4(1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1, 0, 0, 0, 0,  1);
       this->transform = transform;
       initBuffers();
    }

    ~Mesh_Aresta(){
        glDeleteVertexArrays(1, &this->VAO);
        glDeleteBuffers(1, &this->VBO);
    }
    
    void render(int k){
        std::cout << "( "
                  << this->pontos[0].px
                  << " "
                  << this->pontos[0].py
                  << " "
                  << this->pontos[0].pz
                  << " )-------------( "
                  << this->pontos[1].px
                  << " "
                  << this->pontos[1].py
                  << " "
                  << this->pontos[1].pz
                  << " )\n";
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
        std::vector<Mesh_Aresta*> arests;
    public:

    Modelo(Solid *s){
        Face      *f;
        Loop      *l;
        Half_edge *half;
        f = s->s_faces;
        while(f){
            std::cout << "Face " << f->face_num << ":\n";
            l = f->f_loops;
            while(l){
                std::cout << "Loop:\n";
                half = l->l_edg;
                do{
                    Mesh_Aresta* ar = new Mesh_Aresta(half->vtx->vcood, half->nextH->vtx->vcood);
                    this->arests.push_back(ar);
                }while((half = half->nextH) != l->l_edg);
                l = l->nextL;
            }
            f = f->nextF;
        }
    }

    void Render(Shader* shader){
        for(auto& i : this->arests){
            i->render(shader);
        }
    }

    ~Modelo(){
        
    }
};
