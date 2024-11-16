#pragma once

#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QBrush>
#include <QString>
#include <QFont>

class CourseNode : public QGraphicsEllipseItem
{
public:
    CourseNode(const QString& courseInfo, QGraphicsScene* scene, int x, int y, int width = 50, int height = 50)
    : QGraphicsEllipseItem(x, y, width, height), courseInfo(courseInfo), scene(scene) {
        setBrush(QBrush(Qt::cyan));
        setPen(QPen(Qt::black));
        setAcceptHoverEvents(true);
        infoBox = nullptr;
    }
protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override 
    {
        infoBox = new QGraphicsTextItem();
        infoBox->setPlainText(courseInfo);
        infoBox->setZValue(1);
        QGraphicsRectItem* backgroundInit = scene->addRect(
            infoBox->boundingRect().adjusted(-5, -5, 5, 5),
            QPen(Qt::black),
            QBrush(Qt::yellow)
        );
        background = backgroundInit;
        background->setZValue(0);
        infoBox->setParentItem(background);
        QFont font;
        infoBox->setFont(font);
        infoBox->setDefaultTextColor(Qt::black);
        scene->addItem(infoBox);
        QPointF pos = this->scenePos();
        infoBox->setPos(pos.x() + rect().width() + 5, pos.y());
    }

    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override 
    {
        if (infoBox)
        {
            scene->removeItem(infoBox);
            scene->removeItem(background);
            delete infoBox;
            delete background;
            infoBox = nullptr;
            background = nullptr;
        }
    }

private:
    QString courseInfo;
    QGraphicsScene* scene;
    QGraphicsTextItem* infoBox;
    QGraphicsRectItem* background;
};