#pragma once

#ifndef HALF_EDGE
#define HALF_EDGE

//includes libs
#include<glm/vec3.hpp>
#include<glm/vec4.hpp>

//includes c++
#include<vector>
#include<stdlib.h>
#include<math.h>
#include <iostream>
#include <utility>
#include<new>

using std::nothrow;

typedef short Id;

typedef struct solid Solid;
typedef struct face Face;
typedef struct loop Loop;
typedef struct half_edge Half_edge;
typedef struct vertex Vertex;
typedef struct edge Edge;

//var globais
Solid* firstSolid = nullptr;


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
    glm::vec4 *feq;
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

union node
{
Solid s;
Face f;
Loop l;
Half_edge h;
Vertex v;
Edge e;
};


/* node type parameters for memory allocation routines */
#define SOLID       0
#define FACE        1
#define LOOP        2
#define HALFEDGE    3
#define EDGE        4
#define VERTEX      5

#define NIL 0
#define PI  3.141592653589793


/*macros*/
#define mate(he) (((he) == (he)->edg->he1) ? \
                  (he)->edg->he2 : (he)->edg->he1)

void ListSolidos(Solid *s)
{
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
                std::cout << half->vtx->vertex_num
                          << ": ("
                          << half->vtx->vcood.x
                          << " "
                          << half->vtx->vcood.y
                          << " "
                          << half->vtx->vcood.z
                          << ")\n";
            }while((half = half->nextH) != l->l_edg);
            std::cout << "\n";
            l = l->nextL;
        }
        f = f->nextF;
    }
}


//criar solido no fim da lista encadeada
unsigned nodesize[] = 
{
    sizeof(Solid), sizeof(Face), sizeof(Loop),
    sizeof(Half_edge), sizeof(Edge), sizeof(Vertex),0
};

void addlist(int what, node *which, node* where){
    switch (what)
    {
    case SOLID:
        which->s.nextS = firstSolid;
        which->s.prevS = (Solid *) NIL;
        if(firstSolid)
            firstSolid->prevS = (Solid *) which;
        firstSolid = (Solid *) which;
        break;

    case FACE:
        which->f.nextF = where->s.s_faces;
        which->f.prevF = (Face *) NIL;
        if(where->s.s_faces)
            where->s.s_faces->prevF = (Face *) which;
        where->s.s_faces = (Face *) which;
        which->f.f_solid = (Solid *) where;
        break;

    case LOOP:
        which->l.nextL = where->f.f_loops;
        which->l.prevL = (Loop *) NIL;
        if(where->f.f_loops)
            where->f.f_loops->prevL = (Loop *) which;
        where->f.f_loops = (Loop *) which;
        which->l.l_face = (Face *) where;
        break;

    case EDGE:
        which->e.nextE = where->s.s_edges;
        which->e.prevE = (Edge *) NIL;
        if(where->s.s_edges)
            where->s.s_edges->prevE = (Edge *) which;
        where->s.s_edges = (Edge *) which;
        break;

    case VERTEX:
        which->v.nextV = where->s.s_vertex;
        which->v.prevV = (Vertex *) NIL;
        if(where->s.s_vertex)
            where->s.s_vertex->prevV = (Vertex *) which;
        where->s.s_vertex = (Vertex *) which;
        break;
    
    default:
        break;
    }
}

node *newNode(int what, node *where){
    node* nN;
    nN = (node *)malloc(nodesize[what]);
    switch (what)
    {
    case SOLID:
        addlist(SOLID, nN, nullptr);
        nN->s.s_faces  = (Face *) NIL;
        nN->s.s_edges  = (Edge *) NIL;
        nN->s.s_vertex = (Vertex *) NIL;
        break;

    case FACE:
        addlist(FACE, nN, where);
        nN->f.f_loops = (Loop *) NIL;
        nN->f.fl_out  = (Loop *) NIL;
        break;

    case LOOP:
        addlist(LOOP, nN, where);
        break;

    case EDGE:
        addlist(EDGE, nN, where);
        break;

    case VERTEX:
        addlist(VERTEX, nN, where);
        nN->v.v_edge = (Half_edge * ) NIL;
        break;

    default:
        break;
    }
    return nN;
}

