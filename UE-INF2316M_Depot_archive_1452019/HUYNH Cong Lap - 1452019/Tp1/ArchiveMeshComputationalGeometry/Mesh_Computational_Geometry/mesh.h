#ifndef MESH_H
#define MESH_H

#include <QGLWidget>
#include <QMap>
#include <QString>
#include <QVector>
#include <QTextStream>
#include <QFile>
#include <iterator>
#include <iostream>
// TO MODIFY
// Advice => You should create a new file for each module but not necessarily for each class
class Vertex
{

    // Class to be enriched, otherwise it would have been simply named as 3DPoint!!
    double _x;
    double _y;
    double _z;


public:
    Vertex():_x(),_y(),_z() {}
    Vertex(float x_, float y_, float z_):_x(x_),_y(y_),_z(z_) {}
    // get
    double x() const { return _x; }
    double y() const { return _y; }
    double z() const { return _z; }
};


//** TO MODIFY
class Triangle
{
    Vertex _a;
    Vertex _b;
    Vertex _c;



public:
    Triangle():_a(),_b(),_c() {}
    Triangle(Vertex a_, Vertex b_, Vertex c_):_a(a_),_b(b_),_c(c_) {}
    // get
    Vertex a() const { return _a; }
    Vertex b() const { return _b; }
    Vertex c() const { return _c; }
};

// New class for Vertex and Triangle -> PVertex, Triangle
class PVertex // un sommet
{

    int _vtriangle_index; // keep a index face of this vertex
    double _x;
    double _y;
    double _z;


public:
    PVertex():_x(),_y(),_z(), _vtriangle_index(-1) {}
    PVertex(float x_, float y_, float z_):_x(x_),_y(y_),_z(z_), _vtriangle_index(-1) {}
    // get
    double x() const { return _x; }
    double y() const { return _y; }
    double z() const { return _z; }

    void setFace(int faceIndex){ _vtriangle_index = faceIndex; }
    int getFace() {return _vtriangle_index; } // return the face index that this vertex belongs to
};


class PTriangle
{

    int _vertex1Index, _vertex2Index, _vertex3Index;
    int _triangle1Index, _triangle2Index,_triangle3Index;


public:
    PTriangle():_vertex1Index(),_vertex2Index(),_vertex3Index(),_triangle1Index(-1),_triangle2Index(-1),_triangle3Index(-1){}

    PTriangle(int a, int b, int c){
        _vertex1Index = a;
        _vertex2Index = b;
        _vertex3Index = c;

        _triangle1Index = -1;
        _triangle2Index = -1;
        _triangle3Index = -1;
    }
    // get
    int a() const { return  _vertex1Index; }
    int b() const { return  _vertex2Index; }
    int c() const { return  _vertex3Index; }


    int voisin1() const { return  _triangle1Index; }
    int voisin2() const { return  _triangle2Index; }
    int voisin3() const { return  _triangle3Index; }

    bool testVertex(int i) // check if this face has this vertex inside
    {
        return (_vertex1Index == i || _vertex2Index==i || _vertex3Index==i);
    }

    void showVoisin()
    {
        std::cout<<"Voisin: "<< _triangle1Index << " and " << _triangle2Index << " and " << _triangle3Index << std::endl;
    }


    void attachedTriangle(int a, int b, int c) // index attached triangles
    {
        _triangle1Index = a;
        _triangle2Index = b;
        _triangle3Index = c;
    }

    void setVoisin(int attached) // index attached triangles
    {
        if (_triangle1Index == -1)
            _triangle1Index = attached;
        else
            if (_triangle2Index == -1)
                _triangle2Index = attached;
            else
                if (_triangle3Index == -1)
                    _triangle3Index = attached;

    }
};



//** TO MODIFY
class Mesh
{
    int _vertex_number;
    int _face_number;
    int _edge_number;

    QVector<PVertex> _vertexArray;
    QVector<PTriangle> _facesArray;
public:

    Mesh();

    PTriangle* getTriangle(int i){ return &_facesArray[i];}


    // The two following functions could be displaced into a module OpenGLDisplayMesh that would include Mesh
    void drawTetraedra();
    void drawTetraedraWireFrame();
    void readMesh(QString fileName);

    void drawTetrahedron(); // draw a Tetrahedron in new structure
    void drawTetrahedronWireFrame(); // draw a Wire Frame Tetrahedron in new Structure


    void draw(); // Draw the mesh
    void drawWire(); // Draw the mesh in WireFrame
    void init();

    bool wireFrameMode = false;


};

#endif // MESH_H
