#ifndef MESH_H
#define MESH_H

#include <QGLWidget>
#include "QDebug"
#include "iostream"
using namespace std;
// TO MODIFY
// Advice => You should create a new file for each module but not necessarily for each class
class Vertex
{
    // Class to be enriched, otherwise it would have been simply named as 3DPoint!!
    int _vtriangle_index; // keep a index face of this vertex
    double _x;
    double _y;
    double _z;


public:
    Vertex():_x(),_y(),_z(), _vtriangle_index(-1) {}

    Vertex(float x_, float y_, float z_):_x(x_),_y(y_),_z(z_), _vtriangle_index(-1) {}
    Vertex(float x_, float y_, float z_, int index):_x(x_),_y(y_),_z(z_), _vtriangle_index(index) {}
    // get
    double x() const { return _x; }
    double y() const { return _y; }
    double z() const { return _z; }

    void setFace(int faceIndex){ _vtriangle_index = faceIndex; } // set one of the faces belongs to this vertex
    int getFace() {return _vtriangle_index; } // return the face index that this vertex belongs to
};

//** TO MODIFY
class Triangle
{
    int _vertex1Index, _vertex2Index, _vertex3Index;
    int _triangle1Index, _triangle2Index,_triangle3Index;

public:
    Triangle():_vertex1Index(),_vertex2Index(),_vertex3Index(),_triangle1Index(-1),_triangle2Index(-1),_triangle3Index(-1){}

    Triangle(int a, int b, int c){
        _vertex1Index = a;
        _vertex2Index = b;
        _vertex3Index = c;

        _triangle1Index = -1;
        _triangle2Index = -1;
        _triangle3Index = -1;
    }

    Triangle(int a, int b, int c, int voisin_one, int voisin_two, int voisin_three){
        _vertex1Index = a;
        _vertex2Index = b;
        _vertex3Index = c;

        _triangle1Index = voisin_one;
        _triangle2Index = voisin_two;
        _triangle3Index = voisin_three;
    }

    // get
        int a() const { return  _vertex1Index; }
        int b() const { return  _vertex2Index; }
        int c() const { return  _vertex3Index; }
        bool isInfinityFace(){ return _vertex1Index == 0 || _vertex2Index == 0 || _vertex3Index == 0;}

        int voisin1() const { return  _triangle1Index; }
        int voisin2() const { return  _triangle2Index; }
        int voisin3() const { return  _triangle3Index; }

        bool testVertex(int i) // check if this face has this vertex inside
            {
                return (_vertex1Index == i || _vertex2Index==i || _vertex3Index==i);
            }

        void attachedVoisin(int a, int b, int c) // set three voisin by index attached triangles
            {
                _triangle1Index = a;
                _triangle2Index = b;
                _triangle3Index = c;
            }

         void setVoisin(int attached) // set one voisin by index attached triangles
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

         void changeVoisin(int i, int n)
         {
             if(_triangle1Index == i)
                 _triangle1Index = n;
             if(_triangle2Index == i)
                 _triangle2Index = n;
             if(_triangle3Index == i)
                 _triangle3Index = n;
         }
         int vertex_index(int n)
         {
             if(_vertex1Index == n)
                 return 0;
             if(_vertex2Index == n)
                 return 1;
             if(_vertex3Index == n)
                 return 2;
         }
};



//** TO MODIFY
class Mesh
{    
    QVector<Vertex> _vertexArray;
    QVector<Triangle> _facesArray;

    QVector<int> _infinityFaces;
public:
    Mesh();
    // Bouding box constructor
    Mesh(Vertex one, Vertex two, Vertex three, Vertex four) // constuctor bouding box with 2 vertex
    {
        _vertexArray.push_back(one);
        _vertexArray.push_back(two);
        _vertexArray.push_back(three);
        _vertexArray.push_back(four);

        Triangle first(0,1,2);
        Triangle second(1,2,3);

        first.setVoisin(1); // set voisin index by 1 = second
        second.setVoisin(0); // set voisin index by 0 = first
        _facesArray.push_back(first);
        _facesArray.push_back(second);
    }

