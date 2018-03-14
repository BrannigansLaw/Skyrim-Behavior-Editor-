#include "evaluateexpressionmodifierui.h"

#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/modifiers/hkbevaluateexpressionmodifier.h"
#include "src/hkxclasses/behavior/hkbexpressiondataarray.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/ui/hkxclassesui/behaviorui/expressiondataarrayui.h"
#include "src/ui/genericdatawidgets.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/behaviorgraphview.h"
#include "src/ui/treegraphicsitem.h"
#include "src/ui/mainwindow.h"

#include <QGridLayout>

#include <QStackedLayout>
#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 3

#define NAME_ROW 0
#define ENABLE_ROW 1
#define ADD_EXPRESSION_ROW 2

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList EvaluateExpressionModifierUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

EvaluateExpressionModifierUI::EvaluateExpressionModifierUI()
    : bsData(nullptr),
      expressionUI(new ExpressionDataArrayUI),
      groupBox(new QGroupBox),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      enable(new CheckBox)
{
    groupBox->setTitle("hkbEvaluateExpressionModifier");
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
    table->setItem(ADD_EXPRESSION_ROW, NAME_COLUMN, new TableWidgetItem("Add Expression", Qt::AlignCenter, QColor(Qt::green), QBrush(Qt::black), "Double click to add a new expression"));
    table->setItem(ADD_EXPRESSION_ROW, BINDING_COLUMN, new TableWidgetItem("hkExpression", Qt::AlignCenter));
    table->setItem(ADD_EXPRESSION_ROW, BINDING_COLUMN, new TableWidgetItem("Remove Selected Expression", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black), "Double click to remove the selected expression"));
    table->setItem(ADD_EXPRESSION_ROW, VALUE_COLUMN, new TableWidgetItem("Edit Selected Expression", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black), "Double click to edit the selected expression"));
    topLyt->addWidget(table, 1, 0, 8, 3);
    groupBox->setLayout(topLyt);
    //Order here must correspond with the ACTIVE_WIDGET Enumerated type!!!
    addWidget(groupBox);
    addWidget(expressionUI);
    connectSignals();
}

void EvaluateExpressionModifierUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)), Qt::UniqueConnection);
    connect(expressionUI, SIGNAL(viewEvents(int)), this, SIGNAL(viewEvents(int)), Qt::UniqueConnection);
    connect(expressionUI, SIGNAL(viewVariables(int)), this, SIGNAL(viewVariables(int)), Qt::UniqueConnection);
    connect(expressionUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()), Qt::UniqueConnection);
}

void EvaluateExpressionModifierUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)));
    disconnect(expressionUI, SIGNAL(viewEvents(int)), this, SIGNAL(viewEvents(int)));
    disconnect(expressionUI, SIGNAL(viewVariables(int)), this, SIGNAL(viewVariables(int)));
    disconnect(expressionUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()));
}

void EvaluateExpressionModifierUI::addExpression(){
    hkbExpressionDataArray *exps = nullptr;
    if (bsData){
        exps = static_cast<hkbExpressionDataArray *>(bsData->expressions.data());
        if (!exps){
            exps = new hkbExpressionDataArray(bsData->getParentFile(), -1);
            bsData->expressions = HkxSharedPtr(exps);
        }
        exps->addExpression();
        bsData->getParentFile()->setIsChanged(true);
        loadDynamicTableRows();
    }else{
        FATAL_RUNTIME_ERROR("EvaluateExpressionModifierUI::addExpression(): The data is nullptr!!");
    }
}

void EvaluateExpressionModifierUI::removeExpression(int index){
    hkbExpressionDataArray *exps = nullptr;
    if (bsData){
        exps = static_cast<hkbExpressionDataArray *>(bsData->expressions.data());
        if (exps){
            if (index < exps->expressionsData.size() && index >= 0){
                exps->removeExpression(index);
            }else{
                WARNING_MESSAGE("EvaluateExpressionModifierUI::removeExpression(): Invalid row index selected!!");
                return;
            }
            bsData->getParentFile()->setIsChanged(true);
            loadDynamicTableRows();
        }else{
            WARNING_MESSAGE("EvaluateExpressionModifierUI::removeExpression(): Event data is nullptr!!");
            return;
        }
    }else{
        FATAL_RUNTIME_ERROR("EvaluateExpressionModifierUI::removeExpression(): The data is nullptr!!");
    }
}

