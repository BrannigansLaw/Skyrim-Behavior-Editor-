#include "mainwindow.h"
#include "fileselectwindow.h"
#include "hkxfile.h"
#include "generators.h"
#include "behaviorgraphui.h"
#include "hkobject.h"
#include "behaviorgraphicons.h"

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

BehaviorGraphView::BehaviorGraphView(BehaviorFile * file)
    : behavior(file),
      behaviorGS(new QGraphicsScene),
      selectedIcon(NULL),
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
      //removeObjAct(new QAction("Remove Selected Object", contextMenu)),
      removeObjBranchAct(new QAction("Remove Selected Object and Children", contextMenu)),
      minScale(0.001),
      maxScale(4),
      initScale(1),
      iconFocusScale(1.5),
      currentScale(1),
      scaleUpFactor(1.2),
      scaleDownFactor(0.8)
{
    setMinimumHeight(200);
    setMinimumWidth(200);
    setScene(behaviorGS);
    setContextMenuPolicy(Qt::CustomContextMenu);
    GeneratorIcon::updateStaticMembers();
    contextMenu->addMenu(appendGeneratorMenu);
    appendGeneratorMenu->addAction(appendSMAct);
    appendGeneratorMenu->addAction(appendMSGAct);
    appendGeneratorMenu->addAction(appendMGAct);
    appendGeneratorMenu->addAction(appendBSISTGAct);
    appendGeneratorMenu->addAction(appendBSSCGAct);
    appendGeneratorMenu->addAction(appendBSOAGAct);
    appendGeneratorMenu->addAction(appendBSCBTGAct);
    appendGeneratorMenu->addAction(appendPMGAct);
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
    //contextMenu->addAction(removeObjAct);
    contextMenu->addAction(removeObjBranchAct);
    connect(appendSMAct, SIGNAL(triggered()), this, SLOT(appendStateMachine()));
    connect(wrapSMAct, SIGNAL(triggered()), this, SLOT(wrapStateMachine()));
    //connect(removeObjAct, SIGNAL(triggered()), this, SLOT(removeSelectedObject()));
    connect(removeObjBranchAct, SIGNAL(triggered()), this, SLOT(removeSelectedObjectBranch()));
}

void BehaviorGraphView::contractBranch(GeneratorIcon * icon, bool contractAll){
    if (!icon){
        return;
    }
    for (int i = 0; i < icon->children.size(); i++){
        icon->children.at(i)->setVisible(false);
        if (icon->children.at(i)->linkToParent){
            icon->children.at(i)->linkToParent->setVisible(false);
        }
        if (contractAll){
            icon->children.at(i)->isExpanded = false;
            contractBranch(icon->children.at(i), true);
        }else{
            contractBranch(icon->children.at(i));
        }
    }
}

void BehaviorGraphView::expandBranch(GeneratorIcon * icon, bool expandAll){
    if (!icon){
        return;
    }
    icon->isExpanded = true;
    for (int i = 0; i < icon->children.size(); i++){
        icon->children.at(i)->setVisible(true);
        if (icon->children.at(i)->linkToParent){
            icon->children.at(i)->linkToParent->setVisible(true);
        }
        if (expandAll){
            expandBranch(icon->children.at(i), true);
        }else if (icon->children.at(i)->isExpanded){
            expandBranch(icon->children.at(i));
        }
    }
}

void BehaviorGraphView::createIconBranch(QList <GeneratorIcon *> & icons, GeneratorIcon *parent){
    icons.append(parent);
    GeneratorIcon *last = icons.last();
    for (int i = 0; i < last->children.size(); i++){
        createIconBranch(icons, last->children.at(i));
    }
}

void BehaviorGraphView::removeIcon(GeneratorIcon *iconToRemove){
    /*GeneratorIcon *icon = NULL;
    hkbGenerator *gen = static_cast<hkbGenerator *>(iconToRemove->data.data());
    int index;
    if (iconToRemove->children.size() > 0 && gen->icons.size() > 1){
        icon = gen->icons.at(1);
        gen->icons[1] = iconToRemove;
        index = icon->parent->children.indexOf(icon);
        icon->parent->children.replace(index, iconToRemove);
        iconToRemove->parent->children.removeAll(iconToRemove);
        iconToRemove->parent = icon->parent;
        iconToRemove->setPos(icon->pos() += QPointF(1.5*icon->boundingRect().width(), 2*icon->boundingRect().height()));
        iconToRemove->parent->linkToParent = icon->parent->linkToParent;
        gen->icons.removeFirst();
        gen->icons.removeAll(icon);
        iconToRemove = icon;
    }else{
        //Create duplicate icon for first child of selected icon and replace selected icon with the duplicate
        index = selectedIcon->parent->children.indexOf(selectedIcon);
        if (index < 0){
            return;
        }
        hkbGenerator *ptr = static_cast<hkbGenerator *>(getFirstChild(iconToRemove->data.data()));
        iconToRemove->parent->children.replace(index, new GeneratorIcon(HkObjectExpSharedPtr(ptr), ));
        iconToRemove->children.at(0)->parent = iconToRemove->parent;
    }
    if (iconToRemove->parent && !iconToRemove->parent->children.isEmpty()){
        iconToRemove->parent->children.removeAll(iconToRemove);
    }
    gen->icons.removeAll(iconToRemove);
    behaviorGS->removeItem(iconToRemove);
    behaviorGS->removeItem(iconToRemove->linkToParent);
    delete iconToRemove->linkToParent;
    delete iconToRemove;
    iconToRemove = NULL;*/
}

