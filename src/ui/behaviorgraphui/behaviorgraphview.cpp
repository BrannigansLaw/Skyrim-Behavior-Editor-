#include "behaviorgraphview.h"
#include "src/ui/mainwindow.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/behavior/generators/hkbgenerator.h"
#include "src/ui/behaviorgraphui/behaviorgraphview.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/ui/behaviorgraphui/customtreegraphicsviewicon.h"
#include "src/ui//hkxclassesui/hkdataui.h"

#include "src/hkxclasses/behavior/generators/hkbstatemachinestateinfo.h"
#include "src/hkxclasses/behavior/generators/hkbstatemachine.h"
#include "src/hkxclasses/behavior/generators/hkbmodifiergenerator.h"
#include "src/hkxclasses/behavior/generators/bsistatetagginggenerator.h"
#include "src/hkxclasses/behavior/generators/bscyclicblendtransitiongenerator.h"
#include "src/hkxclasses/behavior/generators/bsboneswitchgenerator.h"
#include "src/hkxclasses/behavior/generators/bsboneswitchgeneratorbonedata.h"
#include "src/hkxclasses/behavior/generators/bssynchronizedclipgenerator.h"
#include "src/hkxclasses/behavior/generators/hkbmanualselectorgenerator.h"
#include "src/hkxclasses/behavior/generators/hkbblendergeneratorchild.h"
#include "src/hkxclasses/behavior/hkbboneweightarray.h"
#include "src/hkxclasses/behavior/generators/hkbblendergenerator.h"
#include "src/hkxclasses/behavior/generators/bsoffsetanimationgenerator.h"
#include "src/hkxclasses/behavior/generators/hkbposematchinggenerator.h"
#include "src/hkxclasses/behavior/generators/hkbclipgenerator.h"
#include "src/hkxclasses/behavior/generators/hkbbehaviorreferencegenerator.h"
#include "src/hkxclasses/behavior/hkbstatemachinetransitioninfoarray.h"
#include "src/hkxclasses/behavior/hkbstatemachineeventpropertyarray.h"
#include "src/hkxclasses/behavior/generators/hkbbehaviorgraph.h"
#include "src/hkxclasses/behavior/hkbbehaviorgraphdata.h"
#include "src/hkxclasses/behavior/hkbbehaviorgraphstringdata.h"
#include "src/hkxclasses/behavior/hkbvariablevalueset.h"
#include "src/hkxclasses/hkrootlevelcontainer.h"

#include <QBoxLayout>
#include <QMenuBar>
#include <QFile>
#include <QSettings>
#include <QGroupBox>
#include <QMessageBox>
#include <QGraphicsSceneMouseEvent>
#include <QSize>

/**
 * BehaviorGraphView
 */

