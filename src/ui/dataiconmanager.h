#ifndef DATAICONMANAGER_H
#define DATAICONMANAGER_H

#include <QString>
#include <QList>

class TreeGraphicsItem;

class DataIconManager
{
    friend class GraphicsViewData;
    friend class TreeGraphicsScene;
    friend class TreeGraphicsItem;
public:
    virtual QString getName() const;
    virtual bool hasChildren() const;
    bool hasIcons() const;
protected:
    virtual QList <DataIconManager *> getChildren() const;
    virtual int getIndexOfObj(DataIconManager *) const;
    virtual bool insertObjectAt(int , DataIconManager *);
    virtual bool removeObjectAt(int );
private:
    bool wrapObjectAt(int index, DataIconManager *obj, DataIconManager *parentObj);
    TreeGraphicsItem *reconnectToNext();
    void appendIcon(TreeGraphicsItem *icon);
    void removeIcon(TreeGraphicsItem *icon);
private:
    QList <TreeGraphicsItem *> icons;
};

#endif // DATAICONMANAGER_H
