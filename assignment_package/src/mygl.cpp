#include "mygl.h"
#include <la.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>
#include <QFileDialog> // for the file selection button
#include <QMessageBox>
#include <QListWidgetItem>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "halfedge_display.h"

MyGL::MyGL(QWidget *parent)
    : OpenGLContext(parent),
      m_geomSquare(this),
      m_progLambert(this), m_progFlat(this), m_progSkeleton(this),
      m_glCamera(),
      m_mesh(this), // initialize the mesh
      mp_selected_vertex(nullptr), // member pointer to the selected face
      mp_selected_face(nullptr), // member pointer to the selected half-edge
      // Mouse click events
      mp_selected_halfEdge(nullptr),
      mp_selected_joint(nullptr),
      m_wireframe(this),
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
//    m_wireframe.create();

    // Create and set up the diffuse shader
    m_progLambert.create(":/glsl/lambert.vert.glsl", ":/glsl/lambert.frag.glsl");
    // Create and set up the flat lighting shader
    m_progFlat.create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");
    // Create and set up the skinning shader
    m_progSkeleton.create(":/glsl/skeleton.vert.glsl", ":/glsl/skeleton.frag.glsl");

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
    m_progSkeleton.setViewProjMatrix(viewproj);

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
    m_progSkeleton.setViewProjMatrix(m_glCamera.getViewProj());

    m_progLambert.setCamPos(m_glCamera.eye);

    //Create a model matrix. This one rotates the square by PI/4 radians then translates it by <-2,0,0>.
    //Note that we have to transpose the model matrix before passing it to the shader
    //This is because OpenGL expects column-major matrices, but you've
    //implemented row-major matrices.
    //glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-2,0,0)) * glm::rotate(glm::mat4(), 0.25f * 3.14159f, glm::vec3(0,1,0));
    glm::mat4 model = glm::mat4(1.0f); // identity matrix
    //Send the geometry's transformation matrix to the shader
    m_progLambert.setModelMatrix(model);
    m_progFlat.setModelMatrix(model);
    m_progSkeleton.setModelMatrix(model);
    //Draw the example sphere using our lambert shader
    //m_progLambert.draw(m_geomSquare);
//    m_progLambert.draw(m_mesh);

    // if the skeleton is bound (skinning), then use the shader for the skeleton instead of simply drawing the cow with lambert
    if (m_mesh.bound_to_skeleton) {
        std::cout << "I am drawing the skeleton VBO STUFF" << std::endl;
        // TODO: Recompute m_mesh.skeletonOverallTransforms
        m_mesh.skeletonOverallTransforms.clear(); // clear the previous overall transforms
        m_mesh.updateOverallTransforms(); // update the overall transformations
        m_progSkeleton.setOverallTransforms(m_mesh.skeletonOverallTransforms);
        // m_progSkeleton.setBindMats(m_mesh.skeletonBindMats); // DO NOT UPDATE THE BIND MATRICES
        m_progSkeleton.draw(m_mesh);
    } else {
        std::cout << "I am drawing the skeleton standard cow lambert VBO" << std::endl;
        m_progLambert.draw(m_mesh);
    }

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

    // draw the skeleton
    if (m_skeleton_root != NULL)
    {
        traverseSkeleton(this->m_skeleton_root, (this->m_skeleton_root->getOverallTransformation()));
    }

    glEnable(GL_DEPTH_TEST);

    //Now do the same to render the cylinder
    //We've rotated it -45 degrees on the Z axis, then translated it to the point <2,2,0>
    //model = glm::translate(glm::mat4(1.0f), glm::vec3(2,2,0)) * glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(0,0,1));
    //m_progLambert.setModelMatrix(model);
    //m_progLambert.draw(m_geomSquare);
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
    // QMessageBox::information(this, tr("Selected File Name"), filename);

    // convert the QString to a const char*
    std::string str = filename.toStdString();
    const char* my_str = str.c_str();
    std::cout << my_str << std::endl;

    this->m_mesh.load_obj(my_str); // this creates the mesh
    this->m_mesh.destroy();
    this->m_mesh.create();

    // Make sure to display all of the vertices, faces, and halfedges in the list widgets
    emit MyGL::sig_sendMesh(&m_mesh);

    this->update(); // Calls paintGL, among other things (taken from above in keyPressEvent)
}

