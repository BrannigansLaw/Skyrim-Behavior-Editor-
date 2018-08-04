#include "eventdrivenmodifierui.h"

#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/modifiers/hkbeventdrivenmodifier.h"
#include "src/ui/genericdatawidgets.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/TreeGraphicsView.h"
#include "src/ui/behaviorgraphview.h"
#include "src/ui/treegraphicsitem.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"


#include <QHeaderView>
#include <QGroupBox>

#define BASE_NUMBER_OF_ROWS 6

#define NAME_ROW 0
#define ENABLE_ROW 1
#define ACTIVATE_EVENT_ID_ROW 2
#define DEACTIVATE_EVENT_ID_ROW 3
#define ACTIVE_BY_DEFAULT_ROW 4
#define MODIFIER_ROW 5

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList EventDrivenModifierUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

EventDrivenModifierUI::EventDrivenModifierUI()
    : behaviorView(nullptr),
      bsData(nullptr),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      enable(new CheckBox),
      activeByDefault(new CheckBox)
{
    setTitle("hkbEventDrivenModifier");
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(NAME_ROW, NAME_COLUMN, new TableWidgetItem("name"));
    table->setItem(NAME_ROW, TYPE_COLUMN, new TableWidgetItem("hkStringPtr", Qt::AlignCenter));
    table->setItem(NAME_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignVCenter));
    table->setCellWidget(NAME_ROW, VALUE_COLUMN, name);
    table->setItem(ENABLE_ROW, NAME_COLUMN, new TableWidgetItem("enable"));
    table->setItem(ENABLE_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(ENABLE_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ENABLE_ROW, VALUE_COLUMN, enable);
    table->setItem(ACTIVATE_EVENT_ID_ROW, NAME_COLUMN, new TableWidgetItem("activateEventId"));
    table->setItem(ACTIVATE_EVENT_ID_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(ACTIVATE_EVENT_ID_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(ACTIVATE_EVENT_ID_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_EVENTS_TABLE_TIP));
    table->setItem(DEACTIVATE_EVENT_ID_ROW, NAME_COLUMN, new TableWidgetItem("deactivateEventId"));
    table->setItem(DEACTIVATE_EVENT_ID_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(DEACTIVATE_EVENT_ID_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(DEACTIVATE_EVENT_ID_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_EVENTS_TABLE_TIP));
    table->setItem(ACTIVE_BY_DEFAULT_ROW, NAME_COLUMN, new TableWidgetItem("activeByDefault"));
    table->setItem(ACTIVE_BY_DEFAULT_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(ACTIVE_BY_DEFAULT_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ACTIVE_BY_DEFAULT_ROW, VALUE_COLUMN, activeByDefault);
    table->setItem(MODIFIER_ROW, NAME_COLUMN, new TableWidgetItem("modifier"));
    table->setItem(MODIFIER_ROW, TYPE_COLUMN, new TableWidgetItem("hkbModifier", Qt::AlignCenter));
    table->setItem(MODIFIER_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(MODIFIER_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_MODIFIERS_TABLE_TIP));
    topLyt->addWidget(table, 0, 0, 8, 3);
    setLayout(topLyt);
    connectSignals();
}

void EventDrivenModifierUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
    connect(activeByDefault, SIGNAL(released()), this, SLOT(setActiveByDefault()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)), Qt::UniqueConnection);
}

void EventDrivenModifierUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    disconnect(activeByDefault, SIGNAL(released()), this, SLOT(setActiveByDefault()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)));
}

void EventDrivenModifierUI::connectToTables(GenericTableWidget *modifiers, GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *events){
    if (variables && properties && modifiers && events){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(modifiers, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(events, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(modifiers, SIGNAL(elementSelected(int,QString)), this, SLOT(setModifier(int,QString)), Qt::UniqueConnection);
        connect(events, SIGNAL(elementSelected(int,QString)), this, SLOT(eventTableElementSelected(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewModifiers(int,QString,QStringList)), modifiers, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int,QString,QStringList)), variables, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int,QString,QStringList)), properties, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewEvents(int,QString,QStringList)), events, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
    }else{
        CRITICAL_ERROR_MESSAGE("EventDrivenModifierUI::connectToTables(): One or more arguments are nullptr!!");
    }
}