void BehaviorGraphView::removeChildIcons(GeneratorIcon *parent){
    if (!parent){
        return;
    }
    GeneratorIcon *icon;
    hkbGenerator *gen;
    qreal lastY = 0;
    QList <GeneratorIcon *> icons;
    QList <GeneratorIcon *> branch;
    createIconBranch(icons, parent);
    int index;
    for (int j = 0; j < icons.size(); j++){ //Add search function for icons by name!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        gen = static_cast<hkbGenerator *>(icons.at(j)->data.data());
        if (icons.at(j)->children.size() > 0){
            for (int x = 0; x < gen->icons.size(); x++){
                if (!icons.contains(gen->icons.at(x))){
                    if (lastY == 0 || lastY > gen->icons.at(x)->y()){
                        lastY = gen->icons.at(x)->y();
                        icon = gen->icons.at(x);
                        gen->icons[x] = icons.at(j);
                        index = icon->parent->children.indexOf(icon);
                        icon->parent->children.replace(index, icons.at(j));
                        icons.at(j)->parent->children.removeAll(icons.at(j));
                        icons.at(j)->parent = icon->parent;
                        icons.at(j)->setPos(icon->pos() += QPointF(1.5*icon->boundingRect().width(), 2*icon->boundingRect().height()));
                        icons.at(j)->parent->linkToParent = icon->parent->linkToParent;
                        for (int y = 0; y < x; y++){
                            if (!gen->icons.isEmpty() && icons.contains(gen->icons.first())){
                                gen->icons.removeFirst();
                            }
                        }
                        branch.append(icons.at(j));
                        for (int a = branch.size() - 1; a < branch.size(); a++){
                            for (int b = 0; b < branch.at(a)->children.size(); b++){
                                branch.append(branch.at(a)->children.at(b));
                            }
                        }
                        gen->icons.removeAll(icon);
                        behaviorGS->removeItem(icon);
                        behaviorGS->removeItem(icon->linkToParent);
                        break;
                    }
                }
            }
        }else{
            lastY = 0;
        }
    }
    for (int c = 0; c < branch.size(); c++){
        icons.removeAll(branch.at(c));
    }
    for (int k = icons.size() - 1; k >= 0; k--){
        gen = static_cast<hkbGenerator *>(icons.at(k)->data.data());
        if (icons.at(k)->parent && !icons.at(k)->parent->children.isEmpty()){
            icons.at(k)->parent->children.removeAll(icons.at(k));
        }
        gen->icons.removeAll(icons.at(k));
        behaviorGS->removeItem(icons.at(k));
        behaviorGS->removeItem(icons.at(k)->linkToParent);
        delete icons.at(k)->linkToParent;
        delete icons.at(k);
        icons[k] = NULL;
    }
    return;
}

void BehaviorGraphView::removeSelectedObject(){
    /*if (selectedIcon && selectedIcon->parent && selectedIcon->parent->data.constData()){
        qulonglong sig = selectedIcon->parent->data->getSignature();
        switch (sig){
        case HKB_STATE_MACHINE:
        {
            const hkbStateMachine *parent = static_cast<const hkbStateMachine *>(selectedIcon->parent->data.constData());
            hkbStateMachineStateInfo *parentChild;
            for (int i = 0; i < parent->states.size(); i++){
                parentChild = static_cast<hkbStateMachineStateInfo *>(parent->states.at(i).data());
                if (parentChild->generator == selectedIcon->data){
                    parentChild->generator = HkObjectExpSharedPtr(getFirstChild(selectedIcon->data));   //If refcount < 1 remove from behavior pointer list?
                    removeIcon(selectedIcon);
                    break;
                }
            }
            break;
        }
        default:
            return;
        }
        if (behaviorGS && !behaviorGS->items(Qt::AscendingOrder).isEmpty()){
            GeneratorIcon *behaviorGraphIcon = static_cast<GeneratorIcon *>(scene()->items(Qt::AscendingOrder).first())->children.first();
            //expandBranch(behaviorGraphIcon, true);
            repositionIcons(behaviorGraphIcon, true);
            behaviorGS->update();
        }
    }*/
}

void BehaviorGraphView::deleteSelectedBranch(){
    selectedIcon->parent->data.data()->setDataValidity(false);
    if (selectedIcon->children.isEmpty()){
        static_cast<hkbGenerator *>(selectedIcon->data.data())->icons.removeAll(selectedIcon);
        selectedIcon->parent->children.removeAll(selectedIcon);
        behaviorGS->removeItem(selectedIcon->linkToParent);
        behaviorGS->removeItem(selectedIcon);
        delete selectedIcon->linkToParent;
        delete selectedIcon;
    }else{
        removeChildIcons(selectedIcon);
    }
    selectedIcon = NULL;
}

