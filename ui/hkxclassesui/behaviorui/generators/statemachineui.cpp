#include "StateMachineUI.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/hkbstringeventpayload.h"
#include "src/hkxclasses/behavior/hkbstatemachinetransitioninfoarray.h"
#include "src/hkxclasses/behavior/generators/hkbstatemachine.h"
#include "src/hkxclasses/behavior/generators/hkbstatemachinestateinfo.h"
#include "src/ui/genericdatawidgets.h"
#include "src/ui/hkxclassesui/behaviorui/transitionsui.h"
#include "src/ui/hkxclassesui/behaviorui/generators/stateui.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/behaviorgraphview.h"
#include "src/ui/treegraphicsitem.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/ui/hkxclassesui/behaviorui/transitionsui.h"

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

#define BASE_NUMBER_OF_ROWS 15

#define NAME_ROW 0
#define ID_ROW 1
#define PAYLOAD_ROW 2
#define START_STATE_ID_ROW 3
#define RETURN_TO_PREVIOUS_STATE_EVENT_ID_ROW 4
#define RANDOM_TRANSITION_EVENT_ID_ROW 5
#define TRANSITION_TO_NEXT_HIGHER_STATE_EVENT_ID_ROW 6
#define TRANSITION_TO_NEXT_LOWER_STATE_EVENT_ID_ROW 7
#define SYNC_VARIABLE_INDEX_ROW 8
#define WRAP_AROUND_STATE_ID_ROW 9
#define MAX_SIMULTANEOUS_TRANSITIONS_ROW 10
#define START_STATE_MODE_ROW 11
#define SELF_TRANSITION_MODE_ROW 12
#define ADD_CHILD_ROW 13

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

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

QStringList StateMachineUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

