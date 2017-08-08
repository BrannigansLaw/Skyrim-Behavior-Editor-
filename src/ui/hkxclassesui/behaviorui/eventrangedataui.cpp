#include "eventrangedataui.h"

#include "src/utility.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/behavior/hkbstringeventpayload.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/ui/genericdatawidgets.h"

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

#define BASE_NUMBER_OF_ROWS 4

#define EVENT_ID_ROW 0
#define PAYLOAD_ROW 1
#define UPPER_BOUND_ROW 2
#define EVENT_MODE_ROW 3

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList EventRangeDataUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

EventRangeDataUI::EventRangeDataUI()
    : file(NULL),
      rangeIndex(-1),
      bsData(NULL),
      parent(NULL),
      topLyt(new QGridLayout),
      returnPB(new QPushButton("Return")),
      table(new TableWidget),
      payload(new QLineEdit),
      upperBound(new DoubleSpinBox),
      eventMode(new ComboBox)
{
    setTitle("hkbEventRangeData");
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(EVENT_ID_ROW, NAME_COLUMN, new TableWidgetItem("eventId"));
    table->setItem(EVENT_ID_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(EVENT_ID_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(EVENT_ID_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_EVENTS_TABLE_TIP));
    table->setItem(PAYLOAD_ROW, NAME_COLUMN, new TableWidgetItem("payload"));
    table->setItem(PAYLOAD_ROW, TYPE_COLUMN, new TableWidgetItem("hkbStringEventPayload", Qt::AlignCenter));
    table->setItem(PAYLOAD_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(PAYLOAD_ROW, VALUE_COLUMN, payload);
    table->setItem(UPPER_BOUND_ROW, NAME_COLUMN, new TableWidgetItem("upperBound"));
    table->setItem(UPPER_BOUND_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(UPPER_BOUND_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(UPPER_BOUND_ROW, VALUE_COLUMN, upperBound);
    table->setItem(EVENT_MODE_ROW, NAME_COLUMN, new TableWidgetItem("eventMode"));
    table->setItem(EVENT_MODE_ROW, TYPE_COLUMN, new TableWidgetItem("EventMode", Qt::AlignCenter));
    table->setItem(EVENT_MODE_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(EVENT_MODE_ROW, VALUE_COLUMN, eventMode);
    topLyt->addWidget(returnPB, 0, 1, 1, 1);
    topLyt->addWidget(table, 1, 0, 6, 3);
    setLayout(topLyt);
    connectSignals();
}

void EventRangeDataUI::connectSignals(){
    connect(returnPB, SIGNAL(released()), this, SIGNAL(returnToParent()), Qt::UniqueConnection);
    connect(upperBound, SIGNAL(editingFinished()), this, SLOT(setUpperBound()), Qt::UniqueConnection);
    connect(eventMode, SIGNAL(currentIndexChanged(int)), this, SLOT(setEventMode(int)), Qt::UniqueConnection);
    connect(payload, SIGNAL(editingFinished()), this, SLOT(setEventPayload()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)), Qt::UniqueConnection);
}

void EventRangeDataUI::disconnectSignals(){
    disconnect(returnPB, SIGNAL(released()), this, SIGNAL(returnToParent()));
    disconnect(upperBound, SIGNAL(editingFinished()), this, SLOT(setUpperBound()));
    disconnect(eventMode, SIGNAL(currentIndexChanged(int)), this, SLOT(setEventMode(int)));
    disconnect(payload, SIGNAL(editingFinished()), this, SLOT(setEventPayload()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)));
}

void EventRangeDataUI::loadData(BehaviorFile *parentFile, hkbEventRangeDataArray::hkbEventRangeData *ranges, hkbEventRangeDataArray *par, int index){
    disconnectSignals();
    QString text;
    if (parentFile && ranges && par && index > -1){
        parent = par;
        rangeIndex = index;
        file = parentFile;
        bsData = ranges;
        text = file->getEventNameAt(ranges->event.id);
        if (text == ""){
            if (ranges->event.id != -1){
                WARNING_MESSAGE(QString("EventRangeDataUI::loadData(): Invalid event id!!!"));
            }
            text = "NONE";
        }
        QString eventName = file->getEventNameAt(bsData->event.id);
        if (eventName != ""){
            table->item(EVENT_ID_ROW, VALUE_COLUMN)->setText(eventName);
        }else{
            table->item(EVENT_ID_ROW, VALUE_COLUMN)->setText("NONE");
        }
        if (ranges->event.payload.data()){
            payload->setText(static_cast<hkbStringEventPayload *>(ranges->event.payload.data())->data);
        }else{
            payload->setText("");
        }
        upperBound->setValue(bsData->upperBound);
        if (eventMode->count() == 0){
            eventMode->insertItems(0, bsData->EventRangeMode);
        }
        eventMode->setCurrentIndex(bsData->EventRangeMode.indexOf(bsData->eventMode));
        /*hkbVariableBindingSet *varBind = static_cast<hkbVariableBindingSet *>(parent->variableBindingSet.data());
        if (varBind){
            loadBinding(UPPER_BOUND_ROW, BINDING_COLUMN, varBind, "eventData:"+QString::number(rangeIndex)+"/upperBound");
            loadBinding(EVENT_MODE_ROW, BINDING_COLUMN, varBind, "eventData:"+QString::number(rangeIndex)+"/eventMode");
        }else{
            table->item(UPPER_BOUND_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(EVENT_MODE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
        }*/
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EventRangeDataUI::loadData(): Behavior file, bind or event data is null!!!"));
    }
    connectSignals();
}

/*void EventRangeDataUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
    if (bsData){
        if (varBind){
            int index = varBind->getVariableIndexOfBinding(path);
            QString varName;
            if (index != -1){
                if (varBind->getBindingType(path) == hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY){
                    varName = static_cast<BehaviorFile *>(file)->getCharacterPropertyNameAt(index, true);
                    table->item(row, colunm)->setCheckState(Qt::Checked);
                }else{
                    varName = static_cast<BehaviorFile *>(file)->getVariableNameAt(index);
                }
            }
                if (varName == ""){
                    varName = "NONE";
                }
                table->item(row, colunm)->setText(BINDING_ITEM_LABEL+varName);
        }else{
            CRITICAL_ERROR_MESSAGE(QString("EventRangeDataUI::loadBinding(): The variable binding set is NULL!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EventRangeDataUI::loadBinding(): The data is NULL!!"));
    }
}

bool EventRangeDataUI::setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty){
    hkbVariableBindingSet *varBind = static_cast<hkbVariableBindingSet *>(parent->variableBindingSet.data());
    if (bsData){
        if (index == 0){
            varBind->removeBinding(path);
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
        }else if ((!isProperty && static_cast<BehaviorFile *>(file)->getVariableTypeAt(index - 1) == type) ||
                  (isProperty && static_cast<BehaviorFile *>(file)->getCharacterPropertyTypeAt(index - 1) == type)){
            if (!varBind){
                varBind = new hkbVariableBindingSet(file);
                parent->variableBindingSet = HkxSharedPtr(varBind);
            }
            if (isProperty){
                if (!varBind->addBinding(path, variableName, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY)){
                    CRITICAL_ERROR_MESSAGE(QString("EvaluateExpressionModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }else{
                if (!varBind->addBinding(path, variableName, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    CRITICAL_ERROR_MESSAGE(QString("EvaluateExpressionModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            file->toggleChanged(true);
        }else{
            WARNING_MESSAGE(QString("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EventRangeDataUI::setBinding(): The data is NULL!!"));
    }
    return true;
}

void EventRangeDataUI::setBindingVariable(int index, const QString & name){
    if (bsData){
        bool isProperty = false;
        int row = table->currentRow();
        switch (row){
        case UPPER_BOUND_ROW:
            if (table->item(UPPER_BOUND_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "eventData:"+QString::number(rangeIndex)+"/upperBound", VARIABLE_TYPE_REAL, isProperty);
            break;
        default:
            return;
        }
        file->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EventRangeDataUI::setBindingVariable(): The data is NULL!!"))
    }
}*/

void EventRangeDataUI::setEventId(int index, const QString & name){
    if (bsData && file){
        index--;
        if (bsData->event.id != index){
            bsData->event.id = index;
            table->item(EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
            file->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EventRangeDataUI::setEvent(): Behavior file or event data is null!!!"));
    }
}

void EventRangeDataUI::setEventPayload(){
    hkbStringEventPayload *payloadData;
    if (bsData && file){
        payloadData = static_cast<hkbStringEventPayload *>(bsData->event.payload.data());
        if (payload->text() != ""){
            if (payloadData){
                if (payloadData->data != payload->text()){
                    payloadData->data = payload->text();
                }else{
                    return;
                }
            }else{
                payloadData = new hkbStringEventPayload(file, payload->text());
                file->addObjectToFile(payloadData);
                bsData->event.payload = HkxSharedPtr(payloadData);
            }
        }else{
            bsData->event.payload = HkxSharedPtr();
        }
        file->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EventRangeDataUI::setEventPayload(): Behavior file or event data is null!!!"));
    }
}

void EventRangeDataUI::setUpperBound(){
    if (bsData && file){
        if (bsData->upperBound != upperBound->value()){
            bsData->upperBound = upperBound->value();
            file->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EventRangeDataUI::setupperBound(): Behavior file or event data is null!!!"));
    }
}

void EventRangeDataUI::setEventMode(int index){
    if (bsData){
        bsData->eventMode = bsData->EventRangeMode.at(index);
        file->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EventRangeDataUI::setEventMode(): The data is NULL!!"));
    }
}

void EventRangeDataUI::viewSelectedChild(int row, int column){
    if (bsData){
        /*bool properties = false;
        if (column == BINDING_COLUMN){
            switch (row){
            case UPPER_BOUND_ROW:
                if (table->item(UPPER_BOUND_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "eventData:"+QString::number(rangeIndex)+"/upperBound");
                break;
            }
        }else */if (row == EVENT_ID_ROW && column == VALUE_COLUMN){
                emit viewEvents(bsData->event.id + 1);
            }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EventRangeDataUI::viewSelectedChild(): The data is NULL!!"))
    }
}

/*void EventRangeDataUI::selectTableToView(bool viewproperties, const QString & path){
    if (bsData){
        if (viewproperties){
            if (parent->variableBindingSet.data()){
                emit viewProperties(static_cast<hkbVariableBindingSet *>(parent->variableBindingSet.data())->getVariableIndexOfBinding(path) + 1);
            }else{
                emit viewProperties(0);
            }
        }else{
            if (parent->variableBindingSet.data()){
                emit viewVariables(static_cast<hkbVariableBindingSet *>(parent->variableBindingSet.data())->getVariableIndexOfBinding(path) + 1);
            }else{
                emit viewVariables(0);
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EventRangeDataUI::selectTableToView(): The data is NULL!!"));
    }
}*/

void EventRangeDataUI::eventRenamed(const QString & name, int index){
    if (bsData){
        if (index == bsData->event.id){
            table->item(EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EventRangeDataUI::eventRenamed(): The data is NULL!!"))
    }
}

/*void EventRangeDataUI::variableRenamed(const QString & name, int index){
    int bindIndex = -1;
    hkbVariableBindingSet *bind = NULL;
    if (name == ""){
        WARNING_MESSAGE(QString("EventRangeDataUI::variableRenamed(): The new variable name is the empty string!!"))
    }
    if (bsData){
        //index--;
        bind = static_cast<hkbVariableBindingSet *>(parent->variableBindingSet.data());
        if (bind){
            bindIndex = bind->getVariableIndexOfBinding("eventData:"+QString::number(rangeIndex)+"/upperBound");
            if (bindIndex == index){
                table->item(UPPER_BOUND_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EventRangeDataUI::variableRenamed(): The data is NULL!!"))
    }
}*/
