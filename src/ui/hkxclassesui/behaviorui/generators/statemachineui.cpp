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
#include "src/ui/mainwindow.h"

#include <QHeaderView>

#include "src/ui/genericdatawidgets.h"

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
#define ADD_STATE_ROW 12
#define INITIAL_ADD_TRANSITION_ROW 13

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

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
    "hkbBehaviorReferenceGenerator",
    "BGSGamebryoSequenceGenerator"
};

QStringList StateMachineUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

StateMachineUI::StateMachineUI()
    : transitionsButtonRow(INITIAL_ADD_TRANSITION_ROW),
      behaviorView(nullptr),
      bsData(nullptr),
      groupBox(new QGroupBox("hkbStateMachine")),
      topLyt(new QGridLayout),
      eventToSendWhenStateOrTransitionChanges(new CheckButtonCombo("Click to edit")),
      eventUI(new EventUI),
      stateUI(new StateUI),
      transitionUI(new TransitionsUI),
      table(new TableWidget(QColor(Qt::white))),
      typeSelectorCB(new ComboBox),
      name(new LineEdit),
      startStateId(new ComboBox),
      wrapAroundStateId(new CheckBox),
      maxSimultaneousTransitions(new SpinBox),
      startStateMode(new ComboBox),
      selfTransitionMode(new ComboBox)
{
    table->setAcceptDrops(true);
    table->viewport()->setAcceptDrops(true);
    table->setDragDropOverwriteMode(true);
    table->setDropIndicatorShown(true);
    table->setDragDropMode(QAbstractItemView::InternalMove);
    table->setRowSwapRange(INITIAL_ADD_TRANSITION_ROW);
    typeSelectorCB->insertItems(0, types);
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(NAME_ROW, NAME_COLUMN, new TableWidgetItem("name"));
    table->setItem(NAME_ROW, TYPE_COLUMN, new TableWidgetItem("hkStringPtr", Qt::AlignCenter));
    table->setItem(NAME_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(NAME_ROW, VALUE_COLUMN, name);
    table->setItem(EVENT_TO_SEND_WHEN_STATE_OR_TRANSITION_CHANGES_ROW, NAME_COLUMN, new TableWidgetItem("eventToSendWhenStateOrTransitionChanges"));
    table->setItem(EVENT_TO_SEND_WHEN_STATE_OR_TRANSITION_CHANGES_ROW, TYPE_COLUMN, new TableWidgetItem("hkEvent", Qt::AlignCenter));
    table->setItem(EVENT_TO_SEND_WHEN_STATE_OR_TRANSITION_CHANGES_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(EVENT_TO_SEND_WHEN_STATE_OR_TRANSITION_CHANGES_ROW, VALUE_COLUMN, new TableWidgetItem("", Qt::AlignCenter, QColor(Qt::gray)));
    table->setCellWidget(EVENT_TO_SEND_WHEN_STATE_OR_TRANSITION_CHANGES_ROW, VALUE_COLUMN, eventToSendWhenStateOrTransitionChanges);
    table->setItem(START_STATE_ID_ROW, NAME_COLUMN, new TableWidgetItem("startStateId"));
    table->setItem(START_STATE_ID_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(START_STATE_ID_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(START_STATE_ID_ROW, VALUE_COLUMN, startStateId);
    table->setItem(RETURN_TO_PREVIOUS_STATE_EVENT_ID_ROW, NAME_COLUMN, new TableWidgetItem("returnToPreviousStateEventId"));
    table->setItem(RETURN_TO_PREVIOUS_STATE_EVENT_ID_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(RETURN_TO_PREVIOUS_STATE_EVENT_ID_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(RETURN_TO_PREVIOUS_STATE_EVENT_ID_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_EVENTS_TABLE_TIP));
    table->setItem(RANDOM_TRANSITION_EVENT_ID_ROW, NAME_COLUMN, new TableWidgetItem("randomTransitionEventId"));
    table->setItem(RANDOM_TRANSITION_EVENT_ID_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(RANDOM_TRANSITION_EVENT_ID_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(RANDOM_TRANSITION_EVENT_ID_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_EVENTS_TABLE_TIP));
    table->setItem(TRANSITION_TO_NEXT_HIGHER_STATE_EVENT_ID_ROW, NAME_COLUMN, new TableWidgetItem("transitionToNextHigherStateEventId"));
    table->setItem(TRANSITION_TO_NEXT_HIGHER_STATE_EVENT_ID_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(TRANSITION_TO_NEXT_HIGHER_STATE_EVENT_ID_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(TRANSITION_TO_NEXT_HIGHER_STATE_EVENT_ID_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_EVENTS_TABLE_TIP));
    table->setItem(TRANSITION_TO_NEXT_LOWER_STATE_EVENT_ID_ROW, NAME_COLUMN, new TableWidgetItem("transitionToNextLowerStateEventId"));
    table->setItem(TRANSITION_TO_NEXT_LOWER_STATE_EVENT_ID_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(TRANSITION_TO_NEXT_LOWER_STATE_EVENT_ID_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(TRANSITION_TO_NEXT_LOWER_STATE_EVENT_ID_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_EVENTS_TABLE_TIP));
    table->setItem(SYNC_VARIABLE_INDEX_ROW, NAME_COLUMN, new TableWidgetItem("syncVariableIndex"));
    table->setItem(SYNC_VARIABLE_INDEX_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(SYNC_VARIABLE_INDEX_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setItem(SYNC_VARIABLE_INDEX_ROW, VALUE_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(WRAP_AROUND_STATE_ID_ROW, NAME_COLUMN, new TableWidgetItem("wrapAroundStateId"));
    table->setItem(WRAP_AROUND_STATE_ID_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(WRAP_AROUND_STATE_ID_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setItem(WRAP_AROUND_STATE_ID_ROW, VALUE_COLUMN, new TableWidgetItem("", Qt::AlignCenter, QColor(Qt::lightGray)));
    table->setCellWidget(WRAP_AROUND_STATE_ID_ROW, VALUE_COLUMN, wrapAroundStateId);
    table->setItem(MAX_SIMULTANEOUS_TRANSITIONS_ROW, NAME_COLUMN, new TableWidgetItem("maxSimultaneousTransitions"));
    table->setItem(MAX_SIMULTANEOUS_TRANSITIONS_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(MAX_SIMULTANEOUS_TRANSITIONS_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(MAX_SIMULTANEOUS_TRANSITIONS_ROW, VALUE_COLUMN, maxSimultaneousTransitions);
    maxSimultaneousTransitions->setMaximum(32);
    maxSimultaneousTransitions->setMinimum(0);
    table->setItem(START_STATE_MODE_ROW, NAME_COLUMN, new TableWidgetItem("startStateMode"));
    table->setItem(START_STATE_MODE_ROW, TYPE_COLUMN, new TableWidgetItem("StartStateMode", Qt::AlignCenter));
    table->setItem(START_STATE_MODE_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(START_STATE_MODE_ROW, VALUE_COLUMN, startStateMode);
    table->setItem(SELF_TRANSITION_MODE_ROW, NAME_COLUMN, new TableWidgetItem("selfTransitionMode"));
    table->setItem(SELF_TRANSITION_MODE_ROW, TYPE_COLUMN, new TableWidgetItem("SelfTransitionMode", Qt::AlignCenter));
    table->setItem(SELF_TRANSITION_MODE_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(SELF_TRANSITION_MODE_ROW, VALUE_COLUMN, selfTransitionMode);
    table->setItem(ADD_STATE_ROW, NAME_COLUMN, new TableWidgetItem("Add State With Generator", Qt::AlignCenter, QColor(Qt::green), QBrush(Qt::black), "Double click to add a new state with a generator of the type specified in the adjacent combo box"));
    table->setCellWidget(ADD_STATE_ROW, TYPE_COLUMN, typeSelectorCB);
    table->setItem(ADD_STATE_ROW, BINDING_COLUMN, new TableWidgetItem("Remove Selected State", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black), "Double click to remove the selected state"));
    table->setItem(ADD_STATE_ROW, VALUE_COLUMN, new TableWidgetItem("Edit Selected State", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black), "Double click to edit the selected state"));
    table->setItem(INITIAL_ADD_TRANSITION_ROW, NAME_COLUMN, new TableWidgetItem("Add Transition", Qt::AlignCenter, QColor(Qt::green), QBrush(Qt::black), "Double click to add a new transition"));
    table->setItem(INITIAL_ADD_TRANSITION_ROW, TYPE_COLUMN, new TableWidgetItem("hkbStateMachineTransitionInfoArray", Qt::AlignCenter, QColor(Qt::gray)));
    table->setItem(INITIAL_ADD_TRANSITION_ROW, BINDING_COLUMN, new TableWidgetItem("Remove Selected Transition", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black), "Double click to remove the selected transition"));
    table->setItem(INITIAL_ADD_TRANSITION_ROW, VALUE_COLUMN, new TableWidgetItem("Edit Selected Transition", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black), "Double click to edit the selected transition"));
    topLyt->addWidget(table, 1, 0, 8, 3);
    groupBox->setLayout(topLyt);
    //Order here must correspond with the ACTIVE_WIDGET Enumerated type!!!
    addWidget(groupBox);
    addWidget(stateUI);
    addWidget(transitionUI);
    addWidget(eventUI);
    connectSignals();
}

void StateMachineUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(wrapAroundStateId, SIGNAL(clicked(bool)), this, SLOT(setWrapAroundStateId(bool)), Qt::UniqueConnection);
    connect(maxSimultaneousTransitions, SIGNAL(editingFinished()), this, SLOT(setMaxSimultaneousTransitions()), Qt::UniqueConnection);
    connect(startStateId, SIGNAL(currentIndexChanged(int)), this, SLOT(setStartStateId(int)), Qt::UniqueConnection);
    connect(startStateMode, SIGNAL(currentIndexChanged(int)), this, SLOT(setStartStateMode(int)), Qt::UniqueConnection);
    connect(selfTransitionMode, SIGNAL(currentIndexChanged(int)), this, SLOT(setSelfTransitionMode(int)), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)), Qt::UniqueConnection);
    connect(table, SIGNAL(itemDropped(int,int)), this, SLOT(swapGeneratorIndices(int,int)), Qt::UniqueConnection);
    connect(eventToSendWhenStateOrTransitionChanges, SIGNAL(pressed()), this, SLOT(viewEventToSendWhenStateOrTransitionChanges()), Qt::UniqueConnection);
    connect(eventToSendWhenStateOrTransitionChanges, SIGNAL(enabled(bool)), this, SLOT(toggleEventToSendWhenStateOrTransitionChanges(bool)), Qt::UniqueConnection);
    connect(eventUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()), Qt::UniqueConnection);
    connect(eventUI, SIGNAL(viewEvents(int)), this, SIGNAL(viewEvents(int)), Qt::UniqueConnection);
    connect(stateUI, SIGNAL(viewEvents(int)), this, SIGNAL(viewEvents(int)), Qt::UniqueConnection);
    connect(stateUI, SIGNAL(viewVariables(int)), this, SIGNAL(viewVariables(int)), Qt::UniqueConnection);
    connect(stateUI, SIGNAL(viewProperties(int)), this, SIGNAL(viewProperties(int)), Qt::UniqueConnection);
    connect(stateUI, SIGNAL(returnToParent(bool)), this, SLOT(returnToWidget(bool)), Qt::UniqueConnection);
    connect(stateUI, SIGNAL(viewGenerators(int)), this, SIGNAL(viewGenerators(int)), Qt::UniqueConnection);
    connect(stateUI, SIGNAL(stateNameChanged(QString,int)), this, SLOT(stateRenamed(QString,int)), Qt::UniqueConnection);
    connect(stateUI, SIGNAL(stateIdChanged(int,int,QString)), this, SLOT(setStateIDForRow(int,int,QString)), Qt::UniqueConnection);
    connect(transitionUI, SIGNAL(transitionNamChanged(QString,int)), this, SLOT(transitionRenamed(QString,int)), Qt::UniqueConnection);
    connect(transitionUI, SIGNAL(viewVariables(int)), this, SIGNAL(viewVariables(int)), Qt::UniqueConnection);
    connect(transitionUI, SIGNAL(viewProperties(int)), this, SIGNAL(viewProperties(int)), Qt::UniqueConnection);
    connect(transitionUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()), Qt::UniqueConnection);
    connect(transitionUI, SIGNAL(viewEvents(int)), this, SIGNAL(viewEvents(int)), Qt::UniqueConnection);
}

void StateMachineUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(wrapAroundStateId, SIGNAL(clicked(bool)), this, SLOT(setWrapAroundStateId(bool)));
    disconnect(maxSimultaneousTransitions, SIGNAL(editingFinished()), this, SLOT(setMaxSimultaneousTransitions()));
    disconnect(startStateId, SIGNAL(currentIndexChanged(int)), this, SLOT(setStartStateId(int)));
    disconnect(startStateMode, SIGNAL(currentIndexChanged(int)), this, SLOT(setStartStateMode(int)));
    disconnect(selfTransitionMode, SIGNAL(currentIndexChanged(int)), this, SLOT(setSelfTransitionMode(int)));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)));
    disconnect(table, SIGNAL(itemDropped(int,int)), this, SLOT(swapGeneratorIndices(int,int)));
    disconnect(eventToSendWhenStateOrTransitionChanges, SIGNAL(pressed()), this, SLOT(viewEventToSendWhenStateOrTransitionChanges()));
    disconnect(eventToSendWhenStateOrTransitionChanges, SIGNAL(enabled(bool)), this, SLOT(toggleEventToSendWhenStateOrTransitionChanges(bool)));
    disconnect(eventUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()));
    disconnect(eventUI, SIGNAL(viewEvents(int)), this, SIGNAL(viewEvents(int)));
    disconnect(stateUI, SIGNAL(viewEvents(int)), this, SIGNAL(viewEvents(int)));
    disconnect(stateUI, SIGNAL(viewVariables(int)), this, SIGNAL(viewVariables(int)));
    disconnect(stateUI, SIGNAL(viewProperties(int)), this, SIGNAL(viewProperties(int)));
    disconnect(stateUI, SIGNAL(returnToParent(bool)), this, SLOT(returnToWidget(bool)));
    disconnect(stateUI, SIGNAL(viewGenerators(int)), this, SIGNAL(viewGenerators(int)));
    disconnect(stateUI, SIGNAL(stateNameChanged(QString,int)), this, SLOT(stateRenamed(QString,int)));
    disconnect(stateUI, SIGNAL(stateIdChanged(int,int,QString)), this, SLOT(setStateIDForRow(int,int,QString)));
    disconnect(transitionUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()));
    disconnect(transitionUI, SIGNAL(viewVariables(int)), this, SIGNAL(viewVariables(int)));
    disconnect(transitionUI, SIGNAL(viewProperties(int)), this, SIGNAL(viewProperties(int)));
    disconnect(transitionUI, SIGNAL(viewEvents(int)), this, SIGNAL(viewEvents(int)));
    disconnect(transitionUI, SIGNAL(transitionNamChanged(QString,int)), this, SLOT(transitionRenamed(QString,int)));
}

void StateMachineUI::loadData(HkxObject *data){
    disconnectSignals();
    setCurrentIndex(MAIN_WIDGET);
    if (data && data->getSignature() == HKB_STATE_MACHINE){
        hkbVariableBindingSet *varBind = nullptr;
        QString varName;
        QStringList statenames;
        //statenames.append("None");
        bsData = static_cast<hkbStateMachine *>(data);
        statenames = statenames + bsData->getStateNames();
        name->setText(bsData->name);
        if (bsData->eventToSendWhenStateOrTransitionChanges.id > -1){
            eventToSendWhenStateOrTransitionChanges->setChecked(true);
        }else{
            eventToSendWhenStateOrTransitionChanges->setChecked(false);
        }
        startStateId->clear();
        startStateId->insertItems(0, statenames);
        startStateId->setCurrentIndex(startStateId->findText(bsData->getStateName(bsData->startStateId), Qt::MatchCaseSensitive));
        varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (varBind){
            loadBinding(START_STATE_ID_ROW, BINDING_COLUMN, varBind, "startStateId");
            loadBinding(SYNC_VARIABLE_INDEX_ROW, BINDING_COLUMN, varBind, "syncVariableIndex");
            loadBinding(WRAP_AROUND_STATE_ID_ROW, BINDING_COLUMN, varBind, "wrapAroundStateId");
        }else{
            table->item(START_STATE_ID_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(SYNC_VARIABLE_INDEX_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(WRAP_AROUND_STATE_ID_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
        }
        varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getEventNameAt(bsData->returnToPreviousStateEventId);
        loadTableValue(RETURN_TO_PREVIOUS_STATE_EVENT_ID_ROW, varName);
        varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getEventNameAt(bsData->randomTransitionEventId);
        loadTableValue(RANDOM_TRANSITION_EVENT_ID_ROW, varName);
        varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getEventNameAt(bsData->transitionToNextHigherStateEventId);
        loadTableValue(TRANSITION_TO_NEXT_HIGHER_STATE_EVENT_ID_ROW, varName);
        varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getEventNameAt(bsData->transitionToNextLowerStateEventId);
        loadTableValue(TRANSITION_TO_NEXT_LOWER_STATE_EVENT_ID_ROW, varName);
        varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableNameAt(bsData->syncVariableIndex);
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
        FATAL_RUNTIME_ERROR("StateMachineUI::loadData(): The data is nullptr or an incorrect type!!");
    }
    connectSignals();
}

void StateMachineUI::loadDynamicTableRows(){
    //table->setSortingEnabled(false);//Not sure...
    if (bsData){
        int temp = ADD_STATE_ROW + bsData->getNumberOfStates() + 1 - transitionsButtonRow;
        if (temp > 0){
            for (int i = 0; i < temp; i++){
                table->insertRow(transitionsButtonRow);
                transitionsButtonRow++;
            }
        }else if (temp < 0){
            for (int i = temp; i < 0; i++){
                table->removeRow(transitionsButtonRow - 1);
                transitionsButtonRow--;
            }
        }
        transitionsButtonRow = ADD_STATE_ROW + bsData->getNumberOfStates() + 1;
        hkbStateMachineStateInfo *state = nullptr;
        for (int i = INITIAL_ADD_TRANSITION_ROW, j = 0; i < transitionsButtonRow, j < bsData->getNumberOfStates(); i++, j++){
            state = static_cast<hkbStateMachineStateInfo *>(bsData->states.at(j).data());
            if (state){
                setRowItems(i, state->getName()+" ->ID: "+QString::number(state->stateId), state->getClassname(), "Remove", "Edit", "Double click to remove this state", "Double click to edit this state");
            }else{
                FATAL_RUNTIME_ERROR("StateMachineUI::loadData(): Null state found!!!");
            }
        }
        hkbStateMachineTransitionInfoArray *trans = static_cast<hkbStateMachineTransitionInfoArray *>(bsData->wildcardTransitions.data());
        if (trans){
            table->setRowCount(transitionsButtonRow + trans->getNumTransitions() + 1);
            for (int i = transitionsButtonRow + 1, j = 0; i < table->rowCount(), j < trans->getNumTransitions(); i++, j++){
                setRowItems(i, trans->getTransitionNameAt(j), trans->getClassname(), "Remove", "Edit", "Double click to remove this transition", "Double click to edit this transition");
            }
        }else{
            table->setRowCount(transitionsButtonRow + 1);
        }
    }else{
        FATAL_RUNTIME_ERROR("StateMachineUI::loadDynamicTableRows(): The data is nullptr!!");
    }
    //table->setSortingEnabled(true);
}

void StateMachineUI::setRowItems(int row, const QString & name, const QString & classname, const QString & bind, const QString & value, const QString & tip1, const QString & tip2){
    if (table->item(row, NAME_COLUMN)){
        table->item(row, NAME_COLUMN)->setText(name);
    }else{
        table->setItem(row, NAME_COLUMN, new TableWidgetItem(name));
    }
    if (table->item(row, TYPE_COLUMN)){
        table->item(row, TYPE_COLUMN)->setText(classname);
    }else{
        table->setItem(row, TYPE_COLUMN, new TableWidgetItem(classname, Qt::AlignCenter));
    }
    if (table->item(row, BINDING_COLUMN)){
        table->item(row, BINDING_COLUMN)->setText(bind);
    }else{
        table->setItem(row, BINDING_COLUMN, new TableWidgetItem(bind, Qt::AlignCenter, QColor(Qt::red), QBrush(Qt::black), tip1));
    }
    if (table->item(row, VALUE_COLUMN)){
        table->item(row, VALUE_COLUMN)->setText(value);
    }else{
        table->setItem(row, VALUE_COLUMN, new TableWidgetItem(value, Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), tip2));
    }
}

void StateMachineUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            bsData->getParentFile()->setIsChanged(true);
            emit generatorNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData));
        }
    }else{
        FATAL_RUNTIME_ERROR("StateMachineUI::setName(): The data is nullptr!!");
    }
}

