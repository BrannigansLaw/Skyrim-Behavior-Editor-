#include "ManualSelectorGeneratorUI.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/generators/hkbmanualselectorgenerator.h"
#include "src/ui/genericdatawidgets.h"
#include "src/filetypes/hkxfile.h"
#include "src/ui/behaviorgraphui/behaviorgraphui.h"
#include "src/ui/behaviorgraphui/behaviorgraphicons.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>

/*
 * ManualSelectorGeneratorUI
 */

QStringList ManualSelectorGeneratorUI::list = {
    "hkbStateMachine",
    "hkbManualSelectorGenerator",
    "hkbBlenderGenerator",
    "BSiStateTaggingGenerator",
    "BSBoneSwitchGenerator",
    "BSCyclicBlendTransitionGenerator",
    "BSSynchronizedClipGenerator",
    "hkbModifierGenerator",
    "BSOffsetAnimationGenerator",
    "hkbPoseMatchingGenerator",
    "hkbClipGenerator",
    "hkbBehaviorReferenceGenerator"
};

ManualSelectorGeneratorUI::ManualSelectorGeneratorUI()
    : behaviorView(NULL),
      lyt(new QVBoxLayout),
      name(new StringWidget("Name")),
      generators(new GenericTabelWidget("Generators")),
      selectedGeneratorIndex(new IntWidget("SelectedGeneratorIndex")),
      currentGeneratorIndex(new IntWidget("CurrentGeneratorIndex"))
{
    setTitle("hkbManualSelectorGenerator");
    generators->setTypes(list);
    lyt->addWidget(name);
    lyt->addWidget(selectedGeneratorIndex);
    lyt->addWidget(currentGeneratorIndex);
    lyt->addWidget(generators);
    setLayout(lyt);
    connect(generators, SIGNAL(itemAdded(int)), this, SLOT(addNewGenerator(int)));
    connect(generators, SIGNAL(existingItemAdded(int)), this, SLOT(addExistingGenerator(int)));
    connect(generators, SIGNAL(itemRemoved(int)), this, SLOT(removeGenerator(int)));
    connect(selectedGeneratorIndex, SIGNAL(editingDone()), this, SLOT(setSelectedGeneratorIndex()));
    connect(currentGeneratorIndex, SIGNAL(editingDone()), this, SLOT(setCurrentGeneratorIndex()));
}

void ManualSelectorGeneratorUI::setName(){
    if (bsData){
        bsData->name = name->getText();
    }
}

void ManualSelectorGeneratorUI::loadData(HkxObject *data){
    if (data && data->getSignature() == HKB_MANUAL_SELECTOR_GENERATOR){
        hkbGenerator *gen;
        bsData = static_cast<hkbManualSelectorGenerator *>(data);
        name->setText(bsData->name);
        generators->clear();
        QStringList list = bsData->getParentFile()->getGeneratorNames();
        list.prepend("None");
        generators->setObjectList(list);
        for (int i = 0; i < bsData->generators.size(); i++){
            gen = static_cast<hkbGenerator *>(bsData->generators.at(i).data());
            if (bsData->generators.at(i).data()){
                generators->addItem(gen->getName(), gen->getClassname());
            }
        }
        selectedGeneratorIndex->setValue(bsData->selectedGeneratorIndex);
        selectedGeneratorIndex->setBoundVariableList(bsData->getParentFile()->getVariableNames());
        currentGeneratorIndex->setValue(bsData->currentGeneratorIndex);
        currentGeneratorIndex->setBoundVariableList(bsData->getParentFile()->getVariableNames());
    }
}

void ManualSelectorGeneratorUI::setSelectedGeneratorIndex(){
    if (bsData){
        bsData->selectedGeneratorIndex = selectedGeneratorIndex->getValue();
    }
}

void ManualSelectorGeneratorUI::setCurrentGeneratorIndex(){
    if (bsData){
        bsData->currentGeneratorIndex = currentGeneratorIndex->getValue();
    }
}

void ManualSelectorGeneratorUI::addExistingGenerator(int index){
    if (behaviorView){
        HkxObject *ptr = bsData->getParentFile()->getGeneratorDataAt(index - 1);
        if (!behaviorView->selectedIcon->getChildIcon(ptr)){
            if (index < 1 || !ptr || ptr == bsData || !behaviorView->reconnectBranch(NULL, ptr, behaviorView->getSelectedItem())){
                QMessageBox msg;
                msg.setText("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to create a circular branch or dead end!!!");
                msg.exec();
                loadData(bsData);
                return;
            }
        }
        bsData->generators.append(HkxObjectExpSharedPtr(ptr));
    }
}

void ManualSelectorGeneratorUI::addNewGenerator(int index){
    if (!behaviorView){
        return;
    }
    Type type = static_cast<Type>(index);
    switch (type){
    case STATE_MACHINE:
        behaviorView->appendStateMachine();
        break;
    case MANUAL_SELECTOR_GENERATOR:
        behaviorView->appendManualSelectorGenerator();
        break;
    case BLENDER_GENERATOR:
        behaviorView->appendBlenderGenerator();
        break;
    case I_STATE_TAGGING_GENERATOR:
        behaviorView->appendIStateTaggingGenerator();
        break;
    case BONE_SWITCH_GENERATOR:
        behaviorView->appendBoneSwitchGenerator();
        break;
    case CYCLIC_BLEND_TRANSITION_GENERATOR:
        behaviorView->appendCyclicBlendTransitionGenerator();
        break;
    case SYNCHRONIZED_CLIP_GENERATOR:
        behaviorView->appendSynchronizedClipGenerator();
        break;
    case MODIFIER_GENERATOR:
        behaviorView->appendModifierGenerator();
        break;
    case OFFSET_ANIMATION_GENERATOR:
        behaviorView->appendOffsetAnimationGenerator();
        break;
    case POSE_MATCHING_GENERATOR:
        behaviorView->appendPoseMatchingGenerator();
        break;
    case CLIP_GENERATOR:
        behaviorView->appendClipGenerator();
        break;
    case BEHAVIOR_REFERENCE_GENERATOR:
        behaviorView->appendBehaviorReferenceGenerator();
        break;
    default:
        return;
    }
}

void ManualSelectorGeneratorUI::removeGenerator(int index){
    if (!behaviorView || bsData->generators.size() <= index || index < 0){
        return;
    }
    GeneratorIcon *tempIcon = behaviorView->selectedIcon->getChildIcon(bsData->generators.at(index).data());
    if (bsData->generators.count(bsData->generators.at(index)) == 1){
        behaviorView->removeSelectedObjectBranch(tempIcon);
    }else{
        bsData->generators.removeAt(index);
    }
    behaviorView->behavior->removeData();
    loadData(bsData);
}
