#include "mygl.h"
#include <la.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>
#include <QFileDialog> // for the file selection button
#include <QMessageBox>
#include <QListWidgetItem>

MyGL::MyGL(QWidget *parent)
    : OpenGLContext(parent),
      m_geomSquare(this),
      m_progLambert(this), m_progFlat(this),
      m_glCamera(),
      m_mesh(this), // initialize the mesh
      mp_selected_vertex(nullptr), // member pointer to the selected vertex
      mp_selected_face(nullptr), // member pointer to the selected face
      mp_selected_halfEdge(nullptr), // member pointer to the selected half-edge
      // Mouse click events
      m_vertDisplay(this),
      m_faceDisplay(this),
      m_heDisplay(this)
{
    setFocusPolicy(Qt::StrongFocus);
}

MyGL::~MyGL()
{
    makeCurrent();
    glDeleteVertexArrays(1, &vao);
    m_geomSquare.destroy();
}

void MyGL::initializeGL()
{
    // Create an OpenGL context using Qt's QOpenGLFunctions_3_2_Core class
    // If you were programming in a non-Qt context you might use GLEW (GL Extension Wrangler)instead
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();

    // Set a few settings/modes in OpenGL rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Set the size with which points should be rendered
    glPointSize(5);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(0.5, 0.5, 0.5, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    glGenVertexArrays(1, &vao);

    //Create the instances of Cylinder and Sphere.
//    m_geomSquare.create();
    m_mesh.create();

    // Create and set up the diffuse shader
    m_progLambert.create(":/glsl/lambert.vert.glsl", ":/glsl/lambert.frag.glsl");
    // Create and set up the flat lighting shader
    m_progFlat.create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    glBindVertexArray(vao);
}

void MyGL::resizeGL(int w, int h)
{
    //This code sets the concatenated view and perspective projection matrices used for
    //our scene's camera view.
    m_glCamera = Camera(w, h);
    glm::mat4 viewproj = m_glCamera.getViewProj();

    // Upload the view-projection matrix to our shaders (i.e. onto the graphics card)

    m_progLambert.setViewProjMatrix(viewproj);
    m_progFlat.setViewProjMatrix(viewproj);

    printGLErrorLog();
}

//This function is called by Qt any time your GL window is supposed to update
//For example, when the function update() is called, paintGL is called implicitly.
void MyGL::paintGL()
{
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_progFlat.setViewProjMatrix(m_glCamera.getViewProj());
    m_progLambert.setViewProjMatrix(m_glCamera.getViewProj());
    m_progLambert.setCamPos(m_glCamera.eye);
    m_progFlat.setModelMatrix(glm::mat4(1.f));

    //Create a model matrix. This one rotates the square by PI/4 radians then translates it by <-2,0,0>.
    //Note that we have to transpose the model matrix before passing it to the shader
    //This is because OpenGL expects column-major matrices, but you've
    //implemented row-major matrices.
//    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-2,0,0)) * glm::rotate(glm::mat4(), 0.25f * 3.14159f, glm::vec3(0,1,0));
    glm::mat4 model = glm::mat4(1.0f); // identity matrix
    //Send the geometry's transformation matrix to the shader
    m_progLambert.setModelMatrix(model);
    //Draw the example sphere using our lambert shader
//    m_progLambert.draw(m_geomSquare);
    m_progLambert.draw(m_mesh);

    // Draw the vertices, faces, and halfedges selected
    glDisable(GL_DEPTH_TEST); // draw on top of mesh's triangles
    if (mp_selected_vertex != NULL)
    {
        m_progFlat.draw(m_vertDisplay);
    }
    if (mp_selected_face != NULL)
    {
        m_progFlat.draw(m_faceDisplay);
    }
    if (mp_selected_halfEdge != NULL)
    {
        m_progFlat.draw(m_heDisplay);
    }

    glEnable(GL_DEPTH_TEST);

    //Now do the same to render the cylinder
    //We've rotated it -45 degrees on the Z axis, then translated it to the point <2,2,0>
//    model = glm::translate(glm::mat4(1.0f), glm::vec3(2,2,0)) * glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(0,0,1));
//    m_progLambert.setModelMatrix(model);
//    m_progLambert.draw(m_geomSquare);
}


void MyGL::keyPressEvent(QKeyEvent *e)
{
    float amount = 2.0f;
    if(e->modifiers() & Qt::ShiftModifier){
        amount = 10.0f;
    }
    // http://doc.qt.io/qt-5/qt.html#Key-enum
    // This could all be much more efficient if a switch
    // statement were used
    if (e->key() == Qt::Key_Escape) {
        QApplication::quit();
    } else if (e->key() == Qt::Key_Right) {
        m_glCamera.RotateAboutUp(-amount);
    } else if (e->key() == Qt::Key_Left) {
        m_glCamera.RotateAboutUp(amount);
    } else if (e->key() == Qt::Key_Up) {
        m_glCamera.RotateAboutRight(-amount);
    } else if (e->key() == Qt::Key_Down) {
        m_glCamera.RotateAboutRight(amount);
    } else if (e->key() == Qt::Key_1) {
        m_glCamera.fovy += amount;
    } else if (e->key() == Qt::Key_2) {
        m_glCamera.fovy -= amount;
    } else if (e->key() == Qt::Key_W) {
        m_glCamera.TranslateAlongLook(amount);
    } else if (e->key() == Qt::Key_S) {
        m_glCamera.TranslateAlongLook(-amount);
    } else if (e->key() == Qt::Key_D) {
        m_glCamera.TranslateAlongRight(amount);
    } else if (e->key() == Qt::Key_A) {
        m_glCamera.TranslateAlongRight(-amount);
    } else if (e->key() == Qt::Key_Q) {
        m_glCamera.TranslateAlongUp(-amount);
    } else if (e->key() == Qt::Key_E) {
        m_glCamera.TranslateAlongUp(amount);
    } else if (e->key() == Qt::Key_R) {
        m_glCamera = Camera(this->width(), this->height());
    } else if (e->key() == Qt::Key_N) {
        if (mp_selected_halfEdge)
        {
            slot_selectHE(mp_selected_halfEdge->heNext);
        }
    } else if (e->key() == Qt::Key_M) {
        if (mp_selected_halfEdge)
        {
            slot_selectHE(mp_selected_halfEdge->heSym);
        }
    } else if (e->key() == Qt::Key_F) {
        if (mp_selected_halfEdge)
        {
            slot_selectFace(mp_selected_halfEdge->face);
        }
    } else if (e->key() == Qt::Key_V) {
        if (mp_selected_halfEdge)
        {
            slot_selectVertex(mp_selected_halfEdge->vert);
        }
    } else if (e->key() == Qt::Key_H) {

        if (amount == 2.0f) // just H
        {
            if (mp_selected_vertex)
            {
                slot_selectHE(mp_selected_vertex->halfEdge);
            }
        } else {
            if (mp_selected_face)
            {
                slot_selectHE(mp_selected_face->halfEdge);
            }
        }
    }
    m_glCamera.RecomputeAttributes();
    update();  // Calls paintGL, among other things
}

void MyGL::slot_loadObj()
{
    // https://www.youtube.com/watch?v=tKdfpA74HYY&ab_channel=ProgrammingKnowledge
    const QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Open OBJ File"), // title of the file dialog (basically the window that pops up)
                "../", // Default file to open
                "OBJ Files (*.obj)" // filter for obj files
                );
    QMessageBox::information(this, tr("Selected File Name"), filename);

    // convert the QString to a const char*
    std::string str = filename.toStdString();
    const char* my_str = str.c_str();
    std::cout << my_str << std::endl;

//    m_mesh = Mesh(this);
    this->m_mesh.load_obj(my_str); // this creates the mesh
    this->m_mesh.destroy();
    this->m_mesh.create();

    // Make sure to display all of the vertices, faces, and halfedges in the list widgets
    emit MyGL::sig_sendMesh(&m_mesh);

    this->update(); // Calls paintGL, among other things (taken from above in keyPressEvent)
}

