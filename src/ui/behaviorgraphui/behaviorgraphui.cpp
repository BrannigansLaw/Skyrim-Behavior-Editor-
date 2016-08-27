#include "src/ui/mainwindow.h"
#include "src/ui/fileselectwindow.h"
#include "src/filetypes/hkxfile.h"
#include "src/hkxclasses/generators/hkbgenerator.h"
#include "behaviorgraphui.h"
#include "src/hkxclasses/hkxobject.h"
#include "behaviorgraphicons.h"
#include "src/ui/objectdataui/hkobjectui.h"

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
    //contextMenu->addAction(removeObjAct);
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
    //connect(removeObjAct, SIGNAL(triggered()), this, SLOT(removeSelectedObject()));
    connect(removeObjBranchAct, SIGNAL(triggered()), this, SLOT(removeSelectedObjectBranchSlot()));
    if (ui){
        ui->setBehaviorView(this);
        //connect(this, SIGNAL(iconSelected(GeneratorIcon *)), ui, SLOT(changeCurrentDataWidget(GeneratorIcon *)));
    }
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

/*void BehaviorGraphView::removeIcon(GeneratorIcon *iconToRemove){
    GeneratorIcon *icon = NULL;
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
        iconToRemove->parent->children.replace(index, new GeneratorIcon(HkxObjectExpSharedPtr(ptr), ));
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
    iconToRemove = NULL;
}*/

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
                        if (gen->icons.isEmpty()){
                            gen->unlink();
                        }
                        behaviorGS->removeItem(icon);
                        behaviorGS->removeItem(icon->linkToParent);
                        //
                        delete icon->linkToParent;
                        delete icon;
                        icon = NULL;
                        //
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
    for (int d = 0; d < branch.size(); d++){
        expandBranchForIcon(branch.at(d));
    }
    for (int k = icons.size() - 1; k >= 0; k--){
        if (icons.at(k)){//Sometimes there are duplicates in icons??? Investigate why later...
            gen = static_cast<hkbGenerator *>(icons.at(k)->data.data());
            if (icons.at(k)->parent && !icons.at(k)->parent->children.isEmpty()){
                icons.at(k)->parent->children.removeAll(icons.at(k));
            }
            gen->icons.removeAll(icons.at(k));
            if (gen->icons.isEmpty()){
                gen->unlink();
            }
            behaviorGS->removeItem(icons.at(k));
            behaviorGS->removeItem(icons.at(k)->linkToParent);
            delete icons.at(k)->linkToParent;
            delete icons.at(k);
            icons[k] = NULL;
        }
    }
    return;
}

/*void BehaviorGraphView::removeSelectedObject(){
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
                    parentChild->generator = HkxObjectExpSharedPtr(getFirstChild(selectedIcon->data));   //If refcount < 1 remove from behavior pointer list?
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
    }
}*/

void BehaviorGraphView::deleteSelectedBranch(GeneratorIcon *icon){
    if (icon && icon->data.constData() && icon->parent && icon->parent->data.constData()){
        //parent->setDataValidity(false);
        if (icon->children.isEmpty()){
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
            removeChildIcons(icon);
        }
        icon = NULL;
    }
}

/*bool BehaviorGraphView::removeData(const HkxObjectExpSharedPtr & smtptr){
    bool removed = false;
    HkxObject *ptr = smtptr.data();
    if (ptr->getType() == HkxObject::TYPE_GENERATOR && ptr->ref < 4){
        for (int i = 0; i < behavior->generators.size(); i++){
            if (ptr == behavior->generators.at(i).constData()){
                if (ptr->getSignature() == HKB_STATE_MACHINE){
                    hkbStateMachine *obj = static_cast<hkbStateMachine *>(ptr);
                    for (int j = 0; j < obj->states.size(); j++){
                        for (int k = 0; k < behavior->generators.size(); k++){
                            if (obj->states.at(j).constData() == behavior->generators.at(k).constData()){
                                behavior->generators.removeAt(k);
                            }
                        }
                    }
                }else if (ptr->getSignature() == HKB_BLENDER_GENERATOR){
                    hkbBlenderGenerator *obj = static_cast<hkbBlenderGenerator *>(ptr);
                    for (int j = 0; j < obj->children.size(); j++){
                        for (int k = 0; k < behavior->generators.size(); k++){
                            if (obj->children.at(j).constData() == behavior->generators.at(k).constData()){
                                behavior->generators.removeAt(k);
                            }
                        }
                    }
                }else if (ptr->getSignature() == HKB_POSE_MATCHING_GENERATOR ){
                    hkbPoseMatchingGenerator *obj = static_cast<hkbPoseMatchingGenerator *>(ptr);
                    for (int j = 0; j < obj->children.size(); j++){
                        for (int k = 0; k < behavior->generators.size(); k++){
                            if (obj->children.at(j).constData() == behavior->generators.at(k).constData()){
                                behavior->generators.removeAt(k);
                            }
                        }
                    }
                }else if (ptr->getSignature() == BS_BONE_SWITCH_GENERATOR ){
                    BSBoneSwitchGenerator *obj = static_cast<BSBoneSwitchGenerator *>(ptr);
                    for (int j = 0; j < obj->ChildrenA.size(); j++){
                        for (int k = 0; k < behavior->generators.size(); k++){
                            if (obj->ChildrenA.at(j).constData() == behavior->generators.at(k).constData()){
                                behavior->generators.removeAt(k);
                            }
                        }
                    }
                }
                behavior->generators.removeAt(i);
                removed = true;
                break;
            }
        }
    }else if (ptr->getType() == HkxObject::TYPE_MODIFIER && ptr->ref < 4){
        for (int i = 0; i < behavior->modifiers.size(); i++){
            if (ptr == behavior->modifiers.at(i).constData()){
                behavior->modifiers.removeAt(i);
                //Some modifiers have unique children...
                removed = true;
                break;
            }
        }
    }else if (ptr->getType() == HkxObject::TYPE_OTHER && ptr->ref < 3){
        for (int i = 0; i < behavior->otherTypes.size(); i++){
            if (ptr == behavior->otherTypes.at(i).constData()){
                behavior->otherTypes.removeAt(i);
                removed = true;
                break;
            }
        }
    }
    return removed;
}*/