uPtr<Joint> MyGL::parseJson(QJsonObject &obj)
{
    uPtr<Joint> joint = mkU<Joint>(obj["name"].toString());
    QJsonArray position = obj["pos"].toArray();
    joint->pos = glm::vec3(position[0].toDouble(), position[1].toDouble(), position[2].toDouble());
    QJsonArray rotation = obj["rot"].toArray();
    float angle = rotation[0].toDouble();
    glm::vec3 axis = glm::normalize(glm::vec3(rotation[1].toDouble(), rotation[2].toDouble(), rotation[3].toDouble()));
    joint->rot = glm::angleAxis(angle, axis);

    // Need to set the joint attibutes to be drawn correctly
    joint->setGeom(&m_wireframe); // make sure the geometry of the joint is set
    joint->geom->setJoint(joint.get()); // set the joint for the geomtry
    joint->geom->create(); // create the geometry so it can be drawn

    QJsonArray children = obj["children"].toArray(); // the children in the json are a list of child objects
    for (auto child : children){
        QJsonObject childObj = child.toObject(); // make the child into an object
        uPtr<Joint> childPtr = parseJson(childObj); // recurviely get its children
        childPtr->parent = joint.get(); // set the parent for each child node
        joint->addChild(childPtr);
    }
    return joint;
}

void MyGL::slot_loadJson()
{
    const QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Open JSON File"), // title of the file dialog (basically the window that pops up)
                "../", // Default file to open
                "JSON Files (*.json)" // filter for obj files
                );
    // QMessageBox::information(this, tr("Selected File Name"), filename);

    QFile input_file(filename); // input file object

    // Make sure the file is read correctly, else throw error
    if (!input_file.exists())
    {
        std::cout << "JSON file read incorrectly in Skeleton::load_json" << std::endl;
        throw "JSON file read incorrectly";
    }

    // It is recommend to take a look at the documentation for the QJsonDocument, QJsonObject, and QJsonArray classes.
    // I followed this video to parse JSON files:
    // https://www.youtube.com/watch?v=QNfdZY91t68&ab_channel=HuiYuan
    if (input_file.open(QIODevice::ReadOnly | QIODevice::Text)) { // open as read only and as text
        QString file_data = input_file.readAll(); // read the whole file into a QString
        input_file.close(); // no longer need the file to be open
        QJsonDocument document = QJsonDocument::fromJson(file_data.toUtf8()); // make a json document
        QJsonObject object = document.object(); // make the document into an object

        QJsonObject root = object["root"].toObject(); // turn the first key "root" into an object so we can iterate through it

        // Make a recursive function in order to get all of the children correctly
        m_skeleton_root = parseJson(root);
    }

    emit sig_sendSkeletonRoot(m_skeleton_root.get());
    update(); // calls paintGL
}

