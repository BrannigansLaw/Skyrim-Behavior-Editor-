#include "bseventeveryneventsmodifierui.h"

#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/hkbstringeventpayload.h"
#include "src/hkxclasses/behavior/modifiers/BSEventEveryNEventsModifier.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/genericdatawidgets.h"

#include <QGridLayout>
#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 9

#define NAME_ROW 0
#define ENABLE_ROW 1
#define EVENT_TO_CHECK_FOR_ID_ROW 2
#define EVENT_TO_CHECK_FOR_PAYLOAD_ROW 3
#define EVENT_TO_SEND_ID_ROW 4
#define EVENT_TO_SEND_PAYLOAD_ROW 5
#define NUMBER_OF_EVENTS_BEFORE_SEND_ROW 6
#define MINIMUM_NUMBER_OF_EVENTS_BEFORE_SEND_ROW 7
#define RANDOMIZE_NUMBER_OF_EVENTS_ROW 8

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList BSEventEveryNEventsModifierUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

BSEventEveryNEventsModifierUI::BSEventEveryNEventsModifierUI()
    : bsData(nullptr),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      enable(new CheckBox),
      eventToCheckForPayload(new QLineEdit),
      eventToSendPayload(new QLineEdit),
      numberOfEventsBeforeSend(new SpinBox),
      minimumNumberOfEventsBeforeSend(new SpinBox),
      randomizeNumberOfEvents(new CheckBox)
{
    setTitle("BSEventEveryNEventsModifier");
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(NAME_ROW, NAME_COLUMN, new TableWidgetItem("name"));
    table->setItem(NAME_ROW, TYPE_COLUMN, new TableWidgetItem("hkStringPtr", Qt::AlignCenter));
    table->setItem(NAME_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(NAME_ROW, VALUE_COLUMN, name);
    table->setItem(ENABLE_ROW, NAME_COLUMN, new TableWidgetItem("enable"));
    table->setItem(ENABLE_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(ENABLE_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ENABLE_ROW, VALUE_COLUMN, enable);
    table->setItem(EVENT_TO_CHECK_FOR_ID_ROW, NAME_COLUMN, new TableWidgetItem("eventToCheckForId"));
    table->setItem(EVENT_TO_CHECK_FOR_ID_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(EVENT_TO_CHECK_FOR_ID_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(EVENT_TO_CHECK_FOR_ID_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_EVENTS_TABLE_TIP));
    table->setItem(EVENT_TO_CHECK_FOR_PAYLOAD_ROW, NAME_COLUMN, new TableWidgetItem("eventToCheckForPayload"));
    table->setItem(EVENT_TO_CHECK_FOR_PAYLOAD_ROW, TYPE_COLUMN, new TableWidgetItem("hkbStringEventPayload", Qt::AlignCenter));
    table->setItem(EVENT_TO_CHECK_FOR_PAYLOAD_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(EVENT_TO_CHECK_FOR_PAYLOAD_ROW, VALUE_COLUMN, eventToCheckForPayload);
    table->setItem(EVENT_TO_SEND_ID_ROW, NAME_COLUMN, new TableWidgetItem("eventToSendId"));
    table->setItem(EVENT_TO_SEND_ID_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(EVENT_TO_SEND_ID_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(EVENT_TO_SEND_ID_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_EVENTS_TABLE_TIP));
    table->setItem(EVENT_TO_SEND_PAYLOAD_ROW, NAME_COLUMN, new TableWidgetItem("eventToSendPayload"));
    table->setItem(EVENT_TO_SEND_PAYLOAD_ROW, TYPE_COLUMN, new TableWidgetItem("hkbStringEventPayload", Qt::AlignCenter));
    table->setItem(EVENT_TO_SEND_PAYLOAD_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(EVENT_TO_SEND_PAYLOAD_ROW, VALUE_COLUMN, eventToSendPayload);
    table->setItem(NUMBER_OF_EVENTS_BEFORE_SEND_ROW, NAME_COLUMN, new TableWidgetItem("numberOfEventsBeforeSend"));
    table->setItem(NUMBER_OF_EVENTS_BEFORE_SEND_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(NUMBER_OF_EVENTS_BEFORE_SEND_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(NUMBER_OF_EVENTS_BEFORE_SEND_ROW, VALUE_COLUMN, numberOfEventsBeforeSend);
    table->setItem(MINIMUM_NUMBER_OF_EVENTS_BEFORE_SEND_ROW, NAME_COLUMN, new TableWidgetItem("minimumNumberOfEventsBeforeSend"));
    table->setItem(MINIMUM_NUMBER_OF_EVENTS_BEFORE_SEND_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(MINIMUM_NUMBER_OF_EVENTS_BEFORE_SEND_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(MINIMUM_NUMBER_OF_EVENTS_BEFORE_SEND_ROW, VALUE_COLUMN, minimumNumberOfEventsBeforeSend);
    table->setItem(RANDOMIZE_NUMBER_OF_EVENTS_ROW, NAME_COLUMN, new TableWidgetItem("randomizeNumberOfEvents"));
    table->setItem(RANDOMIZE_NUMBER_OF_EVENTS_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(RANDOMIZE_NUMBER_OF_EVENTS_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(RANDOMIZE_NUMBER_OF_EVENTS_ROW, VALUE_COLUMN, randomizeNumberOfEvents);
    topLyt->addWidget(table, 0, 0, 8, 3);
    setLayout(topLyt);
}

void BSEventEveryNEventsModifierUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
    connect(eventToCheckForPayload, SIGNAL(editingFinished()), this, SLOT(setEventToCheckForPayload()), Qt::UniqueConnection);
    connect(eventToSendPayload, SIGNAL(editingFinished()), this, SLOT(setEventToSendPayload()), Qt::UniqueConnection);
    connect(numberOfEventsBeforeSend, SIGNAL(editingFinished()), this, SLOT(setNumberOfEventsBeforeSend()), Qt::UniqueConnection);
    connect(minimumNumberOfEventsBeforeSend, SIGNAL(editingFinished()), this, SLOT(setMinimumNumberOfEventsBeforeSend()), Qt::UniqueConnection);
    connect(randomizeNumberOfEvents, SIGNAL(released()), this, SLOT(setRandomizeNumberOfEvents()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)), Qt::UniqueConnection);
}

void BSEventEveryNEventsModifierUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    disconnect(eventToCheckForPayload, SIGNAL(editingFinished()), this, SLOT(setEventToCheckForPayload()));
    disconnect(eventToSendPayload, SIGNAL(editingFinished()), this, SLOT(setEventToSendPayload()));
    disconnect(numberOfEventsBeforeSend, SIGNAL(editingFinished()), this, SLOT(setNumberOfEventsBeforeSend()));
    disconnect(minimumNumberOfEventsBeforeSend, SIGNAL(editingFinished()), this, SLOT(setMinimumNumberOfEventsBeforeSend()));
    disconnect(randomizeNumberOfEvents, SIGNAL(released()), this, SLOT(setRandomizeNumberOfEvents()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)));
}

void BSEventEveryNEventsModifierUI::connectToTables(GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *events){
    if (variables && properties && events){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(events, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(events, SIGNAL(elementSelected(int,QString)), this, SLOT(eventTableElementSelected(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int,QString,QStringList)), variables, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int,QString,QStringList)), properties, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewEvents(int,QString,QStringList)), events, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
    }else{
        CRITICAL_ERROR_MESSAGE("BSEventEveryNEventsModifierUI::connectToTables(): One or more arguments are nullptr!!");
    }
}

void BSEventEveryNEventsModifierUI::loadData(HkxObject *data){
    disconnectSignals();
    if (data){
        if (data->getSignature() == BS_EVENT_EVERY_N_EVENTS_MODIFIER){
            bsData = static_cast<BSEventEveryNEventsModifier *>(data);
            hkbVariableBindingSet *varBind = nullptr;
            hkbStringEventPayload *payload = static_cast<hkbStringEventPayload *>(bsData->eventToCheckFor.payload.data());
            name->setText(bsData->name);
            enable->setChecked(bsData->enable);
            QString text = static_cast<BehaviorFile *>(bsData->getParentFile())->getEventNameAt(bsData->eventToCheckFor.id);
            if (text != ""){
                table->item(EVENT_TO_CHECK_FOR_ID_ROW, VALUE_COLUMN)->setText(text);
            }else{
                table->item(EVENT_TO_CHECK_FOR_ID_ROW, VALUE_COLUMN)->setText("None");
            }
            if (payload){
                eventToCheckForPayload->setText(payload->getData());
            }else{
                eventToCheckForPayload->setText("");
            }
            text = static_cast<BehaviorFile *>(bsData->getParentFile())->getEventNameAt(bsData->eventToSend.id);
            if (text != ""){
                table->item(EVENT_TO_SEND_ID_ROW, VALUE_COLUMN)->setText(text);
            }else{
                table->item(EVENT_TO_SEND_ID_ROW, VALUE_COLUMN)->setText("None");
            }
            if (payload){
                eventToSendPayload->setText(payload->getData());
            }else{
                eventToSendPayload->setText("");
            }
            numberOfEventsBeforeSend->setValue(bsData->numberOfEventsBeforeSend);
            minimumNumberOfEventsBeforeSend->setValue(bsData->minimumNumberOfEventsBeforeSend);
            randomizeNumberOfEvents->setChecked(bsData->randomizeNumberOfEvents);
            varBind = bsData->getVariableBindingSetData();
            if (varBind){
                loadBinding(ENABLE_ROW, BINDING_COLUMN, varBind, "enable");
                loadBinding(NUMBER_OF_EVENTS_BEFORE_SEND_ROW, BINDING_COLUMN, varBind, "numberOfEventsBeforeSend");
                loadBinding(MINIMUM_NUMBER_OF_EVENTS_BEFORE_SEND_ROW, BINDING_COLUMN, varBind, "minimumNumberOfEventsBeforeSend");
                loadBinding(RANDOMIZE_NUMBER_OF_EVENTS_ROW, BINDING_COLUMN, varBind, "randomizeNumberOfEvents");
            }else{
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(NUMBER_OF_EVENTS_BEFORE_SEND_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(MINIMUM_NUMBER_OF_EVENTS_BEFORE_SEND_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(RANDOMIZE_NUMBER_OF_EVENTS_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
        }else{
            CRITICAL_ERROR_MESSAGE("BSEventEveryNEventsModifierUI::loadData(): The data is an incorrect type!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSEventEveryNEventsModifierUI::loadData(): The data is nullptr!!");
    }
    connectSignals();
}

void BSEventEveryNEventsModifierUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            bsData->setIsFileChanged(true);
            emit modifierNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData));
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSEventEveryNEventsModifierUI::setName(): The data is nullptr!!");
    }
}

void BSEventEveryNEventsModifierUI::setEnable(){
    if (bsData){
        bsData->enable = enable->isChecked();
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("BSEventEveryNEventsModifierUI::setEnable(): The data is nullptr!!");
    }
}

void BSEventEveryNEventsModifierUI::setEventToCheckForId(int index, const QString & name){
    if (bsData){
        index--;
        if (bsData->eventToCheckFor.id != index){
            bsData->eventToCheckFor.id = index;
            table->item(EVENT_TO_CHECK_FOR_ID_ROW, VALUE_COLUMN)->setText(name);
            bsData->setIsFileChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSEventEveryNEventsModifierUI::seteventToCheckForId(): The data is nullptr!!");
    }
}

void BSEventEveryNEventsModifierUI::setEventToCheckForPayload(){
    hkbStringEventPayload *payload;
    if (bsData){
        payload = static_cast<hkbStringEventPayload *>(bsData->eventToCheckFor.payload.data());
        if (eventToCheckForPayload->text() != ""){
            if (payload){
                payload->getData() = eventToCheckForPayload->text();
            }else{
                payload = new hkbStringEventPayload(bsData->getParentFile(), eventToCheckForPayload->text());
                //bsData->getParentFile()->addObjectToFile(payload, -1);
                bsData->eventToCheckFor.payload = HkxSharedPtr(payload);
            }
        }else{
            bsData->eventToCheckFor.payload = HkxSharedPtr();
        }
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("BSEventEveryNEventsModifierUI::seteventToCheckForPayload(): The data is nullptr!!");
    }
}

void BSEventEveryNEventsModifierUI::setEventToSendId(int index, const QString &name){
    if (bsData){
        index--;
        if (bsData->eventToSend.id != index){
            bsData->eventToSend.id = index;
            table->item(EVENT_TO_SEND_ID_ROW, VALUE_COLUMN)->setText(name);
            bsData->setIsFileChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSEventEveryNEventsModifierUI::setEventToSendId(): The data is nullptr!!");
    }
}

void BSEventEveryNEventsModifierUI::setEventToSendPayload(){
    hkbStringEventPayload *payload;
    if (bsData){
        payload = static_cast<hkbStringEventPayload *>(bsData->eventToSend.payload.data());
        if (eventToSendPayload->text() != ""){
            if (payload){
                payload->getData() = eventToSendPayload->text();
            }else{
                payload = new hkbStringEventPayload(bsData->getParentFile(), eventToSendPayload->text());
                //bsData->getParentFile()->addObjectToFile(payload, -1);
                bsData->eventToSend.payload = HkxSharedPtr(payload);
            }
        }else{
            bsData->eventToSend.payload = HkxSharedPtr();
        }
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("BSEventEveryNEventsModifierUI::setEventToSendPayload(): The data is nullptr!!");
    }
}

void BSEventEveryNEventsModifierUI::setNumberOfEventsBeforeSend(){
    if (bsData){
        if (bsData->numberOfEventsBeforeSend != numberOfEventsBeforeSend->value()){
            bsData->numberOfEventsBeforeSend = numberOfEventsBeforeSend->value();
            bsData->setIsFileChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSEventEveryNEventsModifierUI::setnumberOfEventsBeforeSend(): The data is nullptr!!");
    }
}

void BSEventEveryNEventsModifierUI::setMinimumNumberOfEventsBeforeSend(){
    if (bsData){
        if (bsData->minimumNumberOfEventsBeforeSend != minimumNumberOfEventsBeforeSend->value()){
            bsData->minimumNumberOfEventsBeforeSend = minimumNumberOfEventsBeforeSend->value();
            bsData->setIsFileChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSEventEveryNEventsModifierUI::setminimumNumberOfEventsBeforeSend(): The data is nullptr!!");
    }
}

void BSEventEveryNEventsModifierUI::setRandomizeNumberOfEvents(){
    if (bsData){
        bsData->randomizeNumberOfEvents = randomizeNumberOfEvents->isChecked();
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("BSEventEveryNEventsModifierUI::setrandomizeNumberOfEvents(): The data is nullptr!!");
    }
}

void BSEventEveryNEventsModifierUI::eventTableElementSelected(int index, const QString &name){
    index--;
    switch (table->currentRow()){
    case EVENT_TO_CHECK_FOR_ID_ROW:
        setEventToCheckForId(index, name);
        break;
    case EVENT_TO_SEND_ID_ROW:
        setEventToSendId(index, name);
        break;
    default:
        WARNING_MESSAGE("BSEventEveryNEventsModifierUI::eventTableElementSelected(): An unwanted element selected event was recieved!!");
        return;
    }
}

void BSEventEveryNEventsModifierUI::viewSelected(int row, int column){
    if (bsData){
        bool isProperty = false;
        if (column == BINDING_COLUMN){
            switch (row){
            case ENABLE_ROW:
                if (table->item(ENABLE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "enable");
                break;
            case NUMBER_OF_EVENTS_BEFORE_SEND_ROW:
                if (table->item(NUMBER_OF_EVENTS_BEFORE_SEND_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "numberOfEventsBeforeSend");
                break;
            case MINIMUM_NUMBER_OF_EVENTS_BEFORE_SEND_ROW:
                if (table->item(MINIMUM_NUMBER_OF_EVENTS_BEFORE_SEND_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "minimumNumberOfEventsBeforeSend");
                break;
            case RANDOMIZE_NUMBER_OF_EVENTS_ROW:
                if (table->item(RANDOMIZE_NUMBER_OF_EVENTS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "randomizeNumberOfEvents");
                break;
            default:
                return;
            }
        }else if (column == VALUE_COLUMN && row == EVENT_TO_CHECK_FOR_ID_ROW){
            emit viewEvents(bsData->eventToCheckFor.id + 1, QString(), QStringList());
        }else if (column == VALUE_COLUMN && row == EVENT_TO_SEND_ID_ROW){
            emit viewEvents(bsData->eventToSend.id + 1, QString(), QStringList());
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSEventEveryNEventsModifierUI::viewSelected(): The 'bsData' pointer is nullptr!!");
    }
}

void BSEventEveryNEventsModifierUI::selectTableToView(bool viewisProperty, const QString & path){
    if (bsData){
        if (viewisProperty){
            if (bsData->getVariableBindingSetData()){
                emit viewProperties(bsData->getVariableBindingSetData()->getVariableIndexOfBinding(path) + 1, QString(), QStringList());
            }else{
                emit viewProperties(0, QString(), QStringList());
            }
        }else{
            if (bsData->getVariableBindingSetData()){
                emit viewVariables(bsData->getVariableBindingSetData()->getVariableIndexOfBinding(path) + 1, QString(), QStringList());
            }else{
                emit viewVariables(0, QString(), QStringList());
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSEventEveryNEventsModifierUI::selectTableToView(): The data is nullptr!!");
    }
}

void BSEventEveryNEventsModifierUI::eventRenamed(const QString & name, int index){
    if (bsData){
        index--;
        if (index == bsData->eventToCheckFor.id){
            table->item(EVENT_TO_CHECK_FOR_ID_ROW, VALUE_COLUMN)->setText(name);
        }
        if (index == bsData->eventToSend.id){
            table->item(EVENT_TO_SEND_ID_ROW, VALUE_COLUMN)->setText(name);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSEventEveryNEventsModifierUI::eventRenamed(): The data is nullptr!!");
    }
}

void BSEventEveryNEventsModifierUI::variableRenamed(const QString & name, int index){
    if (bsData){
        index--;
        hkbVariableBindingSet *bind = bsData->getVariableBindingSetData();
        if (bind){
            int bindIndex = bind->getVariableIndexOfBinding("enable");
            if (bindIndex == index){
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("numberOfEventsBeforeSend");
            if (bindIndex == index){
                table->item(NUMBER_OF_EVENTS_BEFORE_SEND_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("minimumNumberOfEventsBeforeSend");
            if (bindIndex == index){
                table->item(MINIMUM_NUMBER_OF_EVENTS_BEFORE_SEND_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("randomizeNumberOfEvents");
            if (bindIndex == index){
                table->item(RANDOMIZE_NUMBER_OF_EVENTS_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSEventEveryNEventsModifierUI::variableRenamed(): The 'bsData' pointer is nullptr!!");
    }
}

bool BSEventEveryNEventsModifierUI::setBinding(int index, int row, const QString &variableName, const QString &path, hkVariableType type, bool isProperty){
    hkbVariableBindingSet *varBind = bsData->getVariableBindingSetData();
    if (bsData){
        if (index == 0){
            varBind->removeBinding(path);if (varBind->getNumberOfBindings() == 0){static_cast<HkDynamicObject *>(bsData)->getVariableBindingSet() = HkxSharedPtr(); static_cast<BehaviorFile *>(bsData->getParentFile())->removeOtherData();}
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
        }else if ((!isProperty && areVariableTypesCompatible(static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableTypeAt(index - 1), type)) ||
                  (isProperty && areVariableTypesCompatible(static_cast<BehaviorFile *>(bsData->getParentFile())->getCharacterPropertyTypeAt(index - 1), type))){
            if (!varBind){
                varBind = new hkbVariableBindingSet(bsData->getParentFile());
                bsData->getVariableBindingSet() = HkxSharedPtr(varBind);
            }
            if (isProperty){
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY)){
                    CRITICAL_ERROR_MESSAGE("BSEventEveryNEventsModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }else{
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    CRITICAL_ERROR_MESSAGE("BSEventEveryNEventsModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->setIsFileChanged(true);
        }else{
            WARNING_MESSAGE("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSEventEveryNEventsModifierUI::setBinding(): The data is nullptr!!");
    }
    return true;
}

void BSEventEveryNEventsModifierUI::setBindingVariable(int index, const QString &name){
    if (bsData){
        bool isProperty = false;
        int row = table->currentRow();
        switch (row){
        case ENABLE_ROW:
            if (table->item(ENABLE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "enable", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case NUMBER_OF_EVENTS_BEFORE_SEND_ROW:
            if (table->item(NUMBER_OF_EVENTS_BEFORE_SEND_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "numberOfEventsBeforeSend", VARIABLE_TYPE_INT32, isProperty);
            break;
        case MINIMUM_NUMBER_OF_EVENTS_BEFORE_SEND_ROW:
            if (table->item(MINIMUM_NUMBER_OF_EVENTS_BEFORE_SEND_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "minimumNumberOfEventsBeforeSend", VARIABLE_TYPE_INT32, isProperty);
            break;
        case RANDOMIZE_NUMBER_OF_EVENTS_ROW:
            if (table->item(RANDOMIZE_NUMBER_OF_EVENTS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "randomizeNumberOfEvents", VARIABLE_TYPE_BOOL, isProperty);
            break;
        default:
            return;
        }
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("BSEventEveryNEventsModifierUI::setBindingVariable(): The data is nullptr!!");
    }
}

void BSEventEveryNEventsModifierUI::loadBinding(int row, int column, hkbVariableBindingSet *varBind, const QString &path){
    if (bsData){
        if (varBind){
            int index = varBind->getVariableIndexOfBinding(path);
            QString varName;
            if (index != -1){
                if (varBind->getBindingType(path) == hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY){
                    varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getCharacterPropertyNameAt(index, true);
                    table->item(row, column)->setCheckState(Qt::Checked);
                }else{
                    varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableNameAt(index);
                }
            }
            if (varName == ""){
                varName = "NONE";
            }
            table->item(row, column)->setText(BINDING_ITEM_LABEL+varName);
        }else{
            CRITICAL_ERROR_MESSAGE("BSEventEveryNEventsModifierUI::loadBinding(): The variable binding set is nullptr!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSEventEveryNEventsModifierUI::loadBinding(): The data is nullptr!!");
    }
}

