#include "mesh.h"
#include <iostream>
#include <QFile>
Mesh::Mesh()
{

}

// The following functions could be displaced into a module OpenGLDisplayMesh that would include Mesh
// Draw a vertex
void glVertexDraw(Vertex vertex) {
    glVertex3f(vertex.x(), vertex.y(), vertex.z());
}

//Example with a tetraedra
void Mesh::drawTetraedra() {
    QVector<Vertex> vertexTab;
    vertexTab.push_back(Vertex(-0.5,-0.5,-0.5)); //0
    vertexTab.push_back(Vertex(0.5,-0.5,-0.5)); // 1
    vertexTab.push_back(Vertex(0,0.5,-0.5)); // 2
    vertexTab.push_back(Vertex(0,-0.5,0.5)); // 3

    int faces[12]={
        0,1,2, // 1
        1,3,2, // 2
        3,0,2, // 3
        0,1,3  // 4
    };

    for(int i = 0; i < 12; i+=3) {

        if (i < 3) glColor3d(1,0,0);
        else if (i < 6) glColor3d(0,1,0);
        else if (i < 9) glColor3d(0,0,1);
        else glColor3d(1,1,0);

        glBegin(GL_TRIANGLES);
        glVertexDraw(vertexTab[faces[i]]);
        glVertexDraw(vertexTab[faces[i+1]]);
        glVertexDraw(vertexTab[faces[i+2]]);
        glEnd();
    }


}


double randd() {
  return (double)rand() / (RAND_MAX + 1.0);
}

void Mesh::draw()
{

    glPointSize(3);
    glLineWidth(3);
    for(int i = 0; i < _facesArray.size(); i++) {

        glColor3d(1,0,0);

        glBegin(GL_POINTS);
        glVertexDraw(_vertexArray[_facesArray[i].a()]);
        glVertexDraw(_vertexArray[_facesArray[i].b()]);
        glVertexDraw(_vertexArray[_facesArray[i].c()]);
        glEnd();
        glColor3d(0,0,1);
        glBegin(GL_LINE_STRIP);
        glVertexDraw(_vertexArray[_facesArray[i].a()]);
        glVertexDraw(_vertexArray[_facesArray[i].b()]);
        glEnd();


        glBegin(GL_LINE_STRIP);
        glVertexDraw(_vertexArray[_facesArray[i].b()]);
        glVertexDraw(_vertexArray[_facesArray[i].c()]);
        glEnd();


        glBegin(GL_LINE_STRIP);
        glVertexDraw(_vertexArray[_facesArray[i].c()]);
        glVertexDraw(_vertexArray[_facesArray[i].a()]);
        glEnd();

        if(!_facesArray[i].isInfinityFace())
        {
            glColor3d(0,1,0);
            glBegin(GL_TRIANGLES);
            glVertexDraw(_vertexArray[_facesArray[i].a()]);
            glVertexDraw(_vertexArray[_facesArray[i].b()]);
            glVertexDraw(_vertexArray[_facesArray[i].c()]);
            glEnd();
        }


}


}

//Example with a wireframe tedraedra
void Mesh::drawTetraedraWireFrame() {
    QVector<Vertex> vertexTab;
    vertexTab.push_back(Vertex(-0.5,-0.5,-0.5)); //0
    vertexTab.push_back(Vertex(0.5,-0.5,-0.5)); // 1
    vertexTab.push_back(Vertex(0,0.5,-0.5)); // 2
    vertexTab.push_back(Vertex(0,-0.5,0.5)); // 3

    int faces[12]={
        0,1,2, // 1
        1,3,2, // 2
        3,0,2, // 3
        0,1,3  // 4
    };

    for(int i = 0; i < 12; i+=3) {
        glBegin(GL_LINE_STRIP);
            glVertexDraw(vertexTab[faces[i]]);
            glVertexDraw(vertexTab[faces[i+1]]);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertexDraw(vertexTab[faces[i+1]]);
            glVertexDraw(vertexTab[faces[i+2]]);
        glEnd();
        glBegin(GL_LINE_STRIP);
            glVertexDraw(vertexTab[faces[i+2]]);
            glVertexDraw(vertexTab[faces[i]]);
        glEnd();
    }

}
void Mesh::readPointFromFile(QString fileName)
{
    QFile inputFile(fileName);
        inputFile.open(QIODevice::ReadOnly);
        if (!inputFile.isOpen()){
            std::cout << "Can't not find documents" << std::endl;
            return;
         }

        QTextStream stream(&inputFile);
        QString line = stream.readLine();

        QString numStr;

        while (!line.isNull()) {

               line = stream.readLine();
               if(line.split(" ", QString::SkipEmptyParts).count() == 3)
               {
                   // read vertex
                   float a = line.split(" ", QString::SkipEmptyParts)[0].toFloat();
                   float b = line.split(" ", QString::SkipEmptyParts)[1].toFloat();
                   float c = line.split(" ", QString::SkipEmptyParts)[2].toFloat();
                   insertMesh(Vertex(a,b,c));

               }
        }
}

bool Mesh::newSplitTriangle(Vertex pt, int index_face)
{

    int pt_index = _vertexArray.size();
    _vertexArray.push_back(pt);

    int vertex1 = _facesArray[index_face].a();
    int vertex2 = _facesArray[index_face].b();
    int vertex3 = _facesArray[index_face].c();

    Triangle one(vertex1,vertex2,pt_index,_facesArray.size(),_facesArray.size()+1,_facesArray[index_face].voisin3());
    Triangle two(pt_index,vertex2,vertex3,_facesArray[index_face].voisin1(),_facesArray.size()+1,index_face);
    Triangle three(vertex1,pt_index,vertex3,_facesArray.size(),_facesArray[index_face].voisin2(),index_face);

    int voisin_1 = _facesArray[index_face].voisin1();
    int voisin_2 = _facesArray[index_face].voisin2();
    int voisin_3 = _facesArray[index_face].voisin3();

    _facesArray[voisin_1].changeVoisin(index_face,_facesArray.size());
    _facesArray[voisin_2].changeVoisin(index_face,_facesArray.size()+1);


    _facesArray[index_face] = one;
    _facesArray.push_back(two);
    _facesArray.push_back(three);

    return true;

}