    void oneTriangleInit()
    {
        Vertex infinite(5,5,-5,1);
        Vertex one(0,0,0,0);
        Vertex two(0,-5,0,0);
        Vertex three(5,0,0,0);

        _facesArray.resize(0);
        _facesArray.push_back(Triangle(1,2,3,2,3,1));
        _facesArray.push_back(Triangle(0,1,2,0,2,3));
        _facesArray.push_back(Triangle(0,2,3,0,3,1));
        _facesArray.push_back(Triangle(0,3,1,0,1,2));


        _vertexArray.push_back(infinite);
        _vertexArray.push_back(one);
        _vertexArray.push_back(two);
        _vertexArray.push_back(three);

        _infinityFaces.push_back(1);
        _infinityFaces.push_back(2);
        _infinityFaces.push_back(3);
    }
    // Check a point inside a triangle
    float sign (Vertex p1, Vertex p2, Vertex p3)
    {
        return (p1.x() - p3.x()) * (p2.y() - p3.y()) - (p2.x() - p3.x()) * (p1.y() - p3.y());
    }

    bool PointInTriangle (Vertex pt, Vertex v1, Vertex v2, Vertex v3)
    {
        bool b1, b2, b3;

        b1 = sign(pt, v1, v2) < 0.0f;
        b2 = sign(pt, v2, v3) < 0.0f;
        b3 = sign(pt, v3, v1) < 0.0f;

        return ((b1 == b2) && (b2 == b3));
    }


    bool testVoisin(Triangle one, Triangle two) // test if 2 triangle is voisin
    {
        int a1 = one.a();
        int b1 = one.b();
        int c1 = one.c();

        int a2 = two.a();
        int b2 = two.b();
        int c2 = two.c();
        bool flag1 = (a1 == a2 || a1 == b2 || a1 == c2) && (b1 == a2 || b1 == b2 || b1 == c2);
        bool flag2 = (a1 == a2 || a1 == b2 || a1 == c2) && (c1 == a2 || c1 == b2 || c1 == c2);
        bool flag3 = (c1 == a2 || c1 == b2 || c1 == c2) && (b1 == a2 || b1 == b2 || b1 == c2);

        return flag1 || flag2 || flag3;
    }

