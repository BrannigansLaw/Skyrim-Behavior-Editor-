#include "behaviorvariablesui.h"

#include "src/ui/treegraphicsitem.h"
#include "src/ui/hkxclassesui/behaviorui/boneweightarrayui.h"
#include "src/ui/genericdatawidgets.h"
#include "src/hkxclasses/behavior/generators/hkbgenerator.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/behaviorgraphview.h"
#include "src/hkxclasses/behavior/generators/bsistatetagginggenerator.h"
#include "src/hkxclasses/behavior/generators/hkbmodifiergenerator.h"
#include "src/ui/hkxclassesui/behaviorui/generators/bsistatetagginggeneratorui.h"
#include "src/ui/hkxclassesui/behaviorui/generators/modifiergeneratorui.h"
#include "src/ui/hkxclassesui/behaviorui/generators/manualselectorgeneratorui.h"

#include "src/hkxclasses/behavior/hkbbehaviorgraphdata.h"
#include "src/hkxclasses/behavior/hkbbehaviorgraphstringdata.h"
#include "src/hkxclasses/behavior/hkbvariablevalueset.h"

#include <QPushButton>
#include <QMessageBox>
#include <QSignalMapper>
#include <QTableWidget>
#include <QHeaderView>
#include <QComboBox>

#include "src/ui/genericdatawidgets.h"
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QStackedLayout>

QStringList BehaviorVariablesUI::types = {
    "BOOL",
    "INT32",
    "REAL",
    "POINTER",
    "VECTOR4",
    "QUATERNION"
};

QStringList BehaviorVariablesUI::headerLabels = {
    "Name",
    "Type",
    "Value"
};

BehaviorVariablesUI::BehaviorVariablesUI(const QString &title)
    : dataUI(nullptr),
      verLyt(new QVBoxLayout),
      loadedData(nullptr),
      table(new TableWidget),
      addObjectPB(new QPushButton("Add Variable")),
      removeObjectPB(new QPushButton("Remove Selected Variable")),
      buttonLyt(new QHBoxLayout),
      boolName(new LineEdit),
      intName(new LineEdit),
      doubleName(new LineEdit),
      quadName(new LineEdit),
      boolCB(new CheckBox),
      intSB(new SpinBox),
      doubleSB(new DoubleSpinBox),
      quadWidget(new QuadVariableWidget),
      variableWidget(new TableWidget),
      boneWeightArrayWidget(new BoneWeightArrayUI()),
      stackLyt(new QStackedLayout),
      returnBoolPB(new QPushButton("Return To Parent")),
      returnIntPB(new QPushButton("Return To Parent")),
      returnDoublePB(new QPushButton("Return To Parent")),
      returnQuadPB(new QPushButton("Return To Parent")),
      typeSelector(new ComboBox),
      nameMapper(new QSignalMapper),
      valueMapper(new QSignalMapper)
{
    setTitle(title);
    //setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    stackLyt->addWidget(table);
    stackLyt->addWidget(variableWidget);
    stackLyt->addWidget(boneWeightArrayWidget);
    stackLyt->setCurrentIndex(TABLE_WIDGET);
    typeSelector->insertItems(0, types);
    buttonLyt->addWidget(addObjectPB, 1);
    buttonLyt->addWidget(typeSelector, 2);
    buttonLyt->addWidget(removeObjectPB, 1);
    //table->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels(headerLabels);
    verLyt->addLayout(buttonLyt, 1);
    verLyt->addLayout(stackLyt, 10);
    variableWidget->setRowCount(4);
    variableWidget->setColumnCount(3);
    variableWidget->setCellWidget(0, 0, boolName);
    variableWidget->setCellWidget(0, 1, boolCB);
    variableWidget->setCellWidget(0, 2, returnBoolPB);
    variableWidget->setCellWidget(1, 0, intName);
    variableWidget->setCellWidget(1, 1, intSB);
    variableWidget->setCellWidget(1, 2, returnIntPB);
    variableWidget->setCellWidget(2, 0, doubleName);
    variableWidget->setCellWidget(2, 1, doubleSB);
    variableWidget->setCellWidget(2, 2, returnDoublePB);
    variableWidget->setCellWidget(3, 0, quadName);
    variableWidget->setCellWidget(3, 1, quadWidget);
    variableWidget->setCellWidget(3, 2, returnQuadPB);
    nameMapper->setMapping(boolName, 0);
    nameMapper->setMapping(intName, 1);
    nameMapper->setMapping(doubleName, 2);
    nameMapper->setMapping(quadName, 3);
    connect(boolName, SIGNAL(editingFinished()), nameMapper, SLOT(map()));
    connect(intName, SIGNAL(editingFinished()), nameMapper, SLOT(map()));
    connect(doubleName, SIGNAL(editingFinished()), nameMapper, SLOT(map()));
    connect(quadName, SIGNAL(editingFinished()), nameMapper, SLOT(map()));
    valueMapper->setMapping(boolCB, 0);
    valueMapper->setMapping(intSB, 1);
    valueMapper->setMapping(doubleSB, 2);
    valueMapper->setMapping(quadWidget, 3);
    connect(boolCB, SIGNAL(released()), valueMapper, SLOT(map()));
    connect(intSB, SIGNAL(editingFinished()), valueMapper, SLOT(map()));
    connect(doubleSB, SIGNAL(editingFinished()), valueMapper, SLOT(map()));
    connect(quadWidget, SIGNAL(editingFinished()), valueMapper, SLOT(map()));
    setLayout(verLyt);
    connect(removeObjectPB, SIGNAL(pressed()), this, SLOT(removeVariable()));
    connect(addObjectPB, SIGNAL(pressed()), this, SLOT(addVariable()));
    connect(nameMapper, SIGNAL(mapped(int)), this, SLOT(renameSelectedVariable(int)));
    connect(valueMapper, SIGNAL(mapped(int)), this, SLOT(setVariableValue(int)));
    connect(table, SIGNAL(cellClicked(int,int)), this, SLOT(viewVariable(int,int)));
    connect(returnBoolPB, SIGNAL(released()), this, SLOT(returnToTable()));
    connect(returnIntPB, SIGNAL(released()), this, SLOT(returnToTable()));
    connect(returnDoublePB, SIGNAL(released()), this, SLOT(returnToTable()));
    connect(returnQuadPB, SIGNAL(released()), this, SLOT(returnToTable()));
    connect(boneWeightArrayWidget, SIGNAL(returnToParent()), this, SLOT(returnToTable()));
}

