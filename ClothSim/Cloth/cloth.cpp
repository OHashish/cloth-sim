#define _USE_MATH_DEFINES
#include <fstream>
#include <string>
#include "cloth.h"
#include "string.h"
#include "cstring"
#include "stdlib.h"
#include <algorithm>
#include <iterator>
#include <math.h>
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
#include <QWidget>
#include <QGLWidget>
#include <QTimer>
#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QMouseEvent>
#include <iostream>
#include "Jacobi"
#include "Dense"
#include <QImage>
#include <QProcess>

double framespeed=0;
int posx=0;
int posy=0;
int posz=0;
double rotx=0;
double roty=0;
double rotz=0;
int togglePlay=0;
double stop=0;

int toggleSS1=0;
int toggleRotate=0;

int toggleSS2=0;
int toggleWind=0;
typedef struct materialStruct {
  GLfloat ambient[4];
  GLfloat diffuse[4];
  GLfloat specular[4];
  GLfloat shininess;
} materialStruct;

static materialStruct chromeMaterials = {
    {0.25f, 0.25f, 0.25f, 1.0f  },
    {0.4f, 0.4f, 0.4f, 1.0f },
    {0.774597f, 0.774597f, 0.774597f, 1.0f },
    50.8f
};

Cloth::Cloth(){

}

void Cloth::loadObject(){
     toggleSS1=0;
     toggleRotate=0;

     toggleSS2=0;
     toggleWind=0;

        auto fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                        QDir::home().absolutePath(),
                                                        tr("Images (*.obj)"),0,QFileDialog::DontUseNativeDialog);
         string fileNameChar=fileName.toStdString();
        Clear();
            std::ifstream geometryFile(fileNameChar);
            readObjectData(geometryFile);


}

Cloth::Cloth(QWidget *parent,string fileName,float clothDimension) : QGLWidget(parent){
    CLOTH_DIMENSION=clothDimension;
    QTimer *ptimer = new QTimer(this);
    ptimer->start(20);
    connect(ptimer, SIGNAL(timeout()),  this, SLOT(updateTime()));

    Clear();
    meshCreator(CLOTH_DIMENSION);
}

Cloth::~Cloth(){

}
void Cloth::Clear(){
    vertices.clear();
    faceNormals.clear();
    faceTexCoords.clear();
    faceVertices.clear();
    normals.clear();
    textureCoords.clear();
}
void Cloth::initializeGL(){
    glEnable(GL_DEPTH_TEST);
//    glShadeModel(GL_FLAT);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);   
    glEnable(GL_COLOR_MATERIAL);
    glClearColor( 0.0, 0.0,  0.0,  0.0);
}

void Cloth::resizeGL(int w, int h){ 
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double aspectRatio = double(w) / double(h);
    if (aspectRatio > 1.0)
            glFrustum (-aspectRatio * 0.01, aspectRatio * 0.01, -0.01, 0.01, 0.01, 200.0);
    else
            glFrustum ( -0.01, 0.01,-aspectRatio * 0.01, aspectRatio * 0.01, 0.01, 200.0);


}

