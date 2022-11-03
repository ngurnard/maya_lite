//#pragma once
//#include "joint.h"
//#include "drawable.h"
//#include "smartpointerhelp.h"
//#include <vector>

//// Skeleton is much like the mesh class
//class Skeleton : public Drawable {
//private:
//    std::vector<uPtr<Joint>> skeleton; // stores the faces

//public:

//    // Constructors/Destructors
//    Skeleton(OpenGLContext* context); // default constructor

//    void create() override; // Need to implemnent the create() virtual function from the drawable class

//    // need to load in the json to be drawn
//    void load_json(const QString filename);
//    uPtr<Joint> parseJson(QJsonObject &obj);

//    // Need to delete the mesh essentially if loading a new obj from the file dialog so it doesnt combine 2 objs
//    void clearSkeleton();

////    void updateJoint(Joint*);

//    //    GLenum drawMode() override;
//};
