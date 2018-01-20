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
    connect(treeScene, SIGNAL(iconSelected(TreeGraphicsItem*)), this, SIGNAL(iconSelected(TreeGraphicsItem*)));
}

QSize TreeGraphicsView::sizeHint() const{
    return QSize(500, 400);
}

bool TreeGraphicsView::drawGraph(DataIconManager *rootData, bool allowDuplicates){
    if (treeScene->drawGraph(rootData, allowDuplicates)){
        return true;
    }
    return false;
}

void TreeGraphicsView::wheelEvent(QWheelEvent *event){
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    if (event->delta() > 0/* && currentScale < maxScale*/){
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

void TreeGraphicsView::contractAllBranches(){
    treeScene->rootIcon->setIsExpanded(false);
    treeScene->contractBranch(treeScene->rootIcon, true);
}

void TreeGraphicsView::expandAllBranches(){
    treeScene->expandBranch(treeScene->rootIcon, true);
    treeScene->rootIcon->reposition();
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

bool TreeGraphicsView::reconnectIcon(TreeGraphicsItem *oldIconParent, DataIconManager *dataToReplace, DataIconManager *replacementData, bool removeData){
    bool result = treeScene->reconnectIcon(oldIconParent, dataToReplace, replacementData, removeData);
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
