#include "bsdisttriggermodifierui.h"

#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/hkbstringeventpayload.h"
#include "src/hkxclasses/behavior/modifiers/bsdisttriggermodifier.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/genericdatawidgets.h"

#include <QGridLayout>
#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 7

#define NAME_ROW 0
#define ENABLE_ROW 1
#define TARGET_POSITION_ROW 2
#define DISTANCE_ROW 3
#define DISTANCE_TRIGGER_ROW 4
#define TRIGGER_EVENT_ID_ROW 5
#define TRIGGER_EVENT_PAYLOAD_ROW 6

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList BSDistTriggerModifierUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

BSDistTriggerModifierUI::BSDistTriggerModifierUI()
    : bsData(nullptr),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      enable(new CheckBox),
      targetPosition(new QuadVariableWidget),
      distance(new DoubleSpinBox),
      distanceTrigger(new DoubleSpinBox),
      triggerEventPayload(new QLineEdit)
{
    setTitle("BSDistTriggerModifier");
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
    table->setItem(TARGET_POSITION_ROW, NAME_COLUMN, new TableWidgetItem("targetPosition"));
    table->setItem(TARGET_POSITION_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(TARGET_POSITION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(TARGET_POSITION_ROW, VALUE_COLUMN, targetPosition);
    table->setItem(DISTANCE_ROW, NAME_COLUMN, new TableWidgetItem("distance"));
    table->setItem(DISTANCE_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(DISTANCE_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(DISTANCE_ROW, VALUE_COLUMN, distance);
    table->setItem(DISTANCE_TRIGGER_ROW, NAME_COLUMN, new TableWidgetItem("distanceTrigger"));
    table->setItem(DISTANCE_TRIGGER_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(DISTANCE_TRIGGER_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(DISTANCE_TRIGGER_ROW, VALUE_COLUMN, distanceTrigger);
    table->setItem(TRIGGER_EVENT_ID_ROW, NAME_COLUMN, new TableWidgetItem("triggerEventId"));
    table->setItem(TRIGGER_EVENT_ID_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(TRIGGER_EVENT_ID_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(TRIGGER_EVENT_ID_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_EVENTS_TABLE_TIP));
    table->setItem(TRIGGER_EVENT_PAYLOAD_ROW, NAME_COLUMN, new TableWidgetItem("triggerEventPayload"));
    table->setItem(TRIGGER_EVENT_PAYLOAD_ROW, TYPE_COLUMN, new TableWidgetItem("hkbStringEventPayload", Qt::AlignCenter));
    table->setItem(TRIGGER_EVENT_PAYLOAD_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(TRIGGER_EVENT_PAYLOAD_ROW, VALUE_COLUMN, triggerEventPayload);
    topLyt->addWidget(table, 0, 0, 8, 3);
    setLayout(topLyt);
}

void BSDistTriggerModifierUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
    connect(targetPosition, SIGNAL(editingFinished()), this, SLOT(setTargetPosition()), Qt::UniqueConnection);
    connect(distance, SIGNAL(editingFinished()), this, SLOT(setDistance()), Qt::UniqueConnection);
    connect(distanceTrigger, SIGNAL(editingFinished()), this, SLOT(setDistanceTrigger()), Qt::UniqueConnection);
    connect(triggerEventPayload, SIGNAL(editingFinished()), this, SLOT(setTriggerEventPayload()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)), Qt::UniqueConnection);
}

void BSDistTriggerModifierUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    disconnect(targetPosition, SIGNAL(editingFinished()), this, SLOT(setTargetPosition()));
    disconnect(distance, SIGNAL(editingFinished()), this, SLOT(setDistance()));
    disconnect(distanceTrigger, SIGNAL(editingFinished()), this, SLOT(setDistanceTrigger()));
    disconnect(triggerEventPayload, SIGNAL(editingFinished()), this, SLOT(setTriggerEventPayload()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)));
}

void BSDistTriggerModifierUI::connectToTables(GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *events){
    if (variables && properties && events){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(events, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(events, SIGNAL(elementSelected(int,QString)), this, SLOT(setTriggerEventId(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int,QString,QStringList)), variables, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int,QString,QStringList)), properties, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewEvents(int,QString,QStringList)), events, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
    }else{
        CRITICAL_ERROR_MESSAGE("BSDistTriggerModifierUI::connectToTables(): One or more arguments are nullptr!!");
    }
}

void BSDistTriggerModifierUI::loadData(HkxObject *data){
    disconnectSignals();
    if (data){
        if (data->getSignature() == BS_DIST_TRIGGER_MODIFER){
            bsData = static_cast<BSDistTriggerModifier *>(data);
            hkbVariableBindingSet *varBind = nullptr;
            hkbStringEventPayload *payload = static_cast<hkbStringEventPayload *>(bsData->triggerEvent.payload.data());
            name->setText(bsData->name);
            enable->setChecked(bsData->enable);
            targetPosition->setValue(bsData->targetPosition);
            distance->setValue(bsData->distance);
            distanceTrigger->setValue(bsData->distanceTrigger);
            QString text = static_cast<BehaviorFile *>(bsData->getParentFile())->getEventNameAt(bsData->triggerEvent.id);
            if (text != ""){
                table->item(TRIGGER_EVENT_ID_ROW, VALUE_COLUMN)->setText(text);
            }else{
                table->item(TRIGGER_EVENT_ID_ROW, VALUE_COLUMN)->setText("None");
            }
            if (payload){
                triggerEventPayload->setText(payload->data);
            }else{
                triggerEventPayload->setText("");
            }
            varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (varBind){
                loadBinding(ENABLE_ROW, BINDING_COLUMN, varBind, "enable");
                loadBinding(TARGET_POSITION_ROW, BINDING_COLUMN, varBind, "targetPosition");
                loadBinding(DISTANCE_ROW, BINDING_COLUMN, varBind, "distance");
                loadBinding(DISTANCE_TRIGGER_ROW, BINDING_COLUMN, varBind, "distanceTrigger");
            }else{
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(TARGET_POSITION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(DISTANCE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(DISTANCE_TRIGGER_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
        }else{
            CRITICAL_ERROR_MESSAGE("BSDistTriggerModifierUI::loadData(): The data is an incorrect type!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSDistTriggerModifierUI::loadData(): The data is nullptr!!");
    }
    connectSignals();
}

void BSDistTriggerModifierUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            bsData->getParentFile()->setIsChanged(true);
            emit modifierNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData));
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSDistTriggerModifierUI::setName(): The data is nullptr!!");
    }
}

void BSDistTriggerModifierUI::setEnable(){
    if (bsData){
        bsData->enable = enable->isChecked();
        bsData->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("BSDistTriggerModifierUI::setEnable(): The data is nullptr!!");
    }
}

void BSDistTriggerModifierUI::setTargetPosition(){
    if (bsData){
        if (bsData->targetPosition != targetPosition->value()){
            bsData->targetPosition = targetPosition->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSDistTriggerModifierUI::settargetPosition(): The data is nullptr!!");
    }
}

void BSDistTriggerModifierUI::setDistance(){
    if (bsData){
        if (bsData->distance != distance->value()){
            bsData->distance = distance->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSDistTriggerModifierUI::setdistance(): The data is nullptr!!");
    }
}

void BSDistTriggerModifierUI::setDistanceTrigger(){
    if (bsData){
        if (bsData->distanceTrigger != distanceTrigger->value()){
            bsData->distanceTrigger = distanceTrigger->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSDistTriggerModifierUI::setdistanceTrigger(): The data is nullptr!!");
    }
}

void BSDistTriggerModifierUI::setTriggerEventId(int index, const QString & name){
    if (bsData){
        index--;
        if (bsData->triggerEvent.id != index){
            bsData->triggerEvent.id = index;
            table->item(TRIGGER_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSDistTriggerModifierUI::setTriggerEventId(): The data is nullptr!!");
    }
}

void BSDistTriggerModifierUI::setTriggerEventPayload(){
    hkbStringEventPayload *payload;
    if (bsData){
        payload = static_cast<hkbStringEventPayload *>(bsData->triggerEvent.payload.data());
        if (triggerEventPayload->text() != ""){
            if (payload){
                payload->data = triggerEventPayload->text();
            }else{
                payload = new hkbStringEventPayload(bsData->getParentFile(), triggerEventPayload->text());
                bsData->getParentFile()->addObjectToFile(payload, -1);
                bsData->triggerEvent.payload = HkxSharedPtr(payload);
            }
        }else{
            bsData->triggerEvent.payload = HkxSharedPtr();
        }
        bsData->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("BSDistTriggerModifierUI::setTriggerEventPayload(): The data is nullptr!!");
    }
}

void BSDistTriggerModifierUI::viewSelected(int row, int column){
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
            case TARGET_POSITION_ROW:
                if (table->item(TARGET_POSITION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "targetPosition");
                break;
            case DISTANCE_ROW:
                if (table->item(DISTANCE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "distance");
                break;
            case DISTANCE_TRIGGER_ROW:
                if (table->item(DISTANCE_TRIGGER_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "distanceTrigger");
                break;
            default:
                return;
            }
        }else if (column == VALUE_COLUMN && row == TRIGGER_EVENT_ID_ROW){
            emit viewEvents(bsData->triggerEvent.id + 1, QString(), QStringList());
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSDistTriggerModifierUI::viewSelected(): The 'bsData' pointer is nullptr!!");
    }
}

void BSDistTriggerModifierUI::selectTableToView(bool viewisProperty, const QString & path){
    if (bsData){
        if (viewisProperty){
            if (bsData->variableBindingSet.data()){
                emit viewProperties(static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data())->getVariableIndexOfBinding(path) + 1, QString(), QStringList());
            }else{
                emit viewProperties(0, QString(), QStringList());
            }
        }else{
            if (bsData->variableBindingSet.data()){
                emit viewVariables(static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data())->getVariableIndexOfBinding(path) + 1, QString(), QStringList());
            }else{
                emit viewVariables(0, QString(), QStringList());
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSDistTriggerModifierUI::selectTableToView(): The data is nullptr!!");
    }
}

void BSDistTriggerModifierUI::eventRenamed(const QString & name, int index){
    if (bsData){
        index--;
        if (index == bsData->triggerEvent.id){
            table->item(TRIGGER_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSDistTriggerModifierUI::eventRenamed(): The data is nullptr!!");
    }
}

void BSDistTriggerModifierUI::variableRenamed(const QString & name, int index){
    if (bsData){
        index--;
        hkbVariableBindingSet *bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (bind){
            int bindIndex = bind->getVariableIndexOfBinding("enable");
            if (bindIndex == index){
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("targetPosition");
            if (bindIndex == index){
                table->item(TARGET_POSITION_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("distance");
            if (bindIndex == index){
                table->item(DISTANCE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("distanceTrigger");
            if (bindIndex == index){
                table->item(DISTANCE_TRIGGER_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSDistTriggerModifierUI::variableRenamed(): The 'bsData' pointer is nullptr!!");
    }
}

bool BSDistTriggerModifierUI::setBinding(int index, int row, const QString &variableName, const QString &path, hkVariableType type, bool isProperty){
    hkbVariableBindingSet *varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
    if (bsData){
        if (index == 0){
            varBind->removeBinding(path);if (varBind->getNumberOfBindings() == 0){static_cast<HkDynamicObject *>(bsData)->variableBindingSet = HkxSharedPtr(); static_cast<BehaviorFile *>(bsData->getParentFile())->removeOtherData();}
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
        }else if ((!isProperty && areVariableTypesCompatible(static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableTypeAt(index - 1), type)) ||
                  (isProperty && areVariableTypesCompatible(static_cast<BehaviorFile *>(bsData->getParentFile())->getCharacterPropertyTypeAt(index - 1), type))){
            if (!varBind){
                varBind = new hkbVariableBindingSet(bsData->getParentFile());
                bsData->variableBindingSet = HkxSharedPtr(varBind);
            }
            if (isProperty){
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY)){
                    CRITICAL_ERROR_MESSAGE("BSDistTriggerModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }else{
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    CRITICAL_ERROR_MESSAGE("BSDistTriggerModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->setIsChanged(true);
        }else{
            WARNING_MESSAGE("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSDistTriggerModifierUI::setBinding(): The data is nullptr!!");
    }
    return true;
}

void BSDistTriggerModifierUI::setBindingVariable(int index, const QString &name){
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
        case TARGET_POSITION_ROW:
            if (table->item(TARGET_POSITION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "targetPosition", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case DISTANCE_ROW:
            if (table->item(DISTANCE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "distance", VARIABLE_TYPE_REAL, isProperty);
            break;
        case DISTANCE_TRIGGER_ROW:
            if (table->item(DISTANCE_TRIGGER_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "distanceTrigger", VARIABLE_TYPE_REAL, isProperty);
            break;
        default:
            return;
        }
        bsData->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("BSDistTriggerModifierUI::setBindingVariable(): The data is nullptr!!");
    }
}

void BSDistTriggerModifierUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
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
            CRITICAL_ERROR_MESSAGE("BSDistTriggerModifierUI::loadBinding(): The variable binding set is nullptr!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSDistTriggerModifierUI::loadBinding(): The data is nullptr!!");
    }
}