//criacao de halfEdge
Half_edge *add_halfedge(Edge *e, Vertex *v, Half_edge *where, int sign)
{
    Half_edge* half;

    if(where->edg == NIL){
        half = where;
    }else{
        half = (Half_edge*)newNode(HALFEDGE, NIL);
        where->prevH->nextH = half;
        half->prevH = where->prevH;
        where->prevH = half;
        half->nextH = where;
    }

    half->edg = e;
    half->vtx = v;
    half->w_loop = where->w_loop;
   
    
    if(sign >= 0){
        e->he1 = half;
    }else{
        e->he2 = half;
    }

    return half;
}



/*MAKE_VERTICE_FACE_SOLID: 
    Operator mvfs creates a new solid named sn that consists of one
    face f and one vertex v with coordinates z, y, and z. It returns
    a pointer to the new solid created. Note that the operator mvfs
    is the only one that has just one level of implementation: there
    are no separate low-level and high-level operators. 
*/
Solid *MVFS(Id s, Id f, Id v, glm::vec4 coords)
{
    Solid     *newSolid;
    Face      *newFace;
    Vertex    *newVertex;
    Half_edge *newHalf;
    Loop      *newLoop;
    
    //cria solido
    newSolid = (Solid *) newNode(SOLID, NIL);
    //cria face ligando ao solido
    newFace = (Face *) newNode(FACE, (node *)newSolid);
    //cria loop ligando a face
    newLoop = (Loop *) newNode(LOOP, (node *)newFace);
    //cria vertex ligando ao solido
    newVertex = (Vertex *) newNode(VERTEX, (node *)newSolid);
    //cria halfEdge
    newHalf = (Half_edge *) newNode(HALFEDGE, NIL);


    //segunda parte
    newSolid->solid_num = s;
    newFace->face_num = f;
    newFace->fl_out = newLoop;
    newLoop->l_edg = newHalf;
    newHalf->w_loop = newLoop;
    newHalf->nextH = newHalf->prevH = newHalf;
    newHalf->vtx = newVertex;
    newHalf->edg = NIL;

    newVertex->vertex_num = v;
    newVertex->vcood = coords;
    newVertex->v_edge = newHalf;
    
    return newSolid;
}

/*Low_level_Make_Edge_Vertex:
    Operator lmev “splits” the vertex pointed at by hel and he2,
    and adds a new edge between the resulting two vertices. The
    identifier v and the coordinates z, y, and z are assigned to the
    new vertex. The case that hel and he@ are identical is allowed;
    in this case, the new vertex and edge are added into the face of
    he1. The new edge is oriented from the new vertex to the old one. 
*/
void LMEV(Half_edge *h1, Half_edge *h2, Id v, glm::vec4 coords)
{
    Vertex    *newVertex;
    Half_edge *half;
    Edge      *newEdge;

    newEdge =   (Edge*) newNode(EDGE, (node *)h1->w_loop->l_face->f_solid);
    newVertex = (Vertex*) newNode(VERTEX, (node *)h1->w_loop->l_face->f_solid);
    newVertex->vertex_num = v;
    newVertex->vcood = coords;

    half = h1;
    while(half != h2){
        half->vtx = newVertex;
        half = mate(half)->nextH;
    }
    
    add_halfedge(newEdge, h2->vtx,   h1, -1);
    add_halfedge(newEdge, newVertex, h2,  1);

    newVertex->v_edge = h2->prevH;
    h2->vtx->v_edge   = h2;
}

