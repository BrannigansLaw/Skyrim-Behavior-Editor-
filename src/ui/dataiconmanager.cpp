#include "dataiconmanager.h"
#include "treegraphicsitem.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/behavior/generators/hkbgenerator.h"
#include "src/hkxclasses/behavior/generators/hkbclipgenerator.h"
#include "src/hkxclasses/behavior/modifiers/hkbmodifier.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"

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

void DataIconManager::injectWhileMerging(HkxObject *recessiveobj){
    if (recessiveobj){
        //recessiveobj->fixMergedEventIndices(static_cast<BehaviorFile *>(getParentFile()));
        DataIconManager *recobj = static_cast<DataIconManager *>(recessiveobj);
        QList <DataIconManager *> domchildren = getChildren();
        QList <DataIconManager *> recchildren = recobj->getChildren();
        QList <DataIconManager *> tempchildren;
        //QList <DataIconManager *> tempchildren2;
        DataIconManager *domchild;
        DataIconManager *recchild;
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
            variableBindingSet = HkxSharedPtr(new hkbVariableBindingSet(getParentFile()));
            variableBindingSet.data()->merge(recobj->variableBindingSet.data());
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
                    tempchildren = recchildren.at(j)->getChildren();
                    for (auto k = 0; k < tempchildren.size(); k++){
                        tempsig = tempchildren.at(k)->getSignature();
                        tempchild = tempchildren.at(k);
                        if ((domsig == tempsig) && ((domchild->getName() == tempchild->getName()) || //For FNIS problem in mt_behavior NPC_TurnLeft90
                                                  (domsig == HKB_CLIP_GENERATOR && !QString::compare(static_cast<hkbClipGenerator *>(domchild)->getAnimationName().section("\\", -1, -1),
                                                                                                     static_cast<hkbClipGenerator *>(tempchild)->getAnimationName().section("\\", -1, -1), Qt::CaseInsensitive))))
                        {
                            insertObjectAt(i, recchildren.at(j));
                            if (!static_cast<BehaviorFile *>(getParentFile())->existsInBehavior(recchildren.at(j))){
                                getParentFile()->addObjectToFile(recchildren.at(j), -1);
                                getParentFile()->addObjectToFile(recchildren.at(j)->variableBindingSet.data(), -1);
                            }
                            if (recchildren.at(j)->variableBindingSet.data()){
                                static_cast<hkbVariableBindingSet *>(recchildren.at(j)->variableBindingSet.data())->fixMergedIndices(static_cast<BehaviorFile *>(getParentFile()));
                            }
                            recchildren.removeAt(j);
                            found = true;
                            for (auto m = 0; m < tempchildren.size(); m++){
                                if (!static_cast<BehaviorFile *>(getParentFile())->existsInBehavior(tempchildren.at(m))){
                                    getParentFile()->addObjectToFile(tempchildren.at(m), -1);
                                    getParentFile()->addObjectToFile(tempchildren.at(m)->variableBindingSet.data(), -1);
                                }
                                if (tempchildren.at(m)->variableBindingSet.data()){
                                    static_cast<hkbVariableBindingSet *>(tempchildren.at(m)->variableBindingSet.data())->fixMergedIndices(static_cast<BehaviorFile *>(getParentFile()));
                                }
                                objects = static_cast<DataIconManager *>(tempchildren.at(m))->getChildren();
                                while (!objects.isEmpty()){
                                    if (!static_cast<BehaviorFile *>(getParentFile())->existsInBehavior(objects.last())){
                                        getParentFile()->addObjectToFile(objects.last(), -1);
                                        getParentFile()->addObjectToFile(objects.last()->variableBindingSet.data(), -1);
                                        children = objects.last()->getChildren();
                                    }
                                    if (objects.last()->variableBindingSet.data()){
                                        static_cast<hkbVariableBindingSet *>(objects.last()->variableBindingSet.data())->fixMergedIndices(static_cast<BehaviorFile *>(getParentFile()));
                                    }
                                    objects.removeLast();
                                    objects = objects + children;
                                    children.clear();
                                }
                            }
                            k = tempchildren.size();
                            j = -1;
                        }
                    }
                    /*if (!found){
                        for (auto n = 0; n < tempchildren.size(); n++){
                            tempchildren2 = tempchildren.at(n)->getChildren();
                            for (auto o = 0; o < tempchildren2.size(); o++){
                                tempsig = tempchildren2.at(o)->getSignature();
                                tempchild = static_cast<Type *>(tempchildren2.at(o));
                                if (domsig == tempsig && ((domsig == HKB_CLIP_GENERATOR && !QString::compare(reinterpret_cast<hkbClipGenerator *>(domchild)->getAnimationName(), reinterpret_cast<hkbClipGenerator *>(tempchild)->getAnimationName(), Qt::CaseInsensitive)) ||
                                                          (domchild->getName() == tempchild->getName())))//For FNIS...
                                {
                                    insertObjectAt(i, tempchildren.at(n));
                                    getParentFile()->addObjectToFile(tempchildren.at(n), -1);
                                    if (tempchildren.at(n)->variableBindingSet.data()){
                                        static_cast<hkbVariableBindingSet *>(tempchildren.at(n)->variableBindingSet.data())->fixMergedIndices(static_cast<BehaviorFile *>(getParentFile()));
                                    }
                                    getParentFile()->addObjectToFile(tempchildren.at(n)->variableBindingSet.data(), -1);
                                    found = true;
                                    for (auto p = 0; p < tempchildren2.size(); p++){
                                        getParentFile()->addObjectToFile(tempchildren2.at(p), -1);
                                        if (tempchildren2.at(p)->variableBindingSet.data()){
                                            static_cast<hkbVariableBindingSet *>(tempchildren2.at(p)->variableBindingSet.data())->fixMergedIndices(static_cast<BehaviorFile *>(getParentFile()));
                                        }
                                        getParentFile()->addObjectToFile(tempchildren2.at(p)->variableBindingSet.data(), -1);
                                        objects = static_cast<DataIconManager *>(tempchildren2.at(p))->getChildren();
                                        while (!objects.isEmpty()){
                                            if (!static_cast<BehaviorFile *>(getParentFile())->existsInBehavior(objects.last())){
                                                getParentFile()->addObjectToFile(objects.last(), -1);
                                                children = objects.last()->getChildren();
                                            }
                                            objects.removeLast();
                                            objects = objects + children;
                                            children.clear();
                                        }
                                    }
                                    //o = tempchildren2.size();
                                    //n = tempchildren.size();
                                }
                            }
                        }
                    }*/
                }
            }
        }
    }else{
        WARNING_MESSAGE("DataIconManager::injectWhileMerging() hkbGenerator: nullptr!");
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
