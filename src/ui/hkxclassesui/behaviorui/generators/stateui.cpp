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
#include "src/ui/behaviorgraphui/behaviorgraphview.h"
#include "src/ui/behaviorgraphui/customtreegraphicsviewicon.h"
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

/*
 * StateUI
 */

QStringList StateUI::headerLabels1 = {
    "Name",
    "Type",
    "Value"
};

QStringList StateUI::headerLabels2 = {
    "Event",
    "Payload"
};

QStringList StateUI::headerLabels3 = {
    "To State",
    "Priority",
    "Edit"
};

StateUI::StateUI()
    : generatorTable(new HkxObjectTableWidget("Select a hkbGenerator!")),
      behaviorView(NULL),
      bsData(NULL),
      lyt(new QVBoxLayout),
      table(new TableWidget),
      returnPB(new QPushButton("Return")),
      name(new QLineEdit),
      generator(new QPushButton("NULL")),
      stateId(new SpinBox),
      probability(new DoubleSpinBox),
      enable(new QCheckBox),
      enterNotifyEvents(new TableWidget),
      exitNotifyEvents(new TableWidget),
      transitions(new TableWidget),
      addEnterEventPB(new QPushButton("Add Enter Event")),
      removeEnterEventPB(new QPushButton("Remove Selected Enter Event")),
      typeEnterEventCB(new ComboBox),
      enterEventSignalMapper(new QSignalMapper),
      enterEventPayloadSignalMapper(new QSignalMapper),
      enterEventButtonLyt(new QHBoxLayout),
      addExitEventPB(new QPushButton("Add Exit Event")),
      removeExitEventPB(new QPushButton("Remove Selected Exit Event")),
      typeExitEventCB(new ComboBox),
      exitEventSignalMapper(new QSignalMapper),
      exitEventPayloadSignalMapper(new QSignalMapper),
      exitEventButtonLyt(new QHBoxLayout),
      addTransitionPB(new QPushButton("Add Transition")),
      removeTransitionPB(new QPushButton("Remove Selected Transition")),
      typeTransitionCB(new ComboBox),
      transitionButtonLyt(new QHBoxLayout)
{
    setTitle("hkbStateMachineStateInfo");
    generatorTable->hide();
    enterEventButtonLyt->addWidget(addEnterEventPB, 1);
    enterEventButtonLyt->addWidget(typeEnterEventCB, 2);
    enterEventButtonLyt->addWidget(removeEnterEventPB, 1);
    exitEventButtonLyt->addWidget(addExitEventPB, 1);
    exitEventButtonLyt->addWidget(typeExitEventCB, 2);
    exitEventButtonLyt->addWidget(removeExitEventPB, 1);
    transitionButtonLyt->addWidget(addTransitionPB, 1);
    transitionButtonLyt->addWidget(typeTransitionCB, 2);
    transitionButtonLyt->addWidget(removeTransitionPB, 1);
    table->setRowCount(5);
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels(headerLabels1);
    table->setCellWidget(0, 1, returnPB);
    table->setCellWidget(1, 0, name);
    table->setItem(1, 1, new QTableWidgetItem("String"));
    table->setItem(1, 2, new QTableWidgetItem("N/A"));
    table->setItem(2, 0, new QTableWidgetItem("generator"));
    table->setItem(2, 1, new QTableWidgetItem("hkbGenerator"));
    table->setCellWidget(2, 2, generator);
    table->setItem(2, 2, new QTableWidgetItem("NULL"));
    table->setItem(3, 0, new QTableWidgetItem("stateId"));
    table->setItem(3, 1, new QTableWidgetItem("Int"));
    table->setCellWidget(3, 2, stateId);
    table->setItem(4, 0, new QTableWidgetItem("probability"));
    table->setItem(4, 1, new QTableWidgetItem("Double"));
    table->setCellWidget(4, 2, probability);
    table->setItem(5, 0, new QTableWidgetItem("enable"));
    table->setItem(5, 1, new QTableWidgetItem("Bool"));
    table->setCellWidget(5, 2, enable);
    enterNotifyEvents->setColumnCount(2);
    enterNotifyEvents->setHorizontalHeaderLabels(headerLabels2);
    exitNotifyEvents->setHorizontalHeaderLabels(headerLabels2);
    transitions->setColumnCount(3);
    transitions->setHorizontalHeaderLabels(headerLabels3);
    lyt->addWidget(table);
    lyt->addLayout(enterEventButtonLyt);
    lyt->addWidget(enterNotifyEvents);
    lyt->addLayout(exitEventButtonLyt);
    lyt->addWidget(exitNotifyEvents);
    lyt->addLayout(transitionButtonLyt);
    lyt->addWidget(transitions);
    setLayout(lyt);
    connect(returnPB, SIGNAL(released()), this, SIGNAL(toParentStateMachine()));
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    connect(generator, SIGNAL(released()), this, SLOT(viewGenerators()));
    connect(generatorTable, SIGNAL(elementSelected(int)), this, SLOT(setGenerator(int)));
    connect(generatorTable, SIGNAL(hideWindow()), this, SLOT(viewGenerators()));
    connect(stateId, SIGNAL(editingFinished()), this, SLOT(setStateId()));
    connect(probability, SIGNAL(editingFinished()), this, SLOT(setProbability()));
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    connect(enterEventSignalMapper, SIGNAL(mapped(int)), this, SLOT(setEnterEventAt(int)));
    connect(enterEventPayloadSignalMapper, SIGNAL(mapped(int)), this, SLOT(setEnterEventPayloadAt(int)));
    connect(exitEventSignalMapper, SIGNAL(mapped(int)), this, SLOT(setExitEventAt(int)));
    connect(exitEventPayloadSignalMapper, SIGNAL(mapped(int)), this, SLOT(setExitEventPayloadAt(int)));
    connect(addEnterEventPB, SIGNAL(released()), this, SLOT(addEnterEvent()));
    connect(removeEnterEventPB, SIGNAL(released()), this, SLOT(removeEnterEvent()));
    connect(addExitEventPB, SIGNAL(released()), this, SLOT(addExitEvent()));
    connect(removeExitEventPB, SIGNAL(released()), this, SLOT(removeExitEvent()));
    connect(addTransitionPB, SIGNAL(released()), this, SLOT(addTransition()));
    connect(removeTransitionPB, SIGNAL(released()), this, SLOT(removeTransition()));
    connect(transitions, SIGNAL(cellClicked(int,int)), this, SLOT(transitionSelected(int,int)));
}

