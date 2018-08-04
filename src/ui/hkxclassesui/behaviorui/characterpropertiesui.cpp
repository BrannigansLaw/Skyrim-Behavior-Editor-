#include "characterpropertiesui.h"

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

#include "src/hkxclasses/behavior/hkbcharacterdata.h"
#include "src/hkxclasses/behavior/hkbcharacterstringdata.h"
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

QStringList CharacterPropertiesUI::types = {
    "BOOL",
    "INT32",
    "REAL",
    "POINTER",
    "VECTOR4",
    "QUATERNION"
};

QStringList CharacterPropertiesUI::headerLabels = {
    "Name",
    "Type",
    "Value"
};

CharacterPropertiesUI::CharacterPropertiesUI(const QString &title)
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
    table->setEditTriggers(QAbstractItemView::DoubleClicked);
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
    connect(boolName, SIGNAL(editingFinished()), nameMapper, SLOT(map()), Qt::UniqueConnection);
    connect(intName, SIGNAL(editingFinished()), nameMapper, SLOT(map()), Qt::UniqueConnection);
    connect(doubleName, SIGNAL(editingFinished()), nameMapper, SLOT(map()), Qt::UniqueConnection);
    connect(quadName, SIGNAL(editingFinished()), nameMapper, SLOT(map()), Qt::UniqueConnection);
    valueMapper->setMapping(boolCB, 0);
    valueMapper->setMapping(intSB, 1);
    valueMapper->setMapping(doubleSB, 2);
    valueMapper->setMapping(quadWidget, 3);
    connect(boolCB, SIGNAL(released()), valueMapper, SLOT(map()), Qt::UniqueConnection);
    connect(intSB, SIGNAL(editingFinished()), valueMapper, SLOT(map()), Qt::UniqueConnection);
    connect(doubleSB, SIGNAL(editingFinished()), valueMapper, SLOT(map()), Qt::UniqueConnection);
    connect(quadWidget, SIGNAL(editingFinished()), valueMapper, SLOT(map()), Qt::UniqueConnection);
    setLayout(verLyt);
    connect(removeObjectPB, SIGNAL(pressed()), this, SLOT(removeVariable()), Qt::UniqueConnection);
    connect(addObjectPB, SIGNAL(pressed()), this, SLOT(addVariable()), Qt::UniqueConnection);
    connect(nameMapper, SIGNAL(mapped(int)), this, SLOT(renameSelectedVariable(int)), Qt::UniqueConnection);
    connect(valueMapper, SIGNAL(mapped(int)), this, SLOT(setVariableValue(int)), Qt::UniqueConnection);
    connect(table, SIGNAL(cellClicked(int,int)), this, SLOT(viewVariable(int,int)), Qt::UniqueConnection);
    connect(returnBoolPB, SIGNAL(released()), this, SLOT(returnToTable()), Qt::UniqueConnection);
    connect(returnIntPB, SIGNAL(released()), this, SLOT(returnToTable()), Qt::UniqueConnection);
    connect(returnDoublePB, SIGNAL(released()), this, SLOT(returnToTable()), Qt::UniqueConnection);
    connect(returnQuadPB, SIGNAL(released()), this, SLOT(returnToTable()), Qt::UniqueConnection);
    connect(boneWeightArrayWidget, SIGNAL(returnToParent()), this, SLOT(returnToTable()), Qt::UniqueConnection);
    connect(table, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(setVariableName(QTableWidgetItem*)), Qt::UniqueConnection);
}

/*QSize CharacterPropertiesUI::sizeHint() const{
    return QSize(300, 300);
}*/

void CharacterPropertiesUI::setVariableValue(int type){
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
    loadedData->setIsFileChanged(true);
}

void CharacterPropertiesUI::renameSelectedVariable(int type){
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
    loadedData->setCharacterPropertyNameAt(table->currentRow(), newName);
    loadedData->setIsFileChanged(true);
    emit variableNameChanged(newName, table->currentRow());
}

void CharacterPropertiesUI::setVariableName(QTableWidgetItem *item){
    if (item && item->text() != "" && loadedData){
        int column = table->column(item);
        if (column == 0){
            loadedData->setCharacterPropertyNameAt(table->row(item), item->text());
        }
    }else{
        LogFile::writeToLog("CharacterPropertiesUI::setVariableName: error!!");
    }
}

void CharacterPropertiesUI::removeVariableFromTable(int row){
    if (row < table->rowCount()){
        table->removeRow(row);
    }
}

void CharacterPropertiesUI::loadVariable(CheckBox *variableWid){
    if (loadedData){
        disconnect(variableWid, 0, this, 0);
        int index = table->currentRow();
        boolName->setText(loadedData->getCharacterPropertyNameAt(index));
        variableWid->setChecked(loadedData->getWordVariableValueAt(index));
        hideOtherVariables(0);
        connect(variableWid, SIGNAL(released()), this, SLOT(setVariableValue()), Qt::UniqueConnection);
    }
}

void CharacterPropertiesUI::loadVariable(SpinBox *variableWid){
    if (loadedData){
        disconnect(variableWid, 0, this, 0);
        int index = table->currentRow();
        intName->setText(loadedData->getCharacterPropertyNameAt(index));
        variableWid->setValue(loadedData->getWordVariableValueAt(index));
        hideOtherVariables(1);
        connect(variableWid, SIGNAL(editingFinished()), this, SLOT(setVariableValue()), Qt::UniqueConnection);
    }
}