void BehaviorGraphView::removeSelectedObjectBranch(){
    //Popup removing branch???
    if (selectedIcon && selectedIcon->parent && selectedIcon->parent->data.constData()){
        qulonglong sig = selectedIcon->parent->data->getSignature();
        switch (sig){
        case HKB_STATE_MACHINE:
        {
            const hkbStateMachine *parent = static_cast<const hkbStateMachine *>(selectedIcon->parent->data.constData());
            hkbStateMachineStateInfo *parentChild;
            for (int i = 0; i < parent->states.size(); i++){
                parentChild = static_cast<hkbStateMachineStateInfo *>(parent->states.at(i).data());
                if (parentChild->generator == selectedIcon->data){
                    deleteSelectedBranch();
                    parentChild->generator = HkObjectExpSharedPtr();
                    break;
                }
            }
            break;
        }
        case HKB_MANUAL_SELECTOR_GENERATOR:
        {
            hkbManualSelectorGenerator *parent = static_cast<hkbManualSelectorGenerator *>(selectedIcon->parent->data.data());
            for (int i = 0; i < parent->generators.size(); i++){
                if (parent->generators.at(i) == selectedIcon->data){
                    deleteSelectedBranch();
                    parent->generators[i] = HkObjectExpSharedPtr();
                    break;
                }
            }
            break;
        }
        case HKB_BLENDER_GENERATOR:
        {
            const hkbBlenderGenerator *parent = static_cast<const hkbBlenderGenerator *>(selectedIcon->parent->data.constData());
            hkbBlenderGeneratorChild *parentChild;
            for (int i = 0; i < parent->children.size(); i++){
                parentChild = static_cast<hkbBlenderGeneratorChild *>(parent->children.at(i).data());
                if (parentChild->generator == selectedIcon->data){
                    deleteSelectedBranch();
                    parentChild->generator = HkObjectExpSharedPtr();
                    break;
                }
            }
            break;
        }
        case BS_I_STATE_TAGGING_GENERATOR:
        {
            BSiStateTaggingGenerator *parent = static_cast<BSiStateTaggingGenerator *>(selectedIcon->parent->data.data());
            deleteSelectedBranch();
            parent->pDefaultGenerator = HkObjectExpSharedPtr();
            break;
        }
        case BS_BONE_SWITCH_GENERATOR:
        {
            BSBoneSwitchGenerator *parent = static_cast<BSBoneSwitchGenerator *>(selectedIcon->parent->data.data());
            BSBoneSwitchGeneratorBoneData *parentChild;
            if (parent->pDefaultGenerator == selectedIcon->data){
                deleteSelectedBranch();
                parent->pDefaultGenerator = HkObjectExpSharedPtr();
                break;
            }
            for (int i = 0; i < parent->ChildrenA.size(); i++){
                parentChild = static_cast<BSBoneSwitchGeneratorBoneData *>(parent->ChildrenA.at(i).data());
                if (parentChild->pGenerator == selectedIcon->data){
                    deleteSelectedBranch();
                    parentChild->pGenerator = HkObjectExpSharedPtr();
                    break;
                }
            }
            break;
        }
        case BS_CYCLIC_BLEND_TRANSITION_GENERATOR:
        {
            BSCyclicBlendTransitionGenerator *parent = static_cast<BSCyclicBlendTransitionGenerator *>(selectedIcon->parent->data.data());
            deleteSelectedBranch();
            parent->pBlenderGenerator = HkObjectExpSharedPtr();
            break;
        }
        case BS_SYNCHRONIZED_CLIP_GENERATOR:
        {
            BSSynchronizedClipGenerator *parent = static_cast<BSSynchronizedClipGenerator *>(selectedIcon->parent->data.data());
            deleteSelectedBranch();
            parent->pClipGenerator = HkObjectExpSharedPtr();
            break;
        }
        case HKB_MODIFIER_GENERATOR:
        {
            hkbModifierGenerator *parent = static_cast<hkbModifierGenerator *>(selectedIcon->parent->data.data());
            if (parent->modifier == selectedIcon->data){
                deleteSelectedBranch();
                parent->modifier = HkObjectExpSharedPtr();
            }else if (parent->generator == selectedIcon->data){
                deleteSelectedBranch();
                parent->generator = HkObjectExpSharedPtr();
            }
            break;
        }
        case BS_OFFSET_ANIMATION_GENERATOR:
        {
            BSOffsetAnimationGenerator *parent = static_cast<BSOffsetAnimationGenerator *>(selectedIcon->parent->data.data());
            if (parent->pOffsetClipGenerator == selectedIcon->data){
                deleteSelectedBranch();
                parent->pOffsetClipGenerator = HkObjectExpSharedPtr();
            }else if (parent->pDefaultGenerator == selectedIcon->data){
                deleteSelectedBranch();
                parent->pDefaultGenerator = HkObjectExpSharedPtr();
            }
            break;
        }
        case HKB_POSE_MATCHING_GENERATOR:
        {
            const hkbPoseMatchingGenerator *parent = static_cast<const hkbPoseMatchingGenerator *>(selectedIcon->parent->data.constData());
            hkbBlenderGeneratorChild *parentChild;
            for (int i = 0; i < parent->children.size(); i++){
                parentChild = static_cast<hkbBlenderGeneratorChild *>(parent->children.at(i).data());
                if (parentChild->generator == selectedIcon->data){
                    deleteSelectedBranch();
                    parentChild->generator = HkObjectExpSharedPtr();
                    break;
                }
            }
            break;
        }
        case HKB_BEHAVIOR_GRAPH:
        {
            hkbBehaviorGraph *parent = static_cast<hkbBehaviorGraph *>(selectedIcon->parent->data.data());
            deleteSelectedBranch();
            parent->rootGenerator = HkObjectExpSharedPtr();
            break;
        }
        default:
            return;
        }
        if (behaviorGS && !behaviorGS->items(Qt::AscendingOrder).isEmpty()){
            GeneratorIcon *behaviorGraphIcon = static_cast<GeneratorIcon *>(scene()->items(Qt::AscendingOrder).first())->children.first();
            //expandBranch(behaviorGraphIcon, true);
            repositionIcons(behaviorGraphIcon, true);
            behaviorGS->update();
        }
    }
}

HkObject * BehaviorGraphView::getFirstChild(const HkObjectExpSharedPtr & obj){
    if (obj.constData()){
        qulonglong sig = obj->getSignature();
        switch (sig) {
        case HKB_STATE_MACHINE:
        {
            hkbStateMachine *ptr = static_cast<hkbStateMachine *>(obj.data());
            return static_cast<hkbStateMachineStateInfo *>(ptr->states.first().data())->generator.data();
        }
        case HKB_MANUAL_SELECTOR_GENERATOR:
        {
            hkbManualSelectorGenerator *ptr = static_cast<hkbManualSelectorGenerator *>(obj.data());
            return ptr->generators.first().data();
        }
        case HKB_BLENDER_GENERATOR:
        {
            hkbBlenderGenerator *ptr = static_cast<hkbBlenderGenerator *>(obj.data());
            return static_cast<hkbBlenderGeneratorChild *>(ptr->children.first().data())->generator.data();
        }
        case BS_I_STATE_TAGGING_GENERATOR:
        {
            BSiStateTaggingGenerator *ptr = static_cast<BSiStateTaggingGenerator *>(obj.data());
            return ptr->pDefaultGenerator.data();
        }
        case BS_BONE_SWITCH_GENERATOR:
        {
            BSBoneSwitchGenerator *ptr = static_cast<BSBoneSwitchGenerator *>(obj.data());
            return ptr->pDefaultGenerator.data();
        }
        case BS_CYCLIC_BLEND_TRANSITION_GENERATOR:
        {
            BSCyclicBlendTransitionGenerator *ptr = static_cast<BSCyclicBlendTransitionGenerator *>(obj.data());
            return ptr->pBlenderGenerator.data();
        }
        case BS_SYNCHRONIZED_CLIP_GENERATOR:
        {
            BSSynchronizedClipGenerator *ptr = static_cast<BSSynchronizedClipGenerator *>(obj.data());
            return ptr->pClipGenerator.data();
        }
        case HKB_MODIFIER_GENERATOR:
        {
            hkbModifierGenerator *ptr = static_cast<hkbModifierGenerator *>(obj.data());
            return ptr->generator.data();
        }
        case BS_OFFSET_ANIMATION_GENERATOR:
        {
            BSOffsetAnimationGenerator *ptr = static_cast<BSOffsetAnimationGenerator *>(obj.data());
            return ptr->pDefaultGenerator.data();
        }
        case HKB_POSE_MATCHING_GENERATOR:
        {
            hkbPoseMatchingGenerator *ptr = static_cast<hkbPoseMatchingGenerator *>(obj.data());
            return static_cast<hkbBlenderGeneratorChild *>(ptr->children.first().data())->generator.data();
        }
        case HKB_BEHAVIOR_GRAPH:
        {
            hkbBehaviorGraph *ptr = static_cast<hkbBehaviorGraph *>(obj.data());
            return ptr->rootGenerator.data();
        }
        default:
            return NULL;
        }
    }
    return NULL;
}

