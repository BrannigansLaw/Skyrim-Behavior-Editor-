#include "treegraphicsscene.h"
#include "treegraphicsitem.h"
#include "dataiconmanager.h"
#include "src/ui/hkxclassesui/behaviorui/behaviorgraphicon.h"
#include "src/ui/behaviorgraphview.h"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QScrollBar>

#define MAX_NUM_GRAPH_ICONS 10000

TreeGraphicsScene::TreeGraphicsScene()
    : rootIcon(NULL),
      selectedIcon(NULL),
      canDeleteRoot(false)
{
    setBackgroundBrush(QBrush(Qt::gray));
    setItemIndexMethod(QGraphicsScene::NoIndex);
}

void TreeGraphicsScene::setCanDeleteRoot(bool value){
    canDeleteRoot = value;
}

/*void TreeGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    QGraphicsScene::mouseReleaseEvent(event);
    QList <QGraphicsItem *> children;
    QPainterPath path(event->scenePos());
    path.addRect(event->scenePos().x(), event->scenePos().y(), 1, 1);
    setSelectionArea(path);
    children = selectedItems();
    if (!children.isEmpty() && !dynamic_cast<QGraphicsPathItem *>(children.first())){
        if (event->button() == Qt::RightButton){
            //Test...
            //addItemToGraph((TreeGraphicsItem *)children.first(), new DataIconManager(), 0);
            removeItemFromGraph((TreeGraphicsItem *)children.first(), -1);
            //Test End
        }
    }
    clearSelection();
}*/

void TreeGraphicsScene::selectIcon(TreeGraphicsItem *icon, bool expand){
    QList <TreeGraphicsItem *> branch;
    if (selectedIcon){
        selectedIcon->unselect();
    }
    selectedIcon = icon;
    if (selectedIcon){
        if (expand){
            if (selectedIcon->getIsExpanded()){
                contractBranch(selectedIcon);
                selectedIcon->setIsExpanded(false);
            }else{
                expandBranch(selectedIcon);
            }
            if (!selectedIcon->isPrimaryIcon()){
                selectedIcon = selectedIcon->getPrimaryIcon();
                if (!selectedIcon->isVisible()){
                    branch = selectedIcon->getAllIconsInBranch();
                    for (int i = 0; i < branch.size(); i++){
                        expandBranch(branch.at(i));
                    }
                }
                static_cast<BehaviorGraphView *>(views().first())->refocus();//May need to expand the branch???
            }
            selectedIcon->reposition();
        }
        selectedIcon->setIconSelected();
        emit iconSelected(selectedIcon);
    }
}

void TreeGraphicsScene::contractBranch(TreeGraphicsItem *icon, bool contractAll){
    QList <QGraphicsItem *> children;
    if (icon){
        children = icon->childItems();
        for (int i = 0; i < children.size(); i++){
            children.at(i)->setVisible(false);
            ((TreeGraphicsItem *)children.at(i))->path->setVisible(false);
            if (contractAll){
                ((TreeGraphicsItem *)children.at(i))->setIsExpanded(false);
                contractBranch((TreeGraphicsItem *)children.at(i), true);
            }else{
                contractBranch((TreeGraphicsItem *)children.at(i));
            }
        }
    }
    update();
}

void TreeGraphicsScene::expandBranch(TreeGraphicsItem *icon, bool expandAll){
    QList <QGraphicsItem *> children;
    if (icon){
        children = icon->childItems();
        icon->setIsExpanded(true);
        for (int i = 0; i < children.size(); i++){
            children.at(i)->setVisible(true);
            ((TreeGraphicsItem *)children.at(i))->path->setVisible(true);
            if (expandAll){
                expandBranch((TreeGraphicsItem *)children.at(i), true);
            }else if (((TreeGraphicsItem *)children.at(i))->getIsExpanded()){
                expandBranch((TreeGraphicsItem *)children.at(i));
            }
        }
    }
}

