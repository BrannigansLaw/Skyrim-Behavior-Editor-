#include "blendergeneratorchildui.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/generators/hkbblendergeneratorchild.h"
#include "src/hkxclasses/behavior/generators/hkbblendergenerator.h"
#include "src/ui/genericdatawidgets.h"
#include "src/ui/hkxclassesui/behaviorui/boneweightarrayui.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/ui/behaviorgraphview.h"
#include "src/ui/treegraphicsitem.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QCheckBox>
#include <QStackedLayout>
#include <QHeaderView>
#include <QSpinBox>

#define BASE_NUMBER_OF_ROWS 5

#define RETURN_BUTTON_ROW 0
#define BONE_WEIGHTS_ROW 1
#define WEIGHT_ROW 2
#define WORLD_FROM_MODEL_WEIGHT_ROW 3
#define GENERATOR_ROW 4

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

/*
 * BlenderGeneratorChildUI
 */

QStringList BlenderGeneratorChildUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

BlenderGeneratorChildUI::BlenderGeneratorChildUI()
    : behaviorView(NULL),
      bsData(NULL),
      stackLyt(new QStackedLayout),
      returnPB(new QPushButton("Return")),
      table(new TableWidget),
      boneWeights(new BoneWeightArrayUI),
      weight(new DoubleSpinBox),
      worldFromModelWeight(new DoubleSpinBox)
{
    setTitle("hkbBlenderGeneratorChild");
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setCellWidget(RETURN_BUTTON_ROW, NAME_COLUMN, returnPB);
    table->setItem(BONE_WEIGHTS_ROW, NAME_COLUMN, new QTableWidgetItem("boneWeights"));
    table->setItem(BONE_WEIGHTS_ROW, TYPE_COLUMN, new QTableWidgetItem("hkbBoneWeightArray"));
    table->setItem(BONE_WEIGHTS_ROW, BINDING_COLUMN, new QTableWidgetItem("NONE"));
    table->setItem(BONE_WEIGHTS_ROW, VALUE_COLUMN, new QTableWidgetItem("NONE"));
    table->setItem(WEIGHT_ROW, NAME_COLUMN, new QTableWidgetItem("weight"));
    table->setItem(WEIGHT_ROW, TYPE_COLUMN, new QTableWidgetItem("hkReal"));
    table->setItem(WEIGHT_ROW, BINDING_COLUMN, new QTableWidgetItem("NONE"));
    table->setCellWidget(WEIGHT_ROW, VALUE_COLUMN, weight);
    table->setItem(WORLD_FROM_MODEL_WEIGHT_ROW, NAME_COLUMN, new QTableWidgetItem("worldFromModelWeight"));
    table->setItem(WORLD_FROM_MODEL_WEIGHT_ROW, TYPE_COLUMN, new QTableWidgetItem("hkReal"));
    table->setItem(WORLD_FROM_MODEL_WEIGHT_ROW, BINDING_COLUMN, new QTableWidgetItem("NONE"));
    table->setCellWidget(WORLD_FROM_MODEL_WEIGHT_ROW, VALUE_COLUMN, worldFromModelWeight);
    table->setItem(GENERATOR_ROW, NAME_COLUMN, new QTableWidgetItem("generator"));
    table->setItem(GENERATOR_ROW, TYPE_COLUMN, new QTableWidgetItem("hkbGenerator"));
    table->setItem(GENERATOR_ROW, BINDING_COLUMN, new QTableWidgetItem("N/A"));
    table->setItem(GENERATOR_ROW, VALUE_COLUMN, new QTableWidgetItem("NONE"));
    stackLyt->addWidget(table);
    stackLyt->addWidget(boneWeights);
    setLayout(stackLyt);
    connect(boneWeights, SIGNAL(returnToParent()), this, SLOT(returnToWidget()));
    connect(weight, SIGNAL(editingFinished()), this, SLOT(setWeight()));
    connect(worldFromModelWeight, SIGNAL(editingFinished()), this, SLOT(setWorldFromModelWeight()));
    connect(table, SIGNAL(cellClicked(int,int)), this, SLOT(viewSelected(int,int)));
    connect(returnPB, SIGNAL(released()), this, SIGNAL(returnToParent()));
}