StateMachineUI::StateMachineUI()
    : rowIndexOfChildToRemove(-1),
      rowIndexOfTransitionButtonPanel(ADD_CHILD_ROW + 1),
      behaviorView(NULL),
      bsData(NULL),
      groupBox(new QGroupBox("hkbStateMachine")),
      topLyt(new QGridLayout),
      stateUI(new StateUI),
      transitionUI(new TransitionsUI),
      table(new TableWidget),
      addStatePB(new QPushButton("Add State With Generator")),
      removeStatePB(new QPushButton("Remove Selected State")),
      typeSelectorCB(new ComboBox),
      //addTransitionPB(new QPushButton("Add Transition")),
      //removeTransitionPB(new QPushButton("Remove Selected Transition")),
      name(new LineEdit),
      payload(new LineEdit),
      startStateId(new ComboBox),
      wrapAroundStateId(new QCheckBox),
      maxSimultaneousTransitions(new SpinBox),
      startStateMode(new ComboBox),
      selfTransitionMode(new ComboBox)
{
    typeSelectorCB->insertItems(0, types);
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(NAME_ROW, NAME_COLUMN, new QTableWidgetItem("name"));
    table->setItem(NAME_ROW, TYPE_COLUMN, new QTableWidgetItem("hkStringPtr"));
    table->setItem(NAME_ROW, BINDING_COLUMN, new QTableWidgetItem("N/A"));
    table->setCellWidget(NAME_ROW, VALUE_COLUMN, name);
    table->setItem(ID_ROW, NAME_COLUMN, new QTableWidgetItem("id"));
    table->setItem(ID_ROW, TYPE_COLUMN, new QTableWidgetItem("hkEvent"));
    table->setItem(ID_ROW, BINDING_COLUMN, new QTableWidgetItem("N/A"));
    table->setItem(ID_ROW, VALUE_COLUMN, new QTableWidgetItem("NONE"));
    table->setItem(PAYLOAD_ROW, NAME_COLUMN, new QTableWidgetItem("payload"));
    table->setItem(PAYLOAD_ROW, TYPE_COLUMN, new QTableWidgetItem("hkbEventPayload"));
    table->setItem(PAYLOAD_ROW, BINDING_COLUMN, new QTableWidgetItem("N/A"));
    table->setCellWidget(PAYLOAD_ROW, VALUE_COLUMN, payload);
    table->setItem(START_STATE_ID_ROW, NAME_COLUMN, new QTableWidgetItem("startStateId"));
    table->setItem(START_STATE_ID_ROW, TYPE_COLUMN, new QTableWidgetItem("hkInt32"));
    table->setItem(START_STATE_ID_ROW, BINDING_COLUMN, new QTableWidgetItem("N/A"));
    table->setCellWidget(START_STATE_ID_ROW, VALUE_COLUMN, startStateId);
    table->setItem(RETURN_TO_PREVIOUS_STATE_EVENT_ID_ROW, NAME_COLUMN, new QTableWidgetItem("returnToPreviousStateEventId"));
    table->setItem(RETURN_TO_PREVIOUS_STATE_EVENT_ID_ROW, TYPE_COLUMN, new QTableWidgetItem("hkEvent"));
    table->setItem(RETURN_TO_PREVIOUS_STATE_EVENT_ID_ROW, BINDING_COLUMN, new QTableWidgetItem("N/A"));
    table->setItem(RETURN_TO_PREVIOUS_STATE_EVENT_ID_ROW, VALUE_COLUMN, new QTableWidgetItem("NONE"));
    table->setItem(RANDOM_TRANSITION_EVENT_ID_ROW, NAME_COLUMN, new QTableWidgetItem("randomTransitionEventId"));
    table->setItem(RANDOM_TRANSITION_EVENT_ID_ROW, TYPE_COLUMN, new QTableWidgetItem("hkEvent"));
    table->setItem(RANDOM_TRANSITION_EVENT_ID_ROW, BINDING_COLUMN, new QTableWidgetItem("N/A"));
    table->setItem(RANDOM_TRANSITION_EVENT_ID_ROW, VALUE_COLUMN, new QTableWidgetItem("NONE"));
    table->setItem(TRANSITION_TO_NEXT_HIGHER_STATE_EVENT_ID_ROW, NAME_COLUMN, new QTableWidgetItem("transitionToNextHigherStateEventId"));
    table->setItem(TRANSITION_TO_NEXT_HIGHER_STATE_EVENT_ID_ROW, TYPE_COLUMN, new QTableWidgetItem("hkEvent"));
    table->setItem(TRANSITION_TO_NEXT_HIGHER_STATE_EVENT_ID_ROW, BINDING_COLUMN, new QTableWidgetItem("N/A"));
    table->setItem(TRANSITION_TO_NEXT_HIGHER_STATE_EVENT_ID_ROW, VALUE_COLUMN, new QTableWidgetItem("NONE"));
    table->setItem(TRANSITION_TO_NEXT_LOWER_STATE_EVENT_ID_ROW, NAME_COLUMN, new QTableWidgetItem("transitionToNextLowerStateEventId"));
    table->setItem(TRANSITION_TO_NEXT_LOWER_STATE_EVENT_ID_ROW, TYPE_COLUMN, new QTableWidgetItem("hkEvent"));
    table->setItem(TRANSITION_TO_NEXT_LOWER_STATE_EVENT_ID_ROW, BINDING_COLUMN, new QTableWidgetItem("N/A"));
    table->setItem(TRANSITION_TO_NEXT_LOWER_STATE_EVENT_ID_ROW, VALUE_COLUMN, new QTableWidgetItem("NONE"));
    table->setItem(SYNC_VARIABLE_INDEX_ROW, NAME_COLUMN, new QTableWidgetItem("syncVariableIndex"));
    table->setItem(SYNC_VARIABLE_INDEX_ROW, TYPE_COLUMN, new QTableWidgetItem("hkInt32"));
    table->setItem(SYNC_VARIABLE_INDEX_ROW, BINDING_COLUMN, new QTableWidgetItem("N/A"));
    table->setItem(SYNC_VARIABLE_INDEX_ROW, VALUE_COLUMN, new QTableWidgetItem("NONE"));
    table->setItem(WRAP_AROUND_STATE_ID_ROW, NAME_COLUMN, new QTableWidgetItem("wrapAroundStateId"));
    table->setItem(WRAP_AROUND_STATE_ID_ROW, TYPE_COLUMN, new QTableWidgetItem("hkBool"));
    table->setItem(WRAP_AROUND_STATE_ID_ROW, BINDING_COLUMN, new QTableWidgetItem("NONE"));
    table->setCellWidget(WRAP_AROUND_STATE_ID_ROW, VALUE_COLUMN, wrapAroundStateId);
    table->setItem(MAX_SIMULTANEOUS_TRANSITIONS_ROW, NAME_COLUMN, new QTableWidgetItem("maxSimultaneousTransitions"));
    table->setItem(MAX_SIMULTANEOUS_TRANSITIONS_ROW, TYPE_COLUMN, new QTableWidgetItem("hkInt32"));
    table->setItem(MAX_SIMULTANEOUS_TRANSITIONS_ROW, BINDING_COLUMN, new QTableWidgetItem("N/A"));
    table->setCellWidget(MAX_SIMULTANEOUS_TRANSITIONS_ROW, VALUE_COLUMN, maxSimultaneousTransitions);
    table->setItem(START_STATE_MODE_ROW, NAME_COLUMN, new QTableWidgetItem("startStateMode"));
    table->setItem(START_STATE_MODE_ROW, TYPE_COLUMN, new QTableWidgetItem("hkEnum"));
    table->setItem(START_STATE_MODE_ROW, BINDING_COLUMN, new QTableWidgetItem("N/A"));
    table->setCellWidget(START_STATE_MODE_ROW, VALUE_COLUMN, startStateMode);
    table->setItem(SELF_TRANSITION_MODE_ROW, NAME_COLUMN, new QTableWidgetItem("selfTransitionMode"));
    table->setItem(SELF_TRANSITION_MODE_ROW, TYPE_COLUMN, new QTableWidgetItem("hkEnum"));
    table->setItem(SELF_TRANSITION_MODE_ROW, BINDING_COLUMN, new QTableWidgetItem("N/A"));
    table->setCellWidget(SELF_TRANSITION_MODE_ROW, VALUE_COLUMN, selfTransitionMode);
    table->setCellWidget(ADD_CHILD_ROW, NAME_COLUMN, addStatePB);
    table->setCellWidget(ADD_CHILD_ROW, TYPE_COLUMN, typeSelectorCB);
    table->setCellWidget(ADD_CHILD_ROW, BINDING_COLUMN, removeStatePB);
    table->setItem(ADD_CHILD_ROW, VALUE_COLUMN, new QTableWidgetItem("N/A"));
    //table->setCellWidget(ADD_CHILD_ROW + 1, NAME_COLUMN, addTransitionPB);
    table->setCellWidget(ADD_CHILD_ROW + 1, NAME_COLUMN, new QPushButton("Add Transition"));
    table->setItem(ADD_CHILD_ROW + 1, TYPE_COLUMN, new QTableWidgetItem("hkbStateMachineTransitionInfoArray"));
    //table->setCellWidget(ADD_CHILD_ROW + 1, BINDING_COLUMN, removeTransitionPB);
    table->setCellWidget(ADD_CHILD_ROW + 1, BINDING_COLUMN, new QPushButton("Remove Selected Transition"));
    table->setItem(ADD_CHILD_ROW + 1, VALUE_COLUMN, new QTableWidgetItem("N/A"));
    topLyt->addWidget(table, 1, 0, 8, 3);
    groupBox->setLayout(topLyt);
    addWidget(groupBox);
    addWidget(stateUI);
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(payload, SIGNAL(editingFinished()), this, SLOT(setPayload()), Qt::UniqueConnection);
    connect(wrapAroundStateId, SIGNAL(clicked(bool)), this, SLOT(setWrapAroundStateId(bool)), Qt::UniqueConnection);
    connect(maxSimultaneousTransitions, SIGNAL(editingFinished()), this, SLOT(setMaxSimultaneousTransitions()), Qt::UniqueConnection);
    connect(startStateId, SIGNAL(activated(int)), this, SLOT(setStartStateId(int)), Qt::UniqueConnection);
    connect(startStateMode, SIGNAL(activated(int)), this, SLOT(setStartStateMode(int)), Qt::UniqueConnection);
    connect(selfTransitionMode, SIGNAL(activated(int)), this, SLOT(setSelfTransitionMode(int)), Qt::UniqueConnection);
    connect(table, SIGNAL(cellClicked(int,int)), this, SLOT(viewSelectedChild(int,int)), Qt::UniqueConnection);
    connect(addStatePB, SIGNAL(released()), this, SLOT(addNewStateWithGenerator()), Qt::UniqueConnection);
    connect(removeStatePB, SIGNAL(released()), this, SLOT(removeObjectChild()), Qt::UniqueConnection);
    /*connect(addTransitionPB, SIGNAL(released()), this, SLOT(addNewTransition()), Qt::UniqueConnection);
    connect(removeTransitionPB, SIGNAL(released()), this, SLOT(removeObjectChild()), Qt::UniqueConnection);*/
    connect(qobject_cast<QPushButton *>(table->cellWidget(ADD_CHILD_ROW + 1, NAME_COLUMN)), SIGNAL(released()), this, SLOT(addNewTransition()), Qt::UniqueConnection);
    connect(qobject_cast<QPushButton *>(table->cellWidget(ADD_CHILD_ROW + 1, BINDING_COLUMN)), SIGNAL(released()), this, SLOT(removeObjectChild()), Qt::UniqueConnection);
    connect(stateUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()), Qt::UniqueConnection);
    connect(stateUI, SIGNAL(viewVariables(int)), this, SIGNAL(viewVariables(int)), Qt::UniqueConnection);
    connect(stateUI, SIGNAL(viewProperties(int)), this, SIGNAL(viewProperties(int)), Qt::UniqueConnection);
    connect(stateUI, SIGNAL(viewGenerators(int)), this, SIGNAL(viewGenerators(int)), Qt::UniqueConnection);
}

