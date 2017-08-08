#include "bspassbytargettriggermodifierui.h"

#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/hkbstringeventpayload.h"
#include "src/hkxclasses/behavior/modifiers/BSPassByTargetTriggerModifier.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/genericdatawidgets.h"

#include <QGridLayout>
#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 7

#define NAME_ROW 0
#define ENABLE_ROW 1
#define TARGET_POSITION_ROW 2
#define RADIUS_ROW 3
#define MOVEMENT_DIRECTION_ROW 4
#define TRIGGER_EVENT_ID_ROW 5
#define TRIGGER_EVENT_PAYLOAD_ROW 6

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList BSPassByTargetTriggerModifierUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

BSPassByTargetTriggerModifierUI::BSPassByTargetTriggerModifierUI()
    : bsData(NULL),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      enable(new CheckBox),
      targetPosition(new QuadVariableWidget),
      radius(new DoubleSpinBox),
      movementDirection(new QuadVariableWidget),
      triggerEventPayload(new QLineEdit)
{
    setTitle("BSPassByTargetTriggerModifierUI");
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
    table->setItem(RADIUS_ROW, NAME_COLUMN, new TableWidgetItem("radius"));
    table->setItem(RADIUS_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(RADIUS_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(RADIUS_ROW, VALUE_COLUMN, radius);
    table->setItem(MOVEMENT_DIRECTION_ROW, NAME_COLUMN, new TableWidgetItem("movementDirection"));
    table->setItem(MOVEMENT_DIRECTION_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(MOVEMENT_DIRECTION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(MOVEMENT_DIRECTION_ROW, VALUE_COLUMN, movementDirection);
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

void BSPassByTargetTriggerModifierUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
    connect(targetPosition, SIGNAL(editingFinished()), this, SLOT(setTargetPosition()), Qt::UniqueConnection);
    connect(radius, SIGNAL(editingFinished()), this, SLOT(setRadius()), Qt::UniqueConnection);
    connect(movementDirection, SIGNAL(editingFinished()), this, SLOT(setMovementDirection()), Qt::UniqueConnection);
    connect(triggerEventPayload, SIGNAL(editingFinished()), this, SLOT(setTriggerEventPayload()), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setResetAlarm()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)), Qt::UniqueConnection);
}

void BSPassByTargetTriggerModifierUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    disconnect(targetPosition, SIGNAL(editingFinished()), this, SLOT(setTargetPosition()));
    disconnect(radius, SIGNAL(editingFinished()), this, SLOT(setRadius()));
    disconnect(movementDirection, SIGNAL(editingFinished()), this, SLOT(setMovementDirection()));
    disconnect(triggerEventPayload, SIGNAL(editingFinished()), this, SLOT(setTriggerEventPayload()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)));
}

void BSPassByTargetTriggerModifierUI::connectToTables(GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *events){
    if (variables && properties && events){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(events, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(events, SIGNAL(elementSelected(int,QString)), this, SLOT(setTriggerEventId(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int)), variables, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int)), properties, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewEvents(int)), events, SLOT(showTable(int)), Qt::UniqueConnection);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSPassByTargetTriggerModifierUI::connectToTables(): One or more arguments are NULL!!"))
    }
}

