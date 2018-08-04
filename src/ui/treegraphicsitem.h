#ifndef TREEGRAPHICSITEM_H
#define TREEGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QPainter>

class TreeGraphicsScene;
class DataIconManager;

class GraphicsPathItem: public QGraphicsPathItem
{
protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *){
        painter->setRenderHints(QPainter::Antialiasing);
        painter->setPen(QPen(QBrush(Qt::black), 2));
        painter->drawPath(path());
    }
};

class TreeGraphicsItem: public QGraphicsItem
{
    friend class TreeGraphicsScene;
    friend class DataIconManager;
    friend class HkDataUI;
    friend class BehaviorGraphView;
    friend class TreeGraphicsView;
public:
    TreeGraphicsItem(TreeGraphicsItem *parent, DataIconManager *obj, int indexToInsert = -1, Qt::GlobalColor color = Qt::gray);
    virtual ~TreeGraphicsItem();
    virtual QRectF boundingRect() const;
    virtual QRectF branchExpandCollapseBox() const;
    void setBrushColor(Qt::GlobalColor color);
    void setPenColor(Qt::GlobalColor color);
    bool reorderChildren();
    TreeGraphicsItem *getChildWithData(DataIconManager *data);
    bool isCircular(TreeGraphicsItem *itemtocheck) const;
protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) Q_DECL_OVERRIDE;
    virtual void setIconSelected();
    virtual void unselect();
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void setPosition(const QPointF &pos);
private:
    qreal getYCoordinate();
    void reposition();
    bool isDataDescendant(TreeGraphicsItem *icon) const;
    bool isDataDescendant(DataIconManager *data) const;
    bool isPrimaryIcon() const;
    TreeGraphicsItem * getPrimaryIcon() const;
    TreeGraphicsItem * setParent(TreeGraphicsItem *newParent, int indexToInsert = -1);
    TreeGraphicsItem *reconnectToNextDuplicate();
    bool hasIcons() const;
    TreeGraphicsItem * getNextChildAfter(TreeGraphicsItem *child);
    bool hasSameData(TreeGraphicsItem *icon) const;
    bool hasSameData(DataIconManager *data) const;
    int getIconIndex();
    //int determineInsertionIndex() const;
    //TreeGraphicsItem *getChildWithData(DataIconManager *data, int index);
    TreeGraphicsItem *getReplacementIcon(DataIconManager *data);
    int getIndexOfChild(TreeGraphicsItem *child) const;
    int getIndexofIconWithData(DataIconManager *data) const;
    bool getIsExpanded() const;
    void setIsExpanded(bool value);
    void setPathToParent();
    QVector <TreeGraphicsItem *> getAllIconsInBranch(TreeGraphicsItem *iconToFind = nullptr) const;
protected:
    Qt::GlobalColor brushColor;
    Qt::GlobalColor penColor;
    DataIconManager * itemData;
    bool isExpanded;
    qreal yCoordinate;
    GraphicsPathItem *path;
    QFont itemFont;
    //QString name;
    //TreeGraphicsItem *parent;
};

#endif // TREEGRAPHICSITEM_H
