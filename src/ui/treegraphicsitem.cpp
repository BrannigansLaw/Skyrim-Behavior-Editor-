#include "treegraphicsitem.h"
#include "treegraphicsscene.h"
#include "dataiconmanager.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>

#include "src/utility.h"
#include "src/hkxclasses/hkxobject.h"

#define ITEM_WIDTH 500
#define ITEM_HEIGHT 50

#define MAX_NUM_GRAPH_ICONS 10000

TreeGraphicsItem::TreeGraphicsItem(TreeGraphicsItem *parent, DataIconManager *obj, int indexToInsert, Qt::GlobalColor color)
    : QGraphicsItem(parent),
      brushColor(color),
      itemData(obj),
      isExpanded(true),
      yCoordinate(0),
      path(new GraphicsPathItem),
      itemFont(QFont("Times", 16, QFont::Bold))
{
    int index = -1;
    QList <QGraphicsItem *> children;
    setFlags(QGraphicsItem::ItemIsSelectable);
    itemData->appendIcon(this);
    //name = itemData->getName();
    if (parentItem()){
        if (indexToInsert > -1 && indexToInsert < parentItem()->childItems().size()){
            children = parentItem()->childItems();
            for (int i = 0; i < children.size(); i++){
                children[i]->setParentItem(nullptr);
            }
            index = children.indexOf(this);
            children.insert(indexToInsert, this);
            if (index > indexToInsert){
                index++;
            }
            if (index < children.size()){
                children.removeAt(index);
            }
            for (int i = 0; i < children.size(); i++){
                children[i]->setParentItem(parent);
            }
        }
        setPosition(QPointF(boundingRect().width()*2, getYCoordinate()));
    }
}

TreeGraphicsItem::~TreeGraphicsItem(){
    if (!itemData->hasIcons()){
        itemData->removeObjectAt(-1);
    }
    itemData->removeIcon(this);
    if (scene()){
        scene()->removeItem(this);
        /*if (path->scene()){
            scene()->removeItem(path);
        }*/
    }
    delete path;
    setParentItem(nullptr);
}

QRectF TreeGraphicsItem::boundingRect() const{
    return QRectF(0,0,ITEM_WIDTH,ITEM_HEIGHT);
}

QRectF TreeGraphicsItem::branchExpandCollapseBox() const{
    return QRectF(boundingRect().width()*0.9, 0, boundingRect().width()*0.1, boundingRect().height());
}

void TreeGraphicsItem::setBrushColor(Qt::GlobalColor color){
    brushColor = color;
    scene()->update(boundingRect());
}

void TreeGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *){
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(Qt::black);
    painter->setBrush(brushColor);
    painter->drawRect(boundingRect());
    painter->setFont(itemFont);
    painter->drawText(boundingRect(), itemData->getName());
    if (!childItems().isEmpty()){
        if (!isExpanded){
            painter->setBrush(Qt::red);
        }else{
            painter->setBrush(Qt::blue);
        }
    }else{
        painter->setBrush(Qt::blue);
    }
    painter->drawRect(branchExpandCollapseBox());
}

void TreeGraphicsItem::setIconSelected(){
    brushColor = Qt::green;
    scene()->update(QRectF(scenePos(), scenePos() + QPointF(boundingRect().width(), boundingRect().height())));
}

void TreeGraphicsItem::unselect(){
    brushColor = Qt::gray;
    scene()->update(QRectF(scenePos(), scenePos() + QPointF(boundingRect().width(), boundingRect().height())));
}

void TreeGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    if (boundingRect().contains(event->pos())){
        if (branchExpandCollapseBox().contains(event->pos())){
            if (event->modifiers() == Qt::ShiftModifier){
                ((TreeGraphicsScene *)scene())->selectIcon(this, TreeGraphicsScene::EXPAND_CONTRACT_ALL);
            }else{
                ((TreeGraphicsScene *)scene())->selectIcon(this, TreeGraphicsScene::EXPAND_CONTRACT_ONE);
            }
        }else{
            ((TreeGraphicsScene *)scene())->selectIcon(this, TreeGraphicsScene::EXPAND_CONTRACT_ZERO);
        }
    }
}

void TreeGraphicsItem::setPosition(const QPointF &pos){
    setPos(pos);
    setPathToParent();
}

