#include "combinetransformsmodifierui.h"

#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/modifiers/hkbCombineTransformsModifier.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/genericdatawidgets.h"

#include <QGridLayout>
#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 11

#define NAME_ROW 0
#define ENABLE_ROW 1
#define TRANSLATION_OUT_ROW 2
#define ROTATION_OUT_ROW 3
#define LEFT_TRANSLATION_ROW 4
#define LEFT_ROTATION_ROW 5
#define RIGHT_TRANSLATION_ROW 6
#define RIGHT_ROTATION_ROW 7
#define INVERT_LEFT_TRANSFORM_ROW 8
#define INVERT_RIGHT_TRANSFORM_ROW 9
#define INVERT_RESULT_ROW 10

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList CombineTransformsModifierUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

CombineTransformsModifierUI::CombineTransformsModifierUI()
    : bsData(NULL),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      enable(new CheckBox),
      translationOut(new QuadVariableWidget),
      rotationOut(new QuadVariableWidget),
      leftTranslation(new QuadVariableWidget),
      leftRotation(new QuadVariableWidget),
      rightTranslation(new QuadVariableWidget),
      rightRotation(new QuadVariableWidget),
      invertLeftTransform(new CheckBox),
      invertRightTransform(new CheckBox),
      invertResult(new CheckBox)
{
    setTitle("hkbCombineTransformsModifier");
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(NAME_ROW, NAME_COLUMN, new TableWidgetItem("name"));
    table->setItem(NAME_ROW, TYPE_COLUMN, new TableWidgetItem("hkStringPtr", Qt::AlignCenter));
    table->setItem(NAME_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(NAME_ROW, VALUE_COLUMN, name);
    table->setItem(ENABLE_ROW, NAME_COLUMN, new TableWidgetItem("enable"));
    table->setItem(ENABLE_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(ENABLE_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ENABLE_ROW, VALUE_COLUMN, enable);
    table->setItem(TRANSLATION_OUT_ROW, NAME_COLUMN, new TableWidgetItem("translationOut"));
    table->setItem(TRANSLATION_OUT_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(TRANSLATION_OUT_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(TRANSLATION_OUT_ROW, VALUE_COLUMN, translationOut);
    table->setItem(ROTATION_OUT_ROW, NAME_COLUMN, new TableWidgetItem("rotationOut"));
    table->setItem(ROTATION_OUT_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(ROTATION_OUT_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ROTATION_OUT_ROW, VALUE_COLUMN, rotationOut);
    table->setItem(LEFT_TRANSLATION_ROW, NAME_COLUMN, new TableWidgetItem("leftTranslation"));
    table->setItem(LEFT_TRANSLATION_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(LEFT_TRANSLATION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(LEFT_TRANSLATION_ROW, VALUE_COLUMN, leftTranslation);
    table->setItem(LEFT_ROTATION_ROW, NAME_COLUMN, new TableWidgetItem("leftRotation"));
    table->setItem(LEFT_ROTATION_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(LEFT_ROTATION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(LEFT_ROTATION_ROW, VALUE_COLUMN, leftRotation);
    table->setItem(RIGHT_TRANSLATION_ROW, NAME_COLUMN, new TableWidgetItem("rightTranslation"));
    table->setItem(RIGHT_TRANSLATION_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(RIGHT_TRANSLATION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(RIGHT_TRANSLATION_ROW, VALUE_COLUMN, rightTranslation);
    table->setItem(RIGHT_ROTATION_ROW, NAME_COLUMN, new TableWidgetItem("rightRotation"));
    table->setItem(RIGHT_ROTATION_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(RIGHT_ROTATION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(RIGHT_ROTATION_ROW, VALUE_COLUMN, rightRotation);
    table->setItem(INVERT_LEFT_TRANSFORM_ROW, NAME_COLUMN, new TableWidgetItem("invertLeftTransform"));
    table->setItem(INVERT_LEFT_TRANSFORM_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(INVERT_LEFT_TRANSFORM_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(INVERT_LEFT_TRANSFORM_ROW, VALUE_COLUMN, invertLeftTransform);
    table->setItem(INVERT_RIGHT_TRANSFORM_ROW, NAME_COLUMN, new TableWidgetItem("invertRightTransform"));
    table->setItem(INVERT_RIGHT_TRANSFORM_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(INVERT_RIGHT_TRANSFORM_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(INVERT_RIGHT_TRANSFORM_ROW, VALUE_COLUMN, invertRightTransform);
    table->setItem(INVERT_RESULT_ROW, NAME_COLUMN, new TableWidgetItem("invertResult"));
    table->setItem(INVERT_RESULT_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(INVERT_RESULT_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(INVERT_RESULT_ROW, VALUE_COLUMN, invertResult);
    topLyt->addWidget(table, 0, 0, 8, 3);
    setLayout(topLyt);
}

void CombineTransformsModifierUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
    connect(translationOut, SIGNAL(editingFinished()), this, SLOT(setTranslationOut()), Qt::UniqueConnection);
    connect(rotationOut, SIGNAL(editingFinished()), this, SLOT(setRotationOut()), Qt::UniqueConnection);
    connect(leftTranslation, SIGNAL(editingFinished()), this, SLOT(setLeftTranslation()), Qt::UniqueConnection);
    connect(leftRotation, SIGNAL(editingFinished()), this, SLOT(setLeftRotation()), Qt::UniqueConnection);
    connect(rightTranslation, SIGNAL(editingFinished()), this, SLOT(setRightTranslation()), Qt::UniqueConnection);
    connect(rightRotation, SIGNAL(editingFinished()), this, SLOT(setRightRotation()), Qt::UniqueConnection);
    connect(invertLeftTransform, SIGNAL(released()), this, SLOT(setInvertLeftTransform()), Qt::UniqueConnection);
    connect(invertRightTransform, SIGNAL(released()), this, SLOT(setInvertRightTransform()), Qt::UniqueConnection);
    connect(invertResult, SIGNAL(released()), this, SLOT(setInvertResult()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)), Qt::UniqueConnection);
}

void CombineTransformsModifierUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    disconnect(translationOut, SIGNAL(editingFinished()), this, SLOT(setTranslationOut()));
    disconnect(rotationOut, SIGNAL(editingFinished()), this, SLOT(setRotationOut()));
    disconnect(leftTranslation, SIGNAL(editingFinished()), this, SLOT(setLeftTranslation()));
    disconnect(leftRotation, SIGNAL(editingFinished()), this, SLOT(setLeftRotation()));
    disconnect(rightTranslation, SIGNAL(editingFinished()), this, SLOT(setRightTranslation()));
    disconnect(rightRotation, SIGNAL(editingFinished()), this, SLOT(setRightRotation()));
    disconnect(invertLeftTransform, SIGNAL(released()), this, SLOT(setInvertLeftTransform()));
    disconnect(invertRightTransform, SIGNAL(released()), this, SLOT(setInvertRightTransform()));
    disconnect(invertResult, SIGNAL(released()), this, SLOT(setInvertResult()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)));
}

void CombineTransformsModifierUI::connectToTables(GenericTableWidget *variables, GenericTableWidget *properties){
    if (variables && properties){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int)), variables, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int)), properties, SLOT(showTable(int)), Qt::UniqueConnection);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("CombineTransformsModifierUI::connectToTables(): One or more arguments are NULL!!"))
    }
}

void CombineTransformsModifierUI::loadData(HkxObject *data){
    disconnectSignals();
    if (data){
        if (data->getSignature() == HKB_COMBINE_TRANSFORMS_MODIFIER){
            hkbVariableBindingSet *varBind = NULL;
            bsData = static_cast<hkbCombineTransformsModifier *>(data);
            name->setText(bsData->name);
            enable->setChecked(bsData->enable);
            translationOut->setValue(bsData->translationOut);
            rotationOut->setValue(bsData->rotationOut);
            leftTranslation->setValue(bsData->leftTranslation);
            leftRotation->setValue(bsData->leftRotation);
            rightTranslation->setValue(bsData->rightTranslation);
            rightRotation->setValue(bsData->rightRotation);
            invertLeftTransform->setChecked(bsData->invertLeftTransform);
            invertRightTransform->setChecked(bsData->invertRightTransform);
            invertResult->setChecked(bsData->invertResult);
            varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (varBind){
                loadBinding(ENABLE_ROW, BINDING_COLUMN, varBind, "enable");
                loadBinding(TRANSLATION_OUT_ROW, BINDING_COLUMN, varBind, "translationOut");
                loadBinding(ROTATION_OUT_ROW, BINDING_COLUMN, varBind, "rotationOut");
                loadBinding(LEFT_TRANSLATION_ROW, BINDING_COLUMN, varBind, "leftTranslation");
                loadBinding(LEFT_ROTATION_ROW, BINDING_COLUMN, varBind, "leftRotation");
                loadBinding(RIGHT_TRANSLATION_ROW, BINDING_COLUMN, varBind, "rightTranslation");
                loadBinding(RIGHT_ROTATION_ROW, BINDING_COLUMN, varBind, "rightRotation");
                loadBinding(INVERT_LEFT_TRANSFORM_ROW, BINDING_COLUMN, varBind, "invertLeftTransform");
                loadBinding(INVERT_RIGHT_TRANSFORM_ROW, BINDING_COLUMN, varBind, "invertRightTransform");
                loadBinding(INVERT_RESULT_ROW, BINDING_COLUMN, varBind, "invertResult");
            }else{
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(TRANSLATION_OUT_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(ROTATION_OUT_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(LEFT_TRANSLATION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(LEFT_ROTATION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(RIGHT_TRANSLATION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(RIGHT_ROTATION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(INVERT_LEFT_TRANSFORM_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(INVERT_RIGHT_TRANSFORM_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(INVERT_RESULT_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
        }else{
            CRITICAL_ERROR_MESSAGE(QString("CombineTransformsModifierUI::loadData(): The data is an incorrect type!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("CombineTransformsModifierUI::loadData(): The data is NULL!!"));
    }
    connectSignals();
}

void CombineTransformsModifierUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            bsData->getParentFile()->toggleChanged(true);
            emit modifierNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("CombineTransformsModifierUI::setName(): The data is NULL!!"));
    }
}

void CombineTransformsModifierUI::setEnable(){
    if (bsData){
        if (bsData->enable != enable->isChecked()){
            bsData->enable = enable->isChecked();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("CombineTransformsModifierUI::setEnable(): The data is NULL!!"));
    }
}

void CombineTransformsModifierUI::setTranslationOut(){
    if (bsData){
        if (bsData->translationOut != translationOut->value()){
            bsData->translationOut = translationOut->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("CombineTransformsModifierUI::setTranslationOut(): The data is NULL!!"));
    }
}

void CombineTransformsModifierUI::setRotationOut(){
    if (bsData){
        if (bsData->rotationOut != rotationOut->value()){
            bsData->rotationOut = rotationOut->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("CombineTransformsModifierUI::setRotationOut(): The data is NULL!!"));
    }
}

void CombineTransformsModifierUI::setLeftTranslation(){
    if (bsData){
        if (bsData->leftTranslation != leftTranslation->value()){
            bsData->leftTranslation = leftTranslation->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("CombineTransformsModifierUI::setLeftTranslation(): The data is NULL!!"));
    }
}

void CombineTransformsModifierUI::setLeftRotation(){
    if (bsData){
        if (bsData->leftRotation != leftRotation->value()){
            bsData->leftRotation = leftRotation->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("CombineTransformsModifierUI::setLeftRotation(): The data is NULL!!"));
    }
}

void CombineTransformsModifierUI::setRightTranslation(){
    if (bsData){
        if (bsData->rightTranslation != rightTranslation->value()){
            bsData->rightTranslation = rightTranslation->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("CombineTransformsModifierUI::setRightTranslation(): The data is NULL!!"));
    }
}

void CombineTransformsModifierUI::setRightRotation(){
    if (bsData){
        if (bsData->rightRotation != rightRotation->value()){
            bsData->rightRotation = rightRotation->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("CombineTransformsModifierUI::setRightRotation(): The data is NULL!!"));
    }
}

void CombineTransformsModifierUI::setInvertLeftTransform(){
    if (bsData){
        if (bsData->invertLeftTransform != invertLeftTransform->isChecked()){
            bsData->invertLeftTransform = invertLeftTransform->isChecked();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("CombineTransformsModifierUI::setInvertLeftTransform(): The data is NULL!!"));
    }
}

void CombineTransformsModifierUI::setInvertRightTransform(){
    if (bsData){
        if (bsData->invertRightTransform != invertRightTransform->isChecked()){
            bsData->invertRightTransform = invertRightTransform->isChecked();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("CombineTransformsModifierUI::setInvertRightTransform(): The data is NULL!!"));
    }
}

void CombineTransformsModifierUI::setInvertResult(){
    if (bsData){
        if (bsData->invertResult != invertResult->isChecked()){
            bsData->invertResult = invertResult->isChecked();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("CombineTransformsModifierUI::setInvertResult(): The data is NULL!!"));
    }
}

void CombineTransformsModifierUI::viewSelected(int row, int column){
    if (bsData){
        bool isProperty = false;
        if (column == BINDING_COLUMN){
            switch (row){
            case ENABLE_ROW:
                if (table->item(ENABLE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "enable");
                break;
            case TRANSLATION_OUT_ROW:
                if (table->item(TRANSLATION_OUT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "translationOut");
                break;
            case ROTATION_OUT_ROW:
                if (table->item(ROTATION_OUT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "rotationOut");
                break;
            case LEFT_TRANSLATION_ROW:
                if (table->item(LEFT_TRANSLATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "leftTranslation");
                break;
            case LEFT_ROTATION_ROW:
                if (table->item(LEFT_ROTATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "leftRotation");
                break;
            case RIGHT_TRANSLATION_ROW:
                if (table->item(RIGHT_TRANSLATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "rightTranslation");
                break;
            case RIGHT_ROTATION_ROW:
                if (table->item(RIGHT_ROTATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "rightRotation");
                break;
            case INVERT_LEFT_TRANSFORM_ROW:
                if (table->item(INVERT_LEFT_TRANSFORM_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "invertLeftTransform");
                break;
            case INVERT_RIGHT_TRANSFORM_ROW:
                if (table->item(INVERT_RIGHT_TRANSFORM_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "invertRightTransform");
                break;
            case INVERT_RESULT_ROW:
                if (table->item(INVERT_RESULT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "invertResult");
                break;
            default:
                return;
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("CombineTransformsModifierUI::viewSelected(): The 'bsData' pointer is NULL!!"))
    }
}

void CombineTransformsModifierUI::selectTableToView(bool viewisProperty, const QString & path){
    if (bsData){
        if (viewisProperty){
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
        CRITICAL_ERROR_MESSAGE(QString("CombineTransformsModifierUI::selectTableToView(): The data is NULL!!"));
    }
}

void CombineTransformsModifierUI::variableRenamed(const QString & name, int index){
    if (bsData){
        index--;
        hkbVariableBindingSet *bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (bind){
            int bindIndex = bind->getVariableIndexOfBinding("enable");
            if (bindIndex == index){
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("translationOut");
            if (bindIndex == index){
                table->item(TRANSLATION_OUT_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("rotationOut");
            if (bindIndex == index){
                table->item(ROTATION_OUT_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("leftTranslation");
            if (bindIndex == index){
                table->item(LEFT_TRANSLATION_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("leftRotation");
            if (bindIndex == index){
                table->item(LEFT_ROTATION_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("rightTranslation");
            if (bindIndex == index){
                table->item(RIGHT_TRANSLATION_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("rightRotation");
            if (bindIndex == index){
                table->item(RIGHT_ROTATION_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("invertLeftTransform");
            if (bindIndex == index){
                table->item(INVERT_LEFT_TRANSFORM_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("invertRightTransform");
            if (bindIndex == index){
                table->item(INVERT_RIGHT_TRANSFORM_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("invertResult");
            if (bindIndex == index){
                table->item(INVERT_RESULT_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("CombineTransformsModifierUI::variableRenamed(): The 'bsData' pointer is NULL!!"))
    }
}

bool CombineTransformsModifierUI::setBinding(int index, int row, const QString &variableName, const QString &path, hkVariableType type, bool isProperty){
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
                if (!varBind->addBinding(path, variableName, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY)){
                    CRITICAL_ERROR_MESSAGE(QString("CombineTransformsModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }else{
                if (!varBind->addBinding(path, variableName, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    CRITICAL_ERROR_MESSAGE(QString("CombineTransformsModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->toggleChanged(true);
        }else{
            WARNING_MESSAGE(QString("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("CombineTransformsModifierUI::setBinding(): The data is NULL!!"));
    }
    return true;
}

void CombineTransformsModifierUI::setBindingVariable(int index, const QString &name){
    if (bsData){
        bool isProperty = false;
        int row = table->currentRow();
        switch (row){
        case ENABLE_ROW:
            if (table->item(ENABLE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "enable", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case TRANSLATION_OUT_ROW:
            if (table->item(TRANSLATION_OUT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "translationOut", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case ROTATION_OUT_ROW:
            if (table->item(ROTATION_OUT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "rotationOut", VARIABLE_TYPE_QUATERNION, isProperty);
            break;
        case LEFT_TRANSLATION_ROW:
            if (table->item(LEFT_TRANSLATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "leftTranslation", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case LEFT_ROTATION_ROW:
            if (table->item(LEFT_ROTATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "leftRotation", VARIABLE_TYPE_QUATERNION, isProperty);
            break;
        case RIGHT_TRANSLATION_ROW:
            if (table->item(RIGHT_TRANSLATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "rightTranslation", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case RIGHT_ROTATION_ROW:
            if (table->item(RIGHT_ROTATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "rightRotation", VARIABLE_TYPE_QUATERNION, isProperty);
            break;
        case INVERT_LEFT_TRANSFORM_ROW:
            if (table->item(INVERT_LEFT_TRANSFORM_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "invertLeftTransform", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case INVERT_RIGHT_TRANSFORM_ROW:
            if (table->item(INVERT_RIGHT_TRANSFORM_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "invertRightTransform", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case INVERT_RESULT_ROW:
            if (table->item(INVERT_RESULT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "invertResult", VARIABLE_TYPE_BOOL, isProperty);
            break;
        default:
            return;
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("CombineTransformsModifierUI::setBindingVariable(): The data is NULL!!"));
    }
}

void CombineTransformsModifierUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
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
            CRITICAL_ERROR_MESSAGE(QString("CombineTransformsModifierUI::loadBinding(): The variable binding set is NULL!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("CombineTransformsModifierUI::loadBinding(): The data is NULL!!"));
    }
}
