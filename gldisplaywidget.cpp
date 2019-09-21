#include "gldisplaywidget.h"
#ifdef __APPLE__
#include <glu.h>
#else
#include <GL/glu.h>
#endif

#include "QDebug"
#include <iostream>
GLDisplayWidget::GLDisplayWidget(QWidget *parent) : QGLWidget(parent)
{
    // Update the scene
    connect(&_timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    // connect(pushButton,SIGNAL(released()),this, SLOT(onWireframe()));
    _timer.start(16);
}

void GLDisplayWidget::initThetrahedron()
{

    QVector<Vertex> vertexTab;
    QVector<Face> faceTab;

    vertexTab.push_back(Vertex(-0.5, -0.5, -0.5)); //0
    vertexTab.push_back(Vertex(0.5, -0.5, -0.5));  // 1
    vertexTab.push_back(Vertex(0, 0.5, -0.5));     // 2
    vertexTab.push_back(Vertex(0, -0.5, 0.5));     // 3

    faceTab.push_back(Face(0, 1, 2));
    faceTab.push_back(Face(1, 3, 2));
    faceTab.push_back(Face(3, 0, 2));
    faceTab.push_back(Face(0, 1, 3));

    vertexTab[0].setFaceIndex(1);
    vertexTab[1].setFaceIndex(2);
    vertexTab[2].setFaceIndex(3);
    vertexTab[3].setFaceIndex(0);

    faceTab[0].setNeibFace(1, 2, 3);
    faceTab[1].setNeibFace(0, 2, 3);
    faceTab[2].setNeibFace(0, 1, 3);
    faceTab[3].setNeibFace(0, 1, 2);

    _mesh.setMesh(vertexTab, faceTab);
    _mesh.defineNeighbourFaces();
}

void GLDisplayWidget::initPyramid()
{

    QVector<Vertex> vertexTab;
    QVector<Face> faceTab;

    vertexTab.push_back(Vertex(-0.5, 0.0, -0.5)); //0
    vertexTab.push_back(Vertex(0.5, 0.0, -0.5));  // 1
    vertexTab.push_back(Vertex(0.5, 0.0, 0.5));   // 2
    vertexTab.push_back(Vertex(-0.5, 0.0, 0.5));  // 3
    vertexTab.push_back(Vertex(0., 2.0, 0.));     // 4

    faceTab.push_back(Face(0, 2, 1));
    faceTab.push_back(Face(0, 3, 2));
    faceTab.push_back(Face(0, 1, 4));
    faceTab.push_back(Face(1, 4, 2));
    faceTab.push_back(Face(2, 4, 3));
    faceTab.push_back(Face(0, 3, 4));

    vertexTab[0].setFaceIndex(1);
    vertexTab[1].setFaceIndex(2);
    vertexTab[2].setFaceIndex(3);
    vertexTab[3].setFaceIndex(4);
    vertexTab[4].setFaceIndex(2);

    faceTab[0].setNeibFace(3, 2, 1);
    faceTab[1].setNeibFace(4, 0, 5);
    faceTab[2].setNeibFace(3, 5, 0);
    faceTab[3].setNeibFace(4, 0, 2);
    faceTab[4].setNeibFace(5, 1, 3);
    faceTab[5].setNeibFace(4, 2, 1);

    _mesh.setMesh(vertexTab, faceTab);
    _mesh.defineNeighbourFaces();
}

void GLDisplayWidget::init2DBBox()
{

    QVector<Vertex> vertexTab;
    QVector<Face> faceTab;

    vertexTab.push_back(Vertex(-0.5, -0.5, -0.5)); //0
    vertexTab.push_back(Vertex(0.5, -0.5, -0.5));  // 1
    vertexTab.push_back(Vertex(0, 0.5, -0.5));     // 2
    vertexTab.push_back(Vertex(0, -0.5, 0.5));     // 3

    faceTab.push_back(Face(0, 1, 2));
    faceTab.push_back(Face(1, 3, 2));
    faceTab.push_back(Face(3, 0, 2));
    faceTab.push_back(Face(0, 1, 3));

    vertexTab[0].setFaceIndex(1);
    vertexTab[1].setFaceIndex(2);
    vertexTab[2].setFaceIndex(3);
    vertexTab[3].setFaceIndex(0);

    _mesh.setMesh(vertexTab, faceTab);
    _mesh.defineNeighbourFaces();
}

void GLDisplayWidget::initQueenMesh()
{
    // If you are in qtCreator
    _mesh.meshWithFile("../meshcomputation/data/queen.off");
    // If you are at the root of the project
    //_mesh.meshWithFile("data/queen.off");
}

void GLDisplayWidget::initCubeMesh()
{
    // If you are in qtCreator
    _mesh.meshWithFile("../meshcomputation/data/cube.off");
    // If you are at the root of the project
    //_mesh.meshWithFile("data/cube.off");
}

void GLDisplayWidget::initializeGL()
{
    // background color
    glClearColor(0.2, 0.2, 0.2, 1);

    // Shader
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    // Construction of the mesh before it is displayed
    // To add....
    //initPyramid();
    //initQueenMesh();
    initCubeMesh();
    Mesh titi;
    titi.meshWithFile("../meshcomputation/data/cube.off");
    titi.printFaces();
    Iterator_on_vertices its;
    Circulator_on_faces cf;
    for (its = titi.v_begin(); its != titi.v_pend(); ++its)
    {
        std::cout << "Getting new incident face \n";
        Circulator_on_faces cfbegin = titi.incident_f(*its);
        int cmpt = 0;
        for (cf = cfbegin, ++cf; cf != cfbegin; cf++){
            cmpt++;
            //std::cout << cmpt << "\n";
        }
        std::cout << "valence of the vertex "<< cmpt << std::endl;
    }
}

void GLDisplayWidget::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Center the camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);

    // Translation
    glTranslated(_X, _Y, _Z);

    // Rotation
    glRotatef(_angle, 1.0f, 1.0f, 0.0f);

    // Color for your mesh
    glColor3f(0, 1, 0);

    drawMesh();
}

void GLDisplayWidget::resizeGL(int width, int height)
{

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

    updateGL();
}

// - - - - - - - - - - - - Mouse Management  - - - - - - - - - - - - - - - -
// When you click, the position of your mouse is saved
void GLDisplayWidget::mousePressEvent(QMouseEvent *event)
{
    if (event != NULL)
        _lastPosMouse = event->pos();
}

// Mouse movement management
void GLDisplayWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - _lastPosMouse.x();
    // int dy = event->y() - lastPosMouse.y();

    if (event != NULL)
    {
        _angle += dx;
        _lastPosMouse = event->pos();

        updateGL();
    }
}

// Mouse Management for the zoom
void GLDisplayWidget::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta();
    double stepZoom = 0.25;
    if (!numDegrees.isNull())
    {
        _Z = (numDegrees.x() > 0 || numDegrees.y() > 0) ? _Z + stepZoom : _Z - stepZoom;
    }
}

void GLDisplayWidget::setDisplayWireframe()
{
    displayMode = 1;
}

void GLDisplayWidget::setDisplayPlain()
{
    displayMode = 0;
}
void GLDisplayWidget::drawMesh()
{
    if (displayMode == 0)
    {
        _mesh.drawMesh();
    }
    else if (displayMode == 1)
    {
        _mesh.drawMeshWireFrame();
    }
}
