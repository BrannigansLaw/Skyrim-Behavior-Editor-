#include "treegraphicsview.h"
#include "treegraphicsscene.h"

#include <QWheelEvent>
#include <QMouseEvent>

TreeGraphicsView::TreeGraphicsView(QMenu *menu)
    : popUpMenu(menu),
      treeScene(new TreeGraphicsScene()),
      minScale(0.001),
      maxScale(4),
      initScale(1),
      iconFocusScale(1.5),
      currentScale(1),
      scaleUpFactor(1.2),
      scaleDownFactor(0.8)
{
    setScene(treeScene);
}

bool TreeGraphicsView::drawGraph(DataIconManager *rootData, bool allowDuplicates){
    if (treeScene->drawGraph(rootData, allowDuplicates)){
        return true;
    }
    return false;
}

void TreeGraphicsView::wheelEvent(QWheelEvent *event){
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    if (event->delta() > 0 && currentScale < maxScale){
        scale(scaleUpFactor, scaleUpFactor);
        currentScale = currentScale*scaleUpFactor;
    }else if (currentScale > minScale){
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
    treeScene->contractBranch(treeScene->rootIcon, true);
}

void TreeGraphicsView::expandAllBranches(){
    treeScene->expandBranch(treeScene->rootIcon, true);
}

void TreeGraphicsView::selectRoot(){
    treeScene->selectIcon(treeScene->rootIcon, false);
}

TreeGraphicsItem *TreeGraphicsView::getSelectedItem() const{
    return treeScene->selectedIcon;
}

bool TreeGraphicsView::reconnectIcon(TreeGraphicsItem *oldIconParent, DataIconManager *dataToReplace, DataIconManager *replacementData){
    return treeScene->reconnectIcon(oldIconParent, dataToReplace, replacementData);
}

bool TreeGraphicsView::removeItemFromGraph(TreeGraphicsItem *item, int indexToRemove){
    if (item == treeScene->selectedIcon){
        treeScene->selectedIcon = NULL;
    }
    return treeScene->removeItemFromGraph(item, indexToRemove);
}

TreeGraphicsItem *TreeGraphicsView::addItemToGraph(TreeGraphicsItem *selectedIcon, DataIconManager *data, int indexToInsert, bool inject, bool allowDuplicates){
    return treeScene->addItemToGraph(selectedIcon, data, indexToInsert, inject, allowDuplicates);
}
