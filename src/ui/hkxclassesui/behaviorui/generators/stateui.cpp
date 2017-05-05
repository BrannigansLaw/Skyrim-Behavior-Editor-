#include "stateui.h"
#include "StateUI.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/generators/hkbstatemachine.h"
#include "src/hkxclasses/behavior/generators/hkbstatemachinestateinfo.h"
#include "src/hkxclasses/behavior/hkbstatemachineeventpropertyarray.h"
#include "src/hkxclasses/behavior/hkbstringeventpayload.h"
#include "src/ui/genericdatawidgets.h"
#include "src/ui/hkxclassesui/behaviorui/transitionsui.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/behaviorgraphview.h"
#include "src/ui/treegraphicsitem.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QLineEdit>
#include <QComboBox>
#include <QHeaderView>
#include <QSpinBox>
#include <QStackedLayout>
#include <QSignalMapper>
#include <QComboBox>

#define BASE_NUMBER_OF_ROWS 9

#define RETURN_BUTTON_ROW 0
#define NAME_ROW 1
#define STATE_ID_ROW 2
#define PROBABILITY_ROW 3
#define ENABLE_ROW 4
#define TRANSITIONS_ROW 5
#define GENERATOR_ROW 6
#define ADD_ENTER_EVENT_ROW 7

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

QStringList StateUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