void StateMachineUI::loadData(HkxObject *data){
    hkbStateMachineStateInfo *state = NULL;
    hkbStateMachineTransitionInfoArray *trans = NULL;
    hkbVariableBindingSet *varBind = NULL;
    QString varName;
    int ind = 0;
    if (data && data->getSignature() == HKB_STATE_MACHINE){
        bsData = static_cast<hkbStateMachine *>(data);
        rowIndexOfChildToRemove = -1;
        name->setText(bsData->name);
        payload->clear();
        if (bsData->payload.data()){
            payload->setText(static_cast<hkbStringEventPayload *>(bsData->payload.data())->data);
        }
        startStateId->clear();
        startStateId->insertItems(0, bsData->getStateNames());
        startStateId->setCurrentIndex(startStateId->findText(bsData->getStateName(bsData->startStateId), Qt::MatchCaseSensitive));
        varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (varBind){
            ind = varBind->getVariableIndexOfBinding("startStateId");
            if (ind != -1){
                varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableNameAt(ind);
                if (varName == ""){
                    varName = "NONE";
                }
                table->itemAt(START_STATE_ID_ROW, BINDING_COLUMN)->setText(varName);
            }else{
                ind = varBind->getVariableIndexOfBinding("syncVariableIndex");
                if (ind != -1){
                    varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableNameAt(ind);
                    if (varName == ""){
                        varName = "NONE";
                    }
                    table->itemAt(SYNC_VARIABLE_INDEX_ROW, BINDING_COLUMN)->setText(varName);
                }else{
                    ind = varBind->getVariableIndexOfBinding("wrapAroundStateId");
                    if (ind != -1){
                        varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableNameAt(ind);
                        if (varName == ""){
                            varName = "NONE";
                        }
                        table->itemAt(WRAP_AROUND_STATE_ID_ROW, BINDING_COLUMN)->setText(varName);
                    }
                }
            }
        }else{
            table->itemAt(START_STATE_ID_ROW, BINDING_COLUMN)->setText("NONE");
            table->itemAt(SYNC_VARIABLE_INDEX_ROW, BINDING_COLUMN)->setText("NONE");
            table->itemAt(WRAP_AROUND_STATE_ID_ROW, BINDING_COLUMN)->setText("NONE");
        }
        varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getEventNameAt(bsData->returnToPreviousStateEventId);
        if (varName != ""){
            table->itemAt(RETURN_TO_PREVIOUS_STATE_EVENT_ID_ROW, VALUE_COLUMN)->setText(varName);
        }else{
            table->itemAt(RETURN_TO_PREVIOUS_STATE_EVENT_ID_ROW, VALUE_COLUMN)->setText("NONE");
        }
        varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getEventNameAt(bsData->randomTransitionEventId);
        if (varName != ""){
            table->itemAt(RANDOM_TRANSITION_EVENT_ID_ROW, VALUE_COLUMN)->setText(varName);
        }else{
            table->itemAt(RANDOM_TRANSITION_EVENT_ID_ROW, VALUE_COLUMN)->setText("NONE");
        }
        varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getEventNameAt(bsData->transitionToNextHigherStateEventId);
        if (varName != ""){
            table->itemAt(TRANSITION_TO_NEXT_HIGHER_STATE_EVENT_ID_ROW, VALUE_COLUMN)->setText(varName);
        }else{
            table->itemAt(TRANSITION_TO_NEXT_HIGHER_STATE_EVENT_ID_ROW, VALUE_COLUMN)->setText("NONE");
        }
        varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getEventNameAt(bsData->transitionToNextLowerStateEventId);
        if (varName != ""){
            table->itemAt(TRANSITION_TO_NEXT_LOWER_STATE_EVENT_ID_ROW, VALUE_COLUMN)->setText(varName);
        }else{
            table->itemAt(TRANSITION_TO_NEXT_LOWER_STATE_EVENT_ID_ROW, VALUE_COLUMN)->setText("NONE");
        }
        varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getEventNameAt(bsData->transitionToNextLowerStateEventId);
        if (varName != ""){
            table->itemAt(TRANSITION_TO_NEXT_LOWER_STATE_EVENT_ID_ROW, VALUE_COLUMN)->setText(varName);
        }else{
            table->itemAt(TRANSITION_TO_NEXT_LOWER_STATE_EVENT_ID_ROW, VALUE_COLUMN)->setText("NONE");
        }
        varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableNameAt(bsData->syncVariableIndex);
        if (varName != ""){
            table->itemAt(SYNC_VARIABLE_INDEX_ROW, VALUE_COLUMN)->setText(varName);
        }else{
            table->itemAt(SYNC_VARIABLE_INDEX_ROW, VALUE_COLUMN)->setText("NONE");
        }
        wrapAroundStateId->setChecked(bsData->wrapAroundStateId);
        maxSimultaneousTransitions->setValue(bsData->maxSimultaneousTransitions);
        if (startStateMode->count() == 0){
            startStateMode->insertItems(0, bsData->StartStateMode);
        }
        startStateMode->setCurrentIndex(bsData->StartStateMode.indexOf(bsData->startStateMode));
        if (selfTransitionMode->count() == 0){
            selfTransitionMode->insertItems(0, bsData->SelfTransitionMode);
        }
        selfTransitionMode->setCurrentIndex(bsData->SelfTransitionMode.indexOf(bsData->selfTransitionMode));
        if (rowIndexOfTransitionButtonPanel != bsData->states.size() + BASE_NUMBER_OF_ROWS - 1){
            //moveRowItems(rowIndexOfTransitionButtonPanel, bsData->states.size() + BASE_NUMBER_OF_ROWS - 1);
            //rowIndexOfTransitionButtonPanel = bsData->states.size() + BASE_NUMBER_OF_ROWS - 1;
            moveRowWidgets(bsData->states.size() + BASE_NUMBER_OF_ROWS - 1);
        }
        for (int i = 0, k; i < bsData->states.size(); i++){
            k = i + BASE_NUMBER_OF_ROWS - 1;
            state = static_cast<hkbStateMachineStateInfo *>(bsData->states.at(i).data());
            if (k >= table->rowCount()){//Unnecessary?
                table->setRowCount(table->rowCount() + 1);
                if (k == rowIndexOfTransitionButtonPanel){
                    rowIndexOfTransitionButtonPanel = k + 1;
                    moveRowItems(k, rowIndexOfTransitionButtonPanel);
                    /*if (moveRowItems(k, rowIndexOfTransitionButtonPanel) != 0){
                        CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::loadData(): moveRowItems() returned unexpected value!!!"))
                    }*/
                }
                table->setItem(k, NAME_COLUMN, new QTableWidgetItem(state->getName()));
                table->setItem(k, TYPE_COLUMN, new QTableWidgetItem(state->getClassname()));
                table->setItem(k, BINDING_COLUMN, new QTableWidgetItem("N/A"));
                table->setItem(k, VALUE_COLUMN, new QTableWidgetItem("Click to Edit"));
            }else{
                //table->setRowHidden(k, false);
                if (k == rowIndexOfTransitionButtonPanel){
                    rowIndexOfTransitionButtonPanel = k + 1;
                    moveRowItems(k, rowIndexOfTransitionButtonPanel);
                    /*if (moveRowItems(k, rowIndexOfTransitionButtonPanel) != 0){
                        CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::loadData(): moveRowItems() returned unexpected value!!!"))
                    }*/
                }
                if (table->item(k, NAME_COLUMN)){
                    table->item(k, NAME_COLUMN)->setText(state->getName());
                    table->item(k, TYPE_COLUMN)->setText(state->getClassname());
                }else{
                    table->setItem(k, NAME_COLUMN, new QTableWidgetItem(state->getName()));
                    table->setItem(k, TYPE_COLUMN, new QTableWidgetItem(state->getClassname()));
                    table->setItem(k, BINDING_COLUMN, new QTableWidgetItem("N/A"));
                    table->setItem(k, VALUE_COLUMN, new QTableWidgetItem("Click to Edit"));
                }
            }
        }
        trans = static_cast<hkbStateMachineTransitionInfoArray *>(bsData->wildcardTransitions.data());
        if (trans){
            for (int i = 0, k = bsData->states.size() + BASE_NUMBER_OF_ROWS; i < trans->getNumTransitions(); i++, k++){
                if (k >= table->rowCount()){
                    table->setRowCount(table->rowCount() + 1);
                    if (k == rowIndexOfTransitionButtonPanel){
                        CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::loadData(): moveRowItems() returned unexpected value!!!"))
                    }
                    table->setItem(k, NAME_COLUMN, new QTableWidgetItem(trans->getTransitionNameAt(i)));
                    table->setItem(k, TYPE_COLUMN, new QTableWidgetItem(trans->getClassname()));
                    table->setItem(k, BINDING_COLUMN, new QTableWidgetItem("N/A"));
                    table->setItem(k, VALUE_COLUMN, new QTableWidgetItem("Click to Edit"));
                }else{
                    //table->setRowHidden(k, false);
                    if (k == rowIndexOfTransitionButtonPanel){
                        CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::loadData(): moveRowItems() returned unexpected value!!!"))
                    }
                    if (table->item(k, NAME_COLUMN)){
                        table->item(k, NAME_COLUMN)->setText(trans->getTransitionNameAt(i));
                        table->item(k, TYPE_COLUMN)->setText(trans->getClassname());
                    }else{
                        table->setItem(k, NAME_COLUMN, new QTableWidgetItem(trans->getTransitionNameAt(i)));
                        table->setItem(k, TYPE_COLUMN, new QTableWidgetItem(trans->getClassname()));
                        table->setItem(k, BINDING_COLUMN, new QTableWidgetItem("N/A"));
                        table->setItem(k, VALUE_COLUMN, new QTableWidgetItem("Click to Edit"));
                    }
                }
            }
            for (int i = bsData->states.size() + trans->getNumTransitions() + BASE_NUMBER_OF_ROWS; i < table->rowCount(); i++){
                table->setRowHidden(i, true);
            }
        }else{
            for (int i = bsData->states.size() + BASE_NUMBER_OF_ROWS; i < table->rowCount(); i++){
                table->setRowHidden(i, true);
            }
        }
    }
}