bool TreeGraphicsScene::drawGraph(DataIconManager *rootData, bool allowDuplicates){
    QList <DataIconManager *> objects;
    QList <DataIconManager *> children = rootData->getChildren();
    QList <TreeGraphicsItem *> parentIcons;
    QVector <short> numChildren;
    TreeGraphicsItem *newIcon = NULL;
    if (rootData){
        rootIcon = new BehaviorGraphIcon(NULL, rootData);
        addItem(rootIcon);
        objects.append(children);
        numChildren.append(children.size());
        parentIcons.append(rootIcon);
        while (!objects.isEmpty() && !numChildren.isEmpty() && !parentIcons.isEmpty()){
            newIcon = addItemToGraph(parentIcons.first(), objects.first(), 0, false, allowDuplicates, true);
            if (newIcon && newIcon->isDataDescendant(newIcon)){
                return false;
            }
            if (objects.first()->hasIcons()){
                children.clear();
            }else{
                children = objects.first()->getChildren();
            }
            numChildren.first()--;
            if (numChildren.first() == 0){
                numChildren.removeFirst();
                parentIcons.removeFirst();
            }
            objects.removeFirst();
            if (!children.isEmpty()){
                if (newIcon){
                    parentIcons.prepend(newIcon);
                    numChildren.prepend(children.size());
                    objects = children + objects;
                }
            }
        }
    }
    //setSceneRect(sceneRect().marginsAdded(QMarginsF(100, 100, 1000, 1000)));
    return true;
}

//Appends "data" to the 'itemData' of "selectedIcon" after creating a new icon representing "data" and appending it to "selectedIcon"...
TreeGraphicsItem * TreeGraphicsScene::addItemToGraph(TreeGraphicsItem *selectedIcon, DataIconManager *data, int indexToInsert, bool inject, bool allowDuplicates, bool isFirstDraw){
    TreeGraphicsItem *newIcon = NULL;
    TreeGraphicsItem *parent = ((TreeGraphicsItem *)selectedIcon->parentItem());
    QList <QGraphicsItem *> children;
    if (data){
        if (selectedIcon){
            if (!allowDuplicates){
                children = selectedIcon->childItems();
                for (int i = 0; i < children.size(); i++){
                    if (((TreeGraphicsItem *)children.at(i))->itemData == data){
                        if (!inject && !isFirstDraw){
                            selectedIcon->itemData->insertObjectAt(indexToInsert, data);
                        }else if (parent && inject){
                            selectedIcon->itemData->wrapObjectAt(indexToInsert, data, parent->itemData);
                            //selectedIcon->setParent(newIcon, newIcon->getIndexofIconWithData(selectedIcon->itemData));//Not sure...
                        }
                        return NULL;
                    }
                }
            }
        }
        if (!selectedIcon->isPrimaryIcon()){
            if (selectedIcon->getPrimaryIcon()){
                selectedIcon = selectedIcon->getPrimaryIcon();
            }else{
                return NULL;    //Error...
            }
        }
        if (!inject){
            if (isFirstDraw){
                newIcon = new BehaviorGraphIcon(selectedIcon, data, selectedIcon->getIndexofIconWithData(data));
            }else{
                newIcon = new BehaviorGraphIcon(selectedIcon, data, indexToInsert);
                selectedIcon->itemData->insertObjectAt(indexToInsert, data);
            }
        }else if (parent){
            newIcon = new BehaviorGraphIcon(parent, data, parent->getIndexofIconWithData(selectedIcon->itemData));
            selectedIcon->itemData->wrapObjectAt(indexToInsert, data, parent->itemData);
            selectedIcon->setParent(newIcon, newIcon->getIndexofIconWithData(selectedIcon->itemData));
        }else{
            delete newIcon;
            return NULL;
        }
        if (newIcon->isDataDescendant(newIcon)){
            delete newIcon;
            return NULL;
        }
        addItem(newIcon);  //newIcon is added to scene already???
        //newIcon->reposition();
        addItem(newIcon->path);
    }
    return newIcon;
}

bool TreeGraphicsScene::reconnectIcon(TreeGraphicsItem *oldIconParent, DataIconManager *dataToReplace, DataIconManager *replacementData, bool removeData){
    TreeGraphicsItem *iconToReplace = NULL;
    TreeGraphicsItem *replacementIcon = NULL;
    TreeGraphicsItem *oldParent = NULL;
    QList <DataIconManager *> dataChildren;
    //int indexOfReplacementData = -1;
    //int indexOfOldData = -1;
    int index = -1;
    if (oldIconParent){
        if (!dataToReplace){
            addItemToGraph(oldIconParent, replacementData, oldIconParent->itemData->getIndexOfObj(dataToReplace));
            oldIconParent->reposition();
            return true;
        }else if (dataToReplace != replacementData){
            iconToReplace = oldIconParent->getChildWithData(dataToReplace);
            if (replacementData == NULL){
                removeItemFromGraph(iconToReplace, oldIconParent->itemData->getIndexOfObj(dataToReplace), removeData);
                return true;
            }else if (replacementData->icons.isEmpty() || (!oldIconParent->isDataDescendant(replacementData) && !oldIconParent->hasSameData(replacementData))){
                dataChildren = oldIconParent->itemData->getChildren();
                //indexOfReplacementData = dataChildren.indexOf(replacementData);
                //indexOfOldData = dataChildren.indexOf(dataToReplace);
                replacementIcon = oldIconParent->getReplacementIcon(replacementData);
                index = oldIconParent->getIndexOfChild(iconToReplace);
                if (iconToReplace){
                    removeItemFromGraph(iconToReplace, oldIconParent->itemData->getIndexOfObj(dataToReplace), removeData, false, replacementData);
                }
                if (replacementIcon){
                    oldParent = replacementIcon->setParent(oldIconParent, index);
                    addItemToGraph(oldParent, replacementData, oldIconParent->itemData->getIndexOfObj(replacementData));
                    oldIconParent->itemData->insertObjectAt(oldIconParent->itemData->getIndexOfObj(dataToReplace), replacementData);
                }else{
                    addItemToGraph(oldIconParent, replacementData, oldIconParent->itemData->getIndexOfObj(dataToReplace));
                }
                if (!oldIconParent->reorderChildren()){
                    return false;
                }
                return true;
            }
        }
    }
    return false;
}

