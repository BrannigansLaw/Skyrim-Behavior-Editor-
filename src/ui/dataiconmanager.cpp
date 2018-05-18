#include "dataiconmanager.h"
#include "treegraphicsitem.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/behavior/generators/hkbgenerator.h"
#include "src/hkxclasses/behavior/modifiers/hkbmodifier.h"

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
    }/*else if (getType() == TYPE_MODIFIER){
        injectWhileMerging(((hkbModifier *)recessiveObject));
    }*/
    return true;
}

void DataIconManager::injectWhileMerging(hkbGenerator *recessiveobj){
    if (recessiveobj){
        setIsMerged(true);
        QList <DataIconManager *> domchildren = getChildren();
        QList <DataIconManager *> recchildren = static_cast<DataIconManager *>(recessiveobj)->getChildren();
        QList <DataIconManager *> tempchildren;
        hkbGenerator *domchild;
        hkbGenerator *recchild;
        hkbGenerator *tempchild;
        HkxSignature domsig;
        HkxSignature recsig;
        HkxSignature tempsig;
        bool found;
        for (auto i = 0; i < domchildren.size(); i++){
            found = false;
            domsig = domchildren.at(i)->getSignature();
            domchild = static_cast<hkbGenerator *>(domchildren.at(i));
            for (auto j = 0; j < recchildren.size(); j++){
                recsig = recchildren.at(j)->getSignature();
                recchild = static_cast<hkbGenerator *>(recchildren.at(j));
                if (domsig == recsig && domchild->getName() == recchild->getName()){
                    found = true;
                    break;
                }
            }
            if (!found){
                for (auto j = 0; j < recchildren.size(); j++){
                    tempchildren = recchildren.at(j)->getChildren();
                    for (auto k = 0; k < tempchildren.size(); k++){
                        tempsig = tempchildren.at(k)->getSignature();
                        tempchild = static_cast<hkbGenerator *>(tempchildren.at(k));
                        if (domsig == tempsig && domchild->getName() == tempchild->getName()){
                            insertObjectAt(i, recchildren.at(j));
                            found = true;
                            k = tempchildren.size();
                            j = recchildren.size();
                        }
                    }
                    if (!found){
                        //WARNING_MESSAGE("DataIconManager::injectWhileMerging() hkbGenerator: Cannot inject!");
                    }
                }
            }
        }
    }else{
        WARNING_MESSAGE("DataIconManager::injectWhileMerging() hkbGenerator: nullptr!");
    }
}

void DataIconManager::injectWhileMerging(hkbModifier *recessiveobj){
    if (recessiveobj){
        QList <DataIconManager *> domchildren = getChildren();
        QList <DataIconManager *> recchildren = static_cast<DataIconManager *>(recessiveobj)->getChildren();
        QList <DataIconManager *> tempchildren;
        hkbModifier *domchild;
        hkbModifier *recchild;
        hkbModifier *tempchild;
        HkxSignature domsig;
        HkxSignature recsig;
        HkxSignature tempsig;
        bool found;
        for (auto i = 0; i < domchildren.size(); i++){
            found = false;
            domsig = domchildren.at(i)->getSignature();
            domchild = static_cast<hkbModifier *>(domchildren.at(i));
            for (auto j = 0; j < recchildren.size(); j++){
                recsig = recchildren.at(j)->getSignature();
                recchild = static_cast<hkbModifier *>(recchildren.at(j));
                if (domsig == recsig && domchild->getName() == recchild->getName()){
                    found = true;
                    break;
                }
            }
            if (!found){
                for (auto j = 0; j < recchildren.size(); j++){
                    tempchildren = recchildren.at(j)->getChildren();
                    for (auto k = 0; k < tempchildren.size(); k++){
                        tempsig = tempchildren.at(k)->getSignature();
                        tempchild = static_cast<hkbModifier *>(tempchildren.at(k));
                        if (domsig == tempsig && domchild->getName() == tempchild->getName()){
                            insertObjectAt(i, recchildren.at(j));
                            found = true;
                            k = tempchildren.size();
                            j = recchildren.size();
                        }
                    }
                    if (!found){
                        //WARNING_MESSAGE("DataIconManager::injectWhileMerging() hkbGenerator: Cannot inject!");
                    }
                }
            }
        }
    }else{
        WARNING_MESSAGE("DataIconManager::injectWhileMerging() hkbModifier: nullptr!");
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
