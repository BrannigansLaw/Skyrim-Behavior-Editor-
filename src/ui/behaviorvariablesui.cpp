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
#include <QStackedLayout>

BehaviorVariablesUI::BehaviorVariablesUI(const QString &title)
    : dataUI(NULL),
      verLyt(new QVBoxLayout),
      stack(new QStackedLayout),
      loadedData(NULL),
      noDataL(new QLabel("No Data Selected!")),
      backPB(new QPushButton("To Parent")),
      view(new GenericListWidget("Behavior Variables")),
      boolWidget(new BoolWidget("")),
      intWidget(new IntWidget("")),
      doubleWidget(new DoubleWidget("")),
      quadVariableWidget(new QuadVariableWidget("")),
      quaternionWidget(new QuadVariableWidget(""))
{
    setTitle(title);
    stack->addWidget(view);
    stack->addWidget(boolWidget);
    stack->addWidget(intWidget);
    stack->addWidget(doubleWidget);
    stack->addWidget(quadVariableWidget);
    stack->addWidget(quaternionWidget);
    boolWidget->setBoundVariableVisbility(false);
    intWidget->setBoundVariableVisbility(false);
    doubleWidget->setBoundVariableVisbility(false);
    quadVariableWidget->setBoundVariableVisbility(false);
    quaternionWidget->setBoundVariableVisbility(false);
    boolWidget->setNameReadOnly(false);
    intWidget->setNameReadOnly(false);
    doubleWidget->setNameReadOnly(false);
    quadVariableWidget->setNameReadOnly(false);
    quaternionWidget->setNameReadOnly(false);
    backPB->setMaximumSize(size().width()*0.2, size().height()*0.1);
    verLyt->addWidget(backPB, 1, Qt::AlignCenter);
    verLyt->addLayout(stack, 20);
    setLayout(verLyt);
    connect(view, SIGNAL(itemRemoved(int)), this, SLOT(removeVariable(int)));
    connect(view, SIGNAL(itemAdded(int)), this, SLOT(addVariable(int)));
    connect(view, SIGNAL(itemSelected(int,int)), this, SLOT(changeCurrentDataWidget(int, int)));
    connect(backPB, SIGNAL(pressed()), this, SLOT(loadTable()));
    connect(boolWidget, SIGNAL(editingDone()), this, SLOT(setBoolVariableValue()));
    connect(intWidget, SIGNAL(editingDone()), this, SLOT(setIntVariableValue()));
    connect(doubleWidget, SIGNAL(editingDone()), this, SLOT(setDoubleVariableValue()));
    connect(quadVariableWidget, SIGNAL(editingDone()), this, SLOT(setVector4VariableValue()));
    connect(quaternionWidget, SIGNAL(editingDone()), this, SLOT(setQuaternionVariableValue()));
    connect(boolWidget, SIGNAL(textEdited(QString)), this, SLOT(renameSelectedVariable(QString)));
    connect(intWidget, SIGNAL(textEdited(QString)), this, SLOT(renameSelectedVariable(QString)));
    connect(doubleWidget, SIGNAL(textEdited(QString)), this, SLOT(renameSelectedVariable(QString)));
    connect(quadVariableWidget, SIGNAL(textEdited(QString)), this, SLOT(renameSelectedVariable(QString)));
    connect(quaternionWidget, SIGNAL(textEdited(QString)), this, SLOT(renameSelectedVariable(QString)));
}

void BehaviorVariablesUI::setBoolVariableValue(){
    loadedData->setWordVariableValueAt(view->getSelectedRow(), boolWidget->getValue());
}

void BehaviorVariablesUI::setIntVariableValue(){
    loadedData->setWordVariableValueAt(view->getSelectedRow(), intWidget->getValue());
}

void BehaviorVariablesUI::setDoubleVariableValue(){
    loadedData->setWordVariableValueAt(view->getSelectedRow(), doubleWidget->getValue());
}

void BehaviorVariablesUI::setVector4VariableValue(){
    loadedData->setQuadVariableValueAt(view->getSelectedRow(), quadVariableWidget->getValue());
}

void BehaviorVariablesUI::setQuaternionVariableValue(){
    loadedData->setQuadVariableValueAt(view->getSelectedRow(), quaternionWidget->getValue());
}

void BehaviorVariablesUI::renameSelectedVariable(const QString & name){
    loadedData->setVariableNameAt(view->getSelectedRow(), name);
    view->renameItemAt(view->getSelectedRow(), name);
}

void BehaviorVariablesUI::loadTable(){
    stack->setCurrentIndex(TABLE_WIDGET);
}

void BehaviorVariablesUI::loadData(HkxObject *data){
    if (data && data->getSignature() == HKB_BEHAVIOR_GRAPH_DATA){
        QString type;
        loadedData = static_cast<hkbBehaviorGraphData *>(data);
        hkbBehaviorGraphStringData *variableNames = static_cast<hkbBehaviorGraphStringData *>(loadedData->stringData.data());
        view->clear();
        QStringList types = {
            "VARIABLE_TYPE_BOOL",
            "VARIABLE_TYPE_INT32",
            "VARIABLE_TYPE_REAL",
            "VARIABLE_TYPE_POINTER",
            "VARIABLE_TYPE_VECTOR4",
            "VARIABLE_TYPE_QUATERNION"
        };
        view->setTypes(types);
        for (int i = 0; i < variableNames->variableNames.size(); i++){
            if (i < loadedData->variableInfos.size()){
                type = loadedData->variableInfos.at(i).type;
                view->addItem(variableNames->variableNames.at(i), type);
            }
        }
    }
}

