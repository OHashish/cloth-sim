#include "clothwindow.h"
#include "cloth.h"
#include <QDebug>
#include <iostream>
#include <QToolBar>
#include <QPushButton>
#include <QWindow>
#include <QTextEdit>
#include <QLineEdit>
// constructor / destructor
ClothWindow::ClothWindow(QWidget *parent)
    : QWidget(parent)
    { // constructor



    // create menu bar

//    QString fileName = QFileDialog::getOpenFileName(this);

//    string fileNameChar=fileName.toStdString();
//    qDebug()<<fileNameChar;


    // create the window layout
    windowLayoutMain = new QVBoxLayout(this);

    windowLayoutUI = new QHBoxLayout(this);

    windowLayouttest= new QHBoxLayout(this);


    // create main widget
    string fileNameChar ="triangle_backplane.obj";

    Cloth* reader = new Cloth(NULL,fileNameChar,10);


    windowLayoutMain->addWidget(reader);
    windowLayoutMain->addLayout(windowLayoutUI);
    windowLayoutUI->addLayout(windowLayouttest);

//    QLineEdit *text = new QLineEdit(this);
//    QAction *myAction = text->addAction(QIcon("/Users/omarhashish/Desktop/tim.jpg"),QLineEdit::TrailingPosition);
//    connect(myAction,SLOT(tri(released()), reader, SLOT(loadObject()));
//    text->setMaximumWidth(90);
//    windowLayoutUI->addWidget(text);


    QPushButton *loadbutton = new QPushButton("Load",this);
    connect(loadbutton,SIGNAL(released()), reader, SLOT(loadObject()));

    windowLayoutUI->addWidget(loadbutton);

    QPushButton *playPause = new QPushButton("Save Video (Series of images)",this);
    connect(playPause,SIGNAL(released()), reader, SLOT(playPause()));

    windowLayoutUI->addWidget(playPause);

    QPushButton *saveButton = new QPushButton("Save new .obj file",this);
    connect(saveButton,SIGNAL(released()), reader, SLOT(saveFile()));

    windowLayoutUI->addWidget(saveButton);

    QPushButton *SS1 = new QPushButton("SS1",this);
    connect(SS1,SIGNAL(released()), reader, SLOT(SS1()));

    windowLayoutUI->addWidget(SS1);

    QPushButton *Rotate = new QPushButton("SS1 Rotate",this);
    connect(Rotate,SIGNAL(released()), reader, SLOT(SS1Rotate()));

    windowLayoutUI->addWidget(Rotate);

    QPushButton *SS2 = new QPushButton("SS2",this);
    connect(SS2,SIGNAL(released()), reader, SLOT(SS2()));

    windowLayoutUI->addWidget(SS2);

    QPushButton *Wind = new QPushButton("SS2 Wind",this);
    connect(Wind,SIGNAL(released()), reader, SLOT(SS2Wind()));

    windowLayoutUI->addWidget(Wind);
//    QSlider *otherSpeedSlider = new QSlider(Qt::Horizontal);
//    QDoubleSpinBox *otherSpeedSlider =  new QDoubleSpinBox;
//    otherSpeedSlider->setMaximumWidth(80);
//    otherSpeedSlider->setMaximumHeight(80);
//    otherSpeedSlider->setMinimum(-100);
//    otherSpeedSlider->setMaximum(100);
//    windowLayouttest->addWidget(otherSpeedSlider);
//    connect(otherSpeedSlider, SIGNAL(valueChanged(double)),  reader  , SLOT(saveVideo(double)));

    //Slider
    QSlider *px = new QSlider(Qt::Vertical);
    px->setMaximumWidth(80);
    px->setMaximumHeight(80);
    px->setMinimum(-360);
    px->setMaximum(360);
    windowLayouttest->addWidget(px);
    connect(px, SIGNAL(valueChanged(int)),  reader  , SLOT(posX(int)));

    //Slider
    QSlider *py = new QSlider(Qt::Vertical);
    py->setMaximumWidth(80);
    py->setMaximumHeight(80);
    py->setMinimum(-360);
    py->setMaximum(360);
    windowLayouttest->addWidget(py);
    connect(py, SIGNAL(valueChanged(int)),  reader  , SLOT(posY(int)));

    //Slider
    QSlider *pz = new QSlider(Qt::Vertical);
    pz->setMaximumWidth(80);
    pz->setMaximumHeight(80);
    pz->setMinimum(-360);
    pz->setMaximum(360);
    windowLayouttest->addWidget(pz);
    connect(pz, SIGNAL(valueChanged(int)),  reader  , SLOT(posZ(int)));

    //Slider
    QDoubleSpinBox *rx = new QDoubleSpinBox;
    rx->setMaximumWidth(80);
    rx->setMaximumHeight(80);
    rx->setMinimum(-100);
    rx->setMaximum(100);
    windowLayouttest->addWidget(rx);
    connect(rx, SIGNAL(valueChanged(double)),  reader  , SLOT(rotX(double)));

    //Slider
    QDoubleSpinBox *ry = new QDoubleSpinBox;
    ry->setMaximumWidth(80);
    ry->setMaximumHeight(80);
    ry->setMinimum(-100);
    ry->setMaximum(100);
    windowLayouttest->addWidget(ry);
    connect(ry, SIGNAL(valueChanged(double)),  reader  , SLOT(rotY(double)));

    //Slider
    QDoubleSpinBox *rz = new QDoubleSpinBox;
    rz->setMaximumWidth(80);
    rz->setMaximumHeight(80);
    rz->setMinimum(-100);
    rz->setMaximum(100);
    windowLayouttest->addWidget(rz);
    connect(rz, SIGNAL(valueChanged(double)),  reader  , SLOT(rotZ(double)));

    ptimer = new QTimer(this);

    ptimer->start(20);
    connect(ptimer, SIGNAL(timeout()),  reader, SLOT(updateTime()));

    } // constructor

ClothWindow::~ClothWindow()
    { // destructor
    delete ptimer;

    } // destructor

void ClothWindow::handleButton(){
//    ClothWindow* window = new ClothWindow(NULL);
qDebug()<<"BENAS";

}