void StateMachineUI::setName(){
    if (bsData){
        bsData->name = name->text();//Make sure name is valid...
        ((DataIconManager *)(bsData))->updateIconNames();
        bsData->getParentFile()->toggleChanged(true);
        emit generatorNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData) + 1);
    }
}

void StateMachineUI::setEventToSendWhenStateOrTransitionChanges(int index){
    if (bsData){
        bsData->id = index - 1;
        bsData->getParentFile()->toggleChanged(true);
    }
}

void StateMachineUI::setPayload(){
    if (bsData){
        bsData->payload = HkxObjectExpSharedPtr(new hkbStringEventPayload(bsData->getParentFile(), payload->text()));
        bsData->getParentFile()->toggleChanged(true);
    }
}

void StateMachineUI::setReturnToPreviousStateEventId(int index){
    if (bsData){
        bsData->returnToPreviousStateEventId = index - 1;
        bsData->getParentFile()->toggleChanged(true);
    }
}

void StateMachineUI::setStartStateId(int index){
    if (bsData && index < bsData->states.size()&& index > -1){
        bsData->startStateId = static_cast<hkbStateMachineStateInfo *>(bsData->states.at(index).data())->stateId;
        bsData->getParentFile()->toggleChanged(true);
    }
}

bool StateMachineUI::setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type){
    hkbVariableBindingSet *varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
    if (bsData){
        if (index == 0){
            varBind->removeBinding(path);
            table->item(row, BINDING_COLUMN)->setText("None");
        }else if (static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableTypeAt(index - 1) == type){
            if (!varBind){
                varBind = new hkbVariableBindingSet(bsData->getParentFile());
                bsData->variableBindingSet = HkxObjectExpSharedPtr(varBind);
                bsData->getParentFile()->addObjectToFile(varBind, -1);
            }
            if (type == VARIABLE_TYPE_POINTER){
                varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY);
            }else{
                varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE);
            }
            table->item(row, BINDING_COLUMN)->setText(variableName);
            bsData->getParentFile()->toggleChanged(true);
        }else{
            WARNING_MESSAGE(QString("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateMachineUI: The data is NULL!!"))
    }
    return true;
}

