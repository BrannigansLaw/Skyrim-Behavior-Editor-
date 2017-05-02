#ifndef DATAICONMANAGER_H
#define DATAICONMANAGER_H

#include <QString>
#include <QList>

#include "src/hkxclasses/hkxobject.h"

class TreeGraphicsItem;

class DataIconManager: public HkDynamicObject
{
    friend class GraphicsViewData;
    friend class TreeGraphicsScene;
    friend class TreeGraphicsItem;
public:
    virtual QString getName() const;
    virtual bool hasChildren() const;
    bool hasIcons() const;
    void updateIconNames();
protected:
    virtual QList <DataIconManager *> getChildren() const;
    virtual int getIndexOfObj(DataIconManager *) const;
    virtual bool insertObjectAt(int , DataIconManager *);
    virtual bool removeObjectAt(int );//If index is -1 remove all...
    DataIconManager(HkxFile *parent, long ref = -1);
private:
    bool wrapObjectAt(int index, DataIconManager *obj, DataIconManager *parentObj);
    TreeGraphicsItem *reconnectToNext();
    void appendIcon(TreeGraphicsItem *icon);
    void removeIcon(TreeGraphicsItem *icon);
private:
    QList <TreeGraphicsItem *> icons;
};

#endif // DATAICONMANAGER_H
