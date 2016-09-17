#include "stateui.h"
#include "StateUI.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/generators/hkbstatemachine.h"
#include "src/hkxclasses/generators/hkbstatemachinestateinfo.h"
#include "src/ui/genericdatawidgets.h"
#include "src/ui/transitionsui.h"
#include "src/filetypes/hkxfile.h"
#include "src/ui/behaviorgraphui/behaviorgraphui.h"
#include "src/ui/behaviorgraphui/behaviorgraphicons.h"
#include "src/hkxclasses/hkbvariablebindingset.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QLineEdit>
#include <QComboBox>
#include <QHeaderView>
#include <QSpinBox>
#include <QStackedLayout>
#include <QSignalMapper>

/*
 * StateUI
 */

QStringList StateUI::headerLabels1 = {
    "Name",
    "Type",
    "Value"
};

StateUI::StateUI()
    : behaviorView(NULL),
      bsData(NULL),
      lyt(new QVBoxLayout),
      table(new QTableWidget),
      name(new QLineEdit),
      generator(new QComboBox),
      stateId(new QSpinBox),
      probability(new QDoubleSpinBox),
      enable(new QCheckBox),
      enterNotifyEvents(new QTableWidget),
      exitNotifyEvents(new QTableWidget),
      transitions(new QTableWidget),
      stackLyt(new QStackedLayout),
      addEnterEventPB(new QPushButton("Add Enter Event")),
      removeEnterEventPB(new QPushButton("Remove Selected Enter Event")),
      typeEnterEventCB(new QComboBox),
      enterEventSignalMapper(new QSignalMapper),
      enterEventButtonLyt(new QHBoxLayout),
      addExitEventPB(new QPushButton("Add Exit Event")),
      removeExitEventPB(new QPushButton("Remove Selected Exit Event")),
      typeExitEventCB(new QComboBox),
      exitEventSignalMapper(new QSignalMapper),
      exitEventButtonLyt(new QHBoxLayout),
      addTransitionPB(new QPushButton("Add Transition")),
      removeTransitionPB(new QPushButton("Remove Selected Transition")),
      typeTransitionCB(new QComboBox),
      //transitionSignalMapper(new QSignalMapper),
      transitionButtonLyt(new QHBoxLayout),
      transitionWidget(new TransitionsUI)
{
    setTitle("hkbStateMachineStateInfo");
    stackLyt->addWidget(transitions);
    stackLyt->addWidget(transitionWidget);
    stackLyt->setCurrentIndex(TRANSITION_TABLE);
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
    table->setCellWidget(0, 0, name);
    table->setItem(0, 1, new QTableWidgetItem("String"));
    table->setItem(0, 2, new QTableWidgetItem("N/A"));
    table->setItem(1, 0, new QTableWidgetItem("generator"));
    table->setItem(1, 1, new QTableWidgetItem("hkbGenerator"));
    table->setCellWidget(1, 2, generator);
    table->setItem(2, 0, new QTableWidgetItem("stateId"));
    table->setItem(2, 1, new QTableWidgetItem("Int"));
    table->setCellWidget(2, 2, stateId);
    table->setItem(3, 0, new QTableWidgetItem("probability"));
    table->setItem(3, 1, new QTableWidgetItem("Double"));
    table->setCellWidget(3, 2, probability);
    table->setItem(4, 0, new QTableWidgetItem("enable"));
    table->setItem(4, 1, new QTableWidgetItem("Bool"));
    table->setCellWidget(4, 2, enable);
    table->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    lyt->addWidget(table);
    lyt->addLayout(enterEventButtonLyt);
    lyt->addWidget(enterNotifyEvents);
    lyt->addLayout(exitEventButtonLyt);
    lyt->addWidget(exitNotifyEvents);
    lyt->addLayout(transitionButtonLyt);
    lyt->addLayout(stackLyt);
    setLayout(lyt);
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    connect(generator, SIGNAL(currentIndexChanged(int)), this, SLOT(setGenerator(int)));
    connect(stateId, SIGNAL(editingFinished()), this, SLOT(setStateId()));
    connect(probability, SIGNAL(editingFinished()), this, SLOT(setProbability()));
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    connect(enterEventSignalMapper, SIGNAL(mapped(int)), this, SLOT(setEnterEventAt(int)));
    connect(exitEventSignalMapper, SIGNAL(mapped(int)), this, SLOT(setExitEventAt(int)));
    connect(addEnterEventPB, SIGNAL(released()), this, SLOT(addEnterEvent()));
    connect(removeEnterEventPB, SIGNAL(released()), this, SLOT(removeEnterEvent(int)));
    connect(addExitEventPB, SIGNAL(released()), this, SLOT(addExitEvent()));
    connect(removeExitEventPB, SIGNAL(released()), this, SLOT(removeExitEvent()));
    connect(addTransitionPB, SIGNAL(released()), this, SLOT(addTransition()));
    connect(removeTransitionPB, SIGNAL(released()), this, SLOT(removeTransition()));
    connect(transitions, SIGNAL(cellClicked(int,int)), this, SLOT(viewTransition(int,int)));
    connect(transitionWidget, SIGNAL(returnToParent()), this, SLOT(viewTransitionTable()));
}

