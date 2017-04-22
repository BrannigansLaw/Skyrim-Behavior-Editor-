#ifndef TREEGRAPHICSITEM_H
#define TREEGRAPHICSITEM_H

#include <QGraphicsItem>

class TreeGraphicsScene;
class DataIconManager;

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
protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) Q_DECL_OVERRIDE;
    virtual void setIconSelected();
    virtual void unselect();
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void setPosition(const QPointF &pos);
private:
    qreal getYCoordinate();
    bool reorderChildren();
    void reposition();
    bool isDescendant(TreeGraphicsItem *icon) const;
    bool isPrimaryIcon() const;
    TreeGraphicsItem * getPrimaryIcon() const;
    TreeGraphicsItem * setParent(TreeGraphicsItem *newParent, int indexToInsert = -1);
    TreeGraphicsItem *reconnectToNextDuplicate();
    bool hasIcons() const;
    TreeGraphicsItem * getNextChildAfter(TreeGraphicsItem *child);
    bool hasSameData(TreeGraphicsItem *icon) const;
    bool hasSameData(DataIconManager *data) const;
    int getIconIndex();
    int determineInsertionIndex() const;
    TreeGraphicsItem *getChildWithData(DataIconManager *data);
    //TreeGraphicsItem *getChildWithData(DataIconManager *data, int index);
    TreeGraphicsItem *getReplacementIcon(DataIconManager *data);
    int getIndexOfChild(TreeGraphicsItem *child) const;
    int getIndexofIconWithData(DataIconManager *data) const;
    bool getIsExpanded() const;
    void setIsExpanded(bool value);
    void setPathToParent();
    QList <TreeGraphicsItem *> getAllIconsInBranch(TreeGraphicsItem *iconToFind = NULL) const;
private:
    Qt::GlobalColor brushColor;
    DataIconManager * itemData;
    bool isExpanded;
    qreal yCoordinate;
    QGraphicsPathItem *path;
    QString name;
    TreeGraphicsItem *parent;
};

#endif // TREEGRAPHICSITEM_H
