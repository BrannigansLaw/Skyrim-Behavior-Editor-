#include "behaviorvariablesui.h"

#include "src/ui/behaviorgraphui/behaviorgraphicons.h"
#include "src/ui/genericdatawidgets.h"
#include "src/hkxclasses/generators/hkbgenerator.h"
#include "src/filetypes/hkxfile.h"
#include "src/ui/behaviorgraphui/behaviorgraphui.h"
#include "src/hkxclasses/generators/bsistatetagginggenerator.h"
#include "src/hkxclasses/generators/hkbmodifiergenerator.h"
#include "src/ui/objectdataui/generators/bsistatetagginggeneratorui.h"
#include "src/ui/objectdataui/generators/modifiergeneratorui.h"
#include "src/ui/objectdataui/generators/manualselectorgeneratorui.h"

#include "src/hkxclasses/hkbbehaviorgraphdata.h"
#include "src/hkxclasses/hkbbehaviorgraphstringdata.h"
#include "src/hkxclasses/hkbvariablevalueset.h"

#include <QPushButton>
#include <QMessageBox>
#include <QSignalMapper>
#include <QTableWidget>
#include <QHeaderView>
#include <QComboBox>

QStringList BehaviorVariablesUI::types = {
    "VARIABLE_TYPE_BOOL",
    "VARIABLE_TYPE_INT32",
    "VARIABLE_TYPE_REAL",
    "VARIABLE_TYPE_POINTER",
    "VARIABLE_TYPE_VECTOR4",
    "VARIABLE_TYPE_QUATERNION"
};

QStringList BehaviorVariablesUI::headerLabels = {
    "Name",
    "Type",
    "Value"
};

BehaviorVariablesUI::BehaviorVariablesUI(const QString &title)
    : dataUI(NULL),
      verLyt(new QVBoxLayout),
      loadedData(NULL),
      table(new QTableWidget),
      addObjectPB(new QPushButton("Add Variable")),
      removeObjectPB(new QPushButton("Remove Selected Variable")),
      buttonLyt(new QHBoxLayout),
      valueMapper(new QSignalMapper),
      nameMapper(new QSignalMapper),
      typeSelector(new QComboBox)
{
    setTitle(title);
    buttonLyt->addWidget(addObjectPB, 1);
    buttonLyt->addWidget(typeSelector, 2);
    buttonLyt->addWidget(removeObjectPB, 1);
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels(headerLabels);
    verLyt->addLayout(buttonLyt, 1);
    verLyt->addWidget(table, 10);
    typeSelector->insertItems(0, types);
    table->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    setLayout(verLyt);
    connect(addObjectPB, SIGNAL(pressed()), this, SLOT(addVariable()));
    connect(removeObjectPB, SIGNAL(pressed()), this, SLOT(removeVariable()));
    connect(nameMapper, SIGNAL(mapped(int)), this, SLOT(renameSelectedVariable(int)));
    connect(valueMapper, SIGNAL(mapped(int)), this, SLOT(setVariableValue(int)));
}

void BehaviorVariablesUI::setVariableValue(int index){
    QString type = table->item(index, 1)->text();
    if (type == "VARIABLE_TYPE_BOOL"){
        loadedData->setWordVariableValueAt(index, qobject_cast<QCheckBox *>(table->cellWidget(index, 2))->isChecked());
    }else if (type == "VARIABLE_TYPE_INT32"){
        loadedData->setWordVariableValueAt(index, qobject_cast<QSpinBox *>(table->cellWidget(index, 2))->value());
    }else if (type == "VARIABLE_TYPE_REAL"){
        loadedData->setWordVariableValueAt(index, qobject_cast<QDoubleSpinBox *>(table->cellWidget(index, 2))->value());
    }else if (type == "VARIABLE_TYPE_POINTER"){
        loadedData->setWordVariableValueAt(index, qobject_cast<QSpinBox *>(table->cellWidget(index, 2))->value());
    }else if (type == "VARIABLE_TYPE_VECTOR4"){
        loadedData->setQuadVariableValueAt(index, qobject_cast<QuadVariableWidget *>(table->cellWidget(index, 2))->getValue());
    }else if (type == "VARIABLE_TYPE_QUATERNION"){
        loadedData->setQuadVariableValueAt(index, qobject_cast<QuadVariableWidget *>(table->cellWidget(index, 2))->getValue());
    }
}

void BehaviorVariablesUI::renameSelectedVariable(int index){
    QString newName = qobject_cast<QLineEdit *>(table->cellWidget(index, 0))->text();
    loadedData->setVariableNameAt(index, newName);
    emit variableNameChanged(newName, index);
}

template <typename T, typename W>
void BehaviorVariablesUI::addVariableToTable(const QString & name, const QString & type, const T & value, W *widget){
    int row = table->rowCount();
    QLineEdit *lineEdit = new QLineEdit(name);
    table->setRowCount(row + 1);
    table->setCellWidget(row, 0, lineEdit);
    table->setItem(row, 1, new QTableWidgetItem(type));
    widget->setValue(value);
    table->setCellWidget(row, 2, widget);
    connect(widget, SIGNAL(editingFinished()), valueMapper, SLOT(map()));
    connect(lineEdit, SIGNAL(editingFinished()), nameMapper, SLOT(map()));
    valueMapper->setMapping(widget, row);
    nameMapper->setMapping(lineEdit, row);
}