void BlenderGeneratorChildUI::loadData(HkxObject *data){
    if (data){
        if (data->getSignature() == HKB_BLENDER_GENERATOR_CHILD){
            bsData = static_cast<hkbBlenderGeneratorChild *>(data);
            boneWeights->loadData(bsData->boneWeights.data());
            weight->setValue(bsData->weight);
            worldFromModelWeight->setValue(bsData->worldFromModelWeight);
            if (bsData->generator.data()){
                table->item(GENERATOR_ROW, VALUE_COLUMN)->setText(static_cast<hkbGenerator *>(bsData->generator.data())->getName());
            }else{
                table->item(GENERATOR_ROW, VALUE_COLUMN)->setText("NONE");
            }
            hkbVariableBindingSet *bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (bind){
                int bindIndex = bind->getVariableIndexOfBinding("boneWeights");
                if (bindIndex >= 0){
                    table->item(BONE_WEIGHTS_ROW, BINDING_COLUMN)->setText(static_cast<BehaviorFile *>(bsData->getParentFile())->getCharacterPropertyNameAt(bindIndex));
                }else{
                    table->item(BONE_WEIGHTS_ROW, BINDING_COLUMN)->setText("NONE");
                }
                bindIndex = bind->getVariableIndexOfBinding("weight");
                if (bindIndex >= 0){
                    table->item(WEIGHT_ROW, BINDING_COLUMN)->setText(static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableNameAt(bindIndex));
                }else{
                    table->item(WEIGHT_ROW, BINDING_COLUMN)->setText("NONE");
                }
                bindIndex = bind->getVariableIndexOfBinding("worldFromModelWeight");
                if (bindIndex >= 0){
                    table->item(WORLD_FROM_MODEL_WEIGHT_ROW, BINDING_COLUMN)->setText(static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableNameAt(bindIndex));
                }else{
                    table->item(WORLD_FROM_MODEL_WEIGHT_ROW, BINDING_COLUMN)->setText("NONE");
                }
            }
        }else{
            WARNING_MESSAGE(QString("BlenderGeneratorChildUI: The data passed to the UI is the wrong type!\nSIGNATURE: "+QString::number(data->getSignature(), 16)))
        }
    }else{
        WARNING_MESSAGE(QString("BlenderGeneratorChildUI: The data passed to the UI is NULL!!!"))
    }
}

void BlenderGeneratorChildUI::setGenerator(int index, const QString & name){
    if (bsData){
        if (behaviorView){
            hkbBlenderGenerator *gen = static_cast<hkbBlenderGenerator *>(bsData->getParentGenerator());
            int indexOfChild = gen->getIndexOfChild(bsData);
            DataIconManager *ptr = static_cast<BehaviorFile *>(bsData->getParentFile())->getGeneratorDataAt(index - 1);
            if (!gen){
                CRITICAL_ERROR_MESSAGE(QString("The currently loaded 'hkbBlenderGeneratorChild' has no parent 'hkbBlenderGenerator' or 'hkbPoseMatchingGenerator'!!!"))
            }else if (!ptr || ptr == bsData || !behaviorView->reconnectBranch(static_cast<DataIconManager *>(bsData->generator.data()), ptr, behaviorView->getSelectedItem())){
                WARNING_MESSAGE(QString("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\nYou are attempting to create a circular branch or dead end!!!"))
            }else{
                bsData->generator = HkxObjectExpSharedPtr(ptr);
                gen->addChildAt(bsData->generator, indexOfChild);
                behaviorView->removeGeneratorData();
                table->item(GENERATOR_ROW, VALUE_COLUMN)->setText(name);
                bsData->getParentFile()->toggleChanged(true);
            }
        }else{
            CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorChildUI: The 'behaviorView' pointer is NULL!!"))
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorChildUI: The 'bsData' pointer is NULL!!"))
    }
}

bool BlenderGeneratorChildUI::setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type){
    hkbVariableBindingSet *varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
    if (index == 0){
        varBind->removeBinding(path);
        table->item(row, BINDING_COLUMN)->setText("None");
    }else if (static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableTypeAt(index - 1) == type){
        if (!varBind){
            varBind = new hkbVariableBindingSet(bsData->getParentFile());
            bsData->variableBindingSet = HkxObjectExpSharedPtr(varBind);
            bsData->getParentFile()->addObjectToFile(varBind, -1);
        }
        if (type == VARIABLE_TYPE_POINTER){
            varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY);
        }else{
            varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE);
        }
        table->item(row, BINDING_COLUMN)->setText(variableName);
        bsData->getParentFile()->toggleChanged(true);
    }else{
        WARNING_MESSAGE(QString("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\nYou are attempting to bind a variable of an invalid type for this data field!!!"))
    }
    return true;
}

