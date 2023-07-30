#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-register"

#pragma clang diagnostic pop
#include "cloth.h"
#include <iostream>
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
#include <cmath>
#include <QGLWidget>
#include <QApplication>
#include <QFileDialog>
#include <QWindow>
#include "clothwindow.h"
#include <QProcess>
int main(int argc, char **argv){
    QApplication app(argc, argv);

    ClothWindow* window = new ClothWindow(NULL);
    window->resize(1400, 1000);

    // show the window
    window->show();

    // set QT running
    return app.exec();

}