void StateMachineUI::setBindingVariable(int index, const QString & name){
    if (bsData){
        int row = table->currentRow();
        switch (row){
        case START_STATE_ID_ROW:
            setBinding(index, row, name, "startStateId", VARIABLE_TYPE_INT32);
            break;
        case SYNC_VARIABLE_INDEX_ROW:
            setBinding(index, row, name, "syncVariableIndex", VARIABLE_TYPE_INT32);
            break;
        case WRAP_AROUND_STATE_ID_ROW:
            setBinding(index, row, name, "wrapAroundStateId", VARIABLE_TYPE_BOOL);
            break;
        default:
            return;
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateMachineUI: The data is NULL!!"))
    }
}

void StateMachineUI::setRandomTransitionEventId(int index){
    if (bsData){
        bsData->randomTransitionEventId = index - 1;
        bsData->getParentFile()->toggleChanged(true);
    }
}

void StateMachineUI::setTransitionToNextHigherStateEventId(int index){
    if (bsData){
        bsData->transitionToNextHigherStateEventId = index - 1;
        bsData->getParentFile()->toggleChanged(true);
    }
}

void StateMachineUI::setTransitionToNextLowerStateEventId(int index){
    if (bsData){
        bsData->transitionToNextLowerStateEventId = index - 1;
        bsData->getParentFile()->toggleChanged(true);
    }
}

void StateMachineUI::setSyncVariableIndex(int index){
    if (bsData){
        bsData->syncVariableIndex = index - 1;
        bsData->getParentFile()->toggleChanged(true);
    }
}

void StateMachineUI::setWrapAroundStateId(bool checked){
    if (bsData){
        bsData->wrapAroundStateId = checked;
        bsData->getParentFile()->toggleChanged(true);
    }
}

void StateMachineUI::setMaxSimultaneousTransitions(){
    if (bsData){
        bsData->maxSimultaneousTransitions = maxSimultaneousTransitions->value();
        bsData->getParentFile()->toggleChanged(true);
    }
}

void StateMachineUI::setStartStateMode(int index){
    if (bsData){
        bsData->startStateMode = bsData->StartStateMode.at(index);
        bsData->getParentFile()->toggleChanged(true);
    }
}

void StateMachineUI::setSelfTransitionMode(int index){
    if (bsData){
        bsData->selfTransitionMode = bsData->SelfTransitionMode.at(index);
        bsData->getParentFile()->toggleChanged(true);
    }
}

void StateMachineUI::viewSelectedChild(int row, int column){
    int result;
    int index = 0;
    QString path;
    hkbStateMachineTransitionInfoArray *trans = NULL;
    if (bsData){
        if (row < BASE_NUMBER_OF_ROWS - 1 && row >= 0){
            if (column == BINDING_COLUMN){
                switch (row){
                case START_STATE_ID_ROW:
                    path = "startStateId";
                    break;
                case SYNC_VARIABLE_INDEX_ROW:
                    path = "syncVariableIndex";
                    break;
                case WRAP_AROUND_STATE_ID_ROW:
                    path = "wrapAroundStateId";
                    break;
                default:
                    return;
                }
                if (bsData->variableBindingSet.data()){
                    emit viewVariables(static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data())->getVariableIndexOfBinding(path) + 1);
                }else{
                    emit viewVariables(0);
                }
            }else if (column == VALUE_COLUMN){
                switch (row){
                case ID_ROW:
                    path = "id";
                    index = bsData->id;
                    break;
                case RETURN_TO_PREVIOUS_STATE_EVENT_ID_ROW:
                    path = "returnToPreviousStateEventId";
                    index = bsData->returnToPreviousStateEventId;
                    break;
                case RANDOM_TRANSITION_EVENT_ID_ROW:
                    path = "randomTransitionEventId";
                    index = bsData->randomTransitionEventId;
                    break;
                case TRANSITION_TO_NEXT_HIGHER_STATE_EVENT_ID_ROW:
                    path = "transitionToNextHigherStateEventId";
                    index = bsData->transitionToNextHigherStateEventId;
                    break;
                case TRANSITION_TO_NEXT_LOWER_STATE_EVENT_ID_ROW:
                    path = "transitionToNextLowerStateEventId";
                    index = bsData->transitionToNextLowerStateEventId;
                    break;
                default:
                    return;
                }
                emit viewEvents(index + 1);
            }
        }else{
            result = row - BASE_NUMBER_OF_ROWS + 1;
            rowIndexOfChildToRemove = row;
            trans = static_cast<hkbStateMachineTransitionInfoArray *>(bsData->wildcardTransitions.data());
            if (bsData->states.size() > result && result >= 0){
                if (column == VALUE_COLUMN){
                    stateUI->loadData(static_cast<hkbStateMachineStateInfo *>(bsData->states.at(result).data()));
                    setCurrentIndex(STATE_WIDGET);
                }
            }else if (trans && bsData->states.size() < result && result < bsData->states.size() + trans->getNumTransitions()){
                if (column == VALUE_COLUMN && result != rowIndexOfTransitionButtonPanel){
                    stateUI->loadData(static_cast<hkbStateMachineStateInfo *>(bsData->states.at(result).data()));
                    setCurrentIndex(TRANSITION_WIDGET);
                }
            }else{
                CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::viewSelectedChild(): Invalid index of child to view!!"))
            }
        }
    }
}

