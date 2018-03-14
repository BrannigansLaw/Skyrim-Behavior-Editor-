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
#include "src/ui/mainwindow.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"

#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 8

#define NAME_ROW 0
#define STATE_ID_ROW 1
#define PROBABILITY_ROW 2
#define ENABLE_ROW 3
#define GENERATOR_ROW 4
#define ADD_ENTER_EVENT_ROW 5
#define INITIAL_EXIT_EVENT_ROW 6
#define INITIAL_ADD_TRANSITION_ROW 7

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList StateUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

StateUI::StateUI()
    : behaviorView(nullptr),
      bsData(nullptr),
      stateIndex(-1),
      exitEventsButtonRow(INITIAL_EXIT_EVENT_ROW),
      transitionsButtonRow(INITIAL_ADD_TRANSITION_ROW),
      groupBox(new QGroupBox("hkbStateMachineStateInfo")),
      topLyt(new QGridLayout),
      transitionUI(new TransitionsUI()),
      eventUI(new EventUI()),
      table(new TableWidget(QColor(Qt::white))),
      returnPB(new QPushButton("Return")),
      name(new LineEdit),
      stateId(new SpinBox),
      probability(new DoubleSpinBox),
      enable(new CheckBox),
      enableTransitions(new CheckBox("Enable Transitions"))
{
    stateId->setReadOnly(true);
    transitionUI->flagGlobalWildcard->setDisabled(true);
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
    table->setItem(ENABLE_ROW, VALUE_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter, QColor(Qt::lightGray)));
    table->setCellWidget(ENABLE_ROW, VALUE_COLUMN, enable);
    table->setItem(GENERATOR_ROW, NAME_COLUMN, new TableWidgetItem("generator"));
    table->setItem(GENERATOR_ROW, TYPE_COLUMN, new TableWidgetItem("hkbGenerator", Qt::AlignCenter));
    table->setItem(GENERATOR_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(GENERATOR_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_GENERATORS_TABLE_TIP));
    table->setItem(ADD_ENTER_EVENT_ROW, NAME_COLUMN, new TableWidgetItem("Add Enter Event", Qt::AlignCenter, QColor(Qt::green), QBrush(Qt::black), "Double click to add a event to be emitted when this state is entered"));
    table->setItem(ADD_ENTER_EVENT_ROW, TYPE_COLUMN, new TableWidgetItem("hkbStateMachineEventPropertyArray", Qt::AlignCenter, QColor(Qt::gray)));
    table->setItem(ADD_ENTER_EVENT_ROW, BINDING_COLUMN, new TableWidgetItem("Remove Selected Enter Event", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black)));
    table->setItem(ADD_ENTER_EVENT_ROW, VALUE_COLUMN, new TableWidgetItem("Edit Selected Enter Event", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black)));
    table->setItem(INITIAL_EXIT_EVENT_ROW, NAME_COLUMN, new TableWidgetItem("Add Exit Event", Qt::AlignCenter, QColor(Qt::green), QBrush(Qt::black), "Double click to add a event to be emitted when this state is exited"));
    table->setItem(INITIAL_EXIT_EVENT_ROW, TYPE_COLUMN, new TableWidgetItem("hkbStateMachineEventPropertyArray", Qt::AlignCenter, QColor(Qt::gray)));
    table->setItem(INITIAL_EXIT_EVENT_ROW, BINDING_COLUMN, new TableWidgetItem("Remove Selected Exit Event", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black)));
    table->setItem(INITIAL_EXIT_EVENT_ROW, VALUE_COLUMN, new TableWidgetItem("Edit Selected Exit Event", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black)));
    table->setItem(INITIAL_ADD_TRANSITION_ROW, NAME_COLUMN, new TableWidgetItem("Add Transition", Qt::AlignCenter, QColor(Qt::green), QBrush(Qt::black), "Double click to add a new transition"));
    table->setItem(INITIAL_ADD_TRANSITION_ROW, TYPE_COLUMN, new TableWidgetItem("hkbStateMachineTransitionInfoArray", Qt::AlignCenter, QColor(Qt::gray)));
    table->setItem(INITIAL_ADD_TRANSITION_ROW, BINDING_COLUMN, new TableWidgetItem("Remove Selected Transition", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black)));
    table->setItem(INITIAL_ADD_TRANSITION_ROW, VALUE_COLUMN, new TableWidgetItem("Edit Selected Transition", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black)));
    topLyt->addWidget(returnPB, 0, 1, 1, 1);
    topLyt->addWidget(table, 1, 0, 8, 3);
    groupBox->setLayout(topLyt);
    //Order here must correspond with the ACTIVE_WIDGET Enumerated type!!!
    addWidget(groupBox);
    addWidget(eventUI);
    addWidget(transitionUI);
    connectSignals();
}

