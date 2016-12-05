#include "customtreegraphicsview.h"
#include "src/ui/mainwindow.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/behavior/generators/hkbgenerator.h"
#include "src/ui/behaviorgraphui/behaviorgraphview.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/ui/behaviorgraphui/customtreegraphicsviewicon.h"
#include "src/ui//hkxclassesui/hkdataui.h"

#include <QBoxLayout>
#include <QMenuBar>
#include <QFile>
#include <QSettings>
#include <QGroupBox>
#include <QMessageBox>
#include <QGraphicsSceneMouseEvent>
#include <QSize>

/**
 * CustomTreeGraphicsView
 */

CustomTreeGraphicsView::CustomTreeGraphicsView(QMenu *contextMenu, CustomTreeGraphicsViewIcon *rootIcon)
    : behaviorGS(new QGraphicsScene),
      selectedIcon(NULL),
      rootIcon(rootIcon),
      popUpMenu(contextMenu),
      minScale(0.001),
      maxScale(4),
      initScale(1),
      iconFocusScale(1.5),
      currentScale(1),
      scaleUpFactor(1.2),
      scaleDownFactor(0.8)
{
    setMinimumHeight(200);
    setMinimumWidth(200);
    setScene(behaviorGS);
    setContextMenuPolicy(Qt::CustomContextMenu);
    CustomTreeGraphicsViewIcon::updateStaticMembers();
}

QSize CustomTreeGraphicsView::sizeHint() const{
    return QSize(500, 400);
}

CustomTreeGraphicsViewIcon* CustomTreeGraphicsView::getSelectedItem() const{
    return selectedIcon;
}

void CustomTreeGraphicsView::contractBranch(CustomTreeGraphicsViewIcon * icon, bool contractAll){
    if (!icon){
        return;
    }
    for (int i = 0; i < icon->children.size(); i++){
        icon->children.at(i)->setVisible(false);
        if (icon->children.at(i)->linkToParent){
            icon->children.at(i)->linkToParent->setVisible(false);
        }
        if (contractAll){
            icon->children.at(i)->isExpanded = false;
            contractBranch(icon->children.at(i), true);
        }else{
            contractBranch(icon->children.at(i));
        }
    }
}

void CustomTreeGraphicsView::expandBranch(CustomTreeGraphicsViewIcon * icon, bool expandAll){
    if (!icon){
        return;
    }
    icon->isExpanded = true;
    for (int i = 0; i < icon->children.size(); i++){
        icon->children.at(i)->setVisible(true);
        if (icon->children.at(i)->linkToParent){
            icon->children.at(i)->linkToParent->setVisible(true);
        }
        if (expandAll){
            expandBranch(icon->children.at(i), true);
        }else if (icon->children.at(i)->isExpanded){
            expandBranch(icon->children.at(i));
        }
    }
}

void CustomTreeGraphicsView::createIconBranch(QList <CustomTreeGraphicsViewIcon *> & icons, CustomTreeGraphicsViewIcon *parent){
    icons.append(parent);
    CustomTreeGraphicsViewIcon *last = icons.last();
    for (int i = 0; i < last->children.size(); i++){
        createIconBranch(icons, last->children.at(i));
    }
}