BehaviorGraphView::BehaviorGraphView(HkDataUI *mainUI, BehaviorFile * file)
    : CustomTreeGraphicsView(),
      ui(mainUI),
      behavior(file),
      contextMenu(new QMenu(this)),
      appendGeneratorMenu(new QMenu("Append Generator:", contextMenu)),
      appendSMAct(new QAction("State Machine", appendGeneratorMenu)),
      appendMSGAct(new QAction("Manual Selector Generator", appendGeneratorMenu)),
      appendMGAct(new QAction("Modifier Generator", appendGeneratorMenu)),
      appendBSISTGAct(new QAction("BS iState Tagging Generator", appendGeneratorMenu)),
      appendBSSCGAct(new QAction("BS Synchronized Clip Generator", appendGeneratorMenu)),
      appendBSOAGAct(new QAction("BS Offset Animation Generator", appendGeneratorMenu)),
      appendBSCBTGAct(new QAction("BS Cyclic Blend Transition Generator", appendGeneratorMenu)),
      appendPMGAct(new QAction("Pose Matching Generator", appendGeneratorMenu)),
      appendCGAct(new QAction("Clip Generator", appendGeneratorMenu)),
      appendBRGAct(new QAction("Behavior Reference Generator", appendGeneratorMenu)),
      appendBlenderMenu(new QMenu("Append Blend:", contextMenu)),
      appendBGAct(new QAction("Blender Generator", appendBlenderMenu)),
      appendBSBSGAct(new QAction("BS Bone Switch Generator", appendBlenderMenu)),
      wrapGeneratorMenu(new QMenu("Wrap inside Generator:", contextMenu)),
      wrapSMAct(new QAction("State Machine", wrapGeneratorMenu)),
      wrapMSGAct(new QAction("Manual Selector Generator", wrapGeneratorMenu)),
      wrapMGAct(new QAction("Modifier Generator", wrapGeneratorMenu)),
      wrapBSISTGAct(new QAction("BS iState Tagging Generator", wrapGeneratorMenu)),
      wrapBSSCGAct(new QAction("BS Synchronized Clip Generator", wrapGeneratorMenu)),
      wrapBSOAGAct(new QAction("BS Offset Animation Generator", wrapGeneratorMenu)),
      wrapBSCBTGAct(new QAction("BS Cyclic Blend Transition Generator", wrapGeneratorMenu)),
      wrapPMGAct(new QAction("Pose Matching Generator", wrapGeneratorMenu)),
      wrapBlenderMenu(new QMenu("Wrap inside Blend:", contextMenu)),
      wrapBGAct(new QAction("Blender Generator", wrapBlenderMenu)),
      wrapBSBSGAct(new QAction("BS Bone Switch Generator", wrapBlenderMenu)),
      removeObjBranchAct(new QAction("Remove Selected Object and Children", contextMenu))
{
    contextMenu->addMenu(appendGeneratorMenu);
    appendGeneratorMenu->addAction(appendSMAct);
    appendGeneratorMenu->addAction(appendMSGAct);
    appendGeneratorMenu->addAction(appendMGAct);
    appendGeneratorMenu->addAction(appendBSISTGAct);
    appendGeneratorMenu->addAction(appendBSSCGAct);
    appendGeneratorMenu->addAction(appendBSOAGAct);
    appendGeneratorMenu->addAction(appendBSCBTGAct);
    appendGeneratorMenu->addAction(appendPMGAct);
    appendGeneratorMenu->addAction(appendCGAct);
    appendGeneratorMenu->addAction(appendBRGAct);
    contextMenu->addMenu(appendBlenderMenu);
    appendBlenderMenu->addAction(appendBGAct);
    appendBlenderMenu->addAction(appendBSBSGAct);
    contextMenu->addMenu(wrapGeneratorMenu);
    wrapGeneratorMenu->addAction(wrapSMAct);
    wrapGeneratorMenu->addAction(wrapMSGAct);
    wrapGeneratorMenu->addAction(wrapMGAct);
    wrapGeneratorMenu->addAction(wrapBSISTGAct);
    wrapGeneratorMenu->addAction(wrapBSSCGAct);
    wrapGeneratorMenu->addAction(wrapBSOAGAct);
    wrapGeneratorMenu->addAction(wrapBSCBTGAct);
    wrapGeneratorMenu->addAction(wrapPMGAct);
    contextMenu->addMenu(wrapBlenderMenu);
    wrapBlenderMenu->addAction(wrapBGAct);
    wrapBlenderMenu->addAction(wrapBSBSGAct);
    contextMenu->addAction(removeObjBranchAct);
    setContextMenu(contextMenu);
    connect(appendSMAct, SIGNAL(triggered()), this, SLOT(appendStateMachine()));
    connect(appendMSGAct, SIGNAL(triggered()), this, SLOT(appendManualSelectorGenerator()));
    connect(appendMGAct, SIGNAL(triggered()), this, SLOT(appendModifierGenerator()));
    connect(appendBSISTGAct, SIGNAL(triggered()), this, SLOT(appendIStateTaggingGenerator()));
    connect(appendBSSCGAct, SIGNAL(triggered()), this, SLOT(appendSynchronizedClipGenerator()));
    connect(appendBSOAGAct, SIGNAL(triggered()), this, SLOT(appendOffsetAnimationGenerator()));
    connect(appendBSCBTGAct, SIGNAL(triggered()), this, SLOT(appendCyclicBlendTransitionGenerator()));
    connect(appendPMGAct, SIGNAL(triggered()), this, SLOT(appendPoseMatchingGenerator()));
    connect(appendBGAct, SIGNAL(triggered()), this, SLOT(appendBlenderGenerator()));
    connect(appendBSBSGAct, SIGNAL(triggered()), this, SLOT(appendBoneSwitchGenerator()));
    connect(appendCGAct, SIGNAL(triggered()), this, SLOT(appendClipGenerator()));
    connect(appendBRGAct, SIGNAL(triggered()), this, SLOT(appendBehaviorReferenceGenerator()));
    connect(wrapMSGAct, SIGNAL(triggered()), this, SLOT(wrapManualSelectorGenerator()));
    connect(wrapMGAct, SIGNAL(triggered()), this, SLOT(wrapModifierGenerator()));
    connect(wrapBSISTGAct, SIGNAL(triggered()), this, SLOT(wrapIStateTaggingGenerator()));
    connect(wrapBSSCGAct, SIGNAL(triggered()), this, SLOT(wrapSynchronizedClipGenerator()));
    connect(wrapBSOAGAct, SIGNAL(triggered()), this, SLOT(wrapOffsetAnimationGenerator()));
    connect(wrapBSCBTGAct, SIGNAL(triggered()), this, SLOT(wrapCyclicBlendTransitionGenerator()));
    connect(wrapBSBSGAct, SIGNAL(triggered()), this, SLOT(wrapBoneSwitchGenerator()));
    connect(wrapSMAct, SIGNAL(triggered()), this, SLOT(wrapStateMachine()));
    connect(wrapBGAct, SIGNAL(triggered()), this, SLOT(wrapBlenderGenerator()));
    connect(wrapPMGAct, SIGNAL(triggered()), this, SLOT(wrapPoseMatchingGenerator()));
    connect(removeObjBranchAct, SIGNAL(triggered()), this, SLOT(deleteSelectedObjectBranchSlot()));
    if (ui){
        ui->setBehaviorView(this);
        //connect(this, SIGNAL(iconSelected(BehaviorGraphViewIcon *)), ui, SLOT(changeCurrentDataWidget(BehaviorGraphViewIcon *)));
    }
}