void StateMachineUI::moveRowWidgets(int targetRow){//Have to do this because I cannot swap cell widgets around for some fucking reason...
    table->setRowCount(targetRow + 1);
    table->removeCellWidget(rowIndexOfTransitionButtonPanel, NAME_COLUMN);
    table->removeCellWidget(rowIndexOfTransitionButtonPanel, BINDING_COLUMN);
    for (int i = 0; i < VALUE_COLUMN; i++){
        if (table->itemAt(rowIndexOfTransitionButtonPanel, i)){
            delete table->takeItem(rowIndexOfTransitionButtonPanel, 0);
        }
    }
    rowIndexOfTransitionButtonPanel = targetRow;
    table->setCellWidget(rowIndexOfTransitionButtonPanel, NAME_COLUMN, new QPushButton("Add Transition"));
    table->setCellWidget(rowIndexOfTransitionButtonPanel, BINDING_COLUMN, new QPushButton("Remove Selected Transition"));
    connect(qobject_cast<QPushButton *>(table->cellWidget(rowIndexOfTransitionButtonPanel, NAME_COLUMN)), SIGNAL(released()), this, SLOT(addNewTransition()), Qt::UniqueConnection);
    connect(qobject_cast<QPushButton *>(table->cellWidget(rowIndexOfTransitionButtonPanel, BINDING_COLUMN)), SIGNAL(released()), this, SLOT(removeObjectChild()), Qt::UniqueConnection);
}

