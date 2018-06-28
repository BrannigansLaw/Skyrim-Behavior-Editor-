#include "dataiconmanager.h"
#include "treegraphicsitem.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/behavior/generators/hkbgenerator.h"
#include "src/hkxclasses/behavior/generators/hkbclipgenerator.h"
#include "src/hkxclasses/behavior/modifiers/hkbmodifier.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "behaviorgraphview.h"

#include <QGraphicsScene>

QString DataIconManager::getName() const{
    return "";
}

bool DataIconManager::hasChildren() const{
    return false;
}

bool DataIconManager::hasIcons() const{
    if (!icons.isEmpty()){
        if (icons.size() > 1){
            return true;
        }
    }else{
        CRITICAL_ERROR_MESSAGE("DataIconManager::hasIcons(): 'icons' is empty!!!");
    }
    return false;
}

void DataIconManager::updateIconNames(){
    //icons.first()->scene()->update();
    for (int i = 0; i < icons.size(); i++){
        icons.at(i)->update(/*QRectF(icons.at(i)->pos(), QSizeF(icons.at(i)->boundingRect().size()))*/);
    }
}

bool DataIconManager::hasSameSignatureAndName(const DataIconManager *other) const{
    if (!other || getSignature() != other->getSignature() || getName() != other->getName()){
        return false;
    }
    return true;
}

QList<DataIconManager *> DataIconManager::getChildren() const{
    return QList<DataIconManager *> ();
}

int DataIconManager::getIndexOfObj(DataIconManager *) const{
    return -1;
}

bool DataIconManager::wrapObjectAt(int index, DataIconManager *obj, DataIconManager *parentObj){
    if (obj && parentObj){
        if (parentObj->insertObjectAt(parentObj->getIndexOfObj(this), obj) && obj->insertObjectAt(index, this)){
            return true;
        }
    }
    return false;
}

bool DataIconManager::insertObjectAt(int, DataIconManager *){
    return false;
}

bool DataIconManager::removeObjectAt(int ){
    return false;
}

DataIconManager::DataIconManager(HkxFile *parent, long ref)
    : HkDynamicObject(parent, ref)
{
    //
}

bool DataIconManager::merge(HkxObject *recessiveObject){
    if (getType() == TYPE_GENERATOR){
        injectWhileMerging(((hkbGenerator *)recessiveObject));
    }else if (getType() == TYPE_MODIFIER){
        injectWhileMerging(((hkbModifier *)recessiveObject));
    }
    return true;
}

void DataIconManager::setDataInvalid(){
    for (int i = 0; i < icons.size(); i++){
        icons.at(i)->setPenColor(Qt::red);
        icons.at(i)->update(/*QRectF(icons.at(i)->pos(), QSizeF(icons.at(i)->boundingRect().size()))*/);
    }
}

void DataIconManager::setFocusOnTopIcon(){
    TreeGraphicsItem *icon;
    if (!icons.isEmpty()){
        icon = icons.first();
        if (icon && icon->scene() && !icon->scene()->views().isEmpty()){
            icon = icons.first();
            static_cast<BehaviorGraphView *>(icon->scene()->views().first())->centerOn(icon);
            static_cast<BehaviorGraphView *>(icon->scene()->views().first())->setSelectedItem(icon);
        }else{
            LogFile::writeToLog(getName()+": icons is missing scene or view!!");
        }
    }else{
        LogFile::writeToLog(getName()+": icons is empty!!");
    }
}