template <typename T>
void BehaviorVariablesUI::addVariableToTable(const QString & name, const QString & type, const T & value, QCheckBox *widget){
    int row = table->rowCount();
    QLineEdit *lineEdit = new QLineEdit(name);
    table->setRowCount(row + 1);
    table->setCellWidget(row, 0, lineEdit);
    table->setItem(row, 1, new QTableWidgetItem(type));
    widget->setChecked(value);
    table->setCellWidget(row, 2, widget);
    connect(widget, SIGNAL(clicked(bool)), valueMapper, SLOT(map()));
    connect(lineEdit, SIGNAL(editingFinished()), nameMapper, SLOT(map()));
    valueMapper->setMapping(widget, row);
    nameMapper->setMapping(lineEdit, row);
}

void BehaviorVariablesUI::removeVariableFromTable(int row){
    if (row < table->rowCount()){
        table->removeRow(row);
        for (int i = row; i < table->rowCount(); i++){
            valueMapper->setMapping(table->cellWidget(i, 2), i);
            nameMapper->setMapping(table->cellWidget(i, 0), i);
        }
    }
}

void BehaviorVariablesUI::loadData(HkxObject *data){
    if (data && data->getSignature() == HKB_BEHAVIOR_GRAPH_DATA){
        bool ok;
        loadedData = static_cast<hkbBehaviorGraphData *>(data);
        hkbBehaviorGraphStringData *vars = static_cast<hkbBehaviorGraphStringData *>(loadedData->stringData.data());
        hkbVariableValueSet *varValues = static_cast<hkbVariableValueSet *>(loadedData->variableInitialValues.data());
        table->setRowCount(0);
        QString type;
        QSpinBox *spinBox;
        QDoubleSpinBox *doubleBox;
        for (int i = 0; i < vars->variableNames.size(); i++){
            type = loadedData->variableInfos.at(i).type;
            if (type == "VARIABLE_TYPE_BOOL"){
                addVariableToTable(vars->variableNames.at(i), "VARIABLE_TYPE_BOOL", varValues->wordVariableValues.at(i), new QCheckBox);
            }else if (type == "VARIABLE_TYPE_INT32"){
                spinBox = new QSpinBox;
                addVariableToTable(vars->variableNames.at(i), "VARIABLE_TYPE_INT32", varValues->wordVariableValues.at(i), spinBox);
            }else if (type == "VARIABLE_TYPE_REAL"){
                doubleBox = new QDoubleSpinBox;
                doubleBox->setMaximum(std::numeric_limits<double>::max());
                doubleBox->setMinimum(std::numeric_limits<double>::min());
                addVariableToTable(vars->variableNames.at(i), "VARIABLE_TYPE_REAL", varValues->wordVariableValues.at(i), doubleBox);
            }else if (type == "VARIABLE_TYPE_POINTER"){
                addVariableToTable(vars->variableNames.at(i), "VARIABLE_TYPE_POINTER", varValues->wordVariableValues.at(i), new QSpinBox);
            }else if (type == "VARIABLE_TYPE_VECTOR4"){
                addVariableToTable(vars->variableNames.at(i), "VARIABLE_TYPE_VECTOR4", loadedData->getQuadVariable(i, &ok), new QuadVariableWidget);
            }else if (type == "VARIABLE_TYPE_QUATERNION"){
                addVariableToTable(vars->variableNames.at(i), "VARIABLE_TYPE_QUATERNION", loadedData->getQuadVariable(i, &ok), new QuadVariableWidget);
            }
        }
    }
}

void BehaviorVariablesUI::addVariable(){
    int type = typeSelector->currentIndex();
    hkVariableType varType;
    hkbBehaviorGraphStringData *vars = static_cast<hkbBehaviorGraphStringData *>(loadedData->stringData.data());
    switch (type){
    case VARIABLE_TYPE_BOOL:
        varType = hkVariableType::VARIABLE_TYPE_BOOL;
        loadedData->addVariable(varType);
        addVariableToTable(vars->variableNames.last(), "VARIABLE_TYPE_BOOL", false, new QCheckBox);
        break;
    case VARIABLE_TYPE_INT32:
        varType = hkVariableType::VARIABLE_TYPE_INT32;
        loadedData->addVariable(varType);
        addVariableToTable(vars->variableNames.last(), "VARIABLE_TYPE_INT32", 0, new QSpinBox);
        break;
    case VARIABLE_TYPE_REAL:
        varType = hkVariableType::VARIABLE_TYPE_REAL;
        loadedData->addVariable(varType);
        addVariableToTable(vars->variableNames.last(), "VARIABLE_TYPE_REAL", 0, new QDoubleSpinBox);
        break;
    case VARIABLE_TYPE_POINTER:
        varType = hkVariableType::VARIABLE_TYPE_POINTER;
        loadedData->addVariable(varType);
        addVariableToTable(vars->variableNames.last(), "VARIABLE_TYPE_POINTER", 0, new QSpinBox);
        break;
    case VARIABLE_TYPE_VECTOR4:
        varType = hkVariableType::VARIABLE_TYPE_VECTOR4;
        loadedData->addVariable(varType);
        addVariableToTable(vars->variableNames.last(), "VARIABLE_TYPE_VECTOR4", hkQuadVariable(), new QuadVariableWidget);
        break;
    case VARIABLE_TYPE_QUATERNION:
        varType = hkVariableType::VARIABLE_TYPE_QUATERNION;
        loadedData->addVariable(varType);
        addVariableToTable(vars->variableNames.last(), "VARIABLE_TYPE_QUATERNION", hkQuadVariable(), new QuadVariableWidget);
        break;
    default:
        return;
    }
    emit variableAdded(vars->variableNames.last());
}

void BehaviorVariablesUI::removeVariable(){
    int index = table->currentRow();
    loadedData->removeVariable(index);
    removeVariableFromTable(index);
    emit variableRemoved(index);
}

void BehaviorVariablesUI::setHkDataUI(HkDataUI *ui){
    dataUI = ui;
}
