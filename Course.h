#pragma once
#include <string>
#include <vector>
#include <QString>
#include <QGraphicsLineItem>

enum Category
{
    NONE,
    COMP_SCI,
    MATH,
    STATS,
    DEPT,
    JUNIOR,
    TEST
};

class Course
{
protected:
    std::string name;
    int num;
    Category category;
    std::vector<QGraphicsLineItem*> edges;

public:

    std::vector<Course*> prerequisites;
    Course();
    Course(std::string, int, Category, std::vector<Course*>);

    ~Course();

    std::string getName();
    int getNum();
    Category getCategory();
    std::string getCategoryName();
    QString getShortTitle();
    std::vector<QGraphicsLineItem*> getEdges();

    void setName(std::string);
    void setNum(int);
    void setCategory(Category);

    void addPrereq(Course*);
    void addEdge(QGraphicsLineItem*);

    int calculatePrereqHeight(std::vector<Course*>&);
    int calculatePrereqHeight();
};