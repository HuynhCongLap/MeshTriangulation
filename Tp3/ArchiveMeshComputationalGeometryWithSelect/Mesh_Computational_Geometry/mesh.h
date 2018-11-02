#ifndef MESH_H
#define MESH_H

#include <QGLWidget>
#include "QDebug"
#include "iostream"
#include <QQueue>
#include <QStack>
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
class BorderEdge{

    int vertex_one;
    int vertex_two;
    int m_face;
public:
    BorderEdge(){}
    BorderEdge(const BorderEdge &copy){
        vertex_one = copy.one();
        vertex_two = copy.two();
        m_face = copy.face();
    }
    BorderEdge(int one, int two, int face):vertex_one(one),vertex_two(two), m_face(face){}
    bool duplicate(BorderEdge a){
        return (vertex_one == a.one() && vertex_two == a.two() ) || (vertex_one == a.two() && vertex_two == a.one());
    }
public:
    int one() const {return vertex_one;}
    int two() const {return vertex_two;}
    int face()const {return m_face;}
};

//** TO MODIFY
class Triangle
{

    // add  later
    QVector <int> _vertexIndex;
    QVector <int> _triangleIndex;
public:
    Triangle(){}

    Triangle(int a, int b, int c){
        _vertexIndex.push_back(a);
        _vertexIndex.push_back(b);
        _vertexIndex.push_back(c);
    }

    Triangle(int a, int b, int c, int voisin_one, int voisin_two, int voisin_three){
        _vertexIndex.push_back(a);
        _vertexIndex.push_back(b);
        _vertexIndex.push_back(c);

        _triangleIndex.push_back(voisin_one);
        _triangleIndex.push_back(voisin_two);
        _triangleIndex.push_back(voisin_three);
    }

    // get
        int a() const { return  _vertexIndex[0]; }
        int b() const { return  _vertexIndex[1]; }
        int c() const { return  _vertexIndex[2]; }
        bool isInfinityFace(){ return _vertexIndex.indexOf(0)!=-1; }

        int voisin1() const { return  _triangleIndex[0]; }
        int voisin2() const { return  _triangleIndex[1]; }
        int voisin3() const { return  _triangleIndex[2]; }

        int getVoisin(int n) {
            if (n > 2 || n < 0)
                return -1;
            else
                return _triangleIndex[n];
          }
        int getVertex(int n){
            if (n > 2 || n < 0)
                return -1;
            else
                return _vertexIndex[n];
        }

        bool testVertex(int i) // check if this face has this vertex inside
            {
                return _vertexIndex.indexOf(i) != -1;
            }

        void attachedVoisin(int a, int b, int c) // set three voisin by index attached triangles
            {
                _triangleIndex[0] = a;
                _triangleIndex[1] = b;
                _triangleIndex[2] = c;
            }


         void changeVoisin(int i, int n)
         {
             int index = _triangleIndex.indexOf(i);
             if(index == -1)
                 return;
             else
                 _triangleIndex[index] = n;
         }

         void changeVoisin(BorderEdge be)
         {
             int v_i = vertex_index(be.two());
             changeVoisin(getVoisin(next_vertex_index(v_i)),be.face());
         }

         void changeVertex(int i, int n)
         {
             int index = _vertexIndex.indexOf(i);
             if(index == -1)
                 return;
             else
                 _vertexIndex[index] = n;
         }

         int vertex_index(int n)
         {
             return _vertexIndex.indexOf(n);
         }

         int voisin_index(int n)
         {
             return _triangleIndex.indexOf(n);
         }
         int next_vertex_index(int i)
         {
             if (i > 2 || i < 0)
                 return -1;
             else
             {
                 i++;
                 if(i>2)
                     return 0;
                 else
                     return i;
             }
         }
};


//** TO MODIFY
class Mesh
{    
    QVector<Vertex> _vertexArray;
    QVector<Triangle> _facesArray;

    QVector<int> _infinityFaces;
    QStack<BorderEdge> _borderEgde;
public:
    Mesh();
    // Bouding box constructor


