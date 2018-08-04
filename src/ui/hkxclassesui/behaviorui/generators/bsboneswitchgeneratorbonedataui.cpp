#include "bsboneswitchgeneratorbonedataui.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/generators/bsboneswitchgenerator.h"
#include "src/hkxclasses/behavior/generators/hkbblendergeneratorchild.h"
#include "src/hkxclasses/behavior/generators/hkbstatemachinestateinfo.h"
#include "src/hkxclasses/behavior/generators/bsboneswitchgeneratorbonedata.h"
#include "src/ui/genericdatawidgets.h"
#include "src/ui/hkxclassesui/behaviorui/boneweightarrayui.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/hkxclasses/behavior/hkbboneweightarray.h"
#include "src/ui/behaviorgraphview.h"
#include "src/ui/treegraphicsitem.h"

#include "src/ui/genericdatawidgets.h"
#include <QStackedLayout>
#include <QHeaderView>
#include <QGroupBox>

#define BASE_NUMBER_OF_ROWS 2

#define BONE_WEIGHTS_ROW 0
#define GENERATOR_ROW 1

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

const QStringList BSBoneSwitchGeneratorBoneDataUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

BSBoneSwitchGeneratorBoneDataUI::BSBoneSwitchGeneratorBoneDataUI()
    : behaviorView(nullptr),
      bsData(nullptr),
      childIndex(0),
      topLyt(new QGridLayout),
      groupBox(new QGroupBox("BSBoneSwitchGeneratorBoneData")),
      returnPB(new QPushButton("Return")),
      table(new TableWidget(QColor(Qt::white))),
      spBoneWeight(new CheckButtonCombo("Edit")),
      boneWeightArrayUI(new BoneWeightArrayUI)
{
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(BONE_WEIGHTS_ROW, NAME_COLUMN, new TableWidgetItem("spBoneWeight"));
    table->setItem(BONE_WEIGHTS_ROW, TYPE_COLUMN, new TableWidgetItem("hkbBoneWeightArray", Qt::AlignCenter));
    table->setItem(BONE_WEIGHTS_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setItem(BONE_WEIGHTS_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray)));
    table->setCellWidget(BONE_WEIGHTS_ROW, VALUE_COLUMN, spBoneWeight);
    table->setItem(GENERATOR_ROW, NAME_COLUMN, new TableWidgetItem("pGenerator"));
    table->setItem(GENERATOR_ROW, TYPE_COLUMN, new TableWidgetItem("hkbGenerator", Qt::AlignCenter));
    table->setItem(GENERATOR_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(GENERATOR_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_GENERATORS_TABLE_TIP));
    topLyt->addWidget(returnPB, 0, 1, 1, 1);
    topLyt->addWidget(table, 1, 0, 8, 3);
    groupBox->setLayout(topLyt);
    //Order here must correspond with the ACTIVE_WIDGET Enumerated type!!!
    addWidget(groupBox);
    addWidget(boneWeightArrayUI);
    returnPB->setVisible(false);
    toggleSignals(true);
}

void BSBoneSwitchGeneratorBoneDataUI::toggleSignals(bool toggleconnections){
    if (toggleconnections){
        connect(boneWeightArrayUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()), Qt::UniqueConnection);
        connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)), Qt::UniqueConnection);
        connect(spBoneWeight, SIGNAL(pressed()), this, SLOT(viewBoneWeights()), Qt::UniqueConnection);
        connect(spBoneWeight, SIGNAL(enabled(bool)), this, SLOT(toggleBoneWeights(bool)), Qt::UniqueConnection);
        connect(returnPB, SIGNAL(clicked(bool)), this, SIGNAL(returnToParent(bool)), Qt::UniqueConnection);
    }else{
        disconnect(returnPB, SIGNAL(clicked(bool)), this, SIGNAL(returnToParent(bool)));
        disconnect(boneWeightArrayUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()));
        disconnect(spBoneWeight, SIGNAL(pressed()), this, SLOT(viewBoneWeights()));
        disconnect(spBoneWeight, SIGNAL(enabled(bool)), this, SLOT(toggleBoneWeights(bool)));
        disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)));
    }
}

