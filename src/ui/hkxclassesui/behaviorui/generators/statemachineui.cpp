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
#include "src/ui/hkxclassesui/behaviorui/eventui.h"

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

#define BASE_NUMBER_OF_ROWS 14

#define NAME_ROW 0
#define EVENT_TO_SEND_WHEN_STATE_OR_TRANSITION_CHANGES_ROW 1
#define START_STATE_ID_ROW 2
#define RETURN_TO_PREVIOUS_STATE_EVENT_ID_ROW 3
#define RANDOM_TRANSITION_EVENT_ID_ROW 4
#define TRANSITION_TO_NEXT_HIGHER_STATE_EVENT_ID_ROW 5
#define TRANSITION_TO_NEXT_LOWER_STATE_EVENT_ID_ROW 6
#define SYNC_VARIABLE_INDEX_ROW 7
#define WRAP_AROUND_STATE_ID_ROW 8
#define MAX_SIMULTANEOUS_TRANSITIONS_ROW 9
#define START_STATE_MODE_ROW 10
#define SELF_TRANSITION_MODE_ROW 11
#define ADD_CHILD_ROW 12

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
    : rowToRemove(-1),
      transitionsButtonRow(ADD_CHILD_ROW + 1),
      behaviorView(NULL),
      bsData(NULL),
      groupBox(new QGroupBox("hkbStateMachine")),
      topLyt(new QGridLayout),
      eventUI(new EventUI),
      stateUI(new StateUI),
      transitionUI(new TransitionsUI),
      table(new TableWidget),
      addStatePB(new QPushButton("Add State With Generator")),
      removeStatePB(new QPushButton("Remove Selected State")),
      typeSelectorCB(new ComboBox),
      addTransitionPB(new QPushButton("Add Transition")),
      removeTransitionPB(new QPushButton("Remove Selected Transition")),
      name(new LineEdit),
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
    table->setItem(EVENT_TO_SEND_WHEN_STATE_OR_TRANSITION_CHANGES_ROW, NAME_COLUMN, new QTableWidgetItem("eventToSendWhenStateOrTransitionChanges"));
    table->setItem(EVENT_TO_SEND_WHEN_STATE_OR_TRANSITION_CHANGES_ROW, TYPE_COLUMN, new QTableWidgetItem("hkEventPayload"));
    table->setItem(EVENT_TO_SEND_WHEN_STATE_OR_TRANSITION_CHANGES_ROW, BINDING_COLUMN, new QTableWidgetItem("N/A"));
    table->setItem(EVENT_TO_SEND_WHEN_STATE_OR_TRANSITION_CHANGES_ROW, VALUE_COLUMN, new QTableWidgetItem("Click To Edit"));
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
    table->setCellWidget(ADD_CHILD_ROW + 1, NAME_COLUMN, addTransitionPB);
    table->setItem(ADD_CHILD_ROW + 1, TYPE_COLUMN, new QTableWidgetItem("hkbStateMachineTransitionInfoArray"));
    table->setCellWidget(ADD_CHILD_ROW + 1, BINDING_COLUMN, removeTransitionPB);
    table->setItem(ADD_CHILD_ROW + 1, VALUE_COLUMN, new QTableWidgetItem("N/A"));
    topLyt->addWidget(table, 1, 0, 8, 3);
    groupBox->setLayout(topLyt);
    //Order here must correspond with the ACTIVE_WIDGET Enumerated type!!!
    addWidget(groupBox);
    addWidget(stateUI);
    addWidget(transitionUI);
    addWidget(eventUI);
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(wrapAroundStateId, SIGNAL(clicked(bool)), this, SLOT(setWrapAroundStateId(bool)), Qt::UniqueConnection);
    connect(maxSimultaneousTransitions, SIGNAL(editingFinished()), this, SLOT(setMaxSimultaneousTransitions()), Qt::UniqueConnection);
    connect(startStateId, SIGNAL(activated(int)), this, SLOT(setStartStateId(int)), Qt::UniqueConnection);
    connect(startStateMode, SIGNAL(activated(int)), this, SLOT(setStartStateMode(int)), Qt::UniqueConnection);
    connect(selfTransitionMode, SIGNAL(activated(int)), this, SLOT(setSelfTransitionMode(int)), Qt::UniqueConnection);
    connect(table, SIGNAL(cellClicked(int,int)), this, SLOT(viewSelectedChild(int,int)), Qt::UniqueConnection);
    connect(addStatePB, SIGNAL(released()), this, SLOT(addNewStateWithGenerator()), Qt::UniqueConnection);
    connect(removeStatePB, SIGNAL(released()), this, SLOT(removeObjectChild()), Qt::UniqueConnection);
    connect(addTransitionPB, SIGNAL(released()), this, SLOT(addTransition()), Qt::UniqueConnection);
    connect(removeTransitionPB, SIGNAL(released()), this, SLOT(removeObjectChild()), Qt::UniqueConnection);
    connect(eventUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()), Qt::UniqueConnection);
    connect(eventUI, SIGNAL(viewEvents(int)), this, SIGNAL(viewEvents(int)), Qt::UniqueConnection);
    connect(stateUI, SIGNAL(viewEvents(int)), this, SIGNAL(viewEvents(int)), Qt::UniqueConnection);
    connect(stateUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()), Qt::UniqueConnection);
    connect(stateUI, SIGNAL(viewVariables(int)), this, SIGNAL(viewVariables(int)), Qt::UniqueConnection);
    //connect(stateUI, SIGNAL(viewProperties(int)), this, SIGNAL(viewProperties(int)), Qt::UniqueConnection);
    connect(stateUI, SIGNAL(viewGenerators(int)), this, SIGNAL(viewGenerators(int)), Qt::UniqueConnection);
}