void BehaviorGraphView::writeToLog(const QString &message, bool isError){
    behavior->writeToLog(message, isError);
}

QStringList BehaviorGraphView::getEventNames() const{
    return behavior->getEventNames();
}

void BehaviorGraphView::removeGeneratorData(){
    QVector <int> removedIndices;
    removedIndices = behavior->removeGeneratorData();
    for (int i = 0; i < removedIndices.size(); i++){
        emit removedGenerator(removedIndices.at(i) + 1);
    }
}

void BehaviorGraphView::removeModifierData(){
    QVector <int> removedIndices = behavior->removeModifierData();
    for (int i = 0; i < removedIndices.size(); i++){
        emit removedModifier(removedIndices.at(i) + 1);
    }
}

void BehaviorGraphView::removeOtherData(){
    behavior->removeOtherData();
}

void BehaviorGraphView::deleteSelectedObjectBranchSlot(){
    removeSelectedObjectBranchSlot();
    ui->changeCurrentDataWidget(NULL);
    removeGeneratorData();
    removeModifierData();
    behavior->removeOtherData();
}

void BehaviorGraphView::appendStateMachine(){
    if (selectedIcon && selectedIcon->data.constData()){
        hkbStateMachine *obj = new hkbStateMachine(behavior);
        obj->evaulateDataValidity();
        if (!appendObject(obj, selectedIcon)){
            delete obj;
        }
        if (selectedIcon->parent && selectedIcon->parent->data.data()){
            selectedIcon->parent->data.data()->evaulateDataValidity();
        }
        selectedIcon->data.data()->evaulateDataValidity();
        emit addedGenerator(obj->getName());
    }
}

void BehaviorGraphView::appendManualSelectorGenerator(){
    if (selectedIcon && selectedIcon->data.constData()){
        hkbManualSelectorGenerator *obj = new hkbManualSelectorGenerator(behavior);
        obj->evaulateDataValidity();
        if (!appendObject(obj, selectedIcon)){
            delete obj;
        }
        if (selectedIcon->parent && selectedIcon->parent->data.data()){
            selectedIcon->parent->data.data()->evaulateDataValidity();
        }
        selectedIcon->data.data()->evaulateDataValidity();
        emit addedGenerator(obj->getName());
    }
}

