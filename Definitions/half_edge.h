#ifndef HALF_EDGE
#define HALF_EDGE

#include<glm/vec3.hpp>
#include<glm/vec4.hpp>
#include<stdlib.h>
#include<new>

using std::nothrow;

typedef short Id;

typedef struct solid Solid;
typedef struct face Face;
typedef struct loop Loop;
typedef struct half_edge Half_edge;
typedef struct vertex Vertex;
typedef struct edge Edge;

struct solid{
    Id solid_num;
    Face *s_faces;
    Edge *s_edges;
    Vertex *s_vertex;
    Solid *nextS, *prevS;
};

struct face{
    Id face_num;
    Solid *f_solid;
    Loop *fl_out; //loop externo
    Loop *f_loops; //todos os loops
    //Vector *feq ???
    Face *nextF, *prevF;
};

struct loop{
    Half_edge *l_edg;
    Face *l_face;
    Loop *nextL, *prevL;
};

struct half_edge{
    Edge *edg;
    Vertex *vtx;
    Loop *w_loop;
    Loop *f_loops;
    Half_edge *nextH, *prevH;
};

struct edge{
    Half_edge *he1, *he2;
    Edge *nextE, *prevE;
};


struct vertex{
    Id vertex_num;
    Half_edge *v_edge;
    glm::vec4 vcood;
    Vertex *nextV, *prevV;
};

void printSolidos(Solid *s){
    Solid* iteradorS = s;
    std::cout << "inicio para o fim:" << "\n";
    int k = 0;
    while(iteradorS){
        std::cout << "Solid id:" << iteradorS->solid_num << "\n";
        Face* iteradorF = iteradorS->s_faces;
        while(iteradorF){
            std::cout << "Face id:" << iteradorF->face_num << "\n";
            Loop *iteradorL = iteradorF->f_loops;
            while (iteradorL)
            {
                std::cout << "Loop:" << "\n";
                Half_edge* iteradorH = iteradorL->l_edg;
                do{
                    std::cout 
                    << "vertex id:"
                    << iteradorH->vtx->vertex_num 
                    << ": ( " 
                    << iteradorH->vtx->vcood.x
                    << " " 
                    << iteradorH->vtx->vcood.y
                    << " " 
                    << iteradorH->vtx->vcood.z
                    << " )";
                }while((iteradorH = iteradorH->nextH) != iteradorL->l_edg);
                std::cout << "\n";
                iteradorL = iteradorL->nextL;
            }
            iteradorF = iteradorF->nextF;
        }
        iteradorS = iteradorS->nextS;
    }
}

//criacao de halfEdge
Half_edge *add_halfedge(Edge *e, Vertex *v, Half_edge *where, int sign){
    Half_edge* half;

    if(where == nullptr){
        half = new(nothrow) Half_edge;
    }else if(where->edg == nullptr){
        half = where;
    }else{
        half = new(nothrow) Half_edge;
        where->prevH->nextH = half;
        half->prevH = where->prevH;
        where->prevH = half;
        half->nextH = where;
    }

    half->edg = e;
    half->vtx = v;
    if(where != nullptr){
        half->w_loop = where->w_loop;
    }else{
        half->w_loop = nullptr;
    }
    
    if(sign >= 0){
        e->he1 = half;
    }else{
        e->he2 = half;
    }

    return half;
}

Half_edge *del_halfedge(Half_edge *half){
    if(half == nullptr ){
        return nullptr;

    }else if(half->edg == nullptr){

        delete half;
        return nullptr;
    }else if(half->nextH == half){

        half->edg = nullptr;
        return half;

    }else{
        half->prevH->nextH = half->nextH;
        half->nextH->prevH = half->prevH;
        Half_edge* halfP = half->prevH;
        delete half;
        return halfP;
    }
}

//criar solido no fim da lista encadeada
Solid *make_solido(Id s, Solid *ant){ //ok
    //aloca um novo solido
    Solid *nS = new(nothrow)Solid;
    //inicia os valores do solido
    nS->solid_num = s;
    nS->nextS = nullptr;
    nS->prevS = ant;
    if(ant != nullptr){
        if(ant->nextS == nullptr){ // o solido passado esta no final da lista encadeada
            ant->nextS = nS;
        }else{ // o solido passado nao esta no final da lista encadeada
            nS->nextS = ant->nextS;
            nS->nextS->prevS = nS;
            ant->nextS = nS;
        }
    }
    nS->s_edges  = nullptr;
    nS->s_faces  = nullptr;
    nS->s_vertex = nullptr;
    return nS;
}