/*void StateUI::setGeneratorTable(HkxObjectTableWidget *genTable){
    if (genTable){
        generatorTable = genTable;
        connect(generatorTable, SIGNAL(elementSelected(int)), this, SLOT(setGenerator(int)));
        connect(generatorTable, SIGNAL(hideWindow()), this, SLOT(viewGenerators()));
    }
}*/

void StateUI::viewGenerators(){
    if (!generatorTable->isVisible()){
        generatorTable->show();
        //connect(generatorTable, SIGNAL(elementSelected(int)), this, SLOT(setGenerator(int)));
    }else{
        generatorTable->hide();
        //disconnect(generatorTable, SIGNAL(elementSelected(int)), this, SLOT(setGenerator(int)));
    }
}

/*void StateUI::addGeneratorToLists(const QString & name){
    generator->insertItem(generator->count(), name);
}

void StateUI::removeGeneratorFromLists(int index){
    generator->removeItem(index);
}

void StateUI::renameGeneratorInLists(const QString & name, int index){
    index++;
    generator->setItemText(index, name);
}*/

void StateUI::addEventToLists(const QString & name){
    QComboBox *eventList = NULL;
    for (int i = 0; i < enterNotifyEvents->rowCount(); i++){
        eventList = qobject_cast<QComboBox *>(enterNotifyEvents->cellWidget(i, 0));
        eventList->insertItem(eventList->count(), name);
    }
    for (int i = 0; i < exitNotifyEvents->rowCount(); i++){
        eventList = qobject_cast<QComboBox *>(exitNotifyEvents->cellWidget(i, 0));
        eventList->insertItem(eventList->count(), name);
    }
}