void BehaviorGraphView::appendModifierGenerator(){
    if (selectedIcon && selectedIcon->data.constData()){
        hkbModifierGenerator *obj = new hkbModifierGenerator(behavior);
        obj->evaulateDataValidity();
        if (!appendObject(obj, selectedIcon)){
            delete obj;
        }
        if (selectedIcon->parent && selectedIcon->parent->data.data()){
            selectedIcon->parent->data.data()->evaulateDataValidity();
        }
        selectedIcon->data.data()->evaulateDataValidity();
        emit addedGenerator(obj->getName());
    }
}

void BehaviorGraphView::appendIStateTaggingGenerator(){
    if (selectedIcon && selectedIcon->data.constData()){
        BSiStateTaggingGenerator *obj = new BSiStateTaggingGenerator(behavior);
        obj->evaulateDataValidity();
        if (!appendObject(obj, selectedIcon)){
            delete obj;
        }
        if (selectedIcon->parent && selectedIcon->parent->data.data()){
            selectedIcon->parent->data.data()->evaulateDataValidity();
        }
        selectedIcon->data.data()->evaulateDataValidity();
        emit addedGenerator(obj->getName());
    }
}

void BehaviorGraphView::appendSynchronizedClipGenerator(){
    if (selectedIcon && selectedIcon->data.constData()){
        BSSynchronizedClipGenerator *obj = new BSSynchronizedClipGenerator(behavior);
        obj->evaulateDataValidity();
        if (!appendObject(obj, selectedIcon)){
            delete obj;
        }
        if (selectedIcon->parent && selectedIcon->parent->data.data()){
            selectedIcon->parent->data.data()->evaulateDataValidity();
        }
        selectedIcon->data.data()->evaulateDataValidity();
        emit addedGenerator(obj->getName());
    }
}

void BehaviorGraphView::appendOffsetAnimationGenerator(){
    if (selectedIcon && selectedIcon->data.constData()){
        BSOffsetAnimationGenerator *obj = new BSOffsetAnimationGenerator(behavior);
        obj->evaulateDataValidity();
        if (!appendObject(obj, selectedIcon)){
            delete obj;
        }
        if (selectedIcon->parent && selectedIcon->parent->data.data()){
            selectedIcon->parent->data.data()->evaulateDataValidity();
        }
        selectedIcon->data.data()->evaulateDataValidity();
        emit addedGenerator(obj->getName());
    }
}

void BehaviorGraphView::appendCyclicBlendTransitionGenerator(){
    if (selectedIcon && selectedIcon->data.constData()){
        BSCyclicBlendTransitionGenerator *obj = new BSCyclicBlendTransitionGenerator(behavior);
        obj->evaulateDataValidity();
        if (!appendObject(obj, selectedIcon)){
            delete obj;
        }
        if (selectedIcon->parent && selectedIcon->parent->data.data()){
            selectedIcon->parent->data.data()->evaulateDataValidity();
        }
        selectedIcon->data.data()->evaulateDataValidity();
        emit addedGenerator(obj->getName());
    }
}

void BehaviorGraphView::appendPoseMatchingGenerator(){
    if (selectedIcon && selectedIcon->data.constData()){
        hkbPoseMatchingGenerator *obj = new hkbPoseMatchingGenerator(behavior);
        obj->evaulateDataValidity();
        if (!appendObject(obj, selectedIcon)){
            delete obj;
        }
        if (selectedIcon->parent && selectedIcon->parent->data.data()){
            selectedIcon->parent->data.data()->evaulateDataValidity();
        }
        selectedIcon->data.data()->evaulateDataValidity();
        emit addedGenerator(obj->getName());
    }
}