/*Low_Level_Make_Edge_Face:
Operator lmef adds a new edge between halfedges hel and he2,
and “splits” their common face into two faces such that he1 will
occur in the new face f, and he2 remains in the old face. The
new edge is oriented from he1->vtx to he2->vtx. Halfedges
he1 and he2 must belong to the same loop (i.e., he1->wloop== he2->wloop). 
They may be equal, in which case a “circular” face with just one edge is created. 
A pointer to the new face is returned. 
*/
Face *LMEF(Half_edge *he1, Half_edge *he2, Id f)
{
    //def variaveis
    Face        *newFace;
    Loop        *newLoop;
    Edge        *newEdge;
    Half_edge   *he, *nhe1, *nhe2, *temp;
    
    newFace = (Face *)  newNode(FACE, (node*)he1->w_loop->l_face->f_solid);
    newLoop = (Loop *)  newNode(LOOP, (node *)newFace);
    newEdge =  (Edge*)  newNode(EDGE, (node *)he1->w_loop->l_face->f_solid);

    newFace->face_num = f;
    newFace->fl_out   = newLoop;

    he = he1;
    while(he != he2){
        he->w_loop = newLoop;
        he = he->nextH;
    }
    nhe1 = add_halfedge(newEdge, he2->vtx, he1, -1);
    nhe2 = add_halfedge(newEdge, he1->vtx, he2,  1);

    nhe1->prevH->nextH = nhe2;
    nhe2->prevH->nextH = nhe1;
    temp = nhe1->prevH;
    nhe1->prevH = nhe2->prevH;
    nhe2->prevH = temp;

    newLoop->l_edg = nhe1;
    he2->w_loop->l_edg = nhe2;
    
    return newFace;  
}

//high level functions

//getters
Solid* getSolid(Id sn){
    Solid* iterador;

    //percorre a lista endadeada a apartir do solido fs para frente
    for(iterador = firstSolid; iterador != NIL; iterador = iterador->nextS){
        if(iterador->solid_num == sn){
            return iterador;
        }
    }

    return NIL;
}

Face* getFace(Solid* fs, Id fn){
    Face* iterador;
    //percorre a lista endadeada a apartir do solido fs para frente
    for(iterador = fs->s_faces; iterador != NIL; iterador = iterador->nextF){
        if(iterador->face_num == fn){
            return iterador;
        }
    }

    return NIL;
}

Half_edge* getHalfEdge(Face* f, Id vn1, Id vn2){
    Loop *itLoop;
    Half_edge *itHalf;
   // std::cout << "ENTROU HALF EDGE\n"; 
    for(itLoop = f->f_loops; itLoop != NIL; itLoop = itLoop->nextL){
       // std::cout << "ENTROU LOOP\n"; 
        itHalf = itLoop->l_edg;
        do{
            //std::cout << "Aresta: xi:" << itHalf->vtx->vertex_num << " xf:" << itHalf->nextH->vtx->vertex_num <<"\n";
            if(itHalf->vtx->vertex_num == vn1 && itHalf->nextH->vtx->vertex_num == vn2)
            {
               // std::cout << "RETURN HALF EDGE\n";
                return itHalf;
            }
        }while((itHalf = itHalf->nextH) != itLoop->l_edg);
       // std::cout << "SAIU LOOP\n"; 
    }
    //std::cout << "RETURN NIL\n";
    return NIL;
}

/*LMEV de high level*/
int MEV(Id s, Id f1, Id f2, Id v1, Id v2, Id v3, Id v4, glm::vec4 coords)
{
    Solid *oldSolid;
    Face  *oldFace1, *oldFace2;
    Half_edge *he1, *he2;

    if((oldSolid = getSolid(s)) == NIL)
    {
        std::cout << "MEV: solido " << s << "not found\n";
        return -1;
    }
    if((oldFace1 = getFace(oldSolid, f1)) == NIL)
    {
        std::cout << "MEV: face " << f1 << " not found in solido " << s << "\n";
        return -1;
    }
    if((oldFace2 = getFace(oldSolid, f2)) == NIL)
    {
        std::cout << "MEV: face " << f2 << " not found in solido " << s << "\n";
        return -1;
    }
    if((he1 = getHalfEdge(oldFace1, v1, v2)) == NIL)
    {
        std::cout << "MEV: edge " << v1 << "-" << v2 << " not found in face " << f1 << "\n";
        return -1;
    }
    if((he2 = getHalfEdge(oldFace2, v1, v3)) == NIL)
    {
        std::cout << "MEV: edge " << v1 << "-" << v3 << " not found in face " << f2 << "\n";
        return -1;
    }
    LMEV(he1, he2, v4, coords);
    return 0;
}

