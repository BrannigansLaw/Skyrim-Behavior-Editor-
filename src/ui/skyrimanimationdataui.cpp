#include "skyrimanimationdataui.h"

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

#define BASE_NUMBER_OF_ROWS 3

#define DURATION_ROW 0
#define ADD_TRANSLATION_ROW 1
#define INITIAL_ROTATION_ROW 2

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList SkyrimAnimationDataUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

SkyrimAnimationDataUI::SkyrimAnimationDataUI()
    : bsData(NULL),
      //animationIndex(-1),
      rotationsButtonRow(INITIAL_ROTATION_ROW),
      groupBox(new QGroupBox("AnimationData")),
      topLyt(new QGridLayout),
      rotationUI(new RotationUI()),
      translationUI(new EventUI()),
      table(new TableWidget(QColor(Qt::white))),
      returnPB(new QPushButton("Return")),
      duration(new DoubleSpinBox)
{
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(DURATION_ROW, NAME_COLUMN, new TableWidgetItem("duration"));
    table->setItem(DURATION_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(DURATION_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(DURATION_ROW, VALUE_COLUMN, duration);
    table->setItem(ADD_TRANSLATION_ROW, NAME_COLUMN, new TableWidgetItem("Add Translation", Qt::AlignCenter, QColor(Qt::green), QBrush(Qt::black), "Double click to add a Translation"));
    table->setItem(ADD_TRANSLATION_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter, QColor(Qt::gray)));
    table->setItem(ADD_TRANSLATION_ROW, BINDING_COLUMN, new TableWidgetItem("Remove Selected Translation", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black)));
    table->setItem(ADD_TRANSLATION_ROW, VALUE_COLUMN, new TableWidgetItem("Edit Selected Translation", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black)));
    table->setItem(INITIAL_ROTATION_ROW, NAME_COLUMN, new TableWidgetItem("Add Rotation", Qt::AlignCenter, QColor(Qt::green), QBrush(Qt::black), "Double click to add a Rotation"));
    table->setItem(INITIAL_ROTATION_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector5", Qt::AlignCenter, QColor(Qt::gray)));
    table->setItem(INITIAL_ROTATION_ROW, BINDING_COLUMN, new TableWidgetItem("Remove Selected Rotation", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black)));
    table->setItem(INITIAL_ROTATION_ROW, VALUE_COLUMN, new TableWidgetItem("Edit Selected Rotation", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black)));
    topLyt->addWidget(returnPB, 0, 1, 1, 1);
    topLyt->addWidget(table, 1, 0, 8, 3);
    groupBox->setLayout(topLyt);
    //Order here must correspond with the ACTIVE_WIDGET Enumerated type!!!
    addWidget(groupBox);
    addWidget(translationUI);
    addWidget(rotationUI);
    connectSignals();
}

void SkyrimAnimationDataUI::connectSignals(){
    connect(returnPB, SIGNAL(clicked(bool)), this, SIGNAL(returnToParent(bool)), Qt::UniqueConnection);
    connect(duration, SIGNAL(editingFinished()), this, SLOT(setDuration()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)), Qt::UniqueConnection);
    connect(translationUI, SIGNAL(), this, SLOT(returnToWidget()), Qt::UniqueConnection);
    connect(translationUI, SIGNAL(viewEvents(int)), this, SIGNAL(viewEvents(int)), Qt::UniqueConnection);
    connect(rotationUI, SIGNAL(viewEvents(int)), this, SIGNAL(viewEvents(int)), Qt::UniqueConnection);
    connect(rotationUI, SIGNAL(viewVariables(int)), this, SIGNAL(viewVariables(int)), Qt::UniqueConnection);
    connect(rotationUI, SIGNAL(viewProperties(int)), this, SIGNAL(viewProperties(int)), Qt::UniqueConnection);
    connect(rotationUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()), Qt::UniqueConnection);
    connect(rotationUI, SIGNAL(transitionNamChanged(QString,int)), this, SLOT(transitionRenamed(QString,int)), Qt::UniqueConnection);
}