bool BehaviorGraphView::drawBranch(GeneratorIcon * rootIcon){
    QList <HkObjectExpSharedPtr> objects;
    QList <GeneratorIcon *> parentIcons;
    QVector <short> objectChildCount;
    int result;
    objects.append(rootIcon->data);
    parentIcons.append(rootIcon->parent);
    static_cast<hkbGenerator *>(rootIcon->data.data())->icons.removeAll(rootIcon);
    behaviorGS->removeItem(rootIcon);
    behaviorGS->removeItem(rootIcon->linkToParent);
    objectChildCount.append(1);
    while (!objects.isEmpty()){
        if (!objects.last().data()){
            return false;
        }
        qulonglong sig = objects.last()->getSignature();
        switch (sig){
        case HKB_STATE_MACHINE:
        {
            hkbStateMachine *ptr = static_cast<hkbStateMachine *>(objects.last().data());
            result = initializeIconsForNewBranch(ptr, objects, parentIcons, objectChildCount);
            if (result == 0){
                for (int i = ptr->states.size() - 1; i >= 0; i--){
                    objects.append(static_cast<hkbStateMachineStateInfo *>(ptr->states.at(i).data())->generator);
                    objectChildCount.last()++;
                }
            }else if (result == -1){
                return false;
            }
            break;
        }
        case BS_SYNCHRONIZED_CLIP_GENERATOR:
        {
            BSSynchronizedClipGenerator *ptr = static_cast<BSSynchronizedClipGenerator *>(objects.last().data());
            result = initializeIconsForNewBranch(ptr, objects, parentIcons, objectChildCount);
            if (result == 0){
                objects.append(ptr->pClipGenerator);
                objectChildCount.last()++;
            }else if (result == -1){
                return false;
            }
            break;
        }
        case BS_I_STATE_TAGGING_GENERATOR:
        {
            BSiStateTaggingGenerator *ptr = static_cast<BSiStateTaggingGenerator *>(objects.last().data());
            result = initializeIconsForNewBranch(ptr, objects, parentIcons, objectChildCount);
            if (result == 0){
                objects.append(ptr->pDefaultGenerator);
                objectChildCount.last()++;
            }else if (result == -1){
                return false;
            }
            break;
        }
        case BS_BONE_SWITCH_GENERATOR:
        {
            BSBoneSwitchGenerator *ptr = static_cast<BSBoneSwitchGenerator *>(objects.last().data());
            result = initializeIconsForNewBranch(ptr, objects, parentIcons, objectChildCount);
            if (result == 0){
                objects.append(ptr->pDefaultGenerator);
                objectChildCount.last()++;
                for (int i = ptr->ChildrenA.size() - 1; i >= 0; i--){
                    objects.append(static_cast<BSBoneSwitchGeneratorBoneData *>(ptr->ChildrenA.at(i).data())->pGenerator);
                    objectChildCount.last()++;
                }
            }else if (result == -1){
                return false;
            }
            break;
        }
        case HKB_MANUAL_SELECTOR_GENERATOR:
        {
            hkbManualSelectorGenerator *ptr = static_cast<hkbManualSelectorGenerator *>(objects.last().data());
            result = initializeIconsForNewBranch(ptr, objects, parentIcons, objectChildCount);
            if (result == 0){
                for (int i = ptr->generators.size() - 1; i >= 0; i--){
                    objects.append(ptr->generators.at(i));
                    objectChildCount.last()++;
                }
            }else if (result == -1){
                return false;
            }
            break;
        }
        case HKB_BLENDER_GENERATOR:
        {
            hkbBlenderGenerator *ptr = static_cast<hkbBlenderGenerator *>(objects.last().data());
            result = initializeIconsForNewBranch(ptr, objects, parentIcons, objectChildCount);
            if (result == 0){
                for (int i = ptr->children.size() - 1; i >= 0; i--){
                    objects.append(static_cast<hkbBlenderGeneratorChild *>(ptr->children.at(i).data())->generator);
                    objectChildCount.last()++;
                }
            }else if (result == -1){
                return false;
            }
            break;
        }
        case HKB_POSE_MATCHING_GENERATOR:
        {
            hkbPoseMatchingGenerator *ptr = static_cast<hkbPoseMatchingGenerator *>(objects.last().data());
            result = initializeIconsForNewBranch(ptr, objects, parentIcons, objectChildCount);
            if (result == 0){
                for (int i = ptr->children.size() - 1; i >= 0; i--){
                    objects.append(static_cast<hkbBlenderGeneratorChild *>(ptr->children.at(i).data())->generator);
                    objectChildCount.last()++;
                }
            }else if (result == -1){
                return false;
            }
            break;
        }
        case HKB_MODIFIER_GENERATOR:
        {
            hkbModifierGenerator *ptr = static_cast<hkbModifierGenerator *>(objects.last().data());
            result = initializeIconsForNewBranch(ptr, objects, parentIcons, objectChildCount);
            if (result == 0){
                //objects.append(ptr->modifier);
                //objectChildCount.last()++;
                objects.append(ptr->generator);
                objectChildCount.last()++;
            }else if (result == -1){
                return false;
            }
            break;
        }
        case BS_OFFSET_ANIMATION_GENERATOR:
        {
            BSOffsetAnimationGenerator *ptr = static_cast<BSOffsetAnimationGenerator *>(objects.last().data());
            result = initializeIconsForNewBranch(ptr, objects, parentIcons, objectChildCount);
            if (result == 0){
                objects.append(ptr->pDefaultGenerator);
                objectChildCount.last()++;
                objects.append(ptr->pOffsetClipGenerator);
                objectChildCount.last()++;
            }else if (result == -1){
                return false;
            }
            break;
        }
        case BS_CYCLIC_BLEND_TRANSITION_GENERATOR:
        {
            BSCyclicBlendTransitionGenerator *ptr = static_cast<BSCyclicBlendTransitionGenerator *>(objects.last().data());
            result = initializeIconsForNewBranch(ptr, objects, parentIcons, objectChildCount);
            if (result == 0){
                objects.append(ptr->pBlenderGenerator);
                objectChildCount.last()++;
            }else if (result == -1){
                return false;
            }
            break;
        }
        case HKB_BEHAVIOR_REFERENCE_GENERATOR:
        {
            hkbBehaviorReferenceGenerator *ptr = static_cast<hkbBehaviorReferenceGenerator *>(objects.last().data());
            if (initializeIconsForNewBranch(ptr, objects, parentIcons, objectChildCount) == -1){
                return false;
            }
            break;
        }
        case HKB_CLIP_GENERATOR:
        {
            hkbClipGenerator *ptr = static_cast<hkbClipGenerator *>(objects.last().data());
            if (initializeIconsForNewBranch(ptr, objects, parentIcons, objectChildCount) == -1){
                return false;
            }
            break;
        }
        case HKB_BEHAVIOR_GRAPH:
        {
            hkbBehaviorGraph *ptr = static_cast<hkbBehaviorGraph *>(objects.last().data());
            GeneratorIcon *icon = new GeneratorIcon(objects.last(), ptr->name);
            objects.removeLast();
            behaviorGS->addItem(icon);
            objects.append(ptr->rootGenerator);
            objectChildCount.append(1);
            parentIcons.append(icon);
            break;
        }
        case HK_ROOT_LEVEL_CONTAINER:
        {
            hkRootLevelContainer *ptr = static_cast<hkRootLevelContainer *>(objects.last().data());
            objects.removeLast();
            for (int i = ptr->namedVariants.size() - 1; i >= 0; i--){
                objects.append(ptr->namedVariants.at(i).variant);
            }
            break;
        }
        default:
            break;
        }
    }
    //setSceneRect(0, 0, frameSize().width(), frameSize().height());
    return true;
}