void CustomTreeGraphicsView::removeChildIcons(CustomTreeGraphicsViewIcon *parent, CustomTreeGraphicsViewIcon *iconToKeep){
    if (!parent){
        return;
    }
    CustomTreeGraphicsViewIcon *icon;
    CustomTreeGraphicsViewIcon *oldParent = parent->parent;
    DataIconManager *gen;
    qreal lastY = 0;
    QList <CustomTreeGraphicsViewIcon *> icons;
    QList <CustomTreeGraphicsViewIcon *> branch;
    createIconBranch(icons, parent);
    int index = 0;
    for (int j = 0; j < icons.size(); j++){ //Add search function for icons by name!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        if (icons.at(j) == iconToKeep && !iconToKeep->children.isEmpty()){
            gen = static_cast<DataIconManager *>(iconToKeep->data.data());
            if (gen){
                for (int x = 0; x < gen->icons.size(); x++){
                    if (icons.contains(gen->icons.at(x))){
                        index = j + 1;
                        if (index < icons.size()){
                            icons.insert(index, iconToKeep->setParentReturnDuplicate(oldParent));
                        }else{
                            icons.append(iconToKeep->setParentReturnDuplicate(oldParent));
                        }
                        createIconBranch(branch, iconToKeep);
                        break;
                    }
                }
            }
        }else if (icons.at(j)->children.size() > 0){
            gen = static_cast<DataIconManager *>(icons.at(j)->data.data());
            if (gen){
                for (int x = 0; x < gen->icons.size(); x++){
                    if (!icons.contains(gen->icons.at(x))){
                        if (lastY == 0 || lastY > gen->icons.at(x)->y()){
                            lastY = gen->icons.at(x)->y();
                            icon = gen->icons.at(x);
                            gen->icons[x] = icons.at(j);
                            icons.insert(j + 1, icons.at(j)->setParentReturnDuplicate(icon->parent));
                            for (int y = 0; y < x; y++){
                                if (!gen->icons.isEmpty() && icons.contains(gen->icons.first())){
                                    gen->icons.removeFirst();
                                }
                            }
                            branch.append(icons.at(j));
                            for (int a = branch.size() - 1; a < branch.size(); a++){
                                for (int b = 0; b < branch.at(a)->children.size(); b++){
                                    branch.append(branch.at(a)->children.at(b));
                                }
                            }
                            removeIconFromGraph(icon);
                            break;
                        }
                    }
                }
            }
        }else{
            lastY = 0;
        }
    }
    for (int c = 0; c < branch.size(); c++){
        icons.removeAll(branch.at(c));
    }
    for (int d = 0; d < branch.size(); d++){
        expandBranchForIcon(branch.at(d));
    }
    for (int k = icons.size() - 1; k >= 0; k--){
        if (icons.at(k)){//Sometimes there are duplicates in icons??? Investigate why later...
            removeIconFromGraph(icons.at(k));
        }
    }
    return;
}

void CustomTreeGraphicsView::removeIconFromGraph(CustomTreeGraphicsViewIcon *iconToRemove){
    DataIconManager *gen = static_cast<DataIconManager *>(iconToRemove->data.data());
    if (iconToRemove->parent && !iconToRemove->parent->children.isEmpty()){
        iconToRemove->parent->children.removeAll(iconToRemove);
    }
    gen->icons.removeAll(iconToRemove);
    if (gen->icons.isEmpty()){
        gen->unlink();
    }
    behaviorGS->removeItem(iconToRemove);
    behaviorGS->removeItem(iconToRemove->linkToParent);
    delete iconToRemove->linkToParent;
    delete iconToRemove;
    iconToRemove = NULL;
}

void CustomTreeGraphicsView::deleteSelectedBranch(CustomTreeGraphicsViewIcon *icon, CustomTreeGraphicsViewIcon *iconToKeep){
    if (icon && icon->data.constData() && icon->parent && icon->parent->data.constData()){
        if (icon->children.isEmpty() && iconToKeep != icon){
            static_cast<DataIconManager *>(icon->data.data())->icons.removeAll(icon);
            if (static_cast<DataIconManager *>(icon->data.data())->icons.isEmpty()){
                icon->data.data()->unlink();
            }
            icon->parent->children.removeAll(icon);
            behaviorGS->removeItem(icon->linkToParent);
            behaviorGS->removeItem(icon);
            delete icon->linkToParent;
            delete icon;
        }else{
            removeChildIcons(icon, iconToKeep);
        }
        icon = NULL;
    }
}

