#ifndef __GL_POLYGON_WINDOW_H__
#define __GL_POLYGON_WINDOW_H__ 1

#include <QGLWidget>
#include <QMenuBar>
#include <QSlider>
#include <QTimer>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QBoxLayout>
#include <QLabel>
#include <QComboBox>
#include "cloth.h"

class ClothWindow: public QWidget
{
    Q_OBJECT
public slots:
    void handleButton();
public:


    // constructor / destructor
    ClothWindow(QWidget *parent);
    ~ClothWindow();

    // window layout
    QBoxLayout *windowLayout;

    QVBoxLayout *windowLayoutMain;
    QHBoxLayout *windowLayoutUI;
    QHBoxLayout *windowLayouttest;



    // a timer
    QTimer *ptimer;

    // resets all the interface elements
    void ResetInterface();
};

#endif
