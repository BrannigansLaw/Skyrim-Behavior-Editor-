#ifndef BEHAVIORGRAPHUI_H
#define BEHAVIORGRAPHUI_H

#include <QWidget>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QGraphicsItem>

#include "src/hkxclasses/hkxobject.h"

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
class GeneratorIcon;

class BehaviorGraphView: public QGraphicsView
{
    Q_OBJECT
    friend class GeneratorIcon;
    friend class HkDataUI;
    friend class ManualSelectorGeneratorUI;
    friend class ModifierGeneratorUI;
    friend class BSiStateTaggingGeneratorUI;
    friend class StateMachineUI;
    friend class StateUI;
public:
    BehaviorGraphView(HkDataUI *mainUI, BehaviorFile * file);
    virtual ~BehaviorGraphView();
    void writeToLog(const QString & message, bool isError = false);
    bool drawBehaviorGraph();
    bool reconnectBranch(HkxObject *oldChild, HkxObject *newChild, GeneratorIcon *icon);
    QSize sizeHint() const Q_DECL_OVERRIDE{
        return QSize(500, 400);
    }
    GeneratorIcon* getSelectedItem() const{
        return selectedIcon;
    }
    void removeGeneratorData();
    void removeModifierData();
    void removeOtherData();
protected:
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
signals:
    void iconSelected(GeneratorIcon *icon);
    void addedGenerator(const QString & name);
    void addedModifier(const QString & name);
    void removedGenerator(int index);
    void removedModifier(int index);
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
    void removeSelectedObjectBranch(GeneratorIcon *icon, GeneratorIcon *iconToKeep = NULL);
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
    void fixIconPositions(GeneratorIcon *rootIcon);
    //bool removeData(const HkxObjectExpSharedPtr & smtptr);
    void expandBranchForIcon(GeneratorIcon *icon);
    //bool drawBranch(GeneratorIcon * rootIcon);
    //void removeIcon(GeneratorIcon *iconToRemove);
    void removeChildIcons(GeneratorIcon *parent, GeneratorIcon *iconToKeep = NULL);
    void repositionIcons(GeneratorIcon * icon, bool updateNonVisable = false);
    void popUpMenuRequested(const QPoint &pos, GeneratorIcon *icon);
    void expandBranch(GeneratorIcon * icon, bool expandAll = false);
    void contractBranch(GeneratorIcon * icon, bool contractAll = false);
    HkxObject * getFirstChild(const HkxObjectExpSharedPtr &obj);
    HkxObjectExpSharedPtr * getFirstChildSmartPointer(HkxObject *obj);
    int manageIcons();
    bool positionIcon(GeneratorIcon * icon);
    void createIconBranch(QList <GeneratorIcon *> & icons, GeneratorIcon *parent);
    void deleteSelectedBranch(GeneratorIcon *icon, GeneratorIcon *iconToKeep = NULL);

    template <typename T> bool appendObject(T * ptr, GeneratorIcon *parentObjIcon);
    template <typename T> void wrapObject(T *obj, GeneratorIcon *parentObjIcon);
    template<typename T> int initializeIconsForNewBranch(const T & ptr, QList<HkxObjectExpSharedPtr> & objects, QList<GeneratorIcon *> & parentIcons, QVector <short> & objectChildCount);
    template<typename T> int initializeIcons(const T & ptr, QList<HkxObjectExpSharedPtr> & objects, QList<GeneratorIcon *> & parentIcons, QVector <short> & objectChildCount);
    template<typename T> GeneratorIcon * addIconToGraph(const HkxObjectExpSharedPtr & obj, const T & type, GeneratorIcon * parentIcon);
    template<typename T> GeneratorIcon * initalizeInjectedIcon(const HkxObjectExpSharedPtr & obj, const T & type, GeneratorIcon * parentIcon, GeneratorIcon * selected);
    template<typename T> GeneratorIcon * initalizeAppendedIcon(const HkxObjectExpSharedPtr & obj, const T & type, GeneratorIcon * parentIcon);
};

#endif // BEHAVIORGRAPHUI_H
