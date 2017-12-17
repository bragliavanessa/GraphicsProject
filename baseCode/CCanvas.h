/************************************************************************/
/* Guards                                                               */
/************************************************************************/
#ifndef CCANVAS_H
#define CCANVAS_H

#include <iostream>
#include <QtOpenGL>
#include <QGLWidget>
#include <QTimer>

#include "Base.h"
#include "texture.hpp"

#include "ObjModel.h"
#include "PlyModel.h"
#include "globals.h"

using namespace std;

/************************************************************************/
/* Canvas to draw                                                       */
/************************************************************************/
class CCanvas : public QGLWidget
{
  Q_OBJECT

public:
    explicit CCanvas(QWidget *parent = 0) : QGLWidget(parent),
        body_texture(global_path + "/../images/body.png"),
        textureTrain(global_path + "/../images/train1.jpg"),
        texturePlanet1(global_path + "/../images/earth.jpg"),
        texturePlanet2(global_path + "/../images/moon.png"),
        texturePlanet3(global_path + "/../images/pluton.png"),
        modelTrain(global_path + "/../images/ship.obj"),
        modelTrain2(global_path + "/../images/train.ply"),
        skybox(global_path + "/../images/skybox.obj"),
        body(global_path + "/../images/body_test.obj"),
        logo(global_path + "/../images/logo.obj"),
        tail(global_path + "/../images/tail.obj"),
        wing_left(global_path + "/../images/wing_left.obj"),
        wing_right(global_path + "/../images/wing_right.obj"),
        turret(global_path + "/../images/turret.obj"),
        engine(global_path + "/../images/engine.obj"),
        textureSky(global_path + "/../images/skybox.jpg")
    {
        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));
        timer->start(10);
    }

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

private:
    void lookAt(const GLdouble eyex,
                const GLdouble eyey,
                const GLdouble eyez,
                const GLdouble centerx,
                const GLdouble centery,
                const GLdouble centerz,
                const GLdouble upx,
                const GLdouble upy,
                const GLdouble upz);

    void glPerspective(const GLdouble fovy,
                       const GLdouble aspect,
                       const GLdouble zNear,
                       const GLdouble zFar);


    enum View {
        Perspective = 0,    // View the scene from a perspective (from above, from a side, or whatever)
        Cockpit             // View the scene from the train cockpit (if you want, or whatever other view)
    };

    void setView(View _view);

    // Models and textures
    Texture textureTrain;
    Texture body_texture;
    Texture textureSky;
    Texture texturePlanet1;
    Texture texturePlanet2;
    Texture texturePlanet3;
    // Model loaded from .obj format
    ObjModel modelTrain;
    ObjModel skybox;

    ObjModel body;
    ObjModel logo;
    ObjModel tail;
    ObjModel engine;
    ObjModel wing_left;
    ObjModel wing_right;
    ObjModel turret;

    // Model loaded from .ply format
    PlyModel modelTrain2;
};

#endif
