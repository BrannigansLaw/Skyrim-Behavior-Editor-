#include "bslimbikmodifierui.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/modifiers/bslimbikmodifier.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/genericdatawidgets.h"

#include <QGridLayout>
#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 8

#define NAME_ROW 0
#define ENABLE_ROW 1
#define LIMIT_ANGLE_DEGREES_ROW 2
#define START_BONE_INDEX_ROW 3
#define END_BONE_INDEX_ROW 4
#define GAIN_ROW 5
#define BONE_RADIUS_ROW 6
#define CAST_OFFSET_ROW 7

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

QStringList BSLimbIKModifierUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

BSLimbIKModifierUI::BSLimbIKModifierUI()
    : bsData(NULL),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::cyan))),
      name(new LineEdit),
      enable(new QCheckBox),
      limitAngleDegrees(new DoubleSpinBox),
      startBoneIndex(new ComboBox),
      endBoneIndex(new ComboBox),
      gain(new DoubleSpinBox),
      boneRadius(new DoubleSpinBox),
      castOffset(new DoubleSpinBox)
{
    setTitle("BSLimbIKModifier");
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(NAME_ROW, NAME_COLUMN, new TableWidgetItem("name"));
    table->setItem(NAME_ROW, TYPE_COLUMN, new TableWidgetItem("hkStringPtr", Qt::AlignCenter));
    table->setItem(NAME_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(NAME_ROW, VALUE_COLUMN, name);
    table->setItem(ENABLE_ROW, NAME_COLUMN, new TableWidgetItem("enable"));
    table->setItem(ENABLE_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(ENABLE_ROW, BINDING_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP));
    table->setCellWidget(ENABLE_ROW, VALUE_COLUMN, enable);
    table->setItem(LIMIT_ANGLE_DEGREES_ROW, NAME_COLUMN, new TableWidgetItem("limitAngleDegrees"));
    table->setItem(LIMIT_ANGLE_DEGREES_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(LIMIT_ANGLE_DEGREES_ROW, BINDING_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP));
    table->setCellWidget(LIMIT_ANGLE_DEGREES_ROW, VALUE_COLUMN, limitAngleDegrees);
    table->setItem(START_BONE_INDEX_ROW, NAME_COLUMN, new TableWidgetItem("startBoneIndex"));
    table->setItem(START_BONE_INDEX_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(START_BONE_INDEX_ROW, BINDING_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP));
    table->setCellWidget(START_BONE_INDEX_ROW, VALUE_COLUMN, startBoneIndex);
    table->setItem(END_BONE_INDEX_ROW, NAME_COLUMN, new TableWidgetItem("endBoneIndex"));
    table->setItem(END_BONE_INDEX_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(END_BONE_INDEX_ROW, BINDING_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP));
    table->setCellWidget(END_BONE_INDEX_ROW, VALUE_COLUMN, endBoneIndex);
    table->setItem(GAIN_ROW, NAME_COLUMN, new TableWidgetItem("gain"));
    table->setItem(GAIN_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(GAIN_ROW, BINDING_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP));
    table->setCellWidget(GAIN_ROW, VALUE_COLUMN, gain);
    table->setItem(BONE_RADIUS_ROW, NAME_COLUMN, new TableWidgetItem("boneRadius"));
    table->setItem(BONE_RADIUS_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(BONE_RADIUS_ROW, BINDING_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP));
    table->setCellWidget(BONE_RADIUS_ROW, VALUE_COLUMN, boneRadius);
    table->setItem(CAST_OFFSET_ROW, NAME_COLUMN, new TableWidgetItem("castOffset"));
    table->setItem(CAST_OFFSET_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(CAST_OFFSET_ROW, BINDING_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP));
    table->setCellWidget(CAST_OFFSET_ROW, VALUE_COLUMN, castOffset);
    topLyt->addWidget(table, 0, 0, 8, 3);
    setLayout(topLyt);
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellClicked(int,int)), this, SLOT(tableItemSelected(int,int)), Qt::UniqueConnection);
}

