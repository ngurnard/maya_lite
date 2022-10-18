#ifndef MYGL_H
#define MYGL_H

#include <openglcontext.h>
#include <utils.h>
#include <shaderprogram.h>
#include <scene/squareplane.h>
#include "camera.h"

#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include "mesh.h"
#include <QListWidgetItem>
#include "vertex_display.h"
#include "face_display.h"
#include "halfedge_display.h"


class MyGL
    : public OpenGLContext
{
    Q_OBJECT
private:
    SquarePlane m_geomSquare;// The instance of a unit cylinder we can use to render any cylinder
    ShaderProgram m_progLambert;// A shader program that uses lambertian reflection
    ShaderProgram m_progFlat;// A shader program that uses "flat" reflection (no shadowing at all)

    GLuint vao; // A handle for our vertex array object. This will store the VBOs created in our geometry classes.
                // Don't worry too much about this. Just know it is necessary in order to render geometry.

    Camera m_glCamera;

    Mesh m_mesh; // define the mesh in the mygl window

    Vertex *mp_selected_vertex; // member pointer to the selected vertex
    Face *mp_selected_face; // member pointer to the selected face
    HalfEdge *mp_selected_halfEdge; // member pointer to the selected half-edge
public:
    explicit MyGL(QWidget *parent = nullptr);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    // Have an instance of VertexDisplay that is
    // drawn in paintGL, and has VBO data representing
    // the position of the currently selected Vertex so
    // it can be drawn as a GL_POINTS
    // Mouse click events
    VertexDisplay m_vertDisplay;
    FaceDisplay m_faceDisplay;
    HalfEdgeDisplay m_heDisplay;

protected:
    void keyPressEvent(QKeyEvent *e);

signals:
    void sig_sendMesh(Mesh*);

public slots:
    void slot_loadObj(); // call the load obj function to set the mesh in mygl
    // Mouse click event slots
    void slot_selectVertex(QListWidgetItem*);
    void slot_selectFace(QListWidgetItem*);
    void slot_selectHE(QListWidgetItem*); // selecting a half edge in the gui
};


#endif // MYGL_H