void StateUI::connectSignals(){
    connect(returnPB, SIGNAL(clicked(bool)), this, SIGNAL(returnToParent(bool)), Qt::UniqueConnection);
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(stateId, SIGNAL(editingFinished()), this, SLOT(setStateId()), Qt::UniqueConnection);
    connect(probability, SIGNAL(editingFinished()), this, SLOT(setProbability()), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)), Qt::UniqueConnection);
    connect(eventUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()), Qt::UniqueConnection);
    connect(eventUI, SIGNAL(viewEvents(int)), this, SIGNAL(viewEvents(int)), Qt::UniqueConnection);
    connect(transitionUI, SIGNAL(viewEvents(int)), this, SIGNAL(viewEvents(int)), Qt::UniqueConnection);
    connect(transitionUI, SIGNAL(viewVariables(int)), this, SIGNAL(viewVariables(int)), Qt::UniqueConnection);
    connect(transitionUI, SIGNAL(viewProperties(int)), this, SIGNAL(viewProperties(int)), Qt::UniqueConnection);
    connect(transitionUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()), Qt::UniqueConnection);
    connect(transitionUI, SIGNAL(transitionNamChanged(QString,int)), this, SLOT(transitionRenamed(QString,int)), Qt::UniqueConnection);
}

void StateUI::disconnectSignals(){
    disconnect(returnPB, SIGNAL(clicked(bool)), this, SIGNAL(returnToParent(bool)));
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(stateId, SIGNAL(editingFinished()), this, SLOT(setStateId()));
    disconnect(probability, SIGNAL(editingFinished()), this, SLOT(setProbability()));
    disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)));
    disconnect(eventUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()));
    disconnect(eventUI, SIGNAL(viewEvents(int)), this, SIGNAL(viewEvents(int)));
    disconnect(transitionUI, SIGNAL(viewEvents(int)), this, SIGNAL(viewEvents(int)));
    disconnect(transitionUI, SIGNAL(viewVariables(int)), this, SIGNAL(viewVariables(int)));
    disconnect(transitionUI, SIGNAL(viewProperties(int)), this, SIGNAL(viewProperties(int)));
    disconnect(transitionUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()));
    disconnect(transitionUI, SIGNAL(transitionNamChanged(QString,int)), this, SLOT(transitionRenamed(QString,int)));
}

void StateUI::variableTableElementSelected(int index, const QString &name){
    if (bsData){
        switch (currentIndex()){
        case TRANSITION_WIDGET:
            transitionUI->variableTableElementSelected(index, name);
            break;
        default:
            FATAL_RUNTIME_ERROR("StateUI::variableTableElementSelected(): Event relayed to wrong widget!!");
            return;
        }
    }else{
        FATAL_RUNTIME_ERROR("StateUI::variableTableElementSelected(): The data is nullptr!!");
    }
}

void StateUI::loadData(HkxObject *data, int stateindex){
    disconnectSignals();
    setCurrentIndex(MAIN_WIDGET);
    if (data && data->getSignature() == HKB_STATE_MACHINE_STATE_INFO){
        bsData = static_cast<hkbStateMachineStateInfo *>(data);
        stateIndex = stateindex;
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
        FATAL_RUNTIME_ERROR("StateUI::loadData(): The data is nullptr or an incorrect type!!");
    }
    connectSignals();
}

