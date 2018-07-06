#include "treegraphicsview.h"
#include "treegraphicsscene.h"
#include "treegraphicsitem.h"

#include <QWheelEvent>
#include <QMouseEvent>

TreeGraphicsView::TreeGraphicsView(QMenu *menu)
    : popUpMenu(menu),
      treeScene(new TreeGraphicsScene()),
      //minScale(0.001),
      //maxScale(0.5),
      initScale(1),
      iconFocusScale(1.5),
      currentScale(1),
      scaleUpFactor(1.2),
      scaleDownFactor(0.8)
{
    setScene(treeScene);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    connect(treeScene, SIGNAL(iconSelected(TreeGraphicsItem*)), this, SIGNAL(iconSelected(TreeGraphicsItem*)), Qt::UniqueConnection);
}

QSize TreeGraphicsView::sizeHint() const{
    return QSize(500, 400);
}

bool TreeGraphicsView::drawGraphMT(DataIconManager *rootData, bool allowDuplicates, int &taskcount, std::mutex & mutex, std::condition_variable & conditionVar){
    bool value = treeScene->drawGraph(rootData, allowDuplicates);
    mutex.lock();
    taskcount--;
    conditionVar.notify_one();
    mutex.unlock();
    return value;
}

bool TreeGraphicsView::drawGraph(DataIconManager *rootData, bool allowDuplicates){
    return treeScene->drawGraph(rootData, allowDuplicates);
}

void TreeGraphicsView::wheelEvent(QWheelEvent *event){
    zoom(event->delta());
}

void TreeGraphicsView::zoom(int delta){
    if (delta > 0/* && currentScale < maxScale*/){
        scale(scaleUpFactor, scaleUpFactor);
        currentScale = currentScale*scaleUpFactor;
    }else/* if (currentScale > minScale)*/{
        scale(scaleDownFactor, scaleDownFactor);
        currentScale = currentScale*scaleDownFactor;
    }
}

void TreeGraphicsView::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton){
        setDragMode(QGraphicsView::ScrollHandDrag);
    }
    QGraphicsView::mousePressEvent(event);
}

void TreeGraphicsView::mouseReleaseEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton){
        setDragMode(QGraphicsView::NoDrag);
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void TreeGraphicsView::contractBranch(){
    if (getSelectedItem()){
        //getSelectedItem()->setIsExpanded(false);
        treeScene->contractBranch(getSelectedItem(), true);
        getSelectedItem()->reposition();
    }
}

void TreeGraphicsView::expandBranch(){
    if (getSelectedItem()){
        treeScene->expandBranch(getSelectedItem(), true);
        getSelectedItem()->reposition();
    }
}

void TreeGraphicsView::selectRoot(){
    treeScene->selectIcon(treeScene->rootIcon, TreeGraphicsScene::EXPAND_CONTRACT_ZERO);
}

TreeGraphicsItem *TreeGraphicsView::getSelectedItem() const{
    return treeScene->selectedIcon;
}

DataIconManager *TreeGraphicsView::getSelectedData() const{
    if (treeScene->selectedIcon){
        return treeScene->selectedIcon->itemData;
    }
    return nullptr;
}

bool TreeGraphicsView::reconnectIcon(TreeGraphicsItem *oldIconParent, DataIconManager *dataToReplace, int replaceindex, DataIconManager *replacementData, bool removeData){
    bool result = treeScene->reconnectIcon(oldIconParent, dataToReplace, replaceindex, replacementData, removeData);
    //oldIconParent->reposition();
    return result;
}

bool TreeGraphicsView::removeItemFromGraph(TreeGraphicsItem *item, int indexToRemove, bool removeData, bool removeAllSameData){
    if (item == treeScene->selectedIcon){
        treeScene->selectedIcon = nullptr;
    }
    return treeScene->removeItemFromGraph(item, indexToRemove, removeData, removeAllSameData);
}

TreeGraphicsItem *TreeGraphicsView::addItemToGraph(TreeGraphicsItem *selectedIcon, DataIconManager *data, int indexToInsert, bool inject, bool allowDuplicates){
    return treeScene->addItemToGraph(selectedIcon, data, indexToInsert, inject, allowDuplicates);
}
