#ifndef TREEGRAPHICSSCENE_H
#define TREEGRAPHICSSCENE_H

#include <QGraphicsScene>

class DataIconManager;
class TreeGraphicsItem;

class TreeGraphicsScene: public QGraphicsScene
{
    friend class TreeGraphicsView;
    friend class TreeGraphicsItem;
public:
    TreeGraphicsScene();
    void setCanDeleteRoot(bool value);
protected:
    //virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
private:
    void selectIcon(TreeGraphicsItem *icon, bool expand);
    void contractBranch(TreeGraphicsItem *icon, bool contractAll = false);
    void expandBranch(TreeGraphicsItem *icon, bool expandAll = false);
    bool drawGraph(DataIconManager *rootData, bool allowDuplicates);
    TreeGraphicsItem * addItemToGraph(TreeGraphicsItem *selectedIcon, DataIconManager *data, int indexToInsert, bool inject = false, bool allowDuplicates = false);
    bool reconnectIcon(TreeGraphicsItem *oldIconParent, DataIconManager *dataToReplace, DataIconManager *replacementData);
    bool removeItemFromGraph(TreeGraphicsItem *item, int indexToRemove);
private:
    TreeGraphicsItem *rootIcon;
    TreeGraphicsItem *selectedIcon;
    bool canDeleteRoot;
};

#endif // TREEGRAPHICSSCENE_H