void BlenderGeneratorChildUI::setBindingVariable(int index, const QString & name){
    if (bsData){
        int row = table->currentRow();
        switch (row){
        case BONE_WEIGHTS_ROW:
            setBinding(index, row, name, "boneWeights", VARIABLE_TYPE_POINTER);
            break;
        case WEIGHT_ROW:
            setBinding(index, row, name, "weight", VARIABLE_TYPE_REAL);
            break;
        case WORLD_FROM_MODEL_WEIGHT_ROW:
            setBinding(index, row, name, "worldFromModelWeight", VARIABLE_TYPE_REAL);
            break;
        default:
            return;
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorChildUI: The 'bsData' pointer is NULL!!"))
    }
}

void BlenderGeneratorChildUI::setWeight(){
    if (bsData){
        bsData->weight = weight->value();
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorChildUI: The 'bsData' pointer is NULL!!"))
    }
}

void BlenderGeneratorChildUI::setWorldFromModelWeight(){
    if (bsData){
        bsData->worldFromModelWeight = worldFromModelWeight->value();
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorChildUI: The 'bsData' pointer is NULL!!"))
    }
}

void BlenderGeneratorChildUI::viewSelected(int row, int column){
    if (bsData){
        QString path;
        switch (row){
        case BONE_WEIGHTS_ROW:
            path = "boneWeights";
            break;
        case WEIGHT_ROW:
            path = "weight";
            break;
        case WORLD_FROM_MODEL_WEIGHT_ROW:
            path = "worldFromModelWeight";
            break;
        }
        if (row == BONE_WEIGHTS_ROW){
            if (column == VALUE_COLUMN){
                boneWeights->loadData(bsData->boneWeights.data());
                stackLyt->setCurrentIndex(CHILD_WIDGET);
            }else if (column == BINDING_COLUMN){
                if (bsData->variableBindingSet.data()){
                    emit viewProperties(static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data())->getVariableIndexOfBinding(path) + 1);
                }else{
                    emit viewProperties(0);
                }
            }
        }else if (row == GENERATOR_ROW && column == VALUE_COLUMN){
            emit viewGenerators(static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData->generator) + 1);
        }else if (row > RETURN_BUTTON_ROW && row != GENERATOR_ROW && column == BINDING_COLUMN){
            if (bsData->variableBindingSet.data()){
                emit viewVariables(static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data())->getVariableIndexOfBinding(path) + 1);
            }else{
                emit viewVariables(0);
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorChildUI: The 'bsData' pointer is NULL!!"))
    }
}

void BlenderGeneratorChildUI::returnToWidget(){
    stackLyt->setCurrentIndex(MAIN_WIDGET);
}

void BlenderGeneratorChildUI::variableRenamed(const QString & name, int index){
    if (bsData){
        index--;
        hkbVariableBindingSet *bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (bind){
            int bindIndex = bind->getVariableIndexOfBinding("boneWeights");
            if (bindIndex == index){
                table->item(BONE_WEIGHTS_ROW, BINDING_COLUMN)->setText(name);
            }else{
                bindIndex = bind->getVariableIndexOfBinding("weight");
                if (bindIndex == index){
                    table->item(WEIGHT_ROW, BINDING_COLUMN)->setText(name);
                }else{
                    bindIndex = bind->getVariableIndexOfBinding("worldFromModelWeight");
                    if (bindIndex == index){
                        table->item(WORLD_FROM_MODEL_WEIGHT_ROW, BINDING_COLUMN)->setText(name);
                    }
                }
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorChildUI: The 'bsData' pointer is NULL!!"))
    }
}

void BlenderGeneratorChildUI::setBehaviorView(BehaviorGraphView *view){
    behaviorView = view;
}