void BehaviorGraphView::appendBlenderGenerator(){
    if (selectedIcon && selectedIcon->data.constData()){
        hkbBlenderGenerator *obj = new hkbBlenderGenerator(behavior);
        obj->evaulateDataValidity();
        if (!appendObject(obj, selectedIcon)){
            delete obj;
        }
        if (selectedIcon->parent && selectedIcon->parent->data.data()){
            selectedIcon->parent->data.data()->evaulateDataValidity();
        }
        selectedIcon->data.data()->evaulateDataValidity();
        emit addedGenerator(obj->getName());
    }
}

void BehaviorGraphView::appendBoneSwitchGenerator(){
    if (selectedIcon && selectedIcon->data.constData()){
        BSBoneSwitchGenerator *obj = new BSBoneSwitchGenerator(behavior);
        obj->evaulateDataValidity();
        if (!appendObject(obj, selectedIcon)){
            delete obj;
        }
        if (selectedIcon->parent && selectedIcon->parent->data.data()){
            selectedIcon->parent->data.data()->evaulateDataValidity();
        }
        selectedIcon->data.data()->evaulateDataValidity();
        emit addedGenerator(obj->getName());
    }
}

void BehaviorGraphView::appendClipGenerator(){
    if (selectedIcon && selectedIcon->data.constData()){
        hkbClipGenerator *obj = new hkbClipGenerator(behavior);
        if (!appendObject(obj, selectedIcon)){
            delete obj;
        }
        if (selectedIcon->parent && selectedIcon->parent->data.data()){
            selectedIcon->parent->data.data()->evaulateDataValidity();
        }
        selectedIcon->data.data()->evaulateDataValidity();
        emit addedGenerator(obj->getName());
    }
}

void BehaviorGraphView::appendBehaviorReferenceGenerator(){
    if (selectedIcon && selectedIcon->data.constData()){
        hkbBehaviorReferenceGenerator *obj = new hkbBehaviorReferenceGenerator(behavior);
        if (!appendObject(obj, selectedIcon)){
            delete obj;
        }
        if (selectedIcon->parent && selectedIcon->parent->data.data()){
            selectedIcon->parent->data.data()->evaulateDataValidity();
        }
        selectedIcon->data.data()->evaulateDataValidity();
        emit addedGenerator(obj->getName());
    }
}

void BehaviorGraphView::wrapManualSelectorGenerator(){
    if (selectedIcon && selectedIcon->parent && selectedIcon->parent->data.constData()){
        hkbManualSelectorGenerator *obj = new hkbManualSelectorGenerator(behavior);
        wrapObject(obj, selectedIcon);
        emit addedGenerator(obj->getName());
    }
}

void BehaviorGraphView::wrapModifierGenerator(){
    if (selectedIcon && selectedIcon->parent && selectedIcon->parent->data.constData()){
        hkbModifierGenerator *obj = new hkbModifierGenerator(behavior);
        wrapObject(obj, selectedIcon);
        emit addedGenerator(obj->getName());
    }
}

void BehaviorGraphView::wrapIStateTaggingGenerator(){
    if (selectedIcon && selectedIcon->parent && selectedIcon->parent->data.constData()){
        BSiStateTaggingGenerator *obj = new BSiStateTaggingGenerator(behavior);
        wrapObject(obj, selectedIcon);
        emit addedGenerator(obj->getName());
    }
}

void BehaviorGraphView::wrapSynchronizedClipGenerator(){
    if (selectedIcon && selectedIcon->parent && selectedIcon->parent->data.constData()){
        BSSynchronizedClipGenerator *obj = new BSSynchronizedClipGenerator(behavior);
        wrapObject(obj, selectedIcon);
        emit addedGenerator(obj->getName());
    }
}

void BehaviorGraphView::wrapOffsetAnimationGenerator(){
    if (selectedIcon && selectedIcon->parent && selectedIcon->parent->data.constData()){
        BSOffsetAnimationGenerator *obj = new BSOffsetAnimationGenerator(behavior);
        wrapObject(obj, selectedIcon);
        emit addedGenerator(obj->getName());
    }
}

void BehaviorGraphView::wrapCyclicBlendTransitionGenerator(){
    if (selectedIcon && selectedIcon->parent && selectedIcon->parent->data.constData()){
        BSCyclicBlendTransitionGenerator *obj = new BSCyclicBlendTransitionGenerator(behavior);
        wrapObject(obj, selectedIcon);
        emit addedGenerator(obj->getName());
    }
}

