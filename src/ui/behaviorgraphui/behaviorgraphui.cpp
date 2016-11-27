#include "src/ui/mainwindow.h"
#include "src/ui/fileselectwindow.h"
#include "src/filetypes/hkxfile.h"
#include "src/hkxclasses/generators/hkbgenerator.h"
#include "behaviorgraphui.h"
#include "src/hkxclasses/hkxobject.h"
#include "behaviorgraphicons.h"
#include "src/ui/hkdataui.h"

#include "src/hkxclasses/generators/hkbstatemachinestateinfo.h"
#include "src/hkxclasses/generators/hkbstatemachine.h"
#include "src/hkxclasses/generators/hkbmodifiergenerator.h"
#include "src/hkxclasses/generators/bsistatetagginggenerator.h"
#include "src/hkxclasses/generators/bscyclicblendtransitiongenerator.h"
#include "src/hkxclasses/generators/bsboneswitchgenerator.h"
#include "src/hkxclasses/generators/bsboneswitchgeneratorbonedata.h"
#include "src/hkxclasses/generators/bssynchronizedclipgenerator.h"
#include "src/hkxclasses/generators/hkbmanualselectorgenerator.h"
#include "src/hkxclasses/generators/hkbblendergeneratorchild.h"
#include "src/hkxclasses/hkbboneweightarray.h"
#include "src/hkxclasses/generators/hkbblendergenerator.h"
#include "src/hkxclasses/generators/bsoffsetanimationgenerator.h"
#include "src/hkxclasses/generators/hkbposematchinggenerator.h"
#include "src/hkxclasses/generators/hkbclipgenerator.h"
#include "src/hkxclasses/generators/hkbbehaviorreferencegenerator.h"
#include "src/hkxclasses/hkbstatemachinetransitioninfoarray.h"
#include "src/hkxclasses/hkbstatemachineeventpropertyarray.h"
#include "src/hkxclasses/generators/hkbbehaviorgraph.h"
#include "src/hkxclasses/hkbbehaviorgraphdata.h"
#include "src/hkxclasses/hkbbehaviorgraphstringdata.h"
#include "src/hkxclasses/hkbvariablevalueset.h"
#include "src/hkxclasses/hkrootlevelcontainer.h"

#include <QBoxLayout>
#include <QMenuBar>
#include <QFile>
#include <QSettings>
#include <QGroupBox>
#include <QMessageBox>
#include <QGraphicsSceneMouseEvent>
#include <QSize>

bool confirmationDialogue(const QString & message, QWidget *parent){
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(parent, "Application", message, QMessageBox::Yes | QMessageBox::Cancel);
    if (ret == QMessageBox::Yes){
        return true;
    }else if (ret == QMessageBox::Cancel){
        return false;
    }
    return false;
}

/**
 * BehaviorGraphView
 */

BehaviorGraphView::BehaviorGraphView(HkDataUI *mainUI, BehaviorFile * file)
    : ui(mainUI),
      behavior(file),
      behaviorGS(new QGraphicsScene),
      selectedIcon(NULL),
      rootIcon(NULL),
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
    connect(removeObjBranchAct, SIGNAL(triggered()), this, SLOT(removeSelectedObjectBranchSlot()));
    if (ui){
        ui->setBehaviorView(this);
        connect(this, SIGNAL(iconSelected(GeneratorIcon *)), ui, SLOT(changeCurrentDataWidget(GeneratorIcon *)));
    }
}

void BehaviorGraphView::writeToLog(const QString &message, bool isError){
    behavior->writeToLog(message, isError);
}