qreal TreeGraphicsItem::getYCoordinate(){
    QList <QGraphicsItem *> children;
    QList <QGraphicsItem *> tempList;
    if (parentItem()){
        yCoordinate = ((TreeGraphicsItem *)parentItem())->boundingRect().height()*2;
        children = parentItem()->childItems();
        while (!children.isEmpty()){
            if (children.first() != this){
                if (children.first()->isVisible()){
                    yCoordinate = yCoordinate + ((TreeGraphicsItem *)children.first())->boundingRect().height()*2;
                }
                tempList = children.first()->childItems();
                children.removeFirst();
                children = tempList + children;
            }else{
                return yCoordinate;
            }
        }
    }
    return 0;
}

bool TreeGraphicsItem::reorderChildren(){
    QList <QGraphicsItem *> children = childItems();
    QList <DataIconManager *> dataChildren = itemData->getChildren();
    //QList <DataIconManager *> reparentedData;
    for (int j = 0; j < dataChildren.size(); j++){
        for (int k = j + 1; k < dataChildren.size(); k++){
            if (dataChildren.at(j) == dataChildren.at(k)){
                dataChildren.removeAt(k);
            }
        }
    }
    for (int i = 0; i < children.size(); i++){
        children[i]->setParentItem(nullptr);
    }
    if (children.size() <= dataChildren.size()){//Fix this...
        for (int i = 0; i < dataChildren.size(); i++){
            for (int k = 0; k < children.size(); k++){
                if (((TreeGraphicsItem *)children.at(k))->itemData == dataChildren.at(i)/* && !reparentedData.contains(dataChildren.at(i))*/){
                    children[k]->setParentItem(this);
                    //reparentedData.append(dataChildren.at(i));
                    break;
                }
            }
        }
        reposition();
        return true;
    }else{
        return false;
    }
}

void TreeGraphicsItem::reposition(){
    QList <QGraphicsItem *> children = childItems();
    QList <QGraphicsItem *> tempList;
    TreeGraphicsItem *parent = nullptr;
    TreeGraphicsItem *child = nullptr;
    setPosition(QPointF(boundingRect().width()*2, getYCoordinate()));
    while (!children.isEmpty()){
        tempList.clear();
        if (children.first()->isVisible()){
            ((TreeGraphicsItem *)children.first())->setPosition(QPointF(boundingRect().width()*2, ((TreeGraphicsItem *)children.first())->getYCoordinate()));
            tempList = children.first()->childItems();
        }
        children.removeFirst();
        children = tempList + children;
    }
    parent = (TreeGraphicsItem *)parentItem();
    child = this;
    while (parent){
        children = parent->childItems();
        while (!children.isEmpty() && children.first() != child){
            children.removeFirst();
        }
        while (!children.isEmpty()){
            tempList.clear();
            if (children.first()->isVisible()){
                ((TreeGraphicsItem *)children.first())->setPosition(QPointF(boundingRect().width()*2, ((TreeGraphicsItem *)children.first())->getYCoordinate()));
                tempList = children.first()->childItems();
            }
            children.removeFirst();
            children = tempList + children;
        }
        child = parent;
        parent = (TreeGraphicsItem *)parent->parentItem();
    }
    scene()->update();
}

bool TreeGraphicsItem::isDataDescendant(TreeGraphicsItem *icon) const{
    if (icon){
        TreeGraphicsItem *parent = (TreeGraphicsItem *)parentItem();
        if (parent){
            if (parent->hasSameData(icon)){
                return true;
            }else{
                if (parent->isDataDescendant(icon)){
                    return true;
                }
            }
        }
    }
    return false;
}

bool TreeGraphicsItem::isDataDescendant(DataIconManager *data) const{
    if (data){
        TreeGraphicsItem *parent = (TreeGraphicsItem *)parentItem();
        if (parent){
            if (parent->hasSameData(data)){
                return true;
            }else{
                if (parent->isDataDescendant(data)){
                    return true;
                }
            }
        }
    }
    return false;
}

bool TreeGraphicsItem::isPrimaryIcon() const{
    if (!itemData->icons.isEmpty()){
        if (itemData->icons.first() == this || itemData->icons.size() < 2){
            return true;
        }
    }
    return false;
}

TreeGraphicsItem *TreeGraphicsItem::getPrimaryIcon() const{
    if (!itemData->icons.isEmpty()){
        return itemData->icons.first();
    }
    return nullptr;
}

