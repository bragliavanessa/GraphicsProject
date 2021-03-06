#include "CCanvas.h"
#include "Base.h"
#include "Sphere.h"
#include "Circle.h"

using namespace std;

float alpha = 90;
int view = 0;
double turret_rot = 0;

struct camera {
    double x = 0;
    double y = 0;
    double z = 65;
    double dx = 0;
    double dy = 0;
    double dz = 64;
    double ux = 0;
    double uy = 1;
    double uz = 0;
} c;
//-----------------------------------------------------------------------------

void CCanvas::keyPressEvent(QKeyEvent *event) {
    std::cout << "Pressed " << event->key() << std::endl;
    double temp_x;
    double temp_z;
    double temp_y;
    double ang_x;
    double temp;
    std::cout<<c.x<<" "<<c.y<<" "<<c.z<<endl;
    std::cout<<c.dx<<" "<<c.dy<<" "<<c.dz<<endl;

    switch(event->key()){
    case 32: //space
        view++;
        view %= 3;
        break;
    case 83: //s
        temp = c.z;
        c.z += c.z - c.dz;
        c.dz += temp - c.dz;
        temp = c.x;
        c.x += c.x - c.dx;
        c.dx += temp - c.dx;
        temp = c.y;
        c.y += c.y - c.dy;
        c.dy += temp - c.dy;
        break;
    case 87: //w
        temp = c.z;
        c.z -= c.z - c.dz;//1
        c.dz -= temp - c.dz;
        temp = c.x;
        c.x -= c.x - c.dx;//2
        c.dx -= temp - c.dx;
        temp = c.y;
        c.y -= c.y - c.dy;//2
        c.dy -= temp - c.dy;
        break;
//    case 68: //d
//        c.x+=1;
//        c.dx+=1;
//        break;
//    case 65: //a
//        c.x-=1;
//        c.dx-=1;
//        break;
    case 16777235://up
        temp_y = c.y - c.dy;
        temp_z = c.z - c.dz;
        ang_x = std::atan2f(temp_y,temp_z) - 0.1;
        c.y = c.dy + std::sin(ang_x);
        c.z = c.dz + std::cos(ang_x);
        break;
    case 65:
    case 16777234://left
        temp_x = c.x - c.dx;
        temp_z = c.z - c.dz;
        ang_x = std::atan2f(temp_x,temp_z) + 0.1;
        c.x = c.dx + std::sin(ang_x);
        c.z = c.dz + std::cos(ang_x);
        break;
    case 16777237://down
        temp_y = c.y - c.dy;
        temp_z = c.z - c.dz;
        ang_x = std::atan2f(temp_y,temp_z) + 0.1;
        c.y = c.dy + std::sin(ang_x);
        c.z = c.dz + std::cos(ang_x);
        break;
    case 68:
    case 16777236://right
        temp_x = c.x - c.dx;
        temp_z = c.z - c.dz;
        ang_x = std::atan2f(temp_x,temp_z) - 0.1;
        c.x = c.dx + std::sin(ang_x);
        c.z = c.dz + std::cos(ang_x);
        break;
    }
}
void CCanvas::initializeGL()
{
    this->setFocusPolicy(Qt::StrongFocus);
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);			   // black background
    glClearDepth(1.0f);								   // depth buffer setup
    glEnable(GL_DEPTH_TEST);						   // enables depth testing
    glDepthFunc(GL_LEQUAL);							   // the type of depth testing to do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // really nice perspective calculations
    glShadeModel(GL_SMOOTH);

    // One light source
    glEnable(GL_LIGHTING);

    glEnable(GL_LIGHT0);
    /*
     * The position is transformed by the modelview matrix when glLightfv is called (just as if it were
     * a point), and it is stored in eye coordinates. If the w component of the position is 0.0,
     * the light is treated as a directional source. Diffuse and specular lighting calculations take
     * the light's direction, but not its actual position, into account, and attenuation is disabled.
     * Otherwise, diffuse and specular lighting calculations are based on the actual location of the
     * light in eye coordinates, and attenuation is enabled. The default position is (0,0,1,0); thus,
     * the default light source is directional, parallel to, and in the direction of the -z axis.
     */
    GLfloat lightpos[] = {10.0, 10.0, 5.0, 5.0};
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);



    /*
     * Before you can use the texture you need to initialize it by calling the setTexture() method.
     * Before you can use OBJ/PLY model, you need to initialize it by calling init() method.
     */
    textureTrain.setTexture();
    textureSky.setTexture();
    body_texture.setTexture();
    texturePlanet1.setTexture();
    texturePlanet2.setTexture();
    texturePlanet3.setTexture();
    skybox.init();
    modelTrain.init();
    modelTrain2.init();
    engine.init();
    turret.init();
    body.init();
    wing_left.init();
    wing_right.init();
    tail.init();
    logo.init();
}

