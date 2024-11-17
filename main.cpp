#include <iostream>
#include <fstream>
#include <sstream>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QString>
#include "Course.h"
#include "CourseNode.h"


Course* searchPrereqs(std::vector<Course*>, std::string);
int readCoursesData(std::vector<Course*>&, std::ifstream&);
void SetCategoryFromString(Course*, std::string);
QGraphicsEllipseItem* addCourseNode(QGraphicsScene*, Course*, int, int, std::map<Course*, std::vector<QGraphicsLineItem*>>);
QGraphicsLineItem* addEdge(QGraphicsScene*, QGraphicsEllipseItem*, QGraphicsEllipseItem*);
std::string CategoryToString(Category);

int main(int argc, char *argv[])
{
    std::cout << "Starting application" << std::endl;
    // Display Initialization
    QApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
    QApplication app(argc, argv);
    std::cout << "QApplication initialized" << std::endl;
    QGraphicsScene scene;
    QGraphicsView view(&scene);
    view.setRenderHint(QPainter::Antialiasing);
    std::cout << "Set Render Hint" << std::endl;
    view.setWindowTitle("Course Prerequisites Graph");
    std::cout << "Set Window Title" << std::endl;
    view.resize(1600, 1200);
    std::cout << "Resizing" << std::endl;
    // Course Initialization
    std::vector<Course*> courses;
    std::cout << "Courses vector initialized" << std::endl;
    std::ifstream catalogFile("Catalog.dat");
    if (!catalogFile.is_open())
    {
        std::cout << "ERROR: Failed to open Catalog.dat" << std::endl;
        return -1;
    }
    else
    {
        std::cout << "Success! Catalog.dat Opened" << std::endl;
    }
    if (readCoursesData(courses, catalogFile) != 0)
    {
        std::cout << "ERROR: Failed to read course data" << std::endl;
        return -1;
    }
    else if (courses.empty())
    {
        std::cout << "ERROR: No courses found in Catalog.dat" << std::endl;
        return -1;
    }
    else 
    {
        std::cout << "Success! Course data read." << std::endl;
    }
    catalogFile.close();

    // Map courses to ellipse nodes
    std::map<Course*, QGraphicsEllipseItem*> nodeMap;
    std::map<Course*, std::vector<QGraphicsLineItem*>> nodeEdges;
    int x = 50, y = 50;
    std::cout << "Initialized Map" << std::endl;

    for (auto course : courses)
    {
        if (!course) continue;
        QGraphicsEllipseItem* node = addCourseNode(&scene, course, x, y, nodeEdges);
        nodeMap[course] = node;
        x += 200;
        if (x > 1400)
        {
            x = 50;
            y += 100;
        }
    }

    std::cout << "Mapped Courses to Nodes" << std::endl;

    // Connect Nodes with Edges
    for (auto course : courses) 
    {
        for (auto prereq : course->prerequisites)
        {
            QGraphicsLineItem* edge = addEdge(&scene, nodeMap[course], nodeMap[prereq]);
            nodeEdges[course].push_back(edge);
        }
    }

    int maxPrereqCount = 0;
    int minPrereqCount = 1;
    std::string maxPrereqCourseName;
    std::string minPrereqCourseName;
    for (auto course : courses)
    {
        int prereqCount = course->calculatePrereqHeight();
        if (prereqCount > maxPrereqCount) 
        {
            maxPrereqCount = prereqCount;
            maxPrereqCourseName = course->getName();
        }
        if (prereqCount < minPrereqCount) 
        {
            minPrereqCount = prereqCount;
            minPrereqCourseName = course->getName();
        }
    }
    std::cout << "MAX PREREQ: " << maxPrereqCourseName << " with " << maxPrereqCount << " prerequisistes." << std::endl;
    std::cout << "MIN PREREQ: " << minPrereqCourseName << " with " << minPrereqCount << " prerequisistes." << std::endl;
    

    view.show();
    
    int result = app.exec();
    for (auto course : courses)
    {
        delete course;
    }
    return result;
}

int readCoursesData(std::vector<Course*>& courses, std::ifstream& fin)
{
    std::string line;
    while(std::getline(fin, line))
    {
        std::vector<std::string> substrings;
        std::stringstream ss(line);
        std::string token;

        Course* course = new Course;

        while (std::getline(ss, token, ';'))
        {
            substrings.push_back(token);
        }
        course->setName(substrings[2]);
        course->setNum(std::stoi(substrings[1]));
        SetCategoryFromString(course, substrings[0]);

        if (substrings.size() > 3)
        {
            std::stringstream prereqsSS(substrings[3]);
            std::vector<std::string> prereqSubstrings;

            while (std::getline(prereqsSS, token, ','))
            {
                prereqSubstrings.push_back(token);
            }
            for (std::string reqString : prereqSubstrings)
            {
                Course* prereqPtr = searchPrereqs(courses, reqString);
                if (prereqPtr != nullptr){course->addPrereq(prereqPtr);}
                else
                {
                    std::cout << "ERROR: Prerequisite not found" << std::endl;
                    std::cout << "\tfor: " << course->getName() << std::endl;
                }
            }
        }
        courses.push_back(course);
    }
    return 0;
}