/*Special MEV that "DRAW A LINE"*/
int SMEV(Id s, Id f1, Id v1, Id v4, glm::vec4 coords){
    //return  MEV(s, f1, f1, v1, v2, v2, v4, coords);
    Solid *oldSolid;
    Face  *oldFace1;
    Half_edge *he1;

    if((oldSolid = getSolid(s)) == NIL)
    {
        std::cout << "SMEV: solido " << s << "not found\n";
        return -1;
    }
    if((oldFace1 = getFace(oldSolid, f1)) == NIL)
    {
        std::cout << "SMEV: face " << f1 << " not found in solido " << s << "\n";
        return -1;
    }
    if((he1 = getHalfEdge(oldFace1, v1, v1)) != NIL)
    {
        
    }else if((he1 = getHalfEdge(oldFace1, v1, v1+1)) != NIL){

    }else if((he1 = getHalfEdge(oldFace1, v1, v1-1)) != NIL){

    }else{
        std::cout << "SMEV: edge " << v1 << " not found in face " << f1 << "\n";
        return -1;
    }
    LMEV(he1, he1, v4, coords);
    return 0;
}

/*LMEF de high level*/
int MEF(Id s, Id f1, Id v1, Id v2, Id v3, Id v4, Id f2){
    Solid *oldSolid;
    Face  *oldFace;
    Half_edge *he1, *he2;

    if((oldSolid = getSolid(s)) == NIL)
    {
        std::cout << "MEF: solido " << s << "not found\n";
        return -1;
    }
    if((oldFace = getFace(oldSolid, f1)) == NIL)
    {
        std::cout << "MEF: face " << f1 << " not found in solido " << s << "\n";
        return -1;
    }
    if((he1 = getHalfEdge(oldFace, v1, v2)) == NIL)
    {
        std::cout << "MEF: edge " << v1 << "-" << v2 << " not found in face " << f1 << "\n";
        return -1;
    }
    if((he2 = getHalfEdge(oldFace, v3, v4)) == NIL)
    {
        std::cout << "MEF: edge " << v3 << "-" << v4 << " not found in face " << f1 << "\n";
        return -1;
    }
    LMEF(he1, he2, f2);
    return 0;
}

int SMEF(Id s, Id f1, Id v1, Id v3, Id f2){
    Solid *oldSolid;
    Face  *oldFace;
    Half_edge *he1, *he2;

    if((oldSolid = getSolid(s)) == NIL)
    {
        std::cout << "SMEF: solido " << s << "not found\n";
        return -1;
    }
    if((oldFace = getFace(oldSolid, f1)) == NIL)
    {
        std::cout << "SMEF: face " << f1 << " not found in solido " << s << "\n";
        return -1;
    }
    //verifica os vertices adijacentes a v1
    if((he1 = getHalfEdge(oldFace, v1, v1)) != NIL)
    {
        
    }else if((he1 = getHalfEdge(oldFace, v1, v1+1)) != NIL)
    {
        
    }else if((he1 = getHalfEdge(oldFace, v1, v1-1)) != NIL)
    {
        
    }else{
        std::cout << "SMEF: edge " << v1 << "-" << v1 << " not found in face " << f1 << "\n";
        std::cout << "SMEF: edge " << v1 << "-" << v1+1 << " not found in face " << f1 << "\n";
        std::cout << "SMEF: edge " << v1 << "-" << v1-1 << " not found in face " << f1 << "\n";
        return -1;
    }

    //verifica os vertices adijacentes a v1
    if((he2 = getHalfEdge(oldFace, v3, v3)) != NIL)
    {
        
    }else if((he2 = getHalfEdge(oldFace, v3, v3+1)) != NIL)
    {
        
    }else if((he2 = getHalfEdge(oldFace, v3, v3-1)) != NIL)
    {
        
    }else{
        std::cout << "SMEF: edge " << v3 << "-" << v3 << " not found in face " << f1 << "\n";
        std::cout << "SMEF: edge " << v3 << "-" << v3+1 << " not found in face " << f1 << "\n";
        std::cout << "SMEF: edge " << v3 << "-" << v3-1 << " not found in face " << f1 << "\n";
        return -1;
    }
    LMEF(he1, he2, f2);
    return 0;
}