void EventDrivenModifierUI::loadData(HkxObject *data){
    disconnectSignals();
    hkbVariableBindingSet *varBind = nullptr;
    if (data){
        if (data->getSignature() == HKB_EVENT_DRIVEN_MODIFIER){
            bsData = static_cast<hkbEventDrivenModifier *>(data);
            name->setText(bsData->name);
            enable->setChecked(bsData->enable);
            QString eventName = static_cast<BehaviorFile *>(bsData->getParentFile())->getEventNameAt(bsData->activateEventId);
            if (eventName != ""){
                table->item(ACTIVATE_EVENT_ID_ROW, VALUE_COLUMN)->setText(eventName);
            }else{
                table->item(ACTIVATE_EVENT_ID_ROW, VALUE_COLUMN)->setText("NONE");
            }
            eventName = static_cast<BehaviorFile *>(bsData->getParentFile())->getEventNameAt(bsData->deactivateEventId);
            if (eventName != ""){
                table->item(DEACTIVATE_EVENT_ID_ROW, VALUE_COLUMN)->setText(eventName);
            }else{
                table->item(DEACTIVATE_EVENT_ID_ROW, VALUE_COLUMN)->setText("NONE");
            }
            activeByDefault->setChecked(bsData->activeByDefault);
            if (bsData->modifier.data()){
                table->item(MODIFIER_ROW, VALUE_COLUMN)->setText(static_cast<hkbModifier *>(bsData->modifier.data())->getName());
            }else{
                table->item(MODIFIER_ROW, VALUE_COLUMN)->setText("NONE");
            }
            varBind = bsData->getVariableBindingSetData();
            if (varBind){
                loadBinding(ENABLE_ROW, BINDING_COLUMN, varBind, "enable");
                loadBinding(ACTIVE_BY_DEFAULT_ROW, BINDING_COLUMN, varBind, "activeByDefault");
            }else{
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(ACTIVE_BY_DEFAULT_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
        }else{
            CRITICAL_ERROR_MESSAGE(QString("EventDrivenModifierUI::loadData(): The data passed to the UI is the wrong type!\nSIGNATURE: "+QString::number(data->getSignature(), 16)).toLocal8Bit().data());
        }
    }else{
        CRITICAL_ERROR_MESSAGE("EventDrivenModifierUI::loadData(): The data passed to the UI is nullptr!!!");
    }
    connectSignals();
}

void EventDrivenModifierUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            bsData->setIsFileChanged(true);
            emit modifierNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData));
        }
    }
}

void EventDrivenModifierUI::setEnable(){
    if (bsData){
        bsData->enable = enable->isChecked();
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("EventDrivenModifierUI::setEnable(): The data is nullptr!!");
    }
}

