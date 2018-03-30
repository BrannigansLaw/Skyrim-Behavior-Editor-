#include "rangesui.h"

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

#define BASE_NUMBER_OF_ROWS 5

#define EVENT_ID_ROW 0
#define PAYLOAD_ROW 1
#define MINIMUM_DISTANCE_ROW 2
#define MAXIMUM_DISTANCE_ROW 3
#define IGNORE_HANDLE_ROW 4

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList RangesUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

RangesUI::RangesUI()
    : file(nullptr),
      rangeIndex(-1),
      bsData(nullptr),
      parent(nullptr),
      topLyt(new QGridLayout),
      returnPB(new QPushButton("Return")),
      table(new TableWidget),
      payload(new QLineEdit),
      minDistance(new DoubleSpinBox),
      maxDistance(new DoubleSpinBox),
      ignoreHandle(new CheckBox)
{
    setTitle("hkRanges");
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
    table->setItem(MINIMUM_DISTANCE_ROW, NAME_COLUMN, new TableWidgetItem("minDistance"));
    table->setItem(MINIMUM_DISTANCE_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(MINIMUM_DISTANCE_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(MINIMUM_DISTANCE_ROW, VALUE_COLUMN, minDistance);
    table->setItem(MAXIMUM_DISTANCE_ROW, NAME_COLUMN, new TableWidgetItem("maxDistance"));
    table->setItem(MAXIMUM_DISTANCE_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(MAXIMUM_DISTANCE_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(MAXIMUM_DISTANCE_ROW, VALUE_COLUMN, maxDistance);
    table->setItem(IGNORE_HANDLE_ROW, NAME_COLUMN, new TableWidgetItem("ignoreHandle"));
    table->setItem(IGNORE_HANDLE_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(IGNORE_HANDLE_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(IGNORE_HANDLE_ROW, VALUE_COLUMN, ignoreHandle);
    topLyt->addWidget(returnPB, 0, 1, 1, 1);
    topLyt->addWidget(table, 1, 0, 6, 3);
    setLayout(topLyt);
    connectSignals();
}

void RangesUI::connectSignals(){
    connect(returnPB, SIGNAL(released()), this, SIGNAL(returnToParent()), Qt::UniqueConnection);
    connect(minDistance, SIGNAL(editingFinished()), this, SLOT(setMinDistance()), Qt::UniqueConnection);
    connect(maxDistance, SIGNAL(editingFinished()), this, SLOT(setMaxDistance()), Qt::UniqueConnection);
    connect(ignoreHandle, SIGNAL(released()), this, SLOT(setIgnoreHandle()), Qt::UniqueConnection);
    connect(payload, SIGNAL(editingFinished()), this, SLOT(setEventPayload()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)), Qt::UniqueConnection);
}

void RangesUI::disconnectSignals(){
    disconnect(returnPB, SIGNAL(released()), this, SIGNAL(returnToParent()));
    disconnect(minDistance, SIGNAL(editingFinished()), this, SLOT(setMinDistance()));
    disconnect(maxDistance, SIGNAL(editingFinished()), this, SLOT(setMaxDistance()));
    disconnect(ignoreHandle, SIGNAL(released()), this, SLOT(setIgnoreHandle()));
    disconnect(payload, SIGNAL(editingFinished()), this, SLOT(setEventPayload()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)));
}

void RangesUI::loadData(BehaviorFile *parentFile, hkbSenseHandleModifier::hkRanges *ranges, hkbSenseHandleModifier *par, int index){
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
                WARNING_MESSAGE("RangesUI::loadData(): Invalid event id!!!");
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
        minDistance->setValue(bsData->minDistance);
        maxDistance->setValue(bsData->maxDistance);
        ignoreHandle->setChecked(bsData->ignoreHandle);
        hkbVariableBindingSet *varBind = static_cast<hkbVariableBindingSet *>(parent->variableBindingSet.data());
        if (varBind){
            loadBinding(MINIMUM_DISTANCE_ROW, BINDING_COLUMN, varBind, "ranges:"+QString::number(rangeIndex)+"/minDistance");
            loadBinding(MAXIMUM_DISTANCE_ROW, BINDING_COLUMN, varBind, "ranges:"+QString::number(rangeIndex)+"/maxDistance");
            loadBinding(IGNORE_HANDLE_ROW, BINDING_COLUMN, varBind, "ranges:"+QString::number(rangeIndex)+"/ignoreHandle");
        }else{
            table->item(MINIMUM_DISTANCE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(MAXIMUM_DISTANCE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(IGNORE_HANDLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
        }
    }else{
        FATAL_RUNTIME_ERROR("RangesUI::loadData(): Behavior file, bind or event data is null!!!");
    }
    connectSignals();
}

void RangesUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
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
            FATAL_RUNTIME_ERROR("RangesUI::loadBinding(): The variable binding set is nullptr!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("RangesUI::loadBinding(): The data is nullptr!!");
    }
}

bool RangesUI::setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty){
    hkbVariableBindingSet *varBind = static_cast<hkbVariableBindingSet *>(parent->variableBindingSet.data());
    if (bsData){
        if (index == 0){
            varBind->removeBinding(path);if (varBind->getNumberOfBindings() == 0){static_cast<HkDynamicObject *>(parent)->variableBindingSet = HkxSharedPtr();}
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
        }else if ((!isProperty && static_cast<BehaviorFile *>(file)->getVariableTypeAt(index - 1) == type) ||
                  (isProperty && static_cast<BehaviorFile *>(file)->getCharacterPropertyTypeAt(index - 1) == type)){
            if (!varBind){
                varBind = new hkbVariableBindingSet(file);
                parent->variableBindingSet = HkxSharedPtr(varBind);
            }
            if (isProperty){
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY)){
                    FATAL_RUNTIME_ERROR("EvaluateExpressionModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }else{
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    FATAL_RUNTIME_ERROR("EvaluateExpressionModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            file->setIsChanged(true);
        }else{
            WARNING_MESSAGE("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("RangesUI::setBinding(): The data is nullptr!!");
    }
    return true;
}

void RangesUI::setBindingVariable(int index, const QString & name){
    if (bsData){
        bool isProperty = false;
        int row = table->currentRow();
        switch (row){
        case MINIMUM_DISTANCE_ROW:
            if (table->item(MINIMUM_DISTANCE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "ranges:"+QString::number(rangeIndex)+"/minDistance", VARIABLE_TYPE_REAL, isProperty);
            break;
        case MAXIMUM_DISTANCE_ROW:
            if (table->item(MAXIMUM_DISTANCE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "ranges:"+QString::number(rangeIndex)+"/maxDistance", VARIABLE_TYPE_REAL, isProperty);
            break;
        case IGNORE_HANDLE_ROW:
            if (table->item(IGNORE_HANDLE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "ranges:"+QString::number(rangeIndex)+"/ignoreHandle", VARIABLE_TYPE_BOOL, isProperty);
            break;
        default:
            return;
        }
        file->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("RangesUI::setBindingVariable(): The data is nullptr!!");
    }
}

/*QSize RangesUI::sizeHint() const{
    return QSize(1600, 800);
}

QSize RangesUI::minimumSizeHint() const{
    return QSize(1200, 600);
}*/

void RangesUI::setEventId(int index, const QString & name){
    if (bsData && file){
        index--;
        if (bsData->event.id != index){
            bsData->event.id = index;
            table->item(EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
            file->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("RangesUI::setEvent(): Behavior file or event data is null!!!");
    }
}

void RangesUI::setEventPayload(){
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
        file->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("RangesUI::setEventPayload(): Behavior file or event data is null!!!");
    }
}

void RangesUI::setMinDistance(){
    if (bsData && file){
        if (bsData->minDistance != minDistance->value()){
            bsData->minDistance = minDistance->value();
            file->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("RangesUI::setMinDistance(): Behavior file or event data is null!!!");
    }
}

void RangesUI::setMaxDistance(){
    if (bsData && file){
        if (bsData->maxDistance != maxDistance->value()){
            bsData->maxDistance = maxDistance->value();
            file->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("RangesUI::setMaxDistance(): Behavior file or event data is null!!!");
    }
}

void RangesUI::setIgnoreHandle(){
    if (bsData && file){
        if (bsData->ignoreHandle != ignoreHandle->isChecked()){
            bsData->ignoreHandle = ignoreHandle->isChecked();
            file->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("RangesUI::setIsAnnotation(): Behavior file or event data is null!!!");
    }
}

void RangesUI::viewSelectedChild(int row, int column){
    if (bsData){
        bool properties = false;
        if (column == BINDING_COLUMN){
            switch (row){
            case MINIMUM_DISTANCE_ROW:
                if (table->item(MINIMUM_DISTANCE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "ranges:"+QString::number(rangeIndex)+"/minDistance");
                break;
            case MAXIMUM_DISTANCE_ROW:
                if (table->item(MAXIMUM_DISTANCE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "ranges:"+QString::number(rangeIndex)+"/maxDistance");
                break;
            case IGNORE_HANDLE_ROW:
                if (table->item(IGNORE_HANDLE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "ranges:"+QString::number(rangeIndex)+"/ignoreHandle");
                break;
            }
        }else if (row == EVENT_ID_ROW && column == VALUE_COLUMN){
                emit viewEvents(bsData->event.id + 1, QString(), QStringList());
            }
    }else{
        FATAL_RUNTIME_ERROR("RangesUI::viewSelectedChild(): The data is nullptr!!");
    }
}

void RangesUI::selectTableToView(bool viewproperties, const QString & path){
    if (bsData){
        if (viewproperties){
            if (parent->variableBindingSet.data()){
                emit viewProperties(static_cast<hkbVariableBindingSet *>(parent->variableBindingSet.data())->getVariableIndexOfBinding(path) + 1, QString(), QStringList());
            }else{
                emit viewProperties(0, QString(), QStringList());
            }
        }else{
            if (parent->variableBindingSet.data()){
                emit viewVariables(static_cast<hkbVariableBindingSet *>(parent->variableBindingSet.data())->getVariableIndexOfBinding(path) + 1, QString(), QStringList());
            }else{
                emit viewVariables(0, QString(), QStringList());
            }
        }
    }else{
        FATAL_RUNTIME_ERROR("RangesUI::selectTableToView(): The data is nullptr!!");
    }
}

void RangesUI::eventRenamed(const QString & name, int index){
    if (bsData){
        if (index == bsData->event.id){
            table->item(EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        }
    }else{
        FATAL_RUNTIME_ERROR("RangesUI::eventRenamed(): The data is nullptr!!");
    }
}

void RangesUI::variableRenamed(const QString & name, int index){
    int bindIndex = -1;
    hkbVariableBindingSet *bind = nullptr;
    if (name == ""){
        WARNING_MESSAGE("RangesUI::variableRenamed(): The new variable name is the empty string!!");
    }
    if (bsData){
        //index--;
        bind = static_cast<hkbVariableBindingSet *>(parent->variableBindingSet.data());
        if (bind){
            bindIndex = bind->getVariableIndexOfBinding("ranges:"+QString::number(rangeIndex)+"/minDistance");
            if (bindIndex == index){
                table->item(MINIMUM_DISTANCE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("ranges:"+QString::number(rangeIndex)+"/maxDistance");
            if (bindIndex == index){
                table->item(MAXIMUM_DISTANCE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("ranges:"+QString::number(rangeIndex)+"/ignoreHandle");
            if (bindIndex == index){
                table->item(IGNORE_HANDLE_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        FATAL_RUNTIME_ERROR("RangesUI::variableRenamed(): The data is nullptr!!");
    }
}