void Cloth::paintGL(){ 
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // set light position first

    GLfloat light_position[] = {35.0f+(float)rotx, 31.0f+(float)roty, -22.0f+(float)rotz ,1.0};
    glRotatef(posx,1,0,0);
    glRotatef(posy,0,1,0);
    glRotatef(posz,0,0,1);
//    GLfloat light_position[] = {35.0f, 31.0f, -22.0f ,1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // apply translation for interface control
    glLoadIdentity();




    glTranslatef(translate_x-1, translate_y-4, -12);
    glRotatef(posx,1,0,0);
    glRotatef(posy,0,1,0);
    glRotatef(posz,0,0,1);
    glPushMatrix();
        glPushMatrix();
            if (toggleSS1==1){
                float surfaceColour[4] = { 0.3f, 0.0f, 0.9f, 0.0f };
                glColor4fv(surfaceColour);
                //Place Sphere on Surphace
                glTranslatef(0, +2, 0);
                if (toggleSS2==1){
                    glRotatef((float)framespeed,0,1,0);
                }
                GLUquadricObj *globeSphere = gluNewQuadric();
                gluQuadricDrawStyle(globeSphere,GLU_FILL);
                gluQuadricNormals(globeSphere,GLU_SMOOTH);
                gluSphere(globeSphere,2,32,16);
            }
        glPopMatrix();
        glPushMatrix();
            glTranslatef(-15,0 , -15);
            glColor3f(0.6,0.6,0.6);

            glBegin(GL_QUADS);
            glNormal3f(0,1,0);
                glVertex3f( 20,0.00, 20);
                glVertex3f( 20,0.00,10);
                glVertex3f(10,0.00,10);
                glVertex3f(10,0.00, 20);
            glEnd();

            glColor3f(0.3,0.3,0.3);
            glBegin(GL_LINES);
            glNormal3f(0,1,0);
                for(int i=0;i<=30;i++){
                    glVertex3f(i,0,0);
                    glVertex3f(i,0,30);
                    glVertex3f(0,0,i);
                    glVertex3f(30,0,i);
                };
            glEnd();
         glPopMatrix();
        glPushMatrix();
            glTranslatef(-4,0 , -4);
            renderObject();
            updateForces();
            updateVertices();
        glPopMatrix();
    glPopMatrix();

}
void Cloth::updateVertices(){
    vertices.clear();
    normals.clear();
    textureCoords.clear();
    for(vector<glm::vec3> row :particlePosition){
        for(glm::vec3 col :row){
            vertices.push_back(col);
            normals.push_back(glm::normalize(col));
        }
    }
}
void Cloth::meshCreator(int dimensions){

    for(int x=0;x<dimensions;x++){
        for(int y=0;y<dimensions;y++){
            vertices.push_back(glm::vec3(x,dimensions,y));
            normals.push_back(1.0f*glm::normalize(glm::vec3(x,dimensions,y)));
            glm::vec3 temp;
            temp.x=(x)/((float)dimensions);
            temp.y=(y)/((float)dimensions);
            textureCoords.push_back(temp);
        }
    };

    particlePosition.resize(dimensions);
    particleID.resize(dimensions);
    particleVelocity.resize(dimensions);
    oldParticlePosition.resize(dimensions);
    for (int i = 0; i < dimensions; i++)
    {
        particlePosition[i].resize(dimensions);
        particleID[i].resize(dimensions);
        particleVelocity[i].resize(dimensions);
        oldParticlePosition[i].resize(dimensions);
    }
    for (int i = 0; i < vertices.size(); i++)
    {
        int row = i / dimensions;
        int col = i %dimensions;
        particlePosition[row][col] = vertices[i];
        particleID[row][col] = i;
        particleVelocity[row][col] = glm::vec3(0,0,0);
        oldParticlePosition[row][col] = vertices[i];
    }

    for(int i=0;i<dimensions-1;i++){
            for(int j=0;j<dimensions-1;j++){
            std::vector<unsigned int> tri;
            tri.push_back(particleID[i][j]);
            tri.push_back(particleID[i+1][j]);
            tri.push_back(particleID[i][j+1]);
            faceTexCoords.push_back(tri);
            faceVertices.push_back(tri);
            faceNormals.push_back(tri);

            tri.clear();
            tri.push_back(particleID[i+1][j]);
            tri.push_back(particleID[i+1][j+1]);
            tri.push_back(particleID[i][j+1]);
            faceTexCoords.push_back(tri);
            faceVertices.push_back(tri);
            faceNormals.push_back(tri);
        }
    }
}