void BehaviorGraphView::wrapBoneSwitchGenerator(){
    if (selectedIcon && selectedIcon->parent && selectedIcon->parent->data.constData()){
        BSBoneSwitchGenerator *obj = new BSBoneSwitchGenerator(behavior);
        obj->evaulateDataValidity();
        wrapObject(obj, selectedIcon);
        emit addedGenerator(obj->getName());
    }
}

void BehaviorGraphView::wrapStateMachine(){
    if (selectedIcon && selectedIcon->parent && selectedIcon->parent->data.constData()){
        hkbStateMachine *obj = new hkbStateMachine(behavior);
        hkbStateMachineStateInfo *state = new hkbStateMachineStateInfo(behavior, obj);
        obj->states.append(HkxObjectExpSharedPtr(state));
        wrapObject(obj, selectedIcon);
        emit addedGenerator(obj->getName());
    }
}

void BehaviorGraphView::wrapBlenderGenerator(){
    if (selectedIcon && selectedIcon->parent && selectedIcon->parent->data.constData()){
        hkbBlenderGenerator *obj = new hkbBlenderGenerator(behavior);
        hkbBlenderGeneratorChild *blendChild = new hkbBlenderGeneratorChild(behavior);
        obj->children.append(HkxObjectExpSharedPtr(blendChild));
        wrapObject(obj, selectedIcon);
        emit addedGenerator(obj->getName());
    }
}

void BehaviorGraphView::wrapPoseMatchingGenerator(){
    if (selectedIcon && selectedIcon->parent && selectedIcon->parent->data.constData()){
        hkbPoseMatchingGenerator *obj = new hkbPoseMatchingGenerator(behavior);
        hkbBlenderGeneratorChild *blendChild = new hkbBlenderGeneratorChild(behavior);
        obj->children.append(HkxObjectExpSharedPtr(blendChild));
        wrapObject(obj, selectedIcon);
        emit addedGenerator(obj->getName());
    }
}