void SkyrimAnimationDataUI::disconnectSignals(){
    disconnect(returnPB, SIGNAL(clicked(bool)), this, SIGNAL(returnToParent(bool)));
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(stateId, SIGNAL(editingFinished()), this, SLOT(setStateId()));
    disconnect(duration, SIGNAL(editingFinished()), this, SLOT(setDuration()));
    disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)));
    disconnect(translationUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()));
    disconnect(translationUI, SIGNAL(viewEvents(int)), this, SIGNAL(viewEvents(int)));
    disconnect(rotationUI, SIGNAL(viewEvents(int)), this, SIGNAL(viewEvents(int)));
    disconnect(rotationUI, SIGNAL(viewVariables(int)), this, SIGNAL(viewVariables(int)));
    disconnect(rotationUI, SIGNAL(viewProperties(int)), this, SIGNAL(viewProperties(int)));
    disconnect(rotationUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()));
    disconnect(rotationUI, SIGNAL(transitionNamChanged(QString,int)), this, SLOT(transitionRenamed(QString,int)));
}

void SkyrimAnimationDataUI::variableTableElementSelected(int index, const QString &name){
    if (bsData){
        switch (currentIndex()){
        case TRANSITION_WIDGET:
            rotationUI->variableTableElementSelected(index, name);
            break;
        default:
            CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimationDataUI::variableTableElementSelected(): Event relayed to wrong widget!!"));
            return;
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimationDataUI::variableTableElementSelected(): The data is NULL!!"));
    }
}

void SkyrimAnimationDataUI::loadData(SkyrimAnimData::AnimMotionData & data){
    disconnectSignals();
    setCurrentIndex(MAIN_WIDGET);
    if (data && data->getSignature() == HKB_STATE_MACHINE_STATE_INFO){
        bsData = static_cast<hkbStateMachineStateInfo *>(data);
        animationIndex = stateindex;
        name->setText(bsData->name);
        stateId->setValue(bsData->stateId);
        duration->setValue(bsData->duration);
        enable->setChecked(bsData->enable);
        if (bsData->generator.data()){
            table->item(GENERATOR_ROW, VALUE_COLUMN)->setText(static_cast<hkbGenerator *>(bsData->generator.data())->getName());
        }else{
            table->item(GENERATOR_ROW, VALUE_COLUMN)->setText("NONE");
        }
        loadDynamicTableRows();
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimationDataUI::loadData(): The data is NULL or an incorrect type!!"));
    }
    connectSignals();
}

void SkyrimAnimationDataUI::loadDynamicTableRows(){
    if (bsData){
        hkbStateMachineEventPropertyArray *events = static_cast<hkbStateMachineEventPropertyArray *>(bsData->enterNotifyEvents.data());
        int temp;
        if (events){
            temp = ADD_TRANSLATION_ROW + events->events.size() + 1 - rotationsButtonRow;
        }else{
            temp = INITIAL_ROTATION_ROW - rotationsButtonRow;
        }
        if (temp > 0){
            for (int i = 0; i < temp; i++){
                table->insertRow(rotationsButtonRow);
                rotationsButtonRow++;
                transitionsButtonRow++;
            }
        }else if (temp < 0){
            for (int i = temp; i < 0; i++){
                table->removeRow(rotationsButtonRow - 1);
                rotationsButtonRow--;
                transitionsButtonRow--;
            }
        }
        events = static_cast<hkbStateMachineEventPropertyArray *>(bsData->exitNotifyEvents.data());
        if (events){
            temp = rotationsButtonRow + events->events.size() + 1 - transitionsButtonRow;
        }else{
            temp = rotationsButtonRow + 1 - transitionsButtonRow;
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
            for (int i = INITIAL_ROTATION_ROW, j = 0; i < rotationsButtonRow, j < events->events.size(); i++, j++){
                eventName = static_cast<BehaviorFile *>(bsData->getParentFile())->getEventNameAt(events->events.at(j).id);
                if (eventName != ""){
                    setRowItems(i, eventName, events->getClassname(), "Remove", "Edit", "Double click to remove this enter event", "Double click to edit this enter event");
                }else{
                    CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimationDataUI::loadDynamicTableRows(): Invalid event name!!!"));
                }
            }
        }
        events = static_cast<hkbStateMachineEventPropertyArray *>(bsData->exitNotifyEvents.data());
        if (events){
            for (int i = rotationsButtonRow + 1, j = 0; i < transitionsButtonRow, j < events->events.size(); i++, j++){
                eventName = static_cast<BehaviorFile *>(bsData->getParentFile())->getEventNameAt(events->events.at(j).id);
                if (eventName != ""){
                    setRowItems(i, eventName, events->getClassname(), "Remove", "Edit", "Double click to remove this exit event", "Double click to edit this exit event");
                }else{
                    CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimationDataUI::loadDynamicTableRows(): Invalid event name!!!"));
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
        CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimationDataUI::loadDynamicTableRows(): The data is NULL!!"));
    }
}


void SkyrimAnimationDataUI::setRowItems(int row, const QString & name, const QString & classname, const QString & bind, const QString & value, const QString & tip1, const QString & tip2){
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

void SkyrimAnimationDataUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            bsData->getParentFile()->toggleChanged(true);
            emit stateNameChanged(name->text(), animationIndex);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimationDataUI::setName(): The data is NULL!!"));
    }
}

void SkyrimAnimationDataUI::setStateId(){
    if (bsData){
        if (bsData->setStateId(stateId->value())){
            emit stateIdChanged(animationIndex, bsData->stateId, bsData->name);
            bsData->getParentFile()->toggleChanged(true);
        }else{
            WARNING_MESSAGE(QString("SkyrimAnimationDataUI::setStateId(): Another state has the selected state ID!!! The state ID for this state was not changed!!!"));
            disconnect(stateId, SIGNAL(editingFinished()), this, SLOT(setStateId()));
            stateId->setValue(bsData->stateId);
            connect(stateId, SIGNAL(editingFinished()), this, SLOT(setStateId()), Qt::UniqueConnection);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimationDataUI::setStateId(): The data is NULL!!"));
    }
}

void SkyrimAnimationDataUI::setDuration(){
    if (bsData){
        if (bsData->duration != duration->value()){
            bsData->duration = duration->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimationDataUI::setduration(): The data is NULL!!"));
    }
}

void SkyrimAnimationDataUI::setEnable(){
    if (bsData){
        bsData->enable = enable->isChecked();
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimationDataUI::setEnable(): The data is NULL!!"));
    }
}

void SkyrimAnimationDataUI::eventTableElementSelected(int index, const QString & name){
    switch (currentIndex()){
    case EVENT_PAYLOAD_WIDGET:
        translationUI->setEvent(index, name);
        break;
    case TRANSITION_WIDGET:
        rotationUI->eventTableElementSelected(index, name);
        break;
    default:
        WARNING_MESSAGE(QString("StateMachineUI::eventTableElementSelected(): An unwanted element selected event was recieved!!"));
    }
    loadDynamicTableRows(); //Inefficient...
}

void SkyrimAnimationDataUI::addTranslation(){
    hkbStateMachineEventPropertyArray *enterEvents = NULL;
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
        CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimationDataUI::addEnterEvent(): The data is NULL!!"));
    }
}

void SkyrimAnimationDataUI::removeTranslation(int index){
    hkbStateMachineEventPropertyArray *events = NULL;
    if (bsData){
        events = static_cast<hkbStateMachineEventPropertyArray *>(bsData->enterNotifyEvents.data());
        if (events){
            if (index < events->events.size() && index >= 0){
                events->removeEvent(index);
            }else{
                WARNING_MESSAGE(QString("SkyrimAnimationDataUI::removeEnterEvent(): Invalid row index selected!!"));
                return;
            }
        }else{
            WARNING_MESSAGE(QString("SkyrimAnimationDataUI::removeEnterEvent(): Event data is NULL!!"));
            return;
        }
        loadDynamicTableRows();
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimationDataUI::removeEnterEvent(): The data is NULL!!"));
    }
}

void SkyrimAnimationDataUI::addRotation(){
    hkbStateMachineEventPropertyArray *exitEvents = NULL;
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
        CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimationDataUI::addExitEvent(): The data is NULL!!"));
    }
}

void SkyrimAnimationDataUI::removeRotation(int index){
    hkbStateMachineEventPropertyArray *events = NULL;
    if (bsData){
        events = static_cast<hkbStateMachineEventPropertyArray *>(bsData->exitNotifyEvents.data());
        if (events){
            if (index < events->events.size() && index >= 0){
                events->removeEvent(index);
            }else{
                WARNING_MESSAGE(QString("SkyrimAnimationDataUI::removeExitEvent(): Invalid row index selected!!"));
                return;
            }
        }else{
            WARNING_MESSAGE(QString("SkyrimAnimationDataUI::removeExitEvent(): Event data is NULL!!"));
            return;
        }
        loadDynamicTableRows();
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimationDataUI::removeExitEvent(): The data is NULL!!"));
    }
}

void SkyrimAnimationDataUI::addTransition(){
    hkbStateMachineTransitionInfoArray *trans = NULL;
    if (bsData){
        trans = static_cast<hkbStateMachineTransitionInfoArray *>(bsData->transitions.data());
        if (!trans){
            trans = new hkbStateMachineTransitionInfoArray(bsData->getParentFile(), bsData->getParentStateMachine(), -1);
            bsData->transitions = HkxSharedPtr(trans);
        }
        trans->addTransition();
        loadDynamicTableRows();
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimationDataUI::addTransition(): The data is NULL!!"));
    }
}

void SkyrimAnimationDataUI::removeTransition(int index){
    hkbStateMachineTransitionInfoArray *trans = NULL;
    if (bsData){
        trans = static_cast<hkbStateMachineTransitionInfoArray *>(bsData->transitions.data());
        if (trans){
            if (index < trans->getNumTransitions() && index >= 0){
                trans->removeTransition(index);
            }else{
                WARNING_MESSAGE(QString("SkyrimAnimationDataUI::removeTransition(): Invalid row index selected!!"));
                return;
            }
        }else{
            WARNING_MESSAGE(QString("SkyrimAnimationDataUI::removeTransition(): Transition data is NULL!!"));
            return;
        }
        loadDynamicTableRows();
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimationDataUI::removeTransition(): The data is NULL!!"));
    }
}

void SkyrimAnimationDataUI::viewSelectedChild(int row, int column){
    int result;
    int count = 0;
    hkbStateMachineTransitionInfoArray *trans = NULL;
    hkbStateMachineEventPropertyArray *enterEvents = NULL;
    hkbStateMachineEventPropertyArray *exitEvents = NULL;
    if (bsData){
        if (row == GENERATOR_ROW && column == VALUE_COLUMN){
            emit viewGenerators(static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData->generator) + 1);
        }else if (row == ADD_TRANSLATION_ROW && column == NAME_COLUMN){
            addTranslation();
        }else if (row == rotationsButtonRow && column == NAME_COLUMN){
            addRotation();
        }else if (row == transitionsButtonRow && column == NAME_COLUMN){
            addTransition();
        }else if (row > ADD_TRANSLATION_ROW && row < rotationsButtonRow){
            enterEvents = static_cast<hkbStateMachineEventPropertyArray *>(bsData->enterNotifyEvents.data());
            result = row - ADD_TRANSLATION_ROW - 1;
            if (enterEvents->events.size() > result && result >= 0){
                if (column == VALUE_COLUMN){
                    translationUI->loadData(static_cast<BehaviorFile *>(bsData->getParentFile()), &enterEvents->events[result]);
                    setCurrentIndex(EVENT_PAYLOAD_WIDGET);
                }else if (column == BINDING_COLUMN){
                    if (MainWindow::yesNoDialogue("Are you sure you want to remove the enter event \""+table->item(row, NAME_COLUMN)->text()+"\"?") == QMessageBox::Yes){
                        removeTranslation(result);
                    }
                }
            }else{
                CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimationDataUI::viewSelectedChild(): Invalid index of child to view!!"));
            }
        }else if (row > rotationsButtonRow && row < transitionsButtonRow){
            exitEvents = static_cast<hkbStateMachineEventPropertyArray *>(bsData->exitNotifyEvents.data());
            result = row - rotationsButtonRow - 1;
            if (exitEvents->events.size() > result && result >= 0){
                if (column == VALUE_COLUMN){
                    translationUI->loadData(static_cast<BehaviorFile *>(bsData->getParentFile()), &exitEvents->events[result]);
                    setCurrentIndex(EVENT_PAYLOAD_WIDGET);
                }else if (column == BINDING_COLUMN){
                    if (MainWindow::yesNoDialogue("Are you sure you want to remove the exit event \""+table->item(row, NAME_COLUMN)->text()+"\"?") == QMessageBox::Yes){
                        removeRotation(result);
                    }
                }
            }else{
                CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimationDataUI::viewSelectedChild(): Invalid index of child to view!!"));
            }
        }else if (row > rotationsButtonRow && row < table->rowCount()){
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
                    rotationUI->loadData(static_cast<BehaviorFile *>(bsData->getParentFile()), bsData->getParentStateMachine(), &trans->transitions[result], result);
                    setCurrentIndex(TRANSITION_WIDGET);
                }else if (column == BINDING_COLUMN){
                    if (MainWindow::yesNoDialogue("Are you sure you want to remove the transition \""+table->item(row, NAME_COLUMN)->text()+"\"?") == QMessageBox::Yes){
                        removeTransition(result);
                    }
                }
            }else{
                CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimationDataUI::viewSelectedChild(): Invalid index of child to view!!"));
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimationDataUI::viewSelectedChild(): The data is NULL!!"));
    }
}

void SkyrimAnimationDataUI::setGenerator(int index, const QString & name){
    hkbStateMachine *gen = NULL;
    DataIconManager *ptr = NULL;
    if (bsData){
        if (behaviorView){
            gen = static_cast<hkbStateMachine *>(bsData->getParentStateMachine());
            ptr = static_cast<BehaviorFile *>(bsData->getParentFile())->getGeneratorDataAt(index - 1);
            if (ptr){
                if (name != ptr->getName()){
                    CRITICAL_ERROR_MESSAGE(QString("The name of the selected object does not match it's name in the object selection table!!!"));
                    return;
                }else if (!gen){
                    CRITICAL_ERROR_MESSAGE(QString("The currently loaded 'hkbStateMachineStateInfo' has no parent 'hkbStateMachine'!!!"));
                    return;
                }else if (ptr == bsData || !behaviorView->reconnectIcon(behaviorView->getSelectedItem(), static_cast<DataIconManager*>(bsData->generator.data()), ptr, false)){
                    WARNING_MESSAGE(QString("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\nYou are attempting to create a circular branch or dead end!!!"));
                    return;
                }
            }else{
                if (behaviorView->getSelectedItem()){
                    behaviorView->removeItemFromGraph(behaviorView->getSelectedItem()->getChildWithData(static_cast<DataIconManager*>(bsData->generator.data())), animationIndex);
                }else{
                    CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimationDataUI::setGenerator(): The selected icon is NULL!!"));
                    return;
                }
            }
            behaviorView->removeGeneratorData();
            table->item(GENERATOR_ROW, VALUE_COLUMN)->setText(name);
            bsData->getParentFile()->toggleChanged(true);
            emit returnToParent(true);
        }else{
            CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimationDataUI::setGenerator(): The 'behaviorView' pointer is NULL!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimationDataUI::setGenerator(): The 'bsData' pointer is NULL!!"));
    }
}

void SkyrimAnimationDataUI::setBehaviorView(BehaviorGraphView *view){
    behaviorView = view;
}

void SkyrimAnimationDataUI::transitionRenamed(const QString &name, int index){
    if (bsData){
        if (table->item(transitionsButtonRow + index + 1, NAME_COLUMN)){
            table->item(transitionsButtonRow + index + 1, NAME_COLUMN)->setText(name);
        }else{
            CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimationDataUI::transitionRenamed(): Invalid row selected!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimationDataUI::transitionRenamed(): The data is NULL!!"));
    }
}

void SkyrimAnimationDataUI::eventRenamed(const QString & name, int index){
    if (bsData){
        translationUI->eventRenamed(name, index);
        loadDynamicTableRows(); //Inefficient...
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimationDataUI::eventRenamed(): The data is NULL!!"));
    }
}

void SkyrimAnimationDataUI::generatorRenamed(const QString & name, int index){
    if (bsData){
        index--;
        if (index == static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData->generator)){
            table->item(GENERATOR_ROW, VALUE_COLUMN)->setText(name);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("SkyrimAnimationDataUI::generatorRenamed(): The 'bsData' pointer is NULL!!"));
    }
}

void SkyrimAnimationDataUI::returnToWidget(){
    setCurrentIndex(MAIN_WIDGET);
}
