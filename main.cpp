#include <iostream>
#include <fstream>
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

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    std::vector<Course*> courses;
    if (readCoursesData(courses, "Catalog.dat") != 0)
    {
        std::cout << "ERROR: Failed to read course data." << std::endl;
        return -1;
    }
    for (auto course : courses)
    {
        std::cout << course->getName() << std::endl;
        if (!course->prerequisites.empty()) 
        {
            std::vector<Course*> prereqsSeen = {};
            std::cout << "\tPREREQS COUNT:" << course->calculatePrereqHeight(prereqsSeen) << std::endl;
            for (auto c : course->prerequisites)
            std::cout << '\t' << c->getName() << std::endl;
        }
        else
        {
            std::cout << "\tCourse has no prerequisites" << std::endl;
        }
    }
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