void BehaviorGraphView::appendStateMachine(){
    if (selectedIcon && selectedIcon->data.constData()){
        hkbStateMachine *stateMachine = new hkbStateMachine(behavior);
        stateMachine->setDataValidity(false);
        hkbStateMachineStateInfo *state = new hkbStateMachineStateInfo(behavior);
        stateMachine->states.append(HkObjectExpSharedPtr(state));
        hkbGenerator *selectedData = static_cast<hkbGenerator *>(selectedIcon->data.data());
        qulonglong sig = selectedIcon->data->getSignature();
        switch (sig){
        case HKB_STATE_MACHINE:
        {
            hkbStateMachine *obj = static_cast<hkbStateMachine *>(selectedIcon->data.data());
            hkbStateMachineStateInfo *objChild = new hkbStateMachineStateInfo(behavior);
            behavior->generators.append(HkObjectExpSharedPtr(objChild));
            obj->states.append(HkObjectExpSharedPtr(objChild));
            objChild->generator = HkObjectExpSharedPtr(stateMachine);
            stateMachine->appendIcon(initalizeAppendedIcon(objChild->generator, stateMachine, selectedData->icons.first()));
            break;
        }
        case HKB_MANUAL_SELECTOR_GENERATOR:
        {
            hkbManualSelectorGenerator *obj = static_cast<hkbManualSelectorGenerator *>(selectedIcon->data.data());
            obj->generators.append(HkObjectExpSharedPtr(stateMachine));
            stateMachine->appendIcon(initalizeAppendedIcon(obj->generators.last(), stateMachine, selectedData->icons.first()));
            break;
        }
        case HKB_BLENDER_GENERATOR:
        {
            const hkbBlenderGenerator *obj = static_cast<const hkbBlenderGenerator *>(selectedIcon->data.constData());
            hkbBlenderGeneratorChild *objChild = new hkbBlenderGeneratorChild(behavior);;
            behavior->generators.append(HkObjectExpSharedPtr(objChild));
            obj->children.append(HkObjectExpSharedPtr(objChild));
            objChild->generator = HkObjectExpSharedPtr(stateMachine);
            stateMachine->appendIcon(initalizeAppendedIcon(objChild->generator, stateMachine, selectedData->icons.first()));
            break;
        }
        case BS_I_STATE_TAGGING_GENERATOR:
        {
            BSiStateTaggingGenerator *obj = static_cast<BSiStateTaggingGenerator *>(selectedIcon->data.data());
            if (confirmationDialogue("The existing child branch of this object will be replaced by the new state machine!\nAre you sure you want to do this?", this)){
                obj->pDefaultGenerator = HkObjectExpSharedPtr(stateMachine);
                stateMachine->appendIcon(initalizeInjectedIcon(obj->pDefaultGenerator, stateMachine, selectedData->icons.first()));
            }else{
                goto DELETE_AND_RETURN;
            }
            break;
        }/*
        case BS_BONE_SWITCH_GENERATOR:
        {
            BSBoneSwitchGenerator *obj = static_cast<BSBoneSwitchGenerator *>(selectedIcon->data.data());
            BSBoneSwitchGeneratorBoneData *objChild;
            if (obj->pDefaultGenerator == selectedIcon->data){
                state->generator = obj->pDefaultGenerator;
                obj->pDefaultGenerator = HkObjectExpSharedPtr(stateMachine);
                stateMachine->appendIcon(initalizeInjectedIcon(obj->pDefaultGenerator, stateMachine, selectedIcon));
                drawIcon = false;
            }
            for (int i = 0; i < obj->ChildrenA.size(); i++){
                objChild = static_cast<BSBoneSwitchGeneratorBoneData *>(obj->ChildrenA.at(i).data());
                if (objChild->pGenerator == selectedIcon->data){
                    state->generator = objChild->pGenerator;
                    objChild->pGenerator = HkObjectExpSharedPtr(stateMachine);
                    if (drawIcon){
                        stateMachine->appendIcon(initalizeInjectedIcon(objChild->pGenerator, stateMachine, selectedIcon));
                        drawIcon = false;
                    }
                }
            }
            break;
        }
        case HKB_MODIFIER_GENERATOR:
        {
            hkbModifierGenerator *obj = static_cast<hkbModifierGenerator *>(selectedIcon->data.data());
            if (obj->generator == selectedIcon->data){
                state->generator = obj->generator;
                obj->generator = HkObjectExpSharedPtr(stateMachine);
                stateMachine->appendIcon(initalizeInjectedIcon(obj->generator, stateMachine, selectedIcon));
            }
            break;
        }
        case BS_OFFSET_ANIMATION_GENERATOR:
        {
            BSOffsetAnimationGenerator *obj = static_cast<BSOffsetAnimationGenerator *>(selectedIcon->data.data());
            if (obj->pDefaultGenerator == selectedIcon->data){
                state->generator = obj->pDefaultGenerator;
                obj->pDefaultGenerator = HkObjectExpSharedPtr(stateMachine);
                stateMachine->appendIcon(initalizeInjectedIcon(obj->pDefaultGenerator, stateMachine, selectedIcon));
            }
            break;
        }
        case HKB_POSE_MATCHING_GENERATOR:
        {
            const hkbPoseMatchingGenerator *obj = static_cast<const hkbPoseMatchingGenerator *>(selectedIcon->data.constData());
            hkbBlenderGeneratorChild *objChild;
            for (int i = 0; i < obj->children.size(); i++){
                objChild = static_cast<hkbBlenderGeneratorChild *>(obj->children.at(i).data());
                if (objChild->generator == selectedIcon->data){
                    state->generator = objChild->generator;
                    objChild->generator = HkObjectExpSharedPtr(stateMachine);
                    if (drawIcon){
                        stateMachine->appendIcon(initalizeInjectedIcon(objChild->generator, stateMachine, selectedIcon));
                        drawIcon = false;
                    }
                }
            }
            break;
        }
        case HKB_BEHAVIOR_GRAPH:
        {
            hkbBehaviorGraph *obj = static_cast<hkbBehaviorGraph *>(selectedIcon->data.data());
            if (obj->rootGenerator == selectedIcon->data){
                state->generator = obj->rootGenerator;
                obj->rootGenerator = HkObjectExpSharedPtr(stateMachine);
                stateMachine->appendIcon(initalizeInjectedIcon(obj->rootGenerator, stateMachine, selectedIcon));
            }
            break;
        }*/
        default:
        DELETE_AND_RETURN:
            delete stateMachine;
            delete state;
            return;
        }
        behavior->generators.append(HkObjectExpSharedPtr(stateMachine));
        behavior->generators.append(HkObjectExpSharedPtr(state));
        if (behaviorGS && !behaviorGS->items(Qt::AscendingOrder).isEmpty()){
            GeneratorIcon *behaviorGraphIcon = static_cast<GeneratorIcon *>(scene()->items(Qt::AscendingOrder).first())->children.first();
            //expandBranch(behaviorGraphIcon, true);
            expandBranchForIcon(selectedData->icons.first());
            expandBranch(selectedData->icons.first());
            repositionIcons(behaviorGraphIcon, true);
            behaviorGS->update();
            centerOn(selectedData->icons.first());
        }
    }
}

