#ifndef BEHAVIORGRAPHUI_H
#define BEHAVIORGRAPHUI_H

#include <QWidget>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QGraphicsItem>

class FileSelectWindow;
class QVBoxLayout;
class QMenuBar;
class QMenu;
class QFile;
class QGraphicsScene;
class QGroupBox;
class QHBoxLayout;
class CustomTreeGraphicsViewIcon;
class DataIconManager;

/**
 * @brief The CustomTreeGraphicsView class
 * To use:
 * Create a DataIconManager class with your custom data and implement the following virtual functions in every final class:
 * - virtual int getIndexToInsertIcon() const;
 * - QString getName() const;
 * - virtual bool hasChildren() const;
 * - virtual bool wrapObject(DataIconManager *objToInject, DataIconManager *childToReplace);
 * - virtual bool appendObject(DataIconManager *objToAppend);
 * - virtual bool removeObject(DataIconManager *objToRemove, bool removeAll = true);
 * - virtual int addChildrenToList(QList<DataIconManager *> & list,(QList <QExplicitlySharedDataPointer <DataIconManager>> & list, bool reverseOrder = true);
 */

class CustomTreeGraphicsView: public QGraphicsView
{
    Q_OBJECT
    friend class CustomTreeGraphicsViewIcon;
    friend class BehaviorGraphView;
    friend class HkDataUI;
    friend class ManualSelectorGeneratorUI;
    friend class ModifierGeneratorUI;
    friend class BSiStateTaggingGeneratorUI;
    friend class StateMachineUI;
    friend class StateUI;
    friend class TransitionsUI;
    friend class MainWindow;
public:
    virtual ~CustomTreeGraphicsView();
    bool draw(DataIconManager *root, DataIconManager *firstChild);
    bool reconnectBranch(DataIconManager *oldChild, DataIconManager *newChild, CustomTreeGraphicsViewIcon *icon);
    QSize sizeHint() const Q_DECL_OVERRIDE;
    CustomTreeGraphicsViewIcon* getSelectedItem() const;
protected:
    CustomTreeGraphicsView(QMenu *contextMenu = NULL, CustomTreeGraphicsViewIcon *rootIcon = NULL);
    void setContextMenu(QMenu *contextMenu);
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
signals:
    void branchRemoved();
private slots:
    void removeSelectedObjectBranchSlot();
private:
    void removeSelectedObjectBranch(CustomTreeGraphicsViewIcon *icon, CustomTreeGraphicsViewIcon *iconToKeep = NULL, bool removeAll = true);
    void removeIconFromGraph(CustomTreeGraphicsViewIcon *iconToRemove);
private:
    QGraphicsScene *behaviorGS;
    CustomTreeGraphicsViewIcon *selectedIcon;
    CustomTreeGraphicsViewIcon *rootIcon;
    QMenu *popUpMenu;
    const qreal minScale;
    const qreal maxScale;
    const qreal initScale;
    const qreal iconFocusScale;
    qreal currentScale;
    const qreal scaleUpFactor;
    const qreal scaleDownFactor;
private:
    void expandBranchForIcon(CustomTreeGraphicsViewIcon *icon);
    void removeChildIcons(CustomTreeGraphicsViewIcon *parent, CustomTreeGraphicsViewIcon *iconToKeep = NULL);
    void repositionIcons(CustomTreeGraphicsViewIcon * icon, bool updateNonVisable = false);
    void expandBranch(CustomTreeGraphicsViewIcon * icon, bool expandAll = false);
    void contractBranch(CustomTreeGraphicsViewIcon * icon, bool contractAll = false);
    bool positionIcon(CustomTreeGraphicsViewIcon * icon);
    void createIconBranch(QList <CustomTreeGraphicsViewIcon *> & icons, CustomTreeGraphicsViewIcon *parent);
    void deleteSelectedBranch(CustomTreeGraphicsViewIcon *icon, CustomTreeGraphicsViewIcon *iconToKeep = NULL);
    bool appendObject(DataIconManager * ptr, CustomTreeGraphicsViewIcon *parentObjIcon);
    void wrapObject(DataIconManager *obj, CustomTreeGraphicsViewIcon *parentObjIcon);
    int initializeIconsForNewBranch(DataIconManager *ptr, QList <DataIconManager *> & objects, QList<CustomTreeGraphicsViewIcon *> & parentIcons, QVector <short> & objectChildCount);
    int initializeIcons(DataIconManager *ptr, QList <DataIconManager *> & objects, QList<CustomTreeGraphicsViewIcon *> & parentIcons, QVector <short> & objectChildCount);
    CustomTreeGraphicsViewIcon * addIconToGraph(DataIconManager *obj, CustomTreeGraphicsViewIcon * parentIcon);
    CustomTreeGraphicsViewIcon * initalizeInjectedIcon(DataIconManager *obj, CustomTreeGraphicsViewIcon * parentIcon, CustomTreeGraphicsViewIcon * selected);
    CustomTreeGraphicsViewIcon * initalizeAppendedIcon(DataIconManager *obj, CustomTreeGraphicsViewIcon * parentIcon);
};

#endif // BEHAVIORGRAPHUI_H