//-----------------------------------------------------------------------------

void CCanvas::glPerspective(const GLdouble fovy, const GLdouble aspect, const GLdouble zNear, const GLdouble zFar)
{
    const GLdouble d = 1.0 / tan(fovy / 360.0 * PI);
    const GLdouble delta = zNear - zFar;

    GLdouble *mat = new GLdouble[16];

    mat[0] = d / aspect;
    mat[1] = 0.0;
    mat[2] = 0.0;
    mat[3] = 0.0;

    mat[4] = 0.0;
    mat[5] = d;
    mat[6] = 0.0;
    mat[7] = 0.0;

    mat[8]  = 0.0;
    mat[9]  = 0.0;
    mat[10] = (zNear + zFar) / delta;
    mat[11] = -1.0;

    mat[12] = 0.0;
    mat[13] = 0.0;
    mat[14] = 2.0 * zNear * zFar / delta;
    mat[15] = 0.0;

    glMultMatrixd(mat);

    delete[] mat;
}

void CCanvas::lookAt(const GLdouble eyeX,
                     const GLdouble eyeY,						// VP on the course slides
                     const GLdouble eyeZ,
                     const GLdouble centerX,
                     const GLdouble centerY,					// q on the course slides
                     const GLdouble centerZ,
                     const GLdouble upX,
                     const GLdouble upY,							// VUP on the course slides
                     const GLdouble upZ )
{
    Point3d VP(eyeX, eyeY, eyeZ);
    Point3d q(centerX, centerY, centerZ);
    Point3d VUP(upX, upY, upZ);
    Point3d VPN = VP-q;

    // From slide 5, Lecture 13
    Point3d z(VPN.normalized());            // z' = VPN / ||VPN||
    Point3d x((VUP ^ z).normalized());      // x' = VUP*z / ||VUP*z||
    Point3d y(z^x);                         // y' = z*x
    Point3d p(VP);                          // p' = VP

    GLdouble *mat = new GLdouble[16];

    // TODO: set up the LookAt matrix correctly!
    mat[0] = x.x();
    mat[1] = y.x();
    mat[2] = z.x();
    mat[3] = 0.0;

    mat[4] = x.y();
    mat[5] = y.y();
    mat[6] = z.y();
    mat[7] = 0.0;

    mat[8] = x.z();
    mat[9] = y.z();
    mat[10] = z.z();
    mat[11] = 0.0;

    mat[12] = -x*p;
    mat[13] = -y*p;
    mat[14] = -z*p;
    mat[15] = 1.0;

    glMultMatrixd(mat);

    delete[] mat;
}

void CCanvas::resizeGL(int width, int height)
{
    // set up the window-to-viewport transformation
    glViewport(0, 0, width, height);

    // vertical camera opening angle
    double beta = 60.0;

    // aspect ratio
    double gamma;
    if(height > 0) gamma = width / (double)height;
    else gamma = width;

    // front and back clipping plane at
    double n = -0.01;
    double f = -1000.0;

    // frustum corners
    // double t = -tan(beta * 3.14159 / 360.0) * n;
    // double b = -t;
    // double r = gamma * t;
    // double l = -r;

    // set projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // glFrustum(l,r, b,t, -n,-f);

    // alternatively, directly from alpha and gamma
    glPerspective(beta, gamma, -n, -f);
}

//-----------------------------------------------------------------------------

void CCanvas::setView(View _view) {
    switch(_view) {
    case Perspective:
        glTranslatef(0.f, 0.f, -95.0f);
        break;
    case Cockpit:
        // Maybe you want to have an option to view the scene from the train cockpit, up to you
        glRotatef(60, 0,1,0);
        glRotatef(10, 0,0,1);
        glTranslatef(0.f, 0.f, -65.0f);
        glRotatef(-90,0,1,0);
        glTranslatef(24, 0, -10);
        glRotatef(-alpha*100,0,1,0);
        glTranslatef(0.f,10.f,0.0f);
        break;
    case Free:
        lookAt(c.x,c.y,c.z,  c.dx,c.dy,c.dz,  c.ux,c.uy,c.uz);
        break;
    }
}

