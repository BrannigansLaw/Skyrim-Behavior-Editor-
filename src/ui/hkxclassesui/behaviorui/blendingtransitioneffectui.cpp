#include "blendingtransitioneffectui.h"
#include "src/hkxclasses/behavior/hkbblendingtransitioneffect.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/ui/genericdatawidgets.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>

#include "src/ui/genericdatawidgets.h"
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

#define BINDING_ITEM_LABEL QString("Use Property     ")

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
    table->setItem(NAME_ROW, TYPE_COLUMN, new TableWidgetItem("hkStringPtr", Qt::AlignCenter));
    table->setItem(NAME_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(NAME_ROW, VALUE_COLUMN, name);
    table->setItem(SELF_TRANSITION_MODE_ROW, NAME_COLUMN, new TableWidgetItem("selfTransitionMode"));
    table->setItem(SELF_TRANSITION_MODE_ROW, TYPE_COLUMN, new TableWidgetItem("SelfTransitionMode", Qt::AlignCenter));
    table->setItem(SELF_TRANSITION_MODE_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(SELF_TRANSITION_MODE_ROW, VALUE_COLUMN, selfTransitionMode);
    table->setItem(EVENT_MODE_ROW, NAME_COLUMN, new TableWidgetItem("eventMode"));
    table->setItem(EVENT_MODE_ROW, TYPE_COLUMN, new TableWidgetItem("EventMode", Qt::AlignCenter));
    table->setItem(EVENT_MODE_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(EVENT_MODE_ROW, VALUE_COLUMN, eventMode);
    table->setItem(DURATION_ROW, NAME_COLUMN, new TableWidgetItem("duration"));
    table->setItem(DURATION_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(DURATION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(DURATION_ROW, VALUE_COLUMN, duration);
    table->setItem(TO_GENERATOR_START_TIME_FRACTION_ROW, NAME_COLUMN, new TableWidgetItem("toGeneratorStartTimeFraction"));
    table->setItem(TO_GENERATOR_START_TIME_FRACTION_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(TO_GENERATOR_START_TIME_FRACTION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(TO_GENERATOR_START_TIME_FRACTION_ROW, VALUE_COLUMN, toGeneratorStartTimeFraction);
    table->setItem(FLAG_SYNC_ROW, NAME_COLUMN, new TableWidgetItem("flagSync"));
    table->setItem(FLAG_SYNC_ROW, TYPE_COLUMN, new TableWidgetItem("FlagBits", Qt::AlignCenter));
    table->setItem(FLAG_SYNC_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(FLAG_SYNC_ROW, VALUE_COLUMN, flagSync);
    table->setItem(FLAG_IGNORE_FROM_WORLD_FROM_MODEL_ROW, NAME_COLUMN, new TableWidgetItem("flagIgnoreFromWorldFromModel"));
    table->setItem(FLAG_IGNORE_FROM_WORLD_FROM_MODEL_ROW, TYPE_COLUMN, new TableWidgetItem("FlagBits", Qt::AlignCenter));
    table->setItem(FLAG_IGNORE_FROM_WORLD_FROM_MODEL_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(FLAG_IGNORE_FROM_WORLD_FROM_MODEL_ROW, VALUE_COLUMN, flagIgnoreFromWorldFromModel);
    table->setItem(FLAG_IGNORE_TO_WORLD_FROM_MODEL_ROW, NAME_COLUMN, new TableWidgetItem("flagIgnoreToWorldFromModel"));
    table->setItem(FLAG_IGNORE_TO_WORLD_FROM_MODEL_ROW, TYPE_COLUMN, new TableWidgetItem("FlagBits", Qt::AlignCenter));
    table->setItem(FLAG_IGNORE_TO_WORLD_FROM_MODEL_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(FLAG_IGNORE_TO_WORLD_FROM_MODEL_ROW, VALUE_COLUMN, flagIgnoreToWorldFromModel);
    table->setItem(END_MODE_ROW, NAME_COLUMN, new TableWidgetItem("endMode"));
    table->setItem(END_MODE_ROW, TYPE_COLUMN, new TableWidgetItem("EndMode", Qt::AlignCenter));
    table->setItem(END_MODE_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(END_MODE_ROW, VALUE_COLUMN, endMode);
    table->setItem(BLEND_CURVE_ROW, NAME_COLUMN, new TableWidgetItem("blendCurve"));
    table->setItem(BLEND_CURVE_ROW, TYPE_COLUMN, new TableWidgetItem("BlendCurve", Qt::AlignCenter));
    table->setItem(BLEND_CURVE_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(BLEND_CURVE_ROW, VALUE_COLUMN, blendCurve);
    topLyt->addWidget(returnPB, 0, 1, 1, 1);
    topLyt->addWidget(table, 1, 0, 8, 3);
    setLayout(topLyt);
    connectSignals();
}

void BlendingTransitionEffectUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(returnPB, SIGNAL(released()), this, SIGNAL(returnToParent()), Qt::UniqueConnection);
    connect(selfTransitionMode, SIGNAL(currentIndexChanged(int)), this, SLOT(setSelfTransitionMode(int)), Qt::UniqueConnection);
    connect(eventMode, SIGNAL(currentIndexChanged(int)), this, SLOT(setEventMode(int)), Qt::UniqueConnection);
    connect(duration, SIGNAL(editingFinished()), this, SLOT(setDuration()), Qt::UniqueConnection);
    connect(toGeneratorStartTimeFraction, SIGNAL(editingFinished()), this, SLOT(setToGeneratorStartTimeFraction()), Qt::UniqueConnection);
    connect(flagSync, SIGNAL(released()), this, SLOT(toggleSyncFlag()), Qt::UniqueConnection);
    connect(flagIgnoreFromWorldFromModel, SIGNAL(released()), this, SLOT(toggleIgnoreFromWorldFromModelFlag()), Qt::UniqueConnection);
    connect(flagIgnoreToWorldFromModel, SIGNAL(released()), this, SLOT(toggleIgnoreToWorldFromModelFlag()), Qt::UniqueConnection);
    connect(endMode, SIGNAL(currentIndexChanged(int)), this, SLOT(setEndMode(int)), Qt::UniqueConnection);
    connect(blendCurve, SIGNAL(currentIndexChanged(int)), this, SLOT(setBlendCurve(int)), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)), Qt::UniqueConnection);
}

void BlendingTransitionEffectUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(returnPB, SIGNAL(released()), this, SIGNAL(returnToParent()));
    disconnect(selfTransitionMode, SIGNAL(currentIndexChanged(int)), this, SLOT(setSelfTransitionMode(int)));
    disconnect(eventMode, SIGNAL(currentIndexChanged(int)), this, SLOT(setEventMode(int)));
    disconnect(duration, SIGNAL(editingFinished()), this, SLOT(setDuration()));
    disconnect(toGeneratorStartTimeFraction, SIGNAL(editingFinished()), this, SLOT(setToGeneratorStartTimeFraction()));
    disconnect(flagSync, SIGNAL(released()), this, SLOT(toggleSyncFlag()));
    disconnect(flagIgnoreFromWorldFromModel, SIGNAL(released()), this, SLOT(toggleIgnoreFromWorldFromModelFlag()));
    disconnect(flagIgnoreToWorldFromModel, SIGNAL(released()), this, SLOT(toggleIgnoreToWorldFromModelFlag()));
    disconnect(endMode, SIGNAL(currentIndexChanged(int)), this, SLOT(setEndMode(int)));
    disconnect(blendCurve, SIGNAL(currentIndexChanged(int)), this, SLOT(setBlendCurve(int)));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)));
}

void BlendingTransitionEffectUI::loadData(HkxObject *data){
    disconnectSignals();
    hkbVariableBindingSet *varBind = NULL;
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
            loadBinding(DURATION_ROW, BINDING_COLUMN, varBind, "duration");
            loadBinding(TO_GENERATOR_START_TIME_FRACTION_ROW, BINDING_COLUMN, varBind, "toGeneratorStartTimeFraction");
        }else{
            table->item(DURATION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(TO_GENERATOR_START_TIME_FRACTION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
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
    connectSignals();
}

void BlendingTransitionEffectUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            bsData->getParentFile()->toggleChanged(true);
            emit transitionEffectRenamed(bsData->name);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlendingTransitionEffectUI::setName(): The data is NULL!!"));
    }
}

void BlendingTransitionEffectUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
    if (bsData){
        if (varBind){
            int index = varBind->getVariableIndexOfBinding(path);
            QString varName;
            if (index != -1){
                if (varBind->getBindingType(path) == hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY){
                    varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getCharacterPropertyNameAt(index, true);
                    table->item(row, colunm)->setCheckState(Qt::Checked);
                }else{
                    varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableNameAt(index);
                }
                if (varName == ""){
                    varName = "NONE";
                }
                table->item(row, colunm)->setText(BINDING_ITEM_LABEL+varName);
            }
        }else{
            CRITICAL_ERROR_MESSAGE(QString("BlendingTransitionEffectUI::loadBinding(): The variable binding set is NULL!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlendingTransitionEffectUI::loadBinding(): The data is NULL!!"));
    }
}

void BlendingTransitionEffectUI::setBindingVariable(int index, const QString &name){
    if (bsData){
        int row = table->currentRow();
        switch (row){
        case DURATION_ROW:
            if (table->item(DURATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                setBinding(index, row, name, "duration", VARIABLE_TYPE_REAL, true);
            }else{
                setBinding(index, row, name, "duration", VARIABLE_TYPE_REAL, false);
            }
            break;
        case TO_GENERATOR_START_TIME_FRACTION_ROW:
            if (table->item(TO_GENERATOR_START_TIME_FRACTION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                setBinding(index, row, name, "toGeneratorStartTimeFraction", VARIABLE_TYPE_REAL, true);
            }else{
                setBinding(index, row, name, "toGeneratorStartTimeFraction", VARIABLE_TYPE_REAL, false);
            }
            break;
        default:
            return;
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlendingTransitionEffectUI::setBindingVariable(): The 'bsData' pointer is NULL!!"))
    }
}

void BlendingTransitionEffectUI::setSelfTransitionMode(int index){
    if (bsData){
        bsData->selfTransitionMode = bsData->SelfTransitionMode.at(index);
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlendingTransitionEffectUI::setSelfTransitionMode(): The data is NULL!!"));
    }
}

void BlendingTransitionEffectUI::setEventMode(int index){
    if (bsData){
        bsData->eventMode = bsData->EventMode.at(index);
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlendingTransitionEffectUI::setEventMode(): The data is NULL!!"));
    }
}

void BlendingTransitionEffectUI::setDuration(){
    if (bsData){
        bsData->duration = duration->value();
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlendingTransitionEffectUI::setDuration(): The data is NULL!!"));
    }
}

void BlendingTransitionEffectUI::setToGeneratorStartTimeFraction(){
    if (bsData){
        bsData->toGeneratorStartTimeFraction = toGeneratorStartTimeFraction->value();
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlendingTransitionEffectUI::setToGeneratorStartTimeFraction(): The data is NULL!!"));
    }
}

void BlendingTransitionEffectUI::setEndMode(int index){
    if (bsData){
        bsData->endMode = bsData->EndMode.at(index);
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlendingTransitionEffectUI::setEndMode(): The data is NULL!!"));
    }
}

void BlendingTransitionEffectUI::setBlendCurve(int index){
    if (bsData){
        bsData->blendCurve = bsData->BlendCurve.at(index);
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlendingTransitionEffectUI::setBlendCurve(): The data is NULL!!"));
    }
}

void BlendingTransitionEffectUI::toggleSyncFlag(){
    if (bsData){
        if (flagSync->isChecked()){
            if (bsData->flags == ""){
                bsData->flags.append("FLAG_SYNC");
            }else if (!bsData->flags.contains("FLAG_SYNC")){
                bsData->flags.append("|FLAG_SYNC");
            }
        }else{
            if (bsData->flags == ""){
                bsData->flags.remove("FLAG_SYNC");
            }else{
                bsData->flags.remove("|FLAG_SYNC");
            }
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlendingTransitionEffectUI::setFlagSync(): The data is NULL!!"))
    }
}

void BlendingTransitionEffectUI::toggleIgnoreFromWorldFromModelFlag(){
    if (bsData){
        if (flagIgnoreFromWorldFromModel->isChecked()){
            if (bsData->flags == ""){
                bsData->flags.append("FLAG_IGNORE_FROM_WORLD_FROM_MODEL");
            }else if (!bsData->flags.contains("FLAG_IGNORE_FROM_WORLD_FROM_MODEL")){
                bsData->flags.append("|FLAG_IGNORE_FROM_WORLD_FROM_MODEL");
            }
        }else{
            if (bsData->flags == ""){
                bsData->flags.remove("FLAG_IGNORE_FROM_WORLD_FROM_MODEL");
            }else{
                bsData->flags.remove("|FLAG_IGNORE_FROM_WORLD_FROM_MODEL");
            }
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlendingTransitionEffectUI::setFlagIgnoreFromWorldFromModel(): The data is NULL!!"))
    }
}

void BlendingTransitionEffectUI::toggleIgnoreToWorldFromModelFlag(){
    if (bsData){
        if (flagIgnoreToWorldFromModel->isChecked()){
            if (bsData->flags == ""){
                bsData->flags.append("FLAG_IGNORE_TO_WORLD_FROM_MODEL");
            }else if (!bsData->flags.contains("FLAG_IGNORE_TO_WORLD_FROM_MODEL")){
                bsData->flags.append("|FLAG_IGNORE_TO_WORLD_FROM_MODEL");
            }
        }else{
            if (bsData->flags == ""){
                bsData->flags.remove("FLAG_IGNORE_TO_WORLD_FROM_MODEL");
            }else{
                bsData->flags.remove("|FLAG_IGNORE_TO_WORLD_FROM_MODEL");
            }
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlendingTransitionEffectUI::setFlagIgnoreToWorldFromModel(): The data is NULL!!"))
    }
}

void BlendingTransitionEffectUI::selectTableToView(bool viewproperties, const QString & path){
    if (bsData){
        if (viewproperties){
            if (bsData->variableBindingSet.data()){
                emit viewProperties(static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data())->getVariableIndexOfBinding(path) + 1);
            }else{
                emit viewProperties(0);
            }
        }else{
            if (bsData->variableBindingSet.data()){
                emit viewVariables(static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data())->getVariableIndexOfBinding(path) + 1);
            }else{
                emit viewVariables(0);
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlendingTransitionEffectUI::selectTableToView(): The data is NULL!!"));
    }
}

void BlendingTransitionEffectUI::viewSelectedChild(int row, int column){
    bool properties = false;
    if (bsData){
        if (column == BINDING_COLUMN){
            switch (row){
            case DURATION_ROW:
                if (table->item(DURATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "duration");
                break;
            case TO_GENERATOR_START_TIME_FRACTION_ROW:
                if (table->item(TO_GENERATOR_START_TIME_FRACTION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "toGeneratorStartTimeFraction");
                break;
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("StateMachineUI::viewSelectedChild(): The data is NULL!!"));
    }
}

void BlendingTransitionEffectUI::variableRenamed(const QString &name, int index){
    int bindIndex = -1;
    hkbVariableBindingSet *bind = NULL;
    if (bsData){
        index--;
        bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (bind){
            bindIndex = bind->getVariableIndexOfBinding("duration");
            if (bindIndex == index){
                table->item(DURATION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+name);
            }
            bindIndex = bind->getVariableIndexOfBinding("toGeneratorStartTimeFraction");
            if (bindIndex == index){
                table->item(TO_GENERATOR_START_TIME_FRACTION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+name);
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlendingTransitionEffectUI::variableRenamed(): The data is NULL!!"));
    }
}

bool BlendingTransitionEffectUI::setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty){
    hkbVariableBindingSet *varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
    if (bsData){
        if (index == 0){
            varBind->removeBinding(path);
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
        }else if ((!isProperty && static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableTypeAt(index - 1) == type) ||
                  (isProperty && static_cast<BehaviorFile *>(bsData->getParentFile())->getCharacterPropertyTypeAt(index - 1) == type)){
            if (!varBind){
                varBind = new hkbVariableBindingSet(bsData->getParentFile());
                bsData->variableBindingSet = HkxSharedPtr(varBind);
            }
            if (isProperty){
                varBind->addBinding(path, variableName, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY);
            }else{
                varBind->addBinding(path, variableName, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE);
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->toggleChanged(true);
        }else{
            WARNING_MESSAGE(QString("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BlendingTransitionEffectUI::setBinding(): The data is NULL!!"));
    }
    return true;
}
