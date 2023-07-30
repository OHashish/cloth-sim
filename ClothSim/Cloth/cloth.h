#pragma warning(disable: 4018)
#ifndef  _CLOTH_H_
#define  _CLOTH_H_


#include <vector>
#include <map>
#include <string>
#include <QGLWidget>
#include <QWidget>
#include <QObject>
#include <QFileDialog>
#include "clothwindow.h"
#include <glm/glm.hpp>

using namespace  std;

class  Cloth: public QGLWidget
{

Q_OBJECT
public slots:

void updateTime();
void saveVideo(double);
void saveFile();
void loadObject();

void SS1();
void SS1Rotate();
void SS2();
void SS2Wind();

void posX(int);
void posY(int);
void posZ(int);

void rotX(double);
void rotY(double);
void rotZ(double);

void playPause();

  public:
    Cloth();
    Cloth( QWidget *parent,string fileName,float clothDimension);
    ~Cloth();
    void  Clear();
    // translation in window x,y
    GLfloat translate_x, translate_y;
    GLfloat last_x, last_y;
    const glm::vec3 GRAVITY=glm::vec3(0,-9.8,0);

    float CLOTH_DIMENSION;

    // which button was last pressed
    int whichButton;

    // called when OpenGL context is set up
    void initializeGL();
    // called every time the widget is resized
    void resizeGL(int w, int h);
    // called every time the widget needs painting
    void paintGL();

    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

    void updateForces();

    // a variable to store the texture's ID on the GPU
    GLuint textureID;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> textureCoords;

    std::vector<std::vector<unsigned int> > faceVertices;
    std::vector<std::vector<unsigned int> > faceNormals;
    std::vector<std::vector<unsigned int> > faceTexCoords;

     std::vector<std::vector<glm::vec3> > particlePosition;
     std::vector<std::vector<glm::vec3> > particleVelocity;
     std::vector<std::vector<unsigned int> > particleID;
     std::vector<QImage> qimageScreenshots;

     std::vector<std::vector<glm::vec3> > oldParticlePosition;

void readObjectData(std::istream &geometryStream);

void meshCreator(int dimensions);
// write routine
void saveCurrentFrame(std::ostream &geometryStream);

    void renderObject();
    void updateVertices();
};



#endif // _CLOTH_H_
