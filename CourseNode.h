#pragma once

#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QGraphicsSceneHoverEvent>
#include <QTimer>
#include <QBrush>
#include <QString>
#include <QFont>

class CourseNode : public QGraphicsEllipseItem
{
public:
    CourseNode(const QString& courseInfo, QGraphicsScene* scene, 
                int x, int y, 
                std::map<Course*, std::vector<QGraphicsLineItem*>>& edgesMap,
                Course* coursePtr,
                int width = 50, int height = 50)
    : QGraphicsEllipseItem(x, y, width, height), courseInfo(courseInfo), scene(scene), nodeEdges(edgesMap), course(coursePtr)
    {
        setBrush(QBrush(Qt::cyan));
        setPen(QPen(Qt::black));
        setAcceptHoverEvents(true);
        QFont font;
        

        infoBox = new QGraphicsTextItem();
        infoBox->setPlainText(courseInfo);
        
        infoBox->setFont(font);
        infoBox->setDefaultTextColor(Qt::black);
        QRectF textRect = infoBox->boundingRect();
        QRectF paddedRect = textRect.adjusted(-5,-5,75,5);
        background = new QGraphicsRectItem(paddedRect);
        background->setBrush(QBrush(Qt::white));
        background->setPen(QPen(Qt::black));

        scene->addItem(background);
        scene->addItem(infoBox);
        background->hide();
        infoBox->hide();
    }
~CourseNode()
{
    delete background;
    delete infoBox;
}

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override 
    {
        if (nodeEdges.count(course) > 0) 
        {
            for (auto edge : nodeEdges[course])
            {
                QPen highlightPen(Qt::yellow);
                highlightPen.setWidth(2);
                edge->setPen(highlightPen);
            }
        }
        QPointF mousePos = event->scenePos();
        
        background->setZValue(1);
        background->setPos(mousePos);
        infoBox->setParentItem(background);
        infoBox->setPos(5,5);
        background->show();
        infoBox->show();
    }

    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override 
    {
        if (nodeEdges.count(course) > 0)
        {
            for (auto edge : nodeEdges[course])
            {
                QPen defaultPen(Qt::black);
                edge->setPen(defaultPen);
            }
        }
        if (infoBox)
        {
            background->hide();
            infoBox->hide();
        }
    }

private:
    Course* course;
    QString courseInfo;
    QString titleQstr;
    QGraphicsScene* scene;
    QGraphicsTextItem* title;
    QGraphicsTextItem* infoBox;
    QGraphicsRectItem* background;
    std::map<Course*, std::vector<QGraphicsLineItem*>>& nodeEdges;
};