#include "blendingtransitioneffectui.h"
#include "src/hkxclasses/behavior/hkbblendingtransitioneffect.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/ui/genericdatawidgets.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QCheckBox>
#include <QStackedLayout>
#include <QHeaderView>
#include <QSpinBox>
#include <QGroupBox>

#define BASE_NUMBER_OF_ROWS 10

#define NAME_ROW 0
#define SELF_TRANSITION_MODE_ROW 1
#define EVENT_MODE_ROW 2
#define DURATION_ROW 3
#define TO_GENERATOR_START_TIME_FRACTION_ROW 4
#define FLAG_SYNC_ROW 5
#define FLAG_IGNORE_FROM_WORLD_FROM_MODEL_ROW 6
#define FLAG_IGNORE_TO_WORLD_FROM_MODEL_ROW 7
#define END_MODE_ROW 8
#define BLEND_CURVE_ROW 9

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

QStringList BlendingTransitionEffectUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

BlendingTransitionEffectUI::BlendingTransitionEffectUI()
    : bsData(NULL),
      topLyt(new QGridLayout),
      returnPB(new QPushButton("Return")),
      table(new TableWidget),
      name(new LineEdit),
      selfTransitionMode(new ComboBox),
      eventMode(new ComboBox),
      duration(new DoubleSpinBox),
      toGeneratorStartTimeFraction(new DoubleSpinBox),
      flagSync(new QCheckBox),
      flagIgnoreFromWorldFromModel(new QCheckBox),
      flagIgnoreToWorldFromModel(new QCheckBox),
      endMode(new ComboBox),
      blendCurve(new ComboBox)
{
    setTitle("hkbBlendingTransitionEffect");
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(NAME_ROW, NAME_COLUMN, new TableWidgetItem("name"));
    table->setItem(NAME_ROW, TYPE_COLUMN, new TableWidgetItem("hkStringPtr"));
    table->setItem(NAME_ROW, BINDING_COLUMN, new TableWidgetItem("N/A"));
    table->setCellWidget(NAME_ROW, VALUE_COLUMN, name);
    table->setItem(SELF_TRANSITION_MODE_ROW, NAME_COLUMN, new TableWidgetItem("selfTransitionMode"));
    table->setItem(SELF_TRANSITION_MODE_ROW, TYPE_COLUMN, new TableWidgetItem("SelfTransitionMode"));
    table->setItem(SELF_TRANSITION_MODE_ROW, BINDING_COLUMN, new TableWidgetItem("N/A"));
    table->setCellWidget(SELF_TRANSITION_MODE_ROW, VALUE_COLUMN, selfTransitionMode);
    table->setItem(EVENT_MODE_ROW, NAME_COLUMN, new TableWidgetItem("eventMode"));
    table->setItem(EVENT_MODE_ROW, TYPE_COLUMN, new TableWidgetItem("EventMode"));
    table->setItem(EVENT_MODE_ROW, BINDING_COLUMN, new TableWidgetItem("N/A"));
    table->setCellWidget(EVENT_MODE_ROW, VALUE_COLUMN, eventMode);
    table->setItem(DURATION_ROW, NAME_COLUMN, new TableWidgetItem("duration"));
    table->setItem(DURATION_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal"));
    table->setItem(DURATION_ROW, BINDING_COLUMN, new TableWidgetItem("NONE"));
    table->setCellWidget(DURATION_ROW, VALUE_COLUMN, duration);
    table->setItem(TO_GENERATOR_START_TIME_FRACTION_ROW, NAME_COLUMN, new TableWidgetItem("toGeneratorStartTimeFraction"));
    table->setItem(TO_GENERATOR_START_TIME_FRACTION_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal"));
    table->setItem(TO_GENERATOR_START_TIME_FRACTION_ROW, BINDING_COLUMN, new TableWidgetItem("NONE"));
    table->setCellWidget(TO_GENERATOR_START_TIME_FRACTION_ROW, VALUE_COLUMN, toGeneratorStartTimeFraction);
    table->setItem(FLAG_SYNC_ROW, NAME_COLUMN, new TableWidgetItem("flagSync"));
    table->setItem(FLAG_SYNC_ROW, TYPE_COLUMN, new TableWidgetItem("FlagBits"));
    table->setItem(FLAG_SYNC_ROW, BINDING_COLUMN, new TableWidgetItem("N/A"));
    table->setCellWidget(FLAG_SYNC_ROW, VALUE_COLUMN, flagSync);
    table->setItem(FLAG_IGNORE_FROM_WORLD_FROM_MODEL_ROW, NAME_COLUMN, new TableWidgetItem("flagIgnoreFromWorldFromModel"));
    table->setItem(FLAG_IGNORE_FROM_WORLD_FROM_MODEL_ROW, TYPE_COLUMN, new TableWidgetItem("FlagBits"));
    table->setItem(FLAG_IGNORE_FROM_WORLD_FROM_MODEL_ROW, BINDING_COLUMN, new TableWidgetItem("N/A"));
    table->setCellWidget(FLAG_IGNORE_FROM_WORLD_FROM_MODEL_ROW, VALUE_COLUMN, flagIgnoreFromWorldFromModel);
    table->setItem(FLAG_IGNORE_TO_WORLD_FROM_MODEL_ROW, NAME_COLUMN, new TableWidgetItem("flagIgnoreToWorldFromModel"));
    table->setItem(FLAG_IGNORE_TO_WORLD_FROM_MODEL_ROW, TYPE_COLUMN, new TableWidgetItem("FlagBits"));
    table->setItem(FLAG_IGNORE_TO_WORLD_FROM_MODEL_ROW, BINDING_COLUMN, new TableWidgetItem("N/A"));
    table->setCellWidget(FLAG_IGNORE_TO_WORLD_FROM_MODEL_ROW, VALUE_COLUMN, flagIgnoreToWorldFromModel);
    table->setItem(END_MODE_ROW, NAME_COLUMN, new TableWidgetItem("endMode"));
    table->setItem(END_MODE_ROW, TYPE_COLUMN, new TableWidgetItem("EndMode"));
    table->setItem(END_MODE_ROW, BINDING_COLUMN, new TableWidgetItem("N/A"));
    table->setCellWidget(END_MODE_ROW, VALUE_COLUMN, endMode);
    table->setItem(BLEND_CURVE_ROW, NAME_COLUMN, new TableWidgetItem("blendCurve"));
    table->setItem(BLEND_CURVE_ROW, TYPE_COLUMN, new TableWidgetItem("BlendCurve"));
    table->setItem(BLEND_CURVE_ROW, BINDING_COLUMN, new TableWidgetItem("N/A"));
    table->setCellWidget(BLEND_CURVE_ROW, VALUE_COLUMN, blendCurve);
    topLyt->addWidget(returnPB, 0, 1, 1, 1);
    topLyt->addWidget(table, 1, 0, 8, 3);
    setLayout(topLyt);
    connect(returnPB, SIGNAL(released()), this, SIGNAL(returnToParent()), Qt::UniqueConnection);
}

void BlendingTransitionEffectUI::loadData(HkxObject *data){
    blockSignals(true);
    hkbVariableBindingSet *varBind = NULL;
    QString varName;
    int ind = 0;
    if (data && data->getSignature() == HKB_BLENDING_TRANSITION_EFFECT){
        bsData = static_cast<hkbBlendingTransitionEffect *>(data);
        name->setText(bsData->name);
        if (selfTransitionMode->count() == 0){
            selfTransitionMode->insertItems(0, bsData->SelfTransitionMode);
        }
        selfTransitionMode->setCurrentIndex(bsData->SelfTransitionMode.indexOf(bsData->selfTransitionMode));
        if (eventMode->count() == 0){
            eventMode->insertItems(0, bsData->EventMode);
        }
        eventMode->setCurrentIndex(bsData->EventMode.indexOf(bsData->eventMode));
        varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (varBind){
            ind = varBind->getVariableIndexOfBinding("duration");
            if (ind != -1){
                varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableNameAt(ind);
                if (varName == ""){
                    varName = "NONE";
                }
                table->item(DURATION_ROW, BINDING_COLUMN)->setText(varName);
            }
        }else{
            table->item(DURATION_ROW, BINDING_COLUMN)->setText("NONE");
        }
        QStringList flags = bsData->flags.split("|");
        flagSync->setChecked(false);
        flagIgnoreFromWorldFromModel->setChecked(false);
        flagIgnoreToWorldFromModel->setChecked(false);
        if (flags.isEmpty()){
            if (bsData->flags == "FLAG_SYNC"){
                flagSync->setChecked(true);
            }else if (bsData->flags == "FLAG_IGNORE_FROM_WORLD_FROM_MODEL"){
                flagIgnoreFromWorldFromModel->setChecked(true);
            }else if (bsData->flags == "FLAG_IGNORE_TO_WORLD_FROM_MODEL"){
                flagIgnoreToWorldFromModel->setChecked(true);
            }
        }else{
            for (int i = 0; i < flags.size(); i++){
                if (flags.at(i) == "FLAG_SYNC"){
                    flagSync->setChecked(true);
                }else if (flags.at(i) == "FLAG_IGNORE_FROM_WORLD_FROM_MODEL"){
                    flagIgnoreFromWorldFromModel->setChecked(true);
                }else if (flags.at(i) == "FLAG_IGNORE_TO_WORLD_FROM_MODEL"){
                    flagIgnoreToWorldFromModel->setChecked(true);
                }
            }
        }
        duration->setValue(bsData->duration);
        toGeneratorStartTimeFraction->setValue(bsData->toGeneratorStartTimeFraction);
        if (endMode->count() == 0){
            endMode->insertItems(0, bsData->EndMode);
        }
        endMode->setCurrentIndex(bsData->EndMode.indexOf(bsData->endMode));
        if (blendCurve->count() == 0){
            blendCurve->insertItems(0, bsData->BlendCurve);
        }
        blendCurve->setCurrentIndex(bsData->BlendCurve.indexOf(bsData->blendCurve));
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlendingTransitionEffectUI::loadData(): The data is NULL or an incorrect type!!"));
    }
    blockSignals(false);
}

void BlendingTransitionEffectUI::setName(){
    //
}

void BlendingTransitionEffectUI::setBindingVariable(int index, const QString &name){
    if (bsData){
        int row = table->currentRow();
        switch (row){
        case DURATION_ROW:
            setBinding(index, row, name, "duration", VARIABLE_TYPE_REAL);
            break;
        default:
            return;
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlendingTransitionEffectUI::setBindingVariable(): The 'bsData' pointer is NULL!!"))
    }
}

void BlendingTransitionEffectUI::setSelfTransitionMode(){
    //
}

void BlendingTransitionEffectUI::setEventMode(){
    //
}

void BlendingTransitionEffectUI::setDuration(){
    //
}

void BlendingTransitionEffectUI::setToGeneratorStartTimeFraction(){
    //
}

void BlendingTransitionEffectUI::setEndMode(){
    //
}

void BlendingTransitionEffectUI::setBlendCurve(){
    //
}

void BlendingTransitionEffectUI::setFlagSync(){
    //
}

void BlendingTransitionEffectUI::setFlagIgnoreFromWorldFromModel(){
    //
}

void BlendingTransitionEffectUI::setFlagIgnoreToWorldFromModel(){
    //
}

void BlendingTransitionEffectUI::variableRenamed(const QString &name, int index){
    //
}

bool BlendingTransitionEffectUI::setBinding(int index, int row, const QString &variableName, const QString &path, hkVariableType type){
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
        CRITICAL_ERROR_MESSAGE(QString("BlendingTransitionEffectUI::setBinding(): The 'bsData' pointer is NULL!!"))
        return false;
    }
    return true;
}