Course* searchPrereqs(std::vector<Course*> courses, std::string prereqKey)
{
    Category cat;
    int spacePos = prereqKey.find(' ');
    std::string catStr;
    int num;
    if (spacePos != -1)
    {
        catStr = prereqKey.substr(0, spacePos);
    
        num = std::stoi(prereqKey.substr(spacePos + 1));
    }
    else
    {
        catStr = prereqKey;
        num = 0;
    }
    if (catStr == "COMP-SCI"){cat = Category::COMP_SCI;}
    else if (catStr == "MATH"){cat = Category::MATH;}
    else if (catStr == "STATS"){cat = Category::STATS;}
    else if (catStr == "DEPT"){cat = Category::DEPT;}
    else if (catStr == "TEST"){cat = Category::TEST;}
    else if (catStr == "JUNIOR"){cat = Category::JUNIOR;}
    else{cat = Category::NONE;}


    for (Course* course : courses)
    {
        if (course->getCategory() == cat && course->getNum() == num)
        {
            return course;
        }
    }
    return nullptr;
}

void SetCategoryFromString(Course* course, std::string str)
{
    if (str == "COMP-SCI"){course->setCategory(Category::COMP_SCI);}
    else if (str == "MATH"){course->setCategory(Category::MATH);}
    else if (str == "STATS"){course->setCategory(Category::STATS);}
    else if (str == "TEST"){course->setCategory(Category::TEST);}
    else if (str == "DEPT"){course->setCategory(Category::DEPT);}
    else if (str == "JUNIOR"){course->setCategory(Category::JUNIOR);}
}

QGraphicsEllipseItem* addCourseNode(QGraphicsScene* scene, Course* course, int x, int y, std::map<Course*, std::vector<QGraphicsLineItem*>> edgeMap)
{
    if (!scene)
    {
        std::cout << "ERROR: QGraphicsScene is null" << std::endl;
        return nullptr;
    }

    if (!course)
    {
        std::cout << "ERROR: Course is null" << std::endl;
        return nullptr;
    }

    QString courseInfo = QString::fromStdString(
        CategoryToString(course->getCategory()) + " " + std::to_string(course->getNum()) +
        "\nCourse: " + course->getName() + "\nPrereqs: " + std::to_string(course->calculatePrereqHeight())
    );
    CourseNode* node = new CourseNode(courseInfo, scene, x, y, edgeMap, course);
    scene->addItem(node);
    return node;
}

QGraphicsLineItem* addEdge(QGraphicsScene* scene, QGraphicsEllipseItem* startNode, QGraphicsEllipseItem* endNode)
{
    
    QPointF endCenter = startNode->mapToScene(startNode->rect().center());
    QPointF startCenter = endNode->mapToScene(endNode->rect().center());

    QGraphicsLineItem* edge = scene->addLine(QLineF(startCenter, endCenter), QPen(Qt::black));
    QLineF line(startCenter, endCenter);
    double angle = std::atan2(line.dy(), line.dx());

    const double arrowSize = 10;

    QPointF arrowP1 = endCenter + QPointF(-std::cos(angle + M_PI / 4) * arrowSize,
                                          -std::sin(angle + M_PI / 4) * arrowSize);
    QPointF arrowP2 = endCenter + QPointF(-std::cos(angle - M_PI / 4) * arrowSize,
                                          -std::sin(angle - M_PI / 4) * arrowSize);
    QGraphicsLineItem* arrowLine1 = scene->addLine(QLineF(endCenter, arrowP1), QPen(Qt::black));
    QGraphicsLineItem* arrowLine2 = scene->addLine(QLineF(endCenter, arrowP2), QPen(Qt::black));
    return edge;
}

std::string CategoryToString(Category cat)
{
    switch(cat)
    {
        case Category::COMP_SCI:
            return "COMP_SCI";
        case Category::DEPT:
            return "DEPT";
        case Category::JUNIOR:
            return "JUNIOR";
        case Category::MATH:
            return "MATH";
        case Category::NONE:
            return "NONE";
        case Category::STATS:
            return "STATS";
        case Category::TEST:
            return "TEST";
        default:
            return "EMPTY";
    }
}