void DataIconManager::injectWhileMerging(HkxObject *recessiveobj){
    if (!getIsMerged() && recessiveobj){
        DataIconManager *recobj = static_cast<DataIconManager *>(recessiveobj);
        QList <DataIconManager *> domchildren = getChildren();
        QList <DataIconManager *> recchildren = recobj->getChildren();
        QList <DataIconManager *> tempchildren;
        DataIconManager *domchild;
        DataIconManager *recchild;
        DataIconManager *obj;
        DataIconManager *tempchild;
        HkxSignature domsig;
        HkxSignature recsig;
        HkxSignature tempsig;
        bool found;
        QList <DataIconManager *> objects;
        QList <DataIconManager *> children;
        if (variableBindingSet.data()){
            variableBindingSet.data()->merge(recobj->variableBindingSet.data());
        }else if (recobj->variableBindingSet.data()){
            variableBindingSet = HkxSharedPtr(recobj->variableBindingSet.data());
            recobj->fixMergedIndices(static_cast<BehaviorFile *>(getParentFile()));
            getParentFile()->addObjectToFile(recobj->variableBindingSet.data(), -1);
        }
        for (auto i = 0; i < domchildren.size(); i++){
            found = false;
            domsig = domchildren.at(i)->getSignature();
            domchild = domchildren.at(i);
            for (auto j = 0; j < recchildren.size(); j++){
                recsig = recchildren.at(j)->getSignature();
                recchild = recchildren.at(j);
                if (domsig == recsig && domchild->getName() == recchild->getName()){
                    found = true;
                    break;
                }
            }
            if (!found){
                for (auto j = recchildren.size() - 1; j >= 0; j--){
                    recchild = recchildren.at(j);
                    tempchildren = recchild->getChildren();
                    for (auto k = 0; k < tempchildren.size(); k++){
                        tempsig = tempchildren.at(k)->getSignature();
                        tempchild = tempchildren.at(k);
                        if ((domsig == tempsig) && ((domchild->getName() == tempchild->getName())/* || //For FNIS problem in mt_behavior NPC_TurnLeft90
                                                  (domsig == HKB_CLIP_GENERATOR && !QString::compare(static_cast<hkbClipGenerator *>(domchild)->getAnimationName().section("\\", -1, -1),
                                                                                                     static_cast<hkbClipGenerator *>(tempchild)->getAnimationName().section("\\", -1, -1), Qt::CaseInsensitive))*/))
                        {
                            insertObjectAt(i, recchild);
                            if (!static_cast<BehaviorFile *>(getParentFile())->existsInBehavior(recchild)){
                                recchild->fixMergedIndices(static_cast<BehaviorFile *>(getParentFile()));
                                recchild->fixMergedEventIndices(static_cast<BehaviorFile *>(getParentFile()));
                                getParentFile()->addObjectToFile(recchild, -1);
                                getParentFile()->addObjectToFile(recchild->variableBindingSet.data(), -1);
                            }
                            //tempchildren.removeAt(k);
                            for (auto m = 0; m < tempchildren.size(); m++){
                                tempchild = tempchildren.at(m);
                                if (!static_cast<BehaviorFile *>(getParentFile())->existsInBehavior(tempchild)){
                                    tempchild->fixMergedIndices(static_cast<BehaviorFile *>(getParentFile()));
                                    tempchild->fixMergedEventIndices(static_cast<BehaviorFile *>(getParentFile()));
                                    getParentFile()->addObjectToFile(tempchild, -1);
                                    getParentFile()->addObjectToFile(tempchild->variableBindingSet.data(), -1);
                                    objects = static_cast<DataIconManager *>(tempchild)->getChildren();
                                    while (!objects.isEmpty()){
                                        obj = objects.last();
                                        if (!static_cast<BehaviorFile *>(getParentFile())->existsInBehavior(obj)){
                                            obj->fixMergedIndices(static_cast<BehaviorFile *>(getParentFile()));
                                            obj->fixMergedEventIndices(static_cast<BehaviorFile *>(getParentFile()));
                                            getParentFile()->addObjectToFile(obj, -1);
                                            getParentFile()->addObjectToFile(obj->variableBindingSet.data(), -1);
                                            children = obj->getChildren();
                                        }
                                        objects.removeLast();
                                        objects = objects + children;
                                        children.clear();
                                    }
                                }
                            }
                            k = tempchildren.size();
                            j = -1;
                        }
                    }
                }
            }
        }
        setIsMerged(true);
    }else{
        //LogFile::writeToLog("DataIconManager::injectWhileMerging() hkbGenerator: nullptr!");
    }
}

TreeGraphicsItem * DataIconManager::reconnectToNext(){
    TreeGraphicsItem *iconToBeRemoved = nullptr;
    QList<QGraphicsItem *> children;
    if (!icons.isEmpty()){
        if (icons.size() > 1){
            iconToBeRemoved = icons.at(1);
            //icons.removeAt(1);
            if (iconToBeRemoved){
                if (iconToBeRemoved->parentItem() && icons.first()){
                    children = iconToBeRemoved->parentItem()->childItems();
                    icons.first()->setParent((TreeGraphicsItem *)iconToBeRemoved->parentItem(), children.indexOf(iconToBeRemoved));
                    return iconToBeRemoved;
                }
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE("DataIconManager::reconnectToNext(): 'icons' is empty!!!");
    }
    return nullptr;
}

void DataIconManager::appendIcon(TreeGraphicsItem *icon){
    //int index = -1;
    if (icon){
        if (icons.isEmpty() || !icons.contains(icon)){
            /*if (icons.size() > 1){
                index = icon->determineInsertionIndex();
                if (index > -1 && index < icons.size()){
                    icons.insert(index, icon);
                    return;
                }
            }*/
            icons.append(icon);
        }
    }
}

void DataIconManager::removeIcon(TreeGraphicsItem *icon){
    if (!icons.isEmpty()){
        icons.removeAll(icon);
    }else{
        CRITICAL_ERROR_MESSAGE("DataIconManager::removeIcon(): 'icons' is empty!!!");
    }
}