int StateMachineUI::moveRowItems(int oldRow, int targetRow){
    int count = 0;
    QTableWidgetItem *oldItem = NULL;
    QWidget *oldWidget = NULL;
    QTableWidgetItem *targetItem = NULL;
    QWidget *targetWidget = NULL;
    if (oldRow != targetRow){
        if (targetRow >= table->rowCount()){
            table->setRowCount(targetRow + 1);
        }
        for (int j = oldRow; j < table->rowCount(); j++){
            table->setRowHidden(j, false);
        }
        for (int i = 0; i < table->columnCount(); i++){
            oldItem = table->takeItem(oldRow, i);
            oldWidget = table->cellWidget(oldRow, i);
            targetItem = table->takeItem(targetRow, i);
            targetWidget = table->cellWidget(targetRow, i);
            if (targetItem){
                count++;
                table->setItem(oldRow, i, targetItem);
            }else if (targetWidget){
                count++;
                table->setCellWidget(oldRow, i, targetWidget);
            }
            if (oldItem){
                count++;
                table->setItem(targetRow, i, oldItem);
            }else if (oldWidget){
                count++;
                table->setCellWidget(targetRow, i, oldWidget);
            }
        }
    }
    return count;
}


void StateMachineUI::generatorRenamed(const QString &name, int index){
    if (name == ""){
        WARNING_MESSAGE(QString("StateMachineUI::generatorRenamed(): The new variable name is the empty string!!"))
    }
    //stateUI->generatorRenamed(name, index);
}

void StateMachineUI::addNewStateWithGenerator(){
    int result;
    Generator_Type typeEnum;
    int lastRow = -1;
    hkbStateMachineTransitionInfoArray *trans = NULL;
    if (bsData && behaviorView){
        typeEnum = static_cast<Generator_Type>(typeSelectorCB->currentIndex());
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
            CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::addNewStateWithGenerator(): Invalid typeEnum!!"))
            return;
        }
        result = BASE_NUMBER_OF_ROWS + bsData->states.size();
        trans = static_cast<hkbStateMachineTransitionInfoArray *>(bsData->wildcardTransitions.data());
        if (trans){
            lastRow = result + trans->getNumTransitions();
        }else{
            lastRow = result;
        }
        for (int i = lastRow; i > result; i--){
            moveRowItems(i, i + 1);
        }
        if (result < table->rowCount()){
            table->setRowCount(table->rowCount() + 1);
            table->setItem(result, NAME_COLUMN, new QTableWidgetItem(static_cast<hkbStateMachineStateInfo *>(bsData->states.last().data())->getName()));
            table->setItem(result, TYPE_COLUMN, new QTableWidgetItem(static_cast<hkbStateMachineStateInfo *>(bsData->states.last().data())->getClassname()));
            table->setItem(result, BINDING_COLUMN, new QTableWidgetItem("N/A"));
            table->setItem(result, VALUE_COLUMN, new QTableWidgetItem("Click to Edit"));
        }else{
            CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::addNewStateWithGenerator(): Unexpected value of 'result'!!"))
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::addNewStateWithGenerator(): The data or behavior graph pointer is NULL!!"))
    }
}

