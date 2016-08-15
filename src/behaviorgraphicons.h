#ifndef BEHAVIORGRAPHICONS_H
#define BEHAVIORGRAPHICONS_H

#include <QWidget>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QGraphicsItem>

#include "hkobject.h"

class GeneratorIcon: public QGraphicsItem
{
    friend class HkDataUI;
    friend class BehaviorGraphView;
public:
    GeneratorIcon(const HkObjectExpSharedPtr & d, const QString & s, GeneratorIcon *par = NULL);
    QRectF boundingRect() const{
        return boundingRectangle;
    }
    QString getName() const{
        return name;
    }
    void setLinkCoordinates(const QLineF & line){
        if (linkToParent){
            linkToParent->setLine(line);
        }
    }
    void errorHighlight(){
        if (pen.color() == Qt::black){
            pen.setColor(Qt::red);
        }else{
            pen.setColor(Qt::black);
        }
        if (scene()){
            scene()->update();
        }
    }
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) Q_DECL_OVERRIDE;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
private:
    static void updateStaticMembers();
    bool getLastIconY(GeneratorIcon *parent, qreal &lastIconY);
    void updatePosition(bool updateNonVisable = false);
private:
    static QRectF boundingRectangle;
    static QRectF button;
    static QRectF textRec;
    static QFont font;
    QPen pen;
    static QBrush brush;
    static QBrush buttonBrush;
    static QLineF vert;
    static QLineF horiz;
    static QRectF ellipse;
    static QPolygonF polygon;
    static QRectF square;
    static QPolygonF arrowHead;
    QRadialGradient rGrad;
    QPen textPen;
    QPainterPath path;
    QString name;
    HkObjectExpSharedPtr data;
    bool isExpanded;
    GeneratorIcon *parent;
    QList <GeneratorIcon *> children;
    QGraphicsLineItem *linkToParent;
};

#endif // BEHAVIORGRAPHICONS_H
