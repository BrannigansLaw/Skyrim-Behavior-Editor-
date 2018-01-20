#include "legui.h"

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

#define BASE_NUMBER_OF_ROWS 6

#define EVENT_ID_ROW 0
#define PAYLOAD_ROW 1
#define GROUND_POSITION_ROW 2
#define VERTICAL_ERROR_ROW 3
#define HIT_SOMETHING_ROW 4
#define IS_PLANTED_MS_ROW 5

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList LegUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

LegUI::LegUI()
    : file(nullptr),
      legIndex(-1),
      bsData(nullptr),
      parent(nullptr),
      topLyt(new QGridLayout),
      returnPB(new QPushButton("Return")),
      table(new TableWidget),
      groundPosition(new QuadVariableWidget),
      payload(new QLineEdit),
      verticalError(new DoubleSpinBox),
      hitSomething(new CheckBox),
      isPlantedMS(new CheckBox)
{
    setTitle("hkLeg");
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
    table->setItem(GROUND_POSITION_ROW, NAME_COLUMN, new TableWidgetItem("groundPosition"));
    table->setItem(GROUND_POSITION_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(GROUND_POSITION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(GROUND_POSITION_ROW, VALUE_COLUMN, groundPosition);
    table->setItem(VERTICAL_ERROR_ROW, NAME_COLUMN, new TableWidgetItem("verticalError"));
    table->setItem(VERTICAL_ERROR_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(VERTICAL_ERROR_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(VERTICAL_ERROR_ROW, VALUE_COLUMN, verticalError);
    table->setItem(HIT_SOMETHING_ROW, NAME_COLUMN, new TableWidgetItem("hitSomething"));
    table->setItem(HIT_SOMETHING_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(HIT_SOMETHING_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(HIT_SOMETHING_ROW, VALUE_COLUMN, hitSomething);
    table->setItem(IS_PLANTED_MS_ROW, NAME_COLUMN, new TableWidgetItem("isPlantedMS"));
    table->setItem(IS_PLANTED_MS_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(IS_PLANTED_MS_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(IS_PLANTED_MS_ROW, VALUE_COLUMN, isPlantedMS);
    topLyt->addWidget(returnPB, 0, 1, 1, 1);
    topLyt->addWidget(table, 1, 0, 6, 3);
    setLayout(topLyt);
    connectSignals();
}

void LegUI::connectSignals(){
    connect(returnPB, SIGNAL(released()), this, SIGNAL(returnToParent()), Qt::UniqueConnection);
    connect(groundPosition, SIGNAL(editingFinished()), this, SLOT(setGroundPosition()), Qt::UniqueConnection);
    connect(verticalError, SIGNAL(editingFinished()), this, SLOT(setVerticalError()), Qt::UniqueConnection);
    connect(hitSomething, SIGNAL(released()), this, SLOT(setHitSomething()), Qt::UniqueConnection);
    connect(isPlantedMS, SIGNAL(released()), this, SLOT(setIsPlantedMS()), Qt::UniqueConnection);
    connect(payload, SIGNAL(editingFinished()), this, SLOT(setEventPayload()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)), Qt::UniqueConnection);
}

void LegUI::disconnectSignals(){
    disconnect(returnPB, SIGNAL(released()), this, SIGNAL(returnToParent()));
    disconnect(groundPosition, SIGNAL(editingFinished()), this, SLOT(setGroundPosition()));
    disconnect(verticalError, SIGNAL(editingFinished()), this, SLOT(setVerticalError()));
    disconnect(hitSomething, SIGNAL(released()), this, SLOT(setHitSomething()));
    disconnect(isPlantedMS, SIGNAL(released()), this, SLOT(setIsPlantedMS()));
    disconnect(payload, SIGNAL(editingFinished()), this, SLOT(setEventPayload()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)));
}

void LegUI::loadData(BehaviorFile *parentFile, hkbFootIkControlsModifier::hkLeg *leg, hkbFootIkControlsModifier *par, int index){
    disconnectSignals();
    QString text;
    if (parentFile && leg && par && index > -1){
        parent = par;
        legIndex = index;
        file = parentFile;
        bsData = leg;
        text = file->getEventNameAt(leg->id);
        if (text == ""){
            if (leg->id != -1){
                WARNING_MESSAGE("LegUI::loadData(): Invalid event id!!!");
            }
            text = "NONE";
        }
        QString eventName = file->getEventNameAt(bsData->id);
        if (eventName != ""){
            table->item(EVENT_ID_ROW, VALUE_COLUMN)->setText(eventName);
        }else{
            table->item(EVENT_ID_ROW, VALUE_COLUMN)->setText("NONE");
        }
        if (leg->payload.data()){
            payload->setText(static_cast<hkbStringEventPayload *>(leg->payload.data())->data);
        }else{
            payload->setText("");
        }
        groundPosition->setValue(bsData->groundPosition);
        verticalError->setValue(bsData->verticalError);
        hitSomething->setChecked(bsData->hitSomething);
        isPlantedMS->setChecked(bsData->isPlantedMS);
        hkbVariableBindingSet *varBind = static_cast<hkbVariableBindingSet *>(parent->variableBindingSet.data());
        if (varBind){
            loadBinding(GROUND_POSITION_ROW, BINDING_COLUMN, varBind, "legs:"+QString::number(legIndex)+"/groundPosition");
            loadBinding(VERTICAL_ERROR_ROW, BINDING_COLUMN, varBind, "legs:"+QString::number(legIndex)+"/verticalError");
            loadBinding(HIT_SOMETHING_ROW, BINDING_COLUMN, varBind, "legs:"+QString::number(legIndex)+"/hitSomething");
            loadBinding(IS_PLANTED_MS_ROW, BINDING_COLUMN, varBind, "legs:"+QString::number(legIndex)+"/isPlantedMS");
        }else{
            table->item(GROUND_POSITION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(VERTICAL_ERROR_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(HIT_SOMETHING_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(IS_PLANTED_MS_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
        }
    }else{
        FATAL_RUNTIME_ERROR("LegUI::loadData(): Behavior file, bind or event data is null!!!");
    }
    connectSignals();
}

void LegUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
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
            FATAL_RUNTIME_ERROR("LegUI::loadBinding(): The variable binding set is nullptr!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("LegUI::loadBinding(): The data is nullptr!!");
    }
}

bool LegUI::setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty){
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
        FATAL_RUNTIME_ERROR("LegUI::setBinding(): The data is nullptr!!");
    }
    return true;
}

void LegUI::setBindingVariable(int index, const QString & name){
    if (bsData){
        bool isProperty = false;
        int row = table->currentRow();
        switch (row){
        case GROUND_POSITION_ROW:
            if (table->item(GROUND_POSITION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "legs:"+QString::number(legIndex)+"/groundPosition", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case VERTICAL_ERROR_ROW:
            if (table->item(VERTICAL_ERROR_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "legs:"+QString::number(legIndex)+"/verticalError", VARIABLE_TYPE_REAL, isProperty);
            break;
        case HIT_SOMETHING_ROW:
            if (table->item(HIT_SOMETHING_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "legs:"+QString::number(legIndex)+"/hitSomething", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case IS_PLANTED_MS_ROW:
            if (table->item(IS_PLANTED_MS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "legs:"+QString::number(legIndex)+"/isPlantedMS", VARIABLE_TYPE_BOOL, isProperty);
            break;
        default:
            return;
        }
        file->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("LegUI::setBindingVariable(): The data is nullptr!!");
    }
}

void LegUI::setEventId(int index, const QString & name){
    if (bsData && file){
        index--;
        if (bsData->id != index){
            bsData->id = index;
            table->item(EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
            file->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("LegUI::setEvent(): Behavior file or event data is null!!!");
    }
}

void LegUI::setEventPayload(){
    hkbStringEventPayload *payloadData;
    if (bsData && file){
        payloadData = static_cast<hkbStringEventPayload *>(bsData->payload.data());
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
                bsData->payload = HkxSharedPtr(payloadData);
            }
        }else{
            bsData->payload = HkxSharedPtr();
        }
        file->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("LegUI::setEventPayload(): Behavior file or event data is null!!!");
    }
}

void LegUI::setGroundPosition(){
    if (bsData && file){
        if (bsData->groundPosition != groundPosition->value()){
            bsData->groundPosition = groundPosition->value();
            file->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("LegUI::setGroundPosition(): Behavior file or event data is null!!!");
    }
}

void LegUI::setVerticalError(){
    if (bsData && file){
        if (bsData->verticalError != verticalError->value()){
            bsData->verticalError = verticalError->value();
            file->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("LegUI::setverticalError(): Behavior file or event data is null!!!");
    }
}

void LegUI::setHitSomething(){
    if (bsData && file){
        if (bsData->hitSomething != hitSomething->isChecked()){
            bsData->hitSomething = hitSomething->isChecked();
            file->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("LegUI::sethitSomething(): Behavior file or event data is null!!!");
    }
}

void LegUI::setIsPlantedMS(){
    if (bsData && file){
        if (bsData->isPlantedMS != isPlantedMS->isChecked()){
            bsData->isPlantedMS = isPlantedMS->isChecked();
            file->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("LegUI::setIsPlantedMS(): Behavior file or event data is null!!!");
    }
}

void LegUI::viewSelectedChild(int row, int column){
    if (bsData){
        bool properties = false;
        if (column == BINDING_COLUMN){
            switch (row){
            case GROUND_POSITION_ROW:
                if (table->item(GROUND_POSITION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "legs:"+QString::number(legIndex)+"/groundPosition");
                break;
            case VERTICAL_ERROR_ROW:
                if (table->item(VERTICAL_ERROR_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "legs:"+QString::number(legIndex)+"/verticalError");
                break;
            case HIT_SOMETHING_ROW:
                if (table->item(HIT_SOMETHING_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "legs:"+QString::number(legIndex)+"/hitSomething");
                break;
            case IS_PLANTED_MS_ROW:
                if (table->item(IS_PLANTED_MS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "legs:"+QString::number(legIndex)+"/isPlantedMS");
                break;
            }
        }else if (row == EVENT_ID_ROW && column == VALUE_COLUMN){
                emit viewEvents(bsData->id + 1);
            }
    }else{
        FATAL_RUNTIME_ERROR("LegUI::viewSelectedChild(): The data is nullptr!!");
    }
}

void LegUI::selectTableToView(bool viewproperties, const QString & path){
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
        FATAL_RUNTIME_ERROR("LegUI::selectTableToView(): The data is nullptr!!");
    }
}

void LegUI::eventRenamed(const QString & name, int index){
    if (bsData){
        if (index == bsData->id){
            table->item(EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        }
    }else{
        FATAL_RUNTIME_ERROR("LegUI::eventRenamed(): The data is nullptr!!");
    }
}

void LegUI::variableRenamed(const QString & name, int index){
    int bindIndex = -1;
    hkbVariableBindingSet *bind = nullptr;
    if (name == ""){
        WARNING_MESSAGE("LegUI::variableRenamed(): The new variable name is the empty string!!");
    }
    if (bsData){
        //index--;
        bind = static_cast<hkbVariableBindingSet *>(parent->variableBindingSet.data());
        if (bind){
            bindIndex = bind->getVariableIndexOfBinding("legs:"+QString::number(legIndex)+"/groundPosition");
            if (bindIndex == index){
                table->item(GROUND_POSITION_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("legs:"+QString::number(legIndex)+"/verticalError");
            if (bindIndex == index){
                table->item(VERTICAL_ERROR_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("legs:"+QString::number(legIndex)+"/hitSomething");
            if (bindIndex == index){
                table->item(HIT_SOMETHING_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("legs:"+QString::number(legIndex)+"/isPlantedMS");
            if (bindIndex == index){
                table->item(IS_PLANTED_MS_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        FATAL_RUNTIME_ERROR("LegUI::variableRenamed(): The data is nullptr!!");
    }
}