TreeGraphicsItem * TreeGraphicsItem::setParent(TreeGraphicsItem *newParent, int indexToInsert){//Swap paths!!!
    int index = -1;
    QList <QGraphicsItem *> children;
    QGraphicsItem *oldParent = parentItem();
    if (oldParent){
        children = oldParent->childItems();
        for (int i = 0; i < children.size(); i++){
            children[i]->setParentItem(nullptr);
        }
        if (children.removeAll(this) != 1){
            CRITICAL_ERROR_MESSAGE("TreeGraphicsItem::setParent(): Error!!!");
        }
        for (int i = 0; i < children.size(); i++){
            children[i]->setParentItem(oldParent);
        }
        children.clear();
        if (newParent){
            children = newParent->childItems();
            index = newParent->getIndexofIconWithData(itemData);
            if (indexToInsert > -1 && indexToInsert < children.size()){
                for (int i = 0; i < children.size(); i++){
                    children[i]->setParentItem(nullptr);
                }
                if (index < children.size()){
                    children.removeAt(index);
                }
                children.insert(indexToInsert, this);
                for (int i = 0; i < children.size(); i++){
                    children[i]->setParentItem(newParent);
                }
            }else{
                for (int i = 0; i < children.size(); i++){
                    children[i]->setParentItem(newParent);
                }
                setParentItem(newParent);
            }
        }else{
            setParentItem(nullptr);
        }
        reposition();
    }else{
        CRITICAL_ERROR_MESSAGE("TreeGraphicsItem::setParent(): Error!!!");
    }
    return (TreeGraphicsItem *)oldParent;
}

TreeGraphicsItem * TreeGraphicsItem::reconnectToNextDuplicate(){
    return itemData->reconnectToNext();
}

bool TreeGraphicsItem::hasIcons() const{
    return itemData->hasIcons();
}

TreeGraphicsItem *TreeGraphicsItem::getNextChildAfter(TreeGraphicsItem *child){
    QList <QGraphicsItem *> children;
    int index = -1;
    children = childItems();
    index = children.indexOf(child) + 1;
    if (index < children.size()){
        return (TreeGraphicsItem *)children.at(index);
    }
    return nullptr;
}

bool TreeGraphicsItem::hasSameData(TreeGraphicsItem *icon) const{
    if (itemData == icon->itemData){
        return true;
    }
    return false;
}

bool TreeGraphicsItem::hasSameData(DataIconManager *data) const{
    if (itemData == data){
        return true;
    }
    return false;
}

int TreeGraphicsItem::getIconIndex(){
    return itemData->icons.indexOf(this);
}

//Returns the index that "this" should be inserted into it's associated DataIconManager's icon list...
/*int TreeGraphicsItem::determineInsertionIndex() const{
    QList <QGraphicsItem *> children;
    QList <QGraphicsItem *> tempList;
    TreeGraphicsItem *parent = nullptr;
    TreeGraphicsItem *child = (TreeGraphicsItem *)this;
    parent = (TreeGraphicsItem *)parentItem();
    while (parent){
        children = parent->childItems();
        for (int i = 0; i < children.size(); i++){
            if (children.at(i) == child){
                break;
            }else if (((TreeGraphicsItem *)children.at(i))->hasSameData((TreeGraphicsItem *)this)){
                return ((TreeGraphicsItem *)children.at(i))->getIconIndex();
            }
            children.removeAt(i);
        }
        while (!children.isEmpty()){
            tempList.clear();
            if (((TreeGraphicsItem *)children.first())->hasSameData((TreeGraphicsItem *)this)){
                return ((TreeGraphicsItem *)children.first())->getIconIndex();
            }
            children.removeFirst();
            children = tempList + children;
        }
        child = parent;
        parent = (TreeGraphicsItem *)parent->parentItem();
    }
    return -1;
}*/

//Returns the first child of "this" whose data is equal to "data"...
TreeGraphicsItem *TreeGraphicsItem::getChildWithData(DataIconManager *data){
    QList <QGraphicsItem *> children = childItems();
    for (int i = 0; i < children.size(); i++){
        if (((TreeGraphicsItem *)children.at(i))->itemData == data){
            return (TreeGraphicsItem *)children.at(i);
        }
    }
    return nullptr;
}

/*TreeGraphicsItem *TreeGraphicsItem::getChildWithData(DataIconManager *data, int index){
    //for when duplicates are allowed...
    return nullptr;
}*/

