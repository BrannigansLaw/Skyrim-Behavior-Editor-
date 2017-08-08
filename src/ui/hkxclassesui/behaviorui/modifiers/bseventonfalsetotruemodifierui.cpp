#include "bseventonfalsetotruemodifierui.h"

#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/hkbstringeventpayload.h"
#include "src/hkxclasses/behavior/modifiers/BSEventOnFalseToTrueModifier.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/genericdatawidgets.h"

#include <QGridLayout>
#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 14

#define NAME_ROW 0
#define ENABLE_ROW 1
#define ENABLE_EVENT_1_ROW 2
#define VARIABLE_TO_TEST_1_ROW 3
#define EVENT_TO_SEND_1_ID_ROW 4
#define EVENT_TO_SEND_1_PAYLOAD_ROW 5
#define ENABLE_EVENT_2_ROW 6
#define VARIABLE_TO_TEST_2_ROW 7
#define EVENT_TO_SEND_2_ID_ROW 8
#define EVENT_TO_SEND_2_PAYLOAD_ROW 9
#define ENABLE_EVENT_3_ROW 10
#define VARIABLE_TO_TEST_3_ROW 11
#define EVENT_TO_SEND_3_ID_ROW 12
#define EVENT_TO_SEND_3_PAYLOAD_ROW 13

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList BSEventOnFalseToTrueModifierUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