Face *make_face(Id f, Solid *solid){
    Face *nF = new(nothrow)Face;
    nF->face_num = f;
    nF->nextF = nullptr;
    nF->prevF = nullptr;
    //setando loops da face
    nF->f_loops = nullptr;
    nF->fl_out = nullptr;
    nF->f_solid = solid;
    if(solid != nullptr){
        if(solid->s_faces == nullptr){
            solid->s_faces = nF;
        }else{
            Face* itFaces = solid->s_faces;
            while(itFaces->nextF != nullptr){
                itFaces = itFaces->nextF;
            }
            itFaces->nextF = nF;
            nF->prevF = itFaces;
        }
    }
    return nF;
}

Loop *make_loop(Face *face){
    Loop *nL = new(nothrow)Loop;
    nL->nextL = nullptr;
    nL->prevL = nullptr;
    nL->l_face = face;

    if(face != nullptr){
        if(face->f_loops == nullptr){
            face->f_loops = nL;
        }else{
            Loop* itLoops = face->f_loops;
            while(itLoops->nextL != nullptr){
                itLoops = itLoops->nextL;
            }
            itLoops->nextL = nL;
            nL->prevL = itLoops;
        }
    }
    nL->l_edg = nullptr;
    return nL;
}

Vertex *make_vertex(Id v, Solid *solid, glm::vec4 coords){
    Vertex* nV = new(nothrow)Vertex;
    nV->nextV = nullptr;
    nV->prevV = nullptr;
    nV->vcood = coords;
    nV->vertex_num = v;
    if(solid != nullptr){
        if(solid->s_vertex == nullptr){
            solid->s_vertex = nV;
        }else{
            Vertex* itVertex = solid->s_vertex;
            while(itVertex->nextV != nullptr){
                itVertex = itVertex->nextV;
            }
            itVertex->nextV = nV;
            nV->prevV = itVertex;
        }
    }
    return nV;
}

Half_edge *make_halfedge(Half_edge *ant){
    Half_edge *half = new(nothrow)Half_edge;
    half->nextH = nullptr;
    half->prevH = ant;
    if(ant != nullptr){
        if(ant->nextH == nullptr){ // o solido passado esta no final da lista encadeada
            ant->nextH = half;
        }else{ // o solido passado nao esta no final da lista encadeada
            half->nextH        = ant->nextH;
            half->nextH->prevH = half;
            ant->nextH         = half;
        }
    }
    half->edg     = nullptr;
    half->vtx     = nullptr;
    half->f_loops = nullptr;
    half->w_loop  = nullptr;
    return half;
}

Edge *make_edge(Solid* solid){
    Edge *nE = new(nothrow)Edge;
    nE->nextE = nullptr;
    nE->prevE = nullptr;

    nE->he1 = nullptr;
    nE->he2 = nullptr;
    if(solid != nullptr){
        if(solid->s_edges == nullptr){
            solid->s_edges = nE;
        }else{
            Edge* itEdge = solid->s_edges;
            while(itEdge->nextE != nullptr){
                itEdge = itEdge->nextE;
            }
            itEdge->nextE = nE;
            nE->prevE = itEdge;
        }
    }
    return nE;
}

Solid *MVFS(Id s, Id f, Id v, glm::vec4 coords){
    Solid     *newSolid;
    Face      *newFace;
    Vertex    *newVertex;
    Half_edge *newHalf;
    Loop      *newLoop;
    
    //cria solido
    newSolid = make_solido(s, nullptr);if(newSolid == nullptr){return nullptr;}
    //cria face ligando ao solido
    newFace = make_face(f, newSolid);if(newFace == nullptr){return nullptr;}
    //cria loop ligando a face
    newLoop = make_loop(newFace);if(newLoop == nullptr){return nullptr;}
    //cria vertex ligando ao solido
    newVertex = make_vertex(v, newSolid, coords);if(newVertex == nullptr){return nullptr;}
    //cria halfEdge
    newHalf = make_halfedge(nullptr);if(newHalf == nullptr){return nullptr;}

    //segunda parte
    newFace->fl_out = newLoop;
    newLoop->l_edg = newHalf;
    newHalf->w_loop = newLoop;
    newHalf->nextH = newHalf->prevH = newHalf;
    newHalf->vtx = newVertex;
    
    return newSolid;
}

#endif