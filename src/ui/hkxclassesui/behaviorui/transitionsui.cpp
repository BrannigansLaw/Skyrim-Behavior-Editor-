#include "transitionsui.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/hkbexpressioncondition.h"
#include "src/hkxclasses/behavior/generators/hkbstatemachine.h"
#include "src/hkxclasses/behavior/generators/hkbstatemachinestateinfo.h"
#include "src/hkxclasses/behavior/hkbstatemachinetransitioninfoarray.h"
#include "src/hkxclasses/behavior/hkbstringeventpayload.h"
#include "src/ui/genericdatawidgets.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/behaviorgraphui/behaviorgraphview.h"
#include "src/ui/behaviorgraphui/customtreegraphicsviewicon.h"
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

QStringList TransitionsUI::headerLabels1 = {
    "Name",
    "Type",
    "Value"
};

TransitionsUI::TransitionsUI()
    : behaviorView(NULL),
      bsData(NULL),
      returnToParentPB(new QPushButton("Return")),
      lyt(new QVBoxLayout),
      table(new TableWidget),
      returnPB(new QPushButton("Return")),
      enterEventIdTI(new ComboBox),
      exitEventIdTI(new ComboBox),
      enterTimeTI(new DoubleSpinBox),
      exitTimeTI(new DoubleSpinBox),
      enterEventIdII(new ComboBox),
      exitEventIdII(new ComboBox),
      enterTimeII(new DoubleSpinBox),
      exitTimeII(new DoubleSpinBox),
      condition(new QLineEdit),
      eventId(new ComboBox),
      toStateId(new ComboBox),
      fromNestedStateId(new ComboBox),
      toNestedStateId(new ComboBox),
      priority(new SpinBox),
      flagWildcard(new QCheckBox),
      flagUseNestedState(new QCheckBox),
      flagDisallowRandomTransition(new QCheckBox),
      flagDisallowReturnToState(new QCheckBox),
      flagAbutEndState(new QCheckBox),
      stackLyt(new QStackedLayout)
{
    setTitle("hkbStateMachineTransitionInfoArray");
    stackLyt->addWidget(table);
    stackLyt->setCurrentIndex(TRANSITION_WIDGET);
    table->setRowCount(15);
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels(headerLabels1);
    table->setCellWidget(0, 1, returnPB);
    table->setItem(1, 0, new QTableWidgetItem("Trigger Interval: enterEventId"));
    table->setItem(1, 1, new QTableWidgetItem("hkEvent"));
    table->setCellWidget(1, 2, enterEventIdTI);
    table->setItem(2, 0, new QTableWidgetItem("Trigger Interval: exitEventId"));
    table->setItem(2, 1, new QTableWidgetItem("hkEvent"));
    table->setCellWidget(2, 2, exitEventIdTI);
    table->setItem(3, 0, new QTableWidgetItem("Trigger Interval: enterTime"));
    table->setItem(3, 1, new QTableWidgetItem("hkReal"));
    table->setCellWidget(3, 2, enterTimeTI);
    table->setItem(4, 0, new QTableWidgetItem("Trigger Interval: exitTime"));
    table->setItem(4, 1, new QTableWidgetItem("hkReal"));
    table->setCellWidget(4, 2, exitTimeTI);
    table->setItem(5, 0, new QTableWidgetItem("Initiate Interval: enterEventId"));
    table->setItem(5, 1, new QTableWidgetItem("hkEvent"));
    table->setCellWidget(5, 2, enterEventIdII);
    table->setItem(6, 0, new QTableWidgetItem("Initiate Interval: exitEventId"));
    table->setItem(6, 1, new QTableWidgetItem("hkEvent"));
    table->setCellWidget(6, 2, exitEventIdII);
    table->setItem(7, 0, new QTableWidgetItem("Initiate Interval: enterTime"));
    table->setItem(7, 1, new QTableWidgetItem("hkReal"));
    table->setCellWidget(7, 2, enterTimeII);
    table->setItem(8, 0, new QTableWidgetItem("Initiate Interval: exitTime"));
    table->setItem(8, 1, new QTableWidgetItem("hkReal"));
    table->setCellWidget(8, 2, exitTimeII);
    table->setItem(9, 0, new QTableWidgetItem("transition"));
    table->setItem(9, 1, new QTableWidgetItem("hkTransitionEffect"));
    table->setItem(9, 2, new QTableWidgetItem("Click to Edit"));
    //table->setCellWidget(9, 2, transition);
    table->setItem(10, 0, new QTableWidgetItem("condition"));
    table->setItem(10, 1, new QTableWidgetItem("hkbExpressionCondition"));
    table->setCellWidget(10, 2, condition);
    table->setItem(11, 0, new QTableWidgetItem("eventId"));
    table->setItem(11, 1, new QTableWidgetItem("hkEvent"));
    table->setCellWidget(11, 2, eventId);
    table->setItem(12, 0, new QTableWidgetItem("toStateId"));
    table->setItem(12, 1, new QTableWidgetItem("hkInt32"));
    table->setCellWidget(12, 2, toStateId);
    table->setItem(13, 0, new QTableWidgetItem("fromNestedStateId"));
    table->setItem(13, 1, new QTableWidgetItem("hkInt32"));
    table->setCellWidget(13, 2, fromNestedStateId);
    table->setItem(14, 0, new QTableWidgetItem("toNestedStateId"));
    table->setItem(14, 1, new QTableWidgetItem("hkInt32"));
    table->setCellWidget(14, 2, toNestedStateId);
    table->setItem(15, 0, new QTableWidgetItem("priority"));
    table->setItem(15, 1, new QTableWidgetItem("hkInt32"));
    table->setCellWidget(15, 2, priority);
    table->setItem(16, 0, new QTableWidgetItem("flagWildcard"));
    table->setItem(16, 1, new QTableWidgetItem("TransitionFlags"));
    table->setCellWidget(16, 2, flagWildcard);
    table->setItem(17, 0, new QTableWidgetItem("flagLocal"));
    table->setItem(17, 1, new QTableWidgetItem("TransitionFlags"));
    table->setCellWidget(17, 2, flagLocal);
    table->setItem(18, 0, new QTableWidgetItem("flagUseNestedState"));
    table->setItem(18, 1, new QTableWidgetItem("TransitionFlags"));
    table->setCellWidget(18, 2, flagUseNestedState);
    table->setItem(19, 0, new QTableWidgetItem("flagDisallowRandomTransition"));
    table->setItem(19, 1, new QTableWidgetItem("TransitionFlags"));
    table->setCellWidget(19, 2, flagDisallowRandomTransition);
    table->setItem(20, 0, new QTableWidgetItem("flagDisallowReturnToState"));
    table->setItem(20, 1, new QTableWidgetItem("TransitionFlags"));
    table->setCellWidget(20, 2, flagDisallowReturnToState);
    table->setItem(21, 0, new QTableWidgetItem("flagAbutEndState"));
    table->setItem(21, 1, new QTableWidgetItem("TransitionFlags"));
    table->setCellWidget(21, 2, flagAbutEndState);
    lyt->addLayout(stackLyt);
    setLayout(lyt);
    connect(returnPB, SIGNAL(released()), this, SIGNAL(returnToParent()));
}