bool BehaviorGraphView::reconnectBranch(HkxObject *oldChild, HkxObject *newChild, GeneratorIcon *icon){
    if (/*!oldChild || */!newChild || !icon){
        return false;
    }
    if (icon->isGranfatherParadox(newChild)){
        return false;
    }
    GeneratorIcon *ptr = NULL;
    hkbGenerator *gen = static_cast<hkbGenerator *>(newChild);
    for (int i = 0; i < icon->children.size(); i++){
        if (icon->children.at(i)->data == oldChild){
            ptr = icon->children.at(i);
        }
    }
    removeSelectedObjectBranch(ptr);
    if (!gen->icons.isEmpty() && gen->icons.first()->pos().y() > icon->pos().y()){
        icon->children.append(gen->icons.first());
        if (gen->icons.first()->parent){
            gen->icons.first()->parent->children.removeAll(gen->icons.first());
            ptr = initalizeAppendedIcon(HkxObjectExpSharedPtr(gen), static_cast<hkbGenerator *>(gen), gen->icons.first()->parent);
            gen->icons.first()->parent = icon;
        }
    }else{
        //LEAKS MEMORY SOMETIMES WHEN ITEMS ARE DELETED!!! ONLY HAPPENS TO SM, OBJS REFFED TWICE???
        initalizeAppendedIcon(newChild, static_cast<hkbGenerator *>(newChild), icon);
    }
    static_cast<hkbGenerator *>(icon->data.data())->setDataValidity(true);
    if (behaviorGS && !behaviorGS->items(Qt::AscendingOrder).isEmpty()){
        GeneratorIcon *behaviorGraphIcon = static_cast<GeneratorIcon *>(scene()->items(Qt::AscendingOrder).first());
        //expandBranch(behaviorGraphIcon);
        //expandBranchForIcon(selectedData->icons.first());
        //expandBranch(selectedData->icons.first());
        expandBranch(behaviorGraphIcon);
        repositionIcons(behaviorGraphIcon, true);
        behaviorGS->update();
        //centerOn(newIcon);
    }
    behavior->removeData();
    return true;
}

void BehaviorGraphView::removeSelectedObjectBranchSlot(){
    removeSelectedObjectBranch(selectedIcon);
    ui->changeCurrentDataWidget(NULL);
    selectedIcon = NULL;
    behavior->removeData();
}

void BehaviorGraphView::removeSelectedObjectBranch(GeneratorIcon *icon){
    if (icon && icon->parent && icon->parent->data.constData()){
        qulonglong sig = icon->parent->data->getSignature();
        switch (sig){
        case HKB_STATE_MACHINE:
        {
            hkbStateMachine *parent = static_cast<hkbStateMachine *>(icon->parent->data.data());
            hkbStateMachineStateInfo *parentChild;
            for (int i = 0; i < parent->states.size(); i++){
                parentChild = static_cast<hkbStateMachineStateInfo *>(parent->states.at(i).data());
                if (parentChild->generator == icon->data){
                    deleteSelectedBranch(icon);
                    parentChild->generator = HkxObjectExpSharedPtr();
                    parent->setDataValidity(false);
                    break;
                }
            }
            break;
        }
        case HKB_MANUAL_SELECTOR_GENERATOR:
        {
            hkbManualSelectorGenerator *parent = static_cast<hkbManualSelectorGenerator *>(icon->parent->data.data());
            for (int i = 0; i < parent->generators.size(); i++){
                if (parent->generators.at(i) == icon->data){
                    deleteSelectedBranch(icon);
                    parent->generators.removeAt(i);
                    break;
                }
            }
            break;
        }
        case HKB_BLENDER_GENERATOR:
        {
            hkbBlenderGenerator *parent = static_cast<hkbBlenderGenerator *>(icon->parent->data.data());
            hkbBlenderGeneratorChild *parentChild;
            for (int i = 0; i < parent->children.size(); i++){
                parentChild = static_cast<hkbBlenderGeneratorChild *>(parent->children.at(i).data());
                if (parentChild->generator == icon->data){
                    deleteSelectedBranch(icon);
                    parentChild->generator = HkxObjectExpSharedPtr();
                    parent->setDataValidity(false);
                    break;
                }
            }
            break;
        }
        case BS_I_STATE_TAGGING_GENERATOR:
        {
            BSiStateTaggingGenerator *parent = static_cast<BSiStateTaggingGenerator *>(icon->parent->data.data());
            deleteSelectedBranch(icon);
            parent->pDefaultGenerator = HkxObjectExpSharedPtr();
            parent->setDataValidity(false);
            break;
        }
        case BS_BONE_SWITCH_GENERATOR:
        {
            BSBoneSwitchGenerator *parent = static_cast<BSBoneSwitchGenerator *>(icon->parent->data.data());
            BSBoneSwitchGeneratorBoneData *parentChild;
            if (parent->pDefaultGenerator == icon->data){
                deleteSelectedBranch(icon);
                parent->pDefaultGenerator = HkxObjectExpSharedPtr();
                parent->setDataValidity(false);
                break;
            }
            for (int i = 0; i < parent->ChildrenA.size(); i++){
                parentChild = static_cast<BSBoneSwitchGeneratorBoneData *>(parent->ChildrenA.at(i).data());
                if (parentChild->pGenerator == icon->data){
                    deleteSelectedBranch(icon);
                    parentChild->pGenerator = HkxObjectExpSharedPtr();
                    parent->setDataValidity(false);
                    break;
                }
            }
            break;
        }
        case BS_CYCLIC_BLEND_TRANSITION_GENERATOR:
        {
            BSCyclicBlendTransitionGenerator *parent = static_cast<BSCyclicBlendTransitionGenerator *>(icon->parent->data.data());
            deleteSelectedBranch(icon);
            parent->pBlenderGenerator = HkxObjectExpSharedPtr();
            parent->setDataValidity(false);
            break;
        }
        case BS_SYNCHRONIZED_CLIP_GENERATOR:
        {
            BSSynchronizedClipGenerator *parent = static_cast<BSSynchronizedClipGenerator *>(icon->parent->data.data());
            deleteSelectedBranch(icon);
            parent->pClipGenerator = HkxObjectExpSharedPtr();
            parent->setDataValidity(false);
            break;
        }
        case HKB_MODIFIER_GENERATOR:
        {
            hkbModifierGenerator *parent = static_cast<hkbModifierGenerator *>(icon->parent->data.data());
            if (parent->modifier == icon->data){
                deleteSelectedBranch(icon);
                //removeData(parent->modifier);
                parent->modifier = HkxObjectExpSharedPtr();
            }else if (parent->generator == icon->data){
                deleteSelectedBranch(icon);
                parent->generator = HkxObjectExpSharedPtr();
                parent->setDataValidity(false);
            }
            break;
        }
        case BS_OFFSET_ANIMATION_GENERATOR:
        {
            BSOffsetAnimationGenerator *parent = static_cast<BSOffsetAnimationGenerator *>(icon->parent->data.data());
            if (parent->pOffsetClipGenerator == icon->data){
                deleteSelectedBranch(icon);
                //removeData(parent->pOffsetClipGenerator);
                parent->pOffsetClipGenerator = HkxObjectExpSharedPtr();
                parent->setDataValidity(false);
            }else if (parent->pDefaultGenerator == icon->data){
                deleteSelectedBranch(icon);
                //removeData(parent->pDefaultGenerator);
                parent->pDefaultGenerator = HkxObjectExpSharedPtr();
                parent->setDataValidity(false);
            }
            break;
        }
        case HKB_POSE_MATCHING_GENERATOR:
        {
            hkbPoseMatchingGenerator *parent = static_cast<hkbPoseMatchingGenerator *>(icon->parent->data.data());
            hkbBlenderGeneratorChild *parentChild;
            for (int i = 0; i < parent->children.size(); i++){
                parentChild = static_cast<hkbBlenderGeneratorChild *>(parent->children.at(i).data());
                if (parentChild->generator == icon->data){
                    deleteSelectedBranch(icon);
                    //removeData(parentChild->generator);
                    parentChild->generator = HkxObjectExpSharedPtr();
                    parent->setDataValidity(false);
                    break;
                }
            }
            break;
        }
        case HKB_BEHAVIOR_GRAPH:
        {
            hkbBehaviorGraph *parent = static_cast<hkbBehaviorGraph *>(icon->parent->data.data());
            deleteSelectedBranch(icon);
            //removeData(parent->rootGenerator);
            parent->rootGenerator = HkxObjectExpSharedPtr();
            parent->setDataValidity(false);
            break;
        }
        default:
            return;
        }
        if (behaviorGS && !behaviorGS->items(Qt::AscendingOrder).isEmpty()){
            GeneratorIcon *behaviorGraphIcon = static_cast<GeneratorIcon *>(scene()->items(Qt::AscendingOrder).first())/*->children.first()*/;
            //expandBranch(behaviorGraphIcon, true);
            repositionIcons(behaviorGraphIcon, true);
            //Cycle through data and call removeData()???
            behaviorGS->update();
        }
        //behavior->removeData();
    }
}