void EvaluateExpressionModifierUI::loadData(HkxObject *data){
    disconnectSignals();
    setCurrentIndex(MAIN_WIDGET);
    hkbVariableBindingSet *varBind = nullptr;
    if (data){
        if (data->getSignature() == HKB_EVALUATE_EXPRESSION_MODIFIER){
            bsData = static_cast<hkbEvaluateExpressionModifier *>(data);
            name->setText(bsData->name);
            enable->setChecked(bsData->enable);
            varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (varBind){
                loadBinding(ENABLE_ROW, BINDING_COLUMN, varBind, "enable");
            }else{
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
            loadDynamicTableRows();
        }else{
            FATAL_RUNTIME_ERROR(QString("EvaluateExpressionModifierUI::loadData(): The data passed to the UI is the wrong type!\nSIGNATURE: "+QString::number(data->getSignature(), 16)).toLocal8Bit().data());
        }
    }else{
        FATAL_RUNTIME_ERROR("EvaluateExpressionModifierUI::loadData(): Attempting to load a null pointer!!");
    }
    connectSignals();
}

void EvaluateExpressionModifierUI::loadDynamicTableRows(){
    //table->setSortingEnabled(false);//Not sure...
    if (bsData){
        int temp = ADD_EXPRESSION_ROW + bsData->getNumberOfExpressions() + 1;
        if (table->rowCount() != temp){
            table->setRowCount(temp);
        }
        hkbExpressionDataArray *exps = static_cast<hkbExpressionDataArray *>(bsData->expressions.data());
        if (exps){
            for (int i = ADD_EXPRESSION_ROW + 1, j = 0; j < bsData->getNumberOfExpressions(); i++, j++){
                setRowItems(i, exps->expressionsData.at(j).expression, exps->getClassname(), "Remove", "Edit", "Double click to remove this expression", "Double click to edit this expression");
            }
        }
    }else{
        FATAL_RUNTIME_ERROR("EvaluateExpressionModifierUI::loadDynamicTableRows(): The data is nullptr!!");
    }
    //table->setSortingEnabled(true);
}

void EvaluateExpressionModifierUI::setRowItems(int row, const QString & name, const QString & classname, const QString & bind, const QString & value, const QString & tip1, const QString & tip2){
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

bool EvaluateExpressionModifierUI::setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty){
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
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY)){
                    FATAL_RUNTIME_ERROR("EvaluateExpressionModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }else{
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    FATAL_RUNTIME_ERROR("EvaluateExpressionModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->setIsChanged(true);
        }else{
            WARNING_MESSAGE("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("EvaluateExpressionModifierUI::setBinding(): The data is nullptr!!");
    }
    return true;
}

void EvaluateExpressionModifierUI::setBindingVariable(int index, const QString & name){
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
        bsData->getParentFile()->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("EvaluateExpressionModifierUI::setBindingVariable(): The data is nullptr!!");
    }
}

void EvaluateExpressionModifierUI::returnToWidget(){
    loadDynamicTableRows();
    setCurrentIndex(MAIN_WIDGET);
}

void EvaluateExpressionModifierUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            emit modifierNameChanged(bsData->name, static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData));
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("EvaluateExpressionModifierUI::setName(): The data is nullptr!!");
    }
}

void EvaluateExpressionModifierUI::setEnable(){
    if (bsData){
        bsData->enable = enable->isChecked();
        bsData->getParentFile()->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("EvaluateExpressionModifierUI::setEnable(): The data is nullptr!!");
    }
}

