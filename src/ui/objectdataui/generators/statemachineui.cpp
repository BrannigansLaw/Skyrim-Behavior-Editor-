#include "StateMachineUI.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/hkbstringeventpayload.h"
#include "src/hkxclasses/hkbstatemachinetransitioninfoarray.h"
#include "src/hkxclasses/generators/hkbstatemachine.h"
#include "src/hkxclasses/generators/hkbstatemachinestateinfo.h"
#include "src/ui/genericdatawidgets.h"
#include "src/ui/transitionsui.h"
#include "src/ui/objectdataui/generators/stateui.h"
#include "src/filetypes/hkxfile.h"
#include "src/ui/behaviorgraphui/behaviorgraphui.h"
#include "src/ui/behaviorgraphui/behaviorgraphicons.h"
#include "src/hkxclasses/hkbvariablebindingset.h"
#include "src/ui/transitionsui.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QTableWidget>
#include <QSpinBox>
#include <QLineEdit>
#include <QHeaderView>
#include <QSignalMapper>
#include <QStackedLayout>
#include <QCheckBox>

/*
 * StateMachineUI
 */

QStringList StateMachineUI::types = {
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

QStringList StateMachineUI::headerLabels1 = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

QStringList StateMachineUI::headerLabels2 = {
    "Name",
    "Type",
    "Value"
};

StateMachineUI::StateMachineUI()
    : behaviorView(NULL),
      bsData(NULL),
      lyt(new QVBoxLayout),
      table(new TableWidget),
      addStatePB(new QPushButton("Add State With Generator")),
      removeStatePB(new QPushButton("Remove Selected State")),
      typeSelectorCB(new ComboBox),
      stateSignalMapper(new QSignalMapper),
      stateButtonLyt(new QHBoxLayout),
      addTransitionPB(new QPushButton("Add Transition")),
      removeTransitionPB(new QPushButton("Remove Selected Transition")),
      typeTransitionCB(new ComboBox),
      transitionSignalMapper(new QSignalMapper),
      transitionButtonLyt(new QHBoxLayout),
      name(new QLineEdit),
      id(new ComboBox),
      payload(new QLineEdit),
      startStateId(new ComboBox),
      startStateIdBind(new ComboBox),
      returnToPreviousStateEventId(new ComboBox),
      randomTransitionEventId(new ComboBox),
      transitionToNextHigherStateEventId(new ComboBox),
      transitionToNextLowerStateEventId(new ComboBox),
      syncVariableIndex(new ComboBox),
      wrapAroundStateId(new QCheckBox),
      maxSimultaneousTransitions(new SpinBox),
      startStateMode(new ComboBox),
      selfTransitionMode(new ComboBox),
      states(new TableWidget),
      wildcardTransitions(new TableWidget)
{
    setTitle("hkbStateMachine");

    stateButtonLyt->addWidget(addStatePB, 1);
    stateButtonLyt->addWidget(typeSelectorCB, 2);
    stateButtonLyt->addWidget(removeStatePB, 1);

    transitionButtonLyt->addWidget(addTransitionPB, 1);
    transitionButtonLyt->addWidget(typeTransitionCB, 2);
    transitionButtonLyt->addWidget(removeTransitionPB, 1);

    table->setRowCount(13);
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels(headerLabels1);

    table->setCellWidget(0, 0, name);
    table->setItem(0, 1, new QTableWidgetItem("String"));
    table->setItem(0, 2, new QTableWidgetItem("N/A"));
    table->setItem(0, 3, new QTableWidgetItem("N/A"));

    table->setItem(1, 0, new QTableWidgetItem("eventToSendWhenStateOrTransitionChanges"));
    table->setItem(1, 1, new QTableWidgetItem("hkEvent"));
    table->setItem(1, 2, new QTableWidgetItem("N/A"));
    table->setCellWidget(1, 3, id);

    table->setItem(2, 0, new QTableWidgetItem("payload"));
    table->setItem(2, 1, new QTableWidgetItem("hkbStringEventPayload"));
    table->setItem(2, 2, new QTableWidgetItem("N/A"));
    table->setCellWidget(2, 3, payload);

    table->setItem(3, 0, new QTableWidgetItem("startStateId"));
    table->setItem(3, 1, new QTableWidgetItem("Int"));
    table->setCellWidget(3, 2, startStateIdBind);
    table->setCellWidget(3, 3, startStateId);

    table->setItem(4, 0, new QTableWidgetItem("returnToPreviousStateEventId"));
    table->setItem(4, 1, new QTableWidgetItem("hkEvent"));
    table->setItem(4, 2, new QTableWidgetItem("N/A"));
    table->setCellWidget(4, 3, returnToPreviousStateEventId);

    table->setItem(5, 0, new QTableWidgetItem("randomTransitionEventId"));
    table->setItem(5, 1, new QTableWidgetItem("hkEvent"));
    table->setItem(5, 2, new QTableWidgetItem("N/A"));
    table->setCellWidget(5, 3, randomTransitionEventId);

    table->setItem(6, 0, new QTableWidgetItem("transitionToNextHigherStateEventId"));
    table->setItem(6, 1, new QTableWidgetItem("hkEvent"));
    table->setItem(6, 2, new QTableWidgetItem("N/A"));
    table->setCellWidget(6, 3, transitionToNextHigherStateEventId);

    table->setItem(7, 0, new QTableWidgetItem("transitionToNextLowerStateEventId"));
    table->setItem(7, 1, new QTableWidgetItem("hkEvent"));
    table->setItem(7, 2, new QTableWidgetItem("N/A"));
    table->setCellWidget(7, 3, transitionToNextLowerStateEventId);

    table->setItem(8, 0, new QTableWidgetItem("syncVariableIndex"));
    table->setItem(8, 1, new QTableWidgetItem("hkEvent"));
    table->setItem(8, 2, new QTableWidgetItem("N/A"));
    table->setCellWidget(8, 3, syncVariableIndex);

    table->setItem(9, 0, new QTableWidgetItem("wrapAroundStateId"));
    table->setItem(9, 1, new QTableWidgetItem("Bool"));
    table->setItem(9, 2, new QTableWidgetItem("N/A"));
    table->setCellWidget(9, 3, wrapAroundStateId);

    table->setItem(10, 0, new QTableWidgetItem("maxSimultaneousTransitions"));
    table->setItem(10, 1, new QTableWidgetItem("Int"));
    table->setItem(10, 2, new QTableWidgetItem("N/A"));
    table->setCellWidget(10, 3, maxSimultaneousTransitions);

    table->setItem(11, 0, new QTableWidgetItem("startStateMode"));
    table->setItem(11, 1, new QTableWidgetItem("StartStateMode"));
    table->setItem(11, 2, new QTableWidgetItem("N/A"));
    table->setCellWidget(11, 3, startStateMode);

    table->setItem(12, 0, new QTableWidgetItem("selfTransitionMode"));
    table->setItem(12, 1, new QTableWidgetItem("SelfTransitionMode"));
    table->setItem(12, 2, new QTableWidgetItem("N/A"));
    table->setCellWidget(12, 3, selfTransitionMode);

    states->setColumnCount(3);
    states->setHorizontalHeaderLabels(headerLabels2);

    wildcardTransitions->setColumnCount(3);
    wildcardTransitions->setHorizontalHeaderLabels(headerLabels2);

    maxSimultaneousTransitions->setMaximum(32);
    maxSimultaneousTransitions->setMinimum(0);

    typeSelectorCB->insertItems(0, types);
    typeTransitionCB->insertItem(0, "hkbStateMachineTransitionInfoArray");

    startStateMode->insertItems(0, bsData->StartStateMode);
    selfTransitionMode->insertItems(0, bsData->SelfTransitionMode);
    lyt->addWidget(table);
    lyt->addLayout(stateButtonLyt);
    lyt->addWidget(states);
    lyt->addLayout(transitionButtonLyt);
    lyt->addWidget(wildcardTransitions);
    setLayout(lyt);
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    connect(id, SIGNAL(activated(int)), this, SLOT(setEventToSendWhenStateOrTransitionChanges(int)));
    connect(payload, SIGNAL(editingFinished()), this, SLOT(setPayload()));
    connect(startStateId, SIGNAL(activated(int)), this, SLOT(setStartStateId(int)));
    connect(startStateIdBind, SIGNAL(activated(int)), this, SLOT(setStartStateIdBind(int)));
    connect(returnToPreviousStateEventId, SIGNAL(activated(int)), this, SLOT(setReturnToPreviousStateEventId(int)));
    connect(randomTransitionEventId, SIGNAL(activated(int)), this, SLOT(setRandomTransitionEventId(int)));
    connect(transitionToNextHigherStateEventId, SIGNAL(activated(int)), this, SLOT(setTransitionToNextHigherStateEventId(int)));
    connect(transitionToNextLowerStateEventId, SIGNAL(activated(int)), this, SLOT(setTransitionToNextLowerStateEventId(int)));
    connect(syncVariableIndex, SIGNAL(activated(int)), this, SLOT(setSyncVariableIndex(int)));
    connect(wrapAroundStateId, SIGNAL(clicked(bool)), this, SLOT(setWrapAroundStateId(bool)));
    connect(maxSimultaneousTransitions, SIGNAL(editingFinished()), this, SLOT(setMaxSimultaneousTransitions()));
    connect(startStateMode, SIGNAL(activated(int)), this, SLOT(setStartStateMode(int)));
    connect(selfTransitionMode, SIGNAL(activated(int)), this, SLOT(setSelfTransitionMode(int)));
    connect(states, SIGNAL(cellClicked(int,int)), this, SLOT(stateSelected(int,int)));
    connect(wildcardTransitions, SIGNAL(cellClicked(int,int)), this, SLOT(transitionSelected(int,int)));
    connect(addStatePB, SIGNAL(released()), this, SLOT(addNewStateWithGenerator()));
    connect(removeStatePB, SIGNAL(released()), this, SLOT(removeStateWithGenerator()));
    connect(addTransitionPB, SIGNAL(released()), this, SLOT(addNewTransition()));
    connect(removeTransitionPB, SIGNAL(released()), this, SLOT(removeTransition()));
}

void StateMachineUI::addEventToLists(const QString & name){
    id->insertItem(id->count(), name);
    returnToPreviousStateEventId->insertItem(returnToPreviousStateEventId->count(), name);
    randomTransitionEventId->insertItem(randomTransitionEventId->count(), name);
    transitionToNextHigherStateEventId->insertItem(transitionToNextHigherStateEventId->count(), name);
    transitionToNextLowerStateEventId->insertItem(transitionToNextLowerStateEventId->count(), name);
}

void StateMachineUI::removeEventFromLists(int index){
    index++;
    if (id->currentIndex() == index){
        id->setCurrentIndex(0);
    }
    id->removeItem(index);
    if (returnToPreviousStateEventId->currentIndex() == index){
        returnToPreviousStateEventId->setCurrentIndex(0);
    }
    returnToPreviousStateEventId->removeItem(index);
    if (randomTransitionEventId->currentIndex() == index){
        randomTransitionEventId->setCurrentIndex(0);
    }
    randomTransitionEventId->removeItem(index);
    if (transitionToNextHigherStateEventId->currentIndex() == index){
        transitionToNextHigherStateEventId->setCurrentIndex(0);
    }
    transitionToNextHigherStateEventId->removeItem(index);
    if (transitionToNextLowerStateEventId->currentIndex() == index){
        transitionToNextLowerStateEventId->setCurrentIndex(0);
    }
    transitionToNextLowerStateEventId->removeItem(index);
}

void StateMachineUI::renameEventInLists(const QString & newName, int index){
    index++;
    id->setItemText(index, newName);
    returnToPreviousStateEventId->setItemText(index, newName);
    randomTransitionEventId->setItemText(index, newName);
    transitionToNextHigherStateEventId->setItemText(index, newName);
    transitionToNextLowerStateEventId->setItemText(index, newName);
}

void StateMachineUI::addVariableToLists(const QString & name){
    startStateIdBind->insertItem(startStateIdBind->count(), name);
}

void StateMachineUI::removeVariableFromLists(int index){
    index++;
    if (startStateIdBind->currentIndex() == index){
        startStateIdBind->setCurrentIndex(0);
    }
    startStateIdBind->removeItem(index);
    if (syncVariableIndex->currentIndex() == index){
        syncVariableIndex->setCurrentIndex(0);
    }
    syncVariableIndex->removeItem(index);
}

void StateMachineUI::renameVariableInLists(const QString & newName, int index){
    index++;
    startStateIdBind->setItemText(index, newName);
}

void StateMachineUI::setEventToSendWhenStateOrTransitionChanges(int index){
    if (bsData){
        bsData->id = index - 1;
    }
}

void StateMachineUI::setPayload(){
    if (bsData){
        bsData->payload = HkxObjectExpSharedPtr(new hkbStringEventPayload(bsData->getParentFile(), payload->text()));
    }
}

void StateMachineUI::setReturnToPreviousStateEventId(int index){
    if (bsData){
        bsData->returnToPreviousStateEventId = index - 1;
    }
}

void StateMachineUI::setStartStateId(int index){
    if (bsData && index < bsData->states.size()&& index > -1){
        bsData->startStateId = static_cast<hkbStateMachineStateInfo *>(bsData->states.at(index).data())->stateId;
    }
}

void StateMachineUI::setStartStateIdBind(int index){
    if (bsData){
        hkbVariableBindingSet *varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (behaviorView->behavior->getVariableTypeAt(index - 1) == VARIABLE_TYPE_INT32){
            if (!varBind){
                varBind = new hkbVariableBindingSet(behaviorView->behavior);
                bsData->variableBindingSet = HkxObjectExpSharedPtr(varBind);
                behaviorView->behavior->addObjectToFile(varBind);
            }
            varBind->addBinding("startStateId", index - 1);
        }else{
            QMessageBox msg;
            msg.setText("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!");
            msg.exec();
            if (varBind){
                int ind = varBind->getVariableIndexOfBinding("startStateId");
                if (ind > -1 && ind < startStateIdBind->count()){
                    startStateIdBind->setCurrentIndex(ind + 1);
                }
            }
        }
    }
}

void StateMachineUI::setRandomTransitionEventId(int index){
    if (bsData){
        bsData->randomTransitionEventId = index - 1;
    }
}

void StateMachineUI::setTransitionToNextHigherStateEventId(int index){
    if (bsData){
        bsData->transitionToNextHigherStateEventId = index - 1;
    }
}

void StateMachineUI::setTransitionToNextLowerStateEventId(int index){
    if (bsData){
        bsData->transitionToNextLowerStateEventId = index - 1;
    }
}

void StateMachineUI::setSyncVariableIndex(int index){
    if (bsData){
        bsData->syncVariableIndex = index - 1;
    }
}

void StateMachineUI::setWrapAroundStateId(bool checked){
    if (bsData){
        bsData->wrapAroundStateId = checked;
    }
}

void StateMachineUI::setMaxSimultaneousTransitions(){
    if (bsData){
        bsData->maxSimultaneousTransitions = maxSimultaneousTransitions->value();
    }
}

void StateMachineUI::setStartStateMode(int index){
    if (bsData){
        bsData->startStateMode = bsData->StartStateMode.at(index);
    }
}

void StateMachineUI::setSelfTransitionMode(int index){
    if (bsData){
        bsData->selfTransitionMode = bsData->SelfTransitionMode.at(index);
    }
}

void StateMachineUI::stateSelected(int row, int column){
    if (column == 2 && bsData && row < bsData->states.size()){
        emit viewState(static_cast<hkbStateMachineStateInfo *>(bsData->states.at(row).data()));
    }
}

void StateMachineUI::transitionSelected(int row, int column){
    hkbStateMachineTransitionInfoArray *trans = static_cast<hkbStateMachineTransitionInfoArray *>(bsData->wildcardTransitions.data());
    if (column == 2 && bsData && row < trans->transitions.size()){
        emit viewTransition(bsData, (HkTransition *)&trans->transitions[row]);
    }
}

void StateMachineUI::addNewStateWithGenerator(){
    if (behaviorView && bsData){
        ComboBox *genSelector = new ComboBox;
        QStringList genList = bsData->getParentFile()->getGeneratorNames();
        genList.prepend("None");
        genSelector->insertItems(0, genList);
        Type typeEnum = static_cast<Type>(typeSelectorCB->currentIndex());
        switch (typeEnum){
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
            delete genSelector;
            return;
        }
        if (!bsData->states.isEmpty() && bsData->states.last().data()){
            states->setRowCount(states->rowCount() + 1);
            states->setItem(states->rowCount(), 0, new QTableWidgetItem(static_cast<hkbGenerator *>(bsData->states.last().data())->getName()));
            states->setItem(states->rowCount(), 1, new QTableWidgetItem(static_cast<hkbGenerator *>(bsData->states.last().data())->getClassname()));
            states->setItem(states->rowCount(), 2, new QTableWidgetItem("Click To Edit"));
        }
    }
}

void StateMachineUI::removeStateWithGenerator(){
    if (bsData){
        int index = states->currentRow();
        if (!behaviorView || bsData->states.size() <= index || index < 0){
            return;
        }
        GeneratorIcon *tempIcon = behaviorView->selectedIcon->getChildIcon(bsData->states.at(index).data());
        if (bsData->states.count(bsData->states.at(index)) == 1){
            behaviorView->removeSelectedObjectBranch(tempIcon, NULL, false);
        }else{
            bsData->removeState(index);
        }
        states->removeRow(index);
        behaviorView->removeGeneratorData();
    }
}

void StateMachineUI::addNewTransition(){
    if (bsData){
        hkbStateMachineTransitionInfoArray *transitions = static_cast<hkbStateMachineTransitionInfoArray *>(bsData->wildcardTransitions.data());
        int i;
        if (transitions){
            transitions->addTransition();
        }else{
            transitions = new hkbStateMachineTransitionInfoArray(bsData->getParentFile(), bsData);
            bsData->wildcardTransitions = HkxObjectExpSharedPtr(transitions);
        }
        i = wildcardTransitions->rowCount();
        wildcardTransitions->setRowCount(wildcardTransitions->rowCount() + 1);
        wildcardTransitions->setItem(i, 0, new QTableWidgetItem("New_Transition"));
        wildcardTransitions->setItem(i, 1, new QTableWidgetItem(transitions->getClassname()));
        wildcardTransitions->setItem(i, 2, new QTableWidgetItem("Edit"));
    }
}

void StateMachineUI::removeTransition(){
    if (bsData){
        hkbStateMachineTransitionInfoArray *transitions = static_cast<hkbStateMachineTransitionInfoArray *>(bsData->wildcardTransitions.data());
        int i = wildcardTransitions->currentRow();
        if (transitions){
            transitions->removeTransition(i);
        }
        wildcardTransitions->removeRow(i);
    }
}


void StateMachineUI::setName(){
    if (bsData){
        bsData->name = name->text();
        bsData->updateIconNames();
        emit generatorNameChanged(name->text(), bsData->getParentFile()->getIndexOfGenerator(bsData) + 1);
    }
}

void StateMachineUI::loadComboBoxes(){
    QStringList varList = behaviorView->behavior->getVariableNames();
    varList.prepend("None");
    QStringList eventList = behaviorView->behavior->getEventNames();
    eventList.prepend("None");
    id->insertItems(0, eventList);
    startStateIdBind->insertItems(0, varList);
    returnToPreviousStateEventId->insertItems(0, eventList);
    randomTransitionEventId->insertItems(0, eventList);
    transitionToNextHigherStateEventId->insertItems(0, eventList);
    transitionToNextLowerStateEventId->insertItems(0, eventList);
    syncVariableIndex->insertItems(0, varList);
}

void StateMachineUI::loadData(HkxObject *data){
    if (data && data->getSignature() == HKB_STATE_MACHINE){
        bsData = static_cast<hkbStateMachine *>(data);
        hkbStateMachineStateInfo *gen;
        hkbStateMachineTransitionInfoArray *trans;
        QStringList varList = bsData->getParentFile()->getVariableNames();
        varList.prepend("None");
        QStringList eventList = bsData->getParentFile()->getEventNames();
        eventList.prepend("None");
        name->setText(bsData->name);
        id->setCurrentIndex(bsData->id + 1);
        payload->clear();
        if (bsData->payload.data()){
            payload->setText(static_cast<hkbStringEventPayload *>(bsData->payload.data())->data);
        }
        startStateId->clear();
        startStateId->insertItems(0, bsData->getStateNames());
        startStateId->setCurrentIndex(startStateId->findText(bsData->getStateName(bsData->startStateId), Qt::MatchCaseSensitive));
        hkbVariableBindingSet *varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (varBind){
            int ind = varBind->getVariableIndexOfBinding("startStateId");
            if (ind > -1 && ind < startStateIdBind->count()){
                startStateIdBind->setCurrentIndex(ind + 1);
            }else{
                startStateIdBind->setCurrentIndex(0);
            }
        }else{
            startStateIdBind->setCurrentIndex(0);
        }
        returnToPreviousStateEventId->setCurrentIndex(bsData->returnToPreviousStateEventId + 1);
        randomTransitionEventId->setCurrentIndex(bsData->randomTransitionEventId + 1);
        transitionToNextHigherStateEventId->setCurrentIndex(bsData->transitionToNextHigherStateEventId + 1);
        transitionToNextLowerStateEventId->setCurrentIndex(bsData->transitionToNextLowerStateEventId + 1);
        syncVariableIndex->setCurrentIndex(bsData->syncVariableIndex + 1);
        maxSimultaneousTransitions->setValue(bsData->maxSimultaneousTransitions);
        startStateMode->setCurrentText(bsData->startStateMode);
        selfTransitionMode->setCurrentText(bsData->selfTransitionMode);
        for (int i = 0; i < bsData->states.size(); i++){
            gen = static_cast<hkbStateMachineStateInfo *>(bsData->states.at(i).data());
            if (bsData->states.at(i).data()){
                if (i >= states->rowCount()){
                    states->setRowCount(states->rowCount() + 1);
                    states->setItem(i, 0, new QTableWidgetItem(gen->getName()));
                    states->setItem(i, 1, new QTableWidgetItem(gen->getClassname()));
                    states->setItem(i, 2, new QTableWidgetItem("Edit"));
                }else{
                    states->setRowHidden(i, false);
                    states->item(i, 0)->setText(gen->getName());
                }
            }
        }
        for (int j = bsData->states.size(); j < states->rowCount(); j++){
            states->setRowHidden(j, true);
        }
        trans = static_cast<hkbStateMachineTransitionInfoArray *>(bsData->wildcardTransitions.data());
        if (trans){
            for (int i = 0; i < trans->getNumTransitions(); i++){
                if (i >= wildcardTransitions->rowCount()){
                    wildcardTransitions->setRowCount(wildcardTransitions->rowCount() + 1);
                    wildcardTransitions->setItem(i, 0, new QTableWidgetItem(trans->getTransitionNameAt(i)));
                    wildcardTransitions->setItem(i, 1, new QTableWidgetItem(trans->getClassname()));
                    wildcardTransitions->setItem(i, 2, new QTableWidgetItem("Edit"));
                }else{
                    wildcardTransitions->setRowHidden(i, false);
                    wildcardTransitions->item(i, 0)->setText(trans->getTransitionNameAt(i));
                }
            }
            for (int j = trans->getNumTransitions(); j < wildcardTransitions->rowCount(); j++){
                wildcardTransitions->setRowHidden(j, true);
            }
        }else{
            for (int j = 0; j < wildcardTransitions->rowCount(); j++){
                wildcardTransitions->setRowHidden(j, true);
            }
        }
    }
}

