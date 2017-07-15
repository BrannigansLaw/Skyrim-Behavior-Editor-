#include "bseventondeactivatemodifierui.h"


#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/hkbstringeventpayload.h"
#include "src/hkxclasses/behavior/modifiers/BSEventOnDeactivateModifier.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/genericdatawidgets.h"

#include <QGridLayout>
#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 4

#define NAME_ROW 0
#define ENABLE_ROW 1
#define EVENT_ID_ROW 2
#define EVENT_PAYLOAD_ROW 3

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList BSEventOnDeactivateModifierUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

BSEventOnDeactivateModifierUI::BSEventOnDeactivateModifierUI()
    : bsData(NULL),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      enable(new CheckBox),
      eventPayload(new QLineEdit)
{
    setTitle("BSEventOnDeactivateModifierUI");
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
    table->setItem(EVENT_ID_ROW, NAME_COLUMN, new TableWidgetItem("eventId"));
    table->setItem(EVENT_ID_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(EVENT_ID_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(EVENT_ID_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_EVENTS_TABLE_TIP));
    table->setItem(EVENT_PAYLOAD_ROW, NAME_COLUMN, new TableWidgetItem("eventPayload"));
    table->setItem(EVENT_PAYLOAD_ROW, TYPE_COLUMN, new TableWidgetItem("hkbStringEventPayload", Qt::AlignCenter));
    table->setItem(EVENT_PAYLOAD_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(EVENT_PAYLOAD_ROW, VALUE_COLUMN, eventPayload);
    topLyt->addWidget(table, 0, 0, 8, 3);
    setLayout(topLyt);
}

void BSEventOnDeactivateModifierUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
    connect(eventPayload, SIGNAL(editingFinished()), this, SLOT(setEventPayload()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)), Qt::UniqueConnection);
}

void BSEventOnDeactivateModifierUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    disconnect(eventPayload, SIGNAL(editingFinished()), this, SLOT(setEventPayload()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)));
}

void BSEventOnDeactivateModifierUI::connectToTables(GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *events){
    if (variables && properties && events){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(events, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(events, SIGNAL(elementSelected(int,QString)), this, SLOT(setEventId(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int)), variables, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int)), properties, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewEvents(int)), events, SLOT(showTable(int)), Qt::UniqueConnection);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSEventOnDeactivateModifierUI::connectToTables(): One or more arguments are NULL!!"))
    }
}

void BSEventOnDeactivateModifierUI::loadData(HkxObject *data){
    disconnectSignals();
    if (data){
        if (data->getSignature() == BS_EVENT_ON_DEACTIVATE_MODIFIER){
            bsData = static_cast<BSEventOnDeactivateModifier *>(data);
            hkbVariableBindingSet *varBind = NULL;
            hkbStringEventPayload *payload = static_cast<hkbStringEventPayload *>(bsData->event.payload.data());
            name->setText(bsData->name);
            enable->setChecked(bsData->enable);
            QString text = static_cast<BehaviorFile *>(bsData->getParentFile())->getEventNameAt(bsData->event.id);
            if (text != ""){
                table->item(EVENT_ID_ROW, VALUE_COLUMN)->setText(text);
            }else{
                table->item(EVENT_ID_ROW, VALUE_COLUMN)->setText("None");
            }
            if (payload){
                eventPayload->setText(payload->data);
            }else{
                eventPayload->setText("");
            }
            varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (varBind){
                loadBinding(ENABLE_ROW, BINDING_COLUMN, varBind, "enable");
            }else{
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
        }else{
            CRITICAL_ERROR_MESSAGE(QString("BSEventOnDeactivateModifierUI::loadData(): The data is an incorrect type!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSEventOnDeactivateModifierUI::loadData(): The data is NULL!!"));
    }
    connectSignals();
}

void BSEventOnDeactivateModifierUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            bsData->getParentFile()->toggleChanged(true);
            emit modifierNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSEventOnDeactivateModifierUI::setName(): The data is NULL!!"));
    }
}