void CharacterPropertiesUI::loadVariable(DoubleSpinBox *variableWid){
    if (loadedData){
        disconnect(variableWid, 0, this, 0);
        int index = table->currentRow();
        doubleName->setText(loadedData->getCharacterPropertyNameAt(index));
        variableWid->setValue(loadedData->getWordVariableValueAt(index));
        hideOtherVariables(2);
        connect(variableWid, SIGNAL(editingFinished()), this, SLOT(setVariableValue()), Qt::UniqueConnection);
    }
}

void CharacterPropertiesUI::loadVariable(QuadVariableWidget *variableWid){
    if (loadedData){
        disconnect(variableWid, 0, this, 0);
        bool ok;
        int index = table->currentRow();
        quadName->setText(loadedData->getCharacterPropertyNameAt(index));
        variableWid->setValue(loadedData->getQuadVariable(index, &ok));
        hideOtherVariables(3);
        connect(variableWid, SIGNAL(editingFinished()), this, SLOT(setVariableValue()), Qt::UniqueConnection);
    }
}

void CharacterPropertiesUI::hideOtherVariables(int indexToView){
    for (auto i = 0; i < variableWidget->rowCount(); i++){
        if (i == indexToView){
            variableWidget->setRowHidden(i, false);
        }else{
            variableWidget->setRowHidden(i, true);
        }
    }
}

void CharacterPropertiesUI::viewVariable(int row, int column){
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

void CharacterPropertiesUI::returnToTable(){
    stackLyt->setCurrentIndex(TABLE_WIDGET);
}

void CharacterPropertiesUI::addVariableToTable(const QString & name, const QString & type){
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

void CharacterPropertiesUI::loadData(HkxObject *data){
    if (data && data->getSignature() == HKB_CHARACTER_DATA){
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        loadedData = static_cast<hkbCharacterData *>(data);
        int row;
        QStringList varNames = loadedData->getCharacterPropertyNames();
        QStringList typeNames = loadedData->getCharacterPropertyTypenames();
        for (auto i = 0; i < varNames.size(); i++){
            row = table->rowCount();
            if (table->rowCount() > i){
                table->setRowHidden(i, false);
                if (table->item(row, 0)){
                    table->item(row, 0)->setText(varNames.at(i));
                }else{
                    table->setItem(row, 0, new QTableWidgetItem(varNames.at(i)));
                }
            }else{
                table->setRowCount(row + 1);
                table->setItem(row, 0, new QTableWidgetItem(varNames.at(i)));
                table->setItem(row, 1, new QTableWidgetItem(typeNames.at(i).section("_", -1, -1)));
                table->setItem(row, 2, new QTableWidgetItem("Edit"));
            }
        }
        for (auto j = varNames.size(); j < table->rowCount(); j++){
            table->setRowHidden(j, true);
        }
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    }
}

void CharacterPropertiesUI::addVariable(){
    if (loadedData){
        auto type = typeSelector->currentIndex();
        hkVariableType varType;
        switch (type){
        case VARIABLE_TYPE_BOOL:
            varType = hkVariableType::VARIABLE_TYPE_BOOL;
            loadedData->addVariable(varType);
            addVariableToTable(loadedData->getLastCharacterPropertyName(), "BOOL");
            break;
        case VARIABLE_TYPE_INT32:
            varType = hkVariableType::VARIABLE_TYPE_INT32;
            loadedData->addVariable(varType);
            addVariableToTable(loadedData->getLastCharacterPropertyName(), "INT32");
            break;
        case VARIABLE_TYPE_REAL:
            varType = hkVariableType::VARIABLE_TYPE_REAL;
            loadedData->addVariable(varType);
            addVariableToTable(loadedData->getLastCharacterPropertyName(), "REAL");
            break;
        case VARIABLE_TYPE_POINTER:
            varType = hkVariableType::VARIABLE_TYPE_POINTER;
            loadedData->addVariable(varType);
            addVariableToTable(loadedData->getLastCharacterPropertyName(), "POINTER");
            break;
        case VARIABLE_TYPE_VECTOR4:
            varType = hkVariableType::VARIABLE_TYPE_VECTOR4;
            loadedData->addVariable(varType);
            addVariableToTable(loadedData->getLastCharacterPropertyName(), "VECTOR4");
            break;
        case VARIABLE_TYPE_QUATERNION:
            varType = hkVariableType::VARIABLE_TYPE_QUATERNION;
            loadedData->addVariable(varType);
            addVariableToTable(loadedData->getLastCharacterPropertyName(), "QUATERNION");
            break;
        default:
            return;
        }
        loadedData->setIsFileChanged(true);
        emit variableAdded(loadedData->getLastCharacterPropertyName());
    }
}

void CharacterPropertiesUI::removeVariable(){
    if (loadedData){
        int index = table->currentRow();

        //TO DO:
        //Need to check if variable is used in hkbExpressionCondition, hkbExpressionDataArray????
        //dont allow delection of iState var??
        //removing vars still bugged...

        //Need to update variables in all behaviors...
        /*QString message = static_cast<BehaviorFile *>(loadedData->getParentFile())->isVariableReferenced(index);
        if (message == ""){
            loadedData->removeVariable(index);
            if (index < table->rowCount()){
                table->removeRow(index);
            }
            if (stackLyt->currentIndex() == VARIABLE_WIDGET){
                stackLyt->setCurrentIndex(TABLE_WIDGET);
            }
            loadedData->setIsFileChanged(true);
            static_cast<BehaviorFile *>(loadedData->getParentFile())->updateVariableIndices(index);
            emit variableRemoved(index);
            table->setFocus();
        }else{
            WARNING_MESSAGE(message);
        }*/
    }
}

void CharacterPropertiesUI::setHkDataUI(HkDataUI *ui){
    dataUI = ui;
}