StateUI::StateUI()
    : behaviorView(NULL),
      bsData(NULL),
      groupBox(new QGroupBox("hkbStateMachineStateInfo")),
      topLyt(new QGridLayout),
      table(new TableWidget),
      returnPB(new QPushButton("Return")),
      name(new LineEdit),
      generator(new QPushButton("NULL")),
      stateId(new SpinBox),
      probability(new DoubleSpinBox),
      enable(new QCheckBox),
      enableTransitions(new QCheckBox("Enable Transitions")),
      addEnterEventPB(new QPushButton("Add Enter Event")),
      removeEnterEventPB(new QPushButton("Remove Selected Enter Event")),
      addExitEventPB(new QPushButton("Add Exit Event")),
      removeExitEventPB(new QPushButton("Remove Selected Exit Event")),
      addTransitionPB(new QPushButton("Add Transition")),
      removeTransitionPB(new QPushButton("Remove Selected Transition"))
{
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(RETURN_BUTTON_ROW, NAME_COLUMN, new QTableWidgetItem("N/A"));
    table->setItem(RETURN_BUTTON_ROW, TYPE_COLUMN, new QTableWidgetItem("N/A"));
    table->setItem(RETURN_BUTTON_ROW, BINDING_COLUMN, new QTableWidgetItem("N/A"));
    table->setCellWidget(RETURN_BUTTON_ROW, VALUE_COLUMN, returnPB);
    table->setItem(NAME_ROW, NAME_COLUMN, new QTableWidgetItem("name"));
    table->setItem(NAME_ROW, TYPE_COLUMN, new QTableWidgetItem("hkStringPtr"));
    table->setItem(NAME_ROW, BINDING_COLUMN, new QTableWidgetItem("N/A"));
    table->setCellWidget(NAME_ROW, VALUE_COLUMN, name);
    table->setItem(STATE_ID_ROW, NAME_COLUMN, new QTableWidgetItem("id"));
    table->setItem(STATE_ID_ROW, TYPE_COLUMN, new QTableWidgetItem("hkInt32"));
    table->setItem(STATE_ID_ROW, BINDING_COLUMN, new QTableWidgetItem("N/A"));
    table->setCellWidget(STATE_ID_ROW, VALUE_COLUMN, stateId);
    table->setItem(PROBABILITY_ROW, NAME_COLUMN, new QTableWidgetItem("probability"));
    table->setItem(PROBABILITY_ROW, TYPE_COLUMN, new QTableWidgetItem("hkReal"));
    table->setItem(PROBABILITY_ROW, BINDING_COLUMN, new QTableWidgetItem("N/A"));
    table->setCellWidget(PROBABILITY_ROW, VALUE_COLUMN, probability);
    table->setItem(ENABLE_ROW, NAME_COLUMN, new QTableWidgetItem("enable"));
    table->setItem(ENABLE_ROW, TYPE_COLUMN, new QTableWidgetItem("hkBool"));
    table->setItem(ENABLE_ROW, BINDING_COLUMN, new QTableWidgetItem("N/A"));
    table->setCellWidget(ENABLE_ROW, VALUE_COLUMN, enable);
    table->setItem(TRANSITIONS_ROW, NAME_COLUMN, new QTableWidgetItem("transitions"));
    table->setItem(TRANSITIONS_ROW, TYPE_COLUMN, new QTableWidgetItem("hkbStateMachineTransitionInfoArray"));
    table->setCellWidget(TRANSITIONS_ROW, BINDING_COLUMN, enableTransitions);
    table->setItem(TRANSITIONS_ROW, VALUE_COLUMN, new QTableWidgetItem("Click To Edit"));
    table->setItem(GENERATOR_ROW, NAME_COLUMN, new QTableWidgetItem("generator"));
    table->setItem(GENERATOR_ROW, TYPE_COLUMN, new QTableWidgetItem("hkbGenerator"));
    table->setItem(GENERATOR_ROW, BINDING_COLUMN, new QTableWidgetItem("N/A"));
    table->setItem(GENERATOR_ROW, VALUE_COLUMN, new QTableWidgetItem("Click To Edit"));
    table->setCellWidget(ADD_ENTER_EVENT_ROW, NAME_COLUMN, addEnterEventPB);
    table->setItem(ADD_ENTER_EVENT_ROW, TYPE_COLUMN, new QTableWidgetItem("hkEvent"));
    table->setCellWidget(ADD_ENTER_EVENT_ROW, BINDING_COLUMN, removeEnterEventPB);
    table->setItem(ADD_ENTER_EVENT_ROW, VALUE_COLUMN, new QTableWidgetItem("N/A"));
    table->setCellWidget(ADD_ENTER_EVENT_ROW + 1, NAME_COLUMN, addExitEventPB);
    table->setItem(ADD_ENTER_EVENT_ROW + 1, TYPE_COLUMN, new QTableWidgetItem("hkEvent"));
    table->setCellWidget(ADD_ENTER_EVENT_ROW + 1, BINDING_COLUMN, removeExitEventPB);
    table->setItem(ADD_ENTER_EVENT_ROW + 1, VALUE_COLUMN, new QTableWidgetItem("N/A"));
    topLyt->addWidget(table, 1, 0, 8, 3);
    groupBox->setLayout(topLyt);
    addWidget(groupBox);
    addWidget(stateUI);

    connect(returnPB, SIGNAL(released()), this, SIGNAL(returnToParent()));
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    connect(generator, SIGNAL(released()), this, SLOT(viewGenerators()));
    connect(stateId, SIGNAL(editingFinished()), this, SLOT(setStateId()));
    connect(probability, SIGNAL(editingFinished()), this, SLOT(setProbability()));
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    connect(addEnterEventPB, SIGNAL(released()), this, SLOT(addEnterEvent()));
    connect(removeEnterEventPB, SIGNAL(released()), this, SLOT(removeEnterEvent()));
    connect(addExitEventPB, SIGNAL(released()), this, SLOT(addExitEvent()));
    connect(removeExitEventPB, SIGNAL(released()), this, SLOT(removeExitEvent()));
    connect(addTransitionPB, SIGNAL(released()), this, SLOT(addTransition()));
    connect(removeTransitionPB, SIGNAL(released()), this, SLOT(removeTransition()));
}

