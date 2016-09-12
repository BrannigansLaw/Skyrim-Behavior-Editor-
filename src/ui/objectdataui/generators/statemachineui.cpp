#include "StateMachineUI.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/hkbstringeventpayload.h"
#include "src/hkxclasses/hkbstatemachinetransitioninfoarray.h"
#include "src/hkxclasses/generators/hkbstatemachine.h"
#include "src/hkxclasses/generators/hkbstatemachinestateinfo.h"
#include "src/ui/genericdatawidgets.h"
#include "src/filetypes/hkxfile.h"
#include "src/ui/behaviorgraphui/behaviorgraphui.h"
#include "src/ui/behaviorgraphui/behaviorgraphicons.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QTableWidget>
#include <QSpinBox>
#include <QLineEdit>
#include <QHeaderView>
#include <QSignalMapper>

/*
 * StateMachineUI
 */

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
      lyt(new QVBoxLayout),
      table(new QTableWidget),
      addStatePB(new QPushButton("Add State")),
      removeStatePB(new QPushButton("Remove Selected State")),
      typeStateCB(new QComboBox),
      stateSignalMapper(new QSignalMapper),
      stateButtonLyt(new QHBoxLayout),
      addTransitionPB(new QPushButton("Add Transition")),
      removeTransitionPB(new QPushButton("Remove Selected Transition")),
      typeTransitionCB(new QComboBox),
      transitionSignalMapper(new QSignalMapper),
      transitionButtonLyt(new QHBoxLayout),
      name(new QLineEdit),
      id(new QComboBox),
      payload(new QLineEdit),
      startStateId(new QComboBox),
      startStateIdBind(new QComboBox),
      returnToPreviousStateEventId(new QComboBox),
      randomTransitionEventId(new QComboBox),
      transitionToNextHigherStateEventId(new QComboBox),
      transitionToNextLowerStateEventId(new QComboBox),
      syncVariableIndex(new QComboBox),
      maxSimultaneousTransitions(new QSpinBox),
      startStateMode(new QComboBox),
      selfTransitionMode(new QComboBox),
      states(new QTableWidget),
      wildcardTransitions(new QTableWidget)
{
    setTitle("hkbStateMachine");

    stateButtonLyt->addWidget(addStatePB, 1);
    stateButtonLyt->addWidget(typeStateCB, 2);
    stateButtonLyt->addWidget(removeStatePB, 1);

    transitionButtonLyt->addWidget(addTransitionPB, 1);
    transitionButtonLyt->addWidget(typeTransitionCB, 2);
    transitionButtonLyt->addWidget(removeTransitionPB, 1);

    table->setRowCount(12);
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

    table->setItem(9, 0, new QTableWidgetItem("maxSimultaneousTransitions"));
    table->setItem(9, 1, new QTableWidgetItem("Int"));
    table->setItem(9, 2, new QTableWidgetItem("N/A"));
    table->setCellWidget(9, 3, maxSimultaneousTransitions);

    table->setItem(10, 0, new QTableWidgetItem("startStateMode"));
    table->setItem(10, 1, new QTableWidgetItem("StartStateMode"));
    table->setItem(10, 2, new QTableWidgetItem("N/A"));
    table->setCellWidget(10, 3, startStateMode);

    table->setItem(11, 0, new QTableWidgetItem("selfTransitionMode"));
    table->setItem(11, 1, new QTableWidgetItem("SelfTransitionMode"));
    table->setItem(11, 2, new QTableWidgetItem("N/A"));
    table->setCellWidget(11, 3, selfTransitionMode);

    table->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);

    states->setColumnCount(3);
    states->setHorizontalHeaderLabels(headerLabels2);
    states->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    states->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    states->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    states->setSelectionBehavior(QAbstractItemView::SelectRows);

    wildcardTransitions->setColumnCount(3);
    wildcardTransitions->setHorizontalHeaderLabels(headerLabels2);
    wildcardTransitions->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    wildcardTransitions->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    wildcardTransitions->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    wildcardTransitions->setSelectionBehavior(QAbstractItemView::SelectRows);

    typeStateCB->insertItem(0, "hkbStateMachineStateInfo");
    typeTransitionCB->insertItem(0, "hkbStateMachineTransitionInfoArray");

    lyt->addWidget(table);
    lyt->addLayout(stateButtonLyt);
    lyt->addWidget(states);
    lyt->addLayout(transitionButtonLyt);
    lyt->addWidget(wildcardTransitions);
    setLayout(lyt);
    /*connect(generators, SIGNAL(itemAdded(int)), this, SLOT(addNewGenerator(int)));
    connect(generators, SIGNAL(existingItemAdded(int)), this, SLOT(addExistingGenerator(int)));
    connect(generators, SIGNAL(itemRemoved(int)), this, SLOT(removeGenerator(int)));
    connect(selectedGeneratorIndex, SIGNAL(editingDone()), this, SLOT(setSelectedGeneratorIndex()));
    connect(currentGeneratorIndex, SIGNAL(editingDone()), this, SLOT(setCurrentGeneratorIndex()));*/
}

void StateMachineUI::setEventToSendWhenStateOrTransitionChanges(){
    //
}

void StateMachineUI::setPayload(){
    //
}

void StateMachineUI::setReturnToPreviousStateEventId(){
    //
}