void BSEventOnDeactivateModifierUI::setEnable(){
    if (bsData){
        if (bsData->enable != enable->isChecked()){
            bsData->enable = enable->isChecked();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSEventOnDeactivateModifierUI::setEnable(): The data is NULL!!"));
    }
}

void BSEventOnDeactivateModifierUI::setEventId(int index, const QString & name){
    if (bsData){
        index--;
        if (bsData->event.id != index){
            bsData->event.id = index;
            table->item(EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSEventOnDeactivateModifierUI::seteventId(): The data is NULL!!"));
    }
}

void BSEventOnDeactivateModifierUI::setEventPayload(){
    hkbStringEventPayload *payload;
    if (bsData){
        payload = static_cast<hkbStringEventPayload *>(bsData->event.payload.data());
        if (eventPayload->text() != ""){
            if (payload){
                payload->data = eventPayload->text();
            }else{
                payload = new hkbStringEventPayload(bsData->getParentFile(), eventPayload->text());
                bsData->getParentFile()->addObjectToFile(payload, -1);
                bsData->event.payload = HkxSharedPtr(payload);
            }
        }else{
            bsData->event.payload = HkxSharedPtr();
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSEventOnDeactivateModifierUI::seteventPayload(): The data is NULL!!"));
    }
}

void BSEventOnDeactivateModifierUI::viewSelected(int row, int column){
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
            default:
                return;
            }
        }else if (column == VALUE_COLUMN && row == EVENT_ID_ROW){
            emit viewEvents(bsData->event.id + 1);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSEventOnDeactivateModifierUI::viewSelected(): The 'bsData' pointer is NULL!!"))
    }
}

void BSEventOnDeactivateModifierUI::selectTableToView(bool viewisProperty, const QString & path){
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
        CRITICAL_ERROR_MESSAGE(QString("BSEventOnDeactivateModifierUI::selectTableToView(): The data is NULL!!"));
    }
}

void BSEventOnDeactivateModifierUI::eventRenamed(const QString & name, int index){
    if (bsData){
        index--;
        if (index == bsData->event.id){
            table->item(EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSEventOnDeactivateModifierUI::eventRenamed(): The data is NULL!!"));
    }
}

void BSEventOnDeactivateModifierUI::variableRenamed(const QString & name, int index){
    if (bsData){
        index--;
        hkbVariableBindingSet *bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (bind){
            int bindIndex = bind->getVariableIndexOfBinding("enable");
            if (bindIndex == index){
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSEventOnDeactivateModifierUI::variableRenamed(): The 'bsData' pointer is NULL!!"))
    }
}

bool BSEventOnDeactivateModifierUI::setBinding(int index, int row, const QString &variableName, const QString &path, hkVariableType type, bool isProperty){
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
                    CRITICAL_ERROR_MESSAGE(QString("BSEventOnDeactivateModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }else{
                if (!varBind->addBinding(path, variableName, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    CRITICAL_ERROR_MESSAGE(QString("BSEventOnDeactivateModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->toggleChanged(true);
        }else{
            WARNING_MESSAGE(QString("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSEventOnDeactivateModifierUI::setBinding(): The data is NULL!!"));
    }
    return true;
}

void BSEventOnDeactivateModifierUI::setBindingVariable(int index, const QString &name){
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
        default:
            return;
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSEventOnDeactivateModifierUI::setBindingVariable(): The data is NULL!!"));
    }
}

void BSEventOnDeactivateModifierUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
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
                if (varName == ""){
                    varName = "NONE";
                }
                table->item(row, colunm)->setText(BINDING_ITEM_LABEL+varName);
            }
        }else{
            CRITICAL_ERROR_MESSAGE(QString("BSEventOnDeactivateModifierUI::loadBinding(): The variable binding set is NULL!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSEventOnDeactivateModifierUI::loadBinding(): The data is NULL!!"));
    }
}