/*QSize BehaviorVariablesUI::sizeHint() const{
    return QSize(300, 300);
}*/

void BehaviorVariablesUI::setVariableValue(int type){
    if (loadedData){
        int index = table->currentRow();
        if (type == 0){
            loadedData->setWordVariableValueAt(index, boolCB->isChecked());
        }else if (type == 1){
            loadedData->setWordVariableValueAt(index, intSB->value());
        }else if (type == 2){
            loadedData->setWordVariableValueAt(index, doubleSB->value());
        }/*else if (type == "POINTER"){
            loadedData->setWordVariableValueAt(index, intSB->value());
        }*/else if (type == 3){
            loadedData->setQuadVariableValueAt(index, quadWidget->value());
        }
        loadedData->getParentFile()->setIsChanged(true);
    }
}

void BehaviorVariablesUI::renameSelectedVariable(int type){
    if (loadedData){
        QString newName;
        if (type == 0){
            newName = boolName->text();
        }else if (type == 1){
            newName = intName->text();
        }else if (type == 2){
            newName = doubleName->text();
        }/*else if (type == "POINTER"){
            loadedData->setWordVariableValueAt(index, intSB->value());
        }*/else if (type == 3){
            newName = quadName->text();
        }
        table->item(table->currentRow(), 0)->setText(newName);
        loadedData->setVariableNameAt(table->currentRow(), newName);
        loadedData->getParentFile()->setIsChanged(true);
        emit variableNameChanged(newName, table->currentRow());
    }
}

void BehaviorVariablesUI::removeVariableFromTable(int row){
    if (row < table->rowCount()){
        table->removeRow(row);
    }
}

void BehaviorVariablesUI::loadVariable(CheckBox *variableWid){
    if (loadedData){
        disconnect(variableWid, 0, this, 0);
        hkbBehaviorGraphStringData *varNames = static_cast<hkbBehaviorGraphStringData *>(loadedData->stringData.data());
        hkbVariableValueSet *varValues = static_cast<hkbVariableValueSet *>(loadedData->variableInitialValues.data());
        int index = table->currentRow();
        if (varValues->wordVariableValues.size() > index && varNames->variableNames.size() > index){
            boolName->setText(varNames->variableNames.at(index));
            variableWid->setChecked(varValues->wordVariableValues.at(index));
        }
        hideOtherVariables(0);
        connect(variableWid, SIGNAL(released()), this, SLOT(setVariableValue()));
    }
}