void StateMachineUI::setReturnToPreviousStateEventId(int index, const QString & name){
    if (bsData){
        bsData->returnToPreviousStateEventId = index - 1;
        table->item(RETURN_TO_PREVIOUS_STATE_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        bsData->getParentFile()->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("StateMachineUI::setReturnToPreviousStateEventId(): The data is nullptr!!");
    }
}

void StateMachineUI::setStartStateId(int index){
    if (bsData && index < bsData->states.size()&& index > -1){
        bsData->startStateId = static_cast<hkbStateMachineStateInfo *>(bsData->states.at(index).data())->stateId;
        bsData->getParentFile()->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("StateMachineUI::setStartStateId(): The data is nullptr!!");
    }
}

void StateMachineUI::setRandomTransitionEventId(int index, const QString & name){
    if (bsData){
        bsData->randomTransitionEventId = index - 1;
        table->item(RANDOM_TRANSITION_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        bsData->getParentFile()->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("StateMachineUI::setRandomTransitionEventId(): The data is nullptr!!");
    }
}

void StateMachineUI::setTransitionToNextHigherStateEventId(int index, const QString &name){
    if (bsData){
        bsData->transitionToNextHigherStateEventId = index - 1;
        table->item(TRANSITION_TO_NEXT_HIGHER_STATE_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        bsData->getParentFile()->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("StateMachineUI::setTransitionToNextHigherStateEventId(): The data is nullptr!!");
    }
}

void StateMachineUI::setTransitionToNextLowerStateEventId(int index, const QString &name){
    if (bsData){
        bsData->transitionToNextLowerStateEventId = index - 1;
        table->item(TRANSITION_TO_NEXT_LOWER_STATE_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        bsData->getParentFile()->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("StateMachineUI::setTransitionToNextLowerStateEventId(): The data is nullptr!!");
    }
}

void StateMachineUI::setSyncVariableIndex(int index){
    if (bsData){
        bsData->syncVariableIndex = index - 1;
        bsData->getParentFile()->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("StateMachineUI::setSyncVariableIndex(): The data is nullptr!!");
    }
}

void StateMachineUI::setWrapAroundStateId(bool checked){
    if (bsData){
        bsData->wrapAroundStateId = checked;
        bsData->getParentFile()->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("StateMachineUI::setWrapAroundStateId(): The data is nullptr!!");
    }
}

void StateMachineUI::setMaxSimultaneousTransitions(){
    if (bsData){
        if (bsData->maxSimultaneousTransitions != maxSimultaneousTransitions->value()){
            bsData->maxSimultaneousTransitions = maxSimultaneousTransitions->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("StateMachineUI::setMaxSimultaneousTransitions(): The data is nullptr!!");
    }
}

void StateMachineUI::setStartStateMode(int index){
    if (bsData){
        bsData->startStateMode = bsData->StartStateMode.at(index);
        bsData->getParentFile()->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("StateMachineUI::setStartStateMode(): The data is nullptr!!");
    }
}

void StateMachineUI::setSelfTransitionMode(int index){
    if (bsData){
        bsData->selfTransitionMode = bsData->SelfTransitionMode.at(index);
        bsData->getParentFile()->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("StateMachineUI::setSelfTransitionMode(): The data is nullptr!!");
    }
}

void StateMachineUI::swapGeneratorIndices(int index1, int index2){
    if (bsData){
        index1 = index1 - INITIAL_ADD_TRANSITION_ROW;
        index2 = index2 - INITIAL_ADD_TRANSITION_ROW;
        if (bsData->states.size() > index1 && bsData->states.size() > index2 && index1 != index2 && index1 >= 0 && index2 >= 0){
            bsData->states.swap(index1, index2);
            if (behaviorView->getSelectedItem()){
                behaviorView->getSelectedItem()->reorderChildren();
            }else{
                FATAL_RUNTIME_ERROR("StateMachineUI::swapGeneratorIndices(): No item selected!!");
            }
            bsData->getParentFile()->setIsChanged(true);
        }else{
            WARNING_MESSAGE("StateMachineUI::swapGeneratorIndices(): Cannot swap these rows!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("StateMachineUI::swapGeneratorIndices(): The data is nullptr!!");
    }
}

void StateMachineUI::setStateIDForRow(int index, int newID, const QString &statename){
    int row = ADD_STATE_ROW + index;
    if (table->item(row, NAME_COLUMN)){
        table->item(row, NAME_COLUMN)->setText(statename+" ->ID: "+QString::number(newID));
    }else{
        FATAL_RUNTIME_ERROR("StateMachineUI::setStateIDForRow(): Unwanted state id change event heard!!");
    }
}

void StateMachineUI::setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty){
    hkbVariableBindingSet *varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
    if (bsData){
        if (index == 0){
            varBind->removeBinding(path);
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
        }else if ((!isProperty && static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableTypeAt(index - 1) == type) ||
                  (isProperty && static_cast<BehaviorFile *>(bsData->getParentFile())->getCharacterPropertyTypeAt(index - 1) == type)){
            if (!varBind){
                varBind = new hkbVariableBindingSet(bsData->getParentFile());
                bsData->variableBindingSet = HkxSharedPtr(varBind);
            }
            if (isProperty){
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY)){
                    FATAL_RUNTIME_ERROR("StateMachineUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }else{
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    FATAL_RUNTIME_ERROR("StateMachineUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->setIsChanged(true);
        }else{
            WARNING_MESSAGE("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("StateMachineUI::setBinding(): The data is nullptr!!");
    }
}

void StateMachineUI::setBindingVariable(int index, const QString & name){
    if (bsData){
        bool isProperty = false;
        int row = table->currentRow();
        switch (row){
        case START_STATE_ID_ROW:
            if (table->item(START_STATE_ID_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "startStateId", VARIABLE_TYPE_INT32, isProperty);
            break;
        case SYNC_VARIABLE_INDEX_ROW:
            if (table->item(SYNC_VARIABLE_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "syncVariableIndex", VARIABLE_TYPE_INT32, isProperty);
            break;
        case WRAP_AROUND_STATE_ID_ROW:
            if (table->item(WRAP_AROUND_STATE_ID_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "wrapAroundStateId", VARIABLE_TYPE_BOOL, isProperty);
            break;
        default:
            return;
        }
        bsData->getParentFile()->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("StateMachineUI::setBindingVariable(): The data is nullptr!!");
    }
}

void StateMachineUI::viewEventToSendWhenStateOrTransitionChanges(){
    if (bsData){
        eventUI->loadData(static_cast<BehaviorFile *>(bsData->getParentFile()), &bsData->eventToSendWhenStateOrTransitionChanges);
        setCurrentIndex(EVENT_PAYLOAD_WIDGET);
    }else{
        FATAL_RUNTIME_ERROR("StateMachineUI::viewEventToSendWhenStateOrTransitionChanges(): The data is nullptr!!");
    }
}

void StateMachineUI::toggleEventToSendWhenStateOrTransitionChanges(bool enable){
    if (bsData){
        if (!enable){
            bsData->eventToSendWhenStateOrTransitionChanges.id = -1;
            bsData->eventToSendWhenStateOrTransitionChanges.payload = HkxSharedPtr();
            static_cast<BehaviorFile *>(bsData->getParentFile())->removeOtherData();
        }
    }else{
        FATAL_RUNTIME_ERROR("StateMachineUI::toggleEventToSendWhenStateOrTransitionChanges(): The data is nullptr!!");
    }
}

void StateMachineUI::stateRenamed(const QString &name, int stateIndex){
    if (bsData){
        if (table->item(INITIAL_ADD_TRANSITION_ROW + stateIndex, NAME_COLUMN)){
            table->item(INITIAL_ADD_TRANSITION_ROW + stateIndex, NAME_COLUMN)->setText(name);
        }else{
            FATAL_RUNTIME_ERROR("StateMachineUI::stateRenamed(): Invalid row selected!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("StateMachineUI::stateRenamed(): The data is nullptr!!");
    }
}

void StateMachineUI::transitionRenamed(const QString &name, int index){
    if (bsData){
        if (table->item(transitionsButtonRow + index + 1, NAME_COLUMN)){
            table->item(transitionsButtonRow + index + 1, NAME_COLUMN)->setText(name);
        }else{
            FATAL_RUNTIME_ERROR("StateMachineUI::transitionRenamed(): Invalid row selected!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("StateMachineUI::transitionRenamed(): The data is nullptr!!");
    }
}

void StateMachineUI::selectTableToView(bool viewproperties, const QString & path){
    if (bsData){
        if (viewproperties){
            if (bsData->variableBindingSet.data()){
                emit viewProperties(static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data())->getVariableIndexOfBinding(path) + 1);
            }else{
                emit viewProperties(0);
            }
        }else{
            if (bsData->variableBindingSet.data()){
                emit viewVariables(static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data())->getVariableIndexOfBinding(path) + 1);
            }else{
                emit viewVariables(0);
            }
        }
    }else{
        FATAL_RUNTIME_ERROR("StateMachineUI::selectTableToView(): The data is nullptr!!");
    }
}

void StateMachineUI::viewSelectedChild(int row, int column){
    int result;
    int index = 0;
    bool properties = false;
    hkbStateMachineTransitionInfoArray *trans = nullptr;
    if (bsData){
        if (row < ADD_STATE_ROW && row >= 0){
            if (column == BINDING_COLUMN){
                switch (row){
                case START_STATE_ID_ROW:
                    if (table->item(START_STATE_ID_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "startStateId");
                    break;
                case SYNC_VARIABLE_INDEX_ROW:
                    if (table->item(SYNC_VARIABLE_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "syncVariableIndex");
                    break;
                case WRAP_AROUND_STATE_ID_ROW:
                    if (table->item(WRAP_AROUND_STATE_ID_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "wrapAroundStateId");
                }
            }else if (column == VALUE_COLUMN){
                if (row == EVENT_TO_SEND_WHEN_STATE_OR_TRANSITION_CHANGES_ROW){
                    eventUI->loadData(static_cast<BehaviorFile *>(bsData->getParentFile()), &bsData->eventToSendWhenStateOrTransitionChanges);
                    setCurrentIndex(EVENT_PAYLOAD_WIDGET);
                }else{
                    switch (row){
                    case EVENT_TO_SEND_WHEN_STATE_OR_TRANSITION_CHANGES_ROW:
                        index = bsData->eventToSendWhenStateOrTransitionChanges.id;
                        break;
                    case RETURN_TO_PREVIOUS_STATE_EVENT_ID_ROW:
                        index = bsData->returnToPreviousStateEventId;
                        break;
                    case RANDOM_TRANSITION_EVENT_ID_ROW:
                        index = bsData->randomTransitionEventId;
                        break;
                    case TRANSITION_TO_NEXT_HIGHER_STATE_EVENT_ID_ROW:
                        index = bsData->transitionToNextHigherStateEventId;
                        break;
                    case TRANSITION_TO_NEXT_LOWER_STATE_EVENT_ID_ROW:
                        index = bsData->transitionToNextLowerStateEventId;
                        break;
                    default:
                        return;
                    }
                    emit viewEvents(index + 1);
                }
            }
        }else if (row == ADD_STATE_ROW && column == NAME_COLUMN){
            addStateWithGenerator();
        }else if (row == transitionsButtonRow && column == NAME_COLUMN){
            addTransition();
        }else if (row > ADD_STATE_ROW && row < transitionsButtonRow){
            result = row - BASE_NUMBER_OF_ROWS + 1;
            if (bsData->getNumberOfStates() > result && result >= 0){
                if (column == VALUE_COLUMN){
                    stateUI->loadData(static_cast<hkbStateMachineStateInfo *>(bsData->states.at(result).data()), result);
                    setCurrentIndex(STATE_WIDGET);
                }else if (column == BINDING_COLUMN){
                    if (MainWindow::yesNoDialogue("Are you sure you want to remove the state \""+static_cast<hkbStateMachineStateInfo *>(bsData->states.at(result).data())->getName()+"\"?") == QMessageBox::Yes){
                        removeState(result);
                    }
                }
            }else{
                FATAL_RUNTIME_ERROR("StateMachineUI::viewSelectedChild(): Invalid index of child to view!!");
            }
        }else if (row > transitionsButtonRow && row < table->rowCount()){
            result = row - BASE_NUMBER_OF_ROWS - bsData->getNumberOfStates();
            trans = static_cast<hkbStateMachineTransitionInfoArray *>(bsData->wildcardTransitions.data());
            if (trans && result < trans->getNumTransitions() && result >= 0){
                if (column == VALUE_COLUMN){
                    transitionUI->loadData(static_cast<BehaviorFile *>(bsData->getParentFile()), bsData, &trans->transitions[result], result);
                    setCurrentIndex(TRANSITION_WIDGET);
                }else if (column == BINDING_COLUMN){
                    if (MainWindow::yesNoDialogue("Are you sure you want to remove the transition \""+trans->getTransitionNameAt(result)+"\"?") == QMessageBox::Yes){
                        removeTransition(result);
                    }
                }
            }else{
                FATAL_RUNTIME_ERROR("StateMachineUI::viewSelectedChild(): Invalid index of child to view!!");
            }
        }
    }else{
        FATAL_RUNTIME_ERROR("StateMachineUI::viewSelectedChild(): The data is nullptr!!");
    }
}

void StateMachineUI::eventTableElementSelected(int index, const QString &name){
    //index--;
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
            WARNING_MESSAGE("StateMachineUI::eventTableElementSelected(): An unwanted element selected event was recieved!!");
            return;
        }
        break;
    case EVENT_PAYLOAD_WIDGET:
        eventUI->setEvent(index, name);
        break;
    case STATE_WIDGET:
        stateUI->eventTableElementSelected(index, name);
        break;
    case TRANSITION_WIDGET:
        transitionUI->eventTableElementSelected(index, name);
        break;
    default:
        WARNING_MESSAGE("StateMachineUI::eventTableElementSelected(): An unwanted element selected event was recieved!!");
    }
}

void StateMachineUI::variableTableElementSelected(int index, const QString &name){
    switch (currentIndex()){
    case MAIN_WIDGET:
        setBindingVariable(index, name);
        break;
    case STATE_WIDGET:
        stateUI->variableTableElementSelected(index, name);
        break;
    case TRANSITION_WIDGET:
        transitionUI->variableTableElementSelected(index, name);
        break;
    default:
        WARNING_MESSAGE("StateMachineUI::variableTableElementSelected(): An unwanted element selected event was recieved!!");
    }
}

void StateMachineUI::generatorTableElementSelected(int index, const QString &name){
    switch (currentIndex()){
    case STATE_WIDGET:
        stateUI->setGenerator(index, name);
        break;
    default:
        WARNING_MESSAGE("StateMachineUI::generatorTableElementSelected(): An unwanted element selected event was recieved!!");
    }
}

void StateMachineUI::generatorRenamed(const QString &name, int index){
    switch (currentIndex()){
    case STATE_WIDGET:
        stateUI->generatorRenamed(name, index);
        break;
    default:
        WARNING_MESSAGE("StateMachineUI::generatorTableElementSelected(): An unwanted element selected event was recieved!!");
    }
}

void StateMachineUI::addStateWithGenerator(){
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
        case GAMEBYRO_SEQUENCE_GENERATOR:
            behaviorView->appendBGSGamebryoSequenceGenerator();
            break;
        default:
            FATAL_RUNTIME_ERROR("StateMachineUI::addNewStateWithGenerator(): Invalid typeEnum!!");
            return;
        }
        loadDynamicTableRows();
    }else{
        FATAL_RUNTIME_ERROR("StateMachineUI::addNewStateWithGenerator(): The data or behavior graph pointer is nullptr!!");
    }
}

void StateMachineUI::removeState(int index){
    //hkbStateMachineStateInfo *child = nullptr;
    int currentindex = -1;
    if (bsData && behaviorView){
        if (index < bsData->states.size() && index >= 0){
            //child = static_cast<hkbStateMachineStateInfo *>(bsData->states.at(index).data());
            //behaviorView->removeItemFromGraph(behaviorView->getSelectedIconsChildIcon(child->generator.data()), index);//Reorderchildren?
            behaviorView->removeItemFromGraph(behaviorView->getSelectedIconsChildIcon(bsData->states.at(index).data()), index);//Reorderchildren?
            behaviorView->removeObjects();
        }else{
            WARNING_MESSAGE("StateMachineUI::removeState(): Invalid index of child to remove!!");
        }
        disconnect(startStateId, SIGNAL(currentIndexChanged(int)), this, SLOT(setStartStateId(int)));
        currentindex = startStateId->currentIndex();
        startStateId->removeItem(index);
        if (currentindex == index){
            startStateId->setCurrentIndex(0);
        }
        connect(startStateId, SIGNAL(currentIndexChanged(int)), this, SLOT(setStartStateId(int)), Qt::UniqueConnection);
        loadDynamicTableRows();
    }else{
        FATAL_RUNTIME_ERROR("StateMachineUI::removeState(): The data or behavior graph pointer is nullptr!!");
    }
}

void StateMachineUI::removeTransition(int index){
    hkbStateMachineTransitionInfoArray *trans = nullptr;
    if (bsData){
        trans = static_cast<hkbStateMachineTransitionInfoArray *>(bsData->wildcardTransitions.data());
        if (trans){
            if (index < trans->getNumTransitions() && index >= 0){
                trans->removeTransition(index);
            }else{
                WARNING_MESSAGE("StateMachineUI::removeTransition(): Invalid row index selected!!");
                return;
            }
        }else{
            WARNING_MESSAGE("StateMachineUI::removeTransition(): Transition data is nullptr!!");
            return;
        }
        loadDynamicTableRows();
    }else{
        FATAL_RUNTIME_ERROR("StateMachineUI::removeTransition(): The data is nullptr!!");
    }
}

void StateMachineUI::addTransition(){
    hkbStateMachineTransitionInfoArray *trans = nullptr;
    if (bsData){
        trans = static_cast<hkbStateMachineTransitionInfoArray *>(bsData->wildcardTransitions.data());
        if (!trans){
            trans = new hkbStateMachineTransitionInfoArray(bsData->getParentFile(), bsData, -1);
            bsData->wildcardTransitions = HkxSharedPtr(trans);
        }
        trans->addTransition();
        loadDynamicTableRows();
    }else{
        FATAL_RUNTIME_ERROR("StateMachineUI::addTransition(): The data is nullptr!!");
    }
}

void StateMachineUI::returnToWidget(bool reloadData){
    if (reloadData){
        loadData(bsData);
    }
    setCurrentIndex(MAIN_WIDGET);
}

void StateMachineUI::returnToWidget(){
    setCurrentIndex(MAIN_WIDGET);
}

void StateMachineUI::connectToTables(GenericTableWidget *generators, GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *events){
    if (generators && variables && events && properties){
        disconnect(events, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(generators, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(events, SIGNAL(elementSelected(int,QString)), this, SLOT(eventTableElementSelected(int,QString)), Qt::UniqueConnection);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(variableTableElementSelected(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(variableTableElementSelected(int,QString)), Qt::UniqueConnection);
        connect(generators, SIGNAL(elementSelected(int,QString)), this, SLOT(generatorTableElementSelected(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewGenerators(int)), generators, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int)), variables, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int)), properties, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewEvents(int)), events, SLOT(showTable(int)), Qt::UniqueConnection);
    }else{
        FATAL_RUNTIME_ERROR("StateMachineUI::connectToTables(): One or more arguments are nullptr!!");
    }
}

void StateMachineUI::variableRenamed(const QString & name, int index){
    int bindIndex = -1;
    hkbVariableBindingSet *bind = nullptr;
    if (name == ""){
        WARNING_MESSAGE("StateMachineUI::variableRenamed(): The new variable name is the empty string!!");
    }
    if (bsData){
        index--;
        bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (bind){
            bindIndex = bind->getVariableIndexOfBinding("startStateId");
            if (bindIndex == index){
                table->item(START_STATE_ID_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+name);
            }
            bindIndex = bind->getVariableIndexOfBinding("syncVariableIndex");
            if (bindIndex == index){
                table->item(SYNC_VARIABLE_INDEX_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+name);
            }
            bindIndex = bind->getVariableIndexOfBinding("wrapAroundStateId");
            if (bindIndex == index){
                table->item(WRAP_AROUND_STATE_ID_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+name);
            }
        }
    }else{
        FATAL_RUNTIME_ERROR("StateMachineUI::variableRenamed(): The data is nullptr!!");
    }
}

void StateMachineUI::eventRenamed(const QString & name, int index){
    if (bsData){
        if (currentIndex() == MAIN_WIDGET){
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
            eventUI->eventRenamed(name, index);
        }
    }else{
        FATAL_RUNTIME_ERROR("StateMachineUI::eventRenamed(): The data is nullptr!!");
    }
}

void StateMachineUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
    if (bsData){
        if (varBind){
            int index = varBind->getVariableIndexOfBinding(path);
            QString varName;
            if (index != -1){
                if (varBind->getBindingType(path) == hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY){
                    varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getCharacterPropertyNameAt(index, true);
                    table->item(row, colunm)->setCheckState(Qt::Checked);
                }else{
                    varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableNameAt(index);
                }
            }
            if (varName == ""){
                varName = "NONE";
            }
            table->item(row, colunm)->setText(BINDING_ITEM_LABEL+varName);
        }else{
            FATAL_RUNTIME_ERROR("StateMachineUI::loadBinding(): The variable binding set is nullptr!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("StateMachineUI::loadBinding(): The data is nullptr!!");
    }
}

void StateMachineUI::loadTableValue(int row, const QString &value){
    if (table->item(row, VALUE_COLUMN)){
        if (value != ""){
            table->item(row, VALUE_COLUMN)->setText(value);
        }else{
            table->item(row, VALUE_COLUMN)->setText("NONE");
        }
    }else{
        FATAL_RUNTIME_ERROR("StateMachineUI::loadTableValue(): There is no table item here!!");
    }
}

void StateMachineUI::setBehaviorView(BehaviorGraphView *view){
    behaviorView = view;
    setCurrentIndex(MAIN_WIDGET);
    stateUI->setBehaviorView(view);
}