void StateUI::addGeneratorToLists(const QString & name){
    disconnect(generator, 0, this, 0);
    generator->insertItem(generator->count(), name);
    connect(generator, SIGNAL(currentIndexChanged(int)), this, SLOT(setGenerator(int)));
}

void StateUI::removeGeneratorFromLists(int index){
    disconnect(generator, 0, this, 0);
    generator->removeItem(index);
    connect(generator, SIGNAL(currentIndexChanged(int)), this, SLOT(setGenerator(int)));
}

void StateUI::renameGeneratorInLists(const QString & name, int index){
    index++;
    generator->setItemText(index, name);
}

void StateUI::addEventToLists(const QString & name){
    //
}

void StateUI::removeEventFromLists(int index){
    index++;
    //
}

void StateUI::renameEventInLists(const QString & newName, int index){
    index++;
    //
}

void StateUI::setName(){
    if (bsData){
        bsData->name = name->text();
        //bsData->updateIconNames();
        emit stateNameChanged(name->text(), bsData->getParentFile()->getIndexOfGenerator(bsData) + 1);
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
    //
}

void StateUI::setEnterEventAt(int index){
    //
}

void StateUI::removeEnterEvent(int index){
    //
}

void StateUI::addExitEvent(){
    //
}

void StateUI::setExitEventAt(int index){
    //
}

void StateUI::removeExitEvent(int index){
    //
}

void StateUI::addTransition(){
    //
}

void StateUI::removeTransition(int index){
    //
}

void StateUI::viewTransition(int row, int column){
    //
}

void StateUI::viewTransitionTable(){
    //
}

void StateUI::setGenerator(int index){
    if (behaviorView && index > -1 && index < generator->count()){
        HkxObject *ptr = bsData->getParentFile()->getGeneratorDataAt(generator->currentIndex() - 1);
        if (!behaviorView->selectedIcon->getChildIcon(ptr)){
            if (/*index < 1 || */!ptr || ptr == bsData || !behaviorView->reconnectBranch(NULL, ptr, behaviorView->getSelectedItem())){
                QMessageBox msg;
                msg.setText("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to create a circular branch or dead end!!!");
                msg.exec();
                int i = bsData->getParentFile()->getIndexOfGenerator(bsData->generator);
                i++;
                disconnect(generator, 0, this, 0);
                generator->setCurrentIndex(i);
                connect(generator, SIGNAL(currentIndexChanged(int)), this, SLOT(setGenerator(int)));
                return;
            }
        }
        if (index > 0){
            bsData->generator = HkxObjectExpSharedPtr(ptr);
            behaviorView->removeGeneratorData();
        }
    }
}

void StateUI::loadData(HkxObject *data){
    if (data && data->getSignature() == HKB_STATE_MACHINE_STATE_INFO){
        bsData = static_cast<hkbStateMachineStateInfo *>(data);
        //hkbst *gen;
        //name->setText(bsData->name);
        /*for (int i = 0; i < bsData->enterNotifyEvents.size(); i++){
            gen = static_cast<hkbGenerator *>(bsData->enterNotifyEvents.at(i).data());
            if (bsData->generators.at(i).data()){
                if (i >= generators->rowCount()){
                    generators->setRowCount(generators->rowCount() + 1);
                    generators->setItem(i, 0, new QTableWidgetItem(gen->getName()));
                    generators->setItem(i, 1, new QTableWidgetItem(gen->getClassname()));
                    comboBox = new QComboBox;
                    comboBox->addItems(genList);
                    generators->setCellWidget(i, 2, comboBox);
                    index = bsData->getParentFile()->getIndexOfGenerator(bsData->generators.at(i)) + 1;
                    if (index > -1 && index < comboBox->count()){
                        comboBox->setCurrentIndex(index);
                    }
                    connect(comboBox, SIGNAL(currentIndexChanged(int)), signalMapper, SLOT(map()));
                    signalMapper->setMapping(comboBox, i);
                }else{
                    generators->setRowHidden(i, false);
                    generators->item(i, 0)->setText(gen->getName());
                    generators->item(i, 1)->setText(gen->getClassname());
                    comboBox = qobject_cast<QComboBox *>(generators->cellWidget(i, 2));
                    index = bsData->getParentFile()->getIndexOfGenerator(bsData->generators.at(i)) + 1;
                    if (index > -1 && index < comboBox->count()){
                        disconnect(signalMapper, 0, this, 0);
                        comboBox->setCurrentIndex(index);
                        connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(setGeneratorAt(int)));
                    }
                    signalMapper->setMapping(comboBox, i);
                }
            }
        }
        for (int j = bsData->generators.size(); j < generators->rowCount(); j++){
            generators->setRowHidden(j, true);
        }
        selectedGeneratorIndex->setValue(bsData->selectedGeneratorIndex);
        currentGeneratorIndex->setValue(bsData->currentGeneratorIndex);*/


        name->setText(bsData->name);
        disconnect(generator, 0, this, 0);
        if (generator->count() == 0){
            QStringList genList = bsData->getParentFile()->getGeneratorNames();
            genList.prepend("None");
            generator->insertItems(0, genList);
        }
        int index = bsData->getParentFile()->getIndexOfGenerator(bsData->generator) + 1;
        generator->setCurrentIndex(index);
        connect(generator, SIGNAL(currentIndexChanged(int)), this, SLOT(setGenerator(int)));
    }
}
