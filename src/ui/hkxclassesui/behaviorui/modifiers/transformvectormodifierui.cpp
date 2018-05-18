#include "transformvectormodifierui.h"

#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/modifiers/hkbTransformVectorModifier.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/genericdatawidgets.h"

#include <QGridLayout>
#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 10

#define NAME_ROW 0
#define ENABLE_ROW 1
#define ROTATION_ROW 2
#define TRANSLATION_ROW 3
#define VECTOR_IN_ROW 4
#define VECTOR_OUT_ROW 5
#define ROTATE_ONLY_ROW 6
#define INVERSE_ROW 7
#define COMPUTE_ON_ACTIVATE_ROW 8
#define COMPUTE_ON_MODIFY_ROW 9

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList TransformVectorModifierUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

TransformVectorModifierUI::TransformVectorModifierUI()
    : bsData(nullptr),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      enable(new CheckBox),
      rotation(new QuadVariableWidget),
      translation(new QuadVariableWidget),
      vectorIn(new QuadVariableWidget),
      vectorOut(new QuadVariableWidget),
      rotateOnly(new CheckBox),
      inverse(new CheckBox),
      computeOnActivate(new CheckBox),
      computeOnModify(new CheckBox)
{
    setTitle("hkbTransformVectorModifier");
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
    table->setItem(ROTATION_ROW, NAME_COLUMN, new TableWidgetItem("rotation"));
    table->setItem(ROTATION_ROW, TYPE_COLUMN, new TableWidgetItem("hkQuaternion", Qt::AlignCenter));
    table->setItem(ROTATION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ROTATION_ROW, VALUE_COLUMN, rotation);
    table->setItem(TRANSLATION_ROW, NAME_COLUMN, new TableWidgetItem("translation"));
    table->setItem(TRANSLATION_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(TRANSLATION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(TRANSLATION_ROW, VALUE_COLUMN, translation);
    table->setItem(VECTOR_IN_ROW, NAME_COLUMN, new TableWidgetItem("vectorIn"));
    table->setItem(VECTOR_IN_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(VECTOR_IN_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(VECTOR_IN_ROW, VALUE_COLUMN, vectorIn);
    table->setItem(VECTOR_OUT_ROW, NAME_COLUMN, new TableWidgetItem("vectorOut"));
    table->setItem(VECTOR_OUT_ROW, TYPE_COLUMN, new TableWidgetItem("hkVector4", Qt::AlignCenter));
    table->setItem(VECTOR_OUT_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(VECTOR_OUT_ROW, VALUE_COLUMN, vectorOut);
    table->setItem(ROTATE_ONLY_ROW, NAME_COLUMN, new TableWidgetItem("rotateOnly"));
    table->setItem(ROTATE_ONLY_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(ROTATE_ONLY_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ROTATE_ONLY_ROW, VALUE_COLUMN, rotateOnly);
    table->setItem(INVERSE_ROW, NAME_COLUMN, new TableWidgetItem("inverse"));
    table->setItem(INVERSE_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(INVERSE_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(INVERSE_ROW, VALUE_COLUMN, inverse);
    table->setItem(COMPUTE_ON_ACTIVATE_ROW, NAME_COLUMN, new TableWidgetItem("computeOnActivate"));
    table->setItem(COMPUTE_ON_ACTIVATE_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(COMPUTE_ON_ACTIVATE_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(COMPUTE_ON_ACTIVATE_ROW, VALUE_COLUMN, computeOnActivate);
    table->setItem(COMPUTE_ON_MODIFY_ROW, NAME_COLUMN, new TableWidgetItem("computeOnModify"));
    table->setItem(COMPUTE_ON_MODIFY_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(COMPUTE_ON_MODIFY_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(COMPUTE_ON_MODIFY_ROW, VALUE_COLUMN, computeOnModify);
    topLyt->addWidget(table, 0, 0, 8, 3);
    setLayout(topLyt);
}

void TransformVectorModifierUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
    connect(rotation, SIGNAL(editingFinished()), this, SLOT(setRotation()), Qt::UniqueConnection);
    connect(translation, SIGNAL(editingFinished()), this, SLOT(setTranslation()), Qt::UniqueConnection);
    connect(vectorIn, SIGNAL(editingFinished()), this, SLOT(setVectorIn()), Qt::UniqueConnection);
    connect(vectorOut, SIGNAL(editingFinished()), this, SLOT(setVectorOut()), Qt::UniqueConnection);
    connect(rotateOnly, SIGNAL(released()), this, SLOT(setRotateOnly()), Qt::UniqueConnection);
    connect(inverse, SIGNAL(released()), this, SLOT(setInverse()), Qt::UniqueConnection);
    connect(computeOnActivate, SIGNAL(released()), this, SLOT(setComputeOnActivate()), Qt::UniqueConnection);
    connect(computeOnModify, SIGNAL(released()), this, SLOT(setComputeOnModify()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)), Qt::UniqueConnection);
}

void TransformVectorModifierUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    disconnect(rotation, SIGNAL(editingFinished()), this, SLOT(setRotation()));
    disconnect(translation, SIGNAL(editingFinished()), this, SLOT(setTranslation()));
    disconnect(vectorIn, SIGNAL(editingFinished()), this, SLOT(setVectorIn()));
    disconnect(vectorOut, SIGNAL(editingFinished()), this, SLOT(setVectorOut()));
    disconnect(rotateOnly, SIGNAL(released()), this, SLOT(setRotateOnly()));
    disconnect(inverse, SIGNAL(released()), this, SLOT(setInverse()));
    disconnect(computeOnActivate, SIGNAL(released()), this, SLOT(setComputeOnActivate()));
    disconnect(computeOnModify, SIGNAL(released()), this, SLOT(setComputeOnModify()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)));
}

void TransformVectorModifierUI::connectToTables(GenericTableWidget *variables, GenericTableWidget *properties){
    if (variables && properties){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int,QString,QStringList)), variables, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int,QString,QStringList)), properties, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
    }else{
        CRITICAL_ERROR_MESSAGE("TransformVectorModifierUI::connectToTables(): One or more arguments are nullptr!!");
    }
}

void TransformVectorModifierUI::loadData(HkxObject *data){
    disconnectSignals();
    if (data){
        if (data->getSignature() == HKB_TRANSFORM_VECTOR_MODIFIER){
            hkbVariableBindingSet *varBind = nullptr;
            bsData = static_cast<hkbTransformVectorModifier *>(data);
            name->setText(bsData->name);
            enable->setChecked(bsData->enable);
            rotation->setValue(bsData->rotation);
            translation->setValue(bsData->translation);
            vectorIn->setValue(bsData->vectorIn);
            vectorOut->setValue(bsData->vectorOut);
            rotateOnly->setChecked(bsData->rotateOnly);
            inverse->setChecked(bsData->inverse);
            computeOnActivate->setChecked(bsData->computeOnActivate);
            computeOnModify->setChecked(bsData->computeOnModify);
            varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (varBind){
                loadBinding(ENABLE_ROW, BINDING_COLUMN, varBind, "enable");
                loadBinding(ROTATION_ROW, BINDING_COLUMN, varBind, "rotation");
                loadBinding(TRANSLATION_ROW, BINDING_COLUMN, varBind, "translation");
                loadBinding(VECTOR_IN_ROW, BINDING_COLUMN, varBind, "vectorIn");
                loadBinding(VECTOR_OUT_ROW, BINDING_COLUMN, varBind, "vectorOut");
                loadBinding(ROTATE_ONLY_ROW, BINDING_COLUMN, varBind, "rotateOnly");
                loadBinding(INVERSE_ROW, BINDING_COLUMN, varBind, "inverse");
                loadBinding(COMPUTE_ON_ACTIVATE_ROW, BINDING_COLUMN, varBind, "computeOnActivate");
                loadBinding(COMPUTE_ON_MODIFY_ROW, BINDING_COLUMN, varBind, "computeOnModify");
            }else{
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(ROTATION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(TRANSLATION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(VECTOR_IN_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(VECTOR_OUT_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(ROTATE_ONLY_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(INVERSE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(COMPUTE_ON_ACTIVATE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(COMPUTE_ON_MODIFY_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
        }else{
            CRITICAL_ERROR_MESSAGE("TransformVectorModifierUI::loadData(): The data is an incorrect type!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("TransformVectorModifierUI::loadData(): The data is nullptr!!");
    }
    connectSignals();
}

void TransformVectorModifierUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            bsData->getParentFile()->setIsChanged(true);
            emit modifierNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData));
        }
    }else{
        CRITICAL_ERROR_MESSAGE("TransformVectorModifierUI::setName(): The data is nullptr!!");
    }
}

void TransformVectorModifierUI::setEnable(){
    if (bsData){
        bsData->enable = enable->isChecked();
        bsData->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("TransformVectorModifierUI::setEnable(): The data is nullptr!!");
    }
}

void TransformVectorModifierUI::setRotation(){
    if (bsData){
        if (bsData->rotation != rotation->value()){
            bsData->rotation = rotation->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("TransformVectorModifierUI::setrotation(): The data is nullptr!!");
    }
}

void TransformVectorModifierUI::setTranslation(){
    if (bsData){
        if (bsData->translation != translation->value()){
            bsData->translation = translation->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("TransformVectorModifierUI::settranslation(): The data is nullptr!!");
    }
}

void TransformVectorModifierUI::setVectorIn(){
    if (bsData){
        if (bsData->vectorIn != vectorIn->value()){
            bsData->vectorIn = vectorIn->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("TransformVectorModifierUI::setvectorIn(): The data is nullptr!!");
    }
}

void TransformVectorModifierUI::setVectorOut(){
    if (bsData){
        if (bsData->vectorOut != vectorOut->value()){
            bsData->vectorOut = vectorOut->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("TransformVectorModifierUI::setvectorOut(): The data is nullptr!!");
    }
}

void TransformVectorModifierUI::setRotateOnly(){
    if (bsData){
        bsData->rotateOnly = rotateOnly->isChecked();
        bsData->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("TransformVectorModifierUI::setrotateOnly(): The data is nullptr!!");
    }
}

void TransformVectorModifierUI::setInverse(){
    if (bsData){
        bsData->inverse = inverse->isChecked();
        bsData->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("TransformVectorModifierUI::setinverse(): The data is nullptr!!");
    }
}

void TransformVectorModifierUI::setComputeOnActivate(){
    if (bsData){
        bsData->computeOnActivate = computeOnActivate->isChecked();
        bsData->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("TransformVectorModifierUI::setcomputeOnActivate(): The data is nullptr!!");
    }
}

void TransformVectorModifierUI::setComputeOnModify(){
    if (bsData){
        bsData->computeOnModify = computeOnModify->isChecked();
        bsData->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("TransformVectorModifierUI::setcomputeOnModify(): The data is nullptr!!");
    }
}

void TransformVectorModifierUI::viewSelected(int row, int column){
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
            case ROTATION_ROW:
                if (table->item(ROTATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "rotation");
                break;
            case TRANSLATION_ROW:
                if (table->item(TRANSLATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "translation");
                break;
            case VECTOR_IN_ROW:
                if (table->item(VECTOR_IN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "vectorIn");
                break;
            case VECTOR_OUT_ROW:
                if (table->item(VECTOR_OUT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "vectorOut");
                break;
            case ROTATE_ONLY_ROW:
                if (table->item(ROTATE_ONLY_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "rotateOnly");
                break;
            case INVERSE_ROW:
                if (table->item(INVERSE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "inverse");
                break;
            case COMPUTE_ON_ACTIVATE_ROW:
                if (table->item(COMPUTE_ON_ACTIVATE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "computeOnActivate");
                break;
            case COMPUTE_ON_MODIFY_ROW:
                if (table->item(COMPUTE_ON_MODIFY_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "computeOnModify");
                break;
            default:
                return;
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE("TransformVectorModifierUI::viewSelected(): The 'bsData' pointer is nullptr!!");
    }
}

void TransformVectorModifierUI::selectTableToView(bool viewisProperty, const QString & path){
    if (bsData){
        if (viewisProperty){
            if (bsData->variableBindingSet.data()){
                emit viewProperties(static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data())->getVariableIndexOfBinding(path) + 1, QString(), QStringList());
            }else{
                emit viewProperties(0, QString(), QStringList());
            }
        }else{
            if (bsData->variableBindingSet.data()){
                emit viewVariables(static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data())->getVariableIndexOfBinding(path) + 1, QString(), QStringList());
            }else{
                emit viewVariables(0, QString(), QStringList());
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE("TransformVectorModifierUI::selectTableToView(): The data is nullptr!!");
    }
}

void TransformVectorModifierUI::variableRenamed(const QString & name, int index){
    if (bsData){
        index--;
        hkbVariableBindingSet *bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (bind){
            int bindIndex = bind->getVariableIndexOfBinding("enable");
            if (bindIndex == index){
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("rotation");
            if (bindIndex == index){
                table->item(ROTATION_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("translation");
            if (bindIndex == index){
                table->item(TRANSLATION_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("vectorIn");
            if (bindIndex == index){
                table->item(VECTOR_IN_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("vectorOut");
            if (bindIndex == index){
                table->item(VECTOR_OUT_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("rotateOnly");
            if (bindIndex == index){
                table->item(ROTATE_ONLY_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("inverse");
            if (bindIndex == index){
                table->item(INVERSE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("computeOnActivate");
            if (bindIndex == index){
                table->item(COMPUTE_ON_ACTIVATE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("computeOnModify");
            if (bindIndex == index){
                table->item(COMPUTE_ON_MODIFY_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE("TransformVectorModifierUI::variableRenamed(): The 'bsData' pointer is nullptr!!");
    }
}

bool TransformVectorModifierUI::setBinding(int index, int row, const QString &variableName, const QString &path, hkVariableType type, bool isProperty){
    hkbVariableBindingSet *varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
    if (bsData){
        if (index == 0){
            varBind->removeBinding(path);if (varBind->getNumberOfBindings() == 0){static_cast<HkDynamicObject *>(bsData)->variableBindingSet = HkxSharedPtr(); static_cast<BehaviorFile *>(bsData->getParentFile())->removeOtherData();}
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
        }else if ((!isProperty && areVariableTypesCompatible(static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableTypeAt(index - 1), type)) ||
                  (isProperty && areVariableTypesCompatible(static_cast<BehaviorFile *>(bsData->getParentFile())->getCharacterPropertyTypeAt(index - 1), type))){
            if (!varBind){
                varBind = new hkbVariableBindingSet(bsData->getParentFile());
                bsData->variableBindingSet = HkxSharedPtr(varBind);
            }
            if (isProperty){
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY)){
                    CRITICAL_ERROR_MESSAGE("TransformVectorModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }else{
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    CRITICAL_ERROR_MESSAGE("TransformVectorModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->setIsChanged(true);
        }else{
            WARNING_MESSAGE("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("TransformVectorModifierUI::setBinding(): The data is nullptr!!");
    }
    return true;
}

void TransformVectorModifierUI::setBindingVariable(int index, const QString &name){
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
        case ROTATION_ROW:
            if (table->item(ROTATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "rotation", VARIABLE_TYPE_QUATERNION, isProperty);
            break;
        case TRANSLATION_ROW:
            if (table->item(TRANSLATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "translation", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case VECTOR_IN_ROW:
            if (table->item(VECTOR_IN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "vectorIn", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case VECTOR_OUT_ROW:
            if (table->item(VECTOR_OUT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "vectorOut", VARIABLE_TYPE_VECTOR4, isProperty);
            break;
        case ROTATE_ONLY_ROW:
            if (table->item(ROTATE_ONLY_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "rotateOnly", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case INVERSE_ROW:
            if (table->item(INVERSE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "inverse", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case COMPUTE_ON_ACTIVATE_ROW:
            if (table->item(COMPUTE_ON_ACTIVATE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "computeOnActivate", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case COMPUTE_ON_MODIFY_ROW:
            if (table->item(COMPUTE_ON_MODIFY_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "computeOnModify", VARIABLE_TYPE_BOOL, isProperty);
            break;
        default:
            return;
        }
        bsData->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("TransformVectorModifierUI::setBindingVariable(): The data is nullptr!!");
    }
}

void TransformVectorModifierUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
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
            }
            if (varName == ""){
                varName = "NONE";
            }
            table->item(row, colunm)->setText(BINDING_ITEM_LABEL+varName);
        }else{
            CRITICAL_ERROR_MESSAGE("TransformVectorModifierUI::loadBinding(): The variable binding set is nullptr!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("TransformVectorModifierUI::loadBinding(): The data is nullptr!!");
    }
}
