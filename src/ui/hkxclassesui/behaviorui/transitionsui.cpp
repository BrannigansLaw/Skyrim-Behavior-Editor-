#include "transitionsui.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/hkbexpressioncondition.h"
#include "src/hkxclasses/behavior/hkbblendingtransitioneffect.h"
#include "src/hkxclasses/behavior/generators/hkbstatemachine.h"
#include "src/hkxclasses/behavior/generators/hkbstatemachinestateinfo.h"
#include "src/hkxclasses/behavior/hkbstatemachinetransitioninfoarray.h"
#include "src/hkxclasses/behavior/hkbstringeventpayload.h"
#include "src/hkxclasses/behavior/hkbstringcondition.h"
#include "src/ui/genericdatawidgets.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/behaviorgraphview.h"
#include "src/ui/treegraphicsitem.h"
#include "src/ui/hkxclassesui/behaviorui/blendingtransitioneffectui.h"
#include "src/filetypes/behaviorfile.h"

#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 22

#define TRIGGER_INTERVAL_ENTER_EVENT_ID_ROW 0
#define TRIGGER_INTERVAL_EXIT_EVENT_ID_ROW 1
#define TRIGGER_INTERVAL_ENTER_TIME_ROW 2
#define TRIGGER_INTERVAL_EXIT_TIME_ROW 3
#define INITIATE_INTERVAL_ENTER_EVENT_ID_ROW 4
#define INITIATE_INTERVAL_EXIT_EVENT_ID_ROW 5
#define INITIATE_INTERVAL_ENTER_TIME_ROW 6
#define INITIATE_INTERVAL_EXIT_TIME_ROW 7
#define TRANSITION_ROW 8
#define CONDITION_ROW 9
#define EVENT_ID_ROW 10
#define TO_STATE_ID_ROW 11
#define FROM_NESTED_STATE_ID_ROW 12
#define TO_NESTED_STATE_ID_ROW 13
#define PRIORITY_ROW 14
#define FLAG_GLOBAL_WILDCARD_ROW 15
#define FLAG_LOCAL_WILDCARD_ROW 16
#define FLAG_USE_NESTED_STATE_ROW 17
#define FLAG_ALLOW_SELF_TRANSITION_ROW 18
#define FLAG_DISALLOW_RANDOM_TRANSITION_ROW 19
#define FLAG_DISALLOW_RETURN_TO_PREVIOUS_STATE_ROW 20
#define FLAG_ABUT_END_STATE_ROW 21

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define VALUE_COLUMN 2

QStringList TransitionsUI::headerLabels = {
    "Name",
    "Type",
    "Value"
};

