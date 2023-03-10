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
#include "joint.h"
#include "joint_wireframe.h"


class MyGL
    : public OpenGLContext
{
    Q_OBJECT
private:
    SquarePlane m_geomSquare;// The instance of a unit cylinder we can use to render any cylinder
    ShaderProgram m_progLambert;// A shader program that uses lambertian reflection
    ShaderProgram m_progFlat;// A shader program that uses "flat" reflection (no shadowing at all)
    ShaderProgram m_progSkeleton; // A shader program that transofmrs vertices based on the joints that influence them (skinning)

    GLuint vao; // A handle for our vertex array object. This will store the VBOs created in our geometry classes.
                // Don't worry too much about this. Just know it is necessary in order to render geometry.

    Camera m_glCamera;

    Mesh m_mesh; // define the mesh in the mygl window

    Vertex *mp_selected_vertex; // member pointer to the selected vertex
    Face *mp_selected_face; // member pointer to the selected face
    HalfEdge *mp_selected_halfEdge; // member pointer to the selected half-edge
    Joint *mp_selected_joint;

    uPtr<Joint> m_skeleton_root; // the root of the skeleton
    JointWireframe m_wireframe; // what the joints should display like

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

    // need to load in the json to be drawn
    uPtr<Joint> parseJson(QJsonObject &obj);

    // Traverse the skeleton to be able to draw it
    void traverseSkeleton(uPtr<Joint> &j, glm::mat4 T);

protected:
    void keyPressEvent(QKeyEvent *e);

signals:
    void sig_sendMesh(Mesh*);
    void sig_sendSkeletonRoot(QTreeWidgetItem*);
    void sig_setFocus();

public slots:
    void slot_loadObj(); // call the load obj function to set the mesh in mygl
    void slot_loadJson(); // call the load json file to set up the skeleton
    // Mouse click event slots
    void slot_selectVertex(QListWidgetItem*);
    void slot_selectFace(QListWidgetItem*);
    void slot_selectHE(QListWidgetItem*); // selecting a half edge in the gui
    void slot_selectJoint(QTreeWidgetItem *i);
    // Slots for attribute modification
    void slot_splitHE();
    void slot_triangulate();
    void slot_modVertPosX(double xx); // to change the vertices with the spin boxes
    void slot_modVertPosY(double yy);
    void slot_modVertPosZ(double zz);
    void slot_modFaceRed(double xx); // to change the colors with the spin boxes
    void slot_modFaceGreen(double yy);
    void slot_modFaceBlue(double zz);
    void slot_modJointPosX(double xx); // to change the joint positions with the spin boxes
    void slot_modJointPosY(double yy);
    void slot_modJointPosZ(double zz);
    void slot_modJointRotXPos(); // to change the joint Rotations with the push buttons
    void slot_modJointRotYPos();
    void slot_modJointRotZPos();
    void slot_modJointRotXNeg(); // to change the joint Rotations with the push buttons
    void slot_modJointRotYNeg();
    void slot_modJointRotZNeg();

    void slot_subdivide(); // subdivision slot
    void slot_skinning(); // skinning button
};


#endif // MYGL_H