/*Returns the icon that is closest to "this" that references "data"...
TreeGraphicsItem *TreeGraphicsItem::getReplacementIcon(DataIconManager *data){
    QList <QGraphicsItem *> children;
    QList <QGraphicsItem *> tempList;
    TreeGraphicsItem *parent = nullptr;
    TreeGraphicsItem *child = nullptr;
    parent = this;
    while (parent){
        children = parent->childItems();
        for (int i = 0; i < children.size(); i++){
            if (children.at(i) == child){
                break;
            }else if (((TreeGraphicsItem *)children.at(i))->hasSameData(data)){
                return ((TreeGraphicsItem *)children.at(i));
            }
            children.removeAt(i);
        }
        while (!children.isEmpty()){
            tempList.clear();
            if (((TreeGraphicsItem *)children.first())->hasSameData(data)){
                return ((TreeGraphicsItem *)children.first());
            }
            children.removeFirst();
            children = tempList + children;
        }
        child = parent;
        parent = (TreeGraphicsItem *)parent->parentItem();
    }
    return nullptr;
}*/

TreeGraphicsItem *TreeGraphicsItem::getReplacementIcon(DataIconManager *data){
    QList <QGraphicsItem *> children;
    QList <QGraphicsItem *> tempList;
    TreeGraphicsItem *parent = nullptr;
    parent = static_cast<TreeGraphicsScene *>(scene())->rootIcon;
    if (parent){
        children = parent->childItems();
        while (!children.isEmpty()){
            if (((TreeGraphicsItem *)children.first())->hasSameData(data) && ((TreeGraphicsItem *)children.first()) != this){
                return ((TreeGraphicsItem *)children.first());
            }
            tempList = children.first()->childItems();
            children.removeFirst();
            children = tempList + children;
        }
    }
    return nullptr;
}

int TreeGraphicsItem::getIndexOfChild(TreeGraphicsItem *child) const{
    QList <QGraphicsItem *> children = childItems();
    if (!children.isEmpty()){
        return children.indexOf(child);
    }
    return -1;
}

int TreeGraphicsItem::getIndexofIconWithData(DataIconManager *data) const{
    QList <QGraphicsItem *> children = childItems();
    for (int i = 0; i < children.size(); i++){
        if (((TreeGraphicsItem *)children.at(i))->hasSameData(data)){
            return i;
        }
    }
    return -1;
}

bool TreeGraphicsItem::getIsExpanded() const{
    return isExpanded;
}

void TreeGraphicsItem::setIsExpanded(bool value){
    isExpanded = value;
}

void TreeGraphicsItem::setPathToParent(){
    if (parentItem()){
        QPainterPath newpath(scenePos());
        DataIconManager *ptr = static_cast<TreeGraphicsItem *>(parentItem())->itemData;
        if (ptr->getSignature() != HKB_BLENDER_GENERATOR && ptr->getSignature() != BS_BONE_SWITCH_GENERATOR && ptr->getSignature() != HKB_POSE_MATCHING_GENERATOR){
            newpath.lineTo(QPointF(scenePos().x() - boundingRect().width(), scenePos().y() - yCoordinate + boundingRect().height()));
        }else{
            newpath.cubicTo(QPointF(scenePos().x() - boundingRect().width()/2, scenePos().y() - yCoordinate),
                            QPointF(scenePos().x() - boundingRect().width()/2, scenePos().y() + yCoordinate),
                            QPointF(scenePos().x() - boundingRect().width(), scenePos().y() - yCoordinate + boundingRect().height()));
        }
        path->setPath(newpath);
    }
}

QList <TreeGraphicsItem *> TreeGraphicsItem::getAllIconsInBranch(TreeGraphicsItem *iconToFind) const{
    QList <TreeGraphicsItem *> list;
    TreeGraphicsItem *branchTip = (TreeGraphicsItem *)this;
    if (iconToFind){
        for (int i = 0; i < MAX_NUM_GRAPH_ICONS, branchTip; i++){
            if (branchTip->parentItem()){
                if (branchTip->parentItem()->childItems().contains(iconToFind)){
                    branchTip = nullptr;
                }else{
                    list.append((TreeGraphicsItem *)branchTip->parentItem());
                    branchTip = (TreeGraphicsItem *)branchTip->parentItem();
                }
            }else{
                branchTip = nullptr;
            }
        }
    }else{
        for (int i = 0; i < MAX_NUM_GRAPH_ICONS, branchTip; i++){
            if (branchTip->parentItem()){
                list.append((TreeGraphicsItem *)branchTip->parentItem());
                branchTip = (TreeGraphicsItem *)branchTip->parentItem();
            }else{
                branchTip = nullptr;
            }
        }
    }
    return list;
}

