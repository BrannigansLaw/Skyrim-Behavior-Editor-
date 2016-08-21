#ifndef BEHAVIORGRAPHUI_H
#define BEHAVIORGRAPHUI_H

#include <QWidget>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QGraphicsItem>

#include "hkobject.h"

class FileSelectWindow;
class QVBoxLayout;
class QMenuBar;
class QMenu;
class QFile;
class QGraphicsScene;
class QGroupBox;
class QHBoxLayout;
class BehaviorFile;
class hkbManualSelectorGenerator;
class hkbBlenderGenerator;
class hkbBlenderGeneratorChild;
class hkbStateMachine;
class hkbStateMachineStateInfo;
class BSBoneSwitchGenerator;
class BSBoneSwitchGeneratorBoneData;
class hkbGenerator;
class HkDataUI;

class BehaviorGraphView: public QGraphicsView
{
    Q_OBJECT
    friend class GeneratorIcon;
    friend class HkDataUI;
public:
    BehaviorGraphView(HkDataUI *mainUI, BehaviorFile * file);
    bool drawBehaviorGraph();
    bool reconnectBranch(const HkObjectExpSharedPtr &oldChild, const HkObjectExpSharedPtr &newChild, GeneratorIcon *icon);
    QSize sizeHint() const Q_DECL_OVERRIDE{
        return QSize(500, 400);
    }
    GeneratorIcon* getSelectedItem() const{
        return selectedIcon;
    }
protected:
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
signals:
    void iconSelected(GeneratorIcon *icon);
private slots:
    void appendStateMachine();
    void appendManualSelectorGenerator();
    void appendModifierGenerator();
    void appendIStateTaggingGenerator();
    void appendSynchronizedClipGenerator();
    void appendOffsetAnimationGenerator();
    void appendCyclicBlendTransitionGenerator();
    void appendPoseMatchingGenerator();
    void appendBlenderGenerator();
    void appendBoneSwitchGenerator();
    void appendClipGenerator();
    void appendBehaviorReferenceGenerator();
    void wrapStateMachine();
    void wrapBlenderGenerator();
    void wrapPoseMatchingGenerator();

    void wrapManualSelectorGenerator();
    void wrapModifierGenerator();
    void wrapIStateTaggingGenerator();
    void wrapSynchronizedClipGenerator();
    void wrapOffsetAnimationGenerator();
    void wrapCyclicBlendTransitionGenerator();
    void wrapBoneSwitchGenerator();
    //void removeSelectedObject();
    void removeSelectedObjectBranchSlot();
private:
    void removeSelectedObjectBranch(GeneratorIcon *icon);
private:
    HkDataUI *ui;
    BehaviorFile *behavior;
    QGraphicsScene *behaviorGS;
    GeneratorIcon *selectedIcon;
    QMenu *contextMenu;
    QMenu *appendGeneratorMenu;
    QAction *appendSMAct;
    QAction *appendMSGAct;
    QAction *appendMGAct;
    QAction *appendBSISTGAct;
    QAction *appendBSSCGAct;
    QAction *appendBSOAGAct;
    QAction *appendBSCBTGAct;
    QAction *appendPMGAct;
    QAction *appendCGAct;
    QAction *appendBRGAct;
    QMenu *appendBlenderMenu;
    QAction *appendBGAct;
    QAction *appendBSBSGAct;
    QMenu *wrapGeneratorMenu;
    QAction *wrapSMAct;
    QAction *wrapMSGAct;
    QAction *wrapMGAct;
    QAction *wrapBSISTGAct;
    QAction *wrapBSSCGAct;
    QAction *wrapBSOAGAct;
    QAction *wrapBSCBTGAct;
    QAction *wrapPMGAct;
    QMenu *wrapBlenderMenu;
    QAction *wrapBGAct;
    QAction *wrapBSBSGAct;
    //QAction *removeObjAct;
    QAction *removeObjBranchAct;
    const qreal minScale;
    const qreal maxScale;
    const qreal initScale;
    const qreal iconFocusScale;
    qreal currentScale;
    const qreal scaleUpFactor;
    const qreal scaleDownFactor;
private:
    //bool removeData(const HkObjectExpSharedPtr & smtptr);
    void expandBranchForIcon(GeneratorIcon *icon);
    //bool drawBranch(GeneratorIcon * rootIcon);
    //void removeIcon(GeneratorIcon *iconToRemove);
    void removeChildIcons(GeneratorIcon *parent);
    void repositionIcons(GeneratorIcon * icon, bool updateNonVisable = false);
    void popUpMenuRequested(const QPoint &pos, GeneratorIcon *icon);
    void expandBranch(GeneratorIcon * icon, bool expandAll = false);
    void contractBranch(GeneratorIcon * icon, bool contractAll = false);
    HkObject * getFirstChild(const HkObjectExpSharedPtr &obj);
    HkObjectExpSharedPtr * getFirstChildSmartPointer(HkObject *obj);
    int manageIcons();
    bool positionIcon(GeneratorIcon * icon);
    void createIconBranch(QList <GeneratorIcon *> & icons, GeneratorIcon *parent);
    void deleteSelectedBranch(GeneratorIcon *icon);

    template <typename T> bool appendObject(T * ptr, GeneratorIcon *parentObjIcon);
    template <typename T> void wrapObject(T *obj, GeneratorIcon *parentObjIcon);
    template<typename T> int initializeIconsForNewBranch(const T & ptr, QList<HkObjectExpSharedPtr> & objects, QList<GeneratorIcon *> & parentIcons, QVector <short> & objectChildCount);
    template<typename T> int initializeIcons(const T & ptr, QList<HkObjectExpSharedPtr> & objects, QList<GeneratorIcon *> & parentIcons, QVector <short> & objectChildCount);
    template<typename T> GeneratorIcon * addIconToGraph(const HkObjectExpSharedPtr & obj, const T & type, GeneratorIcon * parentIcon);
    template<typename T> GeneratorIcon * initalizeInjectedIcon(const HkObjectExpSharedPtr & obj, const T & type, GeneratorIcon * parentIcon, GeneratorIcon * selected);
    template<typename T> GeneratorIcon * initalizeAppendedIcon(const HkObjectExpSharedPtr & obj, const T & type, GeneratorIcon * parentIcon);
};

#endif // BEHAVIORGRAPHUI_H