void StateUI::removeEventFromLists(int index){
    index++;
    QComboBox *eventList = NULL;
    for (int i = 0; i < enterNotifyEvents->rowCount(); i++){
        eventList = qobject_cast<QComboBox *>(enterNotifyEvents->cellWidget(i, 0));
        if (eventList->currentIndex() == index){
            eventList->setCurrentIndex(0);
        }
        eventList->removeItem(index);
    }
    for (int i = 0; i < exitNotifyEvents->rowCount(); i++){
        eventList = qobject_cast<QComboBox *>(exitNotifyEvents->cellWidget(i, 0));
        if (eventList->currentIndex() == index){
            eventList->setCurrentIndex(0);
        }
        eventList->removeItem(index);
    }
}

void StateUI::renameEventInLists(const QString & newName, int index){
    index++;
    QComboBox *eventList = NULL;
    for (int i = 0; i < enterNotifyEvents->rowCount(); i++){
        eventList = qobject_cast<QComboBox *>(enterNotifyEvents->cellWidget(i, 0));
        eventList->setItemText(index, newName);
    }
    for (int i = 0; i < exitNotifyEvents->rowCount(); i++){
        eventList = qobject_cast<QComboBox *>(exitNotifyEvents->cellWidget(i, 0));
        eventList->setItemText(index, newName);
    }
}

void StateUI::setName(){
    if (bsData){
        bsData->name = name->text();
        emit stateNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData) + 1);
    }
}

void StateUI::setStateId(){
    if (bsData){
        bsData->stateId = stateId->value();
    }
}

void StateUI::setProbability(){
    if (bsData){
        bsData->probability = probability->value();
    }
}

void StateUI::setEnable(){
    if (bsData){
        bsData->enable = enable->isChecked();
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
            bsData->enterNotifyEvents = HkxObjectExpSharedPtr(events);
        }
        appendEnterEventTableRow(events->getLastEventIndex(), events, eventList);
    }
}

void StateUI::setEnterEventAt(int index){
    if (bsData){
        hkbStateMachineEventPropertyArray *events = static_cast<hkbStateMachineEventPropertyArray *>(bsData->enterNotifyEvents.data());
        events->setEventId(index, static_cast<ComboBox *>(enterNotifyEvents->cellWidget(index, 0))->currentIndex() - 1);
    }
}

void StateUI::setEnterEventPayloadAt(int index){
    if (bsData){
        hkbStateMachineEventPropertyArray *events = static_cast<hkbStateMachineEventPropertyArray *>(bsData->enterNotifyEvents.data());
        hkbStringEventPayload *payload = static_cast<hkbStringEventPayload *>(events->events.at(index).payload.data());
        QString text;
        text = static_cast<QLineEdit *>(enterNotifyEvents->cellWidget(index, 1))->text();
        if (payload){
            if (text == ""){
                events->events[index].payload = HkxObjectExpSharedPtr();
            }else{
                payload->data = text;
            }
        }else{
            if (text != ""){    //Add to file?
                events->events[index].payload = HkxObjectExpSharedPtr(new hkbStringEventPayload(bsData->getParentFile(), text));
            }
        }
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
            bsData->exitNotifyEvents = HkxObjectExpSharedPtr(events);
        }
        appendExitEventTableRow(events->getLastEventIndex(), events, eventList);
    }
}

void StateUI::setExitEventAt(int index){
    if (bsData){
        hkbStateMachineEventPropertyArray *events = static_cast<hkbStateMachineEventPropertyArray *>(bsData->exitNotifyEvents.data());
        events->setEventId(index, static_cast<ComboBox *>(exitNotifyEvents->cellWidget(index, 0))->currentIndex() - 1);
    }
}

