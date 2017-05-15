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

#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QLineEdit>
#include <QComboBox>
#include <QHeaderView>
#include <QSpinBox>
#include <QTableWidget>
#include <QStackedLayout>

#define BASE_NUMBER_OF_ROWS 20

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
#define FLAG_USE_NESTED_STATE_ROW 16
#define FLAG_DISALLOW_RANDOM_TRANSITION_ROW 17
#define FLAG_DISALLOW_RETURN_TO_PREVIOUS_STATE_ROW 18
#define FLAG_ABUT_END_STATE_ROW 19

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define VALUE_COLUMN 2

QStringList TransitionsUI::headerLabels = {
    "Name",
    "Type",
    "Value"
};

TransitionsUI::TransitionsUI()
    : file(NULL),
      bsData(NULL),
      transition(new BlendingTransitionEffectUI()),
      returnPB(new QPushButton("Return")),
      topLyt(new QGridLayout),
      groupBox(new QGroupBox("hkbStateMachineTransitionInfoArray")),
      table(new TableWidget),
      enterTimeTI(new DoubleSpinBox),
      exitTimeTI(new DoubleSpinBox),
      enterTimeII(new DoubleSpinBox),
      exitTimeII(new DoubleSpinBox),
      condition(new ConditionLineEdit),
      toStateId(new ComboBox),
      fromNestedStateId(new ComboBox),
      toNestedStateId(new ComboBox),
      priority(new SpinBox),
      flagGlobalWildcard(new QCheckBox),
      flagUseNestedState(new QCheckBox),
      flagDisallowRandomTransition(new QCheckBox),
      flagDisallowReturnToState(new QCheckBox),
      flagAbutEndState(new QCheckBox)
{
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(TRIGGER_INTERVAL_ENTER_EVENT_ID_ROW, NAME_COLUMN, new TableWidgetItem("Trigger Interval: Enter Event"));
    table->setItem(TRIGGER_INTERVAL_ENTER_EVENT_ID_ROW, TYPE_COLUMN, new TableWidgetItem("hkEvent"));
    table->setItem(TRIGGER_INTERVAL_ENTER_EVENT_ID_ROW, VALUE_COLUMN, new TableWidgetItem("NONE"));
    table->item(TRIGGER_INTERVAL_ENTER_EVENT_ID_ROW, TYPE_COLUMN)->setTextAlignment(Qt::AlignCenter);
    table->item(TRIGGER_INTERVAL_ENTER_EVENT_ID_ROW, VALUE_COLUMN)->setTextAlignment(Qt::AlignCenter);
    table->item(TRIGGER_INTERVAL_ENTER_EVENT_ID_ROW, VALUE_COLUMN)->setBackgroundColor(QColor(Qt::lightGray));
    table->setItem(TRIGGER_INTERVAL_EXIT_EVENT_ID_ROW, NAME_COLUMN, new TableWidgetItem("Trigger Interval: Exit Event"));
    table->setItem(TRIGGER_INTERVAL_EXIT_EVENT_ID_ROW, TYPE_COLUMN, new TableWidgetItem("hkEvent"));
    table->setItem(TRIGGER_INTERVAL_EXIT_EVENT_ID_ROW, VALUE_COLUMN, new TableWidgetItem("NONE"));
    table->item(TRIGGER_INTERVAL_EXIT_EVENT_ID_ROW, TYPE_COLUMN)->setTextAlignment(Qt::AlignCenter);
    table->item(TRIGGER_INTERVAL_EXIT_EVENT_ID_ROW, VALUE_COLUMN)->setTextAlignment(Qt::AlignCenter);
    table->item(TRIGGER_INTERVAL_EXIT_EVENT_ID_ROW, VALUE_COLUMN)->setBackgroundColor(QColor(Qt::lightGray));
    table->setItem(TRIGGER_INTERVAL_ENTER_TIME_ROW, NAME_COLUMN, new TableWidgetItem("Trigger Interval: Enter Time"));
    table->setItem(TRIGGER_INTERVAL_ENTER_TIME_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal"));
    table->setCellWidget(TRIGGER_INTERVAL_ENTER_TIME_ROW, VALUE_COLUMN, enterTimeTI);
    table->item(TRIGGER_INTERVAL_ENTER_TIME_ROW, TYPE_COLUMN)->setTextAlignment(Qt::AlignCenter);
    table->setItem(TRIGGER_INTERVAL_EXIT_TIME_ROW, NAME_COLUMN, new TableWidgetItem("Trigger Interval: Exit Time"));
    table->setItem(TRIGGER_INTERVAL_EXIT_TIME_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal"));
    table->setCellWidget(TRIGGER_INTERVAL_EXIT_TIME_ROW, VALUE_COLUMN, exitTimeTI);
    table->item(TRIGGER_INTERVAL_EXIT_TIME_ROW, TYPE_COLUMN)->setTextAlignment(Qt::AlignCenter);
    table->setItem(INITIATE_INTERVAL_ENTER_EVENT_ID_ROW, NAME_COLUMN, new TableWidgetItem("Initiate Interval: Enter Event"));
    table->setItem(INITIATE_INTERVAL_ENTER_EVENT_ID_ROW, TYPE_COLUMN, new TableWidgetItem("hkEvent"));
    table->setItem(INITIATE_INTERVAL_ENTER_EVENT_ID_ROW, VALUE_COLUMN, new TableWidgetItem("NONE"));
    table->item(INITIATE_INTERVAL_ENTER_EVENT_ID_ROW, TYPE_COLUMN)->setTextAlignment(Qt::AlignCenter);
    table->item(INITIATE_INTERVAL_ENTER_EVENT_ID_ROW, VALUE_COLUMN)->setTextAlignment(Qt::AlignCenter);
    table->item(INITIATE_INTERVAL_ENTER_EVENT_ID_ROW, VALUE_COLUMN)->setBackgroundColor(QColor(Qt::lightGray));
    table->setItem(INITIATE_INTERVAL_EXIT_EVENT_ID_ROW, NAME_COLUMN, new TableWidgetItem("Initiate Interval: Exit Event"));
    table->setItem(INITIATE_INTERVAL_EXIT_EVENT_ID_ROW, TYPE_COLUMN, new TableWidgetItem("hkEvent"));
    table->setItem(INITIATE_INTERVAL_EXIT_EVENT_ID_ROW, VALUE_COLUMN, new TableWidgetItem("NONE"));
    table->item(INITIATE_INTERVAL_EXIT_EVENT_ID_ROW, TYPE_COLUMN)->setTextAlignment(Qt::AlignCenter);
    table->item(INITIATE_INTERVAL_EXIT_EVENT_ID_ROW, VALUE_COLUMN)->setTextAlignment(Qt::AlignCenter);
    table->item(INITIATE_INTERVAL_EXIT_EVENT_ID_ROW, VALUE_COLUMN)->setBackgroundColor(QColor(Qt::lightGray));
    table->setItem(INITIATE_INTERVAL_ENTER_TIME_ROW, NAME_COLUMN, new TableWidgetItem("Initiate Interval: Enter Time"));
    table->setItem(INITIATE_INTERVAL_ENTER_TIME_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal"));
    table->setCellWidget(INITIATE_INTERVAL_ENTER_TIME_ROW, VALUE_COLUMN, enterTimeII);
    table->item(INITIATE_INTERVAL_ENTER_TIME_ROW, TYPE_COLUMN)->setTextAlignment(Qt::AlignCenter);
    table->setItem(INITIATE_INTERVAL_EXIT_TIME_ROW, NAME_COLUMN, new TableWidgetItem("Initiate Interval: Exit Time"));
    table->setItem(INITIATE_INTERVAL_EXIT_TIME_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal"));
    table->setCellWidget(INITIATE_INTERVAL_EXIT_TIME_ROW, VALUE_COLUMN, exitTimeII);
    table->item(INITIATE_INTERVAL_EXIT_TIME_ROW, TYPE_COLUMN)->setTextAlignment(Qt::AlignCenter);
    table->setItem(TRANSITION_ROW, NAME_COLUMN, new TableWidgetItem("transition"));
    table->setItem(TRANSITION_ROW, TYPE_COLUMN, new TableWidgetItem("hkbTransitionEffect"));
    table->setItem(TRANSITION_ROW, VALUE_COLUMN, new TableWidgetItem("NULL"));
    table->item(TRANSITION_ROW, TYPE_COLUMN)->setTextAlignment(Qt::AlignCenter);
    table->item(TRANSITION_ROW, VALUE_COLUMN)->setTextAlignment(Qt::AlignCenter);
    table->item(TRANSITION_ROW, VALUE_COLUMN)->setBackgroundColor(QColor(Qt::lightGray));
    table->setItem(CONDITION_ROW, NAME_COLUMN, new TableWidgetItem("condition"));
    table->setItem(CONDITION_ROW, TYPE_COLUMN, new TableWidgetItem("hkbStringCondition"));
    table->setCellWidget(CONDITION_ROW, VALUE_COLUMN, condition);
    table->item(CONDITION_ROW, TYPE_COLUMN)->setTextAlignment(Qt::AlignCenter);
    table->setItem(EVENT_ID_ROW, NAME_COLUMN, new TableWidgetItem("eventId"));
    table->setItem(EVENT_ID_ROW, TYPE_COLUMN, new TableWidgetItem("hkEvent"));
    table->setItem(EVENT_ID_ROW, VALUE_COLUMN, new TableWidgetItem("NONE"));
    table->item(EVENT_ID_ROW, TYPE_COLUMN)->setTextAlignment(Qt::AlignCenter);
    table->item(EVENT_ID_ROW, VALUE_COLUMN)->setTextAlignment(Qt::AlignCenter);
    table->item(EVENT_ID_ROW, VALUE_COLUMN)->setBackgroundColor(QColor(Qt::lightGray));
    table->setItem(TO_STATE_ID_ROW, NAME_COLUMN, new TableWidgetItem("toStateId"));
    table->setItem(TO_STATE_ID_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32"));
    table->setCellWidget(TO_STATE_ID_ROW, VALUE_COLUMN, toStateId);
    table->item(TO_STATE_ID_ROW, TYPE_COLUMN)->setTextAlignment(Qt::AlignCenter);
    table->setItem(FROM_NESTED_STATE_ID_ROW, NAME_COLUMN, new TableWidgetItem("fromNestedStateId"));
    table->setItem(FROM_NESTED_STATE_ID_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32"));
    table->setCellWidget(FROM_NESTED_STATE_ID_ROW, VALUE_COLUMN, fromNestedStateId);
    table->item(FROM_NESTED_STATE_ID_ROW, TYPE_COLUMN)->setTextAlignment(Qt::AlignCenter);
    table->setItem(TO_NESTED_STATE_ID_ROW, NAME_COLUMN, new TableWidgetItem("toNestedStateId"));
    table->setItem(TO_NESTED_STATE_ID_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32"));
    table->setCellWidget(TO_NESTED_STATE_ID_ROW, VALUE_COLUMN, toNestedStateId);
    table->item(TO_NESTED_STATE_ID_ROW, TYPE_COLUMN)->setTextAlignment(Qt::AlignCenter);
    table->setItem(PRIORITY_ROW, NAME_COLUMN, new TableWidgetItem("priority"));
    table->setItem(PRIORITY_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal"));
    table->setCellWidget(PRIORITY_ROW, VALUE_COLUMN, priority);
    table->item(PRIORITY_ROW, TYPE_COLUMN)->setTextAlignment(Qt::AlignCenter);
    table->setItem(FLAG_GLOBAL_WILDCARD_ROW, NAME_COLUMN, new TableWidgetItem("flagGlobalWildcard"));
    table->setItem(FLAG_GLOBAL_WILDCARD_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32"));
    table->setCellWidget(FLAG_GLOBAL_WILDCARD_ROW, VALUE_COLUMN, flagGlobalWildcard);
    table->item(FLAG_GLOBAL_WILDCARD_ROW, TYPE_COLUMN)->setTextAlignment(Qt::AlignCenter);
    table->setItem(FLAG_USE_NESTED_STATE_ROW, NAME_COLUMN, new TableWidgetItem("flagUseNestedState"));
    table->setItem(FLAG_USE_NESTED_STATE_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32"));
    table->setCellWidget(FLAG_USE_NESTED_STATE_ROW, VALUE_COLUMN, flagUseNestedState);
    table->item(FLAG_USE_NESTED_STATE_ROW, TYPE_COLUMN)->setTextAlignment(Qt::AlignCenter);
    table->setItem(FLAG_DISALLOW_RANDOM_TRANSITION_ROW, NAME_COLUMN, new TableWidgetItem("flagDisallowRandomTransition"));
    table->setItem(FLAG_DISALLOW_RANDOM_TRANSITION_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32"));
    table->setCellWidget(FLAG_DISALLOW_RANDOM_TRANSITION_ROW, VALUE_COLUMN, flagDisallowRandomTransition);
    table->item(FLAG_DISALLOW_RANDOM_TRANSITION_ROW, TYPE_COLUMN)->setTextAlignment(Qt::AlignCenter);
    table->setItem(FLAG_DISALLOW_RETURN_TO_PREVIOUS_STATE_ROW, NAME_COLUMN, new TableWidgetItem("flagDisallowReturnToState"));
    table->setItem(FLAG_DISALLOW_RETURN_TO_PREVIOUS_STATE_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32"));
    table->setCellWidget(FLAG_DISALLOW_RETURN_TO_PREVIOUS_STATE_ROW, VALUE_COLUMN, flagDisallowReturnToState);
    table->item(FLAG_DISALLOW_RETURN_TO_PREVIOUS_STATE_ROW, TYPE_COLUMN)->setTextAlignment(Qt::AlignCenter);
    table->setItem(FLAG_ABUT_END_STATE_ROW, NAME_COLUMN, new TableWidgetItem("flagAbutEndState"));
    table->setItem(FLAG_ABUT_END_STATE_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32"));
    table->setCellWidget(FLAG_ABUT_END_STATE_ROW, VALUE_COLUMN, flagAbutEndState);
    table->item(FLAG_ABUT_END_STATE_ROW, TYPE_COLUMN)->setTextAlignment(Qt::AlignCenter);
    topLyt->addWidget(returnPB, 0, 1, 1, 1);
    topLyt->addWidget(table, 1, 0, 8, 3);
    groupBox->setLayout(topLyt);
    //Order here must correspond with the ACTIVE_WIDGET Enumerated type!!!
    addWidget(groupBox);
    addWidget(transition);
    connect(returnPB, SIGNAL(released()), this, SIGNAL(returnToParent()), Qt::UniqueConnection);
    connect(transition, SIGNAL(viewVariables(int)), this, SIGNAL(viewVariables(int)), Qt::UniqueConnection);
    connect(transition, SIGNAL(returnToParent()), this, SLOT(returnToWidget()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellClicked(int,int)), this, SLOT(viewSelectedChild(int,int)), Qt::UniqueConnection);
}

void TransitionsUI::setTriggerIntervalEnterEventId(int index, const QString &name){
    //
}

void TransitionsUI::setTriggerIntervalExitEventId(int index, const QString &name){
    //
}

void TransitionsUI::setTriggerIntervalEnterTime(){
    //
}

void TransitionsUI::setTriggerIntervalExitTime(){
    //
}

void TransitionsUI::setInitiateIntervalEnterEventId(int index, const QString &name){
    //
}

void TransitionsUI::setInitiateIntervalExitEventId(int index, const QString &name){
    //
}

void TransitionsUI::setInitiateIntervalEnterTime(){
    //
}

void TransitionsUI::setInitiateIntervalExitTime(){
    //
}

void TransitionsUI::viewTransitionEffect(){
    //
}

void TransitionsUI::setCondition(){
    //
}

void TransitionsUI::setEventId(int index, const QString &name){
    //
}

void TransitionsUI::setToStateId(const QString &name){
    //
}

void TransitionsUI::setFromNestedStateId(const QString &name){
    //
}

void TransitionsUI::setToNestedStateId(const QString &name){
    //
}

void TransitionsUI::setPriority(){
    //
}

void TransitionsUI::toggleGlobalWildcardFlag(){
    //
}

void TransitionsUI::toggleUseNestedStateFlag(){
    //
}

void TransitionsUI::toggleDisallowRandomTransitionFlag(){
    //
}

void TransitionsUI::toggleDisallowReturnToStateFlag(){
    //
}

void TransitionsUI::toggleAbutEndStateFlag(){
    //
}

void TransitionsUI::eventTableElementSelected(int index, const QString &name){
    //
}

void TransitionsUI::returnToWidget(){
    setCurrentIndex(TRANSITION_WIDGET);
}

void TransitionsUI::viewSelectedChild(int row, int column){
    int index = 0;
    if (bsData){
        if (row < BASE_NUMBER_OF_ROWS && row >= 0){
            if (column == VALUE_COLUMN){
                if (row == TRANSITION_ROW){
                    //if ()
                    transition->loadData(static_cast<hkbBlendingTransitionEffect *>(bsData->transition.data()));
                    setCurrentIndex(TRANSITION_EFFECT_WIDGET);
                }else{
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
                    default:
                        return;
                    }
                    emit viewEvents(index + 1);
                }
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlendingTransitionEffectUI::viewSelectedChild(): The data is NULL!!"));
    }
}

void TransitionsUI::loadData(BehaviorFile *parentfile, hkbStateMachine *parent, hkbStateMachineTransitionInfoArray::HkTransition *data){
    blockSignals(true);
    if (data && parent && parentfile){
        bsData = (hkbStateMachineTransitionInfoArray::HkTransition *)data;
        parentObj = parent;
        file = parentfile;
        QString name = file->getEventNameAt(bsData->triggerInterval.enterEventId);
        if (name != ""){
            table->item(TRIGGER_INTERVAL_ENTER_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        }else{
            table->item(TRIGGER_INTERVAL_ENTER_EVENT_ID_ROW, VALUE_COLUMN)->setText("NONE");
        }
        name = file->getEventNameAt(bsData->triggerInterval.exitEventId);
        if (name != ""){
            table->item(TRIGGER_INTERVAL_EXIT_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        }else{
            table->item(TRIGGER_INTERVAL_EXIT_EVENT_ID_ROW, VALUE_COLUMN)->setText("NONE");
        }
        enterTimeTI->setValue(bsData->triggerInterval.enterTime);
        exitTimeTI->setValue(bsData->triggerInterval.exitTime);
        name = file->getEventNameAt(bsData->initiateInterval.enterEventId);
        if (name != ""){
            table->item(INITIATE_INTERVAL_ENTER_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        }else{
            table->item(INITIATE_INTERVAL_ENTER_EVENT_ID_ROW, VALUE_COLUMN)->setText("NONE");
        }
        name = file->getEventNameAt(bsData->initiateInterval.exitEventId);
        if (name != ""){
            table->item(INITIATE_INTERVAL_EXIT_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        }else{
            table->item(INITIATE_INTERVAL_EXIT_EVENT_ID_ROW, VALUE_COLUMN)->setText("NONE");
        }
        enterTimeII->setValue(bsData->initiateInterval.enterTime);
        exitTimeII->setValue(bsData->initiateInterval.exitTime);
        if (bsData->transition.data()){
            table->item(TRANSITION_ROW, VALUE_COLUMN)->setText(static_cast<hkbBlendingTransitionEffect *>(bsData->transition.data())->getName());
        }else{
            table->item(TRANSITION_ROW, VALUE_COLUMN)->setText("NULL");
        }
        if (bsData->condition.data()){
            condition->setText(static_cast<hkbStringCondition *>(bsData->condition.data())->conditionString);
        }else{
            condition->setText("");
        }
        name = file->getEventNameAt(bsData->eventId);
        if (name != ""){
            table->item(EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        }else{
            table->item(EVENT_ID_ROW, VALUE_COLUMN)->setText("NONE");
        }
        toStateId->clear();
        toStateId->addItems(parentObj->getStateNames());
        toStateId->blockSignals(true);
        toStateId->setCurrentIndex(parentObj->getStateNames().indexOf(parentObj->getStateName(bsData->toStateId)));
        toStateId->blockSignals(false);
        //fromNestedStateId->clear();
        //fromNestedStateId->addItems(parentObj->getStateNames());
        toNestedStateId->clear();
        toNestedStateId->addItems(parentObj->getNestedStateNames(bsData->toStateId));
        toNestedStateId->blockSignals(true);
        toNestedStateId->setCurrentIndex(parentObj->getNestedStateNames(bsData->toStateId).indexOf(parentObj->getNestedStateName(bsData->toStateId, bsData->toNestedStateId)));
        toNestedStateId->blockSignals(false);
        priority->setValue(bsData->priority);
        QStringList flags = bsData->flags.split("|");
        flagGlobalWildcard->setChecked(false);
        flagUseNestedState->setChecked(false);
        flagDisallowRandomTransition->setChecked(false);
        flagDisallowReturnToState->setChecked(false);
        flagAbutEndState->setChecked(false);
        if (flags.isEmpty()){
            if (bsData->flags == "FLAG_GLOBAL_WILDCARD_ROW"){
                flagGlobalWildcard->setChecked(true);
            }else if (bsData->flags == "FLAG_USE_NESTED_STATE_ROW"){
                flagUseNestedState->setChecked(true);
            }else if (bsData->flags == "FLAG_DISALLOW_RANDOM_TRANSITION_ROW"){
                flagDisallowRandomTransition->setChecked(true);
            }else if (bsData->flags == "FLAG_DISALLOW_RETURN_TO_PREVIOUS_STATE_ROW"){
                flagDisallowReturnToState->setChecked(true);
            }else if (bsData->flags == "FLAG_ABUT_END_STATE_ROW"){
                flagAbutEndState->setChecked(true);
            }
        }else{
            for (int i = 0; i < flags.size(); i++){
                if (flags.at(i) == "FLAG_GLOBAL_WILDCARD_ROW"){
                    flagGlobalWildcard->setChecked(true);
                }else if (flags.at(i) == "FLAG_USE_NESTED_STATE_ROW"){
                    flagUseNestedState->setChecked(true);
                }else if (flags.at(i) == "FLAG_DISALLOW_RANDOM_TRANSITION_ROW"){
                    flagDisallowRandomTransition->setChecked(true);
                }else if (flags.at(i) == "FLAG_DISALLOW_RETURN_TO_PREVIOUS_STATE_ROW"){
                    flagDisallowReturnToState->setChecked(true);
                }else if (flags.at(i) == "FLAG_ABUT_END_STATE_ROW"){
                    flagAbutEndState->setChecked(true);
                }
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("TransitionsUI::loadData(): The data, parent file or parent state machine is NULL!!"));
    }
    blockSignals(false);
}