void Cloth::updateForces(){
    int dim =CLOTH_DIMENSION;
    float diagRest=pow(2,0.5);

    float k = 35;
    float d = -0.3;
    for(int i=0;i<dim;i++){
        for(int j=0;j<dim;j++){
            if ((!(( i == 0 && j == 0) || ( i == dim-1 && j == 0)))|| toggleSS2==0){
//                if (true){
                glm::vec3 springForce ={0,0,0};
                // p1-p2 Structure
                    if (j+1 < dim){
                        springForce += (   k*( glm::length(-particlePosition[i][j]+particlePosition[i][j+1])-1.0f)+
                                d*glm::dot(-particleVelocity[i][j]+particleVelocity[i][j+1],
                                (-particlePosition[i][j]+particlePosition[i][j+1])/glm::length(-particlePosition[i][j]+particlePosition[i][j+1]))   )*
                                 ((-particlePosition[i][j]+particlePosition[i][j+1])/glm::length(-particlePosition[i][j]+particlePosition[i][j+1]));
                    }
                    if ( i+1 < dim){
                        springForce += (   k*( glm::length(-particlePosition[i][j]+particlePosition[i+1][j])-1.0f)+
                                d*glm::dot(-particleVelocity[i][j]+particleVelocity[i+1][j],
                                (-particlePosition[i][j]+particlePosition[i+1][j])/glm::length(-particlePosition[i][j]+particlePosition[i+1][j]))   )*
                                 ((-particlePosition[i][j]+particlePosition[i+1][j])/glm::length(-particlePosition[i][j]+particlePosition[i+1][j]));
                    }
                    if ( i-1 >= 0 ){
                        springForce += (   k*( glm::length(-particlePosition[i][j]+particlePosition[i-1][j])-1.0f)+
                                d*glm::dot(-particleVelocity[i][j]+particleVelocity[i-1][j],
                                (-particlePosition[i][j]+particlePosition[i-1][j])/glm::length(-particlePosition[i][j]+particlePosition[i-1][j]))   )*
                                 ((-particlePosition[i][j]+particlePosition[i-1][j])/glm::length(-particlePosition[i][j]+particlePosition[i-1][j]));
                    }
                    if ( j-1 >= 0){
                        springForce += (   k*( glm::length(-particlePosition[i][j]+particlePosition[i][j-1])-1.0f)+
                                d*glm::dot(-particleVelocity[i][j]+particleVelocity[i][j-1],
                                ((-particlePosition[i][j]+particlePosition[i][j-1]))/glm::length(-particlePosition[i][j]+particlePosition[i][j-1]))   )*
                                 ((-particlePosition[i][j]+particlePosition[i][j-1])/glm::length(-particlePosition[i][j]+particlePosition[i][j-1]));
                    }


                    //DIAGONAL
                    if (i+1 < dim && j+1 < dim ){
                        springForce += (   k*( glm::length(-particlePosition[i][j]+particlePosition[i+1][j+1])-diagRest)+
                                d*glm::dot(-particleVelocity[i][j]+particleVelocity[i+1][j+1],
                                (-particlePosition[i][j]+particlePosition[i+1][j+1])/glm::length(-particlePosition[i][j]+particlePosition[i+1][j+1]))   )*
                                 ((-particlePosition[i][j]+particlePosition[i+1][j+1])/glm::length(-particlePosition[i][j]+particlePosition[i+1][j+1]));
                    }
                    if ( i+1 < dim && j-1 >= 0){
                        springForce += (   k*( glm::length(-particlePosition[i][j]+particlePosition[i+1][j-1])-diagRest)+
                                d*glm::dot(-particleVelocity[i][j]+particleVelocity[i+1][j-1],
                                (-particlePosition[i][j]+particlePosition[i+1][j-1])/glm::length(-particlePosition[i][j]+particlePosition[i+1][j-1]))   )*
                                 ((-particlePosition[i][j]+particlePosition[i+1][j-1])/glm::length(-particlePosition[i][j]+particlePosition[i+1][j-1]));
                    }
                    if (i-1 >= 0 && j-1 >= 0 ){
                        springForce += (   k*( glm::length(-particlePosition[i][j]+particlePosition[i-1][j-1])-diagRest)+
                                d*glm::dot(-particleVelocity[i][j]+particleVelocity[i-1][j-1],
                               ( particlePosition[i][j]+particlePosition[i-1][j-1])/glm::length(-particlePosition[i][j]+particlePosition[i-1][j-1]))   )*
                                 ((-particlePosition[i][j]+particlePosition[i-1][j-1])/glm::length(-particlePosition[i][j]+particlePosition[i-1][j-1]));
                    }
                    if ( i-1 >= 0 && j+1 < dim){
                        springForce += (   k*( glm::length(-particlePosition[i][j]+particlePosition[i-1][j+1])-diagRest)+
                                d*glm::dot(-particleVelocity[i][j]+particleVelocity[i-1][j+1],
                                (-particlePosition[i][j]+particlePosition[i-1][j+1])/glm::length(-particlePosition[i][j]+particlePosition[i-1][j+1]))   )*
                                 ((-particlePosition[i][j]+particlePosition[i-1][j+1])/glm::length(-particlePosition[i][j]+particlePosition[i-1][j+1]));
                    }

                    //BEND
                    if (j+2 < dim){
                        springForce += (   k*( glm::length(-particlePosition[i][j]+particlePosition[i][j+2])-2.0f)+
                                d*glm::dot(-particleVelocity[i][j]+particleVelocity[i][j+2],
                                (-particlePosition[i][j]+particlePosition[i][j+2])/glm::length(-particlePosition[i][j]+particlePosition[i][j+2]))   )*
                                 ((-particlePosition[i][j]+particlePosition[i][j+2])/glm::length(-particlePosition[i][j]+particlePosition[i][j+2]));
                    }
                    if ( i+2 < dim){
                        springForce += (   k*( glm::length(-particlePosition[i][j]+particlePosition[i+2][j])-2.0f)+
                                d*glm::dot(-particleVelocity[i][j]+particleVelocity[i+2][j],
                                (-particlePosition[i][j]+particlePosition[i+2][j])/glm::length(-particlePosition[i][j]+particlePosition[i+2][j]))   )*
                                 ((-particlePosition[i][j]+particlePosition[i+2][j])/glm::length(-particlePosition[i][j]+particlePosition[i+2][j]));
                    }
                    if ( i-2 >= 0 ){
                        springForce += (   k*( glm::length(-particlePosition[i][j]+particlePosition[i-2][j])-2.0f)+
                                d*glm::dot(-particleVelocity[i][j]+particleVelocity[i-2][j],
                                (-particlePosition[i][j]+particlePosition[i-2][j])/glm::length(-particlePosition[i][j]+particlePosition[i-2][j]))   )*
                                 ((-particlePosition[i][j]+particlePosition[i-2][j])/glm::length(-particlePosition[i][j]+particlePosition[i-2][j]));
                    }
                    if ( j-2 >= 0){
                        springForce += (   k*( glm::length(-particlePosition[i][j]+particlePosition[i][j-2])-2.0f)+
                                d*glm::dot(-particleVelocity[i][j]+particleVelocity[i][j-2],
                                (-particlePosition[i][j]+particlePosition[i][j-2])/glm::length(-particlePosition[i][j]+particlePosition[i][j-2]))   )*
                                 ((-particlePosition[i][j]+particlePosition[i][j-2])/glm::length(-particlePosition[i][j]+particlePosition[i][j-2]));
                    }



            glm::vec3 steadyWindForce ={0,0,5};
            glm::vec3 acceleraion;
            float change=cos(50*(M_PI/180.0)+framespeed);
            steadyWindForce = abs(change)*steadyWindForce;

            float time = 0.07f;
            float mass = 1.0f;


//            glm::vec3 viscous = -0.1f*particleVelocity[i][j] ;

            float radiusIn=2.18;
            float radiusOut=2.22;

            glm::vec3 friction ;
             if ((pow(glm::length(particlePosition[i][j]-glm::vec3(4,2,4)),2))<pow(radiusIn+1,2)){

                friction = glm::cross(particlePosition[i][j]-glm::vec3(4,2,4),glm::vec3(0,1,0));

             }else{

                 friction={0,0,0};
             }

             if(toggleWind==1){
                 acceleraion = ( (GRAVITY*mass) + springForce +steadyWindForce) / (float) mass;
             }else{
                 acceleraion = ( (GRAVITY*mass) + springForce ) / (float) mass;
             }

            //Explicit Verlet
            glm::vec3 futureplus1,futureplus2;
            futureplus1=2.0f*particlePosition[i][j] - oldParticlePosition[i][j] + (acceleraion)*(pow(time,2.0f));
            futureplus2=2.0f*futureplus1 - particlePosition[i][j] + (acceleraion)*(pow(time+time,2.0f));

            if (toggleSS1==1){
                if (( 2.0f*particlePosition[i][j] - oldParticlePosition[i][j] + (acceleraion)*(pow(time,2.0f))).y > 0.01f &&
                      ((pow(glm::length(particlePosition[i][j]-glm::vec3(4,2,4)),2))>(pow(radiusOut,2)))  ){

                    particlePosition[i][j] = 2.0f*particlePosition[i][j] - oldParticlePosition[i][j] + (acceleraion)*(pow(time,2.0f));

                }

                if (((pow(glm::length(particlePosition[i][j]-glm::vec3(4,2,4)),2))<pow(radiusIn+1,2)) && toggleRotate==1){
                    acceleraion = (friction ) / (float) mass;
                    particlePosition[i][j] = 2.0f*particlePosition[i][j] - oldParticlePosition[i][j] + (acceleraion)*(pow(0.03f,2.0f));
                }

                if ((pow(glm::length(particlePosition[i][j]-glm::vec3(4,2,4)),2))<pow(radiusOut-0.02,2)){
                    particlePosition[i][j] = particlePosition[i][j]-
                            0.002f*glm::normalize(-particlePosition[i][j]+glm::vec3(4,2,4));
                }



           }else{
                if (( 2.0f*particlePosition[i][j] - oldParticlePosition[i][j] + (acceleraion)*(pow(time,2.0f))).y > 0.01f ){
                    particlePosition[i][j] = 2.0f*particlePosition[i][j] - oldParticlePosition[i][j] + (acceleraion)*(pow(time,2.0f));
                }
            }


            oldParticlePosition[i][j]= particlePosition[i][j];

            //Explicit Euler
//            particleVelocity[i][j] = particleVelocity[i][j]+ (time*acceleraion);
//            if ((particlePosition[i][j]+ (time)* (particleVelocity[i][j])).y < 0.01f)
//                particleVelocity[i][j] = glm::vec3(0,0,0);
//            particlePosition[i][j] = particlePosition[i][j]+ (time)* (particleVelocity[i][j]);


        }
        }
    }
    updateVertices();

}
void Cloth::updateTime(){

    framespeed=framespeed+0.01;

    if (togglePlay==1){
        if (framespeed<stop){

            QImage openFrames=this->grabFrameBuffer();
            qimageScreenshots.push_back(openFrames);

        }else{
            for (int i = 0 ; i < 5;i++){
            std::stringstream name;
            name << "screenshots/frame"<<i<< ".png";
            QString filename(name.str().c_str());
                    qimageScreenshots[i].save(filename);
                    cout<<"Frame "<<i<< " Saved"<<endl;
            }
            togglePlay=0;
        }
    }
    this->repaint();
}

