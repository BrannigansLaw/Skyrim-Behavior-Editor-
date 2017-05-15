#include "stateui.h"
#include "StateUI.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/generators/hkbstatemachine.h"
#include "src/hkxclasses/behavior/generators/hkbstatemachinestateinfo.h"
#include "src/hkxclasses/behavior/hkbstatemachineeventpropertyarray.h"
#include "src/hkxclasses/behavior/hkbstringeventpayload.h"
#include "src/ui/genericdatawidgets.h"
#include "src/ui/hkxclassesui/behaviorui/transitionsui.h"
#include "src/ui/hkxclassesui/behaviorui/eventui.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/behaviorgraphview.h"
#include "src/ui/treegraphicsitem.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"

#include <QGridLayout>
#include <QHeaderView>
#include <QStackedLayout>

#define BASE_NUMBER_OF_ROWS 8

#define NAME_ROW 0
#define STATE_ID_ROW 1
#define PROBABILITY_ROW 2
#define ENABLE_ROW 3
#define GENERATOR_ROW 4
#define ADD_ENTER_EVENT_ROW 5

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
      stateIndex(-1),
      exitEventsButtonRow(ADD_ENTER_EVENT_ROW + 1),
      transitionsButtonRow(ADD_ENTER_EVENT_ROW + 2),
      groupBox(new QGroupBox("hkbStateMachineStateInfo")),
      topLyt(new QGridLayout),
      transitionUI(new TransitionsUI()),
      eventUI(new EventUI()),
      table(new TableWidget),
      returnPB(new QPushButton("Return")),
      name(new LineEdit),
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
    table->setItem(NAME_ROW, NAME_COLUMN, new TableWidgetItem("name"));
    table->setItem(NAME_ROW, TYPE_COLUMN, new TableWidgetItem("hkStringPtr", Qt::AlignCenter));
    table->setItem(NAME_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(NAME_ROW, VALUE_COLUMN, name);
    table->setItem(STATE_ID_ROW, NAME_COLUMN, new TableWidgetItem("id"));
    table->setItem(STATE_ID_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(STATE_ID_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(STATE_ID_ROW, VALUE_COLUMN, stateId);
    table->setItem(PROBABILITY_ROW, NAME_COLUMN, new TableWidgetItem("probability"));
    table->setItem(PROBABILITY_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(PROBABILITY_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(PROBABILITY_ROW, VALUE_COLUMN, probability);
    table->setItem(ENABLE_ROW, NAME_COLUMN, new TableWidgetItem("enable"));
    table->setItem(ENABLE_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(ENABLE_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(ENABLE_ROW, VALUE_COLUMN, enable);
    table->setItem(GENERATOR_ROW, NAME_COLUMN, new TableWidgetItem("generator"));
    table->setItem(GENERATOR_ROW, TYPE_COLUMN, new TableWidgetItem("hkbGenerator", Qt::AlignCenter));
    table->setItem(GENERATOR_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(GENERATOR_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(219, 219, 219), QBrush(Qt::black), "Click to view the list of generators"));
    table->setCellWidget(ADD_ENTER_EVENT_ROW, NAME_COLUMN, addEnterEventPB);
    table->setItem(ADD_ENTER_EVENT_ROW, TYPE_COLUMN, new TableWidgetItem("hkEvent", Qt::AlignCenter, QColor(219, 219, 219), QBrush(Qt::black)));
    table->setCellWidget(ADD_ENTER_EVENT_ROW, BINDING_COLUMN, removeEnterEventPB);
    table->setItem(ADD_ENTER_EVENT_ROW, VALUE_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(ADD_ENTER_EVENT_ROW + 1, NAME_COLUMN, addExitEventPB);
    table->setItem(ADD_ENTER_EVENT_ROW + 1, TYPE_COLUMN, new TableWidgetItem("hkEvent", Qt::AlignCenter, QColor(219, 219, 219), QBrush(Qt::black)));
    table->setCellWidget(ADD_ENTER_EVENT_ROW + 1, BINDING_COLUMN, removeExitEventPB);
    table->setItem(ADD_ENTER_EVENT_ROW + 1, VALUE_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(ADD_ENTER_EVENT_ROW + 2, NAME_COLUMN, addTransitionPB);
    table->setItem(ADD_ENTER_EVENT_ROW + 2, TYPE_COLUMN, new TableWidgetItem("hkbStateMachineTransitionInfoArray", Qt::AlignCenter, QColor(219, 219, 219), QBrush(Qt::black)));
    table->setCellWidget(ADD_ENTER_EVENT_ROW + 2, BINDING_COLUMN, removeTransitionPB);
    table->setItem(ADD_ENTER_EVENT_ROW + 2, VALUE_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    topLyt->addWidget(returnPB, 0, 1, 1, 1);
    topLyt->addWidget(table, 1, 0, 8, 3);
    groupBox->setLayout(topLyt);
    //Order here must correspond with the ACTIVE_WIDGET Enumerated type!!!
    addWidget(groupBox);
    addWidget(eventUI);
    addWidget(transitionUI);
    connect(returnPB, SIGNAL(pressed()), this, SIGNAL(returnToParent()), Qt::UniqueConnection);
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(stateId, SIGNAL(editingFinished()), this, SLOT(setStateId()), Qt::UniqueConnection);
    connect(probability, SIGNAL(editingFinished()), this, SLOT(setProbability()), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellClicked(int,int)), this, SLOT(viewSelectedChild(int,int)), Qt::UniqueConnection);
    connect(addEnterEventPB, SIGNAL(released()), this, SLOT(addEnterEvent()), Qt::UniqueConnection);
    connect(removeEnterEventPB, SIGNAL(released()), this, SLOT(removeObjectChild()), Qt::UniqueConnection);
    connect(addExitEventPB, SIGNAL(released()), this, SLOT(addExitEvent()), Qt::UniqueConnection);
    connect(removeExitEventPB, SIGNAL(released()), this, SLOT(removeObjectChild()), Qt::UniqueConnection);
    connect(addTransitionPB, SIGNAL(released()), this, SLOT(addTransition()), Qt::UniqueConnection);
    connect(removeTransitionPB, SIGNAL(released()), this, SLOT(removeObjectChild()), Qt::UniqueConnection);
    connect(eventUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()), Qt::UniqueConnection);
    connect(eventUI, SIGNAL(viewEvents(int)), this, SIGNAL(viewEvents(int)), Qt::UniqueConnection);
    connect(transitionUI, SIGNAL(viewVariables(int)), this, SIGNAL(viewVariables(int)), Qt::UniqueConnection);
    connect(transitionUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()), Qt::UniqueConnection);
}

void StateUI::loadData(HkxObject *data, int stateindex){
    blockSignals(true);
    stateIndex = -1;
    setCurrentIndex(MAIN_WIDGET);
    if (data && data->getSignature() == HKB_STATE_MACHINE_STATE_INFO){
        bsData = static_cast<hkbStateMachineStateInfo *>(data);
        stateIndex = stateindex;
        rowToRemove = -1;
        name->setText(bsData->name);
        stateId->setValue(bsData->stateId);
        probability->setValue(bsData->probability);
        enable->setChecked(bsData->enable);
        if (bsData->generator.data()){
            table->item(GENERATOR_ROW, VALUE_COLUMN)->setText(static_cast<hkbGenerator *>(bsData->generator.data())->getName());
        }else{
            table->item(GENERATOR_ROW, VALUE_COLUMN)->setText("NONE");
        }
        loadDynamicTableRows();
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateUI::loadData(): The data is NULL or an incorrect type!!"));
    }
    blockSignals(false);
}

void StateUI::loadDynamicTableRows(){
    if (bsData){
        hkbStateMachineEventPropertyArray *events = static_cast<hkbStateMachineEventPropertyArray *>(bsData->enterNotifyEvents.data());
        int temp;
        if (events){
            temp = ADD_ENTER_EVENT_ROW + events->events.size() + 1 - exitEventsButtonRow;
        }else{
            temp = ADD_ENTER_EVENT_ROW + 1 - exitEventsButtonRow;
        }
        if (temp > 0){
            for (int i = 0; i < temp; i++){
                table->insertRow(exitEventsButtonRow);
                exitEventsButtonRow++;
                transitionsButtonRow++;
            }
        }else if (temp < 0){
            for (int i = temp; i < 0; i++){
                table->removeRow(exitEventsButtonRow - 1);
                exitEventsButtonRow--;
                transitionsButtonRow--;
            }
        }
        events = static_cast<hkbStateMachineEventPropertyArray *>(bsData->exitNotifyEvents.data());
        if (events){
            temp = exitEventsButtonRow + events->events.size() + 1 - transitionsButtonRow;
        }else{
            temp = exitEventsButtonRow + 1 - transitionsButtonRow;
        }
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
        QString eventName;
        events = static_cast<hkbStateMachineEventPropertyArray *>(bsData->enterNotifyEvents.data());
        if (events){
            for (int i = ADD_ENTER_EVENT_ROW + 1, j = 0; i < exitEventsButtonRow, j < events->events.size(); i++, j++){
                eventName = static_cast<BehaviorFile *>(bsData->getParentFile())->getEventNameAt(events->events.at(j).id);
                if (eventName != ""){
                    setRowItems(i, eventName, events->getClassname(), "N/A", "Click to Edit");
                }else{
                    CRITICAL_ERROR_MESSAGE(QString("StateUI::loadDynamicTableRows(): Invalid event name!!!"));
                }
            }
        }
        events = static_cast<hkbStateMachineEventPropertyArray *>(bsData->exitNotifyEvents.data());
        if (events){
            for (int i = exitEventsButtonRow + 1, j = 0; i < transitionsButtonRow, j < events->events.size(); i++, j++){
                eventName = static_cast<BehaviorFile *>(bsData->getParentFile())->getEventNameAt(events->events.at(j).id);
                if (eventName != ""){
                    setRowItems(i, eventName, events->getClassname(), "N/A", "Click to Edit");
                }else{
                    CRITICAL_ERROR_MESSAGE(QString("StateUI::loadDynamicTableRows(): Invalid event name!!!"));
                }
            }
        }
        hkbStateMachineTransitionInfoArray *trans = static_cast<hkbStateMachineTransitionInfoArray *>(bsData->transitions.data());
        if (trans){
            table->setRowCount(transitionsButtonRow + trans->getNumTransitions() + 1);
            for (int i = transitionsButtonRow + 1, j = 0; i < table->rowCount(), j < trans->getNumTransitions(); i++, j++){
                setRowItems(i, trans->getTransitionNameAt(j), trans->getClassname(), "N/A", "Click to Edit");
            }
        }else{
            table->setRowCount(transitionsButtonRow + 1);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateUI::loadDynamicTableRows(): The data is NULL!!"));
    }
}


void StateUI::setRowItems(int row, const QString & name, const QString & classname, const QString & bind, const QString & value){
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
        table->setItem(row, BINDING_COLUMN, new TableWidgetItem(bind, Qt::AlignCenter));
    }
    if (table->item(row, VALUE_COLUMN)){
        table->item(row, VALUE_COLUMN)->setText(value);
    }else{
        table->setItem(row, VALUE_COLUMN, new TableWidgetItem(value, Qt::AlignCenter, QColor(219, 219, 219), QBrush(Qt::black)));
    }
}

void StateUI::setName(){
    if (bsData){
        bsData->name = name->text();
        bsData->getParentFile()->toggleChanged(true);
        emit stateNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData) + 1);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateUI::setName(): The data is NULL!!"));
    }
}

void StateUI::setStateId(){
    if (bsData){
        bsData->setStateId(stateId->value());//Update parent state machine???
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateUI::setStateId(): The data is NULL!!"));
    }
}

void StateUI::setProbability(){
    if (bsData){
        bsData->probability = probability->value();
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateUI::setProbability(): The data is NULL!!"));
    }
}

void StateUI::setEnable(){
    if (bsData){
        bsData->enable = enable->isChecked();
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateUI::setEnable(): The data is NULL!!"));
    }
}

void StateUI::setEvent(int index, const QString & name){
    eventUI->setEvent(index, name);
    loadDynamicTableRows(); //Inefficient...
}

void StateUI::addEnterEvent(){
    hkbStateMachineEventPropertyArray *enterEvents = NULL;
    if (bsData){
        enterEvents = static_cast<hkbStateMachineEventPropertyArray *>(bsData->enterNotifyEvents.data());
        if (!enterEvents){
            enterEvents = new hkbStateMachineEventPropertyArray(bsData->getParentFile(), -1);
            bsData->getParentFile()->addObjectToFile(enterEvents, -1);
            bsData->enterNotifyEvents = HkxSharedPtr(enterEvents);
        }
        enterEvents->addEvent();
        enterEvents->events.last().id = 0;
        loadDynamicTableRows();
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateUI::addEnterEvent(): The data is NULL!!"));
    }
}

void StateUI::addExitEvent(){
    hkbStateMachineEventPropertyArray *exitEvents = NULL;
    if (bsData){
        exitEvents = static_cast<hkbStateMachineEventPropertyArray *>(bsData->exitNotifyEvents.data());
        if (!exitEvents){
            exitEvents = new hkbStateMachineEventPropertyArray(bsData->getParentFile(), -1);
            bsData->getParentFile()->addObjectToFile(exitEvents, -1);
            bsData->exitNotifyEvents = HkxSharedPtr(exitEvents);
        }
        exitEvents->addEvent();
        exitEvents->events.last().id = 0;
        loadDynamicTableRows();
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateUI::addExitEvent(): The data is NULL!!"));
    }
}

void StateUI::removeObjectChild(){
    int result = -1;
    hkbStateMachineEventPropertyArray *enterEvents = NULL;
    hkbStateMachineEventPropertyArray *exitEvents = NULL;
    hkbStateMachineTransitionInfoArray *trans = NULL;
    if (bsData){
        enterEvents = static_cast<hkbStateMachineEventPropertyArray *>(bsData->enterNotifyEvents.data());
        exitEvents = static_cast<hkbStateMachineEventPropertyArray *>(bsData->exitNotifyEvents.data());
        if (rowToRemove != transitionsButtonRow && rowToRemove != exitEventsButtonRow && rowToRemove != ADD_ENTER_EVENT_ROW){
            trans = static_cast<hkbStateMachineTransitionInfoArray *>(bsData->transitions.data());
            if (rowToRemove >= BASE_NUMBER_OF_ROWS + enterEvents->events.size() + exitEvents->events.size()){
                if (trans){
                    result = rowToRemove - BASE_NUMBER_OF_ROWS - enterEvents->events.size() - exitEvents->events.size();
                    if (result != transitionsButtonRow && result < trans->getNumTransitions() && result >= 0){
                        trans->removeTransition(result);
                    }else{
                        WARNING_MESSAGE(QString("StateUI::removeObjectChild(): Invalid row index selected!!"));
                        return;
                    }
                }else{
                    WARNING_MESSAGE(QString("StateUI::removeObjectChild(): Transition data is NULL!!"));
                    return;
                }
            }else if (rowToRemove > exitEventsButtonRow){
                result = rowToRemove - exitEventsButtonRow - 1;
                if (result < exitEvents->events.size() && result >= 0){
                    exitEvents->removeEvent(result);
                    rowToRemove = -1;
                }else{
                    WARNING_MESSAGE(QString("StateUI::removeObjectChild(): Invalid index of child to remove!!"));
                }
            }else if (rowToRemove > ADD_ENTER_EVENT_ROW){
                result = rowToRemove - ADD_ENTER_EVENT_ROW - 1;
                if (result < enterEvents->events.size() && result >= 0){
                    enterEvents->removeEvent(result);
                    rowToRemove = -1;
                }else{
                    WARNING_MESSAGE(QString("StateUI::removeObjectChild(): Invalid index of child to remove!!"));
                }
            }else{
                WARNING_MESSAGE(QString("StateUI::removeObjectChild(): Invalid index of child to remove!!"));
            }
            loadDynamicTableRows();
        }else{
            WARNING_MESSAGE(QString("StateUI::removeObjectChild(): Attempting to remove button row!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateUI::removeObjectChild(): The data or behavior graph pointer is NULL!!"));
    }
}

void StateUI::addTransition(){
    hkbStateMachineTransitionInfoArray *trans = NULL;
    if (bsData){
        trans = static_cast<hkbStateMachineTransitionInfoArray *>(bsData->transitions.data());
        if (!trans){
            trans = new hkbStateMachineTransitionInfoArray(bsData->getParentFile(), bsData->getParentStateMachine(), -1);
            bsData->getParentFile()->addObjectToFile(trans, -1);
            bsData->transitions = HkxSharedPtr(trans);
        }
        trans->addTransition();
        loadDynamicTableRows();
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateUI::addTransition(): The data is NULL!!"));
    }
}

void StateUI::viewSelectedChild(int row, int column){
    int result;
    int count = 0;
    hkbStateMachineTransitionInfoArray *trans = NULL;
    hkbStateMachineEventPropertyArray *enterEvents = NULL;
    hkbStateMachineEventPropertyArray *exitEvents = NULL;
    if (bsData){
        if (row == GENERATOR_ROW && column == VALUE_COLUMN){
            emitViewGenerators();
        }else if (row > ADD_ENTER_EVENT_ROW && row < exitEventsButtonRow){
            enterEvents = static_cast<hkbStateMachineEventPropertyArray *>(bsData->enterNotifyEvents.data());
            result = row - ADD_ENTER_EVENT_ROW - 1;
            rowToRemove = row;
            if (enterEvents->events.size() > result && result >= 0){
                if (column == VALUE_COLUMN){
                    eventUI->loadData(static_cast<BehaviorFile *>(bsData->getParentFile()), &enterEvents->events[result]);
                    setCurrentIndex(EVENT_PAYLOAD_WIDGET);
                }
            }else{
                CRITICAL_ERROR_MESSAGE(QString("StateUI::viewSelectedChild(): Invalid index of child to view!!"));
            }
        }else if (row > exitEventsButtonRow && row < transitionsButtonRow){
            exitEvents = static_cast<hkbStateMachineEventPropertyArray *>(bsData->exitNotifyEvents.data());
            result = row - exitEventsButtonRow - 1;
            rowToRemove = row;
            if (exitEvents->events.size() > result && result >= 0){
                if (column == VALUE_COLUMN){
                    eventUI->loadData(static_cast<BehaviorFile *>(bsData->getParentFile()), &exitEvents->events[result]);
                    setCurrentIndex(EVENT_PAYLOAD_WIDGET);
                }
            }else{
                CRITICAL_ERROR_MESSAGE(QString("StateUI::viewSelectedChild(): Invalid index of child to view!!"));
            }
        }else if (row > exitEventsButtonRow && row < table->rowCount()){
            enterEvents = static_cast<hkbStateMachineEventPropertyArray *>(bsData->enterNotifyEvents.data());
            if (enterEvents){
                count = enterEvents->events.size();
            }
            exitEvents = static_cast<hkbStateMachineEventPropertyArray *>(bsData->exitNotifyEvents.data());
            if (exitEvents){
                count = count + exitEvents->events.size();
            }
            result = row - BASE_NUMBER_OF_ROWS - count;
            rowToRemove = row;
            trans = static_cast<hkbStateMachineTransitionInfoArray *>(bsData->transitions.data());
            if (trans && result < trans->getNumTransitions() && result >= 0){
                if (column == VALUE_COLUMN){
                    transitionUI->loadData(static_cast<BehaviorFile *>(bsData->getParentFile()), bsData->getParentStateMachine(), &trans->transitions[result]);
                    setCurrentIndex(TRANSITION_WIDGET);
                }
            }else{
                CRITICAL_ERROR_MESSAGE(QString("StateUI::viewSelectedChild(): Invalid index of child to view!!"));
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateUI::viewSelectedChild(): The data is NULL!!"));
    }
}

void StateUI::emitViewGenerators(){
    if (bsData && bsData->getParentFile()){
        emit viewGenerators(static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData->generator) + 1);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateUI::emitViewGenerators(): The data or parent file is NULL!!"));
    }
}

void StateUI::setGenerator(int index, const QString & name){
    hkbStateMachine *gen = NULL;
    DataIconManager *ptr = NULL;
    if (bsData){
        if (behaviorView){
            gen = static_cast<hkbStateMachine *>(bsData->getParentStateMachine());
            ptr = static_cast<BehaviorFile *>(bsData->getParentFile())->getGeneratorDataAt(index - 1);
            if (ptr && name != ptr->getName()){
                CRITICAL_ERROR_MESSAGE(QString("The name of the selected object does not match it's name in the object selection table!!!"));
            }else if (!gen){
                CRITICAL_ERROR_MESSAGE(QString("The currently loaded 'hkbStateMachineStateInfo' has no parent 'hkbStateMachine'!!!"));
            }else if (ptr == bsData || !behaviorView->reconnectIcon(behaviorView->getSelectedItem(), (DataIconManager *)bsData->generator.data(), ptr, false)){
                WARNING_MESSAGE(QString("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\nYou are attempting to create a circular branch or dead end!!!"));
            }else{
                if (behaviorView->getSelectedItem()){
                    behaviorView->removeItemFromGraph(behaviorView->getSelectedItem()->getChildWithData((DataIconManager *)bsData->generator.data()), stateIndex);
                }else{
                    CRITICAL_ERROR_MESSAGE(QString("StateUI::setGenerator(): The selected icon is NULL!!"));
                }
                if (bsData->parentSM.data()){
                    static_cast<hkbStateMachine *>(bsData->parentSM.data())->removeState(stateIndex);
                }else{
                    CRITICAL_ERROR_MESSAGE(QString("StateUI::setGenerator(): The state is orphaned!!"));
                }
                behaviorView->removeGeneratorData();
                table->item(GENERATOR_ROW, VALUE_COLUMN)->setText(name);
                bsData->getParentFile()->toggleChanged(true);
                emit returnToParent(true);
            }
        }else{
            CRITICAL_ERROR_MESSAGE(QString("StateUI::setGenerator(): The 'behaviorView' pointer is NULL!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateUI::setGenerator(): The 'bsData' pointer is NULL!!"));
    }
}

void StateUI::setBehaviorView(BehaviorGraphView *view){
    behaviorView = view;
}

void StateUI::eventRenamed(const QString & name, int index){
    if (bsData){
        eventUI->eventRenamed(name, index);
        loadDynamicTableRows(); //Inefficient...
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateUI::eventRenamed(): The data is NULL!!"));
    }
}

void StateUI::generatorRenamed(const QString & name, int index){
    if (bsData){
        index--;
        if (index == static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData->generator)){
            table->item(GENERATOR_ROW, VALUE_COLUMN)->setText(name);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateUI::generatorRenamed(): The 'bsData' pointer is NULL!!"));
    }
}

void StateUI::returnToWidget(){
    setCurrentIndex(MAIN_WIDGET);
}