void EvaluateExpressionModifierUI::viewSelectedChild(int row, int column){
    int result = -1;
    bool properties = false;
    if (bsData){
        if (row < ADD_EXPRESSION_ROW && row >= 0){
            if (column == BINDING_COLUMN){
                switch (row){
                case ENABLE_ROW:
                    if (table->item(ENABLE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "enable");
                    break;
                }
            }
        }else if (row == ADD_EXPRESSION_ROW && column == NAME_COLUMN){
            addExpression();
        }else if (row > ADD_EXPRESSION_ROW && row < ADD_EXPRESSION_ROW + bsData->getNumberOfExpressions() + 1){
            result = row - BASE_NUMBER_OF_ROWS;
            if (bsData->getNumberOfExpressions() > result && result >= 0){
                if (column == VALUE_COLUMN){
                    expressionUI->loadData((BehaviorFile *)(bsData->expressions.data()->getParentFile()), &static_cast<hkbExpressionDataArray *>(bsData->expressions.data())->expressionsData[result]);
                    setCurrentIndex(CHILD_WIDGET);
                }else if (column == BINDING_COLUMN){
                    if (MainWindow::yesNoDialogue("Are you sure you want to remove the expression \""+table->item(row, NAME_COLUMN)->text()+"\"?") == QMessageBox::Yes){
                        removeExpression(result);
                    }
                }
            }else{
                FATAL_RUNTIME_ERROR("EvaluateExpressionModifierUI::viewSelectedChild(): Invalid index of child to view!!");
            }
        }
    }else{
        FATAL_RUNTIME_ERROR("EvaluateExpressionModifierUI::viewSelectedChild(): The data is nullptr!!");
    }
}

void EvaluateExpressionModifierUI::variableTableElementSelected(int index, const QString &name){
    switch (currentIndex()){
    case MAIN_WIDGET:
        setBindingVariable(index, name);
        break;
    case CHILD_WIDGET:
        expressionUI->setAssignmentVariableIndex(index, name);
        break;
    default:
        WARNING_MESSAGE("EvaluateExpressionModifierUI::variableTableElementSelected(): An unwanted element selected event was recieved!!");
    }
}

void EvaluateExpressionModifierUI::connectToTables(GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *events){
    if (variables && events && properties){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(events, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(variableTableElementSelected(int,QString)), Qt::UniqueConnection);
        connect(events, SIGNAL(elementSelected(int,QString)), expressionUI, SLOT(setAssignmentEventIndex(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(variableTableElementSelected(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewEvents(int)), events, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int)), variables, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int)), properties, SLOT(showTable(int)), Qt::UniqueConnection);
    }else{
        FATAL_RUNTIME_ERROR("EvaluateExpressionModifierUI::connectToTables(): One or more arguments are nullptr!!");
    }
}

void EvaluateExpressionModifierUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
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
            FATAL_RUNTIME_ERROR("EvaluateExpressionModifierUI::loadBinding(): The variable binding set is nullptr!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("EvaluateExpressionModifierUI::loadBinding(): The data is nullptr!!");
    }
}

void EvaluateExpressionModifierUI::selectTableToView(bool viewproperties, const QString & path){
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
        FATAL_RUNTIME_ERROR("EvaluateExpressionModifierUI::selectTableToView(): The data is nullptr!!");
    }
}

void EvaluateExpressionModifierUI::eventRenamed(const QString & name, int index){
    if (bsData){
        if (currentIndex() == CHILD_WIDGET){
            expressionUI->eventRenamed(name, index);
        }
    }else{
        FATAL_RUNTIME_ERROR("EvaluateExpressionModifierUI::eventRenamed(): The data is nullptr!!");
    }
}

void EvaluateExpressionModifierUI::variableRenamed(const QString & name, int index){
    int bindIndex = -1;
    hkbVariableBindingSet *bind = nullptr;
    if (name == ""){
        WARNING_MESSAGE("EvaluateExpressionModifierUI::variableRenamed(): The new variable name is the empty string!!");
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
            }
        }else{
            expressionUI->variableRenamed(name, index);
        }
    }else{
        FATAL_RUNTIME_ERROR("EvaluateExpressionModifierUI::variableRenamed(): The data is nullptr!!");
    }
}