void MyGL::slot_selectVertex(QListWidgetItem *i)
{
    // Make it so only the selected displays
    this->mp_selected_face = NULL;
    this->mp_selected_halfEdge = NULL;

    this->mp_selected_vertex = static_cast<Vertex*>(i); // static_cast converts from QListWidgetItem ptr to Vertex ptr
    m_vertDisplay.updateVertex(mp_selected_vertex);
    m_vertDisplay.create();
    this->update();

    emit sig_setFocus();
}

void MyGL::slot_selectFace(QListWidgetItem *i)
{
    // Make it so only the selected displays
    this->mp_selected_vertex = NULL;
    this->mp_selected_halfEdge = NULL;

    this->mp_selected_face = static_cast<Face*>(i);
    m_faceDisplay.updateFace(mp_selected_face);
    m_faceDisplay.create();
    this->update();

    emit sig_setFocus();
}

void MyGL::slot_selectHE(QListWidgetItem *i)
{
    // Make it so only the selected displays
    this->mp_selected_vertex = NULL;
    this->mp_selected_face = NULL;

    this->mp_selected_halfEdge = static_cast<HalfEdge*>(i);
    m_heDisplay.updateHalfEdge(mp_selected_halfEdge);
    m_heDisplay.create();
    this->update();

    emit sig_setFocus();
}

void MyGL::slot_triangulate()
{
    if (this->mp_selected_halfEdge != nullptr)
    {
        m_mesh.splitHE(this->mp_selected_halfEdge);
        emit sig_sendMesh(&m_mesh);
        this->m_mesh.destroy();
        this->m_mesh.create();
        this->update();
    }

    emit sig_setFocus();
}
