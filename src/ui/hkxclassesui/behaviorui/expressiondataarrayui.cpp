#include "expressiondataarrayui.h"

#include "src/utility.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/behavior/hkbexpressiondataarray.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/ui/genericdatawidgets.h"

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

#define BASE_NUMBER_OF_ROWS 4

#define EXPRESSION_ROW 0
#define ASSIGNMENT_VARIABLE_INDEX_ROW 1
#define ASSIGNMENT_EVENT_INDEX_ROW 2
#define EVENT_MODE_ROW 3

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define VALUE_COLUMN 2

QStringList ExpressionDataArrayUI::EventModeUI = {
    "EVENT_MODE_SEND_ONCE",
    "EVENT_MODE_SEND_ON_TRUE",
    "EVENT_MODE_SEND_ON_FALSE_TO_TRUE",
    "EVENT_MODE_SEND_EVERY_FRAME_ONCE_TRUE"
};

QStringList ExpressionDataArrayUI::headerLabels = {
    "Name",
    "Type",
    "Value"
};

ExpressionDataArrayUI::ExpressionDataArrayUI()
    : file(nullptr),
      topLyt(new QGridLayout),
      returnPB(new QPushButton("Return")),
      table(new TableWidget),
      expression(new QLineEdit),
      eventMode(new ComboBox)
{
    setTitle("Expression Editor");
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(EXPRESSION_ROW, NAME_COLUMN, new TableWidgetItem("expression"));
    table->setItem(EXPRESSION_ROW, TYPE_COLUMN, new TableWidgetItem("hkStringPtr", Qt::AlignCenter));
    table->setCellWidget(EXPRESSION_ROW, VALUE_COLUMN, expression);
    table->setItem(ASSIGNMENT_VARIABLE_INDEX_ROW, NAME_COLUMN, new TableWidgetItem("assignmentVariableIndex"));
    table->setItem(ASSIGNMENT_VARIABLE_INDEX_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(ASSIGNMENT_VARIABLE_INDEX_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP));
    table->setItem(ASSIGNMENT_EVENT_INDEX_ROW, NAME_COLUMN, new TableWidgetItem("assignmentEventIndex"));
    table->setItem(ASSIGNMENT_EVENT_INDEX_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(ASSIGNMENT_EVENT_INDEX_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_EVENTS_TABLE_TIP));
    table->setItem(EVENT_MODE_ROW, NAME_COLUMN, new TableWidgetItem("eventMode"));
    table->setItem(EVENT_MODE_ROW, TYPE_COLUMN, new TableWidgetItem("EventMode", Qt::AlignCenter));
    table->setCellWidget(EVENT_MODE_ROW, VALUE_COLUMN, eventMode);
    topLyt->addWidget(returnPB, 0, 1, 1, 1);
    topLyt->addWidget(table, 1, 0, 6, 2);
    setLayout(topLyt);
    connectSignals();
}

void ExpressionDataArrayUI::connectSignals(){
    connect(returnPB, SIGNAL(released()), this, SIGNAL(returnToParent()), Qt::UniqueConnection);
    connect(expression, SIGNAL(editingFinished()), this, SLOT(setExpression()), Qt::UniqueConnection);
    connect(eventMode, SIGNAL(currentIndexChanged(int)), this, SLOT(setEventMode(int)), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)), Qt::UniqueConnection);
}

void ExpressionDataArrayUI::disconnectSignals(){
    disconnect(returnPB, SIGNAL(released()), this, SIGNAL(returnToParent()));
    disconnect(expression, SIGNAL(editingFinished()), this, SLOT(setExpression()));
    disconnect(eventMode, SIGNAL(currentIndexChanged(int)), this, SLOT(setEventMode(int)));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)));
}

