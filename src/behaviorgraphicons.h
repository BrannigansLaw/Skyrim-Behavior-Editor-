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

    bool isGranfatherParadox(GeneratorIcon *child) const{
        GeneratorIcon *parentIcon = parent;
        while (parentIcon){
            if (parentIcon->data == child->data){
                return true;
            }
            parentIcon = parentIcon->parent;
        }
        return false;
    }

    bool isGranfatherParadox(HkObject *child) const{
        if (child == data.data()){
            return true;
        }
        GeneratorIcon *parentIcon = parent;
        while (parentIcon){
            if (parentIcon->data.data() == child){
                return true;
            }
            parentIcon = parentIcon->parent;
        }
        return false;
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

    GeneratorIcon* setParent(GeneratorIcon *newParent){
        if (!newParent){
            return NULL;
        }
        GeneratorIcon *oldParent = parent;
        int i;
        if (parent){
            i = parent->children.indexOf(this);
            parent->children.removeAll(this);
        }
        int index = newParent->children.indexOf(this);
        if (index == -1){
            if (i > 0){
                newParent->children.insert(i - 1, this);
            }else if (i == 0){
                newParent->children.insert(i, this);
            }else{
                newParent->children.append(this);
            }
        }else{
            newParent->children.replace(index, this);
        }
        parent = newParent;
        QLineF line(parent->pos().x() + 1.0*parent->boundingRect().width(), parent->pos().y() + 1.0*parent->boundingRect().height(),\
                    parent->pos().x() + 1.5*parent->boundingRect().width(), parent->boundingRect().height());
        if (linkToParent){
            linkToParent->setLine(line);
        }else{
            linkToParent = new QGraphicsLineItem(line);
        }
        return oldParent;
    }

    void replaceChild(GeneratorIcon *childToReplace, GeneratorIcon *replacementChild){
        if (!childToReplace || !replacementChild){
            return;
        }
        for (int i = 0; i < children.size(); i++){
            if (children.at(i) == childToReplace){
                children.replace(i, replacementChild);
                replacementChild->setParent(this);
                //children.removeLast();//set parent appends but we replaced the child earlier (order is important) so remove...
                childToReplace->setParent(replacementChild);
            }
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
