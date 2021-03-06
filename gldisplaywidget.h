#ifndef GLDISPLAYWIDGET_H
#define GLDISPLAYWIDGET_H

#include <QGLWidget>
#include <QtWidgets>
#include <QTimer>
#include "mesh.h"
#include "testUtil.h"
#include <iostream>

class GLDisplayWidget : public QGLWidget
{
public:
    explicit GLDisplayWidget(QWidget *parent = 0);

    ~GLDisplayWidget();
    void initializeGL(); // The scene may be initialized in this function since the mesh is a data member...
    void paintGL(); // Display the scene Gl
    void resizeGL(int width, int height);
    void drawMesh();
    void setDisplayWireframe();
    void setDisplayPlain();
    void randomFlip();
    void flip(int id1, int id2);
    void highlightFace(int index);
    void randomFHighlight();
    void unmarkAll();
    void circulate(int i);
    void naiveInsert(double x, double y, double z);
    void computeVoronoi();
    void toggleVoronoi();
    void delaunize();
    void delaunayInsert(double x, double y, double z);
    void merge(int vertexId1, int vertexId2);
    void simplify(int k);
    void crust2D();
    void showCrust(bool show);
    int  getVertexCount();
    int  getFaceCount();
    std::string getMeshFile() const;
    void setMeshFile(std::string file);
    void loadMesh();
    void loadCurve();
protected:
    // Mouse Management
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:

    QTimer _timer; // To update the scene
    double _X, _Y ,_Z; // Translation
    float _angle; // Rotation

    QPoint _lastPosMouse; // To keep the last position of the mouse

    Mesh _mesh; // The object to be displayed, may be replaced by a scene if there are several objects....
    int displayMode = 0; // 0 : Plain Mode 1 : WireFrame
    std::string meshFile = "flat02.off"; // File used for save and load mesh

    /*QOpenGLShaderProgram program;
    QOpenGLShader *fragmentShader;
    QOpenGLShader *vertexShader;*/
    // Used for shaderDrawing (win performance)
    GLuint                    shaderProgram;
    GLuint                    vertexShader;
    GLuint                    fragmentShader;

    Circulator_on_faces debugCf;
    int axis;
    void initThetrahedron();
    void initQueenMesh();
    void initCubeMesh();
    void initPyramid();
    void init2DBBox();
    void initFlatMesh();

};

#endif // GLDISPLAYWIDGET_H