void StateUI::setExitEventPayloadAt(int index){
    if (bsData){
        hkbStateMachineEventPropertyArray *events = static_cast<hkbStateMachineEventPropertyArray *>(bsData->exitNotifyEvents.data());
        hkbStringEventPayload *payload = static_cast<hkbStringEventPayload *>(events->events.at(index).payload.data());
        QString text;
        text = static_cast<QLineEdit *>(exitNotifyEvents->cellWidget(index, 1))->text();
        if (payload){
            if (text == ""){
                events->events[index].payload = HkxObjectExpSharedPtr();
            }else{
                payload->data = text;
            }
        }else{
            if (text != ""){    //Add to file?
                events->events[index].payload = HkxObjectExpSharedPtr(new hkbStringEventPayload(bsData->getParentFile(), text));
            }
        }
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
}

void StateUI::addTransition(){
    if (bsData){
        hkbStateMachineTransitionInfoArray *trans = static_cast<hkbStateMachineTransitionInfoArray *>(bsData->transitions.data());
        int i;
        if (trans){
            trans->addTransition();
        }else{
            trans = new hkbStateMachineTransitionInfoArray(bsData->getParentFile(), static_cast<hkbStateMachine *>(bsData->parentSM.data()));
            bsData->transitions = HkxObjectExpSharedPtr(trans);
        }
        i = transitions->rowCount();
        transitions->setRowCount(transitions->rowCount() + 1);
        transitions->setItem(i, 0, new QTableWidgetItem("New_Transition"));
        transitions->setItem(i, 1, new QTableWidgetItem(trans->getClassname()));
        transitions->setItem(i, 2, new QTableWidgetItem("Edit"));
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
    }
}

void StateUI::transitionSelected(int row, int column){
    hkbStateMachineTransitionInfoArray *trans = static_cast<hkbStateMachineTransitionInfoArray *>(bsData->transitions.data());
    if (column == 2 && bsData && row < trans->transitions.size()){
        emit viewTransition((hkbStateMachine *)trans->parent.data(), (HkTransition *)&trans->transitions[row]);
    }
}

void StateUI::setGenerator(int index){
    if (behaviorView && index > -1){
        DataIconManager *ptr = static_cast<BehaviorFile *>(bsData->getParentFile())->getGeneratorDataAt(index);
        if (!behaviorView->getSelectedItem()->getChildIcon(ptr)){
            if (!ptr || ptr == bsData || !behaviorView->reconnectBranch(static_cast<DataIconManager *>(bsData->generator.data()), ptr, behaviorView->getSelectedItem())){
                QMessageBox msg;
                msg.setText("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to create a circular branch or dead end!!!");
                msg.exec();
                return;
            }
        }else{
            behaviorView->removeSelectedObjectBranch(behaviorView->getSelectedItem()->getChildIcon(bsData->generator.data()), NULL, false);
        }
        generator->setText(ptr->getName());
        if (index > 0){
            bsData->generator = HkxObjectExpSharedPtr(ptr);
            behaviorView->removeGeneratorData();
        }
    }
    generatorTable->hide();
}

void StateUI::appendEnterEventTableRow(int index, hkbStateMachineEventPropertyArray *enterEvents, const QStringList & eventList){
    ComboBox *comboBox;
    QLineEdit *lineEdit;
    hkbStringEventPayload *payload = NULL;
    if (index >= enterNotifyEvents->rowCount()){
        enterNotifyEvents->setRowCount(enterNotifyEvents->rowCount() + 1);
        comboBox = new ComboBox;
        comboBox->addItems(eventList);
        enterNotifyEvents->setCellWidget(index, 0, comboBox);
        comboBox->setCurrentIndex(enterEvents->events.at(index).id + 1);
        connect(comboBox, SIGNAL(activated(int)), enterEventSignalMapper, SLOT(map()));
        enterEventSignalMapper->setMapping(comboBox, index);
        lineEdit = new QLineEdit;
        payload = static_cast<hkbStringEventPayload *>(enterEvents->events.at(index).payload.data());
        if (payload){
            lineEdit->setText(payload->data);
        }
        enterNotifyEvents->setCellWidget(index, 1, lineEdit);
        connect(lineEdit, SIGNAL(editingFinished()), enterEventPayloadSignalMapper, SLOT(map()));
        enterEventPayloadSignalMapper->setMapping(lineEdit, index);
    }else{
        enterNotifyEvents->setRowHidden(index, false);
        comboBox = qobject_cast<ComboBox *>(enterNotifyEvents->cellWidget(index, 0));
        enterEventSignalMapper->blockSignals(true);
        comboBox->setCurrentIndex(enterEvents->events.at(index).id + 1);
        connect(enterEventSignalMapper, SIGNAL(mapped(int)), this, SLOT(setEnterEventAt(int)));
        enterEventSignalMapper->setMapping(comboBox, index);
        lineEdit = qobject_cast<QLineEdit *>(enterNotifyEvents->cellWidget(index, 1));
        enterEventSignalMapper->blockSignals(false);
        comboBox->setCurrentIndex(enterEvents->events.at(index).id + 1);
        connect(enterEventPayloadSignalMapper, SIGNAL(mapped(int)), this, SLOT(setEnterEventPayloadAt(int)));
        enterEventPayloadSignalMapper->setMapping(comboBox, index);
    }
}

void StateUI::appendExitEventTableRow(int index, hkbStateMachineEventPropertyArray *exitEvents, const QStringList &eventList){
    ComboBox *comboBox;
    QLineEdit *lineEdit;
    hkbStringEventPayload *payload = NULL;
    if (index >= exitNotifyEvents->rowCount()){
        exitNotifyEvents->setRowCount(exitNotifyEvents->rowCount() + 1);
        comboBox = new ComboBox;
        comboBox->addItems(eventList);
        exitNotifyEvents->setCellWidget(index, 0, comboBox);
        comboBox->setCurrentIndex(exitEvents->events.at(index).id + 1);
        connect(comboBox, SIGNAL(activated(int)), enterEventSignalMapper, SLOT(map()));
        enterEventSignalMapper->setMapping(comboBox, index);
        lineEdit = new QLineEdit;
        payload = static_cast<hkbStringEventPayload *>(exitEvents->events.at(index).payload.data());
        if (payload){
            lineEdit->setText(payload->data);
        }
        exitNotifyEvents->setCellWidget(index, 1, lineEdit);
        connect(lineEdit, SIGNAL(editingFinished()), exitEventPayloadSignalMapper, SLOT(map()));
        exitEventPayloadSignalMapper->setMapping(lineEdit, index);
    }else{
        exitNotifyEvents->setRowHidden(index, false);
        comboBox = qobject_cast<ComboBox *>(exitNotifyEvents->cellWidget(index, 0));
        exitEventSignalMapper->blockSignals(true);
        comboBox->setCurrentIndex(exitEvents->events.at(index).id + 1);
        exitEventSignalMapper->blockSignals(false);
        exitEventSignalMapper->setMapping(comboBox, index);
        lineEdit = qobject_cast<QLineEdit *>(enterNotifyEvents->cellWidget(index, 1));
        exitEventPayloadSignalMapper->blockSignals(true);
        comboBox->setCurrentIndex(exitEvents->events.at(index).id + 1);
        exitEventPayloadSignalMapper->blockSignals(false);
        exitEventPayloadSignalMapper->setMapping(comboBox, index);
    }
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
            generatorTable->loadTable(static_cast<BehaviorFile *>(bsData->getParentFile()));
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
                    transitions->setItem(i, 2, new QTableWidgetItem("Edit"));
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