void BehaviorGraphView::wrapStateMachine(){
    if (selectedIcon && selectedIcon->parent && selectedIcon->parent->data.constData()){
        hkbStateMachine *stateMachine = new hkbStateMachine(behavior);
        //behavior->generators.append(HkObjectExpSharedPtr(stateMachine));
        hkbStateMachineStateInfo *state = new hkbStateMachineStateInfo(behavior);
        //behavior->generators.append(HkObjectExpSharedPtr(state));
        stateMachine->states.append(HkObjectExpSharedPtr(state));
        qulonglong sig = selectedIcon->parent->data->getSignature();
        bool drawIcon = true;
        switch (sig){
        case HKB_STATE_MACHINE:
        {
            const hkbStateMachine *parent = static_cast<const hkbStateMachine *>(selectedIcon->parent->data.constData());
            hkbStateMachineStateInfo *parentChild;
            for (int i = 0; i < parent->states.size(); i++){
                parentChild = static_cast<hkbStateMachineStateInfo *>(parent->states.at(i).data());
                if (parentChild->generator == selectedIcon->data){
                    state->generator = parentChild->generator;
                    parentChild->generator = HkObjectExpSharedPtr(stateMachine);
                    if (drawIcon){
                        stateMachine->appendIcon(initalizeInjectedIcon(parentChild->generator, stateMachine, selectedIcon->parent));
                        drawIcon = false;
                    }
                }
            }
            break;
        }
        case HKB_MANUAL_SELECTOR_GENERATOR:
        {
            hkbManualSelectorGenerator *parent = static_cast<hkbManualSelectorGenerator *>(selectedIcon->parent->data.data());
            for (int i = 0; i < parent->generators.size(); i++){
                if (parent->generators.at(i) == selectedIcon->data){
                    state->generator = parent->generators.at(i);
                    parent->generators[i] = HkObjectExpSharedPtr(stateMachine);
                    if (drawIcon){
                        stateMachine->appendIcon(initalizeInjectedIcon(parent->generators.at(i), stateMachine, selectedIcon->parent));
                        drawIcon = false;
                    }
                }
            }
            break;
        }
        case HKB_BLENDER_GENERATOR:
        {
            const hkbBlenderGenerator *parent = static_cast<const hkbBlenderGenerator *>(selectedIcon->parent->data.constData());
            hkbBlenderGeneratorChild *parentChild;
            for (int i = 0; i < parent->children.size(); i++){
                parentChild = static_cast<hkbBlenderGeneratorChild *>(parent->children.at(i).data());
                if (parentChild->generator == selectedIcon->data){
                    state->generator = parentChild->generator;
                    parentChild->generator = HkObjectExpSharedPtr(stateMachine);
                    if (drawIcon){
                        stateMachine->appendIcon(initalizeInjectedIcon(parentChild->generator, stateMachine, selectedIcon->parent));
                        drawIcon = false;
                    }
                }
            }
            break;
        }
        case BS_I_STATE_TAGGING_GENERATOR:
        {
            BSiStateTaggingGenerator *parent = static_cast<BSiStateTaggingGenerator *>(selectedIcon->parent->data.data());
            if (parent->pDefaultGenerator == selectedIcon->data){
                state->generator = parent->pDefaultGenerator;
                parent->pDefaultGenerator = HkObjectExpSharedPtr(stateMachine);
                stateMachine->appendIcon(initalizeInjectedIcon(parent->pDefaultGenerator, stateMachine, selectedIcon->parent));
            }
            break;
        }
        case BS_BONE_SWITCH_GENERATOR:
        {
            BSBoneSwitchGenerator *parent = static_cast<BSBoneSwitchGenerator *>(selectedIcon->parent->data.data());
            BSBoneSwitchGeneratorBoneData *parentChild;
            if (parent->pDefaultGenerator == selectedIcon->data){
                state->generator = parent->pDefaultGenerator;
                parent->pDefaultGenerator = HkObjectExpSharedPtr(stateMachine);
                stateMachine->appendIcon(initalizeInjectedIcon(parent->pDefaultGenerator, stateMachine, selectedIcon->parent));
                drawIcon = false;
            }
            for (int i = 0; i < parent->ChildrenA.size(); i++){
                parentChild = static_cast<BSBoneSwitchGeneratorBoneData *>(parent->ChildrenA.at(i).data());
                if (parentChild->pGenerator == selectedIcon->data){
                    state->generator = parentChild->pGenerator;
                    parentChild->pGenerator = HkObjectExpSharedPtr(stateMachine);
                    if (drawIcon){
                        stateMachine->appendIcon(initalizeInjectedIcon(parentChild->pGenerator, stateMachine, selectedIcon->parent));
                        drawIcon = false;
                    }
                }
            }
            break;
        }
        case HKB_MODIFIER_GENERATOR:
        {
            hkbModifierGenerator *parent = static_cast<hkbModifierGenerator *>(selectedIcon->parent->data.data());
            if (parent->generator == selectedIcon->data){
                state->generator = parent->generator;
                parent->generator = HkObjectExpSharedPtr(stateMachine);
                stateMachine->appendIcon(initalizeInjectedIcon(parent->generator, stateMachine, selectedIcon->parent));
            }
            break;
        }
        case BS_OFFSET_ANIMATION_GENERATOR:
        {
            BSOffsetAnimationGenerator *parent = static_cast<BSOffsetAnimationGenerator *>(selectedIcon->parent->data.data());
            if (parent->pDefaultGenerator == selectedIcon->data){
                state->generator = parent->pDefaultGenerator;
                parent->pDefaultGenerator = HkObjectExpSharedPtr(stateMachine);
                stateMachine->appendIcon(initalizeInjectedIcon(parent->pDefaultGenerator, stateMachine, selectedIcon->parent));
            }
            break;
        }
        case HKB_POSE_MATCHING_GENERATOR:
        {
            const hkbPoseMatchingGenerator *parent = static_cast<const hkbPoseMatchingGenerator *>(selectedIcon->parent->data.constData());
            hkbBlenderGeneratorChild *parentChild;
            for (int i = 0; i < parent->children.size(); i++){
                parentChild = static_cast<hkbBlenderGeneratorChild *>(parent->children.at(i).data());
                if (parentChild->generator == selectedIcon->data){
                    state->generator = parentChild->generator;
                    parentChild->generator = HkObjectExpSharedPtr(stateMachine);
                    if (drawIcon){
                        stateMachine->appendIcon(initalizeInjectedIcon(parentChild->generator, stateMachine, selectedIcon->parent));
                        drawIcon = false;
                    }
                }
            }
            break;
        }
        case HKB_BEHAVIOR_GRAPH:
        {
            hkbBehaviorGraph *parent = static_cast<hkbBehaviorGraph *>(selectedIcon->parent->data.data());
            if (parent->rootGenerator == selectedIcon->data){
                state->generator = parent->rootGenerator;
                parent->rootGenerator = HkObjectExpSharedPtr(stateMachine);
                stateMachine->appendIcon(initalizeInjectedIcon(parent->rootGenerator, stateMachine, selectedIcon->parent));
            }
            break;
        }
        default:
            delete stateMachine;
            delete state;
            return;
        }
        behavior->generators.append(HkObjectExpSharedPtr(stateMachine));
        behavior->generators.append(HkObjectExpSharedPtr(state));
        if (behaviorGS && !behaviorGS->items(Qt::AscendingOrder).isEmpty()){
            GeneratorIcon *behaviorGraphIcon = static_cast<GeneratorIcon *>(scene()->items(Qt::AscendingOrder).first())->children.first();
            //expandBranch(behaviorGraphIcon, true);
            repositionIcons(behaviorGraphIcon, true);
            behaviorGS->update();
        }
    }
}