QStringList BehaviorGraphView::getEventNames() const{
    return behavior->getEventNames();
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

void BehaviorGraphView::createIconBranch(QList <GeneratorIcon *> & icons, GeneratorIcon *parent){
    icons.append(parent);
    GeneratorIcon *last = icons.last();
    for (int i = 0; i < last->children.size(); i++){
        createIconBranch(icons, last->children.at(i));
    }
}

void BehaviorGraphView::removeChildIcons(GeneratorIcon *parent, GeneratorIcon *iconToKeep){
    if (!parent){
        return;
    }
    GeneratorIcon *icon;
    GeneratorIcon *oldParent = parent->parent;
    hkbGenerator *gen;
    qreal lastY = 0;
    QList <GeneratorIcon *> icons;
    QList <GeneratorIcon *> branch;
    createIconBranch(icons, parent);
    int index = 0;
    for (int j = 0; j < icons.size(); j++){ //Add search function for icons by name!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        if (icons.at(j) == iconToKeep && !iconToKeep->children.isEmpty()){
            gen = static_cast<hkbGenerator *>(iconToKeep->data.data());
            if (gen){
                for (int x = 0; x < gen->icons.size(); x++){
                    if (icons.contains(gen->icons.at(x))){
                        index = j + 1;
                        if (index < icons.size()){
                            icons.insert(index, iconToKeep->setParentReturnDuplicate(oldParent));
                        }else{
                            icons.append(iconToKeep->setParentReturnDuplicate(oldParent));
                        }
                        createIconBranch(branch, iconToKeep);
                        break;
                    }
                }
            }
        }else if (icons.at(j)->children.size() > 0){
            gen = static_cast<hkbGenerator *>(icons.at(j)->data.data());
            if (gen){
                for (int x = 0; x < gen->icons.size(); x++){
                    if (!icons.contains(gen->icons.at(x))){
                        if (lastY == 0 || lastY > gen->icons.at(x)->y()){
                            lastY = gen->icons.at(x)->y();
                            icon = gen->icons.at(x);
                            gen->icons[x] = icons.at(j);
                            icons.insert(j + 1, icons.at(j)->setParentReturnDuplicate(icon->parent));
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
                            removeIconFromGraph(icon);
                            break;
                        }
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
    for (int d = 0; d < branch.size(); d++){
        expandBranchForIcon(branch.at(d));
    }
    for (int k = icons.size() - 1; k >= 0; k--){
        if (icons.at(k)){//Sometimes there are duplicates in icons??? Investigate why later...
            removeIconFromGraph(icons.at(k));
        }
    }
    return;
}

void BehaviorGraphView::removeIconFromGraph(GeneratorIcon *iconToRemove){
    hkbGenerator *gen = static_cast<hkbGenerator *>(iconToRemove->data.data());
    if (iconToRemove->parent && !iconToRemove->parent->children.isEmpty()){
        iconToRemove->parent->children.removeAll(iconToRemove);
    }
    gen->icons.removeAll(iconToRemove);
    if (gen->icons.isEmpty()){
        gen->unlink();
    }
    behaviorGS->removeItem(iconToRemove);
    behaviorGS->removeItem(iconToRemove->linkToParent);
    delete iconToRemove->linkToParent;
    delete iconToRemove;
    iconToRemove = NULL;
}

void BehaviorGraphView::deleteSelectedBranch(GeneratorIcon *icon, GeneratorIcon *iconToKeep){
    if (icon && icon->data.constData() && icon->parent && icon->parent->data.constData()){
        if (icon->children.isEmpty() && iconToKeep != icon){
            static_cast<hkbGenerator *>(icon->data.data())->icons.removeAll(icon);
            if (static_cast<hkbGenerator *>(icon->data.data())->icons.isEmpty()){
                icon->data.data()->unlink();
            }
            icon->parent->children.removeAll(icon);
            behaviorGS->removeItem(icon->linkToParent);
            behaviorGS->removeItem(icon);
            delete icon->linkToParent;
            delete icon;
        }else{
            removeChildIcons(icon, iconToKeep);
        }
        icon = NULL;
    }
}

bool BehaviorGraphView::reconnectBranch(HkxObject *oldChild, HkxObject *newChild, GeneratorIcon *icon){
    if (!newChild || !icon){
        return false;
    }
    if (icon->isGranfatherParadox(newChild)){
        return false;
    }
    GeneratorIcon *oldChildIcon = NULL;
    GeneratorIcon *newChildIcon = NULL;
    hkbGenerator *gen = static_cast<hkbGenerator *>(newChild);
    for (int i = 0; i < icon->children.size(); i++){
        if (icon->children.at(i)->data.data() == oldChild){
            oldChildIcon = icon->children.at(i);
        }
    }
    if (oldChildIcon){
        for (int j =0; j < gen->icons.size(); j++){
            if (oldChildIcon->isDescendant(gen->icons.at(j))){
                newChildIcon = gen->icons.at(j);
                break;
            }
        }
    }
    removeSelectedObjectBranch(oldChildIcon, newChildIcon, false);
    if (!newChildIcon){
        if (gen->getSignature() != HKB_CLIP_GENERATOR && gen->getSignature() != HKB_BEHAVIOR_REFERENCE_GENERATOR && !gen->icons.isEmpty() && gen->icons.first()->pos().y() > icon->pos().y()){
            icon->children.append(gen->icons.first());
            if (gen->icons.first()->parent){
                gen->icons.first()->parent->children.removeAll(gen->icons.first());
                initalizeAppendedIcon(gen, gen->icons.first()->parent);
                gen->icons.first()->parent = icon;
            }
        }else{
            initalizeAppendedIcon(static_cast<hkbGenerator *>(newChild), icon);
        }
    }else if (!icon->hasChildIcon(newChildIcon)){
        initalizeAppendedIcon(static_cast<hkbGenerator *>(newChild), icon);
    }
    if (behaviorGS && !behaviorGS->items(Qt::AscendingOrder).isEmpty()){
        GeneratorIcon *behaviorGraphIcon = static_cast<GeneratorIcon *>(scene()->items(Qt::AscendingOrder).first());
        expandBranch(behaviorGraphIcon);
        repositionIcons(behaviorGraphIcon, true);
        behaviorGS->update();
    }
    return true;
}

void BehaviorGraphView::removeSelectedObjectBranchSlot(){
    if (selectedIcon){
        selectedIcon->unhighlight();
    }
    HkxObject *gen = NULL;
    if (selectedIcon->parent){
        gen = selectedIcon->parent->data.data();
    }
    removeSelectedObjectBranch(selectedIcon);
    if (gen){
        gen->evaulateDataValidity();
    }
    ui->changeCurrentDataWidget(NULL);
    selectedIcon = NULL;
    removeGeneratorData();
    removeModifierData();
    behavior->removeOtherData();
}

void BehaviorGraphView::removeSelectedObjectBranch(GeneratorIcon *icon, GeneratorIcon *iconToKeep, bool removeAll){
    if (icon && icon->parent && icon->parent->data.constData()){
        if (!(static_cast<hkbGenerator *>(icon->parent->data.data())->removeObject(static_cast<hkbGenerator *>(icon->data.data()), removeAll))){
            return;
        }
        deleteSelectedBranch(icon, iconToKeep);
        if (behaviorGS && !behaviorGS->items(Qt::AscendingOrder).isEmpty()){
            GeneratorIcon *behaviorGraphIcon = static_cast<GeneratorIcon *>(scene()->items(Qt::AscendingOrder).first());
            repositionIcons(behaviorGraphIcon, true);
            behaviorGS->update();
        }
    }
}

bool BehaviorGraphView::appendObject(hkbGenerator *ptr, GeneratorIcon *parentObjIcon){
    if (!ptr || !parentObjIcon || !parentObjIcon->data.constData()){
        return false;
    }
    hkbGenerator *selectedData = static_cast<hkbGenerator *>(parentObjIcon->data.data());
    if (!selectedData->appendObject(ptr)){
        return false;
    }
    GeneratorIcon *newIcon = initalizeAppendedIcon(ptr, selectedData->icons.first());
    newIcon->setSelected();
    return true;
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

void BehaviorGraphView::wrapObject(hkbGenerator *obj, GeneratorIcon *parentObjIcon){
    if (!obj || !parentObjIcon || !parentObjIcon->parent || !parentObjIcon->parent->data.constData()){
        return;
    }
    if (!static_cast<hkbGenerator *>(parentObjIcon->parent->data.data())->wrapObject(obj, static_cast<hkbGenerator *>(parentObjIcon->data.data()))){
        delete obj;
        obj = NULL;
        return;
    }
    GeneratorIcon *newIcon = initalizeInjectedIcon(obj, parentObjIcon->parent, selectedIcon);
    newIcon->setSelected();
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

void BehaviorGraphView::popUpMenuRequested(const QPoint &pos, GeneratorIcon *icon){
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

int BehaviorGraphView::initializeIconsForNewBranch(hkbGenerator *ptr, QList<HkxObjectExpSharedPtr> & objects, QList<GeneratorIcon *> & parentIcons, QVector <short> & objectChildCount){
    if (parentIcons.isEmpty() || objects.isEmpty() || objectChildCount.isEmpty()){
        return -1;
    }
    bool isBranchTip = true;
    GeneratorIcon *icon = addIconToGraph(ptr, parentIcons.last());
    if (!icon){
        return -1;
    }
    if (ptr->getSignature() != HKB_CLIP_GENERATOR && ptr->getSignature() != HKB_BEHAVIOR_REFERENCE_GENERATOR){
        isBranchTip = false;
    }
    objects.removeLast();
    objectChildCount.last()--;
    if (objectChildCount.last() < 1){
        parentIcons.removeLast();
        objectChildCount.removeLast();
    }
    if (!isBranchTip){
        objectChildCount.append(0);
        parentIcons.append(icon);
    }
    return 0;
}

int BehaviorGraphView::initializeIcons(hkbGenerator *ptr, QList<HkxObjectExpSharedPtr> & objects, QList<GeneratorIcon *> & parentIcons, QVector <short> & objectChildCount){
    if (parentIcons.isEmpty() || objects.isEmpty() || objectChildCount.isEmpty()){
        return -1;
    }
    bool isBranchTip = true;
    for (int i = 0; i < parentIcons.last()->children.size(); i++){
        if (parentIcons.last()->children.at(i)->data == objects.last()){
            objects.removeLast();
            objectChildCount.last()--;
            if (objectChildCount.last() < 1){
                parentIcons.removeLast();
                objectChildCount.removeLast();
            }
            return 1;
        }
    }
    GeneratorIcon *icon = addIconToGraph(ptr, parentIcons.last());
    if (!icon){
        return -1;
    }
    if (ptr->getSignature() != HKB_CLIP_GENERATOR && ptr->getSignature() != HKB_BEHAVIOR_REFERENCE_GENERATOR){
        isBranchTip = false;
        if (ptr->icons.size() > 1){
            objects.removeLast();
            objectChildCount.last()--;
            if (objectChildCount.last() < 1){
                parentIcons.removeLast();
                objectChildCount.removeLast();
            }
            return 1;
        }
    }
    objects.removeLast();
    objectChildCount.last()--;
    if (objectChildCount.last() < 1){
        parentIcons.removeLast();
        objectChildCount.removeLast();
    }
    if (!isBranchTip){
        objectChildCount.append(0);
        parentIcons.append(icon);
    }
    return 0;
}

GeneratorIcon * BehaviorGraphView::addIconToGraph(hkbGenerator *obj, GeneratorIcon * parentIcon){
    if (!parentIcon || !obj){
        return NULL;
    }
    GeneratorIcon *icon = new GeneratorIcon(obj, obj->getName(), parentIcon);
    //icon->setFlag(QGraphicsItem::ItemIsMovable);
    behaviorGS->addItem(icon->linkToParent);
    behaviorGS->addItem(icon);
    return icon;
}

GeneratorIcon * BehaviorGraphView::initalizeInjectedIcon(hkbGenerator *obj, GeneratorIcon * parentIcon, GeneratorIcon * selected){
    if (!parentIcon){
        return NULL;
    }
    GeneratorIcon *icon = new GeneratorIcon(obj, obj->getName(), parentIcon);
    parentIcon->replaceChild(selected, icon);
    //Order is important??
    behaviorGS->addItem(icon->linkToParent);
    behaviorGS->addItem(icon);
    return icon;
}

GeneratorIcon * BehaviorGraphView::initalizeAppendedIcon(hkbGenerator *obj, GeneratorIcon * parentIcon){
    if (!parentIcon){
        return NULL;
    }
    GeneratorIcon *icon = new GeneratorIcon(obj, obj->getName(), parentIcon);
    //Order is important??
    behaviorGS->addItem(icon->linkToParent);
    behaviorGS->addItem(icon);
    return icon;
}

bool BehaviorGraphView::drawBehaviorGraph(){
    QList <HkxObjectExpSharedPtr> objects;
    QList <GeneratorIcon *> parentIcons;
    QVector <short> objectChildCount;
    hkbGenerator *gen = NULL;
    int result;
    hkRootLevelContainer *root = static_cast<hkRootLevelContainer *>(behavior->getRootObject().data());
    hkbBehaviorGraph *graph = NULL;
    if (!root->namedVariants.isEmpty() && root->namedVariants.at(0).variant.data() && root->namedVariants.at(0).variant.data()->getSignature() == HKB_BEHAVIOR_GRAPH){
        graph = static_cast<hkbBehaviorGraph *>(root->namedVariants.at(0).variant.data());
        GeneratorIcon *icon = new GeneratorIcon(graph, graph->name);
        rootIcon = icon;
        behaviorGS->addItem(icon);
        objects.append(graph->rootGenerator);
        objectChildCount.append(1);
        parentIcons.append(icon);
    }else{
        return false;
    }
    while (!objects.isEmpty()){
        if (!objects.last().data()){
            return false;
        }
        gen = static_cast<hkbGenerator *>(objects.last().data());
        result = initializeIcons(gen, objects, parentIcons, objectChildCount);
        if (result == 0){
            if (!objectChildCount.isEmpty()){
                objectChildCount.last() = objectChildCount.last() + gen->addChildrenToList(objects);
            }
        }else if (result == -1){
            return false;
        }
    }
    return true;
}

BehaviorGraphView::~BehaviorGraphView(){
    ui->setBehaviorView(NULL);
}