/*Criacao de Primitivas*/

/*
get max id of (vertice, face) of a solid with id sn or (0,0) if theres no solid,
if any of the return values are 0 the solid has no face/vertice.
*/
std::pair<Id,Id> getMaxNames(Id sn){
    Solid *s;
    Vertex *v;
    Face *f;
    Id maxVertex = 0;
    Id maxFace   = 0;
    if(firstSolid == nullptr){return std::make_pair<Id,Id>(0,0);}
    s = getSolid(sn);
    for(v = s->s_vertex; v != NIL; v = v->nextV){
        if(maxVertex < v->vertex_num) maxVertex = v->vertex_num;
    }
    for(f = s->s_faces; f != NIL; f = f->nextF){
        if(maxFace < f->face_num) maxFace = f->face_num;
    }

    return std::make_pair(maxVertex, maxFace);
}

/*
Make an arc at (cx, cy), on plane z = h, with radius rad, with n edges,
the arc ranges from angle phi1 to phi2, measured in degrees, where 0.0 = x - axis,
and angle grow counterclockwise. the arc starts from existing vertex v on face f.
*/
void arc(Id s, Id f, Id v, float cx, float cy, float rad, float h, float phi1, float phi2, int n)
{
    float x, y, angle, inc;
    Id prev;
    int i;

    angle   = phi1 * PI / 180.0;
    inc     = (phi2 - phi1) * PI / (180.0 * n);
    prev    = v;
    std::pair max = getMaxNames(s);
    for (i = 0; i < n; i++)
    {
        angle += inc;
        x = cx + cos(angle) * rad; 
        y = cy + sin(angle) * rad;
        SMEV(s, f, prev, ++max.first, glm::vec4(x, y, h, 1));
        prev = max.first;
    }
    
}


Solid *circle(Id sn, float cx, float cy, float rad, float h, int n)
{
    Solid *s;

    s = MVFS(sn, 1, 1, glm::vec4(cx+rad, cy, h, 1));
    arc(sn, 1, 1, cx, cy, rad, h, 0.0, ((n-1)*360.0/n), n-1);
    SMEF(sn, 1, 1, n, 2);
    return s;
}

void sweep(Face* fac, glm::vec3 d)
{
    Loop        *l;
    Half_edge   *first, *scan;
    Vertex      *v;

    std::pair max = getMaxNames(fac->f_solid->solid_num);
    l = fac->f_loops;
    while(l){
        first = l->l_edg;
        scan = first->nextH;
        v = scan->vtx;
        LMEV(scan, scan, ++max.first, glm::vec4(
            v->vcood.x + d.x,
            v->vcood.y + d.y,
            v->vcood.z + d.z,
            1));
        while(scan != first)
        {
            v = scan->nextH->vtx;
            LMEV(scan->nextH, scan->nextH, ++max.first, glm::vec4(
            v->vcood.x + d.x,
            v->vcood.y + d.y,
            v->vcood.z + d.z,
            1));
            LMEF(scan->prevH, scan->nextH->nextH, ++max.second);
            scan = mate(scan->nextH)->nextH;
        }
        LMEF(scan->prevH, scan->nextH->nextH, ++max.second);
        l = l->nextL;
    }
}

//primitivas
Solid *block(Id sn, glm::vec3 d){
    Solid *s;

    s = MVFS(sn, 1, 1, glm::vec4(0.0, 0.0, 0.0, 1.0));
    SMEV(sn, 1,1,2, glm::vec4(d.x, 0.0, 0.0, 1.0));
    SMEV(sn, 1,2,3, glm::vec4(d.x, d.y, 0.0, 1.0));
    SMEV(sn, 1,3,4, glm::vec4(0.0, d.y, 0.0, 1.0));
    SMEF(sn, 1, 1, 4, 2);
    sweep(getFace(s, 2), glm::vec3( 0.0, 0.0, d.z));
    //std::pair max = getMaxNames(s->solid_num);
    //std::cout << "Vertex m:" << max.first << " Face m:" << max.second <<"\n";
    return s;
}




void terminar(){
   
}

#endif