void BehaviorVariablesUI::loadVariable(SpinBox *variableWid){
    if (loadedData){
        disconnect(variableWid, 0, this, 0);
        hkbBehaviorGraphStringData *varNames = static_cast<hkbBehaviorGraphStringData *>(loadedData->stringData.data());
        hkbVariableValueSet *varValues = static_cast<hkbVariableValueSet *>(loadedData->variableInitialValues.data());
        int index = table->currentRow();
        if (varValues->wordVariableValues.size() > index && varNames->variableNames.size() > index){
            intName->setText(varNames->variableNames.at(index));
            variableWid->setValue(varValues->wordVariableValues.at(index));
        }
        hideOtherVariables(1);
        connect(variableWid, SIGNAL(editingFinished()), this, SLOT(setVariableValue()));
    }
}

void BehaviorVariablesUI::loadVariable(DoubleSpinBox *variableWid){
    if (loadedData){
        disconnect(variableWid, 0, this, 0);
        hkbBehaviorGraphStringData *varNames = static_cast<hkbBehaviorGraphStringData *>(loadedData->stringData.data());
        hkbVariableValueSet *varValues = static_cast<hkbVariableValueSet *>(loadedData->variableInitialValues.data());
        int index = table->currentRow();
        if (varValues->wordVariableValues.size() > index && varNames->variableNames.size() > index){
            doubleName->setText(varNames->variableNames.at(index));
            variableWid->setValue(varValues->wordVariableValues.at(index));
        }
        hideOtherVariables(2);
        connect(variableWid, SIGNAL(editingFinished()), this, SLOT(setVariableValue()));
    }
}

void BehaviorVariablesUI::loadVariable(QuadVariableWidget *variableWid){
    if (loadedData){
        disconnect(variableWid, 0, this, 0);
        bool ok;
        hkbBehaviorGraphStringData *varNames = static_cast<hkbBehaviorGraphStringData *>(loadedData->stringData.data());
        hkbVariableValueSet *varValues = static_cast<hkbVariableValueSet *>(loadedData->variableInitialValues.data());
        int index = table->currentRow();
        if (varValues->wordVariableValues.size() > index && varNames->variableNames.size() > index){
            quadName->setText(varNames->variableNames.at(index));
            variableWid->setValue(loadedData->getQuadVariable(index, &ok));
            if (!ok){
                //
            }
        }
        hideOtherVariables(3);
        connect(variableWid, SIGNAL(editingFinished()), this, SLOT(setVariableValue()));
    }
}

void BehaviorVariablesUI::hideOtherVariables(int indexToView){
    for (int i = 0; i < variableWidget->rowCount(); i++){
        if (i == indexToView){
            variableWidget->setRowHidden(i, false);
        }else{
            variableWidget->setRowHidden(i, true);
        }
    }
}

void BehaviorVariablesUI::viewVariable(int row, int column){
    if (column == 2 && loadedData){
        QString type = table->item(row, 1)->text();
        if (type == "BOOL"){
            loadVariable(boolCB);
        }else if (type == "INT32"){
            loadVariable(intSB);
        }else if (type == "REAL"){
            loadVariable(doubleSB);
        }else if (type == "POINTER"){
            boneWeightArrayWidget->loadData(loadedData->getVariantVariable(row));
            stackLyt->setCurrentIndex(BONE_WEIGHTS_WIDGET);
            return;
        }else if (type == "VECTOR4"){
            loadVariable(quadWidget);
        }else if (type == "QUATERNION"){
            loadVariable(quadWidget);
        }else{
            return;
        }
        stackLyt->setCurrentIndex(VARIABLE_WIDGET);
    }
}

void BehaviorVariablesUI::returnToTable(){
    stackLyt->setCurrentIndex(TABLE_WIDGET);
}

void BehaviorVariablesUI::addVariableToTable(const QString & name, const QString & type){
    int row = table->rowCount();
    table->setRowCount(row + 1);
    table->setItem(row, 0, new QTableWidgetItem(name));
    table->setItem(row, 1, new QTableWidgetItem(type));
    table->setItem(row, 2, new QTableWidgetItem("Edit"));
    if (stackLyt->currentIndex() == VARIABLE_WIDGET){
        stackLyt->setCurrentIndex(TABLE_WIDGET);
    }
    table->setCurrentCell(row, 0);
    //emit variableAdded(name);
}