void Cloth::saveVideo(double change){

//    system("ffmpeg -framerate 1 -i frame%d.png -c:v libx264 -r 25 output.mp4");
    this->repaint();
}
void Cloth::saveFile(){

    auto fileName = QFileDialog::getSaveFileName(this,
                                                 tr("Generate OBJ File"),
                                                 QDir::home().absolutePath(),
                                                 tr("OBJ files (*.obj)"));

     string fileNameChar=fileName.toStdString();
     std::ofstream geometryFile(fileNameChar);
    saveCurrentFrame(geometryFile);

    QMessageBox::about(this, tr("About Application"),
                       tr("New Cloth File With Changes Saved!"));


}


void Cloth::mousePressEvent(QMouseEvent *event)
{
    whichButton = event->button();

    float size = (width() > height()) ? height() : width();

    double currentx;
    double currenty;

    currentx= (2.0 * event->x() - size) / size;
    currenty = (size - 2.0 * event->y() ) / size;

    switch(whichButton)
    {
    case Qt::RightButton:
        last_x = currentx;
        last_y = currenty;
        updateGL();
        break;
    }
}

void Cloth::mouseMoveEvent(QMouseEvent *event)
{
    float size = (width() > height()) ? height() : width();

    double currentx;
    double currenty;
    currentx= (2.0 * event->x() - size) / size;
    currenty = (size - 2.0 * event->y() ) / size;

    switch(whichButton)
    {
    case Qt::RightButton:
        translate_x += currentx- last_x;
        translate_y += currenty - last_y;
        last_x = currentx;
        last_y =currenty;
        updateGL();
        break;
    }
}

