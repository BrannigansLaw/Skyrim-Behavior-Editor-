#include "bsmodifyoncemodifierui.h"

#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/hkxclasses/behavior/modifiers/BSModifyOnceModifier.h"
#include "src/hkxclasses/behavior/modifiers/hkbmodifier.h"
#include "src/ui/genericdatawidgets.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/behaviorgraphview.h"
#include "src/ui/treegraphicsitem.h"
#include "src/ui/genericdatawidgets.h"

#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 4

#define NAME_ROW 0
#define ENABLE_ROW 1
#define ON_ACTIVATE_MODIFIER_ROW 2
#define ON_DEACTIVATE_MODIFIER_ROW 3

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList BSModifyOnceModifierUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

BSModifyOnceModifierUI::BSModifyOnceModifierUI()
    : behaviorView(NULL),
      bsData(NULL),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      enable(new CheckBox)
{
    setTitle("BSModifyOnceModifier");
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
    table->setItem(ON_ACTIVATE_MODIFIER_ROW, NAME_COLUMN, new TableWidgetItem("pOnActivateModifier"));
    table->setItem(ON_ACTIVATE_MODIFIER_ROW, TYPE_COLUMN, new TableWidgetItem("hkbModifier", Qt::AlignCenter));
    table->setItem(ON_ACTIVATE_MODIFIER_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(ON_ACTIVATE_MODIFIER_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_MODIFIERS_TABLE_TIP));
    table->setItem(ON_DEACTIVATE_MODIFIER_ROW, NAME_COLUMN, new TableWidgetItem("pOnDeactivateModifier"));
    table->setItem(ON_DEACTIVATE_MODIFIER_ROW, TYPE_COLUMN, new TableWidgetItem("hkbModifier", Qt::AlignCenter));
    table->setItem(ON_DEACTIVATE_MODIFIER_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(ON_DEACTIVATE_MODIFIER_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_GENERATORS_TABLE_TIP));
    topLyt->addWidget(table, 0, 0, 8, 3);
    setLayout(topLyt);
    connectSignals();
}

void BSModifyOnceModifierUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)), Qt::UniqueConnection);
}

void BSModifyOnceModifierUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)));
}

void BSModifyOnceModifierUI::connectToTables(GenericTableWidget *modifiers, GenericTableWidget *variables, GenericTableWidget *properties){
    if (variables && properties && modifiers){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(modifiers, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(modifiers, SIGNAL(elementSelected(int,QString)), this, SLOT(setModifier(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewModifiers(int)), modifiers, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int)), variables, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int)), properties, SLOT(showTable(int)), Qt::UniqueConnection);
    }else{
        (qFatal("BSModifyOnceModifierUI::connectToTables(): One or more arguments are NULL!!"));
    }
}

void BSModifyOnceModifierUI::loadData(HkxObject *data){
    disconnectSignals();
    if (data){
        if (data->getSignature() == HKB_MODIFIER_GENERATOR){
            bsData = static_cast<BSModifyOnceModifier *>(data);
            name->setText(bsData->name);
            if (bsData->pOnActivateModifier.data()){
                table->item(ON_ACTIVATE_MODIFIER_ROW, VALUE_COLUMN)->setText(static_cast<hkbModifier *>(bsData->pOnActivateModifier.data())->getName());
            }else{
                table->item(ON_ACTIVATE_MODIFIER_ROW, VALUE_COLUMN)->setText("NONE");
            }
            if (bsData->pOnDeactivateModifier.data()){
                table->item(ON_DEACTIVATE_MODIFIER_ROW, VALUE_COLUMN)->setText(static_cast<hkbModifier *>(bsData->pOnDeactivateModifier.data())->getName());
            }else{
                table->item(ON_DEACTIVATE_MODIFIER_ROW, VALUE_COLUMN)->setText("NONE");
            }
        }else{
            (qFatal(QString("BSModifyOnceModifierUI::loadData(): The data passed to the UI is the wrong type!\nSIGNATURE: "+QString::number(data->getSignature(), 16)).toLocal8Bit().data()));
        }
    }else{
        (qFatal("BSModifyOnceModifierUI::loadData(): The data passed to the UI is NULL!!!"));
    }
    connectSignals();
}

void BSModifyOnceModifierUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            bsData->getParentFile()->toggleChanged(true);
            emit modifierNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData));
        }
    }else{
        (qFatal("BSModifyOnceModifierUI::setName(): The data is NULL!!"));
    }
}