bool CustomTreeGraphicsView::reconnectBranch(DataIconManager *oldChild, DataIconManager *newChild, CustomTreeGraphicsViewIcon *icon){
    if (!newChild || !icon){
        return false;
    }
    if (icon->isGranfatherParadox(newChild)){
        return false;
    }
    CustomTreeGraphicsViewIcon *oldChildIcon = NULL;
    CustomTreeGraphicsViewIcon *newChildIcon = NULL;
    DataIconManager *gen = static_cast<DataIconManager *>(newChild);
    for (int i = 0; i < icon->children.size(); i++){
        if (icon->children.at(i)->data.data() == oldChild){
            oldChildIcon = icon->children.at(i);
        }
    }
    if (oldChildIcon){
        for (int j =0; j < newChild->icons.size(); j++){
            if (oldChildIcon->isDescendant(newChild->icons.at(j))){
                newChildIcon = newChild->icons.at(j);
                break;
            }
        }
    }
    removeSelectedObjectBranch(oldChildIcon, newChildIcon, false);
    if (!newChildIcon){
        if (newChild->hasChildren() && !newChild->icons.isEmpty() && newChild->icons.first()->pos().y() > icon->pos().y()){
            icon->children.append(newChild->icons.first());
            if (newChild->icons.first()->parent){
                newChild->icons.first()->parent->children.removeAll(newChild->icons.first());
                initalizeAppendedIcon(newChild, newChild->icons.first()->parent);
                newChild->icons.first()->parent = icon;
            }
        }else{
            initalizeAppendedIcon(newChild, icon);
        }
    }else if (!icon->hasChildIcon(newChildIcon)){
        initalizeAppendedIcon(newChild, icon);
    }
    if (behaviorGS && !behaviorGS->items(Qt::AscendingOrder).isEmpty()){
        CustomTreeGraphicsViewIcon *root = static_cast<CustomTreeGraphicsViewIcon *>(scene()->items(Qt::AscendingOrder).first());
        expandBranch(root);
        repositionIcons(root, true);
        behaviorGS->update();
    }
    return true;
}

void CustomTreeGraphicsView::removeSelectedObjectBranchSlot(){
    if (selectedIcon){
        selectedIcon->unhighlight();
    }
    HkxObject *gen = NULL;
    if (selectedIcon->parent){
        gen = selectedIcon->parent->data.data();
    }
    removeSelectedObjectBranch(selectedIcon);
    if (gen){
        gen->evaulateDataValidity();
    }
    selectedIcon = NULL;
    emit branchRemoved();
}

void CustomTreeGraphicsView::removeSelectedObjectBranch(CustomTreeGraphicsViewIcon *icon, CustomTreeGraphicsViewIcon *iconToKeep, bool removeAll){
    if (icon && icon->parent && icon->parent->data.constData()){
        if (!(static_cast<DataIconManager *>(icon->parent->data.data())->removeObject(static_cast<DataIconManager *>(icon->data.data()), removeAll))){
            return;
        }
        deleteSelectedBranch(icon, iconToKeep);
        if (behaviorGS && !behaviorGS->items(Qt::AscendingOrder).isEmpty()){
            CustomTreeGraphicsViewIcon *behaviorGraphIcon = static_cast<CustomTreeGraphicsViewIcon *>(scene()->items(Qt::AscendingOrder).first());
            repositionIcons(behaviorGraphIcon, true);
            behaviorGS->update();
        }
    }
}

bool CustomTreeGraphicsView::appendObject(DataIconManager *ptr, CustomTreeGraphicsViewIcon *parentObjIcon){
    if (!ptr || !parentObjIcon || !parentObjIcon->data.constData()){
        return false;
    }
    DataIconManager *selectedData = static_cast<DataIconManager *>(parentObjIcon->data.data());
    if (!selectedData->appendObject(ptr)){
        return false;
    }
    CustomTreeGraphicsViewIcon *newIcon = initalizeAppendedIcon(ptr, selectedData->icons.first());
    newIcon->setSelected();
    return true;
}