    // split a triangle index n into 3 with a point
    bool splitMesh(Vertex pt, int n)
    {

        Vertex point1 = _vertexArray[_facesArray[n].a()];
        Vertex point2 = _vertexArray[_facesArray[n].b()];
        Vertex point3 = _vertexArray[_facesArray[n].c()];

        if(!PointInTriangle(pt,point1,point2,point3))
            return false; // point is not inside triangle
        else
        {
            _vertexArray.push_back(pt); // push the vertex to array, index = array size
            Triangle one(_vertexArray.size()-1,_facesArray[n].a(),_facesArray[n].b());
            Triangle two(_vertexArray.size()-1,_facesArray[n].a(),_facesArray[n].c());
            Triangle three(_vertexArray.size()-1,_facesArray[n].b(),_facesArray[n].c()); // 3 new triangles

           if(_facesArray[n].voisin1() != -1 && testVoisin(_facesArray[_facesArray[n].voisin1()],two)){
                two.attachedVoisin(n,_facesArray[n].voisin1(),_facesArray.size()+1);
                _facesArray[_facesArray[n].voisin1()].changeVoisin(n,_facesArray.size());
           }
           if(_facesArray[n].voisin2() != -1 && testVoisin(_facesArray[_facesArray[n].voisin2()],two)){
                two.attachedVoisin(n,_facesArray[n].voisin2(),_facesArray.size()+1);
                _facesArray[_facesArray[n].voisin2()].changeVoisin(n,_facesArray.size());
           }

           if(_facesArray[n].voisin3() != -1 && testVoisin(_facesArray[_facesArray[n].voisin3()],two)){
                two.attachedVoisin(n,_facesArray[n].voisin3(),_facesArray.size()+1);
                _facesArray[_facesArray[n].voisin3()].changeVoisin(n,_facesArray.size());
           }

           if(_facesArray[n].voisin1() != -1 && testVoisin(_facesArray[_facesArray[n].voisin1()],three)){
                three.attachedVoisin(n,_facesArray[n].voisin1(),_facesArray.size());
                _facesArray[_facesArray[n].voisin1()].changeVoisin(n,_facesArray.size()+1);
           }
           if(_facesArray[n].voisin2() != -1 && testVoisin(_facesArray[_facesArray[n].voisin2()],three)){
                three.attachedVoisin(n,_facesArray[n].voisin2(),_facesArray.size());
                _facesArray[_facesArray[n].voisin2()].changeVoisin(n,_facesArray.size()+1);
           }

           if(_facesArray[n].voisin3() != -1 && testVoisin(_facesArray[_facesArray[n].voisin3()],three)){
                two.attachedVoisin(n,_facesArray[n].voisin3(),_facesArray.size());
                _facesArray[_facesArray[n].voisin3()].changeVoisin(n,_facesArray.size()+1);
           }

           if(_facesArray[n].voisin1() != -1 && testVoisin(one,_facesArray[_facesArray[n].voisin1()]))
                one.attachedVoisin(_facesArray.size(),_facesArray.size()+1,_facesArray[n].voisin1());
           if(_facesArray[n].voisin2() != -1 && testVoisin(one,_facesArray[_facesArray[n].voisin2()]))
                one.attachedVoisin(_facesArray.size(),_facesArray.size()+1,_facesArray[n].voisin2());
           if(_facesArray[n].voisin3() != -1 && testVoisin(one,_facesArray[_facesArray[n].voisin3()]))
                one.attachedVoisin(_facesArray.size(),_facesArray.size()+1,_facesArray[n].voisin3());


            _facesArray[n] = one; // replace the face n by the first one
            _facesArray.push_back(two); // push the second to array, index = array size
            _facesArray.push_back(three); // push the thrid to array, index = array size + 1


            return true;
        }


    }

    // insert a 2D point into Mesh
    void insertMesh(Vertex pt)
    {
        int i = 0;
        while (!splitMesh(pt,i))
        {
            i++;
        }

     }
    //-----------------------------------------------------
    bool newSplitTriangle(Vertex pt, int index_face);
    int newInsertMesh(Vertex pt)
    {
        for(int i=0 ; i<_facesArray.size(); i++)
        {
            if(_facesArray[i].isInfinityFace())
                continue;
             else
            {
            Vertex point1 = _vertexArray[_facesArray[i].a()];
            Vertex point2 = _vertexArray[_facesArray[i].b()];
            Vertex point3 = _vertexArray[_facesArray[i].c()];
            if(PointInTriangle(pt,point1,point2,point3))
                {
                newSplitTriangle(pt,i);
                return 1; // insert that point outside
                }
            }
        }
            // to do
            return 0 ; // insert that point outside

     }
    void readPointFromFile(QString fileName);

    void deBug()
    {
        cout<<"So luong diem: "<< _vertexArray.size()<<endl;
        cout<<"So luong mat: " << _facesArray.size()<<endl;

       for(int i=0 ; i< _facesArray.size(); i++)
       {
           cout<<"Face thu: " << i <<endl;
           cout<<"V1: " << _facesArray[i].a();
           cout<<" V2: " << _facesArray[i].b();
           cout<<" V3: " << _facesArray[i].c();
           cout<<endl;
           cout<<"Hang Xom 1: " << _facesArray[i].voisin1();
           cout<<" Hang Xom 2: " << _facesArray[i].voisin2();
           cout<<" Hang Xom 3: " << _facesArray[i].voisin3();

           cout<<endl;

       }
    }
    void flip(int triangle_index, int vertex)
    {
       // Triangle face_voisin = _facesArray[triangle_index]
    }
    // The two following functions could be displaced into a module OpenGLDisplayMesh that would include Mesh
    void draw();
    void drawTetraedra();
    void drawTetraedraWireFrame();
};

#endif // MESH_H