void BSModifyOnceModifierUI::setModifier(int index, const QString &name){
    DataIconManager *ptr = NULL;
    int indexOfModifier = -1;
    DataIconManager *mod = NULL;
    int row = -1;
    if (bsData){
        if (behaviorView){
            if (table->currentRow() == ON_ACTIVATE_MODIFIER_ROW){
                mod = static_cast<DataIconManager *>(bsData->pOnActivateModifier.data());
                row = ON_ACTIVATE_MODIFIER_ROW;
            }else if (table->currentRow() == ON_DEACTIVATE_MODIFIER_ROW){
                mod = static_cast<DataIconManager*>(bsData->pOnDeactivateModifier.data());
                row = ON_DEACTIVATE_MODIFIER_ROW;
            }
            ptr = static_cast<BehaviorFile *>(bsData->getParentFile())->getModifierDataAt(index - 1);//Check if the target data is already referenced? Must check if target is clipgen!!!
            indexOfModifier = bsData->getIndexOfObj(mod);
            if (ptr){
                if (name != ptr->getName()){
                    (qFatal("The name of the selected object does not match it's name in the object selection table!!!"));
                    return;
                }else if (ptr == bsData || !behaviorView->reconnectIcon(behaviorView->getSelectedItem(), mod, ptr, false)){
                    (qWarning("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\nYou are attempting to create a circular branch or dead end!!!"));
                    return;
                }
            }else{
                if (behaviorView->getSelectedItem()){
                    behaviorView->removeItemFromGraph(behaviorView->getSelectedItem()->getChildWithData(mod), indexOfModifier);
                }else{
                    (qFatal("BSModifyOnceModifierUI::setGenerator(): The selected icon is NULL!!"));
                    return;
                }
            }
            behaviorView->removeModifierData();
            table->item(row, VALUE_COLUMN)->setText(name);
            bsData->getParentFile()->toggleChanged(true);
        }else{
            (qFatal("BSModifyOnceModifierUI::setGenerator(): The 'behaviorView' pointer is NULL!!"));
        }
    }else{
        (qFatal("BSModifyOnceModifierUI::setGenerator(): The 'bsData' pointer is NULL!!"));
    }
}

void BSModifyOnceModifierUI::selectTableToView(bool viewproperties, const QString & path){
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
        (qFatal("BSModifyOnceModifierUI::selectTableToView(): The data is NULL!!"));
    }
}

void BSModifyOnceModifierUI::setBindingVariable(int index, const QString & name){
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
        default:
            return;
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        (qFatal("BSModifyOnceModifierUI::setBindingVariable(): The data is NULL!!"));
    }
}

bool BSModifyOnceModifierUI::setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty){
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
                    (qFatal("BSModifyOnceModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }else{
                if (!varBind->addBinding(path, variableName, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    (qFatal("BSModifyOnceModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->toggleChanged(true);
        }else{
            (qWarning("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!"));
        }
    }else{
        (qFatal("BSModifyOnceModifierUI::setBinding(): The data is NULL!!"));
    }
    return true;
}

void BSModifyOnceModifierUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
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
            (qFatal("BSModifyOnceModifierUI::loadBinding(): The variable binding set is NULL!!"));
        }
    }else{
        (qFatal("BSModifyOnceModifierUI::loadBinding(): The data is NULL!!"));
    }
}

void BSModifyOnceModifierUI::viewSelected(int row, int column){
    if (bsData){
        if (column == VALUE_COLUMN){
            if (row == ON_ACTIVATE_MODIFIER_ROW){
                emit viewModifiers(static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData->pOnActivateModifier) + 1);
            }else if (row == ON_DEACTIVATE_MODIFIER_ROW){
                emit viewModifiers(static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData->pOnDeactivateModifier) + 1);
            }
        }
    }else{
        (qFatal("BSModifyOnceModifierUI::viewSelected(): The 'bsData' pointer is NULL!!"));
    }
}

void BSModifyOnceModifierUI::variableRenamed(const QString & name, int index){
    if (bsData){
        index--;
        hkbVariableBindingSet *bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (bind){
            int bindIndex = bind->getVariableIndexOfBinding("enable");
            if (bindIndex == index){
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        (qFatal("BSModifyOnceModifierUI::variableRenamed(): The 'bsData' pointer is NULL!!"));
    }
}

void BSModifyOnceModifierUI::modifierRenamed(const QString & name, int index){
    if (bsData){
        index--;
        if (index == static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData->pOnActivateModifier)){
            table->item(ON_ACTIVATE_MODIFIER_ROW, VALUE_COLUMN)->setText(name);
        }
        if (index == static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData->pOnDeactivateModifier)){
            table->item(ON_DEACTIVATE_MODIFIER_ROW, VALUE_COLUMN)->setText(name);
        }
    }else{
        (qFatal("BSModifyOnceModifierUI::modifierRenamed(): The 'bsData' pointer is NULL!!"));
    }
}

void BSModifyOnceModifierUI::setBehaviorView(BehaviorGraphView *view){
    behaviorView = view;
}