void CustomTreeGraphicsView::wrapObject(DataIconManager *obj, CustomTreeGraphicsViewIcon *parentObjIcon){
    if (!obj || !parentObjIcon || !parentObjIcon->parent || !parentObjIcon->parent->data.constData()){
        return;
    }
    if (!static_cast<DataIconManager *>(parentObjIcon->parent->data.data())->wrapObject(obj, static_cast<DataIconManager *>(parentObjIcon->data.data()))){
        delete obj;
        obj = NULL;
        return;
    }
    CustomTreeGraphicsViewIcon *newIcon = initalizeInjectedIcon(obj, parentObjIcon->parent, selectedIcon);
    newIcon->setSelected();
}

void CustomTreeGraphicsView::popUpMenuRequested(const QPoint &pos){
    popUpMenu->popup(viewport()->mapToGlobal(pos));
}

void CustomTreeGraphicsView::repositionIcons(CustomTreeGraphicsViewIcon *icon, bool updateNonVisable){
    if (!icon){
        return;
    }
    for (int i = 0; i < icon->children.size(); i++){
        icon->children.at(i)->updatePosition(updateNonVisable);
        repositionIcons(icon->children.at(i));
    }
}

void CustomTreeGraphicsView::expandBranchForIcon(CustomTreeGraphicsViewIcon *icon){
    if (icon){
        icon->isExpanded = true;
        icon->setVisible(true);
        if (icon->linkToParent){
            icon->linkToParent->setVisible(true);
        }
        for (int i = 0; i < icon->children.size(); i++){
            icon->children.at(i)->setVisible(true);
            if (icon->children.at(i)->linkToParent){
                icon->children.at(i)->linkToParent->setVisible(true);
            }
        }
        expandBranchForIcon(icon->parent);
    }
}

void CustomTreeGraphicsView::wheelEvent(QWheelEvent *event){
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    if (event->delta() > 0 && currentScale < maxScale){
        scale(scaleUpFactor, scaleUpFactor);
        currentScale = currentScale*scaleUpFactor;
    }else if (currentScale > minScale){
        scale(scaleDownFactor, scaleDownFactor);
        currentScale = currentScale*scaleDownFactor;
    }
}

void CustomTreeGraphicsView::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton){
        this->setDragMode(QGraphicsView::ScrollHandDrag);
    }
    QGraphicsView::mousePressEvent(event);
}

void CustomTreeGraphicsView::mouseReleaseEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton){
        this->setDragMode(QGraphicsView::NoDrag);
    }
    QGraphicsView::mouseReleaseEvent(event);
}

int CustomTreeGraphicsView::initializeIconsForNewBranch(DataIconManager *ptr, QList <DataIconManager *> & objects, QList<CustomTreeGraphicsViewIcon *> & parentIcons, QVector <short> & objectChildCount){
    if (parentIcons.isEmpty() || objects.isEmpty() || objectChildCount.isEmpty()){
        return -1;
    }
    bool isBranchTip = true;
    CustomTreeGraphicsViewIcon *icon = addIconToGraph(ptr, parentIcons.last());
    if (!icon){
        return -1;
    }
    if (ptr->hasChildren()){
        isBranchTip = false;
    }
    objects.removeLast();
    objectChildCount.last()--;
    if (objectChildCount.last() < 1){
        parentIcons.removeLast();
        objectChildCount.removeLast();
    }
    if (!isBranchTip){
        objectChildCount.append(0);
        parentIcons.append(icon);
    }
    return 0;
}