void StateMachineUI::loadData(HkxObject *data){
    setCurrentIndex(MAIN_WIDGET);
    hkbVariableBindingSet *varBind = NULL;
    QString varName;
    int ind = 0;
    if (data && data->getSignature() == HKB_STATE_MACHINE){
        bsData = static_cast<hkbStateMachine *>(data);
        rowToRemove = -1;
        name->setText(bsData->name);
        varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getEventNameAt(bsData->eventToSendWhenStateOrTransitionChanges.id);
        if (varName != ""){
            table->itemAt(EVENT_TO_SEND_WHEN_STATE_OR_TRANSITION_CHANGES_ROW, VALUE_COLUMN)->setText(varName);
        }else{
            table->itemAt(EVENT_TO_SEND_WHEN_STATE_OR_TRANSITION_CHANGES_ROW, VALUE_COLUMN)->setText("NONE");
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
        loadDynamicTableRows();
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::loadData(): The data is NULL or an incorrect type!!"));
    }
}

void StateMachineUI::loadDynamicTableRows(){
    if (bsData){
        int temp = ADD_CHILD_ROW + bsData->getNumberOfStates() + 1 - transitionsButtonRow;
        if (temp > 0){
            //insert rows
            for (int i = 0; i < temp; i++){
                table->insertRow(transitionsButtonRow);
                transitionsButtonRow++;
            }
        }else if (temp < 0){
            //remove rows
            for (int i = temp; i < 0; i++){
                table->removeRow(transitionsButtonRow - 1);
                transitionsButtonRow--;
            }
        }
        transitionsButtonRow = ADD_CHILD_ROW + bsData->getNumberOfStates() + 1;
        //loop through rows naming everything
        hkbStateMachineStateInfo *state = NULL;
        for (int i = ADD_CHILD_ROW + 1, j = 0; i < transitionsButtonRow, j < bsData->getNumberOfStates(); i++, j++){
            state = static_cast<hkbStateMachineStateInfo *>(bsData->states.at(j).data());
            if (state){
                setRowItems(i, state->getName(), state->getClassname(), "N/A", "Click to Edit");
            }else{
                CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::loadData(): Null state found!!!"));
            }
        }
        //loop through rows naming everything
        hkbStateMachineTransitionInfoArray *trans = static_cast<hkbStateMachineTransitionInfoArray *>(bsData->wildcardTransitions.data());
        if (trans){
            table->setRowCount(transitionsButtonRow + trans->getNumTransitions() + 1);
            for (int i = transitionsButtonRow + 1, j = 0; i < table->rowCount(), j < trans->getNumTransitions(); i++, j++){
                setRowItems(i, trans->getTransitionNameAt(j), trans->getClassname(), "N/A", "Click to Edit");
            }
        }else{
            table->setRowCount(transitionsButtonRow + 1);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::loadDynamicTableRows(): The data is NULL!!"));
    }
}

void StateMachineUI::setRowItems(int row, const QString & name, const QString & classname, const QString & bind, const QString & value){
    if (table->item(row, NAME_COLUMN)){
        table->item(row, NAME_COLUMN)->setText(name);
    }else{
        table->setItem(row, NAME_COLUMN, new QTableWidgetItem(name));
    }
    if (table->item(row, TYPE_COLUMN)){
        table->item(row, TYPE_COLUMN)->setText(classname);
    }else{
        table->setItem(row, TYPE_COLUMN, new QTableWidgetItem(classname));
    }
    if (table->item(row, BINDING_COLUMN)){
        table->item(row, BINDING_COLUMN)->setText(bind);
    }else{
        table->setItem(row, BINDING_COLUMN, new QTableWidgetItem(bind));
    }
    if (table->item(row, VALUE_COLUMN)){
        table->item(row, VALUE_COLUMN)->setText(value);
    }else{
        table->setItem(row, VALUE_COLUMN, new QTableWidgetItem(value));
    }
}

void StateMachineUI::setName(){
    if (bsData){
        bsData->name = name->text();//Make sure name is valid...
        ((DataIconManager *)(bsData))->updateIconNames();
        bsData->getParentFile()->toggleChanged(true);
        emit generatorNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData) + 1);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::setName(): The data is NULL!!"));
    }
}