void StateMachineUI::setRandomTransitionEventId(){
    //
}

void StateMachineUI::setTransitionToNextHigherStateEventId(){
    //
}

void StateMachineUI::setTransitionToNextLowerStateEventId(){
    //
}

void StateMachineUI::setSyncVariableIndex(){
    //
}

void StateMachineUI::setWrapAroundStateId(){
    //
}

void StateMachineUI::setMaxSimultaneousTransitions(){
    //
}

void StateMachineUI::setStartStateMode(){
    //
}

void StateMachineUI::setSelfTransitionMode(){
    //
}

void StateMachineUI::addNewState(){
    //
}

void StateMachineUI::removeState(int index){
    //
}

void StateMachineUI::addNewTransition(){
    //
}

void StateMachineUI::removeTransition(int index){
    //
}


void StateMachineUI::setName(){
    if (bsData){
        bsData->name = name->text();
    }
}

void StateMachineUI::loadData(HkxObject *data){
    if (data && data->getSignature() == HKB_STATE_MACHINE){
        bsData = static_cast<hkbStateMachine *>(data);
        hkbStateMachineStateInfo *gen;
        hkbStateMachineTransitionInfoArray *trans;
        hkbStateMachineTransitionInfoArray::HkTransition *trs;
        QStringList varList = bsData->getParentFile()->getVariableNames();
        varList.prepend("None");
        QStringList eventList = bsData->getParentFile()->getEventNames();
        eventList.prepend("None");
        startStateIdBind->clear();
        startStateIdBind->insertItems(0, varList);
        name->setText(bsData->name);
        id->clear();
        id->insertItems(0, eventList);
        id->setCurrentIndex(bsData->id + 1);
        payload->clear();
        if (bsData->payload.data()){
            payload->setText(static_cast<hkbStringEventPayload *>(bsData->payload.data())->data);
        }
        startStateId->clear();
        startStateIdBind->clear();
        startStateIdBind->insertItems(0, varList);
        startStateIdBind->setCurrentIndex(/*Get variable binding here*/0);
        returnToPreviousStateEventId->clear();
        returnToPreviousStateEventId->insertItems(0, eventList);
        returnToPreviousStateEventId->setCurrentIndex(bsData->returnToPreviousStateEventId + 1);
        randomTransitionEventId->clear();
        randomTransitionEventId->insertItems(0, eventList);
        randomTransitionEventId->setCurrentIndex(bsData->randomTransitionEventId + 1);
        transitionToNextHigherStateEventId->clear();
        transitionToNextHigherStateEventId->insertItems(0, eventList);
        transitionToNextHigherStateEventId->setCurrentIndex(bsData->transitionToNextHigherStateEventId + 1);
        transitionToNextLowerStateEventId->clear();
        transitionToNextLowerStateEventId->insertItems(0, eventList);
        transitionToNextLowerStateEventId->setCurrentIndex(bsData->transitionToNextLowerStateEventId + 1);
        syncVariableIndex->clear();
        syncVariableIndex->insertItems(0, varList);
        syncVariableIndex->setCurrentIndex(bsData->syncVariableIndex + 1);
        maxSimultaneousTransitions->setValue(bsData->maxSimultaneousTransitions);
        startStateMode->clear();
        startStateMode->insertItems(0, bsData->StartStateMode);
        startStateMode->setCurrentText(bsData->startStateMode);
        selfTransitionMode->clear();
        selfTransitionMode->insertItems(0, bsData->SelfTransitionMode);
        selfTransitionMode->setCurrentText(bsData->selfTransitionMode);
        states->setRowCount(0);
        wildcardTransitions->setRowCount(0);
        for (int i = 0; i < bsData->states.size(); i++){
            gen = static_cast<hkbStateMachineStateInfo *>(bsData->states.at(i).data());
            if (bsData->states.at(i).data()){
                states->setRowCount(states->rowCount() + 1);
                states->setItem(i, 0, new QTableWidgetItem(gen->getName()));
                states->setItem(i, 1, new QTableWidgetItem(gen->getClassname()));
                states->setItem(i, 2, new QTableWidgetItem("Edit"));
                startStateId->insertItem(i, gen->getName());
            }
        }
        startStateId->insertItem(0, "None");
        trans = static_cast<hkbStateMachineTransitionInfoArray *>(bsData->wildcardTransitions.data());
        if (trans){
            for (int i = 0; i < trans->getNumTransitions(); i++){
                wildcardTransitions->setRowCount(wildcardTransitions->rowCount() + 1);
                wildcardTransitions->setItem(i, 0, new QTableWidgetItem(trans->getTransitionNameAt(i)));
                wildcardTransitions->setItem(i, 1, new QTableWidgetItem(trans->getClassname()));
                wildcardTransitions->setItem(i, 2, new QTableWidgetItem("Edit"));
            }
        }
    }
}

/*void StateMachineUI::removeState(int index){
    if (!behaviorView || bsData->states.size() <= index || index < 0){
        return;
    }
    GeneratorIcon *tempIcon = behaviorView->selectedIcon->getChildIcon(bsData->states.at(index).data());
    if (bsData->states.count(bsData->states.at(index)) == 1){
        behaviorView->removeSelectedObjectBranch(tempIcon);
    }else{
        bsData->states.removeAt(index);
    }
    behaviorView->behavior->removeData();
    //loadData(bsData);
}*/
