#include "eventsfromrangemodifierui.h"

#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/modifiers/hkbEventsFromRangeModifier.h"
#include "src/hkxclasses/behavior/hkbEventRangeDataArray.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/ui/hkxclassesui/behaviorui/eventrangedataui.h"
#include "src/ui/genericdatawidgets.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/behaviorgraphview.h"
#include "src/ui/treegraphicsitem.h"
#include "src/ui/mainwindow.h"

#include <QGridLayout>

#include <QStackedLayout>
#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 5

#define NAME_ROW 0
#define ENABLE_ROW 1
#define INPUT_VALUE_ROW 2
#define LOWER_BOUND_ROW 3
#define ADD_RANGE_ROW 4

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList EventsFromRangeModifierUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

EventsFromRangeModifierUI::EventsFromRangeModifierUI()
    : bsData(NULL),
      rangeUI(new EventRangeDataUI),
      groupBox(new QGroupBox),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      enable(new CheckBox),
      inputValue(new DoubleSpinBox),
      lowerBound(new DoubleSpinBox)
{
    groupBox->setTitle("hkbEventsFromRangeModifier");
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
    table->setItem(INPUT_VALUE_ROW, NAME_COLUMN, new TableWidgetItem("inputValue"));
    table->setItem(INPUT_VALUE_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(INPUT_VALUE_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(INPUT_VALUE_ROW, VALUE_COLUMN, inputValue);
    table->setItem(LOWER_BOUND_ROW, NAME_COLUMN, new TableWidgetItem("lowerBound"));
    table->setItem(LOWER_BOUND_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(LOWER_BOUND_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(LOWER_BOUND_ROW, VALUE_COLUMN, lowerBound);
    table->setItem(ADD_RANGE_ROW, NAME_COLUMN, new TableWidgetItem("Add Event Range", Qt::AlignCenter, QColor(Qt::green), QBrush(Qt::black), "Double click to add a new Event Range"));
    table->setItem(ADD_RANGE_ROW, TYPE_COLUMN, new TableWidgetItem("hkEventRangeData", Qt::AlignCenter));
    table->setItem(ADD_RANGE_ROW, BINDING_COLUMN, new TableWidgetItem("Remove Selected Event Range", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black), "Double click to remove the selected Event Range"));
    table->setItem(ADD_RANGE_ROW, VALUE_COLUMN, new TableWidgetItem("Edit Selected Event Range", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black), "Double click to edit the selected Event Range"));
    topLyt->addWidget(table, 1, 0, 8, 3);
    groupBox->setLayout(topLyt);
    //Order here must correspond with the ACTIVE_WIDGET Enumerated type!!!
    addWidget(groupBox);
    addWidget(rangeUI);
    connectSignals();
}

void EventsFromRangeModifierUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
    connect(inputValue, SIGNAL(editingFinished()), this, SLOT(setInputValue()), Qt::UniqueConnection);
    connect(lowerBound, SIGNAL(editingFinished()), this, SLOT(setLowerBound()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)), Qt::UniqueConnection);
    connect(rangeUI, SIGNAL(viewEvents(int)), this, SIGNAL(viewEvents(int)), Qt::UniqueConnection);
    //connect(rangeUI, SIGNAL(viewVariables(int)), this, SIGNAL(viewVariables(int)), Qt::UniqueConnection);
    //connect(rangeUI, SIGNAL(viewProperties(int)), this, SIGNAL(viewProperties(int)), Qt::UniqueConnection);
    connect(rangeUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()), Qt::UniqueConnection);
}

void EventsFromRangeModifierUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    disconnect(inputValue, SIGNAL(editingFinished()), this, SLOT(setInputValue()));
    disconnect(lowerBound, SIGNAL(editingFinished()), this, SLOT(setLowerBound()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)));
    disconnect(rangeUI, SIGNAL(viewEvents(int)), this, SIGNAL(viewEvents(int)));
    //disconnect(rangeUI, SIGNAL(viewVariables(int)), this, SIGNAL(viewVariables(int)));
    //disconnect(rangeUI, SIGNAL(viewProperties(int)), this, SIGNAL(viewProperties(int)));
    disconnect(rangeUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()));
}

void EventsFromRangeModifierUI::addRange(){
    hkbEventRangeDataArray *exps = NULL;
    if (bsData){
        exps = static_cast<hkbEventRangeDataArray *>(bsData->eventRanges.data());
        if (!exps){
            exps = new hkbEventRangeDataArray(bsData->getParentFile(), -1);
            bsData->eventRanges = HkxSharedPtr(exps);
        }
        exps->addEventData();
        bsData->getParentFile()->toggleChanged(true);
        loadDynamicTableRows();
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EventsFromRangeModifierUI::addExpression(): The data is NULL!!"));
    }
}

