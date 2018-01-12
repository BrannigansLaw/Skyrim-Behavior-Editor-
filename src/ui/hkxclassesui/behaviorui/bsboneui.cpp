#include "bsboneui.h"

#include "src/utility.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/ui/genericdatawidgets.h"

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

#define BASE_NUMBER_OF_ROWS 6

#define INDEX_ROW 0
#define FWD_AXIS_LS_ROW 1
#define LIMIT_ANGLE_DEGREES_ROW 2
#define ON_GAIN_ROW 3
#define OFF_GAIN_ROW 4
#define ENABLED_ROW 5

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList BSBoneUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

BSBoneUI::BSBoneUI()
    : file(NULL),
      bsBoneIndex(-1),
      bsData(NULL),
      parent(NULL),
      topLyt(new QGridLayout),
      returnPB(new QPushButton("Return")),
      table(new TableWidget),
      index(new ComboBox),
      fwdAxisLS(new QuadVariableWidget),
      limitAngleDegrees(new DoubleSpinBox),
      onGain(new DoubleSpinBox),
      offGain(new DoubleSpinBox),
      enabled(new CheckBox)
{
    setTitle("BSBoneUI");
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(INDEX_ROW, NAME_COLUMN, new TableWidgetItem("index"));
    table->setItem(INDEX_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(INDEX_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(INDEX_ROW, VALUE_COLUMN, index);
    table->setItem(FWD_AXIS_LS_ROW, NAME_COLUMN, new TableWidgetItem("fwdAxisLS"));
    table->setItem(FWD_AXIS_LS_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(FWD_AXIS_LS_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(FWD_AXIS_LS_ROW, VALUE_COLUMN, fwdAxisLS);
    table->setItem(LIMIT_ANGLE_DEGREES_ROW, NAME_COLUMN, new TableWidgetItem("limitAngleDegrees"));
    table->setItem(LIMIT_ANGLE_DEGREES_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(LIMIT_ANGLE_DEGREES_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(LIMIT_ANGLE_DEGREES_ROW, VALUE_COLUMN, limitAngleDegrees);
    table->setItem(ON_GAIN_ROW, NAME_COLUMN, new TableWidgetItem("onGain"));
    table->setItem(ON_GAIN_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(ON_GAIN_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ON_GAIN_ROW, VALUE_COLUMN, onGain);
    table->setItem(OFF_GAIN_ROW, NAME_COLUMN, new TableWidgetItem("offGain"));
    table->setItem(OFF_GAIN_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(OFF_GAIN_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(OFF_GAIN_ROW, VALUE_COLUMN, offGain);
    table->setItem(ENABLED_ROW, NAME_COLUMN, new TableWidgetItem("enabled"));
    table->setItem(ENABLED_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(ENABLED_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ENABLED_ROW, VALUE_COLUMN, enabled);
    topLyt->addWidget(returnPB, 0, 1, 1, 1);
    topLyt->addWidget(table, 1, 0, 6, 3);
    setLayout(topLyt);
    connectSignals();
}

void BSBoneUI::connectSignals(){
    connect(returnPB, SIGNAL(released()), this, SIGNAL(returnToParent()), Qt::UniqueConnection);
    connect(index, SIGNAL(currentIndexChanged(int)), this, SLOT(setIndex(int)), Qt::UniqueConnection);
    connect(fwdAxisLS, SIGNAL(editingFinished()), this, SLOT(setFwdAxisLS()), Qt::UniqueConnection);
    connect(limitAngleDegrees, SIGNAL(editingFinished()), this, SLOT(setLimitAngleDegrees()), Qt::UniqueConnection);
    connect(onGain, SIGNAL(editingFinished()), this, SLOT(setOnGain()), Qt::UniqueConnection);
    connect(offGain, SIGNAL(editingFinished()), this, SLOT(setOffGain()), Qt::UniqueConnection);
    connect(enabled, SIGNAL(released()), this, SLOT(setEnabled()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)), Qt::UniqueConnection);
}

void BSBoneUI::disconnectSignals(){
    disconnect(returnPB, SIGNAL(released()), this, SIGNAL(returnToParent()));
    disconnect(index, SIGNAL(currentIndexChanged(int)), this, SLOT(setIndex(int)));
    disconnect(fwdAxisLS, SIGNAL(editingFinished()), this, SLOT(setFwdAxisLS()));
    disconnect(limitAngleDegrees, SIGNAL(editingFinished()), this, SLOT(setLimitAngleDegrees()));
    disconnect(onGain, SIGNAL(editingFinished()), this, SLOT(setOnGain()));
    disconnect(offGain, SIGNAL(editingFinished()), this, SLOT(setOffGain()));
    disconnect(enabled, SIGNAL(released()), this, SLOT(setEnabled()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)));
}

void BSBoneUI::loadData(BehaviorFile *parentFile, BSLookAtModifier::BsBone *bon, BSLookAtModifier *par, int ind, bool isEyeBone){
    disconnectSignals();
    if (parentFile && bon && par && ind > -1){
        if (isEyeBone){
            parameterName = "eyeBones:";
        }else{
            parameterName = "bones:";
        }
        parent = par;
        bsBoneIndex = ind;
        file = parentFile;
        bsData = bon;
        QStringList boneNames("None");
        if (index->count() == 0){
            boneNames = boneNames + file->getRigBoneNames();
            index->insertItems(0, boneNames);
        }
        index->setCurrentIndex(bsData->index + 1);
        limitAngleDegrees->setValue(bsData->limitAngleDegrees);
        fwdAxisLS->setValue(bsData->fwdAxisLS);
        onGain->setValue(bsData->onGain);
        offGain->setValue(bsData->offGain);
        enabled->setChecked(bsData->enabled);
        hkbVariableBindingSet *varBind = static_cast<hkbVariableBindingSet *>(parent->variableBindingSet.data());
        if (varBind){
            loadBinding(INDEX_ROW, BINDING_COLUMN, varBind, parameterName+QString::number(bsBoneIndex)+"/index");
            loadBinding(FWD_AXIS_LS_ROW, BINDING_COLUMN, varBind, parameterName+QString::number(bsBoneIndex)+"/fwdAxisLS");
            loadBinding(LIMIT_ANGLE_DEGREES_ROW, BINDING_COLUMN, varBind, parameterName+QString::number(bsBoneIndex)+"/boneIndex");
            loadBinding(ON_GAIN_ROW, BINDING_COLUMN, varBind, parameterName+QString::number(bsBoneIndex)+"/onGain");
            loadBinding(OFF_GAIN_ROW, BINDING_COLUMN, varBind, parameterName+QString::number(bsBoneIndex)+"/offGain");
            loadBinding(ENABLED_ROW, BINDING_COLUMN, varBind, parameterName+QString::number(bsBoneIndex)+"/enabled");
        }else{
            table->item(INDEX_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(FWD_AXIS_LS_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(LIMIT_ANGLE_DEGREES_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(ON_GAIN_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(OFF_GAIN_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            table->item(ENABLED_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
        }
    }else{
        (qFatal("BSBoneUI::loadData(): Behavior file, bind or event data is null!!!"));
    }
    connectSignals();
}

void BSBoneUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
    if (bsData){
        if (varBind){
            int index = varBind->getVariableIndexOfBinding(path);
            QString varName;
            if (index != -1){
                if (varBind->getBindingType(path) == hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY){
                    varName = static_cast<BehaviorFile *>(file)->getCharacterPropertyNameAt(index, true);
                    table->item(row, colunm)->setCheckState(Qt::Checked);
                }else{
                    varName = static_cast<BehaviorFile *>(file)->getVariableNameAt(index);
                }
            }
            if (varName == ""){
                varName = "NONE";
            }
            table->item(row, colunm)->setText(BINDING_ITEM_LABEL+varName);
        }else{
            (qFatal("BSBoneUI::loadBinding(): The variable binding set is NULL!!"));
        }
    }else{
        (qFatal("BSBoneUI::loadBinding(): The data is NULL!!"));
    }
}

bool BSBoneUI::setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty){
    hkbVariableBindingSet *varBind = static_cast<hkbVariableBindingSet *>(parent->variableBindingSet.data());
    if (bsData){
        if (index == 0){
            varBind->removeBinding(path);
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
        }else if ((!isProperty && static_cast<BehaviorFile *>(file)->getVariableTypeAt(index - 1) == type) ||
                  (isProperty && static_cast<BehaviorFile *>(file)->getCharacterPropertyTypeAt(index - 1) == type)){
            if (!varBind){
                varBind = new hkbVariableBindingSet(file);
                parent->variableBindingSet = HkxSharedPtr(varBind);
            }
            if (isProperty){
                if (!varBind->addBinding(path, variableName, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY)){
                    (qFatal("EvaluateExpressionModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }else{
                if (!varBind->addBinding(path, variableName, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    (qFatal("EvaluateExpressionModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            file->setIsChanged(true);
        }else{
            (qWarning("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!"));
        }
    }else{
        (qFatal("BSBoneUI::setBinding(): The data is NULL!!"));
    }
    return true;
}

void BSBoneUI::setBindingVariable(int index, const QString & name){
    if (bsData){
        bool isProperty = false;
        int row = table->currentRow();
        switch (row){
        case INDEX_ROW:
            if (table->item(INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, parameterName+QString::number(bsBoneIndex)+"/index", VARIABLE_TYPE_INT32, isProperty);
            break;
        case FWD_AXIS_LS_ROW:
            if (table->item(FWD_AXIS_LS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, parameterName+QString::number(bsBoneIndex)+"/fwdAxisLS", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case LIMIT_ANGLE_DEGREES_ROW:
            if (table->item(LIMIT_ANGLE_DEGREES_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, parameterName+QString::number(bsBoneIndex)+"/limitAngleDegrees", VARIABLE_TYPE_REAL, isProperty);
            break;
        case ON_GAIN_ROW:
            if (table->item(ON_GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, parameterName+QString::number(bsBoneIndex)+"/onGain", VARIABLE_TYPE_REAL, isProperty);
            break;
        case OFF_GAIN_ROW:
            if (table->item(OFF_GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, parameterName+QString::number(bsBoneIndex)+"/offGain", VARIABLE_TYPE_REAL, isProperty);
            break;
        case ENABLED_ROW:
            if (table->item(ENABLED_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, parameterName+QString::number(bsBoneIndex)+"/enabled", VARIABLE_TYPE_BOOL, isProperty);
            break;
        default:
            return;
        }
        file->setIsChanged(true);
    }else{
        (qFatal("BSBoneUI::setBindingVariable(): The data is NULL!!"));
    }
}

void BSBoneUI::setIndex(int index){
    if (bsData && file){
        bsData->index = index - 1;
        file->setIsChanged(true);
    }else{
        (qFatal("BSBoneUI::setindex(): Behavior file or event data is null!!!"));
    }
}

void BSBoneUI::setFwdAxisLS(){
    if (bsData && file){
        if (bsData->fwdAxisLS != fwdAxisLS->value()){
            bsData->fwdAxisLS = fwdAxisLS->value();
            file->setIsChanged(true);
        }
    }else{
        (qFatal("BSBoneUI::setfwdAxisLS(): Behavior file or event data is null!!!"));
    }
}

void BSBoneUI::setLimitAngleDegrees(){
    if (bsData && file){
        if (bsData->limitAngleDegrees != limitAngleDegrees->value()){
            bsData->limitAngleDegrees = limitAngleDegrees->value();
            file->setIsChanged(true);
        }
    }else{
        (qFatal("BSBoneUI::setLimitAngleDegrees(): Behavior file or event data is null!!!"));
    }
}

void BSBoneUI::setOnGain(){
    if (bsData && file){
        if (bsData->onGain != onGain->value()){
            bsData->onGain = onGain->value();
            file->setIsChanged(true);
        }
    }else{
        (qFatal("BSBoneUI::setOnGain(): Behavior file or event data is null!!!"));
    }
}

void BSBoneUI::setOffGain(){
    if (bsData && file){
        if (bsData->offGain != offGain->value()){
            bsData->offGain = offGain->value();
            file->setIsChanged(true);
        }
    }else{
        (qFatal("BSBoneUI::setOffGain(): Behavior file or event data is null!!!"));
    }
}

void BSBoneUI::setEnabled(){
    if (bsData && file){
        if (bsData->enabled != enabled->isChecked()){
            bsData->enabled = enabled->isChecked();
            file->setIsChanged(true);
        }
    }else{
        (qFatal("BSBoneUI::setEnabled(): Behavior file or event data is null!!!"));
    }
}

void BSBoneUI::viewSelectedChild(int row, int column){
    if (bsData){
        bool properties = false;
        if (column == BINDING_COLUMN){
            switch (row){
            case INDEX_ROW:
                if (table->item(INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, parameterName+QString::number(bsBoneIndex)+"/index");
                break;
            case FWD_AXIS_LS_ROW:
                if (table->item(FWD_AXIS_LS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, parameterName+QString::number(bsBoneIndex)+"/fwdAxisLS");
                break;
            case LIMIT_ANGLE_DEGREES_ROW:
                if (table->item(LIMIT_ANGLE_DEGREES_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, parameterName+QString::number(bsBoneIndex)+"/limitAngleDegrees");
                break;
            case ON_GAIN_ROW:
                if (table->item(ON_GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, parameterName+QString::number(bsBoneIndex)+"/onGain");
                break;
            case OFF_GAIN_ROW:
                if (table->item(OFF_GAIN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, parameterName+QString::number(bsBoneIndex)+"/offGain");
                break;
            case ENABLED_ROW:
                if (table->item(ENABLED_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, parameterName+QString::number(bsBoneIndex)+"/enabled");
                break;
            }
        }
    }else{
        (qFatal("BSBoneUI::viewSelectedChild(): The data is NULL!!"));
    }
}

void BSBoneUI::selectTableToView(bool viewproperties, const QString & path){
    if (bsData){
        if (viewproperties){
            if (parent->variableBindingSet.data()){
                emit viewProperties(static_cast<hkbVariableBindingSet *>(parent->variableBindingSet.data())->getVariableIndexOfBinding(path) + 1);
            }else{
                emit viewProperties(0);
            }
        }else{
            if (parent->variableBindingSet.data()){
                emit viewVariables(static_cast<hkbVariableBindingSet *>(parent->variableBindingSet.data())->getVariableIndexOfBinding(path) + 1);
            }else{
                emit viewVariables(0);
            }
        }
    }else{
        (qFatal("BSBoneUI::selectTableToView(): The data is NULL!!"));
    }
}

void BSBoneUI::variableRenamed(const QString & name, int index){
    int bindIndex = -1;
    hkbVariableBindingSet *bind = NULL;
    if (name == ""){
        (qWarning("BSBoneUI::variableRenamed(): The new variable name is the empty string!!"));
    }
    if (bsData){
        //index--;
        bind = static_cast<hkbVariableBindingSet *>(parent->variableBindingSet.data());
        if (bind){
            bindIndex = bind->getVariableIndexOfBinding(parameterName+QString::number(bsBoneIndex)+"/index");
            if (bindIndex == index){
                table->item(INDEX_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding(parameterName+QString::number(bsBoneIndex)+"/fwdAxisLS");
            if (bindIndex == index){
                table->item(FWD_AXIS_LS_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding(parameterName+QString::number(bsBoneIndex)+"/limitAngleDegrees");
            if (bindIndex == index){
                table->item(LIMIT_ANGLE_DEGREES_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding(parameterName+QString::number(bsBoneIndex)+"/onGain");
            if (bindIndex == index){
                table->item(ON_GAIN_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding(parameterName+QString::number(bsBoneIndex)+"/offGain");
            if (bindIndex == index){
                table->item(OFF_GAIN_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding(parameterName+QString::number(bsBoneIndex)+"/enabled");
            if (bindIndex == index){
                table->item(ENABLED_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        (qFatal("BSBoneUI::variableRenamed(): The data is NULL!!"));
    }
}