void BehaviorGraphView::popUpMenuRequested(const QPoint &pos, const HkObjectExpSharedPtr & obj){
    qulonglong sig = obj->getSignature();
    if (sig == HKB_BLENDER_GENERATOR){
        wrapBSCBTGAct->setVisible(true);
    }else if (sig == HKB_CLIP_GENERATOR){
        wrapBSSCGAct->setVisible(true);
        wrapBSOAGAct->setVisible(true);
    }else{
        wrapBSCBTGAct->setVisible(false);
        wrapBSSCGAct->setVisible(false);
        wrapBSOAGAct->setVisible(false);
    }
    contextMenu->popup(viewport()->mapToGlobal(pos));
}

void BehaviorGraphView::repositionIcons(GeneratorIcon *icon, bool updateNonVisable){
    if (!icon){
        return;
    }
    for (int i = 0; i < icon->children.size(); i++){
        icon->children.at(i)->updatePosition(updateNonVisable);
        repositionIcons(icon->children.at(i));
    }
}

void BehaviorGraphView::expandBranchForIcon(GeneratorIcon *icon){
    if (icon){
        icon->isExpanded = true;
        icon->setVisible(true);
        if (icon->linkToParent){
            icon->linkToParent->setVisible(true);
        }
        for (int i = 0; i < icon->children.size(); i++){
            icon->children.at(i)->setVisible(true);
            if (icon->children.at(i)->linkToParent){
                icon->children.at(i)->linkToParent->setVisible(true);
            }
        }
        expandBranchForIcon(icon->parent);
    }
}

void BehaviorGraphView::wheelEvent(QWheelEvent *event){
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    if (event->delta() > 0 && currentScale < maxScale){
        scale(scaleUpFactor, scaleUpFactor);
        currentScale = currentScale*scaleUpFactor;
    }else if (currentScale > minScale){
        scale(scaleDownFactor, scaleDownFactor);
        currentScale = currentScale*scaleDownFactor;
    }
}

void BehaviorGraphView::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton){
        this->setDragMode(QGraphicsView::ScrollHandDrag);
    }
    QGraphicsView::mousePressEvent(event);
}

void BehaviorGraphView::mouseReleaseEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton){
        this->setDragMode(QGraphicsView::NoDrag);
    }
    QGraphicsView::mouseReleaseEvent(event);
}