void Cloth::mouseReleaseEvent(QMouseEvent *event)
{
    switch(whichButton)
    {
    case Qt::RightButton:
        updateGL();
        break;
    }
}

void Cloth::posX(int pos){posx=pos;this->repaint();}
void Cloth::posY(int pos){posy=pos;this->repaint();}
void Cloth::posZ(int pos){posz=pos;this->repaint();}

void Cloth::rotX(double rot){rotx=rot; this->repaint();}
void Cloth::rotY(double rot){roty=rot;this->repaint();}
void Cloth::rotZ(double rot){rotz=rot; this->repaint();}

void Cloth::playPause(){
    Clear();
    meshCreator(CLOTH_DIMENSION);
    if (togglePlay==1){
        togglePlay=0;
    }else {
        stop=framespeed+5;
        togglePlay=1;
    }

}

void Cloth::SS1(){
    Clear();
    meshCreator(CLOTH_DIMENSION);
    if (toggleSS1==1){
        toggleSS1=0;
    }else {

        toggleSS1=1;
    }
}
void Cloth::SS1Rotate(){
    Clear();
    meshCreator(CLOTH_DIMENSION);
    if (toggleRotate==1){
        toggleRotate=0;
    }else {

        toggleRotate=1;
    }
}
void Cloth::SS2(){
    Clear();
    meshCreator(CLOTH_DIMENSION);
    if (toggleSS2==1){
        toggleSS2=0;
    }else {

        toggleSS2=1;
    }
}