HkxObject * BehaviorGraphView::getFirstChild(const HkxObjectExpSharedPtr & obj){
    if (obj.constData()){
        qulonglong sig = obj->getSignature();
        switch (sig) {
        case HKB_STATE_MACHINE:
        {
            hkbStateMachine *ptr = static_cast<hkbStateMachine *>(obj.data());
            if (!ptr->states.isEmpty()){
                return static_cast<hkbStateMachineStateInfo *>(ptr->states.first().data())->generator.data();
            }
            break;
        }
        case HKB_MANUAL_SELECTOR_GENERATOR:
        {
            hkbManualSelectorGenerator *ptr = static_cast<hkbManualSelectorGenerator *>(obj.data());
            if (!ptr->generators.isEmpty()){
                return ptr->generators.first().data();
            }
            break;
        }
        case HKB_BLENDER_GENERATOR:
        {
            hkbBlenderGenerator *ptr = static_cast<hkbBlenderGenerator *>(obj.data());
            if (!ptr->children.isEmpty()){
                return static_cast<hkbBlenderGeneratorChild *>(ptr->children.first().data())->generator.data();
            }
            break;
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
            if (!ptr->children.isEmpty()){
                return static_cast<hkbBlenderGeneratorChild *>(ptr->children.first().data())->generator.data();
            }
            break;
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

HkxObjectExpSharedPtr * BehaviorGraphView::getFirstChildSmartPointer(HkxObject *obj){
    if (obj){
        qulonglong sig = obj->getSignature();
        switch (sig) {
        case HKB_STATE_MACHINE:
        {
            hkbStateMachine *ptr = static_cast<hkbStateMachine *>(obj);
            if (!ptr->states.isEmpty()){
                return &static_cast<hkbStateMachineStateInfo *>(ptr->states.first().data())->generator;
            }else{
                behavior->generators.append(HkxObjectExpSharedPtr(new hkbStateMachineStateInfo(behavior)));
                ptr->states.append(behavior->generators.last());
                return &static_cast<hkbStateMachineStateInfo *>(ptr->states.first().data())->generator;
            }
        }
        case HKB_MANUAL_SELECTOR_GENERATOR:
        {
            hkbManualSelectorGenerator *ptr = static_cast<hkbManualSelectorGenerator *>(obj);
            if (!ptr->generators.isEmpty()){
                return &ptr->generators.first();
            }else{
                ptr->generators.append(HkxObjectExpSharedPtr());
                return &ptr->generators.first();
            }
        }
        case HKB_BLENDER_GENERATOR:
        {
            hkbBlenderGenerator *ptr = static_cast<hkbBlenderGenerator *>(obj);
            if (!ptr->children.isEmpty()){
                return &static_cast<hkbBlenderGeneratorChild *>(ptr->children.first().data())->generator;
            }else{
                behavior->generators.append(HkxObjectExpSharedPtr(new hkbBlenderGeneratorChild(behavior)));
                ptr->children.append(behavior->generators.last());
                return &static_cast<hkbBlenderGeneratorChild *>(ptr->children.first().data())->generator;
            }
        }
        case BS_I_STATE_TAGGING_GENERATOR:
        {
            BSiStateTaggingGenerator *ptr = static_cast<BSiStateTaggingGenerator *>(obj);
            return &ptr->pDefaultGenerator;
        }
        case BS_BONE_SWITCH_GENERATOR:
        {
            BSBoneSwitchGenerator *ptr = static_cast<BSBoneSwitchGenerator *>(obj);
            return &ptr->pDefaultGenerator;
        }
        case BS_CYCLIC_BLEND_TRANSITION_GENERATOR:
        {
            BSCyclicBlendTransitionGenerator *ptr = static_cast<BSCyclicBlendTransitionGenerator *>(obj);
            return &ptr->pBlenderGenerator;
        }
        case BS_SYNCHRONIZED_CLIP_GENERATOR:
        {
            BSSynchronizedClipGenerator *ptr = static_cast<BSSynchronizedClipGenerator *>(obj);
            return &ptr->pClipGenerator;
        }
        case HKB_MODIFIER_GENERATOR:
        {
            hkbModifierGenerator *ptr = static_cast<hkbModifierGenerator *>(obj);
            return &ptr->generator;
        }
        case BS_OFFSET_ANIMATION_GENERATOR:
        {
            BSOffsetAnimationGenerator *ptr = static_cast<BSOffsetAnimationGenerator *>(obj);
            return &ptr->pDefaultGenerator;
        }
        case HKB_POSE_MATCHING_GENERATOR:
        {
            hkbPoseMatchingGenerator *ptr = static_cast<hkbPoseMatchingGenerator *>(obj);
            if (!ptr->children.isEmpty()){
                return &static_cast<hkbBlenderGeneratorChild *>(ptr->children.first().data())->generator;
            }else{
                behavior->generators.append(HkxObjectExpSharedPtr(new hkbBlenderGeneratorChild(behavior)));
                ptr->children.append(behavior->generators.last());
                return &static_cast<hkbBlenderGeneratorChild *>(ptr->children.first().data())->generator;
            }
        }
        case HKB_BEHAVIOR_GRAPH:
        {
            hkbBehaviorGraph *ptr = static_cast<hkbBehaviorGraph *>(obj);
            return &ptr->rootGenerator;
        }
        default:
            return NULL;
        }
    }
    return NULL;
}

/*bool BehaviorGraphView::drawBranch(GeneratorIcon * rootIcon){
    QList <HkxObjectExpSharedPtr> objects;
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
}*/

template <typename T>
bool BehaviorGraphView::appendObject(T * ptr, GeneratorIcon *parentObjIcon){
    if (!ptr || !parentObjIcon || !parentObjIcon->data.constData()){
        return false;
    }
    hkbGenerator *selectedData = static_cast<hkbGenerator *>(parentObjIcon->data.data());
    bool value = true;
    GeneratorIcon *newIcon = NULL;
    qulonglong sig = parentObjIcon->data->getSignature();
    switch (sig){
    case HKB_STATE_MACHINE:
    {
        hkbStateMachine *obj = static_cast<hkbStateMachine *>(parentObjIcon->data.data());
        hkbStateMachineStateInfo *objChild = new hkbStateMachineStateInfo(behavior);
        //bool isValid = true;
        //behavior->generators.append(HkxObjectExpSharedPtr(objChild));
        obj->states.append(HkxObjectExpSharedPtr(objChild));
        objChild->generator = HkxObjectExpSharedPtr(ptr);
        newIcon = initalizeAppendedIcon(objChild->generator, ptr, selectedData->icons.first());
        obj->setDataValidity(true);
        break;
    }
    case HKB_MANUAL_SELECTOR_GENERATOR:
    {
        hkbManualSelectorGenerator *obj = static_cast<hkbManualSelectorGenerator *>(parentObjIcon->data.data());
        obj->generators.append(HkxObjectExpSharedPtr(ptr));
        obj->setDataValidity(true);
        newIcon = initalizeAppendedIcon(obj->generators.last(), ptr, selectedData->icons.first());
        break;
    }
    case HKB_BLENDER_GENERATOR:
    {
        hkbBlenderGenerator *obj = static_cast<hkbBlenderGenerator *>(parentObjIcon->data.data());
        hkbBlenderGeneratorChild *objChild = new hkbBlenderGeneratorChild(behavior);
        //behavior->generators.append(HkxObjectExpSharedPtr(objChild));
        obj->children.append(HkxObjectExpSharedPtr(objChild));
        objChild->generator = HkxObjectExpSharedPtr(ptr);
        newIcon = initalizeAppendedIcon(objChild->generator, ptr, selectedData->icons.first());
        obj->setDataValidity(true);
        break;
    }
    case BS_I_STATE_TAGGING_GENERATOR:
    {
        BSiStateTaggingGenerator *obj = static_cast<BSiStateTaggingGenerator *>(parentObjIcon->data.data());
        if (!obj->pDefaultGenerator.constData() || confirmationDialogue("The existing child of this object will be replaced by the new object!\nAre you sure you want to do this?", this) && !selectedIcon->children.isEmpty()){
            if (!parentObjIcon->children.isEmpty()){
                removeSelectedObjectBranch(parentObjIcon->children.first());
            }
            obj->pDefaultGenerator = HkxObjectExpSharedPtr(ptr);
            obj->setDataValidity(true);
            newIcon = initalizeAppendedIcon(obj->pDefaultGenerator, ptr, selectedData->icons.first());
        }else{
            value = false;
        }
        break;
    }
    case BS_CYCLIC_BLEND_TRANSITION_GENERATOR:
    {
        BSCyclicBlendTransitionGenerator *obj = static_cast<BSCyclicBlendTransitionGenerator *>(parentObjIcon->data.data());
        if (!obj->pBlenderGenerator.constData() || confirmationDialogue("The existing child of this object will be replaced by the new object!\nAre you sure you want to do this?", this) && !selectedIcon->children.isEmpty()){
            if (!parentObjIcon->children.isEmpty()){
                removeSelectedObjectBranch(parentObjIcon->children.first());
            }
            obj->pBlenderGenerator = HkxObjectExpSharedPtr(ptr);
            obj->setDataValidity(true);
            newIcon = initalizeAppendedIcon(obj->pBlenderGenerator, ptr, selectedData->icons.first());
        }else{
            value = false;
        }
        break;
    }
    case BS_BONE_SWITCH_GENERATOR:
    {
        BSBoneSwitchGenerator *obj = static_cast<BSBoneSwitchGenerator *>(parentObjIcon->data.data());
        BSBoneSwitchGeneratorBoneData *objChild = new BSBoneSwitchGeneratorBoneData(behavior);
        //behavior->generators.append(HkxObjectExpSharedPtr(objChild));
        obj->ChildrenA.append(HkxObjectExpSharedPtr(objChild));
        objChild->pGenerator = HkxObjectExpSharedPtr(ptr);
        newIcon = initalizeAppendedIcon(objChild->pGenerator, ptr, selectedData->icons.first());
        if (obj->pDefaultGenerator.constData()){
            obj->setDataValidity(true);
        }
        break;
    }
    case HKB_MODIFIER_GENERATOR:
    {
        hkbModifierGenerator *obj = static_cast<hkbModifierGenerator *>(parentObjIcon->data.data());
        if (!obj->generator.constData() || confirmationDialogue("The existing child of this object will be replaced by the new object!\nAre you sure you want to do this?", this) && !selectedIcon->children.isEmpty()){
            if (!parentObjIcon->children.isEmpty()){
                removeSelectedObjectBranch(parentObjIcon->children.first());
            }
            obj->generator = HkxObjectExpSharedPtr(ptr);
            obj->setDataValidity(true);
            newIcon = initalizeAppendedIcon(obj->generator, ptr, selectedData->icons.first());
        }else{
            value = false;
        }
        break;
    }
    case BS_SYNCHRONIZED_CLIP_GENERATOR:
    {
        BSSynchronizedClipGenerator *obj = static_cast<BSSynchronizedClipGenerator *>(parentObjIcon->data.data());
        if (!obj->pClipGenerator.constData() || confirmationDialogue("The existing child of this object will be replaced by the new object!\nAre you sure you want to do this?", this) && !selectedIcon->children.isEmpty()){
            if (!parentObjIcon->children.isEmpty()){
                removeSelectedObjectBranch(parentObjIcon->children.first());
            }
            obj->pClipGenerator = HkxObjectExpSharedPtr(ptr);
            obj->setDataValidity(true);
            newIcon = initalizeAppendedIcon(obj->pClipGenerator, ptr, selectedData->icons.first());
        }else{
            value = false;
        }
        break;
    }
    case BS_OFFSET_ANIMATION_GENERATOR:
    {
        BSOffsetAnimationGenerator *obj = static_cast<BSOffsetAnimationGenerator *>(parentObjIcon->data.data());
        if (!obj->pDefaultGenerator.constData() || confirmationDialogue("The existing pDefaultGenerator of this object will be replaced by the new object!\nAre you sure you want to do this?", this) && !selectedIcon->children.isEmpty()){
            if (!parentObjIcon->children.isEmpty()){
                removeSelectedObjectBranch(parentObjIcon->children.first());
            }
            obj->pDefaultGenerator = HkxObjectExpSharedPtr(ptr);
            if (obj->pOffsetClipGenerator.constData()){
                obj->setDataValidity(true);
            }
            newIcon = initalizeAppendedIcon(obj->pDefaultGenerator, ptr, selectedData->icons.first());
        }else{
            value = false;
        }
        break;
    }
    case HKB_POSE_MATCHING_GENERATOR:
    {
        hkbPoseMatchingGenerator *obj = static_cast<hkbPoseMatchingGenerator *>(parentObjIcon->data.data());
        hkbBlenderGeneratorChild *objChild = new hkbBlenderGeneratorChild(behavior);
        //behavior->generators.append(HkxObjectExpSharedPtr(objChild));
        obj->children.append(HkxObjectExpSharedPtr(objChild));
        objChild->generator = HkxObjectExpSharedPtr(ptr);
        newIcon = initalizeAppendedIcon(objChild->generator, ptr, selectedData->icons.first());
        obj->setDataValidity(true);
        break;
    }
    case HKB_BEHAVIOR_GRAPH:
    {
        hkbBehaviorGraph *obj = static_cast<hkbBehaviorGraph *>(parentObjIcon->data.data());
        if (!obj->rootGenerator.constData() || confirmationDialogue("The existing child of this object will be replaced by the new object!\nAre you sure you want to do this?", this) && !selectedIcon->children.isEmpty()){
            if (!parentObjIcon->children.isEmpty()){
                removeSelectedObjectBranch(parentObjIcon->children.first());
            }
            obj->rootGenerator = HkxObjectExpSharedPtr(ptr);
            if (obj->data.constData()){
                obj->setDataValidity(true);
            }
            newIcon = initalizeAppendedIcon(obj->rootGenerator, ptr, selectedData->icons.first());
        }else{
            value = false;
        }
        break;
    }
    default:
        return false;
    }
    behavior->generators.append(HkxObjectExpSharedPtr(ptr));
    //behavior->generators.append(HkxObjectExpSharedPtr(state));
    if (behaviorGS && !behaviorGS->items(Qt::AscendingOrder).isEmpty() && newIcon && ui){
        GeneratorIcon *behaviorGraphIcon = static_cast<GeneratorIcon *>(scene()->items(Qt::AscendingOrder).first());
        //expandBranch(behaviorGraphIcon);
        ui->changeCurrentDataWidget(newIcon);
        expandBranchForIcon(selectedData->icons.first());
        //expandBranch(selectedData->icons.first());
        expandBranch(behaviorGraphIcon);
        repositionIcons(behaviorGraphIcon, true);
        behaviorGS->update();
        centerOn(newIcon);
    }
    return value;
}

void BehaviorGraphView::appendStateMachine(){
    if (selectedIcon && selectedIcon->data.constData()){
        hkbStateMachine *obj = new hkbStateMachine(behavior);
        obj->setDataValidity(false);
        //hkbStateMachineStateInfo *objChild = new hkbStateMachineStateInfo(behavior);
        //obj->states.append(HkxObjectExpSharedPtr(objChild));
        if (!appendObject(obj, selectedIcon)){
            delete obj;
        }else{
            if (ui){
                ui->changeCurrentDataWidget(selectedIcon);
            }
        }
    }
}

void BehaviorGraphView::appendManualSelectorGenerator(){
    if (selectedIcon && selectedIcon->data.constData()){
        hkbManualSelectorGenerator *obj = new hkbManualSelectorGenerator(behavior);
        obj->setDataValidity(false);
        if (!appendObject(obj, selectedIcon)){
            delete obj;
        }else{
            if (ui){
                ui->changeCurrentDataWidget(selectedIcon);
            }
        }
    }
}

void BehaviorGraphView::appendModifierGenerator(){
    if (selectedIcon && selectedIcon->data.constData()){
        hkbModifierGenerator *obj = new hkbModifierGenerator(behavior);
        obj->setDataValidity(false);
        if (!appendObject(obj, selectedIcon)){
            delete obj;
        }else{
            if (ui){
                ui->changeCurrentDataWidget(selectedIcon);
            }
        }
    }
}

void BehaviorGraphView::appendIStateTaggingGenerator(){
    if (selectedIcon && selectedIcon->data.constData()){
        BSiStateTaggingGenerator *obj = new BSiStateTaggingGenerator(behavior);
        obj->setDataValidity(false);
        if (!appendObject(obj, selectedIcon)){
            delete obj;
        }else{
            if (ui){
                ui->changeCurrentDataWidget(selectedIcon);
            }
        }
    }
}

void BehaviorGraphView::appendSynchronizedClipGenerator(){
    if (selectedIcon && selectedIcon->data.constData()){
        BSSynchronizedClipGenerator *obj = new BSSynchronizedClipGenerator(behavior);
        obj->setDataValidity(false);
        if (!appendObject(obj, selectedIcon)){
            delete obj;
        }else{
            if (ui){
                ui->changeCurrentDataWidget(selectedIcon);
            }
        }
    }
}

void BehaviorGraphView::appendOffsetAnimationGenerator(){
    if (selectedIcon && selectedIcon->data.constData()){
        BSOffsetAnimationGenerator *obj = new BSOffsetAnimationGenerator(behavior);
        obj->setDataValidity(false);
        if (!appendObject(obj, selectedIcon)){
            delete obj;
        }else{
            if (ui){
                ui->changeCurrentDataWidget(selectedIcon);
            }
        }
    }
}

void BehaviorGraphView::appendCyclicBlendTransitionGenerator(){
    if (selectedIcon && selectedIcon->data.constData()){
        BSCyclicBlendTransitionGenerator *obj = new BSCyclicBlendTransitionGenerator(behavior);
        obj->setDataValidity(false);
        if (!appendObject(obj, selectedIcon)){
            delete obj;
        }else{
            if (ui){
                ui->changeCurrentDataWidget(selectedIcon);
            }
        }
    }
}

void BehaviorGraphView::appendPoseMatchingGenerator(){
    if (selectedIcon && selectedIcon->data.constData()){
        hkbPoseMatchingGenerator *obj = new hkbPoseMatchingGenerator(behavior);
        obj->setDataValidity(false);
        //hkbBlenderGeneratorChild *objChild = new hkbBlenderGeneratorChild(behavior);
        //obj->children.append(HkxObjectExpSharedPtr(objChild));
        if (!appendObject(obj, selectedIcon)){
            delete obj;
            //delete objChild;
        }else{
            if (ui){
                ui->changeCurrentDataWidget(selectedIcon);
            }
        }
    }
}

void BehaviorGraphView::appendBlenderGenerator(){
    if (selectedIcon && selectedIcon->data.constData()){
        hkbBlenderGenerator *obj = new hkbBlenderGenerator(behavior);
        obj->setDataValidity(false);
        //hkbBlenderGeneratorChild *objChild = new hkbBlenderGeneratorChild(behavior);
        //obj->children.append(HkxObjectExpSharedPtr(objChild));
        if (!appendObject(obj, selectedIcon)){
            delete obj;
            //delete objChild;
        }else{
            if (ui){
                ui->changeCurrentDataWidget(selectedIcon);
            }
        }
    }
}

void BehaviorGraphView::appendBoneSwitchGenerator(){
    if (selectedIcon && selectedIcon->data.constData()){
        BSBoneSwitchGenerator *obj = new BSBoneSwitchGenerator(behavior);
        obj->setDataValidity(false);
        if (!appendObject(obj, selectedIcon)){
            delete obj;
        }else{
            if (ui){
                ui->changeCurrentDataWidget(selectedIcon);
            }
        }
    }
}

void BehaviorGraphView::appendClipGenerator(){
    if (selectedIcon && selectedIcon->data.constData()){
        hkbClipGenerator *obj = new hkbClipGenerator(behavior);
        if (!appendObject(obj, selectedIcon)){
            delete obj;
        }else{
            if (ui){
                ui->changeCurrentDataWidget(selectedIcon);
            }
        }
    }
}

void BehaviorGraphView::appendBehaviorReferenceGenerator(){
    if (selectedIcon && selectedIcon->data.constData()){
        hkbBehaviorReferenceGenerator *obj = new hkbBehaviorReferenceGenerator(behavior);
        if (!appendObject(obj, selectedIcon)){
            delete obj;
        }else{
            if (ui){
                ui->changeCurrentDataWidget(selectedIcon);
            }
        }
    }
}

template <typename T>
void BehaviorGraphView::wrapObject(T *obj, GeneratorIcon *parentObjIcon){
    if (!obj || !parentObjIcon || !parentObjIcon->parent || !parentObjIcon->parent->data.constData()){
        return;
    }
    GeneratorIcon *newIcon = NULL;
    qulonglong sig = parentObjIcon->parent->data->getSignature();
    HkxObjectExpSharedPtr *smtptr = NULL;
    bool drawIcon = true;
    switch (sig){
    case HKB_STATE_MACHINE:
    {
        const hkbStateMachine *parent = static_cast<const hkbStateMachine *>(parentObjIcon->parent->data.constData());
        hkbStateMachineStateInfo *parentChild;
        for (int i = 0; i < parent->states.size(); i++){
            parentChild = static_cast<hkbStateMachineStateInfo *>(parent->states.at(i).data());
            if (parentChild->generator == parentObjIcon->data){
                smtptr = getFirstChildSmartPointer(obj);
                if (!smtptr){
                    return;
                }
                *smtptr = parentChild->generator;
                parentChild->generator = HkxObjectExpSharedPtr(obj);
                if (drawIcon){
                    newIcon = initalizeInjectedIcon(parentChild->generator, obj, parentObjIcon->parent, selectedIcon);
                    drawIcon = false;
                }
            }
        }
        break;
    }
    case HKB_MANUAL_SELECTOR_GENERATOR:
    {
        hkbManualSelectorGenerator *parent = static_cast<hkbManualSelectorGenerator *>(parentObjIcon->parent->data.data());
        for (int i = 0; i < parent->generators.size(); i++){
            if (parent->generators.at(i) == parentObjIcon->data){
                smtptr = getFirstChildSmartPointer(obj);
                if (!smtptr){
                    return;
                }
                *smtptr = parent->generators.at(i);
                parent->generators[i] = HkxObjectExpSharedPtr(obj);
                if (drawIcon){
                    newIcon = initalizeInjectedIcon(parent->generators.at(i), obj, parentObjIcon->parent, selectedIcon);
                    drawIcon = false;
                }
            }
        }
        break;
    }
    case HKB_BLENDER_GENERATOR:
    {
        const hkbBlenderGenerator *parent = static_cast<const hkbBlenderGenerator *>(parentObjIcon->parent->data.constData());
        hkbBlenderGeneratorChild *parentChild;
        for (int i = 0; i < parent->children.size(); i++){
            parentChild = static_cast<hkbBlenderGeneratorChild *>(parent->children.at(i).data());
            if (parentChild->generator == parentObjIcon->data){
                smtptr = getFirstChildSmartPointer(obj);
                if (!smtptr){
                    return;
                }
                *smtptr = parentChild->generator;
                parentChild->generator = HkxObjectExpSharedPtr(obj);
                if (drawIcon){
                    newIcon = initalizeInjectedIcon(parentChild->generator, obj, parentObjIcon->parent, selectedIcon);
                    drawIcon = false;
                }
            }
        }
        break;
    }
    case BS_I_STATE_TAGGING_GENERATOR:
    {
        BSiStateTaggingGenerator *parent = static_cast<BSiStateTaggingGenerator *>(parentObjIcon->parent->data.data());
        if (parent->pDefaultGenerator == parentObjIcon->data){
            smtptr = getFirstChildSmartPointer(obj);
            if (!smtptr){
                return;
            }
            *smtptr = parent->pDefaultGenerator;
            parent->pDefaultGenerator = HkxObjectExpSharedPtr(obj);
            newIcon = initalizeInjectedIcon(parent->pDefaultGenerator, obj, parentObjIcon->parent, selectedIcon);
        }
        break;
    }
    case BS_BONE_SWITCH_GENERATOR:
    {
        BSBoneSwitchGenerator *parent = static_cast<BSBoneSwitchGenerator *>(parentObjIcon->parent->data.data());
        BSBoneSwitchGeneratorBoneData *parentChild;
        if (parent->pDefaultGenerator == parentObjIcon->data){
            smtptr = getFirstChildSmartPointer(obj);
            if (!smtptr){
                return;
            }
            *smtptr = parent->pDefaultGenerator;
            parent->pDefaultGenerator = HkxObjectExpSharedPtr(obj);
            newIcon = initalizeInjectedIcon(parent->pDefaultGenerator, obj, parentObjIcon->parent, selectedIcon);
            drawIcon = false;
        }
        for (int i = 0; i < parent->ChildrenA.size(); i++){
            parentChild = static_cast<BSBoneSwitchGeneratorBoneData *>(parent->ChildrenA.at(i).data());
            if (parentChild->pGenerator == parentObjIcon->data){
                smtptr = getFirstChildSmartPointer(obj);
                if (!smtptr){
                    return;
                }
                *smtptr = parentChild->pGenerator;
                parentChild->pGenerator = HkxObjectExpSharedPtr(obj);
                if (drawIcon){
                    newIcon = initalizeInjectedIcon(parentChild->pGenerator, obj, parentObjIcon->parent, selectedIcon);
                    drawIcon = false;
                }
            }
        }
        break;
    }
    case HKB_MODIFIER_GENERATOR:
    {
        hkbModifierGenerator *parent = static_cast<hkbModifierGenerator *>(parentObjIcon->parent->data.data());
        if (parent->generator == parentObjIcon->data){
            smtptr = getFirstChildSmartPointer(obj);
            if (!smtptr){
                return;
            }
            *smtptr = parent->generator;
            parent->generator = HkxObjectExpSharedPtr(obj);
            newIcon = initalizeInjectedIcon(parent->generator, obj, parentObjIcon->parent, selectedIcon);
        }
        break;
    }
    case BS_OFFSET_ANIMATION_GENERATOR:
    {
        BSOffsetAnimationGenerator *parent = static_cast<BSOffsetAnimationGenerator *>(parentObjIcon->parent->data.data());
        if (parent->pDefaultGenerator == parentObjIcon->data){
            smtptr = getFirstChildSmartPointer(obj);
            if (!smtptr){
                return;
            }
            *smtptr = parent->pDefaultGenerator;
            parent->pDefaultGenerator = HkxObjectExpSharedPtr(obj);
            newIcon = initalizeInjectedIcon(parent->pDefaultGenerator, obj, parentObjIcon->parent, selectedIcon);
        }
        break;
    }
    case HKB_POSE_MATCHING_GENERATOR:
    {
        const hkbPoseMatchingGenerator *parent = static_cast<const hkbPoseMatchingGenerator *>(parentObjIcon->parent->data.constData());
        hkbBlenderGeneratorChild *parentChild;
        for (int i = 0; i < parent->children.size(); i++){
            parentChild = static_cast<hkbBlenderGeneratorChild *>(parent->children.at(i).data());
            if (parentChild->generator == parentObjIcon->data){
                smtptr = getFirstChildSmartPointer(obj);
                if (!smtptr){
                    return;
                }
                *smtptr = parentChild->generator;
                parentChild->generator = HkxObjectExpSharedPtr(obj);
                if (drawIcon){
                    newIcon = initalizeInjectedIcon(parentChild->generator, obj, parentObjIcon->parent, selectedIcon);
                    drawIcon = false;
                }
            }
        }
        break;
    }
    case HKB_BEHAVIOR_GRAPH:
    {
        hkbBehaviorGraph *parent = static_cast<hkbBehaviorGraph *>(parentObjIcon->parent->data.data());
        if (parent->rootGenerator == parentObjIcon->data){
            smtptr = getFirstChildSmartPointer(obj);
            if (!smtptr){
                return;
            }
            *smtptr = parent->rootGenerator;
            parent->rootGenerator = HkxObjectExpSharedPtr(obj);
            newIcon = initalizeInjectedIcon(parent->rootGenerator, obj, parentObjIcon->parent, selectedIcon);
        }
        break;
    }
    default:
        delete obj;
        return;
    }
    behavior->generators.append(HkxObjectExpSharedPtr(obj));
    if (behaviorGS && !behaviorGS->items(Qt::AscendingOrder).isEmpty() && newIcon && ui){
        GeneratorIcon *behaviorGraphIcon = static_cast<GeneratorIcon *>(scene()->items(Qt::AscendingOrder).first());
        //expandBranch(behaviorGraphIcon, true);
        ui->changeCurrentDataWidget(newIcon);
        repositionIcons(behaviorGraphIcon, true);
        behaviorGS->update();
        centerOn(newIcon);
    }
}

void BehaviorGraphView::wrapManualSelectorGenerator(){
    if (selectedIcon && selectedIcon->parent && selectedIcon->parent->data.constData()){
        hkbManualSelectorGenerator *obj = new hkbManualSelectorGenerator(behavior);
        wrapObject(obj, selectedIcon);
    }
}

void BehaviorGraphView::wrapModifierGenerator(){
    if (selectedIcon && selectedIcon->parent && selectedIcon->parent->data.constData()){
        hkbModifierGenerator *obj = new hkbModifierGenerator(behavior);
        wrapObject(obj, selectedIcon);
    }
}

void BehaviorGraphView::wrapIStateTaggingGenerator(){
    if (selectedIcon && selectedIcon->parent && selectedIcon->parent->data.constData()){
        BSiStateTaggingGenerator *obj = new BSiStateTaggingGenerator(behavior);
        wrapObject(obj, selectedIcon);
    }
}

void BehaviorGraphView::wrapSynchronizedClipGenerator(){
    if (selectedIcon && selectedIcon->parent && selectedIcon->parent->data.constData()){
        BSSynchronizedClipGenerator *obj = new BSSynchronizedClipGenerator(behavior);
        wrapObject(obj, selectedIcon);
    }
}

void BehaviorGraphView::wrapOffsetAnimationGenerator(){
    if (selectedIcon && selectedIcon->parent && selectedIcon->parent->data.constData()){
        BSOffsetAnimationGenerator *obj = new BSOffsetAnimationGenerator(behavior);
        wrapObject(obj, selectedIcon);
    }
}

void BehaviorGraphView::wrapCyclicBlendTransitionGenerator(){
    if (selectedIcon && selectedIcon->parent && selectedIcon->parent->data.constData()){
        BSCyclicBlendTransitionGenerator *obj = new BSCyclicBlendTransitionGenerator(behavior);
        wrapObject(obj, selectedIcon);
    }
}

void BehaviorGraphView::wrapBoneSwitchGenerator(){
    if (selectedIcon && selectedIcon->parent && selectedIcon->parent->data.constData()){
        BSBoneSwitchGenerator *obj = new BSBoneSwitchGenerator(behavior);
        obj->setDataValidity(false);
        wrapObject(obj, selectedIcon);
    }
}

void BehaviorGraphView::wrapStateMachine(){
    if (selectedIcon && selectedIcon->parent && selectedIcon->parent->data.constData()){
        hkbStateMachine *obj = new hkbStateMachine(behavior);
        hkbStateMachineStateInfo *state = new hkbStateMachineStateInfo(behavior);
        obj->states.append(HkxObjectExpSharedPtr(state));
        wrapObject(obj, selectedIcon);
    }
}

void BehaviorGraphView::wrapBlenderGenerator(){
    if (selectedIcon && selectedIcon->parent && selectedIcon->parent->data.constData()){
        hkbBlenderGenerator *obj = new hkbBlenderGenerator(behavior);
        hkbBlenderGeneratorChild *blendChild = new hkbBlenderGeneratorChild(behavior);
        obj->children.append(HkxObjectExpSharedPtr(blendChild));
        wrapObject(obj, selectedIcon);
    }
}

void BehaviorGraphView::wrapPoseMatchingGenerator(){
    if (selectedIcon && selectedIcon->parent && selectedIcon->parent->data.constData()){
        hkbPoseMatchingGenerator *obj = new hkbPoseMatchingGenerator(behavior);
        hkbBlenderGeneratorChild *blendChild = new hkbBlenderGeneratorChild(behavior);
        obj->children.append(HkxObjectExpSharedPtr(blendChild));
        wrapObject(obj, selectedIcon);
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
    /*if (selectedIcon && QRectF(mapFromScene(selectedIcon->pos()), QSizeF(selectedIcon->boundingRect().width(), selectedIcon->boundingRect().height())).contains(event->pos())){
        emit iconSelected(selectedIcon);
    }*/
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

template<typename T>
int BehaviorGraphView::initializeIconsForNewBranch(const T & ptr, QList<HkxObjectExpSharedPtr> & objects, QList<GeneratorIcon *> & parentIcons, QVector <short> & objectChildCount){
    if (parentIcons.isEmpty()){
        return -1;
    }
    if (objects.isEmpty()){
        return -1;
    }
    if (objectChildCount.isEmpty()){
        return -1;
    }
    bool isBranchTip = true;
    GeneratorIcon *icon = addIconToGraph(objects.last(), ptr, parentIcons.last());
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

template<typename T>
int BehaviorGraphView::initializeIcons(const T & ptr, QList<HkxObjectExpSharedPtr> & objects, QList<GeneratorIcon *> & parentIcons, QVector <short> & objectChildCount){
    if (parentIcons.isEmpty()){
        return -1;
    }
    if (objects.isEmpty()){
        return -1;
    }
    if (objectChildCount.isEmpty()){
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
    GeneratorIcon *icon = addIconToGraph(objects.last(), ptr, parentIcons.last());
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

template<typename T>
GeneratorIcon * BehaviorGraphView::addIconToGraph(const HkxObjectExpSharedPtr & obj, const T & type, GeneratorIcon * parentIcon){
    if (!parentIcon){
        return NULL;
    }
    if (!type){
        return NULL;
    }
    GeneratorIcon *icon = new GeneratorIcon(obj, type->name, parentIcon);
    //icon->setFlag(QGraphicsItem::ItemIsMovable);
    behaviorGS->addItem(icon->linkToParent);
    behaviorGS->addItem(icon);
    return icon;
}

template<typename T>
GeneratorIcon * BehaviorGraphView::initalizeInjectedIcon(const HkxObjectExpSharedPtr & obj, const T & type, GeneratorIcon * parentIcon, GeneratorIcon * selected){
    if (!parentIcon){
        return NULL;
    }
    GeneratorIcon *icon = new GeneratorIcon(obj, type->name, parentIcon);
    parentIcon->replaceChild(selected, icon);
    //Order is important??
    behaviorGS->addItem(icon->linkToParent);
    behaviorGS->addItem(icon);
    return icon;
}

template<typename T>
GeneratorIcon * BehaviorGraphView::initalizeAppendedIcon(const HkxObjectExpSharedPtr & obj, const T & type, GeneratorIcon * parentIcon){
    if (!parentIcon){
        return NULL;
    }
    GeneratorIcon *icon = new GeneratorIcon(obj, type->getName(), parentIcon);
    //Order is important??
    behaviorGS->addItem(icon->linkToParent);
    behaviorGS->addItem(icon);
    return icon;
}

bool BehaviorGraphView::drawBehaviorGraph(){
    QList <HkxObjectExpSharedPtr> objects;
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
            //ptr->icons.append(icon);
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