void StateUI::loadData(HkxObject *data){
    if (data && data->getSignature() == HKB_STATE_MACHINE_STATE_INFO){
        bsData = static_cast<hkbStateMachineStateInfo *>(data);
        hkbStateMachineEventPropertyArray *enterEvents = static_cast<hkbStateMachineEventPropertyArray *>(bsData->enterNotifyEvents.data());
        hkbStateMachineEventPropertyArray *exitEvents = static_cast<hkbStateMachineEventPropertyArray *>(bsData->exitNotifyEvents.data());
        hkbStateMachineTransitionInfoArray *trans = static_cast<hkbStateMachineTransitionInfoArray *>(bsData->transitions.data());
        QStringList eventList = static_cast<BehaviorFile *>(bsData->getParentFile())->getEventNames();
        eventList.prepend("None");
        if (generatorTable->getNumRows() < 1){
            //generatorTable->loadTable(static_cast<BehaviorFile *>(bsData->getParentFile())->getGeneratorNamesAndTypeNames(), "NULL");
        }
        if (enterEvents){
            for (int i = 0; i < enterEvents->events.size(); i++){
                appendEnterEventTableRow(i, enterEvents, eventList);
            }
            for (int j = enterEvents->events.size(); j < enterNotifyEvents->rowCount(); j++){
                enterNotifyEvents->setRowHidden(j, true);
            }
        }else{
            for (int j = 0; j < enterNotifyEvents->rowCount(); j++){
                enterNotifyEvents->setRowHidden(j, true);
            }
        }
        if (exitEvents){
            for (int i = 0; i < exitEvents->events.size(); i++){
                appendExitEventTableRow(i, exitEvents, eventList);
            }
            for (int j = exitEvents->events.size(); j < exitNotifyEvents->rowCount(); j++){
                exitNotifyEvents->setRowHidden(j, true);
            }
        }else{
            for (int j = 0; j < exitNotifyEvents->rowCount(); j++){
                exitNotifyEvents->setRowHidden(j, true);
            }
        }
        if (trans){
            for (int i = 0; i < trans->transitions.size(); i++){
                if (i >= transitions->rowCount()){
                    transitions->setRowCount(transitions->rowCount() + 1);
                    transitions->setItem(i, 0, new QTableWidgetItem(trans->getTransitionNameAt(i)));
                    transitions->setItem(i, 1, new QTableWidgetItem(QString::number(trans->transitions.at(i).priority)));
                    transitions->setItem(i, 2, new QTableWidgetItem("Click to Edit"));
                }else{
                    transitions->setRowHidden(i, false);
                    transitions->item(i, 0)->setText(trans->getTransitionNameAt(i));
                    transitions->item(i, 1)->setText(QString::number(trans->transitions.at(i).priority));
                }
            }
            for (int j = trans->transitions.size(); j < transitions->rowCount(); j++){
                transitions->setRowHidden(j, true);
            }
        }else{
            for (int j = 0; j < transitions->rowCount(); j++){
                transitions->setRowHidden(j, true);
            }
        }
        name->setText(bsData->name);
        stateId->setValue(bsData->stateId);
        probability->setValue(bsData->probability);
        enable->setChecked(bsData->enable);
        if (bsData->generator.data()){
            generator->setText(static_cast<hkbGenerator *>(bsData->generator.data())->getName());
        }
    }
}

void StateUI::setName(){
    if (bsData){
        bsData->name = name->text();
        bsData->getParentFile()->toggleChanged(true);
        emit stateNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData) + 1);
    }
}

void StateUI::setStateId(){
    if (bsData){
        bsData->setStateId(stateId->value());//Update parent state machine???
        bsData->getParentFile()->toggleChanged(true);
    }
}

void StateUI::setProbability(){
    if (bsData){
        bsData->probability = probability->value();
        bsData->getParentFile()->toggleChanged(true);
    }
}

void StateUI::setEnable(){
    if (bsData){
        bsData->enable = enable->isChecked();
        bsData->getParentFile()->toggleChanged(true);
    }
}

void StateUI::addEnterEvent(){
    if (bsData){
        hkbStateMachineEventPropertyArray *events = static_cast<hkbStateMachineEventPropertyArray *>(bsData->enterNotifyEvents.data());
        QStringList eventList = static_cast<BehaviorFile *>(bsData->getParentFile())->getEventNames();
        eventList.prepend("None");
        if (events){
            events->addEvent();
        }else{
            events = new hkbStateMachineEventPropertyArray(bsData->getParentFile());
            events->addEvent();
            bsData->enterNotifyEvents = HkxSharedPtr(events);
        }
        appendEnterEventTableRow(events->getLastEventIndex(), events, eventList);
        bsData->getParentFile()->toggleChanged(true);
    }
}

void StateUI::removeEnterEvent(){
    enterNotifyEvents->blockSignals(true);
    int index = enterNotifyEvents->currentRow();
    if (bsData){
        hkbStateMachineEventPropertyArray *events = static_cast<hkbStateMachineEventPropertyArray *>(bsData->enterNotifyEvents.data());
        if (events){
            events->removeEvent(index);
        }
        delete enterNotifyEvents->cellWidget(index, 0);
        delete enterNotifyEvents->cellWidget(index, 1);
        enterNotifyEvents->removeRow(index);
    }
    enterNotifyEvents->blockSignals(false);
    bsData->getParentFile()->toggleChanged(true);
}