    void oneTriangleInit()
    {
        Vertex infinite(5,5,-5,1);
        Vertex one(0,0,0,0);
        Vertex two(0,-5,0,0);
        Vertex three(5,-5,0,0);
        Vertex four(5,0,0,0);

        _facesArray.resize(0);
        _facesArray.push_back(Triangle(1,2,3,4,1,3));
        _facesArray.push_back(Triangle(1,3,4,5,2,0));
        _facesArray.push_back(Triangle(0,4,1,1,3,5));
        _facesArray.push_back(Triangle(0,1,2,0,4,2));
        _facesArray.push_back(Triangle(0,2,3,0,5,3));
        _facesArray.push_back(Triangle(0,3,4,1,2,4));


        _vertexArray.push_back(infinite);
        _vertexArray.push_back(one);
        _vertexArray.push_back(two);
        _vertexArray.push_back(three);
        _vertexArray.push_back(four);

        _infinityFaces.push_back(2);
        _infinityFaces.push_back(3);
        _infinityFaces.push_back(4);
        _infinityFaces.push_back(5);
    }
    // Check a point inside a triangle
    void boudingBox(){
        Vertex one(0.5,0,0,0);
        Vertex two(0,0,-5,0);
        Vertex three(5,0,-5,1);
        Vertex four(5,0,0,1);

        Triangle face_one(0,1,2,-1,1,-1);
        Triangle face_two(2,3,0,-1,0,-1);

        _vertexArray.push_back(one);
        _vertexArray.push_back(two);
        _vertexArray.push_back(three);
        _vertexArray.push_back(four);

        _facesArray.push_back(face_one);
        _facesArray.push_back(face_two);

        _borderEgde.push_back(BorderEdge(0,1,0));
        _borderEgde.push_back(BorderEdge(1,2,0));
        _borderEgde.push_back(BorderEdge(2,3,1));
        _borderEgde.push_back(BorderEdge(3,0,1));

    }
    float sign (Vertex p1, Vertex p2, Vertex p3)
    {
        return (p1.x() - p3.x()) * (p2.y() - p3.y()) - (p2.x() - p3.x()) * (p1.y() - p3.y());
    }

    float signxz (Vertex p1, Vertex p2, Vertex p3)
    {
        return (p1.x() - p3.x()) * (p2.z() - p3.z()) - (p2.x() - p3.x()) * (p1.z() - p3.z());
    }

    bool PointInTriangle (Vertex pt, Vertex v1, Vertex v2, Vertex v3)
    {
        bool b1, b2, b3;

        b1 = signxz(pt, v1, v2) < 0.0f;
        b2 = signxz(pt, v2, v3) < 0.0f;
        b3 = signxz(pt, v3, v1) < 0.0f;

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
    bool newSplit(Vertex pt, int index_face){}
    int newInsertMesh(Vertex pt)
    {
        for(int i=0 ; i<_facesArray.size(); i++)
        {

            Vertex point1 = _vertexArray[_facesArray[i].a()];
            Vertex point2 = _vertexArray[_facesArray[i].b()];
            Vertex point3 = _vertexArray[_facesArray[i].c()];
            if(PointInTriangle(pt,point1,point2,point3))
                {
                    newSplitTriangle(pt,i);
                     return 1; // insert that inside outside
                }

        }
            insertPointOutSide(pt);
            // to do
            return 0 ; // insert that point outside

     }

    int newInsert(Vertex pt)
    {
        for(int i=0 ; i<_facesArray.size(); i++)
        {

            Vertex point1 = _vertexArray[_facesArray[i].a()];
            Vertex point2 = _vertexArray[_facesArray[i].b()];
            Vertex point3 = _vertexArray[_facesArray[i].c()];
            if(PointInTriangle(pt,point1,point2,point3))
                {
                    newSplit(pt,i);
                    return 1; // insert that point inside
                }
             else
                {

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
          int vertex_index = _facesArray[triangle_index].vertex_index(vertex);
          if(vertex_index == -1){
            cout << "This face has no this vertex"<<endl;
            return;
          }
          else
          {

              int voisin = _facesArray[triangle_index].getVoisin(vertex_index);
              int _vf = _facesArray[triangle_index].next_vertex_index(vertex_index);
              int _vs = _facesArray[triangle_index].next_vertex_index(_vf);

              int point_two = _facesArray[triangle_index].getVertex(_vf);
              int point_three = _facesArray[triangle_index].getVertex(_vs);

              int voisin_vertex_index = _facesArray[voisin].voisin_index(triangle_index);
              int point_four = _facesArray[voisin].getVertex(voisin_vertex_index);


              int voisin2 = _facesArray[triangle_index].getVoisin(_vf);

              int _vvs = _facesArray[voisin].next_vertex_index(voisin_vertex_index);
              int voisin3 = _facesArray[voisin].getVoisin(_vvs);

              _facesArray[voisin2].changeVoisin(triangle_index,voisin);
              _facesArray[voisin3].changeVoisin(voisin,triangle_index);


              _facesArray[triangle_index].changeVertex(point_three,point_four);
              _facesArray[triangle_index].changeVoisin(voisin2,voisin3);

              _facesArray[voisin].changeVertex(point_two,vertex);
              _facesArray[voisin].changeVoisin(voisin3,voisin2);

          }

    }
    // The two following functions could be displaced into a module OpenGLDisplayMesh that would include Mesh

    void insertPointOutSide(Vertex vt);
    void draw();
    void drawTetraedra();
    void drawTetraedraWireFrame();
};

#endif // MESH_H
