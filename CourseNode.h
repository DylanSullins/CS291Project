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
    CourseNode(const QString& courseInfo, QGraphicsScene* scene, int x, int y, int width = 50, int height = 50)
    : QGraphicsEllipseItem(x, y, width, height), courseInfo(courseInfo), scene(scene) 
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
        //QRect textRect = infoBox->boundingRect().adjusted(-5, -5, 75, 5);
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
        //if (infoBox) return;
        QPointF mousePos = event->scenePos();
        //infoBox = new QGraphicsTextItem();
        //infoBox->setPlainText(courseInfo);
        //infoBox->setZValue(1);
        //QRectF textRect = infoBox->boundingRect();
        //QRectF paddedRect = textRect.adjusted(-5,-5,75,5);
        //scene->addItem(infoBox);

        //QGraphicsRectItem* background = new QGraphicsRectItem(paddedRect);
        //background->setBrush(QBrush(Qt::white));
        //background->setPen(QPen(Qt::black));
        background->setZValue(1);

        //scene->addItem(background);
        background->setPos(mousePos);
        infoBox->setParentItem(background);
        infoBox->setPos(5,5);
        //QFont font;
        //infoBox->setFont(font);
        //infoBox->setDefaultTextColor(Qt::black);
        background->show();
        infoBox->show();
    }

    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override 
    {
        if (infoBox)
        {
            background->hide();
            infoBox->hide();
        }
    }

private:
    QString courseInfo;
    QString titleQstr;
    QGraphicsScene* scene;
    QGraphicsTextItem* title;
    QGraphicsTextItem* infoBox;
    QGraphicsRectItem* background;
};