void CCanvas::paintGL()
{
    static camera c;
    const double RADIUS = 17.1f;
    const double INTERVAL = 0.01;
    static float tau=35;
    // clear screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLfloat amb[]  = {0.4f, 0.4f, 0.4f};
    GLfloat diff[] = {0.7f, 0.7f, 0.7f};
    GLfloat spec[] = {0.4f, 0.4f, 0.4f};
    GLfloat shin = 0.0001;
    // set model-view matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    lookAt(0,0,0,  0,0,-1,  0,1,0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Setup the current view
    switch(view){
    case 0:
        setView(View::Perspective);
        break;
    case 1:
        setView(View::Cockpit);
        break;
    case 2:
        setView(View::Free);
        break;
    }

    textureSky.bind();
    glPushMatrix();
    glRotated(0,0,1,0);
    glScaled(20,20,20);

    skybox.draw();
    glPopMatrix();
    glPushMatrix();
    //    glRotated(180+tau,0,1,0);
    glRotated(180,0,1,0);
    glScaled(20,20,20);

    skybox.draw();
    glPopMatrix();
    textureSky.unbind();

    // Drawing the object with texture
    //    textureTrain.bind();
    // You can stack new transformation matrix if you don't want
    // the previous transformations to apply on this object
    glPushMatrix();
    /*
     * Obtaining the values of the current modelview matrix
     *  GLfloat matrix[16];
     *  glGetFloatv (GL_MODELVIEW_MATRIX, matrix);
    */
    double x = RADIUS * cos(5);
    double y = RADIUS;
    double z = RADIUS * sin(5);

    //    glScaled(25,20,20);
    //    glTranslatef(0.f,-1.5f,-1.0f);

    //    double deltaX = z * cos(alpha) - x * sin(alpha);
    double deltaX = 0 + cos(alpha)*RADIUS;
    //    double deltaY = y * cos(alpha) - 20;
    double deltaY = -15.f + sin(alpha)*RADIUS;
    double deltaZ = -10; //+ cos(alpha)*RADIUS + sin(alpha)*RADIUS; //x * cos(alpha) + z * sin(alpha);
    glColor3f(0.5f, 0.5f, 0.5f);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shin);
    // Look at the ObjModel class to see how the drawing is done

    glScaled(2,2,2);
    glTranslatef(0.f,-10.f,0.0f);
    glRotatef(alpha*100,0,1,0);

    glTranslatef(20, 0, 0);
    glRotatef(180,0,1,0);

    body_texture.bind();
    body.draw();
    body_texture.unbind();



    glPushMatrix();
    glTranslatef(0.f,2.05f,1.4f);
    glRotatef(alpha*60,0,1,0);
    turret.draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.f,0.f,-8.2f);
    glRotatef(180, 0, 1, -0.1f);
    engine.draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.f, 2.98f, -7.2f);
    glRotatef(270,0,1,0);
    glRotatef(7,0,0,1);
    tail.draw();
    glTranslatef(0.81f,-0.46f,0);
    glScalef(1.12,1.12,1);
    glRotatef(alpha*30, 0, 0, 1);
    logo.draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2.8,0,0);
    glTranslatef(-0.2,-0.8,-5);
    glRotatef(95,0,1,0);
    glRotatef(185,0,0,1);
    glRotatef(-10, 50,1,0);
    glRotatef(-10,0,1,0);
    wing_left.draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-2.1,0,0);
    glTranslatef(-0.45,-0.9,-5.1);
    glRotatef(90,0,1,0);
    glRotatef(180,0,0,1);
    glRotatef(-22, 1,1,0);
    glRotatef(210,1,0,0);
    glRotatef(-30,0,1,0);
    glRotatef(0,0,0,1);
    wing_right.draw();
    glPopMatrix();

    tau+=1;

    alpha+=INTERVAL;
    // Look at the PlyModel class to see how the drawing is done
    /*
     * The models you load can have different scales. If you are drawing a proper model but nothing
     * is shown, check the scale of the model, your camera could be for example inside of it.
     */
    //glScalef(0.02f, 0.02f, 0.02f);
    //modelTrain2.draw();
    // Remove the last transformation matrix from the stack - you have drawn your last
    // object with a new transformation and now you go back to the previous one
    glPopMatrix();
    //    textureTrain.unbind();
    texturePlanet1.bind();
    glPushMatrix();
    glScaled(20,20,20);
    glTranslatef(0.f,-1.5f,0);
    glRotatef(-tau/10,0.f,1.f,0.f);
    Sphere s(200, 200);
    s.draw();
    glPopMatrix();
    texturePlanet1.unbind();

    textureTrain.bind();
    glPushMatrix();
    glScaled(10,10, 10);
    glTranslatef(12.f,5.f,-20.0f);
    glRotatef(-tau,0.f,1.f,0.f);
    Sphere planet1(40, 40);
    planet1.draw();
    glPopMatrix();
    textureTrain.unbind();

    texturePlanet2.bind();
    glPushMatrix();
    glScaled(10,10, 10);
    glTranslatef(-5.f,5.f,10.0f);
    glRotatef(-tau/5,0.f,1.f,0.f);
    Sphere planet2(40, 40);
    planet2.draw();
    glPopMatrix();
    texturePlanet2.unbind();

    texturePlanet3.bind();
    glPushMatrix();
    glTranslatef(-10.f,9.f,0.0f);
    glScaled(4,4, 4);
    glRotatef(-tau/10,0.f,1.f,0.f);
    Sphere planet3(40, 40);
    planet3.draw();
    glPopMatrix();
    texturePlanet3.unbind();

}