void EventDrivenModifierUI::setActivateEventId(int index, const QString & name){
    if (bsData){
        bsData->activateEventId = index - 1;
        table->item(ACTIVATE_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("EventDrivenModifierUI::setActivateEventId(): The data is nullptr!!");
    }
}

void EventDrivenModifierUI::setDeactivateEventId(int index, const QString & name){
    if (bsData){
        bsData->deactivateEventId = index - 1;
        table->item(DEACTIVATE_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("EventDrivenModifierUI::setDeactivateEventId(): The data is nullptr!!");
    }
}

void EventDrivenModifierUI::setActiveByDefault(){
    if (bsData){
        bsData->activeByDefault = activeByDefault->isChecked();
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("EventDrivenModifierUI::setActiveByDefault(): The data is nullptr!!");
    }
}

void EventDrivenModifierUI::eventTableElementSelected(int index, const QString &name){
    switch (table->currentRow()){
    case ACTIVATE_EVENT_ID_ROW:
        setActivateEventId(index, name);
        break;
    case DEACTIVATE_EVENT_ID_ROW:
        setDeactivateEventId(index, name);
        break;
    default:
        WARNING_MESSAGE("EventDrivenModifierUI::eventTableElementSelected(): An unwanted element selected event was recieved!!");
        return;
    }
}

void EventDrivenModifierUI::setModifier(int index, const QString & name){
    DataIconManager *ptr = nullptr;
    int indexOfModifier = -1;
    if (bsData){
        if (behaviorView){
            ptr = static_cast<BehaviorFile *>(bsData->getParentFile())->getModifierDataAt(index - 1);
            indexOfModifier = bsData->getIndexOfObj(static_cast<DataIconManager*>(bsData->modifier.data()));
            if (ptr){
                if (name != ptr->getName()){
                    CRITICAL_ERROR_MESSAGE("::setDefaultGenerator():The name of the selected object does not match it's name in the object selection table!!!");
                    return;
                }else if (ptr == bsData || !behaviorView->reconnectIcon(behaviorView->getSelectedItem(), static_cast<DataIconManager*>(bsData->modifier.data()), 0, ptr, false)){
                    WARNING_MESSAGE("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\nYou are attempting to create a circular branch or dead end!!!");
                    return;
                }
            }else{
                if (behaviorView->getSelectedItem()){
                    behaviorView->removeItemFromGraph(behaviorView->getSelectedItem()->getChildWithData(static_cast<DataIconManager*>(bsData->modifier.data())), indexOfModifier);
                }else{
                    CRITICAL_ERROR_MESSAGE("EventDrivenModifierUI::setModifier(): The selected icon is nullptr!!");
                    return;
                }
            }
            behaviorView->removeModifierData();
            table->item(MODIFIER_ROW, VALUE_COLUMN)->setText(name);
            bsData->setIsFileChanged(true);
        }else{
            CRITICAL_ERROR_MESSAGE("EventDrivenModifierUI::setModifier(): The 'behaviorView' pointer is nullptr!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("EventDrivenModifierUI::setModifier(): The 'bsData' pointer is nullptr!!");
    }
}

void EventDrivenModifierUI::loadBinding(int row, int column, hkbVariableBindingSet *varBind, const QString &path){
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
            CRITICAL_ERROR_MESSAGE("EventDrivenModifierUI::loadBinding(): The variable binding set is nullptr!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("EventDrivenModifierUI::loadBinding(): The data is nullptr!!");
    }
}

bool EventDrivenModifierUI::setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty){
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
                    CRITICAL_ERROR_MESSAGE("EventDrivenModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }else{
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    CRITICAL_ERROR_MESSAGE("EventDrivenModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->setIsFileChanged(true);
        }else{
            WARNING_MESSAGE("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\nYou are attempting to bind a variable of an invalid type for this data field!!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("EventDrivenModifierUI::setBinding(): The 'bsData' pointer is nullptr!!");
        return false;
    }
    return true;
}

void EventDrivenModifierUI::setBindingVariable(int index, const QString & name){
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
        case ACTIVE_BY_DEFAULT_ROW:
            if (table->item(ACTIVE_BY_DEFAULT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "activeByDefault", VARIABLE_TYPE_BOOL, isProperty);
            break;
        default:
            return;
        }
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("EventDrivenModifierUI::setBindingVariable(): The 'bsData' pointer is nullptr!!");
    }
}

void EventDrivenModifierUI::selectTableToView(bool viewproperties, const QString & path){
    if (bsData){
        if (viewproperties){
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
        CRITICAL_ERROR_MESSAGE("EventDrivenModifierUI::selectTableToView(): The data is nullptr!!");
    }
}

void EventDrivenModifierUI::viewSelected(int row, int column){
    if (bsData){
        int index = -1;
        bool isProperty = false;
        if (column == BINDING_COLUMN){
            switch (row){
            case ENABLE_ROW:
                if (table->item(ENABLE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "enable");
                break;
            case ACTIVE_BY_DEFAULT_ROW:
                if (table->item(ACTIVE_BY_DEFAULT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "activeByDefault");
                break;
            }
        }else if (row == MODIFIER_ROW && column == VALUE_COLUMN){
            emit viewModifiers(static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData->modifier) + 1, QString(), QStringList());
        }else if (column == VALUE_COLUMN){
            switch (row){
            case ACTIVATE_EVENT_ID_ROW:
                index = bsData->activateEventId;
                break;
            case DEACTIVATE_EVENT_ID_ROW:
                index = bsData->deactivateEventId;
                break;
            default:
                return;
            }
            emit viewEvents(index + 1, QString(), QStringList());
        }
    }else{
        CRITICAL_ERROR_MESSAGE("EventDrivenModifierUI::viewSelected(): The 'bsData' pointer is nullptr!!");
    }
}

void EventDrivenModifierUI::variableRenamed(const QString & name, int index){
    if (bsData){
        index--;
        hkbVariableBindingSet *bind = bsData->getVariableBindingSetData();
        if (bind){
            int bindIndex = bind->getVariableIndexOfBinding("enable");
            if (bindIndex == index){
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("activeByDefault");
            if (bindIndex == index){
                table->item(ACTIVE_BY_DEFAULT_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE("EventDrivenModifierUI::variableRenamed(): The 'bsData' pointer is nullptr!!");
    }
}

void EventDrivenModifierUI::eventRenamed(const QString & name, int index){
    if (bsData){
        if (index == bsData->activateEventId){
            table->item(ACTIVATE_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        }
        if (index == bsData->deactivateEventId){
            table->item(DEACTIVATE_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("EventDrivenModifierUI::eventRenamed(): The data is nullptr!!");
    }
}

void EventDrivenModifierUI::modifierRenamed(const QString &name, int index){
    if (bsData){
        index--;
        if (index == static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData->modifier)){
            table->item(MODIFIER_ROW, VALUE_COLUMN)->setText(name);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("EventDrivenModifierUI::generatorRenamed(): The 'bsData' pointer is nullptr!!");
    }
}

void EventDrivenModifierUI::setBehaviorView(BehaviorGraphView *view){
    behaviorView = view;
}