TransitionsUI::TransitionsUI()
    : file(nullptr),
      bsData(nullptr),
      transitionIndex(-1),
      transitionUI(new BlendingTransitionEffectUI()),
      returnPB(new QPushButton("Return")),
      topLyt(new QGridLayout),
      groupBox(new QGroupBox("hkbStateMachineTransitionInfoArray")),
      table(new TableWidget(QColor(Qt::white))),
      enterTimeTI(new DoubleSpinBox),
      exitTimeTI(new DoubleSpinBox),
      enterTimeII(new DoubleSpinBox),
      exitTimeII(new DoubleSpinBox),
      transition(new CheckButtonCombo("Edit")),
      condition(new ConditionLineEdit),
      toStateId(new ComboBox),
      fromNestedStateId(new ComboBox),
      toNestedStateId(new ComboBox),
      priority(new SpinBox),
      flagGlobalWildcard(new CheckBox),
      flagLocalWildcard(new CheckBox),
      flagUseNestedState(new CheckBox),
      flagAllowSelfTransition(new CheckBox),
      flagDisallowRandomTransition(new CheckBox),
      flagDisallowReturnToState(new CheckBox),
      flagAbutEndState(new CheckBox)
{
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(TRIGGER_INTERVAL_ENTER_EVENT_ID_ROW, NAME_COLUMN, new TableWidgetItem("Trigger Interval: Enter Event Id"));
    table->setItem(TRIGGER_INTERVAL_ENTER_EVENT_ID_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(TRIGGER_INTERVAL_ENTER_EVENT_ID_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_EVENTS_TABLE_TIP));
    table->setItem(TRIGGER_INTERVAL_EXIT_EVENT_ID_ROW, NAME_COLUMN, new TableWidgetItem("Trigger Interval: Exit Event Id"));
    table->setItem(TRIGGER_INTERVAL_EXIT_EVENT_ID_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(TRIGGER_INTERVAL_EXIT_EVENT_ID_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_EVENTS_TABLE_TIP));
    table->setItem(TRIGGER_INTERVAL_ENTER_TIME_ROW, NAME_COLUMN, new TableWidgetItem("Trigger Interval: Enter Time"));
    table->setItem(TRIGGER_INTERVAL_ENTER_TIME_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setCellWidget(TRIGGER_INTERVAL_ENTER_TIME_ROW, VALUE_COLUMN, enterTimeTI);
    table->setItem(TRIGGER_INTERVAL_EXIT_TIME_ROW, NAME_COLUMN, new TableWidgetItem("Trigger Interval: Exit Time"));
    table->setItem(TRIGGER_INTERVAL_EXIT_TIME_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setCellWidget(TRIGGER_INTERVAL_EXIT_TIME_ROW, VALUE_COLUMN, exitTimeTI);
    table->setItem(INITIATE_INTERVAL_ENTER_EVENT_ID_ROW, NAME_COLUMN, new TableWidgetItem("Initiate Interval: Enter Event Id"));
    table->setItem(INITIATE_INTERVAL_ENTER_EVENT_ID_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(INITIATE_INTERVAL_ENTER_EVENT_ID_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_EVENTS_TABLE_TIP));
    table->setItem(INITIATE_INTERVAL_EXIT_EVENT_ID_ROW, NAME_COLUMN, new TableWidgetItem("Initiate Interval: Exit Event Id"));
    table->setItem(INITIATE_INTERVAL_EXIT_EVENT_ID_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(INITIATE_INTERVAL_EXIT_EVENT_ID_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_EVENTS_TABLE_TIP));
    table->setItem(INITIATE_INTERVAL_ENTER_TIME_ROW, NAME_COLUMN, new TableWidgetItem("Initiate Interval: Enter Time"));
    table->setItem(INITIATE_INTERVAL_ENTER_TIME_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setCellWidget(INITIATE_INTERVAL_ENTER_TIME_ROW, VALUE_COLUMN, enterTimeII);
    table->setItem(INITIATE_INTERVAL_EXIT_TIME_ROW, NAME_COLUMN, new TableWidgetItem("Initiate Interval: Exit Time"));
    table->setItem(INITIATE_INTERVAL_EXIT_TIME_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setCellWidget(INITIATE_INTERVAL_EXIT_TIME_ROW, VALUE_COLUMN, exitTimeII);
    table->setItem(TRANSITION_ROW, NAME_COLUMN, new TableWidgetItem("transition"));
    table->setItem(TRANSITION_ROW, TYPE_COLUMN, new TableWidgetItem("hkbTransitionEffect", Qt::AlignCenter));
    table->setItem(TRANSITION_ROW, VALUE_COLUMN, new TableWidgetItem("", Qt::AlignCenter, QColor(Qt::gray)));
    table->setCellWidget(TRANSITION_ROW, VALUE_COLUMN, transition);
    table->setItem(CONDITION_ROW, NAME_COLUMN, new TableWidgetItem("condition"));
    table->setItem(CONDITION_ROW, TYPE_COLUMN, new TableWidgetItem("hkbStringCondition", Qt::AlignCenter));
    table->setCellWidget(CONDITION_ROW, VALUE_COLUMN, condition);
    table->setItem(EVENT_ID_ROW, NAME_COLUMN, new TableWidgetItem("eventId"));
    table->setItem(EVENT_ID_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(EVENT_ID_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_EVENTS_TABLE_TIP));
    table->setItem(TO_STATE_ID_ROW, NAME_COLUMN, new TableWidgetItem("toStateId"));
    table->setItem(TO_STATE_ID_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setCellWidget(TO_STATE_ID_ROW, VALUE_COLUMN, toStateId);
    table->setItem(FROM_NESTED_STATE_ID_ROW, NAME_COLUMN, new TableWidgetItem("fromNestedStateId"));
    table->setItem(FROM_NESTED_STATE_ID_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setCellWidget(FROM_NESTED_STATE_ID_ROW, VALUE_COLUMN, fromNestedStateId);
    table->setItem(TO_NESTED_STATE_ID_ROW, NAME_COLUMN, new TableWidgetItem("toNestedStateId"));
    table->setItem(TO_NESTED_STATE_ID_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setCellWidget(TO_NESTED_STATE_ID_ROW, VALUE_COLUMN, toNestedStateId);
    table->setItem(PRIORITY_ROW, NAME_COLUMN, new TableWidgetItem("priority"));
    table->setItem(PRIORITY_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setCellWidget(PRIORITY_ROW, VALUE_COLUMN, priority);
    table->setItem(FLAG_GLOBAL_WILDCARD_ROW, NAME_COLUMN, new TableWidgetItem("flagGlobalWildcard"));
    table->setItem(FLAG_GLOBAL_WILDCARD_ROW, TYPE_COLUMN, new TableWidgetItem("TransitionFlag", Qt::AlignCenter));
    table->setCellWidget(FLAG_GLOBAL_WILDCARD_ROW, VALUE_COLUMN, flagGlobalWildcard);
    table->setItem(FLAG_LOCAL_WILDCARD_ROW, NAME_COLUMN, new TableWidgetItem("flagLocalWildcard"));
    table->setItem(FLAG_LOCAL_WILDCARD_ROW, TYPE_COLUMN, new TableWidgetItem("TransitionFlag", Qt::AlignCenter));
    table->setCellWidget(FLAG_LOCAL_WILDCARD_ROW, VALUE_COLUMN, flagLocalWildcard);
    table->setItem(FLAG_USE_NESTED_STATE_ROW, NAME_COLUMN, new TableWidgetItem("flagUseNestedState"));
    table->setItem(FLAG_USE_NESTED_STATE_ROW, TYPE_COLUMN, new TableWidgetItem("TransitionFlag", Qt::AlignCenter));
    table->setCellWidget(FLAG_USE_NESTED_STATE_ROW, VALUE_COLUMN, flagUseNestedState);
    table->setItem(FLAG_ALLOW_SELF_TRANSITION_ROW, NAME_COLUMN, new TableWidgetItem("flagAllowSelfTransition"));
    table->setItem(FLAG_ALLOW_SELF_TRANSITION_ROW, TYPE_COLUMN, new TableWidgetItem("TransitionFlag", Qt::AlignCenter));
    table->setCellWidget(FLAG_ALLOW_SELF_TRANSITION_ROW, VALUE_COLUMN, flagAllowSelfTransition);
    table->setItem(FLAG_DISALLOW_RANDOM_TRANSITION_ROW, NAME_COLUMN, new TableWidgetItem("flagDisallowRandomTransition"));
    table->setItem(FLAG_DISALLOW_RANDOM_TRANSITION_ROW, TYPE_COLUMN, new TableWidgetItem("TransitionFlag", Qt::AlignCenter));
    table->setCellWidget(FLAG_DISALLOW_RANDOM_TRANSITION_ROW, VALUE_COLUMN, flagDisallowRandomTransition);
    table->setItem(FLAG_DISALLOW_RETURN_TO_PREVIOUS_STATE_ROW, NAME_COLUMN, new TableWidgetItem("flagDisallowReturnToState"));
    table->setItem(FLAG_DISALLOW_RETURN_TO_PREVIOUS_STATE_ROW, TYPE_COLUMN, new TableWidgetItem("TransitionFlag", Qt::AlignCenter));
    table->setCellWidget(FLAG_DISALLOW_RETURN_TO_PREVIOUS_STATE_ROW, VALUE_COLUMN, flagDisallowReturnToState);
    table->setItem(FLAG_ABUT_END_STATE_ROW, NAME_COLUMN, new TableWidgetItem("flagAbutEndState"));
    table->setItem(FLAG_ABUT_END_STATE_ROW, TYPE_COLUMN, new TableWidgetItem("TransitionFlag", Qt::AlignCenter));
    table->setCellWidget(FLAG_ABUT_END_STATE_ROW, VALUE_COLUMN, flagAbutEndState);
    topLyt->addWidget(returnPB, 0, 1, 1, 1);
    topLyt->addWidget(table, 1, 0, 8, 3);
    groupBox->setLayout(topLyt);
    //Order here must correspond with the ACTIVE_WIDGET Enumerated type!!!
    addWidget(groupBox);
    addWidget(transitionUI);
    connectSignals();
    fromNestedStateId->setDisabled(true);
    toNestedStateId->setDisabled(true);
}

void TransitionsUI::connectSignals(){
    connect(returnPB, SIGNAL(released()), this, SIGNAL(returnToParent()), Qt::UniqueConnection);
    connect(enterTimeTI, SIGNAL(editingFinished()), this, SLOT(setTriggerIntervalEnterTime()), Qt::UniqueConnection);
    connect(exitTimeTI, SIGNAL(editingFinished()), this, SLOT(setTriggerIntervalExitTime()), Qt::UniqueConnection);
    connect(enterTimeII, SIGNAL(editingFinished()), this, SLOT(setInitiateIntervalEnterTime()), Qt::UniqueConnection);
    connect(exitTimeII, SIGNAL(editingFinished()), this, SLOT(setInitiateIntervalExitTime()), Qt::UniqueConnection);
    connect(transition, SIGNAL(pressed()), this, SLOT(viewTransitionEffect()), Qt::UniqueConnection);
    connect(transition, SIGNAL(enabled(bool)), this, SLOT(toggleTransitionEffect(bool)), Qt::UniqueConnection);
    connect(condition, SIGNAL(editingFinished()), this, SLOT(setCondition()), Qt::UniqueConnection);
    connect(toStateId, SIGNAL(currentIndexChanged(QString)), this, SLOT(setToStateId(QString)), Qt::UniqueConnection);
    connect(fromNestedStateId, SIGNAL(currentIndexChanged(QString)), this, SLOT(setFromNestedStateId(QString)), Qt::UniqueConnection);
    connect(toNestedStateId, SIGNAL(currentIndexChanged(QString)), this, SLOT(setToNestedStateId(QString)), Qt::UniqueConnection);
    connect(priority, SIGNAL(editingFinished()), this, SLOT(setPriority()), Qt::UniqueConnection);
    connect(flagGlobalWildcard, SIGNAL(released()), this, SLOT(toggleGlobalWildcardFlag()), Qt::UniqueConnection);
    connect(flagLocalWildcard, SIGNAL(released()), this, SLOT(toggleLocalWildcardFlag()), Qt::UniqueConnection);
    connect(flagUseNestedState, SIGNAL(released()), this, SLOT(toggleUseNestedStateFlag()), Qt::UniqueConnection);
    connect(flagAllowSelfTransition, SIGNAL(released()), this, SLOT(toggleAllowSelfTransition()), Qt::UniqueConnection);
    connect(flagDisallowRandomTransition, SIGNAL(released()), this, SLOT(toggleDisallowRandomTransitionFlag()), Qt::UniqueConnection);
    connect(flagDisallowReturnToState, SIGNAL(released()), this, SLOT(toggleDisallowReturnToStateFlag()), Qt::UniqueConnection);
    connect(flagAbutEndState, SIGNAL(released()), this, SLOT(toggleAbutEndStateFlag()), Qt::UniqueConnection);
    connect(transitionUI, SIGNAL(viewVariables(int,QString,QStringList)), this, SIGNAL(viewVariables(int,QString,QStringList)), Qt::UniqueConnection);
    connect(transitionUI, SIGNAL(viewProperties(int,QString,QStringList)), this, SIGNAL(viewProperties(int,QString,QStringList)), Qt::UniqueConnection);
    connect(transitionUI, SIGNAL(transitionEffectRenamed(QString)), this, SLOT(transitionEffectRenamed(QString)), Qt::UniqueConnection);
    connect(transitionUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)), Qt::UniqueConnection);
}

void TransitionsUI::disconnectSignals(){
    disconnect(returnPB, SIGNAL(released()), this, SIGNAL(returnToParent()));
    disconnect(enterTimeTI, SIGNAL(editingFinished()), this, SLOT(setTriggerIntervalEnterTime()));
    disconnect(exitTimeTI, SIGNAL(editingFinished()), this, SLOT(setTriggerIntervalExitTime()));
    disconnect(enterTimeII, SIGNAL(editingFinished()), this, SLOT(setInitiateIntervalEnterTime()));
    disconnect(exitTimeII, SIGNAL(editingFinished()), this, SLOT(setInitiateIntervalExitTime()));
    disconnect(transition, SIGNAL(pressed()), this, SLOT(viewTransitionEffect()));
    disconnect(transition, SIGNAL(enabled(bool)), this, SLOT(toggleTransitionEffect(bool)));
    disconnect(condition, SIGNAL(editingFinished()), this, SLOT(setCondition()));
    disconnect(toStateId, SIGNAL(currentIndexChanged(QString)), this, SLOT(setToStateId(QString)));
    disconnect(fromNestedStateId, SIGNAL(currentIndexChanged(QString)), this, SLOT(setFromNestedStateId(QString)));
    disconnect(toNestedStateId, SIGNAL(currentIndexChanged(QString)), this, SLOT(setToNestedStateId(QString)));
    disconnect(priority, SIGNAL(editingFinished()), this, SLOT(setPriority()));
    disconnect(flagGlobalWildcard, SIGNAL(released()), this, SLOT(toggleGlobalWildcardFlag()));
    disconnect(flagLocalWildcard, SIGNAL(released()), this, SLOT(toggleLocalWildcardFlag()));
    disconnect(flagUseNestedState, SIGNAL(released()), this, SLOT(toggleUseNestedStateFlag()));
    disconnect(flagAllowSelfTransition, SIGNAL(released()), this, SLOT(toggleAllowSelfTransition()));
    disconnect(flagDisallowRandomTransition, SIGNAL(released()), this, SLOT(toggleDisallowRandomTransitionFlag()));
    disconnect(flagDisallowReturnToState, SIGNAL(released()), this, SLOT(toggleDisallowReturnToStateFlag()));
    disconnect(flagAbutEndState, SIGNAL(released()), this, SLOT(toggleAbutEndStateFlag()));
    disconnect(transitionUI, SIGNAL(viewVariables(int,QString,QStringList)), this, SIGNAL(viewVariables(int,QString,QStringList)));
    disconnect(transitionUI, SIGNAL(viewProperties(int,QString,QStringList)), this, SIGNAL(viewProperties(int,QString,QStringList)));
    disconnect(transitionUI, SIGNAL(transitionEffectRenamed(QString)), this, SLOT(transitionEffectRenamed(QString)));
    disconnect(transitionUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)));
}

void TransitionsUI::loadData(BehaviorFile *parentfile, hkbStateMachine *parent, hkbStateMachineTransitionInfoArray::HkTransition *data, int index){
    disconnectSignals();
    if (data && parent && parentfile){
        bsData = (hkbStateMachineTransitionInfoArray::HkTransition *)data;
        parentObj = parent;
        file = parentfile;
        transitionIndex = index;
        QString name = file->getEventNameAt(bsData->triggerInterval.enterEventId);
        loadTableValue(TRIGGER_INTERVAL_ENTER_EVENT_ID_ROW, name);
        name = file->getEventNameAt(bsData->triggerInterval.exitEventId);
        loadTableValue(TRIGGER_INTERVAL_EXIT_EVENT_ID_ROW, name);
        enterTimeTI->setValue(bsData->triggerInterval.enterTime);
        exitTimeTI->setValue(bsData->triggerInterval.exitTime);
        name = file->getEventNameAt(bsData->initiateInterval.enterEventId);
        loadTableValue(INITIATE_INTERVAL_ENTER_EVENT_ID_ROW, name);
        name = file->getEventNameAt(bsData->initiateInterval.exitEventId);
        loadTableValue(INITIATE_INTERVAL_EXIT_EVENT_ID_ROW, name);
        enterTimeII->setValue(bsData->initiateInterval.enterTime);
        exitTimeII->setValue(bsData->initiateInterval.exitTime);
        if (bsData->transition.data()){
            transition->setChecked(true);
            transition->setText(static_cast<hkbBlendingTransitionEffect *>(bsData->transition.data())->getName());
        }else{
            transition->setChecked(false);
            transition->setText("nullptr");
        }
        if (bsData->condition.data()){
            condition->setText(static_cast<hkbStringCondition *>(bsData->condition.data())->conditionString);
        }else{
            condition->setText("");
        }
        name = file->getEventNameAt(bsData->eventId);
        loadTableValue(EVENT_ID_ROW, name);
        toStateId->clear();
        toStateId->addItems(parentObj->getStateNames());
        toStateId->setCurrentIndex(parentObj->getStateNames().indexOf(parentObj->getStateName(bsData->toStateId)));
        //fromNestedStateId->clear();
        //fromNestedStateId->addItems(parentObj->getStateNames());
        toNestedStateId->clear();
        toNestedStateId->addItems(parentObj->getNestedStateNames(bsData->toStateId));
        toNestedStateId->setCurrentIndex(parentObj->getNestedStateNames(bsData->toStateId).indexOf(parentObj->getNestedStateName(bsData->toStateId, bsData->toNestedStateId)));
        priority->setValue(bsData->priority);
        QStringList flags = bsData->flags.split("|");
        flagGlobalWildcard->setChecked(false);
        flagUseNestedState->setChecked(false);
        flagDisallowRandomTransition->setChecked(false);
        flagDisallowReturnToState->setChecked(false);
        flagAbutEndState->setChecked(false);
        toNestedStateId->setDisabled(true);
        if (flags.isEmpty()){
            if (bsData->flags == "FLAG_IS_GLOBAL_WILDCARD"){
                flagGlobalWildcard->setChecked(true);
            }else if (bsData->flags == "FLAG_IS_LOCAL_WILDCARD"){
                flagLocalWildcard->setChecked(true);
            }else if (bsData->flags == "FLAG_TO_NESTED_STATE_ID_IS_VALID"){
                flagUseNestedState->setChecked(true);
                toNestedStateId->setDisabled(false);
            }else if (bsData->flags == "FLAG_ALLOW_SELF_TRANSITION_BY_TRANSITION_FROM_ANY_STATE"){
                flagAllowSelfTransition->setChecked(true);
            }else if (bsData->flags == "FLAG_DISALLOW_RANDOM_TRANSITION"){
                flagDisallowRandomTransition->setChecked(true);
            }else if (bsData->flags == "FLAG_DISALLOW_RETURN_TO_PREVIOUS_STATE"){
                flagDisallowReturnToState->setChecked(true);
            }else if (bsData->flags == "FLAG_ABUT_AT_END_OF_FROM_GENERATOR"){
                flagAbutEndState->setChecked(true);
            }
        }else{
            for (int i = 0; i < flags.size(); i++){
                if (flags.at(i) == "FLAG_IS_GLOBAL_WILDCARD"){
                    flagGlobalWildcard->setChecked(true);
                }else if (flags.at(i) == "FLAG_IS_LOCAL_WILDCARD"){
                    flagLocalWildcard->setChecked(true);
                }else if (flags.at(i) == "FLAG_TO_NESTED_STATE_ID_IS_VALID"){
                    flagUseNestedState->setChecked(true);
                    toNestedStateId->setDisabled(false);
                }else if (flags.at(i) == "FLAG_ALLOW_SELF_TRANSITION_BY_TRANSITION_FROM_ANY_STATE"){
                    flagAllowSelfTransition->setChecked(true);
                }else if (flags.at(i) == "FLAG_DISALLOW_RANDOM_TRANSITION"){
                    flagDisallowRandomTransition->setChecked(true);
                }else if (flags.at(i) == "FLAG_DISALLOW_RETURN_TO_PREVIOUS_STATE"){
                    flagDisallowReturnToState->setChecked(true);
                }else if (flags.at(i) == "FLAG_ABUT_AT_END_OF_FROM_GENERATOR"){
                    flagAbutEndState->setChecked(true);
                }
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE("TransitionsUI::loadData(): The data, parent file or parent state machine is nullptr!!");
    }
    connectSignals();
}

void TransitionsUI::setTriggerIntervalEnterEventId(int index, const QString &name){
    if (bsData){
        bsData->triggerInterval.enterEventId = index - 1;
        table->item(TRIGGER_INTERVAL_ENTER_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        if (bsData->usingTriggerInterval()){
            if (!bsData->flags.contains("FLAG_USE_TRIGGER_INTERVAL")){
                if (bsData->flags != "0"){
                    bsData->flags.append("|FLAG_USE_TRIGGER_INTERVAL");
                }else{
                    bsData->flags = ("FLAG_USE_TRIGGER_INTERVAL");
                }
            }
        }else{
            bsData->flags.remove("FLAG_USE_TRIGGER_INTERVAL");
            auto lastindex = bsData->flags.lastIndexOf("|");
            if (lastindex > bsData->flags.size() - 1){
                bsData->flags.remove(lastindex, lastindex + 1);
            }
        }
        parentObj->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("TransitionsUI::setTriggerIntervalEnterEventId(): The data is nullptr!!");
    }
}

void TransitionsUI::setTriggerIntervalExitEventId(int index, const QString &name){
    if (bsData){
        bsData->triggerInterval.exitEventId = index - 1;
        table->item(TRIGGER_INTERVAL_EXIT_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        if (bsData->usingTriggerInterval()){
            if (!bsData->flags.contains("FLAG_USE_TRIGGER_INTERVAL")){
                if (bsData->flags != "0"){
                    bsData->flags.append("|FLAG_USE_TRIGGER_INTERVAL");
                }else{
                    bsData->flags = ("FLAG_USE_TRIGGER_INTERVAL");
                }
            }
        }else{
            bsData->flags.remove("FLAG_USE_TRIGGER_INTERVAL");
            auto lastindex = bsData->flags.lastIndexOf("|");
            if (lastindex > bsData->flags.size() - 1){
                bsData->flags.remove(lastindex, lastindex + 1);
            }
        }
        parentObj->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("TransitionsUI::setTriggerIntervalExitEventId(): The data is nullptr!!");
    }
}

void TransitionsUI::setTriggerIntervalEnterTime(){
    if (bsData){
        bsData->triggerInterval.enterTime = enterTimeTI->value();
        if (bsData->usingTriggerInterval()){
            if (!bsData->flags.contains("FLAG_USE_TRIGGER_INTERVAL")){
                if (bsData->flags != "0"){
                    bsData->flags.append("|FLAG_USE_TRIGGER_INTERVAL");
                }else{
                    bsData->flags = ("FLAG_USE_TRIGGER_INTERVAL");
                }
            }
        }else{
            bsData->flags.remove("FLAG_USE_TRIGGER_INTERVAL");
            auto lastindex = bsData->flags.lastIndexOf("|");
            if (lastindex > bsData->flags.size() - 1){
                bsData->flags.remove(lastindex, lastindex + 1);
            }
        }
        parentObj->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("TransitionsUI::setTriggerIntervalEnterTime(): The data is nullptr!!");
    }
}

void TransitionsUI::setTriggerIntervalExitTime(){
    if (bsData){
        bsData->triggerInterval.exitTime = exitTimeTI->value();
        if (bsData->usingTriggerInterval()){
            if (!bsData->flags.contains("FLAG_USE_TRIGGER_INTERVAL")){
                if (bsData->flags != "0"){
                    bsData->flags.append("|FLAG_USE_TRIGGER_INTERVAL");
                }else{
                    bsData->flags = ("FLAG_USE_TRIGGER_INTERVAL");
                }
            }
        }else{
            bsData->flags.remove("FLAG_USE_TRIGGER_INTERVAL");
            auto lastindex = bsData->flags.lastIndexOf("|");
            if (lastindex > bsData->flags.size() - 1){
                bsData->flags.remove(lastindex, lastindex + 1);
            }
        }
        parentObj->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("TransitionsUI::setTriggerIntervalExitTime(): The data is nullptr!!");
    }
}

void TransitionsUI::setInitiateIntervalEnterEventId(int index, const QString &name){
    if (bsData){
        bsData->initiateInterval.enterEventId = index - 1;
        table->item(INITIATE_INTERVAL_ENTER_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        if (bsData->usingTriggerInterval()){
            if (!bsData->flags.contains("FLAG_USE_INITIATE_INTERVAL")){
                if (bsData->flags != "0"){
                    bsData->flags.append("|FLAG_USE_INITIATE_INTERVAL");
                }else{
                    bsData->flags = ("FLAG_USE_INITIATE_INTERVAL");
                }
            }
        }else{
            bsData->flags.remove("FLAG_USE_INITIATE_INTERVAL");
            auto lastindex = bsData->flags.lastIndexOf("|");
            if (lastindex > bsData->flags.size() - 1){
                bsData->flags.remove(lastindex, lastindex + 1);
            }
        }
        parentObj->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("TransitionsUI::setInitiateIntervalEnterEventId(): The data is nullptr!!");
    }
}

void TransitionsUI::setInitiateIntervalExitEventId(int index, const QString &name){
    if (bsData){
        bsData->initiateInterval.exitEventId = index - 1;
        table->item(INITIATE_INTERVAL_EXIT_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        if (bsData->usingTriggerInterval()){
            if (!bsData->flags.contains("FLAG_USE_INITIATE_INTERVAL")){
                if (bsData->flags != "0"){
                    bsData->flags.append("|FLAG_USE_INITIATE_INTERVAL");
                }else{
                    bsData->flags = ("FLAG_USE_INITIATE_INTERVAL");
                }
            }
        }else{
            bsData->flags.remove("FLAG_USE_INITIATE_INTERVAL");
            auto lastindex = bsData->flags.lastIndexOf("|");
            if (lastindex > bsData->flags.size() - 1){
                bsData->flags.remove(lastindex, lastindex + 1);
            }
        }
        parentObj->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("TransitionsUI::setInitiateIntervalExitEventId(): The data is nullptr!!");
    }
}

void TransitionsUI::setInitiateIntervalEnterTime(){
    if (bsData){
        bsData->initiateInterval.enterTime = enterTimeII->value();
        if (bsData->usingTriggerInterval()){
            if (!bsData->flags.contains("FLAG_USE_INITIATE_INTERVAL")){
                if (bsData->flags != "0"){
                    bsData->flags.append("|FLAG_USE_INITIATE_INTERVAL");
                }else{
                    bsData->flags = ("FLAG_USE_INITIATE_INTERVAL");
                }
            }
        }else{
            bsData->flags.remove("FLAG_USE_INITIATE_INTERVAL");
            auto lastindex = bsData->flags.lastIndexOf("|");
            if (lastindex > bsData->flags.size() - 1){
                bsData->flags.remove(lastindex, lastindex + 1);
            }
        }
        parentObj->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("TransitionsUI::setInitiateIntervalEnterTime(): The data is nullptr!!");
    }
}

void TransitionsUI::setInitiateIntervalExitTime(){
    if (bsData){
        bsData->initiateInterval.exitTime = exitTimeII->value();
        if (bsData->usingTriggerInterval()){
            if (!bsData->flags.contains("FLAG_USE_INITIATE_INTERVAL")){
                if (bsData->flags != "0"){
                    bsData->flags.append("|FLAG_USE_INITIATE_INTERVAL");
                }else{
                    bsData->flags = ("FLAG_USE_INITIATE_INTERVAL");
                }
            }
        }else{
            bsData->flags.remove("FLAG_USE_INITIATE_INTERVAL");
            auto lastindex = bsData->flags.lastIndexOf("|");
            if (lastindex > bsData->flags.size() - 1){
                bsData->flags.remove(lastindex, lastindex + 1);
            }
        }
        parentObj->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("TransitionsUI::setInitiateIntervalExitTime(): The data is nullptr!!");
    }
}

void TransitionsUI::viewTransitionEffect(){
    if (bsData){
        transitionUI->loadData(bsData->transition.data());
        setCurrentIndex(TRANSITION_EFFECT_WIDGET);
    }else{
        CRITICAL_ERROR_MESSAGE("TransitionsUI::viewTransitionEffect(): The data is nullptr!!");
    }
}

void TransitionsUI::toggleTransitionEffect(bool enable){
    if (bsData){
        if (!enable){
            bsData->transition = HkxSharedPtr();
            static_cast<BehaviorFile *>(parentObj->getParentFile())->removeOtherData();
        }else if (!bsData->transition.data()){
            hkbBlendingTransitionEffect *effect = new hkbBlendingTransitionEffect(parentObj->getParentFile());
            parentObj->getParentFile()->addObjectToFile(effect, -1);
            bsData->transition = HkxSharedPtr(effect);
            transition->setText(effect->getName());
        }
    }else{
        CRITICAL_ERROR_MESSAGE("TransitionsUI::toggleTransitionEffect(): The data is nullptr!!");
    }
}

void TransitionsUI::setCondition(){
    if (bsData && parentObj){
        if (condition->text() != ""){
            if (bsData->condition.data()){
                static_cast<hkbStringCondition *>(bsData->condition.data())->conditionString = condition->text();
            }else{
                hkbStringCondition *con = new hkbStringCondition(static_cast<BehaviorFile *>(parentObj->getParentFile()), condition->text());
                parentObj->getParentFile()->addObjectToFile(con, -1);
                bsData->condition = HkxSharedPtr(con);
            }
        }else{
            bsData->condition = HkxSharedPtr();
            if (!bsData->flags.contains("FLAG_DISABLE_CONDITION")){
                if (bsData->flags == "0"){
                    bsData->flags = "FLAG_DISABLE_CONDITION";
                }else{
                    bsData->flags.append("|FLAG_DISABLE_CONDITION");
                }
            }
        }
        parentObj->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("TransitionsUI::setCondition(): The data or parent object is nullptr!!");
    }
}

void TransitionsUI::setEventId(int index, const QString &name){
    if (bsData){
        bsData->eventId = index - 1;
        if (name != ""){
            table->item(EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        }else{
            CRITICAL_ERROR_MESSAGE("TransitionsUI::setEventId(): The event name is nullptr!!");
        }
        parentObj->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("TransitionsUI::setEventId(): The data is nullptr!!");
    }
}

void TransitionsUI::setToStateId(const QString &name){
    if (bsData && parentObj){
        if (name != ""){
            bsData->toStateId = parentObj->getStateId(name);
            if (flagUseNestedState->isChecked()){
                disconnect(toNestedStateId, SIGNAL(currentIndexChanged(QString)), this, SLOT(setToNestedStateId(QString)));
                toNestedStateId->clear();
                toNestedStateId->addItems(parentObj->getNestedStateNames(bsData->toStateId));
                toNestedStateId->setCurrentIndex(parentObj->getNestedStateNames(bsData->toStateId).indexOf(parentObj->getNestedStateName(bsData->toStateId, bsData->toNestedStateId)));
                connect(toNestedStateId, SIGNAL(currentIndexChanged(QString)), this, SLOT(setToNestedStateId(QString)), Qt::UniqueConnection);
                emit transitionNamChanged("toNestedState_"+toNestedStateId->currentText(), transitionIndex);
            }else{
                emit transitionNamChanged("to_"+name, transitionIndex);
            }
        }else{
            CRITICAL_ERROR_MESSAGE("TransitionsUI::setToStateId(): The event name is nullptr!!");
        }
        parentObj->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("TransitionsUI::setToStateId(): The data or parent object is nullptr!!");
    }
}

void TransitionsUI::setFromNestedStateId(const QString &name){
    if (bsData && parentObj){
        if (name != ""){
            bsData->fromNestedStateId = parentObj->getNestedStateId(name, bsData->toStateId);
        }else{
            CRITICAL_ERROR_MESSAGE("TransitionsUI::setFromNestedStateId(): The event name is nullptr!!");
        }
        parentObj->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("TransitionsUI::setFromNestedStateId(): The data or parent object is nullptr!!");
    }
}

void TransitionsUI::setToNestedStateId(const QString &name){
    if (bsData && parentObj){
        if (name != ""){
            bsData->toNestedStateId = parentObj->getNestedStateId(name, bsData->toStateId);
            emit transitionNamChanged("toNestedState_"+toNestedStateId->currentText(), transitionIndex);
        }else{
            CRITICAL_ERROR_MESSAGE("TransitionsUI::setToNestedStateId(): The event name is nullptr!!");
        }
        parentObj->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("TransitionsUI::setToNestedStateId(): The data or parent object is nullptr!!");
    }
}

void TransitionsUI::setPriority(){
    if (bsData){
        bsData->priority = priority->value();
        parentObj->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("TransitionsUI::setPriority(): The data is nullptr!!");
    }
}

void TransitionsUI::toggleGlobalWildcardFlag(){
    if (bsData){
        if (flagGlobalWildcard->isChecked()){
            if (bsData->flags == "0"){
                bsData->flags = "FLAG_IS_GLOBAL_WILDCARD";
            }else if (!bsData->flags.contains("FLAG_IS_GLOBAL_WILDCARD")){
                bsData->flags.append("|FLAG_IS_GLOBAL_WILDCARD");
            }
        }else{
            if (bsData->flags == "FLAG_IS_GLOBAL_WILDCARD"){
                bsData->flags = "0";
            }else{
                bsData->flags.remove("|FLAG_IS_GLOBAL_WILDCARD");
            }
        }
        parentObj->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("TransitionsUI::toggleGlobalWildcardFlag(): The data is nullptr!!");
    }
}

void TransitionsUI::toggleLocalWildcardFlag(){
    if (bsData){
        if (flagLocalWildcard->isChecked()){
            if (bsData->flags == "0"){
                bsData->flags = "FLAG_IS_LOCAL_WILDCARD";
            }else if (!bsData->flags.contains("FLAG_IS_LOCAL_WILDCARD")){
                bsData->flags.append("|FLAG_IS_LOCAL_WILDCARD");
            }
        }else{
            if (bsData->flags == "FLAG_IS_LOCAL_WILDCARD"){
                bsData->flags = "0";
            }else{
                bsData->flags.remove("|FLAG_IS_LOCAL_WILDCARD");
            }
        }
        parentObj->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("TransitionsUI::toggleLocalWildcardFlag(): The data is nullptr!!");
    }
}

void TransitionsUI::toggleUseNestedStateFlag(){
    if (bsData){
        if (flagUseNestedState->isChecked()){
            if (bsData->flags == "0"){
                bsData->flags = "FLAG_TO_NESTED_STATE_ID_IS_VALID";
            }else if (!bsData->flags.contains("FLAG_TO_NESTED_STATE_ID_IS_VALID")){
                bsData->flags.append("|FLAG_TO_NESTED_STATE_ID_IS_VALID");
            }
            toNestedStateId->setDisabled(false);
        }else{
            if (bsData->flags == "FLAG_TO_NESTED_STATE_ID_IS_VALID"){
                bsData->flags = "0";
            }else{
                bsData->flags.remove("|FLAG_TO_NESTED_STATE_ID_IS_VALID");
            }
            toNestedStateId->setDisabled(true);
        }
        parentObj->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("TransitionsUI::toggleUseNestedStateFlag(): The data is nullptr!!");
    }
}

void TransitionsUI::toggleAllowSelfTransition(){
    if (bsData){
        if (flagUseNestedState->isChecked()){
            if (bsData->flags == "0"){
                bsData->flags = "FLAG_ALLOW_SELF_TRANSITION_BY_TRANSITION_FROM_ANY_STATE";
            }else if (!bsData->flags.contains("FLAG_ALLOW_SELF_TRANSITION_BY_TRANSITION_FROM_ANY_STATE")){
                bsData->flags.append("|FLAG_ALLOW_SELF_TRANSITION_BY_TRANSITION_FROM_ANY_STATE");
            }
            toNestedStateId->setDisabled(false);
        }else{
            if (bsData->flags == "FLAG_ALLOW_SELF_TRANSITION_BY_TRANSITION_FROM_ANY_STATE"){
                bsData->flags = "0";
            }else{
                bsData->flags.remove("|FLAG_ALLOW_SELF_TRANSITION_BY_TRANSITION_FROM_ANY_STATE");
            }
            toNestedStateId->setDisabled(true);
        }
        parentObj->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("TransitionsUI::toggleAllowSelfTransition(): The data is nullptr!!");
    }
}

void TransitionsUI::toggleDisallowRandomTransitionFlag(){
    if (bsData){
        if (flagDisallowRandomTransition->isChecked()){
            if (bsData->flags == "0"){
                bsData->flags = "FLAG_DISALLOW_RANDOM_TRANSITION";
            }else if (!bsData->flags.contains("FLAG_DISALLOW_RANDOM_TRANSITION")){
                bsData->flags.append("|FLAG_DISALLOW_RANDOM_TRANSITION");
            }
        }else{
            if (bsData->flags == "FLAG_DISALLOW_RANDOM_TRANSITION"){
                bsData->flags = "0";
            }else{
                bsData->flags.remove("|FLAG_DISALLOW_RANDOM_TRANSITION");
            }
        }
        parentObj->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("TransitionsUI::toggleDisallowRandomTransitionFlag(): The data is nullptr!!");
    }
}

void TransitionsUI::toggleDisallowReturnToStateFlag(){
    if (bsData){
        if (flagDisallowReturnToState->isChecked()){
            if (bsData->flags == "0"){
                bsData->flags = "FLAG_DISALLOW_RETURN_TO_PREVIOUS_STATE";
            }else if (!bsData->flags.contains("FLAG_DISALLOW_RETURN_TO_PREVIOUS_STATE")){
                bsData->flags.append("|FLAG_DISALLOW_RETURN_TO_PREVIOUS_STATE");
            }
        }else{
            if (bsData->flags == "FLAG_DISALLOW_RETURN_TO_PREVIOUS_STATE"){
                bsData->flags = "0";
            }else{
                bsData->flags.remove("|FLAG_DISALLOW_RETURN_TO_PREVIOUS_STATE");
            }
        }
        parentObj->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("TransitionsUI::toggleDisallowReturnToStateFlag(): The data is nullptr!!");
    }
}

void TransitionsUI::toggleAbutEndStateFlag(){
    if (bsData){
        if (flagAbutEndState->isChecked()){
            if (bsData->flags == "0"){
                bsData->flags = "FLAG_ABUT_AT_END_OF_FROM_GENERATOR";
            }else if (!bsData->flags.contains("FLAG_ABUT_AT_END_OF_FROM_GENERATOR")){
                bsData->flags.append("|FLAG_ABUT_AT_END_OF_FROM_GENERATOR");
            }
        }else{
            if (bsData->flags == "FLAG_ABUT_AT_END_OF_FROM_GENERATOR"){
                bsData->flags = "0";
            }else{
                bsData->flags.remove("|FLAG_ABUT_AT_END_OF_FROM_GENERATOR");
            }
        }
        parentObj->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("TransitionsUI::toggleAbutEndStateFlag(): The data is nullptr!!");
    }
}

void TransitionsUI::eventTableElementSelected(int index, const QString &name){
    if (bsData){
        switch (table->currentRow()){
        case TRIGGER_INTERVAL_ENTER_EVENT_ID_ROW:
            setTriggerIntervalEnterEventId(index, name);
            break;
        case TRIGGER_INTERVAL_EXIT_EVENT_ID_ROW:
            setTriggerIntervalExitEventId(index, name);
            break;
        case INITIATE_INTERVAL_ENTER_EVENT_ID_ROW:
            setInitiateIntervalEnterEventId(index, name);
            break;
        case INITIATE_INTERVAL_EXIT_EVENT_ID_ROW:
            setInitiateIntervalExitEventId(index, name);
            break;
        case EVENT_ID_ROW:
            setEventId(index, name);
            break;
        default:
            CRITICAL_ERROR_MESSAGE("TransitionsUI::eventTableElementSelected(): Event relayed to wrong table row!!");
            return;
        }
    }else{
        CRITICAL_ERROR_MESSAGE("TransitionsUI::eventTableElementSelected(): The data is nullptr!!");
    }
}

void TransitionsUI::variableTableElementSelected(int index, const QString &name){
    if (bsData){
        switch (currentIndex()){
        case TRANSITION_EFFECT_WIDGET:
            transitionUI->setBindingVariable(index, name);
            break;
        default:
            CRITICAL_ERROR_MESSAGE("TransitionsUI::variableTableElementSelected(): Event relayed to wrong widget!!");
            return;
        }
    }else{
        CRITICAL_ERROR_MESSAGE("TransitionsUI::variableTableElementSelected(): The data is nullptr!!");
    }
}

void TransitionsUI::returnToWidget(){
    setCurrentIndex(TRANSITION_WIDGET);
}

void TransitionsUI::viewSelectedChild(int row, int column){
    int index = 0;
    if (bsData){
        if (row < BASE_NUMBER_OF_ROWS && row >= 0){
            if (column == VALUE_COLUMN){
                switch (row){
                case TRIGGER_INTERVAL_ENTER_EVENT_ID_ROW:
                    index = bsData->triggerInterval.enterEventId;
                    break;
                case TRIGGER_INTERVAL_EXIT_EVENT_ID_ROW:
                    index = bsData->triggerInterval.exitEventId;
                    break;
                case INITIATE_INTERVAL_ENTER_EVENT_ID_ROW:
                    index = bsData->initiateInterval.enterEventId;
                    break;
                case INITIATE_INTERVAL_EXIT_EVENT_ID_ROW:
                    index = bsData->initiateInterval.exitEventId;
                    break;
                case EVENT_ID_ROW:
                    index = bsData->eventId;
                    break;
                default:
                    return;
                }
                emit viewEvents(index + 1, QString(), QStringList());
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BlendingTransitionEffectUI::viewSelectedChild(): The data is nullptr!!");
    }
}

void TransitionsUI::transitionEffectRenamed(const QString &name){
    if (name != ""){
        transition->setText(name);
    }else{
        CRITICAL_ERROR_MESSAGE("TransitionsUI::transitionEffectRenamed(): The name the empty string!!");
    }
}

void TransitionsUI::loadTableValue(int row, const QString &value){
    if (table->item(row, VALUE_COLUMN)){
        if (value != ""){
            table->item(row, VALUE_COLUMN)->setText(value);
        }else{
            table->item(row, VALUE_COLUMN)->setText("NONE");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("TransitionsUI::loadTableValue(): There is no table item here!!");
    }
}

void TransitionsUI::eventRenamed(const QString &name, int index){
    if (bsData){
        if (index == bsData->triggerInterval.enterEventId){
            table->item(TRIGGER_INTERVAL_ENTER_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        }
        if (index == bsData->triggerInterval.exitEventId){
            table->item(TRIGGER_INTERVAL_EXIT_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        }
        if (index == bsData->initiateInterval.enterEventId){
            table->item(INITIATE_INTERVAL_ENTER_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        }
        if (index == bsData->initiateInterval.exitEventId){
            table->item(INITIATE_INTERVAL_EXIT_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        }
        if (index == bsData->eventId){
            table->item(EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("TransitionsUI::eventRenamed(): The data is nullptr!!");
    }
}