void StateMachineUI::removeObjectChild(){
    int result = -1;
    hkbStateMachineStateInfo *child = NULL;
    hkbStateMachineTransitionInfoArray *trans = NULL;
    if (bsData && behaviorView){
        trans = static_cast<hkbStateMachineTransitionInfoArray *>(bsData->wildcardTransitions.data());
        if (rowIndexOfChildToRemove > BASE_NUMBER_OF_ROWS + bsData->states.size()){
            if (trans){
                result = rowIndexOfChildToRemove - BASE_NUMBER_OF_ROWS - bsData->states.size();
                if (result != rowIndexOfTransitionButtonPanel && result < trans->getNumTransitions() && result >= 0){
                    trans->removeTransition(result);
                    delete table->takeItem(rowIndexOfChildToRemove, NAME_COLUMN);
                    delete table->takeItem(rowIndexOfChildToRemove, TYPE_COLUMN);
                    delete table->takeItem(rowIndexOfChildToRemove, BINDING_COLUMN);
                    delete table->takeItem(rowIndexOfChildToRemove, VALUE_COLUMN);
                    table->removeRow(rowIndexOfChildToRemove);
                }else{
                    CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::removeObjectChild(): Invalid row index selected!!"))
                    return;
                }
            }else{
                CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::removeObjectChild(): Transition data is NULL!!"))
                return;
            }
        }else{
            result = rowIndexOfChildToRemove - BASE_NUMBER_OF_ROWS;
            if (result < bsData->states.size() && result >= 0){
                child = static_cast<hkbStateMachineStateInfo *>(bsData->states.at(result).data());
                behaviorView->removeItemFromGraph(behaviorView->getSelectedIconsChildIcon(child->generator.data()), result);//Reorderchildren?
                behaviorView->removeObjects();
                delete table->takeItem(rowIndexOfChildToRemove, NAME_COLUMN);
                delete table->takeItem(rowIndexOfChildToRemove, TYPE_COLUMN);
                delete table->takeItem(rowIndexOfChildToRemove, BINDING_COLUMN);
                delete table->takeItem(rowIndexOfChildToRemove, VALUE_COLUMN);
                table->removeRow(rowIndexOfChildToRemove);
                for (int i = BASE_NUMBER_OF_ROWS, k = 0; i < table->rowCount(); i++, k++){
                    if (!table->isRowHidden(i)){
                        if (!table->item(i, NAME_COLUMN)){
                            table->setItem(i, NAME_COLUMN, new QTableWidgetItem("Child "+QString::number(k)));
                        }else{
                            table->item(i, NAME_COLUMN)->setText("Child "+QString::number(k));
                        }
                    }
                }
                rowIndexOfChildToRemove = -1;
            }else{
                CRITICAL_ERROR_MESSAGE(QString("StateMachineUI: Invalid index of child to remove!!"))
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateMachineUI: The data or behavior graph pointer is NULL!!"))
    }
}

void StateMachineUI::addNewTransition(){
    int result;
    int lastRow = -1;
    hkbStateMachineTransitionInfoArray *trans = NULL;
    if (bsData && behaviorView){
        trans = static_cast<hkbStateMachineTransitionInfoArray *>(bsData->wildcardTransitions.data());
        if (trans){
            result = BASE_NUMBER_OF_ROWS + bsData->states.size() + trans->getNumTransitions();
            lastRow = result + trans->getNumTransitions();
        }else{
            trans = new hkbStateMachineTransitionInfoArray(bsData->getParentFile(), bsData, -1);
            trans->addTransition();
            bsData->getParentFile()->addObjectToFile(trans, -1);
            result = BASE_NUMBER_OF_ROWS + bsData->states.size();
            lastRow = result;
        }
        for (int i = lastRow; i > result; i--){
            moveRowItems(i, i + 1);
        }
        if (result < table->rowCount()){
            table->setRowCount(table->rowCount() + 1);
            table->setItem(result, NAME_COLUMN, new QTableWidgetItem(trans->getTransitionNameAt(0)));
            table->setItem(result, TYPE_COLUMN, new QTableWidgetItem(trans->getClassname()));
            table->setItem(result, BINDING_COLUMN, new QTableWidgetItem("N/A"));
            table->setItem(result, VALUE_COLUMN, new QTableWidgetItem("Click to Edit"));
        }else{
            CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::addNewTransition(): Unexpected value of 'result'!!"))
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::addNewTransition(): The data or behavior graph pointer is NULL!!"))
    }
}

void StateMachineUI::returnToWidget(){
    setCurrentIndex(MAIN_WIDGET);
}

void StateMachineUI::connectChildUI(GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *generators){
    if (variables && properties && generators){
        connect(variables, SIGNAL(elementSelected(int,QString)), stateUI, SLOT(setBindingVariable(int,QString)));
        connect(properties, SIGNAL(elementSelected(int,QString)), stateUI, SLOT(setBindingVariable(int,QString)));
        connect(generators, SIGNAL(elementSelected(int,QString)), stateUI, SLOT(setGenerator(int,QString)));
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::connectChildUI(): One or more arguments are NULL!!"))
    }
}

void StateMachineUI::renameVariable(const QString & name, int index){
    int bindIndex = -1;
    hkbVariableBindingSet *bind = NULL;
    if (bsData){
        index--;
        bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (bind){
            bindIndex = bind->getVariableIndexOfBinding("startStateId");
            if (bindIndex == index){
                table->item(START_STATE_ID_ROW, BINDING_COLUMN)->setText(name);
            }else{
                bindIndex = bind->getVariableIndexOfBinding("syncVariableIndex");
                if (bindIndex == index){
                    table->item(SYNC_VARIABLE_INDEX_ROW, BINDING_COLUMN)->setText(name);
                }else{
                    bindIndex = bind->getVariableIndexOfBinding("wrapAroundStateId");
                    if (bindIndex == index){
                        table->item(WRAP_AROUND_STATE_ID_ROW, BINDING_COLUMN)->setText(name);
                    }
                }
            }
        }
    }
}

void StateMachineUI::eventRenamed(const QString & name, int index){
    if (bsData){
        if (index == bsData->id){
            table->item(ID_ROW, VALUE_COLUMN)->setText(name);
        }
        if (index == bsData->returnToPreviousStateEventId){
            table->item(RETURN_TO_PREVIOUS_STATE_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        }
        if (index == bsData->randomTransitionEventId){
            table->item(RANDOM_TRANSITION_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        }
        if (index == bsData->transitionToNextHigherStateEventId){
            table->item(TRANSITION_TO_NEXT_HIGHER_STATE_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        }
        if (index == bsData->transitionToNextLowerStateEventId){
            table->item(TRANSITION_TO_NEXT_LOWER_STATE_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        }
    }
}

void StateMachineUI::setBehaviorView(BehaviorGraphView *view){
    behaviorView = view;
    stateUI->setBehaviorView(view);
}

