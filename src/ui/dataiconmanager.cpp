#include "dataiconmanager.h"
#include "treegraphicsitem.h"
#include "src/filetypes/behaviorfile.h"

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
        CRITICAL_ERROR_MESSAGE(QString("DataIconManager::hasIcons(): 'icons' is empty!!!"))
        getParentFile()->writeToLog("DataIconManager::hasIcons(): 'icons' is empty!!!");
    }
    return false;
}

void DataIconManager::updateIconNames(){
    icons.first()->scene()->update();
    /*for (int i = 0; i < icons.size(); i++){
        icons.at(i)->update(QRectF(icons.at(i)->pos(), QSizeF(icons.at(i)->boundingRect().size())));
    }*/
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

TreeGraphicsItem * DataIconManager::reconnectToNext(){
    TreeGraphicsItem *iconToBeRemoved = NULL;
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
        CRITICAL_ERROR_MESSAGE(QString("DataIconManager::reconnectToNext(): 'icons' is empty!!!"))
        getParentFile()->writeToLog("DataIconManager::reconnectToNext(): 'icons' is empty!!!");
    }
    return NULL;
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
        CRITICAL_ERROR_MESSAGE(QString("DataIconManager::removeIcon(): 'icons' is empty!!!"))
        getParentFile()->writeToLog("DataIconManager::removeIcon(): 'icons' is empty!!!");
    }
}
