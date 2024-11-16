#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include "Course.h"


Course* searchPrereqs(std::vector<Course*>, std::string);
int readCoursesData(std::vector<Course*>&, std::string);
void SetCategoryFromString(Course*, std::string);
QGraphicsEllipseItem* addCourseNode(QGraphicsScene*, Course*, int, int);
void addEdge(QGraphicsScene*, QGraphicsEllipseItem*, QGraphicsEllipseItem*);

int main(int argc, char *argv[])
{
    std::cout << "Hello." << std::endl;
}

int FAmain(int argc, char *argv[])
{
    std::cout << "Starting application" << std::endl;
    // Display Initialization
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

    std::cout << "Looking for file: " << std::filesystem::absolute("Catalog.dat") << std::endl;
    // Course Initialization
    std::vector<Course*> courses;
    std::cout << "Courses vector initialized" << std::endl;
    std::ifstream testFile("Catalog.dat");
    if (!testFile.is_open())
    {
        std::cout << "ERROR: Failed to open Catalog.dat" << std::endl;
        return -1;
    }
    testFile.close();
    if (readCoursesData(courses, "Catalog.dat") != 0)
    {
        std::cout << "ERROR: Failed to read course data" << std::endl;
        return -1;
    }
    else if (courses.empty())
    {
        std::cout << "ERROR: No courses found in Catalog.dat" << std::endl;
    }
    else 
    {
        std::cout << "Success! Course data read." << std::endl;
    }

    // Map courses to ellipse nodes
    std::map<Course*, QGraphicsEllipseItem*> nodeMap;
    int x = 50, y = 50;

    for (auto course : courses)
    {
        QGraphicsEllipseItem* node = addCourseNode(&scene, course, x, y);
        nodeMap[course] = node;
        x += 200;
        if (x > 1400) {x = 50; y += 100;}
    }

    // Connect Nodes with Edges
    for (auto course : courses) 
    {
        for (auto prereq : course->prerequisites)
        {
            addEdge(&scene, nodeMap[course], nodeMap[prereq]);
        }
    }

    view.show();
    
    int result = app.exec();
    for (auto course : courses)
    {
        delete course;
    }
    return result;    
}

int readCoursesData(std::vector<Course*>& courses, std::string filename)
{
    std::ifstream inFile;
    inFile.open(filename);
    if (!inFile.is_open())
    {
        std::cout << "ERROR: Failed to open file: " << filename << std::endl;
        return -1;
    }

    std::string line;
    while(std::getline(inFile, line))
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
                }
            }
        }
        courses.push_back(course);
    }
    inFile.close();
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

QGraphicsEllipseItem* addCourseNode(QGraphicsScene* scene, Course* course, int x, int y)
{
    QGraphicsEllipseItem* node = scene->addEllipse(x, y, 50, 50, QPen(Qt::black), QBrush(Qt::cyan));
    QGraphicsTextItem* label = scene->addText(QString::fromStdString(course->getName()));
    label->setPos(x + 10, y + 15);
    return node;
}

void addEdge(QGraphicsScene* scene, QGraphicsEllipseItem* startNode, QGraphicsEllipseItem* endNode)
{
    QPointF startCenter = startNode->rect().center() + startNode->pos();
    QPointF endCenter = endNode->rect().center() + endNode->pos();
    QGraphicsLineItem* edge = scene->addLine(QLineF(startCenter, endCenter), QPen(Qt::black));
}