void BSLimbIKModifierUI::loadData(HkxObject *data){
    blockSignals(true);
    if (data){
        if (data->getSignature() == BS_LIMB_IK_MODIFIER){
            QStringList stringlist;
            hkbVariableBindingSet *varBind = NULL;
            bsData = static_cast<BSLimbIKModifier *>(data);
            name->setText(bsData->name);
            enable->setChecked(bsData->enable);
            limitAngleDegrees->setValue(bsData->limitAngleDegrees);
            if (startBoneIndex->count() == 0){
                stringlist = static_cast<BehaviorFile *>(bsData->getParentFile())->getRigBoneNames();
                startBoneIndex->insertItems(0, stringlist);
            }
            startBoneIndex->setCurrentIndex(bsData->startBoneIndex + 1);
            if (endBoneIndex->count() == 0){
                stringlist = static_cast<BehaviorFile *>(bsData->getParentFile())->getRigBoneNames();
                endBoneIndex->insertItems(0, stringlist);
            }
            endBoneIndex->setCurrentIndex(bsData->endBoneIndex + 1);
            gain->setValue(bsData->gain);
            boneRadius->setValue(bsData->boneRadius);
            castOffset->setValue(bsData->castOffset);
            varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (varBind){
                loadBinding(ENABLE_ROW, BINDING_COLUMN, varBind, "enable");
                loadBinding(LIMIT_ANGLE_DEGREES_ROW, BINDING_COLUMN, varBind, "limitAngleDegrees");
                loadBinding(START_BONE_INDEX_ROW, BINDING_COLUMN, varBind, "startBoneIndex");
                loadBinding(END_BONE_INDEX_ROW, BINDING_COLUMN, varBind, "endBoneIndex");
                loadBinding(GAIN_ROW, BINDING_COLUMN, varBind, "gain");
                loadBinding(BONE_RADIUS_ROW, BINDING_COLUMN, varBind, "boneRadius");
                loadBinding(CAST_OFFSET_ROW, BINDING_COLUMN, varBind, "castOffset");
            }else{
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText("NONE");
                table->item(LIMIT_ANGLE_DEGREES_ROW, BINDING_COLUMN)->setText("NONE");
                table->item(START_BONE_INDEX_ROW, BINDING_COLUMN)->setText("NONE");
                table->item(END_BONE_INDEX_ROW, BINDING_COLUMN)->setText("NONE");
                table->item(GAIN_ROW, BINDING_COLUMN)->setText("NONE");
                table->item(BONE_RADIUS_ROW, BINDING_COLUMN)->setText("NONE");
                table->item(CAST_OFFSET_ROW, BINDING_COLUMN)->setText("NONE");
            }
        }else{
            CRITICAL_ERROR_MESSAGE(QString("BehaviorGraphUI::loadData(): The data is an incorrect type!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BehaviorGraphUI::loadData(): The data is NULL!!"));
    }
    blockSignals(false);
}

void BSLimbIKModifierUI::setName()
{

}

void BSLimbIKModifierUI::setEnable()
{

}

void BSLimbIKModifierUI::setLimitAngleDegrees()
{

}

void BSLimbIKModifierUI::setStartBoneIndex(int index)
{

}

void BSLimbIKModifierUI::setEndBoneIndex(int index)
{

}

void BSLimbIKModifierUI::setGain()
{

}

void BSLimbIKModifierUI::setBoneRadius()
{

}

void BSLimbIKModifierUI::setCastOffset()
{

}

void BSLimbIKModifierUI::tableItemSelected(int row, int column)
{

}

void BSLimbIKModifierUI::variableTableElementSelected(int index, const QString &name)
{

}

void BSLimbIKModifierUI::variableRenamed(const QString &name, int index)
{

}

bool BSLimbIKModifierUI::setBinding(int index, int row, const QString &variableName, const QString &path, hkVariableType type)
{
return true;
}

void BSLimbIKModifierUI::setBindingVariable(int index, const QString &name)
{

}

void BSLimbIKModifierUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
    if (bsData){
        if (varBind){
            int index = varBind->getVariableIndexOfBinding(path);
            QString varName;
            if (index != -1){
                varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableNameAt(index);
                if (varName == ""){
                    varName = "NONE";
                }
                table->item(row, colunm)->setText(varName);
            }
        }else{
            CRITICAL_ERROR_MESSAGE(QString("BSLimbIKModifierUI::loadBinding(): The variable binding set is NULL!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSLimbIKModifierUI::loadBinding(): The data is NULL!!"));
    }
}