void StateUI::loadDynamicTableRows(){
    if (bsData){
        hkbStateMachineEventPropertyArray *events = static_cast<hkbStateMachineEventPropertyArray *>(bsData->enterNotifyEvents.data());
        int temp;
        if (events){
            temp = ADD_ENTER_EVENT_ROW + events->events.size() + 1 - exitEventsButtonRow;
        }else{
            temp = INITIAL_EXIT_EVENT_ROW - exitEventsButtonRow;
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
            for (int i = INITIAL_EXIT_EVENT_ROW, j = 0; i < exitEventsButtonRow, j < events->events.size(); i++, j++){
                eventName = static_cast<BehaviorFile *>(bsData->getParentFile())->getEventNameAt(events->events.at(j).id);
                if (eventName != ""){
                    setRowItems(i, eventName, events->getClassname(), "Remove", "Edit", "Double click to remove this enter event", "Double click to edit this enter event");
                }else{
                    FATAL_RUNTIME_ERROR("StateUI::loadDynamicTableRows(): Invalid event name!!!");
                }
            }
        }
        events = static_cast<hkbStateMachineEventPropertyArray *>(bsData->exitNotifyEvents.data());
        if (events){
            for (int i = exitEventsButtonRow + 1, j = 0; i < transitionsButtonRow, j < events->events.size(); i++, j++){
                eventName = static_cast<BehaviorFile *>(bsData->getParentFile())->getEventNameAt(events->events.at(j).id);
                if (eventName != ""){
                    setRowItems(i, eventName, events->getClassname(), "Remove", "Edit", "Double click to remove this exit event", "Double click to edit this exit event");
                }else{
                    FATAL_RUNTIME_ERROR("StateUI::loadDynamicTableRows(): Invalid event name!!!");
                }
            }
        }
        hkbStateMachineTransitionInfoArray *trans = static_cast<hkbStateMachineTransitionInfoArray *>(bsData->transitions.data());
        if (trans){
            table->setRowCount(transitionsButtonRow + trans->getNumTransitions() + 1);
            for (int i = transitionsButtonRow + 1, j = 0; i < table->rowCount(), j < trans->getNumTransitions(); i++, j++){
                setRowItems(i, trans->getTransitionNameAt(j), trans->getClassname(), "Remove", "Edit", "Double click to remove this transition", "Double click to edit this transition");
            }
        }else{
            table->setRowCount(transitionsButtonRow + 1);
        }
    }else{
        FATAL_RUNTIME_ERROR("StateUI::loadDynamicTableRows(): The data is nullptr!!");
    }
}


void StateUI::setRowItems(int row, const QString & name, const QString & classname, const QString & bind, const QString & value, const QString & tip1, const QString & tip2){
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

void StateUI::eventTableElementSelected(int index, const QString &name){
    //index--;
    switch (currentIndex()){
    case EVENT_PAYLOAD_WIDGET:
        eventUI->setEvent(index, name);
        break;
    case TRANSITION_WIDGET:
        transitionUI->eventTableElementSelected(index, name);
        break;
    default:
        WARNING_MESSAGE("StateUI::eventTableElementSelected(): An unwanted element selected event was recieved!!");
    }
    loadDynamicTableRows();
}

void StateUI::generatorTableElementSelected(int index, const QString &name){
    switch (currentIndex()){
    case MAIN_WIDGET:
        setGenerator(index, name);
        break;
    default:
        WARNING_MESSAGE("StateUI::generatorTableElementSelected(): An unwanted element selected event was recieved!!");
    }
}

void StateUI::connectToTables(GenericTableWidget *generators, GenericTableWidget *events){
    if (generators && events){
        disconnect(events, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(generators, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(events, SIGNAL(elementSelected(int,QString)), this, SLOT(eventTableElementSelected(int,QString)), Qt::UniqueConnection);
        connect(generators, SIGNAL(elementSelected(int,QString)), this, SLOT(generatorTableElementSelected(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewGenerators(int)), generators, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewEvents(int)), events, SLOT(showTable(int)), Qt::UniqueConnection);
    }else{
        FATAL_RUNTIME_ERROR("StateUI::connectToTables(): One or more arguments are nullptr!!");
    }
}

void StateUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            bsData->getParentFile()->setIsChanged(true);
            emit stateNameChanged(name->text(), stateIndex);
            emit generatorNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData));
        }
    }else{
        FATAL_RUNTIME_ERROR("StateUI::setName(): The data is nullptr!!");
    }
}