bool TreeGraphicsScene::removeItemFromGraph(TreeGraphicsItem *item, int indexToRemove, bool removeData, bool removeAllSameData, DataIconManager *dataToKeep){
    QList <QGraphicsItem *> children;   //Storage for all referenced icons in the branch whose root is "item"...
    QList <QGraphicsItem *> tempList;   //Storage for the children of the first icon stored in "children"...
    QList <QGraphicsItem *> iconsToRemove;  //Storage for all icons to be removed from the graph...
    TreeGraphicsItem *itemToDeleteParent = NULL;
    TreeGraphicsItem *itemToDelete = NULL;  //Represents any icons to be removed that had children that were adopted by another icon representing the same data...
    QList <DataIconManager *> childrenData;//Used to count the data references if less than 2 remove the icon...
    int dataCount = 0;
    TreeGraphicsItem *iconChild = NULL;
    int count = 0;  //Used to prevent possible infinite looping due to icons referencing ancestors...
    int index = -1; //Used to store the index of the position of "itemToDelete" in "children"...
    if (item){
        itemToDeleteParent = (TreeGraphicsItem *)item->parentItem();
        if (itemToDeleteParent){
            childrenData = itemToDeleteParent->itemData->getChildren();
            if (!childrenData.isEmpty()){
                dataCount = childrenData.count(item->itemData);
            }
            if (dataCount < 2 || removeAllSameData){
                children.append(item);
                for (; count < MAX_NUM_GRAPH_ICONS, !children.isEmpty(); count++){  //Start cycling through children...
                    itemToDelete = NULL;
                    iconChild = (TreeGraphicsItem *)children.first();
                    if (iconChild->hasSameData(dataToKeep)){
                        iconChild->setParent(itemToDeleteParent, itemToDeleteParent->getIndexofIconWithData(item->itemData));
                        tempList.clear();
                    }else{
                        tempList = iconChild->childItems();
                        if (!tempList.isEmpty() && iconChild->isPrimaryIcon() && iconChild->hasIcons()){  //"item" has children and has data that is referenced by other icons...
                            itemToDelete = iconChild->reconnectToNextDuplicate();   //Reconnect "item" to the parent of the next icon that references it's data...
                            index = children.indexOf(itemToDelete);
                            if (index != -1){//Problem here?
                                children.replace(index, iconChild); //"itemToDelete" is the
                            }
                            iconsToRemove.append(itemToDelete);
                            tempList.clear();
                        }else{
                            iconsToRemove.append(iconChild);
                        }
                    }
                    children.removeFirst();
                    children = tempList + children;
                }
                if (removeAllSameData){
                    for (int i = childrenData.size() - 1; i >= 0; i--){
                        if (childrenData.at(i) == item->itemData){
                            itemToDeleteParent->itemData->removeObjectAt(i);
                        }
                    }
                }else if (removeData){
                    itemToDeleteParent->itemData->removeObjectAt(indexToRemove);
                }
                for (int i = iconsToRemove.size() - 1; i >= 0; i--){//Duplicates sometimes...
                    delete iconsToRemove.at(i);
                }
                itemToDeleteParent->reposition();
            }else if (removeData){
                itemToDeleteParent->itemData->removeObjectAt(indexToRemove);
                if (!itemToDeleteParent->reorderChildren()){
                    return false;
                }
            }
        }else if (canDeleteRoot){  //Icon with no parent must be the root...
            delete item;
            rootIcon = NULL;
        }
        //itemToDeleteParent->reorderChildren();
        if (count < MAX_NUM_GRAPH_ICONS){
            return true;
        }
    }
    return false;
}