void ExpressionDataArrayUI::loadData(BehaviorFile *parentFile, hkbExpressionDataArray::hkExpression *exp){
    disconnectSignals();
    QString text;
    if (parentFile && expression){
        file = parentFile;
        bsData = exp;
        expression->setText(bsData->expression);
        QString varName = file->getVariableNameAt(bsData->assignmentVariableIndex);
        if (varName != ""){
            table->item(ASSIGNMENT_VARIABLE_INDEX_ROW, VALUE_COLUMN)->setText(varName);
        }else{
            table->item(ASSIGNMENT_VARIABLE_INDEX_ROW, VALUE_COLUMN)->setText("NONE");
        }
        text = file->getEventNameAt(bsData->assignmentEventIndex);
        if (text != ""){
            table->item(ASSIGNMENT_EVENT_INDEX_ROW, VALUE_COLUMN)->setText(text);
        }else{
            table->item(ASSIGNMENT_EVENT_INDEX_ROW, VALUE_COLUMN)->setText("NONE");
        }
        if (eventMode->count() == 0){
            eventMode->insertItems(0, EventModeUI);
        }
        eventMode->setCurrentIndex(EventModeUI.indexOf(bsData->eventMode));
    }else{
        FATAL_RUNTIME_ERROR("ExpressionDataArrayUI::loadData(): Behavior file or event data is null!!!");
    }
    connectSignals();
}

void ExpressionDataArrayUI::setExpression(){
    if (bsData && file){
        if (bsData->expression != expression->text()){
            bsData->expression = expression->text();
            file->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("ExpressionDataArrayUI::setExpression(): Behavior file or event data is null!!!");
    }
}

void ExpressionDataArrayUI::setEventMode(int index){
    if (bsData){
        bsData->eventMode = EventModeUI.at(index);
        file->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("ExpressionDataArrayUI::setEventMode(): The data is nullptr!!");
    }
}

void ExpressionDataArrayUI::setAssignmentVariableIndex(int index, const QString & name){
    if (bsData && file){
        index--;
        if (bsData->assignmentVariableIndex != index){
            bsData->assignmentVariableIndex = index;
            table->item(ASSIGNMENT_VARIABLE_INDEX_ROW, VALUE_COLUMN)->setText(name);
            file->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("ExpressionDataArrayUI::setAssignmentVariableIndex(): Behavior file or event data is null!!!");
    }
}

void ExpressionDataArrayUI::setAssignmentEventIndex(int index, const QString & name){
    if (bsData && file){
        index--;
        if (bsData->assignmentEventIndex != index){
            bsData->assignmentEventIndex = index;
            table->item(ASSIGNMENT_EVENT_INDEX_ROW, VALUE_COLUMN)->setText(name);
            file->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("ExpressionDataArrayUI::setAssignmentVariableIndex(): Behavior file or event data is null!!!");
    }
}

void ExpressionDataArrayUI::viewSelectedChild(int row, int column){
    if (bsData){
        if (row == ASSIGNMENT_VARIABLE_INDEX_ROW && column == VALUE_COLUMN){
            emit viewVariables(bsData->assignmentVariableIndex + 1, QString(), QStringList());
        }else if (row == ASSIGNMENT_EVENT_INDEX_ROW && column == VALUE_COLUMN){
            emit viewEvents(bsData->assignmentEventIndex + 1, QString(), QStringList());
        }
    }else{
        FATAL_RUNTIME_ERROR("ExpressionDataArrayUI::viewSelectedChild(): The data is nullptr!!");
    }
}

void ExpressionDataArrayUI::eventRenamed(const QString & name, int index){
    if (bsData){
        index--;
        if (index == bsData->assignmentEventIndex){
            table->item(ASSIGNMENT_EVENT_INDEX_ROW, VALUE_COLUMN)->setText(name);
        }
    }else{
        FATAL_RUNTIME_ERROR("ExpressionDataArrayUI::eventRenamed(): The data is nullptr!!");
    }
}

void ExpressionDataArrayUI::variableRenamed(const QString & name, int index){
    if (bsData){
        if (index == bsData->assignmentVariableIndex){
            table->item(ASSIGNMENT_VARIABLE_INDEX_ROW, VALUE_COLUMN)->setText(name);
        }
    }else{
        FATAL_RUNTIME_ERROR("ExpressionDataArrayUI::variableRenamed(): The data is nullptr!!");
    }
}