void StateUI::setStateId(){
    if (bsData){
        if (bsData->setStateId(stateId->value())){
            emit stateIdChanged(stateIndex, bsData->stateId, bsData->name);
            bsData->getParentFile()->setIsChanged(true);
        }else{
            WARNING_MESSAGE("StateUI::setStateId(): Another state has the selected state ID!!! The state ID for this state was not changed!!!");
            disconnect(stateId, SIGNAL(editingFinished()), this, SLOT(setStateId()));
            stateId->setValue(bsData->stateId);
            connect(stateId, SIGNAL(editingFinished()), this, SLOT(setStateId()), Qt::UniqueConnection);
        }
    }else{
        FATAL_RUNTIME_ERROR("StateUI::setStateId(): The data is nullptr!!");
    }
}

void StateUI::setProbability(){
    if (bsData){
        if (bsData->probability != probability->value()){
            bsData->probability = probability->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("StateUI::setProbability(): The data is nullptr!!");
    }
}

void StateUI::setEnable(){
    if (bsData){
        bsData->enable = enable->isChecked();
        bsData->getParentFile()->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("StateUI::setEnable(): The data is nullptr!!");
    }
}

void StateUI::addEnterEvent(){
    hkbStateMachineEventPropertyArray *enterEvents = nullptr;
    if (bsData){
        enterEvents = static_cast<hkbStateMachineEventPropertyArray *>(bsData->enterNotifyEvents.data());
        if (!enterEvents){
            enterEvents = new hkbStateMachineEventPropertyArray(bsData->getParentFile(), -1);
            bsData->enterNotifyEvents = HkxSharedPtr(enterEvents);
        }
        enterEvents->addEvent();
        enterEvents->events.last().id = 0;
        loadDynamicTableRows();
    }else{
        FATAL_RUNTIME_ERROR("StateUI::addEnterEvent(): The data is nullptr!!");
    }
}

void StateUI::removeEnterEvent(int index){
    hkbStateMachineEventPropertyArray *events = nullptr;
    if (bsData){
        events = static_cast<hkbStateMachineEventPropertyArray *>(bsData->enterNotifyEvents.data());
        if (events){
            if (index < events->events.size() && index >= 0){
                events->removeEvent(index);
            }else{
                WARNING_MESSAGE("StateUI::removeEnterEvent(): Invalid row index selected!!");
                return;
            }
        }else{
            WARNING_MESSAGE("StateUI::removeEnterEvent(): Event data is nullptr!!");
            return;
        }
        loadDynamicTableRows();
    }else{
        FATAL_RUNTIME_ERROR("StateUI::removeEnterEvent(): The data is nullptr!!");
    }
}

void StateUI::addExitEvent(){
    hkbStateMachineEventPropertyArray *exitEvents = nullptr;
    if (bsData){
        exitEvents = static_cast<hkbStateMachineEventPropertyArray *>(bsData->exitNotifyEvents.data());
        if (!exitEvents){
            exitEvents = new hkbStateMachineEventPropertyArray(bsData->getParentFile(), -1);
            bsData->exitNotifyEvents = HkxSharedPtr(exitEvents);
        }
        exitEvents->addEvent();
        exitEvents->events.last().id = 0;
        loadDynamicTableRows();
    }else{
        FATAL_RUNTIME_ERROR("StateUI::addExitEvent(): The data is nullptr!!");
    }
}

void StateUI::removeExitEvent(int index){
    hkbStateMachineEventPropertyArray *events = nullptr;
    if (bsData){
        events = static_cast<hkbStateMachineEventPropertyArray *>(bsData->exitNotifyEvents.data());
        if (events){
            if (index < events->events.size() && index >= 0){
                events->removeEvent(index);
            }else{
                WARNING_MESSAGE("StateUI::removeExitEvent(): Invalid row index selected!!");
                return;
            }
        }else{
            WARNING_MESSAGE("StateUI::removeExitEvent(): Event data is nullptr!!");
            return;
        }
        loadDynamicTableRows();
    }else{
        FATAL_RUNTIME_ERROR("StateUI::removeExitEvent(): The data is nullptr!!");
    }
}

void StateUI::addTransition(){
    hkbStateMachineTransitionInfoArray *trans = nullptr;
    if (bsData){
        trans = static_cast<hkbStateMachineTransitionInfoArray *>(bsData->transitions.data());
        if (!trans){
            trans = new hkbStateMachineTransitionInfoArray(bsData->getParentFile(), bsData->getParentStateMachine(), -1);
            bsData->transitions = HkxSharedPtr(trans);
        }
        trans->addTransition();
        loadDynamicTableRows();
    }else{
        FATAL_RUNTIME_ERROR("StateUI::addTransition(): The data is nullptr!!");
    }
}

void StateUI::removeTransition(int index){
    hkbStateMachineTransitionInfoArray *trans = nullptr;
    if (bsData){
        trans = static_cast<hkbStateMachineTransitionInfoArray *>(bsData->transitions.data());
        if (trans){
            if (index < trans->getNumTransitions() && index >= 0){
                trans->removeTransition(index);
            }else{
                WARNING_MESSAGE("StateUI::removeTransition(): Invalid row index selected!!");
                return;
            }
        }else{
            WARNING_MESSAGE("StateUI::removeTransition(): Transition data is nullptr!!");
            return;
        }
        loadDynamicTableRows();
    }else{
        FATAL_RUNTIME_ERROR("StateUI::removeTransition(): The data is nullptr!!");
    }
}

void StateUI::viewSelectedChild(int row, int column){
    int result;
    int count = 0;
    hkbStateMachineTransitionInfoArray *trans = nullptr;
    hkbStateMachineEventPropertyArray *enterEvents = nullptr;
    hkbStateMachineEventPropertyArray *exitEvents = nullptr;
    if (bsData){
        if (row == GENERATOR_ROW && column == VALUE_COLUMN){
            emit viewGenerators(static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData->generator) + 1);
        }else if (row == ADD_ENTER_EVENT_ROW && column == NAME_COLUMN){
            addEnterEvent();
        }else if (row == exitEventsButtonRow && column == NAME_COLUMN){
            addExitEvent();
        }else if (row == transitionsButtonRow && column == NAME_COLUMN){
            addTransition();
        }else if (row > ADD_ENTER_EVENT_ROW && row < exitEventsButtonRow){
            enterEvents = static_cast<hkbStateMachineEventPropertyArray *>(bsData->enterNotifyEvents.data());
            result = row - ADD_ENTER_EVENT_ROW - 1;
            if (enterEvents->events.size() > result && result >= 0){
                if (column == VALUE_COLUMN){
                    eventUI->loadData(static_cast<BehaviorFile *>(bsData->getParentFile()), &enterEvents->events[result]);
                    setCurrentIndex(EVENT_PAYLOAD_WIDGET);
                }else if (column == BINDING_COLUMN){
                    if (MainWindow::yesNoDialogue("Are you sure you want to remove the enter event \""+table->item(row, NAME_COLUMN)->text()+"\"?") == QMessageBox::Yes){
                        removeEnterEvent(result);
                    }
                }
            }else{
                FATAL_RUNTIME_ERROR("StateUI::viewSelectedChild(): Invalid index of child to view!!");
            }
        }else if (row > exitEventsButtonRow && row < transitionsButtonRow){
            exitEvents = static_cast<hkbStateMachineEventPropertyArray *>(bsData->exitNotifyEvents.data());
            result = row - exitEventsButtonRow - 1;
            if (exitEvents->events.size() > result && result >= 0){
                if (column == VALUE_COLUMN){
                    eventUI->loadData(static_cast<BehaviorFile *>(bsData->getParentFile()), &exitEvents->events[result]);
                    setCurrentIndex(EVENT_PAYLOAD_WIDGET);
                }else if (column == BINDING_COLUMN){
                    if (MainWindow::yesNoDialogue("Are you sure you want to remove the exit event \""+table->item(row, NAME_COLUMN)->text()+"\"?") == QMessageBox::Yes){
                        removeExitEvent(result);
                    }
                }
            }else{
                FATAL_RUNTIME_ERROR("StateUI::viewSelectedChild(): Invalid index of child to view!!");
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
            trans = static_cast<hkbStateMachineTransitionInfoArray *>(bsData->transitions.data());
            if (trans && result < trans->getNumTransitions() && result >= 0){
                if (column == VALUE_COLUMN){
                    transitionUI->loadData(static_cast<BehaviorFile *>(bsData->getParentFile()), bsData->getParentStateMachine(), &trans->transitions[result], result);
                    setCurrentIndex(TRANSITION_WIDGET);
                }else if (column == BINDING_COLUMN){
                    if (MainWindow::yesNoDialogue("Are you sure you want to remove the transition \""+table->item(row, NAME_COLUMN)->text()+"\"?") == QMessageBox::Yes){
                        removeTransition(result);
                    }
                }
            }else{
                FATAL_RUNTIME_ERROR("StateUI::viewSelectedChild(): Invalid index of child to view!!");
            }
        }
    }else{
        FATAL_RUNTIME_ERROR("StateUI::viewSelectedChild(): The data is nullptr!!");
    }
}

void StateUI::setGenerator(int index, const QString & name){
    hkbStateMachine *gen = nullptr;
    DataIconManager *ptr = nullptr;
    if (bsData){
        if (behaviorView){
            gen = static_cast<hkbStateMachine *>(bsData->getParentStateMachine());
            ptr = static_cast<BehaviorFile *>(bsData->getParentFile())->getGeneratorDataAt(index - 1);
            if (ptr){
                if (name != ptr->getName()){
                    FATAL_RUNTIME_ERROR("::setDefaultGenerator():The name of the selected object does not match it's name in the object selection table!!!");
                    return;
                }else if (!gen){
                    FATAL_RUNTIME_ERROR("The currently loaded 'hkbStateMachineStateInfo' has no parent 'hkbStateMachine'!!!");
                    return;
                }else if (ptr == bsData || !behaviorView->reconnectIcon(behaviorView->getSelectedItem(), static_cast<DataIconManager*>(bsData->generator.data()), ptr, false)){
                    WARNING_MESSAGE("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\nYou are attempting to create a circular branch or dead end!!!");
                    return;
                }
            }else{
                if (behaviorView->getSelectedItem()){
                    behaviorView->removeItemFromGraph(behaviorView->getSelectedItem()->getChildWithData(static_cast<DataIconManager*>(bsData->generator.data())), stateIndex);
                }else{
                    FATAL_RUNTIME_ERROR("StateUI::setGenerator(): The selected icon is nullptr!!");
                    return;
                }
            }
            behaviorView->removeGeneratorData();
            table->item(GENERATOR_ROW, VALUE_COLUMN)->setText(name);
            bsData->getParentFile()->setIsChanged(true);
            emit returnToParent(true);
        }else{
            FATAL_RUNTIME_ERROR("StateUI::setGenerator(): The 'behaviorView' pointer is nullptr!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("StateUI::setGenerator(): The 'bsData' pointer is nullptr!!");
    }
}

void StateUI::setBehaviorView(BehaviorGraphView *view){
    behaviorView = view;
}

void StateUI::transitionRenamed(const QString &name, int index){
    if (bsData){
        if (table->item(transitionsButtonRow + index + 1, NAME_COLUMN)){
            table->item(transitionsButtonRow + index + 1, NAME_COLUMN)->setText(name);
        }else{
            FATAL_RUNTIME_ERROR("StateUI::transitionRenamed(): Invalid row selected!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("StateUI::transitionRenamed(): The data is nullptr!!");
    }
}

void StateUI::eventRenamed(const QString & name, int index){
    if (bsData){
        if (currentIndex() == EVENT_PAYLOAD_WIDGET){
            eventUI->eventRenamed(name, index);
        }else if (currentIndex() == TRANSITION_WIDGET){
            transitionUI->eventRenamed(name, index);
        }
        loadDynamicTableRows(); //Inefficient...
    }else{
        FATAL_RUNTIME_ERROR("StateUI::eventRenamed(): The data is nullptr!!");
    }
}

void StateUI::generatorRenamed(const QString & name, int index){
    if (bsData){
        index--;
        if (index == static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData->generator)){
            table->item(GENERATOR_ROW, VALUE_COLUMN)->setText(name);
        }
    }else{
        FATAL_RUNTIME_ERROR("StateUI::generatorRenamed(): The 'bsData' pointer is nullptr!!");
    }
}

void StateUI::returnToWidget(){
    setCurrentIndex(MAIN_WIDGET);
}
