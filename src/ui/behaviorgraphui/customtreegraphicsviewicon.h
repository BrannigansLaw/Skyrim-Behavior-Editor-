#ifndef CUSTOMTREEGRAPHICSVIEWICON_H
#define CUSTOMTREEGRAPHICSVIEWICON_H

#include <QWidget>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QGraphicsItem>

#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/generators/hkbgenerator.h"

class CustomTreeGraphicsViewIcon: public QGraphicsItem
{
    friend class HkDataUI;
    friend class BehaviorGraphView;
    friend class CustomTreeGraphicsView;
    friend class ModifierGeneratorUI;
    friend class MainWindow;
public:
    CustomTreeGraphicsViewIcon(DataIconManager *d, const QString & s, CustomTreeGraphicsViewIcon *par = NULL);
    virtual ~CustomTreeGraphicsViewIcon();
    QRectF boundingRect() const;
    QString getName() const;
    void unhighlight();
    bool hasChildIcon(CustomTreeGraphicsViewIcon *child);
    CustomTreeGraphicsViewIcon* getChildIcon(HkxObject *child);
    void unselect();
    void setSelected(QGraphicsSceneMouseEvent *event = NULL);
    void setLinkCoordinates(const QLineF & line);
    bool isGranfatherParadox(CustomTreeGraphicsViewIcon *child) const;
    bool isGranfatherParadox(DataIconManager *child) const;
    bool isDescendant(CustomTreeGraphicsViewIcon *icon);
    void errorHighlight();
    CustomTreeGraphicsViewIcon* setParent(CustomTreeGraphicsViewIcon *newParent);
    CustomTreeGraphicsViewIcon* setParentReturnDuplicate(CustomTreeGraphicsViewIcon *newParent);
    void replaceChild(CustomTreeGraphicsViewIcon *childToReplace, CustomTreeGraphicsViewIcon *replacementChild);
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) Q_DECL_OVERRIDE;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
private:
    static void updateStaticMembers();
    bool getLastIconY(CustomTreeGraphicsViewIcon *parent, qreal &lastIconY);
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
    CustomTreeGraphicsViewIcon *parent;
    QList <CustomTreeGraphicsViewIcon *> children;
    QGraphicsLineItem *linkToParent;
};

#endif // CUSTOMTREEGRAPHICSVIEWICON_H