void BehaviorGraphView::popUpMenuRequested(const QPoint &pos, CustomTreeGraphicsViewIcon *icon){
    qulonglong sig = icon->data.constData()->getSignature();
    if (sig == HKB_BLENDER_GENERATOR){
        wrapBSCBTGAct->setVisible(true);

        appendSMAct->setVisible(true);
        appendMSGAct->setVisible(true);
        appendMGAct->setVisible(true);
        appendBSISTGAct->setVisible(true);
        appendBSSCGAct->setVisible(true);
        appendBSOAGAct->setVisible(true);
        appendBSCBTGAct->setVisible(true);
        appendPMGAct->setVisible(true);
        appendBGAct->setVisible(true);
        appendBSBSGAct->setVisible(true);
        appendCGAct->setVisible(true);
        appendBRGAct->setVisible(true);

        if (icon->parent && icon->parent->data.constData()->getSignature() == HKB_BEHAVIOR_GRAPH){
            wrapMSGAct->setVisible(false);
            wrapMGAct->setVisible(false);
            wrapBSISTGAct->setVisible(false);
            wrapBSSCGAct->setVisible(false);
            wrapBSOAGAct->setVisible(false);
            wrapBSCBTGAct->setVisible(false);
            wrapPMGAct->setVisible(false);
            wrapBGAct->setVisible(false);
            wrapBSBSGAct->setVisible(false);
        }

        removeObjBranchAct->setVisible(true);
    }else if (sig == HKB_CLIP_GENERATOR){

        appendSMAct->setVisible(false);
        appendMSGAct->setVisible(false);
        appendMGAct->setVisible(false);
        appendBSISTGAct->setVisible(false);
        appendBSSCGAct->setVisible(false);
        appendBSOAGAct->setVisible(false);
        appendBSCBTGAct->setVisible(false);
        appendPMGAct->setVisible(false);
        appendBGAct->setVisible(false);
        appendBSBSGAct->setVisible(false);
        appendCGAct->setVisible(false);
        appendBRGAct->setVisible(false);

        if (icon->parent && icon->parent->data.constData()->getSignature() == BS_SYNCHRONIZED_CLIP_GENERATOR || icon->parent->data.constData()->getSignature() == BS_OFFSET_ANIMATION_GENERATOR){
            wrapBSSCGAct->setVisible(false);
            wrapBSOAGAct->setVisible(false);
        }else{
            wrapBSSCGAct->setVisible(true);
            wrapBSOAGAct->setVisible(true);
        }
        removeObjBranchAct->setVisible(true);
    }else if (sig == BS_CYCLIC_BLEND_TRANSITION_GENERATOR){

        appendSMAct->setVisible(false);
        appendMSGAct->setVisible(false);
        appendMGAct->setVisible(false);
        appendBSISTGAct->setVisible(false);
        appendBSSCGAct->setVisible(false);
        appendBSOAGAct->setVisible(false);
        appendBSCBTGAct->setVisible(false);
        appendPMGAct->setVisible(false);
        appendBSBSGAct->setVisible(false);
        appendCGAct->setVisible(false);
        appendBRGAct->setVisible(false);
        removeObjBranchAct->setVisible(true);
    }else if (sig == BS_SYNCHRONIZED_CLIP_GENERATOR){

        appendSMAct->setVisible(false);
        appendMSGAct->setVisible(false);
        appendMGAct->setVisible(false);
        appendBSISTGAct->setVisible(false);
        appendBSSCGAct->setVisible(false);
        appendBSOAGAct->setVisible(false);
        appendBSCBTGAct->setVisible(false);
        appendPMGAct->setVisible(false);
        appendBGAct->setVisible(false);
        appendBSBSGAct->setVisible(false);
        appendBRGAct->setVisible(false);
        removeObjBranchAct->setVisible(true);
    }else if (sig == HKB_BEHAVIOR_GRAPH){

        appendMSGAct->setVisible(false);
        appendMGAct->setVisible(false);
        appendBSISTGAct->setVisible(false);
        appendBSSCGAct->setVisible(false);
        appendBSOAGAct->setVisible(false);
        appendBSCBTGAct->setVisible(false);
        appendPMGAct->setVisible(false);
        appendBGAct->setVisible(false);
        appendBSBSGAct->setVisible(false);
        appendCGAct->setVisible(false);
        appendBRGAct->setVisible(false);

        wrapSMAct->setVisible(false);
        wrapMSGAct->setVisible(false);
        wrapMGAct->setVisible(false);
        wrapBSISTGAct->setVisible(false);
        wrapBSSCGAct->setVisible(false);
        wrapBSOAGAct->setVisible(false);
        wrapBSCBTGAct->setVisible(false);
        wrapPMGAct->setVisible(false);
        wrapBGAct->setVisible(false);
        wrapBSBSGAct->setVisible(false);

        removeObjBranchAct->setVisible(false);
    }else{

        appendSMAct->setVisible(true);
        appendMSGAct->setVisible(true);
        appendMGAct->setVisible(true);
        appendBSISTGAct->setVisible(true);
        appendBSSCGAct->setVisible(true);
        appendBSOAGAct->setVisible(true);
        appendBSCBTGAct->setVisible(true);
        appendPMGAct->setVisible(true);
        appendBGAct->setVisible(true);
        appendBSBSGAct->setVisible(true);
        appendCGAct->setVisible(true);
        appendBRGAct->setVisible(true);

        wrapMSGAct->setVisible(true);
        wrapMGAct->setVisible(true);
        wrapBSISTGAct->setVisible(true);
        wrapPMGAct->setVisible(true);
        wrapBGAct->setVisible(true);
        wrapBSBSGAct->setVisible(true);

        removeObjBranchAct->setVisible(true);

        wrapBSCBTGAct->setVisible(false);
        wrapBSSCGAct->setVisible(false);
        wrapBSOAGAct->setVisible(false);
    }
    contextMenu->popup(viewport()->mapToGlobal(pos));
}

BehaviorGraphView::~BehaviorGraphView(){
    ui->setBehaviorView(NULL);
}
