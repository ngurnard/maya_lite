//#include "skeleton.h"
//#include <QFileDialog>
//#include <QJsonDocument>
//#include <QJsonObject>
//#include <QJsonArray>

//Skeleton::Skeleton(OpenGLContext* context)
//    : Drawable(context)
//{};

//void Skeleton::clearSkeleton()
//{
//    // Need to delete the Skeleton essentially if loading a new json from the file dialog so it doesnt combine 2 jsons
//    this->skeleton.clear(); // clear removes all elements of a vector
//}

//uPtr<Joint> Skeleton::parseJson(QJsonObject &obj)
//{
//    uPtr<Joint> joint = mkU<Joint>();
//    joint->name = obj["name"].toString();
//    QJsonArray position = obj["pos"].toArray();
//    joint->pos.x = position[0].toDouble();
//    joint->pos.y = position[1].toDouble();
//    joint->pos.z = position[2].toDouble();
//    QJsonArray rotation = obj["rot"].toArray();
//    float angle = rotation[0].toDouble();
//    glm::vec3 axis = glm::vec3(rotation[1].toDouble(), rotation[2].toDouble(), rotation[3].toDouble());
//    joint->rot = glm::angleAxis(angle, axis);
//    QJsonObject child = obj["children"].toObject();
//    joint->children = parseJson(child);
//}

//void Skeleton::load_json(const QString filename)
//{
//    // clear skeleton
//    this->clearSkeleton();

//    QFile input_file(filename); // input file object

//    // Make sure the file is read correctly, else throw error
//    if (!input_file.exists())
//    {
//        std::cout << "JSON file read incorrectly in Skeleton::load_json" << std::endl;
//        throw "JSON file read incorrectly";
//    }

//    // It is recommend to take a look at the documentation for the QJsonDocument, QJsonObject, and QJsonArray classes.
//    // I followed this video to parse JSON files:
//    // https://www.youtube.com/watch?v=QNfdZY91t68&ab_channel=HuiYuan
//    if (input_file.open(QIODevice::ReadOnly | QIODevice::Text)) { // open as read only and as text
//        QString file_data = input_file.readAll(); // read the whole file into a QString
//        input_file.close(); // no longer need the file to be open
//        QJsonDocument document = QJsonDocument::fromJson(file_data.toUtf8()); // make a json document
//        QJsonObject object = document.object(); // make the document into an object

//        QJsonObject root = object["root"].toObject(); // turn the first key "root" into an object so we can iterate through it

//        // Make a recursive function in order to get all of the children correctly


//    }
//}

//void Skeleton::create()
//{

//}