void EventsFromRangeModifierUI::removeRange(int index){
    hkbEventRangeDataArray *exps = NULL;
    if (bsData){
        exps = static_cast<hkbEventRangeDataArray *>(bsData->eventRanges.data());
        if (exps){
            if (index < exps->eventData.size() && index >= 0){
                exps->removeEventData(index);
            }else{
                WARNING_MESSAGE(QString("EventsFromRangeModifierUI::removeRange(): Invalid row index selected!!"));
                return;
            }
            bsData->getParentFile()->toggleChanged(true);
            loadDynamicTableRows();
        }else{
            WARNING_MESSAGE(QString("EventsFromRangeModifierUI::removeRange(): Event data is NULL!!"));
            return;
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EventsFromRangeModifierUI::removeRange(): The data is NULL!!"));
    }
}

void EventsFromRangeModifierUI::loadData(HkxObject *data){
    disconnectSignals();
    setCurrentIndex(MAIN_WIDGET);
    hkbVariableBindingSet *varBind = NULL;
    if (data){
        if (data->getSignature() == HKB_EVENTS_FROM_RANGE_MODIFIER){
            bsData = static_cast<hkbEventsFromRangeModifier *>(data);
            name->setText(bsData->name);
            enable->setChecked(bsData->enable);
            inputValue->setValue(bsData->inputValue);
            lowerBound->setValue(bsData->lowerBound);
            varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (varBind){
                loadBinding(ENABLE_ROW, BINDING_COLUMN, varBind, "enable");
                loadBinding(INPUT_VALUE_ROW, BINDING_COLUMN, varBind, "inputValue");
                loadBinding(LOWER_BOUND_ROW, BINDING_COLUMN, varBind, "lowerBound");
            }else{
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(INPUT_VALUE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(LOWER_BOUND_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
            loadDynamicTableRows();
        }else{
            CRITICAL_ERROR_MESSAGE(QString("EventsFromRangeModifierUI::loadData(): The data passed to the UI is the wrong type!\nSIGNATURE: "+QString::number(data->getSignature(), 16)))
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EventsFromRangeModifierUI::loadData(): Attempting to load a null pointer!!"))
    }
    connectSignals();
}

void EventsFromRangeModifierUI::loadDynamicTableRows(){
    //table->setSortingEnabled(false);//Not sure...
    if (bsData){
        int temp = ADD_RANGE_ROW + bsData->getNumberOfRanges() + 1;
        if (table->rowCount() != temp){
            table->setRowCount(temp);
        }
        hkbEventRangeDataArray *exps = static_cast<hkbEventRangeDataArray *>(bsData->eventRanges.data());
        if (exps){
            QString eventName;
            for (int i = ADD_RANGE_ROW + 1, j = 0; j < bsData->getNumberOfRanges(); i++, j++){
                eventName = static_cast<BehaviorFile *>(bsData->getParentFile())->getEventNameAt(exps->eventData.at(j).event.id);
                setRowItems(i, eventName, exps->getClassname(), "Remove", "Edit", "Double click to remove this Event Range", "Double click to edit this Event Range");
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EventsFromRangeModifierUI::loadDynamicTableRows(): The data is NULL!!"));
    }
    //table->setSortingEnabled(true);
}

void EventsFromRangeModifierUI::setRowItems(int row, const QString & name, const QString & classname, const QString & bind, const QString & value, const QString & tip1, const QString & tip2){
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

bool EventsFromRangeModifierUI::setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty){
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
                    CRITICAL_ERROR_MESSAGE(QString("EventsFromRangeModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }else{
                if (!varBind->addBinding(path, variableName, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    CRITICAL_ERROR_MESSAGE(QString("EventsFromRangeModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->toggleChanged(true);
        }else{
            WARNING_MESSAGE(QString("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EventsFromRangeModifierUI::setBinding(): The data is NULL!!"));
    }
    return true;
}

void EventsFromRangeModifierUI::setBindingVariable(int index, const QString & name){
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
        case INPUT_VALUE_ROW:
            if (table->item(INPUT_VALUE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "inputValue", VARIABLE_TYPE_REAL, isProperty);
            break;
        case LOWER_BOUND_ROW:
            if (table->item(LOWER_BOUND_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "lowerBound", VARIABLE_TYPE_REAL, isProperty);
            break;
        default:
            return;
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EventsFromRangeModifierUI::setBindingVariable(): The data is NULL!!"))
    }
}

void EventsFromRangeModifierUI::returnToWidget(){
    loadDynamicTableRows();
    setCurrentIndex(MAIN_WIDGET);
}

void EventsFromRangeModifierUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            emit modifierNameChanged(bsData->name, static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData));
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EventsFromRangeModifierUI::setName(): The data is NULL!!"))
    }
}

void EventsFromRangeModifierUI::setEnable(){
    if (bsData){
        bsData->enable = enable->isChecked();
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EventsFromRangeModifierUI::setEnable(): The data is NULL!!"));
    }
}

void EventsFromRangeModifierUI::setInputValue(){
    if (bsData){
        if (bsData->inputValue != inputValue->value()){
            bsData->inputValue = inputValue->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EventsFromRangeModifierUI::setInputValue(): The data is NULL!!"));
    }
}

void EventsFromRangeModifierUI::setLowerBound(){
    if (bsData){
        if (bsData->lowerBound != lowerBound->value()){
            bsData->lowerBound = lowerBound->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EventsFromRangeModifierUI::setLowerBound(): The data is NULL!!"));
    }
}


void EventsFromRangeModifierUI::viewSelectedChild(int row, int column){
    int result = -1;
    bool properties = false;
    if (bsData){
        if (row < ADD_RANGE_ROW && row >= 0){
            if (column == BINDING_COLUMN){
                switch (row){
                case ENABLE_ROW:
                    if (table->item(ENABLE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "enable");
                    break;
                case INPUT_VALUE_ROW:
                    if (table->item(INPUT_VALUE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "inputValue");
                    break;
                case LOWER_BOUND_ROW:
                    if (table->item(LOWER_BOUND_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "lowerBound");
                    break;
                }
            }
        }else if (row == ADD_RANGE_ROW && column == NAME_COLUMN){
            addRange();
        }else if (row > ADD_RANGE_ROW && row < ADD_RANGE_ROW + bsData->getNumberOfRanges() + 1){
            result = row - BASE_NUMBER_OF_ROWS;
            if (bsData->getNumberOfRanges() > result && result >= 0){
                if (column == VALUE_COLUMN){
                    rangeUI->loadData((BehaviorFile *)(bsData->eventRanges.data()->getParentFile()), static_cast<hkbEventRangeDataArray::hkbEventRangeData *>(&static_cast<hkbEventRangeDataArray *>(bsData->eventRanges.data())->eventData[result]), static_cast<hkbEventRangeDataArray *>(bsData->eventRanges.data()), result);
                    setCurrentIndex(CHILD_WIDGET);
                }else if (column == BINDING_COLUMN){
                    if (MainWindow::yesNoDialogue("Are you sure you want to remove the expression \""+table->item(row, NAME_COLUMN)->text()+"\"?") == QMessageBox::Yes){
                        removeRange(result);
                    }
                }
            }else{
                CRITICAL_ERROR_MESSAGE(QString("EventsFromRangeModifierUI::viewSelectedChild(): Invalid index of child to view!!"))
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EventsFromRangeModifierUI::viewSelectedChild(): The data is NULL!!"))
    }
}

void EventsFromRangeModifierUI::variableTableElementSelected(int index, const QString &name){
    switch (currentIndex()){
    case MAIN_WIDGET:
        setBindingVariable(index, name);
        break;
    /*case CHILD_WIDGET:
        rangeUI->setAssignmentVariableIndex(index, name);
        break;*/
    default:
        WARNING_MESSAGE(QString("EventsFromRangeModifierUI::variableTableElementSelected(): An unwanted element selected event was recieved!!"));;
    }
}

void EventsFromRangeModifierUI::connectToTables(GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *events){
    if (variables && events && properties){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(events, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(variableTableElementSelected(int,QString)), Qt::UniqueConnection);
        connect(events, SIGNAL(elementSelected(int,QString)), rangeUI, SLOT(setEventId(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(variableTableElementSelected(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewEvents(int)), events, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int)), variables, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int)), properties, SLOT(showTable(int)), Qt::UniqueConnection);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EventsFromRangeModifierUI::connectToTables(): One or more arguments are NULL!!"))
    }
}

void EventsFromRangeModifierUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
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
            CRITICAL_ERROR_MESSAGE(QString("EventsFromRangeModifierUI::loadBinding(): The variable binding set is NULL!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EventsFromRangeModifierUI::loadBinding(): The data is NULL!!"));
    }
}

void EventsFromRangeModifierUI::selectTableToView(bool viewproperties, const QString & path){
    if (bsData){
        if (viewproperties){
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
        CRITICAL_ERROR_MESSAGE(QString("EventsFromRangeModifierUI::selectTableToView(): The data is NULL!!"));
    }
}

void EventsFromRangeModifierUI::eventRenamed(const QString & name, int index){
    if (bsData){
        rangeUI->eventRenamed(name, index);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EventsFromRangeModifierUI::eventRenamed(): The data is NULL!!"));
    }
}

void EventsFromRangeModifierUI::variableRenamed(const QString & name, int index){
    int bindIndex = -1;
    hkbVariableBindingSet *bind = NULL;
    if (name == ""){
        WARNING_MESSAGE(QString("EventsFromRangeModifierUI::variableRenamed(): The new variable name is the empty string!!"))
    }
    if (bsData){
        index--;
        if (currentIndex() == MAIN_WIDGET){
            bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (bind){
                bindIndex = bind->getVariableIndexOfBinding("enable");
                if (bindIndex == index){
                    table->item(ENABLE_ROW, BINDING_COLUMN)->setText(name);
                }
                bindIndex = bind->getVariableIndexOfBinding("inputValue");
                if (bindIndex == index){
                    table->item(INPUT_VALUE_ROW, BINDING_COLUMN)->setText(name);
                }
                bindIndex = bind->getVariableIndexOfBinding("lowerBound");
                if (bindIndex == index){
                    table->item(LOWER_BOUND_ROW, BINDING_COLUMN)->setText(name);
                }
            }
        }else{
            //rangeUI->variableRenamed(name, index);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EventsFromRangeModifierUI::variableRenamed(): The data is NULL!!"))
    }
}