BSEventOnFalseToTrueModifierUI::BSEventOnFalseToTrueModifierUI()
    : bsData(NULL),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      enable(new CheckBox),
      bEnableEvent1(new CheckBox),
      bVariableToTest1(new CheckBox),
      eventToSend1Payload(new QLineEdit),
      bEnableEvent2(new CheckBox),
      bVariableToTest2(new CheckBox),
      eventToSend2Payload(new QLineEdit),
      bEnableEvent3(new CheckBox),
      bVariableToTest3(new CheckBox),
      eventToSend3Payload(new QLineEdit)
{
    setTitle("BSEventOnFalseToTrueModifier");
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
    table->setItem(ENABLE_EVENT_1_ROW, NAME_COLUMN, new TableWidgetItem("bEnableEvent1"));
    table->setItem(ENABLE_EVENT_1_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(ENABLE_EVENT_1_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ENABLE_EVENT_1_ROW, VALUE_COLUMN, bEnableEvent1);
    table->setItem(VARIABLE_TO_TEST_1_ROW, NAME_COLUMN, new TableWidgetItem("bVariableToTest1"));
    table->setItem(VARIABLE_TO_TEST_1_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(VARIABLE_TO_TEST_1_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(VARIABLE_TO_TEST_1_ROW, VALUE_COLUMN, bVariableToTest1);
    table->setItem(EVENT_TO_SEND_1_ID_ROW, NAME_COLUMN, new TableWidgetItem("eventToSend1Id"));
    table->setItem(EVENT_TO_SEND_1_ID_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(EVENT_TO_SEND_1_ID_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(EVENT_TO_SEND_1_ID_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_EVENTS_TABLE_TIP));
    table->setItem(EVENT_TO_SEND_1_PAYLOAD_ROW, NAME_COLUMN, new TableWidgetItem("eventToSend1Payload"));
    table->setItem(EVENT_TO_SEND_1_PAYLOAD_ROW, TYPE_COLUMN, new TableWidgetItem("hkbStringEventPayload", Qt::AlignCenter));
    table->setItem(EVENT_TO_SEND_1_PAYLOAD_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(EVENT_TO_SEND_1_PAYLOAD_ROW, VALUE_COLUMN, eventToSend1Payload);
    table->setItem(ENABLE_EVENT_2_ROW, NAME_COLUMN, new TableWidgetItem("bEnableEvent2"));
    table->setItem(ENABLE_EVENT_2_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(ENABLE_EVENT_2_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ENABLE_EVENT_2_ROW, VALUE_COLUMN, bEnableEvent2);
    table->setItem(VARIABLE_TO_TEST_2_ROW, NAME_COLUMN, new TableWidgetItem("bVariableToTest2"));
    table->setItem(VARIABLE_TO_TEST_2_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(VARIABLE_TO_TEST_2_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(VARIABLE_TO_TEST_2_ROW, VALUE_COLUMN, bVariableToTest2);
    table->setItem(EVENT_TO_SEND_2_ID_ROW, NAME_COLUMN, new TableWidgetItem("eventToSend2Id"));
    table->setItem(EVENT_TO_SEND_2_ID_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(EVENT_TO_SEND_2_ID_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(EVENT_TO_SEND_2_ID_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_EVENTS_TABLE_TIP));
    table->setItem(EVENT_TO_SEND_2_PAYLOAD_ROW, NAME_COLUMN, new TableWidgetItem("eventToSend2Payload"));
    table->setItem(EVENT_TO_SEND_2_PAYLOAD_ROW, TYPE_COLUMN, new TableWidgetItem("hkbStringEventPayload", Qt::AlignCenter));
    table->setItem(EVENT_TO_SEND_2_PAYLOAD_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(EVENT_TO_SEND_2_PAYLOAD_ROW, VALUE_COLUMN, eventToSend2Payload);
    table->setItem(ENABLE_EVENT_3_ROW, NAME_COLUMN, new TableWidgetItem("bEnableEvent3"));
    table->setItem(ENABLE_EVENT_3_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(ENABLE_EVENT_3_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ENABLE_EVENT_3_ROW, VALUE_COLUMN, bEnableEvent3);
    table->setItem(VARIABLE_TO_TEST_3_ROW, NAME_COLUMN, new TableWidgetItem("bVariableToTest3"));
    table->setItem(VARIABLE_TO_TEST_3_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(VARIABLE_TO_TEST_3_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(VARIABLE_TO_TEST_3_ROW, VALUE_COLUMN, bVariableToTest3);
    table->setItem(EVENT_TO_SEND_3_ID_ROW, NAME_COLUMN, new TableWidgetItem("eventToSend3Id"));
    table->setItem(EVENT_TO_SEND_3_ID_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(EVENT_TO_SEND_3_ID_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(EVENT_TO_SEND_3_ID_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_EVENTS_TABLE_TIP));
    table->setItem(EVENT_TO_SEND_3_PAYLOAD_ROW, NAME_COLUMN, new TableWidgetItem("eventToSend3Payload"));
    table->setItem(EVENT_TO_SEND_3_PAYLOAD_ROW, TYPE_COLUMN, new TableWidgetItem("hkbStringEventPayload", Qt::AlignCenter));
    table->setItem(EVENT_TO_SEND_3_PAYLOAD_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(EVENT_TO_SEND_3_PAYLOAD_ROW, VALUE_COLUMN, eventToSend3Payload);
    topLyt->addWidget(table, 0, 0, 8, 3);
    setLayout(topLyt);
}

void BSEventOnFalseToTrueModifierUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
    connect(bEnableEvent1, SIGNAL(released()), this, SLOT(setEnableEvent1()), Qt::UniqueConnection);
    connect(bVariableToTest1, SIGNAL(released()), this, SLOT(setVariableToTest1()), Qt::UniqueConnection);
    connect(eventToSend1Payload, SIGNAL(editingFinished()), this, SLOT(setEventToSend1Payload()), Qt::UniqueConnection);
    connect(bEnableEvent2, SIGNAL(released()), this, SLOT(setEnableEvent2()), Qt::UniqueConnection);
    connect(bVariableToTest2, SIGNAL(released()), this, SLOT(setVariableToTest2()), Qt::UniqueConnection);
    connect(eventToSend2Payload, SIGNAL(editingFinished()), this, SLOT(setEventToSend2Payload()), Qt::UniqueConnection);
    connect(bEnableEvent3, SIGNAL(released()), this, SLOT(setEnableEvent3()), Qt::UniqueConnection);
    connect(bVariableToTest3, SIGNAL(released()), this, SLOT(setVariableToTest3()), Qt::UniqueConnection);
    connect(eventToSend3Payload, SIGNAL(editingFinished()), this, SLOT(setEventToSend3Payload()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)), Qt::UniqueConnection);
}

void BSEventOnFalseToTrueModifierUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    disconnect(bEnableEvent1, SIGNAL(released()), this, SLOT(setEnableEvent1()));
    disconnect(bVariableToTest1, SIGNAL(released()), this, SLOT(setVariableToTest1()));
    disconnect(eventToSend1Payload, SIGNAL(editingFinished()), this, SLOT(setEventToSend1Payload()));
    disconnect(bEnableEvent2, SIGNAL(released()), this, SLOT(setEnableEvent2()));
    disconnect(bVariableToTest2, SIGNAL(released()), this, SLOT(setVariableToTest2()));
    disconnect(eventToSend2Payload, SIGNAL(editingFinished()), this, SLOT(setEventToSend2Payload()));
    disconnect(bEnableEvent3, SIGNAL(released()), this, SLOT(setEnableEvent3()));
    disconnect(bVariableToTest3, SIGNAL(released()), this, SLOT(setVariableToTest3()));
    disconnect(eventToSend3Payload, SIGNAL(editingFinished()), this, SLOT(setEventToSend3Payload()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)));
}

void BSEventOnFalseToTrueModifierUI::connectToTables(GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *events){
    if (variables && properties && events){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(events, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(events, SIGNAL(elementSelected(int,QString)), this, SLOT(eventTableElementSelected(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int)), variables, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int)), properties, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewEvents(int)), events, SLOT(showTable(int)), Qt::UniqueConnection);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSEventOnFalseToTrueModifierUI::connectToTables(): One or more arguments are NULL!!"))
    }
}

void BSEventOnFalseToTrueModifierUI::loadData(HkxObject *data){
    disconnectSignals();
    if (data){
        if (data->getSignature() == BS_EVENT_ON_FALSE_TO_TRUE_MODIFIER){
            bsData = static_cast<BSEventOnFalseToTrueModifier *>(data);
            hkbVariableBindingSet *varBind = NULL;
            hkbStringEventPayload *payload1 = static_cast<hkbStringEventPayload *>(bsData->eventToSend1.payload.data());
            hkbStringEventPayload *payload2 = static_cast<hkbStringEventPayload *>(bsData->eventToSend2.payload.data());
            hkbStringEventPayload *payload3 = static_cast<hkbStringEventPayload *>(bsData->eventToSend3.payload.data());
            name->setText(bsData->name);
            enable->setChecked(bsData->enable);
            bEnableEvent1->setChecked(bsData->bEnableEvent1);
            bVariableToTest1->setChecked(bsData->bVariableToTest1);
            QString text = static_cast<BehaviorFile *>(bsData->getParentFile())->getEventNameAt(bsData->eventToSend1.id);
            if (text != ""){
                table->item(EVENT_TO_SEND_1_ID_ROW, VALUE_COLUMN)->setText(text);
            }else{
                table->item(EVENT_TO_SEND_1_ID_ROW, VALUE_COLUMN)->setText("None");
            }
            if (payload1){
                eventToSend1Payload->setText(payload1->data);
            }else{
                eventToSend1Payload->setText("");
            }
            bEnableEvent2->setChecked(bsData->bEnableEvent2);
            bVariableToTest2->setChecked(bsData->bVariableToTest2);
            text = static_cast<BehaviorFile *>(bsData->getParentFile())->getEventNameAt(bsData->eventToSend2.id);
            if (text != ""){
                table->item(EVENT_TO_SEND_2_ID_ROW, VALUE_COLUMN)->setText(text);
            }else{
                table->item(EVENT_TO_SEND_2_ID_ROW, VALUE_COLUMN)->setText("None");
            }
            if (payload2){
                eventToSend2Payload->setText(payload2->data);
            }else{
                eventToSend2Payload->setText("");
            }
            bEnableEvent3->setChecked(bsData->bEnableEvent3);
            bVariableToTest3->setChecked(bsData->bVariableToTest3);
            text = static_cast<BehaviorFile *>(bsData->getParentFile())->getEventNameAt(bsData->eventToSend3.id);
            if (text != ""){
                table->item(EVENT_TO_SEND_3_ID_ROW, VALUE_COLUMN)->setText(text);
            }else{
                table->item(EVENT_TO_SEND_3_ID_ROW, VALUE_COLUMN)->setText("None");
            }
            if (payload3){
                eventToSend3Payload->setText(payload3->data);
            }else{
                eventToSend3Payload->setText("");
            }
            varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (varBind){
                loadBinding(ENABLE_ROW, BINDING_COLUMN, varBind, "enable");
                loadBinding(ENABLE_EVENT_1_ROW, BINDING_COLUMN, varBind, "bEnableEvent1");
                loadBinding(VARIABLE_TO_TEST_1_ROW, BINDING_COLUMN, varBind, "bVariableToTest1");
                loadBinding(ENABLE_EVENT_2_ROW, BINDING_COLUMN, varBind, "bEnableEvent2");
                loadBinding(VARIABLE_TO_TEST_2_ROW, BINDING_COLUMN, varBind, "bVariableToTest2");
                loadBinding(ENABLE_EVENT_3_ROW, BINDING_COLUMN, varBind, "bEnableEvent3");
                loadBinding(VARIABLE_TO_TEST_3_ROW, BINDING_COLUMN, varBind, "bVariableToTest3");
            }else{
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(ENABLE_EVENT_1_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(VARIABLE_TO_TEST_1_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(ENABLE_EVENT_2_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(VARIABLE_TO_TEST_2_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(ENABLE_EVENT_3_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(VARIABLE_TO_TEST_3_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
        }else{
            CRITICAL_ERROR_MESSAGE(QString("BSEventOnFalseToTrueModifierUI::loadData(): The data is an incorrect type!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSEventOnFalseToTrueModifierUI::loadData(): The data is NULL!!"));
    }
    connectSignals();
}

void BSEventOnFalseToTrueModifierUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            bsData->getParentFile()->toggleChanged(true);
            emit modifierNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSEventOnFalseToTrueModifierUI::setName(): The data is NULL!!"));
    }
}

void BSEventOnFalseToTrueModifierUI::setEnable(){
    if (bsData){
        bsData->enable = enable->isChecked();
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSEventOnFalseToTrueModifierUI::setEnable(): The data is NULL!!"));
    }
}

void BSEventOnFalseToTrueModifierUI::setEnableEvent1(){
    if (bsData){
        bsData->bEnableEvent1 = bEnableEvent1->isChecked();
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSEventOnFalseToTrueModifierUI::setbEnableEvent1(): The data is NULL!!"));
    }
}

void BSEventOnFalseToTrueModifierUI::setVariableToTest1(){
    if (bsData){
        bsData->bVariableToTest1 = bVariableToTest1->isChecked();
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSEventOnFalseToTrueModifierUI::setbVariableToTest1(): The data is NULL!!"));
    }
}

void BSEventOnFalseToTrueModifierUI::setEventToSend1Id(int index, const QString & name){
    if (bsData){
        index--;
        if (bsData->eventToSend1.id != index){
            bsData->eventToSend1.id = index;
            table->item(EVENT_TO_SEND_1_ID_ROW, VALUE_COLUMN)->setText(name);
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSEventOnFalseToTrueModifierUI::seteventToSend1Id(): The data is NULL!!"));
    }
}

void BSEventOnFalseToTrueModifierUI::setEventToSend1Payload(){
    hkbStringEventPayload *payload;
    if (bsData){
        payload = static_cast<hkbStringEventPayload *>(bsData->eventToSend1.payload.data());
        if (eventToSend1Payload->text() != ""){
            if (payload){
                payload->data = eventToSend1Payload->text();
            }else{
                payload = new hkbStringEventPayload(bsData->getParentFile(), eventToSend1Payload->text());
                bsData->getParentFile()->addObjectToFile(payload, -1);
                bsData->eventToSend1.payload = HkxSharedPtr(payload);
            }
        }else{
            bsData->eventToSend1.payload = HkxSharedPtr();
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSEventOnFalseToTrueModifierUI::seteventToSend1Payload(): The data is NULL!!"));
    }
}

void BSEventOnFalseToTrueModifierUI::setEnableEvent2(){
    if (bsData){
        bsData->bEnableEvent2 = bEnableEvent2->isChecked();
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSEventOnFalseToTrueModifierUI::setbEnableEvent2(): The data is NULL!!"));
    }
}

void BSEventOnFalseToTrueModifierUI::setVariableToTest2(){
    if (bsData){
        bsData->bVariableToTest2 = bVariableToTest2->isChecked();
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSEventOnFalseToTrueModifierUI::setbVariableToTest2(): The data is NULL!!"));
    }
}

void BSEventOnFalseToTrueModifierUI::setEventToSend2Id(int index, const QString &name){
    if (bsData){
        index--;
        if (bsData->eventToSend2.id != index){
            bsData->eventToSend2.id = index;
            table->item(EVENT_TO_SEND_2_ID_ROW, VALUE_COLUMN)->setText(name);
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSEventOnFalseToTrueModifierUI::seteventToSend2Id(): The data is NULL!!"));
    }
}

void BSEventOnFalseToTrueModifierUI::setEventToSend2Payload(){
    hkbStringEventPayload *payload;
    if (bsData){
        payload = static_cast<hkbStringEventPayload *>(bsData->eventToSend2.payload.data());
        if (eventToSend2Payload->text() != ""){
            if (payload){
                payload->data = eventToSend2Payload->text();
            }else{
                payload = new hkbStringEventPayload(bsData->getParentFile(), eventToSend2Payload->text());
                bsData->getParentFile()->addObjectToFile(payload, -1);
                bsData->eventToSend2.payload = HkxSharedPtr(payload);
            }
        }else{
            bsData->eventToSend2.payload = HkxSharedPtr();
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSEventOnFalseToTrueModifierUI::setEventToSend2Payload(): The data is NULL!!"));
    }
}

void BSEventOnFalseToTrueModifierUI::setEnableEvent3(){
    if (bsData){
        bsData->bEnableEvent3 = bEnableEvent3->isChecked();
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSEventOnFalseToTrueModifierUI::setbEnableEvent3(): The data is NULL!!"));
    }
}

void BSEventOnFalseToTrueModifierUI::setVariableToTest3(){
    if (bsData){
        bsData->bVariableToTest3 = bVariableToTest3->isChecked();
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSEventOnFalseToTrueModifierUI::setbVariableToTest3(): The data is NULL!!"));
    }
}

void BSEventOnFalseToTrueModifierUI::setEventToSend3Id(int index, const QString &name){
    if (bsData){
        index--;
        if (bsData->eventToSend3.id != index){
            bsData->eventToSend3.id = index;
            table->item(EVENT_TO_SEND_3_ID_ROW, VALUE_COLUMN)->setText(name);
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSEventOnFalseToTrueModifierUI::seteventToSend3Id(): The data is NULL!!"));
    }
}

void BSEventOnFalseToTrueModifierUI::setEventToSend3Payload(){
    hkbStringEventPayload *payload;
    if (bsData){
        payload = static_cast<hkbStringEventPayload *>(bsData->eventToSend3.payload.data());
        if (eventToSend3Payload->text() != ""){
            if (payload){
                payload->data = eventToSend3Payload->text();
            }else{
                payload = new hkbStringEventPayload(bsData->getParentFile(), eventToSend3Payload->text());
                bsData->getParentFile()->addObjectToFile(payload, -1);
                bsData->eventToSend3.payload = HkxSharedPtr(payload);
            }
        }else{
            bsData->eventToSend3.payload = HkxSharedPtr();
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSEventOnFalseToTrueModifierUI::setEventToSend3Payload(): The data is NULL!!"));
    }
}

void BSEventOnFalseToTrueModifierUI::eventTableElementSelected(int index, const QString &name){
    //index--;
    switch (table->currentRow()){
    case EVENT_TO_SEND_1_ID_ROW:
        setEventToSend1Id(index, name);
        break;
    case EVENT_TO_SEND_2_ID_ROW:
        setEventToSend2Id(index, name);
        break;
    case EVENT_TO_SEND_3_ID_ROW:
        setEventToSend3Id(index, name);
        break;
    default:
        WARNING_MESSAGE(QString("BSEventOnFalseToTrueModifierUI::eventTableElementSelected(): An unwanted element selected event was recieved!!"));
        return;
    }
}

void BSEventOnFalseToTrueModifierUI::viewSelected(int row, int column){
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
            case ENABLE_EVENT_1_ROW:
                if (table->item(ENABLE_EVENT_1_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "bEnableEvent1");
                break;
            case VARIABLE_TO_TEST_1_ROW:
                if (table->item(VARIABLE_TO_TEST_1_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "bVariableToTest1");
                break;
            case ENABLE_EVENT_2_ROW:
                if (table->item(ENABLE_EVENT_2_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "bEnableEvent2");
                break;
            case VARIABLE_TO_TEST_2_ROW:
                if (table->item(VARIABLE_TO_TEST_2_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "bVariableToTest2");
                break;
            case ENABLE_EVENT_3_ROW:
                if (table->item(ENABLE_EVENT_3_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "bEnableEvent3");
                break;
            case VARIABLE_TO_TEST_3_ROW:
                if (table->item(VARIABLE_TO_TEST_3_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "bVariableToTest3");
                break;
            default:
                return;
            }
        }else if (column == VALUE_COLUMN && row == EVENT_TO_SEND_1_ID_ROW){
            emit viewEvents(bsData->eventToSend1.id + 1);
        }else if (column == VALUE_COLUMN && row == EVENT_TO_SEND_2_ID_ROW){
            emit viewEvents(bsData->eventToSend2.id + 1);
        }else if (column == VALUE_COLUMN && row == EVENT_TO_SEND_3_ID_ROW){
            emit viewEvents(bsData->eventToSend3.id + 1);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSEventOnFalseToTrueModifierUI::viewSelected(): The 'bsData' pointer is NULL!!"))
    }
}

void BSEventOnFalseToTrueModifierUI::selectTableToView(bool viewisProperty, const QString & path){
    if (bsData){
        if (viewisProperty){
            if (bsData->variableBindingSet.data()){
                emit viewProperties(static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data())->getVariableIndexOfBinding(path) + 1);
            }else{
                emit viewProperties(0);
            }
        }else{
            if (bsData->variableBindingSet.data()){
                emit viewVariables(static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data())->getVariableIndexOfBinding(path) + 1);
            }else{
                emit viewVariables(0);
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSEventOnFalseToTrueModifierUI::selectTableToView(): The data is NULL!!"));
    }
}

void BSEventOnFalseToTrueModifierUI::eventRenamed(const QString & name, int index){
    if (bsData){
        index--;
        if (index == bsData->eventToSend1.id){
            table->item(EVENT_TO_SEND_1_ID_ROW, VALUE_COLUMN)->setText(name);
        }
        if (index == bsData->eventToSend2.id){
            table->item(EVENT_TO_SEND_2_ID_ROW, VALUE_COLUMN)->setText(name);
        }
        if (index == bsData->eventToSend3.id){
            table->item(EVENT_TO_SEND_3_ID_ROW, VALUE_COLUMN)->setText(name);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSEventOnFalseToTrueModifierUI::eventRenamed(): The data is NULL!!"));
    }
}

void BSEventOnFalseToTrueModifierUI::variableRenamed(const QString & name, int index){
    if (bsData){
        index--;
        hkbVariableBindingSet *bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (bind){
            int bindIndex = bind->getVariableIndexOfBinding("enable");
            if (bindIndex == index){
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("bEnableEvent1");
            if (bindIndex == index){
                table->item(ENABLE_EVENT_1_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("bVariableToTest1");
            if (bindIndex == index){
                table->item(VARIABLE_TO_TEST_1_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("bEnableEvent2");
            if (bindIndex == index){
                table->item(ENABLE_EVENT_2_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("bVariableToTest2");
            if (bindIndex == index){
                table->item(VARIABLE_TO_TEST_2_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("bEnableEvent3");
            if (bindIndex == index){
                table->item(ENABLE_EVENT_3_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("bVariableToTest3");
            if (bindIndex == index){
                table->item(VARIABLE_TO_TEST_3_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSEventOnFalseToTrueModifierUI::variableRenamed(): The 'bsData' pointer is NULL!!"))
    }
}

bool BSEventOnFalseToTrueModifierUI::setBinding(int index, int row, const QString &variableName, const QString &path, hkVariableType type, bool isProperty){
    hkbVariableBindingSet *varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
    if (bsData){
        if (index == 0){
            varBind->removeBinding(path);
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
        }else if ((!isProperty && static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableTypeAt(index - 1) == type) ||
                  (isProperty && static_cast<BehaviorFile *>(bsData->getParentFile())->getCharacterPropertyTypeAt(index - 1) == type)){
            if (!varBind){
                varBind = new hkbVariableBindingSet(bsData->getParentFile());
                bsData->variableBindingSet = HkxSharedPtr(varBind);
            }
            if (isProperty){
                if (!varBind->addBinding(path, variableName, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY)){
                    CRITICAL_ERROR_MESSAGE(QString("BSEventOnFalseToTrueModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }else{
                if (!varBind->addBinding(path, variableName, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    CRITICAL_ERROR_MESSAGE(QString("BSEventOnFalseToTrueModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->toggleChanged(true);
        }else{
            WARNING_MESSAGE(QString("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSEventOnFalseToTrueModifierUI::setBinding(): The data is NULL!!"));
    }
    return true;
}

void BSEventOnFalseToTrueModifierUI::setBindingVariable(int index, const QString &name){
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
        case ENABLE_EVENT_1_ROW:
            if (table->item(ENABLE_EVENT_1_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "bEnableEvent1", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case VARIABLE_TO_TEST_1_ROW:
            if (table->item(VARIABLE_TO_TEST_1_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "bVariableToTest1", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case ENABLE_EVENT_2_ROW:
            if (table->item(ENABLE_EVENT_2_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "bEnableEvent2", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case VARIABLE_TO_TEST_2_ROW:
            if (table->item(VARIABLE_TO_TEST_2_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "bVariableToTest2", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case ENABLE_EVENT_3_ROW:
            if (table->item(ENABLE_EVENT_3_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "bEnableEvent3", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case VARIABLE_TO_TEST_3_ROW:
            if (table->item(VARIABLE_TO_TEST_3_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "bVariableToTest3", VARIABLE_TYPE_BOOL, isProperty);
            break;
        default:
            return;
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSEventOnFalseToTrueModifierUI::setBindingVariable(): The data is NULL!!"));
    }
}

void BSEventOnFalseToTrueModifierUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
    if (bsData){
        if (varBind){
            int index = varBind->getVariableIndexOfBinding(path);
            QString varName;
            if (index != -1){
                if (varBind->getBindingType(path) == hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY){
                    varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getCharacterPropertyNameAt(index, true);
                    table->item(row, colunm)->setCheckState(Qt::Checked);
                }else{
                    varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableNameAt(index);
                }
            }
            if (varName == ""){
                varName = "NONE";
            }
            table->item(row, colunm)->setText(BINDING_ITEM_LABEL+varName);
        }else{
            CRITICAL_ERROR_MESSAGE(QString("BSEventOnFalseToTrueModifierUI::loadBinding(): The variable binding set is NULL!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSEventOnFalseToTrueModifierUI::loadBinding(): The data is NULL!!"));
    }
}