void TransitionsUI::addEventToLists(const QString & name){
    //
}

void TransitionsUI::removeEventFromLists(int index){
    //
}

void TransitionsUI::renameEventInLists(const QString & newName, int index){
    //
}

void TransitionsUI::addVariableToLists(const QString & name){
    //
}

void TransitionsUI::removeVariableFromLists(int index){
    //
}

void TransitionsUI::renameVariableInLists(const QString & newName, int index){
    //
}

void TransitionsUI::setTriggerIntervalEnterEventId(){
    //
}

void TransitionsUI::setTriggerIntervalExitEventId(){
    //
}

void TransitionsUI::setTriggerIntervalEnterTime(){
    //
}

void TransitionsUI::setTriggerIntervalExitTime(){
    //
}

void TransitionsUI::setInitiateIntervalEnterEventId(){
    //
}

void TransitionsUI::setInitiateIntervalExitEventId(){
    //
}

void TransitionsUI::setInitiateIntervalEnterTime(){
    //
}

void TransitionsUI::setInitiateIntervalExitTime(){
    //
}

void TransitionsUI::viewTransition(){
    //
}

void TransitionsUI::setCondition(){
    //
}

void TransitionsUI::setEventId(){
    //
}

void TransitionsUI::setToStateId(){
    //
}

void TransitionsUI::setFromNestedStateId(){
    //
}

void TransitionsUI::setToNestedStateId(){
    //
}

void TransitionsUI::setPriority(){
    //
}

void TransitionsUI::toggleWildcardFlag(){
    //
}

void TransitionsUI::toggleLocalFlag(){
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

void TransitionsUI::loadComboBoxes(){
    QStringList eventList = behaviorView->behavior->getEventNames();
    eventList.prepend("None");
    enterEventIdTI->insertItems(0, eventList);
    exitEventIdTI->insertItems(0, eventList);
    enterEventIdII->insertItems(0, eventList);
    exitEventIdII->insertItems(0, eventList);
    eventId->insertItems(0, eventList);
}

void TransitionsUI::loadData(hkbStateMachine *parent, HkTransition *data){
    if (data && parent){
        bsData = (hkbStateMachineTransitionInfoArray::HkTransition *)data;
        parentObj = parent;
        hkbExpressionCondition *condit = static_cast<hkbExpressionCondition *>(bsData->condition.data());
        QStringList eventList = behaviorView->getEventNames();
        eventList.prepend("None");
        QStringList stateNames = parentObj->getStateNames();
        stateNames.prepend("None");
        toStateId->clear();
        toStateId->addItems(stateNames);
        enterEventIdTI->setCurrentIndex(bsData->triggerInterval.enterEventId + 1);
        exitEventIdTI->setCurrentIndex(bsData->triggerInterval.exitEventId + 1);
        enterTimeTI->setValue(bsData->triggerInterval.enterTime);
        exitTimeTI->setValue(bsData->triggerInterval.exitTime);

        enterEventIdII->setCurrentIndex(bsData->initiateInterval.enterEventId + 1);
        exitEventIdII->setCurrentIndex(bsData->initiateInterval.exitEventId + 1);
        enterTimeII->setValue(bsData->initiateInterval.enterTime);
        exitTimeII->setValue(bsData->initiateInterval.exitTime);
        if (condit){
            condition->setText(condit->getExpression());
        }
        eventId->setCurrentIndex(bsData->eventId + 1);
        toStateId->setCurrentIndex(bsData->toStateId + 1);
    }
}