void Cloth::SS2Wind(){
    Clear();
    meshCreator(CLOTH_DIMENSION);
    if (toggleWind==1){
        toggleWind=0;
    }else {

        toggleWind=1;
    }
}


void Cloth::readObjectData(std::istream &bufferReadStream){
    char readBuffer[1024];
    while (true){
        char firstChar = char(bufferReadStream.get());

        if (bufferReadStream.eof())
            break;

        switch (firstChar){
            case '#':
                bufferReadStream.getline(readBuffer, 1024);
                break;
            case 'v':
            {
                char secondChar = char(bufferReadStream.get());

                if (bufferReadStream.eof())
                    break;

                switch (secondChar){
                    case ' ':
                    {
                        glm::vec3 vertex;
                        bufferReadStream >> vertex.x;
                        bufferReadStream >> vertex.y;
                        bufferReadStream >> vertex.z;
                        vertices.push_back(vertex);
                        break;
                    }
                    case 'n':
                    {
                        glm::vec3  normal;
                        bufferReadStream >> normal.x;
                        bufferReadStream >> normal.y;
                        bufferReadStream >> normal.z;

                        normals.push_back(normal);
                        break;
                    }
                    case 't':
                    {
                        glm::vec3 texCoord;
                        bufferReadStream >> texCoord.x;
                        bufferReadStream >> texCoord.y;
                        bufferReadStream >> texCoord.z;
                        textureCoords.push_back(texCoord);
                        break;
                    }
                    default:
                        break;
                }
                break;
            }

            case 'f':
            {
                bufferReadStream.getline(readBuffer, 1024);

                std::string lineString = std::string(readBuffer);

                std::stringstream lineParse(lineString);
                std::vector<unsigned int> faceVertexSet;
                std::vector<unsigned int> faceNormalSet;
                std::vector<unsigned int> faceTexCoordSet;

                while (!lineParse.eof()){
                    unsigned int vertexID;
                    unsigned int normalID;
                    unsigned int texCoordID;

                    lineParse >> vertexID;
                    lineParse.get();
                    if (lineParse.eof())
                        break;

                    lineParse >> texCoordID;
                    lineParse.get();
                    if (lineParse.eof())
                        break;

                    lineParse >> normalID;

                    faceVertexSet.push_back(vertexID-1);
                    faceNormalSet.push_back(normalID-1);
                    faceTexCoordSet.push_back(texCoordID-1);
                }

                if (faceVertexSet.size() > 2){
                    faceVertices.push_back(faceVertexSet);
                    faceNormals.push_back(faceNormalSet);
                    faceTexCoords.push_back(faceTexCoordSet);
                }

                break;
            }
            default:
                break;

        }
    }
}
void Cloth::renderObject(){

    float surfaceColour[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
    glColor4fv(surfaceColour);
    QImage fabricPic;
    fabricPic.load("fabric.jpeg");//Path of texture
    QImage picSwap =fabricPic.rgbSwapped().mirrored();
    unsigned char* imageData =picSwap.bits();
    int height= picSwap.height();
    int width= picSwap.width();

    GLuint texture_map;
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &texture_map);
    glBindTexture(GL_TEXTURE_2D, texture_map);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

    glBegin(GL_TRIANGLES);

        for (unsigned int face = 0; face < faceVertices.size(); face++){
            for (unsigned int triangle = 0; triangle < faceVertices[face].size() - 2; triangle++){
                for (unsigned int vertex = 0; vertex < 3; vertex++){
                    unsigned int faceVertex = 0;
                    if (vertex != 0)
                        faceVertex = triangle + vertex;
                    glNormal3f(normals[faceNormals[face][faceVertex]].x,normals[faceNormals[face][faceVertex]].y,normals[faceNormals[face][faceVertex]].z);

                    glTexCoord2f(textureCoords   [faceTexCoords  [face][faceVertex]  ].x,textureCoords   [faceTexCoords  [face][faceVertex]  ].y);

                    glVertex3f(vertices[faceVertices[face][faceVertex]].x,vertices[faceVertices[face][faceVertex]].y,vertices[faceVertices[face][faceVertex]].z);
                    }
                }
            }
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glDeleteTextures(1, &texture_map);
}
void Cloth::saveCurrentFrame(std::ostream &bufferStream){
    // output the vertex coordinates
    for (unsigned int vertex = 0; vertex < vertices.size(); vertex++)
        bufferStream << "v  " << std::fixed << vertices[vertex].x <<" "<< vertices[vertex].y <<" "<<vertices[vertex].z << std::endl;
    bufferStream << "# " << vertices.size() << " vertices" << std::endl;
    bufferStream << std::endl;

    // and the normal vectors
    for (unsigned int normal = 0; normal < normals.size(); normal++)
        bufferStream << "vn " << std::fixed << normals[normal].x <<" "<< normals[normal].y <<" "<< normals[normal].z << std::endl;
    bufferStream << "# " << normals.size() << " vertex normals" << std::endl;
    bufferStream << std::endl;

    // and the texture coordinates
    for (unsigned int texCoord = 0; texCoord < textureCoords.size(); texCoord++)
        bufferStream << "vt " << std::fixed << textureCoords[texCoord].x <<" "<< textureCoords[texCoord].y <<" "<< textureCoords[texCoord].z<< std::endl;
    bufferStream << "# " << textureCoords.size() << " texture coords" << std::endl;
    bufferStream << std::endl;

    // and the faces
    for (unsigned int face = 0; face < faceVertices.size(); face++){
        bufferStream << "f ";
        for (unsigned int vertex = 0; vertex < faceVertices[face].size(); vertex++)
            bufferStream << faceVertices[face][vertex]+1 <<
                              "/" << faceTexCoords[face][vertex]+1 << "/" << faceNormals[face][vertex]+1 <<
                              " " ;

        bufferStream << std::endl;
        }
    bufferStream << "# " << faceVertices.size() << " polygons" << std::endl;
    bufferStream << std::endl;

}
