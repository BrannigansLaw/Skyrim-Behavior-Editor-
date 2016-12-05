#ifndef BEHAVIORGRAPHVIEW_H
#define BEHAVIORGRAPHVIEW_H

#include "src/hkxclasses/hkxobject.h"
#include "src/ui/customtreegraphicsview.h"

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
class BehaviorGraphViewIcon;

class BehaviorGraphView: public CustomTreeGraphicsView
{
    Q_OBJECT
    friend class BehaviorGraphViewIcon;
    friend class HkDataUI;
    friend class ManualSelectorGeneratorUI;
    friend class ModifierGeneratorUI;
    friend class BSiStateTaggingGeneratorUI;
    friend class StateMachineUI;
    friend class StateUI;
    friend class TransitionsUI;
    friend class CustomTreeGraphicsViewIcon;
public:
    BehaviorGraphView(HkDataUI *mainUI, BehaviorFile * file);
    virtual ~BehaviorGraphView();
    void writeToLog(const QString & message, bool isError = false);
    QStringList getEventNames() const;
    void removeGeneratorData();
    void removeModifierData();
    void removeOtherData();
signals:
    //void iconSelected(BehaviorGraphViewIcon *icon);
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
    void deleteSelectedObjectBranchSlot();
    void popUpMenuRequested(const QPoint &pos, CustomTreeGraphicsViewIcon *icon);
private:
    HkDataUI *ui;
    BehaviorFile *behavior;
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
    QAction *removeObjBranchAct;
};

#endif // BEHAVIORGRAPHVIEW_H
