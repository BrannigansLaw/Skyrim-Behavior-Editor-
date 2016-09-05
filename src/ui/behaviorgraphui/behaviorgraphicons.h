#ifndef BEHAVIORGRAPHICONS_H
#define BEHAVIORGRAPHICONS_H

#include <QWidget>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QGraphicsItem>

#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/generators/hkbgenerator.h"

class GeneratorIcon: public QGraphicsItem
{
    friend class HkDataUI;
    friend class BehaviorGraphView;
public:
    GeneratorIcon(const HkxObjectExpSharedPtr & d, const QString & s, GeneratorIcon *par = NULL);

    virtual ~GeneratorIcon(){
        //
    }

    QRectF boundingRect() const{
        return boundingRectangle;
    }

    QString getName() const{
        return name;
    }

    void unhighlight(){
        rGrad.setColorAt(1.0, Qt::black);
        textPen.setColor(Qt::white);
        if (scene()){
            scene()->update();
        }
    }

    GeneratorIcon* getChildIcon(HkxObject *child){
        for (int i = 0; i < children.size(); i++){
            if (children.at(i)->data.data() == child){
                return children.at(i);
            }
        }
        return NULL;
    }

    void unselect();

    void setSelected(QGraphicsSceneMouseEvent *event = NULL);

    /*void removeFromScene(){
        if (scene() && data.data()){
            hkbGenerator *gen = static_cast<hkbGenerator *>(data.data());
            if (parent && !parent->children.isEmpty()){
                parent->children.removeAll(this);
            }
            gen->icons.removeAll(this);
            if (gen->icons.isEmpty()){
                gen->unlink();
            }
            scene()->removeItem(this);
            scene()->removeItem(linkToParent);
        }
    }*/

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

    bool isGranfatherParadox(HkxObject *child) const{
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

    bool isDescendant(GeneratorIcon *icon){
        if (icon){
            for (int i = 0; i < children.size(); i++){
                if (children.at(i) == icon){
                    return true;
                }else{
                    if (children.at(i)->isDescendant(icon)){
                        return true;
                    }
                }
            }
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
            if (i >= 0){
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
    HkxObjectExpSharedPtr data;
    bool isExpanded;
    GeneratorIcon *parent;
    QList <GeneratorIcon *> children;
    QGraphicsLineItem *linkToParent;
};

#endif // BEHAVIORGRAPHICONS_H