int CustomTreeGraphicsView::initializeIcons(DataIconManager *ptr, QList <DataIconManager *> & objects, QList<CustomTreeGraphicsViewIcon *> & parentIcons, QVector <short> & objectChildCount){
    if (parentIcons.isEmpty() || objects.isEmpty() || objectChildCount.isEmpty()){
        return -1;
    }
    bool isBranchTip = true;
    for (int i = 0; i < parentIcons.last()->children.size(); i++){
        if (parentIcons.last()->children.at(i)->data.data() == objects.last()){
            objects.removeLast();
            objectChildCount.last()--;
            if (objectChildCount.last() < 1){
                parentIcons.removeLast();
                objectChildCount.removeLast();
            }
            return 1;
        }
    }
    CustomTreeGraphicsViewIcon *icon = addIconToGraph(ptr, parentIcons.last());
    if (!icon){
        return -1;
    }
    if (ptr->hasChildren()){
        isBranchTip = false;
        if (ptr->icons.size() > 1){
            objects.removeLast();
            objectChildCount.last()--;
            if (objectChildCount.last() < 1){
                parentIcons.removeLast();
                objectChildCount.removeLast();
            }
            return 1;
        }
    }
    objects.removeLast();
    objectChildCount.last()--;
    if (objectChildCount.last() < 1){
        parentIcons.removeLast();
        objectChildCount.removeLast();
    }
    if (!isBranchTip){
        objectChildCount.append(0);
        parentIcons.append(icon);
    }
    return 0;
}

void CustomTreeGraphicsView::setContextMenu(QMenu *contextMenu){
    popUpMenu = contextMenu;
}

CustomTreeGraphicsViewIcon * CustomTreeGraphicsView::addIconToGraph(DataIconManager *obj, CustomTreeGraphicsViewIcon * parentIcon){
    if (!parentIcon || !obj){
        return NULL;
    }
    CustomTreeGraphicsViewIcon *icon = new CustomTreeGraphicsViewIcon(obj, obj->getName(), parentIcon);
    //icon->setFlag(QGraphicsItem::ItemIsMovable);
    behaviorGS->addItem(icon->linkToParent);
    behaviorGS->addItem(icon);
    return icon;
}

CustomTreeGraphicsViewIcon * CustomTreeGraphicsView::initalizeInjectedIcon(DataIconManager *obj, CustomTreeGraphicsViewIcon * parentIcon, CustomTreeGraphicsViewIcon * selected){
    if (!parentIcon){
        return NULL;
    }
    CustomTreeGraphicsViewIcon *icon = new CustomTreeGraphicsViewIcon(obj, obj->getName(), parentIcon);
    parentIcon->replaceChild(selected, icon);
    //Order is important??
    behaviorGS->addItem(icon->linkToParent);
    behaviorGS->addItem(icon);
    return icon;
}

CustomTreeGraphicsViewIcon * CustomTreeGraphicsView::initalizeAppendedIcon(DataIconManager *obj, CustomTreeGraphicsViewIcon * parentIcon){
    if (!parentIcon){
        return NULL;
    }
    CustomTreeGraphicsViewIcon *icon = new CustomTreeGraphicsViewIcon(obj, obj->getName(), parentIcon);
    //Order is important??
    behaviorGS->addItem(icon->linkToParent);
    behaviorGS->addItem(icon);
    return icon;
}

bool CustomTreeGraphicsView::draw(DataIconManager *root, DataIconManager *firstChild){
    if (!root){
        return false;
    }
    QList <DataIconManager *> objects;
    QList <CustomTreeGraphicsViewIcon *> parentIcons;
    QVector <short> objectChildCount;
    DataIconManager *gen = NULL;
    int result;
    if (!root->icons.isEmpty()){
        rootIcon = root->icons.first();
    }else{
        rootIcon = new CustomTreeGraphicsViewIcon(root, NULL);
    }
    behaviorGS->addItem(rootIcon);
    objects.append(firstChild);
    objectChildCount.append(1);
    parentIcons.append(rootIcon);
    while (!objects.isEmpty()){
        if (!objects.last()){
            return false;
        }
        gen = static_cast<DataIconManager *>(objects.last());
        result = initializeIcons(gen, objects, parentIcons, objectChildCount);
        if (result == 0){
            if (!objectChildCount.isEmpty()){
                objectChildCount.last() = objectChildCount.last() + gen->addChildrenToList(objects);
            }
        }else if (result == -1){
            return false;
        }
    }
    return true;
}

CustomTreeGraphicsView::~CustomTreeGraphicsView(){
    //
}
