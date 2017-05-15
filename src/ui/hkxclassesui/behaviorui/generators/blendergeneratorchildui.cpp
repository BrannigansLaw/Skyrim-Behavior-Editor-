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
#include <QGroupBox>

#define BASE_NUMBER_OF_ROWS 4

#define BONE_WEIGHTS_ROW 0
#define WEIGHT_ROW 1
#define WORLD_FROM_MODEL_WEIGHT_ROW 2
#define GENERATOR_ROW 3

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

QStringList BlenderGeneratorChildUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

BlenderGeneratorChildUI::BlenderGeneratorChildUI()
    : behaviorView(NULL),
      bsData(NULL),
      childIndex(0),
      topLyt(new QGridLayout),
      groupBox(new QGroupBox("hkbBlenderGeneratorChild")),
      returnPB(new QPushButton("Return")),
      table(new TableWidget),
      boneWeights(new BoneWeightArrayUI),
      weight(new DoubleSpinBox),
      worldFromModelWeight(new DoubleSpinBox)
{
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(BONE_WEIGHTS_ROW, NAME_COLUMN, new TableWidgetItem("boneWeights"));
    table->setItem(BONE_WEIGHTS_ROW, TYPE_COLUMN, new TableWidgetItem("hkbBoneWeightArray", Qt::AlignCenter));
    table->setItem(BONE_WEIGHTS_ROW, BINDING_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::white), "Click to view the list of variables to bind to this value"));
    table->setItem(BONE_WEIGHTS_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::white), "Click to view the list of bone weights"));
    table->setItem(WEIGHT_ROW, NAME_COLUMN, new TableWidgetItem("weight"));
    table->setItem(WEIGHT_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(WEIGHT_ROW, BINDING_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::white), "Click to view the list of variables to bind to this value"));
    table->setCellWidget(WEIGHT_ROW, VALUE_COLUMN, weight);
    table->setItem(WORLD_FROM_MODEL_WEIGHT_ROW, NAME_COLUMN, new TableWidgetItem("worldFromModelWeight"));
    table->setItem(WORLD_FROM_MODEL_WEIGHT_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(WORLD_FROM_MODEL_WEIGHT_ROW, BINDING_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::white), "Click to view the list of variables to bind to this value"));
    table->setCellWidget(WORLD_FROM_MODEL_WEIGHT_ROW, VALUE_COLUMN, worldFromModelWeight);
    table->setItem(GENERATOR_ROW, NAME_COLUMN, new TableWidgetItem("generator"));
    table->setItem(GENERATOR_ROW, TYPE_COLUMN, new TableWidgetItem("hkbGenerator", Qt::AlignCenter));
    table->setItem(GENERATOR_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(GENERATOR_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::white), "Click to view the list of generators"));
    topLyt->addWidget(returnPB, 0, 1, 1, 1);
    topLyt->addWidget(table, 1, 0, 8, 3);
    groupBox->setLayout(topLyt);
    //Order here must correspond with the ACTIVE_WIDGET Enumerated type!!!
    addWidget(groupBox);
    addWidget(boneWeights);
    connect(boneWeights, SIGNAL(returnToParent()), this, SLOT(returnToWidget()), Qt::UniqueConnection);
    connect(weight, SIGNAL(editingFinished()), this, SLOT(setWeight()), Qt::UniqueConnection);
    connect(worldFromModelWeight, SIGNAL(editingFinished()), this, SLOT(setWorldFromModelWeight()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellClicked(int,int)), this, SLOT(viewSelected(int,int)), Qt::UniqueConnection);
    connect(returnPB, SIGNAL(clicked(bool)), this, SIGNAL(returnToParent(bool)), Qt::UniqueConnection);
}