bool BehaviorGraphView::drawBehaviorGraph(){
    QList <HkObjectExpSharedPtr> objects;
    QList <GeneratorIcon *> parentIcons;
    QVector <short> objectChildCount;
    int result;
    objects.append(behavior->getRootObject());
    while (!objects.isEmpty()){
        if (!objects.last().data()){
            return false;
        }
        qulonglong sig = objects.last()->getSignature();
        switch (sig){
        case HKB_STATE_MACHINE:
        {
            hkbStateMachine *ptr = static_cast<hkbStateMachine *>(objects.last().data());
            result = initializeIcons(ptr, objects, parentIcons, objectChildCount);
            if (result == 0){
                for (int i = ptr->states.size() - 1; i >= 0; i--){
                    objects.append(static_cast<hkbStateMachineStateInfo *>(ptr->states.at(i).data())->generator);
                    objectChildCount.last()++;
                }
            }else if (result == -1){
                return false;
            }
            break;
        }
        case BS_SYNCHRONIZED_CLIP_GENERATOR:
        {
            BSSynchronizedClipGenerator *ptr = static_cast<BSSynchronizedClipGenerator *>(objects.last().data());
            result = initializeIcons(ptr, objects, parentIcons, objectChildCount);
            if (result == 0){
                objects.append(ptr->pClipGenerator);
                objectChildCount.last()++;
            }else if (result == -1){
                return false;
            }
            break;
        }
        case BS_I_STATE_TAGGING_GENERATOR:
        {
            BSiStateTaggingGenerator *ptr = static_cast<BSiStateTaggingGenerator *>(objects.last().data());
            result = initializeIcons(ptr, objects, parentIcons, objectChildCount);
            if (result == 0){
                objects.append(ptr->pDefaultGenerator);
                objectChildCount.last()++;
            }else if (result == -1){
                return false;
            }
            break;
        }
        case BS_BONE_SWITCH_GENERATOR:
        {
            BSBoneSwitchGenerator *ptr = static_cast<BSBoneSwitchGenerator *>(objects.last().data());
            result = initializeIcons(ptr, objects, parentIcons, objectChildCount);
            if (result == 0){
                objects.append(ptr->pDefaultGenerator);
                objectChildCount.last()++;
                for (int i = ptr->ChildrenA.size() - 1; i >= 0; i--){
                    objects.append(static_cast<BSBoneSwitchGeneratorBoneData *>(ptr->ChildrenA.at(i).data())->pGenerator);
                    objectChildCount.last()++;
                }
            }else if (result == -1){
                return false;
            }
            break;
        }
        case HKB_MANUAL_SELECTOR_GENERATOR:
        {
            hkbManualSelectorGenerator *ptr = static_cast<hkbManualSelectorGenerator *>(objects.last().data());
            result = initializeIcons(ptr, objects, parentIcons, objectChildCount);
            if (result == 0){
                for (int i = ptr->generators.size() - 1; i >= 0; i--){
                    objects.append(ptr->generators.at(i));
                    objectChildCount.last()++;
                }
            }else if (result == -1){
                return false;
            }
            break;
        }
        case HKB_BLENDER_GENERATOR:
        {
            hkbBlenderGenerator *ptr = static_cast<hkbBlenderGenerator *>(objects.last().data());
            result = initializeIcons(ptr, objects, parentIcons, objectChildCount);
            if (result == 0){
                for (int i = ptr->children.size() - 1; i >= 0; i--){
                    objects.append(static_cast<hkbBlenderGeneratorChild *>(ptr->children.at(i).data())->generator);
                    objectChildCount.last()++;
                }
            }else if (result == -1){
                return false;
            }
            break;
        }
        case HKB_POSE_MATCHING_GENERATOR:
        {
            hkbPoseMatchingGenerator *ptr = static_cast<hkbPoseMatchingGenerator *>(objects.last().data());
            result = initializeIcons(ptr, objects, parentIcons, objectChildCount);
            if (result == 0){
                for (int i = ptr->children.size() - 1; i >= 0; i--){
                    objects.append(static_cast<hkbBlenderGeneratorChild *>(ptr->children.at(i).data())->generator);
                    objectChildCount.last()++;
                }
            }else if (result == -1){
                return false;
            }
            break;
        }
        case HKB_MODIFIER_GENERATOR:
        {
            hkbModifierGenerator *ptr = static_cast<hkbModifierGenerator *>(objects.last().data());
            result = initializeIcons(ptr, objects, parentIcons, objectChildCount);
            if (result == 0){
                //objects.append(ptr->modifier);
                //objectChildCount.last()++;
                objects.append(ptr->generator);
                objectChildCount.last()++;
            }else if (result == -1){
                return false;
            }
            break;
        }
        case BS_OFFSET_ANIMATION_GENERATOR:
        {
            BSOffsetAnimationGenerator *ptr = static_cast<BSOffsetAnimationGenerator *>(objects.last().data());
            result = initializeIcons(ptr, objects, parentIcons, objectChildCount);
            if (result == 0){
                objects.append(ptr->pDefaultGenerator);
                objectChildCount.last()++;
                objects.append(ptr->pOffsetClipGenerator);
                objectChildCount.last()++;
            }else if (result == -1){
                return false;
            }
            break;
        }
        case BS_CYCLIC_BLEND_TRANSITION_GENERATOR:
        {
            BSCyclicBlendTransitionGenerator *ptr = static_cast<BSCyclicBlendTransitionGenerator *>(objects.last().data());
            result = initializeIcons(ptr, objects, parentIcons, objectChildCount);
            if (result == 0){
                objects.append(ptr->pBlenderGenerator);
                objectChildCount.last()++;
            }else if (result == -1){
                return false;
            }
            break;
        }
        case HKB_BEHAVIOR_REFERENCE_GENERATOR:
        {
            hkbBehaviorReferenceGenerator *ptr = static_cast<hkbBehaviorReferenceGenerator *>(objects.last().data());
            if (initializeIcons(ptr, objects, parentIcons, objectChildCount) == -1){
                return false;
            }
            break;
        }
        case HKB_CLIP_GENERATOR:
        {
            hkbClipGenerator *ptr = static_cast<hkbClipGenerator *>(objects.last().data());
            if (initializeIcons(ptr, objects, parentIcons, objectChildCount) == -1){
                return false;
            }
            break;
        }
        case HKB_BEHAVIOR_GRAPH:
        {
            hkbBehaviorGraph *ptr = static_cast<hkbBehaviorGraph *>(objects.last().data());
            GeneratorIcon *icon = new GeneratorIcon(objects.last(), ptr->name);
            objects.removeLast();
            behaviorGS->addItem(icon);
            objects.append(ptr->rootGenerator);
            objectChildCount.append(1);
            parentIcons.append(icon);
            break;
        }
        case HK_ROOT_LEVEL_CONTAINER:
        {
            hkRootLevelContainer *ptr = static_cast<hkRootLevelContainer *>(objects.last().data());
            objects.removeLast();
            for (int i = ptr->namedVariants.size() - 1; i >= 0; i--){
                objects.append(ptr->namedVariants.at(i).variant);
            }
            break;
        }
        default:
            break;
        }
    }
    //setSceneRect(0, 0, frameSize().width(), frameSize().height());
    return true;
}
