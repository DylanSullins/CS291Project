#include "Course.h"

Course::Course()
{
    name = "";
    num = 0;
    category = NONE;
    prerequisites.clear();
}

Course::Course(std::string nameInput, int numInput, Category categoryInput, std::vector<Course*> prerequisitesInput)
    : name(nameInput), num(numInput), category(categoryInput), prerequisites(prerequisitesInput) {}

Course::~Course()
{
    while(!prerequisites.empty())
    {
        Course* delCourse = prerequisites.back();
        prerequisites.pop_back();
        delete delCourse;
    }
}

std::string Course::getName(){return name;}

int Course::getNum(){return num;}

Category Course::getCategory(){return category;}

std::string Course::getCategoryName()
{
    switch(category)
    {
        case Category::COMP_SCI:
            return "CS";
        case Category::MATH:
            return "MATH";
        case Category::STATS:
            return "STATS";
        case Category::TEST:
            return "TEST";
        case Category::DEPT:
            return "DEPT";
        case Category::NONE:
            return "NONE";
        case Category::JUNIOR:
            return "JUNIOR";
        default:
            return "ERROR";
    }
}

QString Course::getShortTitle()
{
    QString title = QString::fromStdString(
        getCategoryName() + "\n" + std::to_string(getNum())
    );
    return title;
}

std::vector<QGraphicsLineItem*> Course::getEdges(){return edges;}

void Course::setName(std::string n){name = n;}
void Course::setNum(int n){num = n;}
void Course::setCategory(Category c){category = c;}

void Course::addPrereq(Course* prereq)
{
    prerequisites.push_back(prereq);
}

void Course::addEdge(QGraphicsLineItem* edge)
{
    edges.push_back(edge);
}

int Course::calculatePrereqHeight(std::vector<Course*>& prereqsSeen)
{
    if (prerequisites.empty()){return 0;}
    int prereqsCount = 0;
    for (Course* prereq : prerequisites)
    {
        bool found = false;
        for (Course* seenPrereq : prereqsSeen) {
            if (prereq == seenPrereq)
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            ++prereqsCount;
            prereqsSeen.push_back(prereq);
        }
        if (!prereq->prerequisites.empty())
        {
            prereqsCount += prereq->calculatePrereqHeight(prereqsSeen);
        }
    }
    return prereqsCount;
}

int Course::calculatePrereqHeight()
{
    std::vector<Course*> prereqsSeen = {};
    return calculatePrereqHeight(prereqsSeen);
}