void MyGL::traverseSkeleton(uPtr<Joint> &j, glm::mat4 T){

    if (j->parent) // draw the bones
    {
        // display the bone in the skeleton
        HalfEdgeDisplay bone(this);
        bone.position.push_back(glm::vec4(j->parent->getOverallTransformation()[3]));
        bone.position.push_back(glm::vec4(j->getOverallTransformation()[3]));
        bone.create();
        m_progFlat.setModelMatrix(glm::mat4(1.f));
        m_progFlat.draw(bone);
    }

    T = j->getOverallTransformation();

    for (auto& child : j->children){
        traverseSkeleton(child, T);
    }

    if (j->geom)
    {
        if (j.get() == mp_selected_joint)
        {
            j->geom->highlighted = true;
            j->geom->create();
        } else {
            j->geom->highlighted = false;
            j->geom->create();
        }

        m_progFlat.setModelMatrix(T);
        m_progFlat.draw(*(j->geom));
    }
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

void MyGL::slot_selectJoint(QTreeWidgetItem *i)
{
    // Make it so only the selected displays
    this->mp_selected_vertex = NULL;
    this->mp_selected_face = NULL;
    this->mp_selected_halfEdge = NULL;

    this->mp_selected_joint = static_cast<Joint*>(i);
//    m_wireframe.create();
    this->update();

    emit sig_setFocus();
}

void MyGL::slot_splitHE()
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

void MyGL::slot_triangulate()
{
    if (this->mp_selected_face != nullptr)
    {
        m_mesh.triangulate(this->mp_selected_face);
        emit sig_sendMesh(&m_mesh);
        this->m_mesh.destroy();
        this->m_mesh.create();
        this->update();
    }

    emit sig_setFocus();
}

void MyGL::slot_modVertPosX(double xx)
{
    if (this->mp_selected_vertex != nullptr)
    {
        this->mp_selected_vertex->pos.x = xx;
        emit sig_sendMesh(&m_mesh);
        this->m_mesh.destroy();
        this->m_mesh.create();
        this->update();
    }

    emit sig_setFocus();
}

void MyGL::slot_modVertPosY(double yy)
{
    if (this->mp_selected_vertex != nullptr)
    {
        this->mp_selected_vertex->pos.y = yy;
        emit sig_sendMesh(&m_mesh);
        this->m_mesh.destroy();
        this->m_mesh.create();
        this->update();
    }

    emit sig_setFocus();
}

void MyGL::slot_modVertPosZ(double zz)
{
    if (this->mp_selected_vertex != nullptr)
    {
        this->mp_selected_vertex->pos.z = zz;
        emit sig_sendMesh(&m_mesh);
        this->m_mesh.destroy();
        this->m_mesh.create();
        this->update();
    }

    emit sig_setFocus();
}

void MyGL::slot_modJointPosX(double xx)
{
    if (mp_selected_joint != nullptr)
    {
        this->mp_selected_joint->pos.x = xx;
        this->update();
    }

    emit sig_setFocus();
}

void MyGL::slot_modJointPosY(double yy)
{
    if (mp_selected_joint != nullptr)
    {
        this->mp_selected_joint->pos.y = yy;
        this->update();
    }

    emit sig_setFocus();
}

void MyGL::slot_modJointPosZ(double zz)
{
    if (mp_selected_joint != nullptr)
    {
        this->mp_selected_joint->pos.z = zz;
        this->update();
    }

    emit sig_setFocus();
}

void MyGL::slot_modJointRotXPos()
{
    if (mp_selected_joint != nullptr)
    {
        this->mp_selected_joint->rot = glm::angleAxis(glm::radians(5.f), glm::vec3(1, 0, 0)) * this->mp_selected_joint->rot;
        this->update();
    }

    emit sig_setFocus();
}

void MyGL::slot_modJointRotYPos()
{
    if (mp_selected_joint != nullptr)
    {
        this->mp_selected_joint->rot = glm::angleAxis(glm::radians(5.f), glm::vec3(0, 1, 0)) * this->mp_selected_joint->rot;
        this->update();
    }

    emit sig_setFocus();
}

void MyGL::slot_modJointRotZPos()
{
    if (mp_selected_joint != nullptr)
    {
        this->mp_selected_joint->rot = glm::angleAxis(glm::radians(5.f), glm::vec3(0, 0, 1)) * this->mp_selected_joint->rot;
        this->update();
    }

    emit sig_setFocus();
}

void MyGL::slot_modJointRotXNeg()
{
    if (mp_selected_joint != nullptr)
    {
        this->mp_selected_joint->rot *= glm::angleAxis(glm::radians(-5.f), glm::vec3(1, 0, 0));
        this->update();
    }

    emit sig_setFocus();
}

void MyGL::slot_modJointRotYNeg()
{
    if (mp_selected_joint != nullptr)
    {
        this->mp_selected_joint->rot *= glm::angleAxis(glm::radians(-5.f), glm::vec3(0, 1, 0));
        this->update();
    }

    emit sig_setFocus();
}

void MyGL::slot_modJointRotZNeg()
{
    if (mp_selected_joint != nullptr)
    {
        this->mp_selected_joint->rot *= glm::angleAxis(glm::radians(-5.f), glm::vec3(0, 0, 1));
        this->update();
    }

    emit sig_setFocus();
}
void MyGL::slot_modFaceRed(double rr)
{
    if (this->mp_selected_face != nullptr)
    {
        this->mp_selected_face->color.r = rr;
        emit sig_sendMesh(&m_mesh);
        this->m_mesh.destroy();
        this->m_mesh.create();
        this->update();
    }

    emit sig_setFocus();
}

void MyGL::slot_modFaceGreen(double gg)
{
    if (this->mp_selected_face != nullptr)
    {
        this->mp_selected_face->color.g = gg;
        emit sig_sendMesh(&m_mesh);
        this->m_mesh.destroy();
        this->m_mesh.create();
        this->update();
    }

    emit sig_setFocus();
}

void MyGL::slot_modFaceBlue(double bb)
{
    if (this->mp_selected_face != nullptr)
    {
        this->mp_selected_face->color.b = bb;
        emit sig_sendMesh(&m_mesh);
        this->m_mesh.destroy();
        this->m_mesh.create();
        this->update();
    }

    emit sig_setFocus();
}

void MyGL::slot_subdivide()
{
    m_mesh.subdivide();
    emit sig_sendMesh(&m_mesh);
    this->m_mesh.destroy();
    this->m_mesh.create();
    this->update();
}

void MyGL::slot_skinning()
{
    m_mesh.bound_to_skeleton = false; // in case we want to reskin
    if (this->m_skeleton_root != NULL) // the the json is loaded in
    {
        m_mesh.bindToSkeleton(this->m_skeleton_root.get());
        m_progSkeleton.setBindMats(m_mesh.skeletonBindMats); // ONLY SET THE BIND MATRICES ONCE
        this->m_mesh.destroy();
        this->m_mesh.create();
        this->update();
    }
}