void StateMachineUI::setReturnToPreviousStateEventId(int index, const QString & name){
    if (bsData){
        bsData->returnToPreviousStateEventId = index - 1;
        table->item(RETURN_TO_PREVIOUS_STATE_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::setReturnToPreviousStateEventId(): The data is NULL!!"));
    }
}

void StateMachineUI::setStartStateId(int index){
    if (bsData && index < bsData->states.size()&& index > -1){
        bsData->startStateId = static_cast<hkbStateMachineStateInfo *>(bsData->states.at(index).data())->stateId;
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::setStartStateId(): The data is NULL!!"));
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
                bsData->variableBindingSet = HkxSharedPtr(varBind);
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
            WARNING_MESSAGE(QString("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!"));;
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::setBinding(): The data is NULL!!"));
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
        CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::setBindingVariable(): The data is NULL!!"));
    }
}

void StateMachineUI::setRandomTransitionEventId(int index, const QString & name){
    if (bsData){
        bsData->randomTransitionEventId = index - 1;
        table->item(RANDOM_TRANSITION_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::setRandomTransitionEventId(): The data is NULL!!"));
    }
}

void StateMachineUI::setTransitionToNextHigherStateEventId(int index, const QString &name){
    if (bsData){
        bsData->transitionToNextHigherStateEventId = index - 1;
        table->item(TRANSITION_TO_NEXT_HIGHER_STATE_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::setTransitionToNextHigherStateEventId(): The data is NULL!!"));
    }
}

void StateMachineUI::setTransitionToNextLowerStateEventId(int index, const QString &name){
    if (bsData){
        bsData->transitionToNextLowerStateEventId = index - 1;
        table->item(TRANSITION_TO_NEXT_LOWER_STATE_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::setTransitionToNextLowerStateEventId(): The data is NULL!!"));
    }
}

void StateMachineUI::setSyncVariableIndex(int index){
    if (bsData){
        bsData->syncVariableIndex = index - 1;
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::setSyncVariableIndex(): The data is NULL!!"));
    }
}

void StateMachineUI::setWrapAroundStateId(bool checked){
    if (bsData){
        bsData->wrapAroundStateId = checked;
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::setWrapAroundStateId(): The data is NULL!!"));
    }
}

void StateMachineUI::setMaxSimultaneousTransitions(){
    if (bsData){
        bsData->maxSimultaneousTransitions = maxSimultaneousTransitions->value();
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::setMaxSimultaneousTransitions(): The data is NULL!!"));
    }
}

void StateMachineUI::setStartStateMode(int index){
    if (bsData){
        bsData->startStateMode = bsData->StartStateMode.at(index);
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::setStartStateMode(): The data is NULL!!"));
    }
}

void StateMachineUI::setSelfTransitionMode(int index){
    if (bsData){
        bsData->selfTransitionMode = bsData->SelfTransitionMode.at(index);
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::setSelfTransitionMode(): The data is NULL!!"));
    }
}

void StateMachineUI::viewSelectedChild(int row, int column){
    int result;
    int index = 0;
    QString path;
    hkbStateMachineTransitionInfoArray *trans = NULL;
    if (bsData){
        if (row < ADD_CHILD_ROW && row >= 0){
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
                if (row == EVENT_TO_SEND_WHEN_STATE_OR_TRANSITION_CHANGES_ROW){
                    eventUI->loadData(static_cast<BehaviorFile *>(bsData->getParentFile()), &bsData->eventToSendWhenStateOrTransitionChanges);
                    setCurrentIndex(EVENT_PAYLOAD_WIDGET);
                }else{
                    switch (row){
                    case EVENT_TO_SEND_WHEN_STATE_OR_TRANSITION_CHANGES_ROW:
                        path = "id";
                        index = bsData->eventToSendWhenStateOrTransitionChanges.id;
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
            }
        }else if (row > ADD_CHILD_ROW && row < transitionsButtonRow){
            result = row - BASE_NUMBER_OF_ROWS + 1;
            rowToRemove = row;
            if (bsData->getNumberOfStates() > result && result >= 0){
                if (column == VALUE_COLUMN){
                    stateUI->loadData(static_cast<hkbStateMachineStateInfo *>(bsData->states.at(result).data()));
                    setCurrentIndex(STATE_WIDGET);
                }
            }else{
                CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::viewSelectedChild(): Invalid index of child to view!!"));
            }
        }else if (row > transitionsButtonRow && row < table->rowCount()){
            result = row - BASE_NUMBER_OF_ROWS - bsData->getNumberOfStates();
            rowToRemove = row;
            trans = static_cast<hkbStateMachineTransitionInfoArray *>(bsData->wildcardTransitions.data());
            if (trans && result < trans->getNumTransitions() && result >= 0){
                if (column == VALUE_COLUMN){
                    transitionUI->loadData(bsData, &trans->transitions[result]);
                    setCurrentIndex(TRANSITION_WIDGET);
                }
            }else{
                CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::viewSelectedChild(): Invalid index of child to view!!"));
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::viewSelectedChild(): The data is NULL!!"));
    }
}

void StateMachineUI::eventTableElementSelected(int index, const QString &name){
    switch (currentIndex()){
    case MAIN_WIDGET:
        switch (table->currentRow()){
        case RETURN_TO_PREVIOUS_STATE_EVENT_ID_ROW:
            setReturnToPreviousStateEventId(index, name);
            break;
        case RANDOM_TRANSITION_EVENT_ID_ROW:
            setRandomTransitionEventId(index, name);
            break;
        case TRANSITION_TO_NEXT_HIGHER_STATE_EVENT_ID_ROW:
            setTransitionToNextHigherStateEventId(index, name);
            break;
        case TRANSITION_TO_NEXT_LOWER_STATE_EVENT_ID_ROW:
            setTransitionToNextLowerStateEventId(index, name);
            break;
        default:
            WARNING_MESSAGE(QString("StateMachineUI::eventTableElementSelected(): An unwanted element selected event was recieved!!"));
            return;
        }
        break;
    case EVENT_PAYLOAD_WIDGET:
        eventUI->setEvent(index, name);
        break;
    case STATE_WIDGET:
        stateUI->setEvent(index, name);
        break;
    case TRANSITION_WIDGET:
        //transitionUI->setEvent(index, name);
        break;
    default:
        WARNING_MESSAGE(QString("StateMachineUI::eventTableElementSelected(): An unwanted element selected event was recieved!!"));;
    }
}

void StateMachineUI::variableTableElementSelected(int index, const QString &name){
    switch (currentIndex()){
    case MAIN_WIDGET:
        setBindingVariable(index, name);
        break;
    /*case STATE_WIDGET:
        stateUI->setBindingVariable(index, name);
        break;*/
    default:
        WARNING_MESSAGE(QString("StateMachineUI::variableTableElementSelected(): An unwanted element selected event was recieved!!"));;
    }
}

void StateMachineUI::generatorTableElementSelected(int index, const QString &name){
    switch (currentIndex()){
    case STATE_WIDGET:
        stateUI->setGenerator(index, name);
        break;
    default:
        WARNING_MESSAGE(QString("StateMachineUI::generatorTableElementSelected(): An unwanted element selected event was recieved!!"));;
    }
}

void StateMachineUI::generatorRenamed(const QString &name, int index){
    if (name == ""){
        WARNING_MESSAGE(QString("StateMachineUI::generatorRenamed(): The new variable name is the empty string!!"));
    }
    stateUI->generatorRenamed(name, index);
}

void StateMachineUI::addNewStateWithGenerator(){
    Generator_Type typeEnum;
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
            CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::addNewStateWithGenerator(): Invalid typeEnum!!"));
            return;
        }
        loadDynamicTableRows();
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::addNewStateWithGenerator(): The data or behavior graph pointer is NULL!!"));
    }
}

void StateMachineUI::removeObjectChild(){
    int result = -1;
    hkbStateMachineStateInfo *child = NULL;
    hkbStateMachineTransitionInfoArray *trans = NULL;
    if (bsData && behaviorView){
        if (rowToRemove != transitionsButtonRow && rowToRemove != ADD_CHILD_ROW){
            trans = static_cast<hkbStateMachineTransitionInfoArray *>(bsData->wildcardTransitions.data());
            if (rowToRemove >= BASE_NUMBER_OF_ROWS + bsData->states.size()){
                if (trans){
                    result = rowToRemove - BASE_NUMBER_OF_ROWS - bsData->states.size();
                    if (result != transitionsButtonRow && result < trans->getNumTransitions() && result >= 0){
                        trans->removeTransition(result);
                    }else{
                        WARNING_MESSAGE(QString("StateMachineUI::removeObjectChild(): Invalid row index selected!!"));
                        return;
                    }
                }else{
                    WARNING_MESSAGE(QString("StateMachineUI::removeObjectChild(): Transition data is NULL!!"));
                    return;
                }
            }else if (rowToRemove > ADD_CHILD_ROW){
                result = rowToRemove - ADD_CHILD_ROW - 1;
                if (result < bsData->states.size() && result >= 0){
                    child = static_cast<hkbStateMachineStateInfo *>(bsData->states.at(result).data());
                    behaviorView->removeItemFromGraph(behaviorView->getSelectedIconsChildIcon(child->generator.data()), result);//Reorderchildren?
                    behaviorView->removeObjects();
                    //transitionsButtonRow--;
                    rowToRemove = -1;
                }else{
                    WARNING_MESSAGE(QString("StateMachineUI::removeObjectChild(): Invalid index of child to remove!!"));
                }
            }else{
                WARNING_MESSAGE(QString("StateMachineUI::removeObjectChild(): Invalid index of child to remove!!"));
            }
            loadDynamicTableRows();
        }else{
            WARNING_MESSAGE(QString("StateMachineUI::removeObjectChild(): Attempting to remove button row!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::removeObjectChild(): The data or behavior graph pointer is NULL!!"));
    }
}

void StateMachineUI::addTransition(){
    hkbStateMachineTransitionInfoArray *trans = NULL;
    if (bsData){
        trans = static_cast<hkbStateMachineTransitionInfoArray *>(bsData->wildcardTransitions.data());
        if (!trans){
            trans = new hkbStateMachineTransitionInfoArray(bsData->getParentFile(), bsData, -1);
            bsData->getParentFile()->addObjectToFile(trans, -1);
            bsData->wildcardTransitions = HkxSharedPtr(trans);
        }
        trans->addTransition();
        loadDynamicTableRows();
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::addNewTransition(): The data is NULL!!"));
    }
}

void StateMachineUI::returnToWidget(){
    setCurrentIndex(MAIN_WIDGET);
}

void StateMachineUI::connectToTableWidgets(GenericTableWidget *generators, GenericTableWidget *variables, GenericTableWidget *events){
    if (generators && variables && events){
        disconnect(events, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(generators, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(events, SIGNAL(elementSelected(int,QString)), this, SLOT(eventTableElementSelected(int,QString)), Qt::UniqueConnection);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(variableTableElementSelected(int,QString)), Qt::UniqueConnection);
        connect(generators, SIGNAL(elementSelected(int,QString)), this, SLOT(generatorTableElementSelected(int,QString)), Qt::UniqueConnection);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::connectToTableWidgets(): One or more arguments are NULL!!"));
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
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::renameVariable(): The data is NULL!!"));
    }
}

void StateMachineUI::eventRenamed(const QString & name, int index){
    if (bsData){
        eventUI->eventRenamed(name, index);
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
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::eventRenamed(): The data is NULL!!"));
    }
}

void StateMachineUI::setBehaviorView(BehaviorGraphView *view){
    behaviorView = view;
    setCurrentIndex(MAIN_WIDGET);
    stateUI->setBehaviorView(view);
}