void BSPassByTargetTriggerModifierUI::loadData(HkxObject *data){
    disconnectSignals();
    if (data){
        if (data->getSignature() == BS_PASS_BY_TARGET_TRIGGER_MODIFIER){
            bsData = static_cast<BSPassByTargetTriggerModifier *>(data);
            hkbVariableBindingSet *varBind = NULL;
            hkbStringEventPayload *payload = static_cast<hkbStringEventPayload *>(bsData->triggerEvent.payload.data());
            name->setText(bsData->name);
            enable->setChecked(bsData->enable);
            targetPosition->setValue(bsData->targetPosition);
            radius->setValue(bsData->radius);
            movementDirection->setValue(bsData->movementDirection);
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
                loadBinding(RADIUS_ROW, BINDING_COLUMN, varBind, "radius");
                loadBinding(MOVEMENT_DIRECTION_ROW, BINDING_COLUMN, varBind, "movementDirection");
            }else{
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(TARGET_POSITION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(RADIUS_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(MOVEMENT_DIRECTION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
        }else{
            CRITICAL_ERROR_MESSAGE(QString("BSPassByTargetTriggerModifierUI::loadData(): The data is an incorrect type!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSPassByTargetTriggerModifierUI::loadData(): The data is NULL!!"));
    }
    connectSignals();
}

void BSPassByTargetTriggerModifierUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            bsData->getParentFile()->toggleChanged(true);
            emit modifierNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSPassByTargetTriggerModifierUI::setName(): The data is NULL!!"));
    }
}

void BSPassByTargetTriggerModifierUI::setEnable(){
    if (bsData){
        bsData->enable = enable->isChecked();
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSPassByTargetTriggerModifierUI::setEnable(): The data is NULL!!"));
    }
}

void BSPassByTargetTriggerModifierUI::setTargetPosition(){
    if (bsData){
        if (bsData->targetPosition != targetPosition->value()){
            bsData->targetPosition = targetPosition->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSPassByTargetTriggerModifierUI::settargetPosition(): The data is NULL!!"));
    }
}

void BSPassByTargetTriggerModifierUI::setRadius(){
    if (bsData){
        if (bsData->radius != radius->value()){
            bsData->radius = radius->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSPassByTargetTriggerModifierUI::setradius(): The data is NULL!!"));
    }
}

void BSPassByTargetTriggerModifierUI::setMovementDirection(){
    if (bsData){
        if (bsData->movementDirection != movementDirection->value()){
            bsData->movementDirection = movementDirection->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSPassByTargetTriggerModifierUI::setmovementDirection(): The data is NULL!!"));
    }
}

void BSPassByTargetTriggerModifierUI::setTriggerEventId(int index, const QString & name){
    if (bsData){
        index--;
        if (bsData->triggerEvent.id != index){
            bsData->triggerEvent.id = index;
            table->item(TRIGGER_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSPassByTargetTriggerModifierUI::settriggerEventId(): The data is NULL!!"));
    }
}

void BSPassByTargetTriggerModifierUI::setTriggerEventPayload(){
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
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSPassByTargetTriggerModifierUI::settriggerEventPayload(): The data is NULL!!"));
    }
}

void BSPassByTargetTriggerModifierUI::viewSelected(int row, int column){
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
            case RADIUS_ROW:
                if (table->item(RADIUS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "radius");
                break;
            case MOVEMENT_DIRECTION_ROW:
                if (table->item(MOVEMENT_DIRECTION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "movementDirection");
                break;
            default:
                return;
            }
        }else if (column == VALUE_COLUMN && row == TRIGGER_EVENT_ID_ROW){
            emit viewEvents(bsData->triggerEvent.id + 1);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSPassByTargetTriggerModifierUI::viewSelected(): The 'bsData' pointer is NULL!!"))
    }
}

void BSPassByTargetTriggerModifierUI::selectTableToView(bool viewisProperty, const QString & path){
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
        CRITICAL_ERROR_MESSAGE(QString("BSPassByTargetTriggerModifierUI::selectTableToView(): The data is NULL!!"));
    }
}

void BSPassByTargetTriggerModifierUI::eventRenamed(const QString & name, int index){
    if (bsData){
        index--;
        if (index == bsData->triggerEvent.id){
            table->item(TRIGGER_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSPassByTargetTriggerModifierUI::eventRenamed(): The data is NULL!!"));
    }
}

void BSPassByTargetTriggerModifierUI::variableRenamed(const QString & name, int index){
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
            bindIndex = bind->getVariableIndexOfBinding("radius");
            if (bindIndex == index){
                table->item(RADIUS_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("movementDirection");
            if (bindIndex == index){
                table->item(MOVEMENT_DIRECTION_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSPassByTargetTriggerModifierUI::variableRenamed(): The 'bsData' pointer is NULL!!"))
    }
}

bool BSPassByTargetTriggerModifierUI::setBinding(int index, int row, const QString &variableName, const QString &path, hkVariableType type, bool isProperty){
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
                    CRITICAL_ERROR_MESSAGE(QString("BSPassByTargetTriggerModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }else{
                if (!varBind->addBinding(path, variableName, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    CRITICAL_ERROR_MESSAGE(QString("BSPassByTargetTriggerModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->toggleChanged(true);
        }else{
            WARNING_MESSAGE(QString("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSPassByTargetTriggerModifierUI::setBinding(): The data is NULL!!"));
    }
    return true;
}

void BSPassByTargetTriggerModifierUI::setBindingVariable(int index, const QString &name){
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
        case RADIUS_ROW:
            if (table->item(RADIUS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "radius", VARIABLE_TYPE_REAL, isProperty);
            break;
        case MOVEMENT_DIRECTION_ROW:
            if (table->item(MOVEMENT_DIRECTION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "movementDirection", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        default:
            return;
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSPassByTargetTriggerModifierUI::setBindingVariable(): The data is NULL!!"));
    }
}

void BSPassByTargetTriggerModifierUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
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
            CRITICAL_ERROR_MESSAGE(QString("BSPassByTargetTriggerModifierUI::loadBinding(): The variable binding set is NULL!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSPassByTargetTriggerModifierUI::loadBinding(): The data is NULL!!"));
    }
}