void BehaviorVariablesUI::addVariable(int type){
    hkbBehaviorGraphData::hkVariableType varType;
    switch (type){
    case VARIABLE_TYPE_BOOL:
        varType = hkbBehaviorGraphData::hkVariableType::VARIABLE_TYPE_BOOL;
        break;
    case VARIABLE_TYPE_INT32:
        varType = hkbBehaviorGraphData::hkVariableType::VARIABLE_TYPE_INT32;
        break;
    case VARIABLE_TYPE_REAL:
        varType = hkbBehaviorGraphData::hkVariableType::VARIABLE_TYPE_REAL;
        break;
    case VARIABLE_TYPE_POINTER:
        varType = hkbBehaviorGraphData::hkVariableType::VARIABLE_TYPE_POINTER;
        break;
    case VARIABLE_TYPE_VECTOR4:
        varType = hkbBehaviorGraphData::hkVariableType::VARIABLE_TYPE_VECTOR4;
        break;
    case VARIABLE_TYPE_QUATERNION:
        varType = hkbBehaviorGraphData::hkVariableType::VARIABLE_TYPE_QUATERNION;
        break;
    default:
        return;
    }
    loadedData->addVariable(varType, view->getLastItemName());
}

void BehaviorVariablesUI::removeVariable(int index){
    loadedData->removeVariable(index);
}

void BehaviorVariablesUI::changeCurrentDataWidget(int index, int column){
    bool ok;
    hkbVariableValueSet *variableValues = static_cast<hkbVariableValueSet *>(loadedData->variableInitialValues.data());
    hkbBehaviorGraphStringData *variableNames = static_cast<hkbBehaviorGraphStringData *>(loadedData->stringData.data());
    if (column == 2 && loadedData && loadedData->variableInfos.size() > index){
        QString type = loadedData->variableInfos.at(index).type;
        if (type == "VARIABLE_TYPE_BOOL"){
            if (variableValues && variableValues->wordVariableValues.size() > index && variableNames->variableNames.size() > index){
                boolWidget->setValue(variableValues->wordVariableValues.at(index));
                boolWidget->setName(variableNames->variableNames.at(index));
                stack->setCurrentIndex(BOOL_WIDGET);
            }
        }else if (type == "VARIABLE_TYPE_INT8"){
            if (variableValues && variableValues->wordVariableValues.size() > index && variableNames->variableNames.size() > index){
                intWidget->setValue(variableValues->wordVariableValues.at(index));
                intWidget->setName(variableNames->variableNames.at(index));
                stack->setCurrentIndex(INT_WIDGET);
            }
        }else if (type == "VARIABLE_TYPE_INT16"){
            if (variableValues && variableValues->wordVariableValues.size() > index && variableNames->variableNames.size() > index){
                intWidget->setValue(variableValues->wordVariableValues.at(index));
                intWidget->setName(variableNames->variableNames.at(index));
                stack->setCurrentIndex(INT_WIDGET);
            }
        }else if (type == "VARIABLE_TYPE_INT32"){
            if (variableValues && variableValues->wordVariableValues.size() > index && variableNames->variableNames.size() > index){
                intWidget->setValue(variableValues->wordVariableValues.at(index));
                intWidget->setName(variableNames->variableNames.at(index));
                stack->setCurrentIndex(INT_WIDGET);
            }
        }else if (type == "VARIABLE_TYPE_REAL"){
            if (variableValues && variableValues->wordVariableValues.size() > index && variableNames->variableNames.size() > index){
                doubleWidget->setValue(variableValues->wordVariableValues.at(index));
                doubleWidget->setName(variableNames->variableNames.at(index));
                stack->setCurrentIndex(DOUBLE_WIDGET);
            }
        }else if (type == "VARIABLE_TYPE_VECTOR4"){
            if (variableValues && variableValues->wordVariableValues.size() > index && variableNames->variableNames.size() > index){
                quadVariableWidget->setValue(loadedData->getQuadVariable(index, &ok));
                quadVariableWidget->setName(variableNames->variableNames.at(index));
                stack->setCurrentIndex(VECTOR_4_WIDGET);
            }
        }else if (type == "VARIABLE_TYPE_QUATERNION"){
            if (variableValues && variableValues->wordVariableValues.size() > index && variableNames->variableNames.size() > index){
                quaternionWidget->setValue(loadedData->getQuadVariable(index, &ok));
                quaternionWidget->setName(variableNames->variableNames.at(index));
                stack->setCurrentIndex(QUATERNION_WIDGET);
            }
        }
    }
}

void BehaviorVariablesUI::setHkDataUI(HkDataUI *ui){
    dataUI = ui;
}