void BehaviorVariablesUI::loadData(HkxObject *data){
    if (data && data->getSignature() == HKB_BEHAVIOR_GRAPH_DATA){
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        loadedData = static_cast<hkbBehaviorGraphData *>(data);
        int row;
        hkbBehaviorGraphStringData *varNames = static_cast<hkbBehaviorGraphStringData *>(loadedData->stringData.data());
        for (int i = 0; i < varNames->variableNames.size(); i++){
            row = table->rowCount();
            if (table->rowCount() > i){
                table->setRowHidden(i, false);
                if (table->item(row, 0)){
                    table->item(row, 0)->setText(varNames->variableNames.at(i));
                }else{
                    table->setItem(row, 0, new QTableWidgetItem(varNames->variableNames.at(i)));
                }
            }else{
                table->setRowCount(row + 1);
                table->setItem(row, 0, new QTableWidgetItem(varNames->variableNames.at(i)));
                table->setItem(row, 1, new QTableWidgetItem(loadedData->variableInfos.at(i).type.section("_", -1, -1)));
                table->setItem(row, 2, new QTableWidgetItem("Edit"));
            }
        }
        for (int j = varNames->variableNames.size(); j < table->rowCount(); j++){
            table->setRowHidden(j, true);
        }
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    }
}

void BehaviorVariablesUI::clear(){
    for (int i = table->rowCount() - 1; i >= 0; i--){
        table->removeRow(i);
    }
}

void BehaviorVariablesUI::addVariable(){
    if (loadedData){
        int type = typeSelector->currentIndex();
        hkVariableType varType;
        QString typeString;
        hkbBehaviorGraphStringData *vars = static_cast<hkbBehaviorGraphStringData *>(loadedData->stringData.data());
        switch (type){
        case VARIABLE_TYPE_BOOL:
            varType = hkVariableType::VARIABLE_TYPE_BOOL;
            loadedData->addVariable(varType);
            typeString = "BOOL";
            addVariableToTable(vars->variableNames.last(), typeString);
            break;
        case VARIABLE_TYPE_INT32:
            varType = hkVariableType::VARIABLE_TYPE_INT32;
            loadedData->addVariable(varType);
            typeString = "INT32";
            addVariableToTable(vars->variableNames.last(), typeString);
            break;
        case VARIABLE_TYPE_REAL:
            varType = hkVariableType::VARIABLE_TYPE_REAL;
            loadedData->addVariable(varType);
            typeString = "REAL";
            addVariableToTable(vars->variableNames.last(), typeString);
            break;
        case VARIABLE_TYPE_POINTER:
            varType = hkVariableType::VARIABLE_TYPE_POINTER;
            loadedData->addVariable(varType);
            typeString = "POINTER";
            addVariableToTable(vars->variableNames.last(), typeString);
            break;
        case VARIABLE_TYPE_VECTOR4:
            varType = hkVariableType::VARIABLE_TYPE_VECTOR4;
            loadedData->addVariable(varType);
            typeString = "VECTOR4";
            addVariableToTable(vars->variableNames.last(), typeString);
            break;
        case VARIABLE_TYPE_QUATERNION:
            varType = hkVariableType::VARIABLE_TYPE_QUATERNION;
            loadedData->addVariable(varType);
            typeString = "QUATERNION";
            addVariableToTable(vars->variableNames.last(), typeString);
            break;
        default:
            return;
        }
        loadedData->getParentFile()->setIsChanged(true);
        emit variableAdded(vars->variableNames.last(), typeString);
    }
}

void BehaviorVariablesUI::removeVariable(){
    if (loadedData){
        int index = table->currentRow();
        loadedData->removeVariable(index);
        if (index < table->rowCount()){
            table->removeRow(index);
        }
        if (stackLyt->currentIndex() == VARIABLE_WIDGET){
            stackLyt->setCurrentIndex(TABLE_WIDGET);
        }
        loadedData->getParentFile()->setIsChanged(true);
        emit variableRemoved(index);
    }
}

void BehaviorVariablesUI::setHkDataUI(HkDataUI *ui){
    dataUI = ui;
}