void StateUI::addExitEvent(){
    if (bsData){
        hkbStateMachineEventPropertyArray *events = static_cast<hkbStateMachineEventPropertyArray *>(bsData->exitNotifyEvents.data());
        QStringList eventList = static_cast<BehaviorFile *>(bsData->getParentFile())->getEventNames();
        eventList.prepend("None");
        if (events){
            events->addEvent();
        }else{
            events = new hkbStateMachineEventPropertyArray(bsData->getParentFile());
            events->addEvent();
            bsData->exitNotifyEvents = HkxSharedPtr(events);
        }
        appendExitEventTableRow(events->getLastEventIndex(), events, eventList);
        bsData->getParentFile()->toggleChanged(true);
    }
}

void StateUI::removeExitEvent(){
    exitNotifyEvents->blockSignals(true);
    int index = exitNotifyEvents->currentRow();
    if (bsData){
        hkbStateMachineEventPropertyArray *events = static_cast<hkbStateMachineEventPropertyArray *>(bsData->exitNotifyEvents.data());
        if (events){
            events->removeEvent(index);
        }
        delete exitNotifyEvents->cellWidget(index, 0);
        delete exitNotifyEvents->cellWidget(index, 1);
        exitNotifyEvents->removeRow(index);
    }
    exitNotifyEvents->blockSignals(false);
    bsData->getParentFile()->toggleChanged(true);
}

void StateUI::addTransition(){
    if (bsData){
        hkbStateMachineTransitionInfoArray *trans = static_cast<hkbStateMachineTransitionInfoArray *>(bsData->transitions.data());
        int i;
        if (trans){
            trans->addTransition();
        }else{
            trans = new hkbStateMachineTransitionInfoArray(bsData->getParentFile(), static_cast<hkbStateMachine *>(bsData->parentSM.data()));
            bsData->transitions = HkxSharedPtr(trans);
        }
        i = transitions->rowCount();
        transitions->setRowCount(transitions->rowCount() + 1);
        transitions->setItem(i, 0, new QTableWidgetItem("New_Transition"));
        transitions->setItem(i, 1, new QTableWidgetItem(trans->getClassname()));
        transitions->setItem(i, 2, new QTableWidgetItem("Click to Edit"));
        bsData->getParentFile()->toggleChanged(true);
    }
}

void StateUI::removeTransition(){
    if (bsData){
        hkbStateMachineTransitionInfoArray *trans = static_cast<hkbStateMachineTransitionInfoArray *>(bsData->transitions.data());
        int i = transitions->currentRow();
        if (transitions){
            trans->removeTransition(i);
        }
        transitions->removeRow(i);
        bsData->getParentFile()->toggleChanged(true);
    }
}

void StateUI::transitionSelected(int row, int column){
    hkbStateMachineTransitionInfoArray *trans = static_cast<hkbStateMachineTransitionInfoArray *>(bsData->transitions.data());
    if (column == 2 && bsData && row < trans->transitions.size()){
        emit viewTransition((hkbStateMachine *)trans->parent.data(), (HkTransition *)&trans->transitions[row]);
    }
}

void StateUI::setGenerator(int index){
    /*if (behaviorView && index > -1){
        DataIconManager *ptr = static_cast<BehaviorFile *>(bsData->getParentFile())->getGeneratorDataAt(index);
        if (!behaviorView->getSelectedItem()->getChildWithData(ptr)){
            if (!ptr || ptr == bsData || !behaviorView->reconnectIcon(behaviorView->getSelectedItem(), ptr,  reinterpret_cast<DataIconManager *>(bsData->generator.data()))){
                QMessageBox msg;
                msg.setText("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to create a circular branch or dead end!!!");
                msg.exec();
                return;
            }
        }else{
            //behaviorView->removeItemFromGraph(behaviorView->getSelectedItem()->getChildWithData(bsData->generator.data()), NULL, false);
        }
        generator->setText(ptr->getName());
        if (index > 0){
            bsData->generator = HkxObjectExpSharedPtr(ptr);
            behaviorView->removeGeneratorData();
            bsData->getParentFile()->toggleChanged(true);
        }
    }
    generatorTable->hide();*/
}

void StateUI::setBehaviorView(BehaviorGraphView *view){
    behaviorView = view;
}