void BSBoneSwitchGeneratorBoneDataUI::loadData(HkxObject *data, int childindex){
    hkbVariableBindingSet *varBind = nullptr;
    toggleSignals(false);
    if (data){
        if (data->getSignature() == BS_BONE_SWITCH_GENERATOR_BONE_DATA){
            childIndex = childindex;
            bsData = static_cast<BSBoneSwitchGeneratorBoneData *>(data);
            (bsData->getSpBoneWeight().data()) ? spBoneWeight->setChecked(true) : spBoneWeight->setChecked(false);
            table->item(GENERATOR_ROW, VALUE_COLUMN)->setText(bsData->getPGeneratorName());
            varBind = bsData->getVariableBindingSetData();
            if (varBind){
                loadBinding(BONE_WEIGHTS_ROW, BINDING_COLUMN, varBind, "spBoneWeight");
            }else{
                table->item(BONE_WEIGHTS_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
        }else{
            CRITICAL_ERROR_MESSAGE(QString("BSBoneSwitchGeneratorBoneDataUI::loadData(): The data passed to the UI is the wrong type!\nSIGNATURE: "+QString::number(data->getSignature(), 16)).toLocal8Bit().data());
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSBoneSwitchGeneratorBoneDataUI::loadData(): The data passed to the UI is nullptr!!!");
    }
    toggleSignals(true);
}

void BSBoneSwitchGeneratorBoneDataUI::loadBinding(int row, int column, hkbVariableBindingSet *varBind, const QString &path){
    UIHelperFunctions::loadBinding(row, column, varBind, path, table, bsData);
}

void BSBoneSwitchGeneratorBoneDataUI::setGenerator(int index, const QString & name){
    UIHelperFunctions::setGenerator(index, name, bsData, static_cast<hkbGenerator *>(bsData->pGenerator.data()), NULL_SIGNATURE, HkxObject::TYPE_GENERATOR, table, behaviorView, GENERATOR_ROW, VALUE_COLUMN);
    //emit returnToParent(true);
}

void BSBoneSwitchGeneratorBoneDataUI::setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty){
    UIHelperFunctions::setBinding(index, row, BINDING_COLUMN, variableName, path, type, isProperty, table, bsData);
}

void BSBoneSwitchGeneratorBoneDataUI::setBindingVariable(int index, const QString & name){
    if (bsData){
        auto isProperty = false;
        auto row = table->currentRow();
        switch (row){
        case BONE_WEIGHTS_ROW:
            if (table->item(BONE_WEIGHTS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "spBoneWeight", VARIABLE_TYPE_POINTER, isProperty);
            break;
        default:
            return;
        }
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("BSBoneSwitchGeneratorBoneDataUI::setBindingVariable(): The 'bsData' pointer is nullptr!!");
    }
}

void BSBoneSwitchGeneratorBoneDataUI::viewBoneWeights(){
    if (bsData){
        boneWeightArrayUI->loadData(bsData->getSpBoneWeight().data());
        setCurrentIndex(BONE_WEIGHT_ARRAY_WIDGET);
    }else{
        CRITICAL_ERROR_MESSAGE("BSBoneSwitchGeneratorBoneDataUI::viewBoneWeights(): The data is nullptr!!");
    }
}

void BSBoneSwitchGeneratorBoneDataUI::toggleBoneWeights(bool enable){
    if (bsData){
        if (!enable){
            bsData->spBoneWeight = HkxSharedPtr();
            static_cast<BehaviorFile *>(bsData->getParentFile())->removeOtherData();
        }else if (enable && !bsData->getSpBoneWeight().data()){
            bsData->spBoneWeight = HkxSharedPtr(new hkbBoneWeightArray(bsData->getParentFile(), -1, static_cast<BehaviorFile *>(bsData->getParentFile())->getNumberOfBones()));
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSBoneSwitchGeneratorBoneDataUI::toggleBoneWeights(): The data is nullptr!!");
    }
}

void BSBoneSwitchGeneratorBoneDataUI::selectTableToView(bool viewproperties, const QString & path){
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
        CRITICAL_ERROR_MESSAGE("BSBoneSwitchGeneratorBoneDataUI::selectTableToView(): The data is nullptr!!");
    }
}

void BSBoneSwitchGeneratorBoneDataUI::viewSelected(int row, int column){
    if (bsData){
        auto properties = false;
        if (column == BINDING_COLUMN){
            switch (row){
            case BONE_WEIGHTS_ROW:
                if (table->item(BONE_WEIGHTS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "spBoneWeight");
                break;
            }
        }else if (row == GENERATOR_ROW && column == VALUE_COLUMN){
            auto list = {hkbStateMachineStateInfo::getClassname(), hkbBlenderGeneratorChild::getClassname(), BSBoneSwitchGeneratorBoneData::getClassname()};
            emit viewGenerators(bsData->getIndexOfGenerator(bsData->getPGenerator()) + 1, QString(), list);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSBoneSwitchGeneratorBoneDataUI::viewSelected(): The 'bsData' pointer is nullptr!!");
    }
}

void BSBoneSwitchGeneratorBoneDataUI::returnToWidget(){
    setCurrentIndex(MAIN_WIDGET);
}

void BSBoneSwitchGeneratorBoneDataUI::variableRenamed(const QString & name, int index){
    if (bsData){
        hkbVariableBindingSet *bind = bsData->getVariableBindingSetData();
        if (bind){
            auto bindIndex = bind->getVariableIndexOfBinding("spBoneWeight");
            (bindIndex == index) ? table->item(BONE_WEIGHTS_ROW, BINDING_COLUMN)->setText(name) : NULL;
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSBoneSwitchGeneratorBoneDataUI::variableRenamed(): The 'bsData' pointer is nullptr!!");
    }
}

void BSBoneSwitchGeneratorBoneDataUI::generatorRenamed(const QString &name, int index){
    if (bsData){
        if (index == static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData->getPGenerator())){
            table->item(GENERATOR_ROW, VALUE_COLUMN)->setText(name);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSBoneSwitchGeneratorBoneDataUI::generatorRenamed(): The 'bsData' pointer is nullptr!!");
    }
}

void BSBoneSwitchGeneratorBoneDataUI::setBehaviorView(BehaviorGraphView *view){
    behaviorView = view;
}

void BSBoneSwitchGeneratorBoneDataUI::generatorTableElementSelected(int index, const QString &name){
    switch (currentIndex()){
    case MAIN_WIDGET:
        setGenerator(index, name);
        break;
    default:
        WARNING_MESSAGE("BSBoneSwitchGeneratorBoneDataUI::generatorTableElementSelected(): An unwanted element selected event was recieved!!");
    }
}

void BSBoneSwitchGeneratorBoneDataUI::variableTableElementSelected(int index, const QString &name){
    switch (currentIndex()){
    case MAIN_WIDGET:
        setBindingVariable(index, name);
        break;
    default:
        WARNING_MESSAGE("BSBoneSwitchGeneratorBoneDataUI::variableTableElementSelected(): An unwanted element selected event was recieved!!");
    }
}

void BSBoneSwitchGeneratorBoneDataUI::connectToTables(GenericTableWidget *generators, GenericTableWidget *variables, GenericTableWidget *properties){
    if (variables && properties && generators){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(generators, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(variableTableElementSelected(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(variableTableElementSelected(int,QString)), Qt::UniqueConnection);
        connect(generators, SIGNAL(elementSelected(int,QString)), this, SLOT(generatorTableElementSelected(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewGenerators(int,QString,QStringList)), generators, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int,QString,QStringList)), variables, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int,QString,QStringList)), properties, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
    }else{
        CRITICAL_ERROR_MESSAGE("BlenderGeneratorChildUI::connectToTables(): One or more arguments are nullptr!!");
    }
}