void BlenderGeneratorChildUI::loadData(HkxObject *data, int childindex){
    blockSignals(true);
    hkbVariableBindingSet *bind = NULL;
    if (data){
        if (data->getSignature() == HKB_BLENDER_GENERATOR_CHILD){
            childIndex = childindex;
            bsData = static_cast<hkbBlenderGeneratorChild *>(data);
            if (bsData->boneWeights.data()){
                table->item(GENERATOR_ROW, VALUE_COLUMN)->setText("Click to edit");
            }else{
                table->item(GENERATOR_ROW, VALUE_COLUMN)->setText("NULL");
            }
            weight->setValue(bsData->weight);
            worldFromModelWeight->setValue(bsData->worldFromModelWeight);
            if (bsData->generator.data()){
                table->item(GENERATOR_ROW, VALUE_COLUMN)->setText(static_cast<hkbGenerator *>(bsData->generator.data())->getName());
            }else{
                table->item(GENERATOR_ROW, VALUE_COLUMN)->setText("NONE");
            }
            bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
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
            CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorChildUI::loadData(): The data passed to the UI is the wrong type!\nSIGNATURE: "+QString::number(data->getSignature(), 16)))
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorChildUI::loadData(): The data passed to the UI is NULL!!!"))
    }
    blockSignals(false);
}

void BlenderGeneratorChildUI::setGenerator(int index, const QString & name){
    hkbBlenderGenerator *gen = NULL;
    DataIconManager *ptr = NULL;
    if (bsData){
        if (behaviorView){
            gen = static_cast<hkbBlenderGenerator *>(bsData->getParentGenerator());
            ptr = static_cast<BehaviorFile *>(bsData->getParentFile())->getGeneratorDataAt(index - 1);
            if (ptr && name != ptr->getName()){
                CRITICAL_ERROR_MESSAGE(QString("The name of the selected object does not match it's name in the object selection table!!!"))
            }else if (!gen){
                CRITICAL_ERROR_MESSAGE(QString("The currently loaded 'hkbBlenderGeneratorChild' has no parent 'hkbBlenderGenerator' or 'hkbPoseMatchingGenerator'!!!"))
            }else if (ptr == bsData || !behaviorView->reconnectIcon(behaviorView->getSelectedItem(), (DataIconManager *)bsData->generator.data(), ptr, false)){
                WARNING_MESSAGE(QString("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\nYou are attempting to create a circular branch or dead end!!!"))
            }else{
                if (behaviorView->getSelectedItem()){
                    behaviorView->removeItemFromGraph(behaviorView->getSelectedItem()->getChildWithData((DataIconManager *)bsData->generator.data()), childIndex);
                }else{
                    CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorChildUI::setGenerator(): The selected icon is NULL!!"));
                }
                if (bsData->parentBG.data()){
                    static_cast<hkbBlenderGenerator *>(bsData->parentBG.data())->removeObjectAt(childIndex);
                }else{
                    CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorChildUI::setGenerator(): The state is orphaned!!"));
                }
                behaviorView->removeGeneratorData();
                table->item(GENERATOR_ROW, BINDING_COLUMN)->setText(name);
                bsData->getParentFile()->toggleChanged(true);
                emit returnToParent(true);
            }
        }else{
            CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorChildUI::setGenerator(): The 'behaviorView' pointer is NULL!!"))
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorChildUI::setGenerator(): The 'bsData' pointer is NULL!!"))
    }
}

bool BlenderGeneratorChildUI::setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type){
    hkbVariableBindingSet *varBind = NULL;
    if (bsData){
        varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (index == 0){
            varBind->removeBinding(path);
            table->item(row, BINDING_COLUMN)->setText("None");
        }else if (static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableTypeAt(index - 1) == type){
            if (!varBind){
                varBind = new hkbVariableBindingSet(bsData->getParentFile());
                bsData->variableBindingSet = HkxSharedPtr(varBind);
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
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorChildUI::setBinding(): The 'bsData' pointer is NULL!!"))
        return false;
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
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorChildUI::setBindingVariable(): The 'bsData' pointer is NULL!!"))
    }
}

void BlenderGeneratorChildUI::setWeight(){
    if (bsData){
        bsData->weight = weight->value();
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorChildUI::setWeight(): The 'bsData' pointer is NULL!!"))
    }
}

void BlenderGeneratorChildUI::setWorldFromModelWeight(){
    if (bsData){
        bsData->worldFromModelWeight = worldFromModelWeight->value();
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorChildUI::setWorldFromModelWeight(): The 'bsData' pointer is NULL!!"))
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
                setCurrentIndex(BONE_WEIGHTS_ARRAY_WIDGET);
            }else if (column == BINDING_COLUMN){
                if (bsData->variableBindingSet.data()){
                    emit viewProperties(static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data())->getVariableIndexOfBinding(path) + 1);
                }else{
                    emit viewProperties(0);
                }
            }
        }else if (row == GENERATOR_ROW && column == VALUE_COLUMN){
            emit viewGenerators(static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData->generator) + 1);
        }else if (row >= 0 && row != GENERATOR_ROW && column == BINDING_COLUMN){
            if (bsData->variableBindingSet.data()){
                emit viewVariables(static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data())->getVariableIndexOfBinding(path) + 1);
            }else{
                emit viewVariables(0);
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorChildUI::viewSelected(): The 'bsData' pointer is NULL!!"))
    }
}

void BlenderGeneratorChildUI::returnToWidget(){
    setCurrentIndex(MAIN_WIDGET);
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
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorChildUI::variableRenamed(): The 'bsData' pointer is NULL!!"))
    }
}

void BlenderGeneratorChildUI::generatorRenamed(const QString &name, int index){
    if (bsData){
        index--;
        if (index == static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData->generator)){
            table->item(GENERATOR_ROW, VALUE_COLUMN)->setText(name);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlenderGeneratorChildUI::generatorRenamed(): The 'bsData' pointer is NULL!!"))
    }
}

void BlenderGeneratorChildUI::setBehaviorView(BehaviorGraphView *view){
    behaviorView = view;
}
