#include "bsisactivemodifierui.h"

#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/modifiers/bsisactivemodifier.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/genericdatawidgets.h"

#include <QGridLayout>
#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 12

#define NAME_ROW 0
#define ENABLE_ROW 1
#define IS_ACTIVE_0_ROW 2
#define INVERT_ACTIVE_0_ROW 3
#define IS_ACTIVE_1_ROW 4
#define INVERT_ACTIVE_1_ROW 5
#define IS_ACTIVE_2_ROW 6
#define INVERT_ACTIVE_2_ROW 7
#define IS_ACTIVE_3_ROW 8
#define INVERT_ACTIVE_3_ROW 9
#define IS_ACTIVE_4_ROW 10
#define INVERT_ACTIVE_4_ROW 11

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList BSIsActiveModifierUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

BSIsActiveModifierUI::BSIsActiveModifierUI()
    : bsData(nullptr),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      enable(new CheckBox),
      bIsActive0(new CheckBox),
      bInvertActive0(new CheckBox),
      bIsActive1(new CheckBox),
      bInvertActive1(new CheckBox),
      bIsActive2(new CheckBox),
      bInvertActive2(new CheckBox),
      bIsActive3(new CheckBox),
      bInvertActive3(new CheckBox),
      bIsActive4(new CheckBox),
      bInvertActive4(new CheckBox)
{
    setTitle("BSIsActiveModifier");
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
    table->setItem(IS_ACTIVE_0_ROW, NAME_COLUMN, new TableWidgetItem("bIsActive0"));
    table->setItem(IS_ACTIVE_0_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(IS_ACTIVE_0_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(IS_ACTIVE_0_ROW, VALUE_COLUMN, bIsActive0);
    table->setItem(INVERT_ACTIVE_0_ROW, NAME_COLUMN, new TableWidgetItem("bInvertActive0"));
    table->setItem(INVERT_ACTIVE_0_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(INVERT_ACTIVE_0_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(INVERT_ACTIVE_0_ROW, VALUE_COLUMN, bInvertActive0);
    table->setItem(IS_ACTIVE_1_ROW, NAME_COLUMN, new TableWidgetItem("bIsActive1"));
    table->setItem(IS_ACTIVE_1_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(IS_ACTIVE_1_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(IS_ACTIVE_1_ROW, VALUE_COLUMN, bIsActive1);
    table->setItem(INVERT_ACTIVE_1_ROW, NAME_COLUMN, new TableWidgetItem("bInvertActive1"));
    table->setItem(INVERT_ACTIVE_1_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(INVERT_ACTIVE_1_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(INVERT_ACTIVE_1_ROW, VALUE_COLUMN, bInvertActive1);
    table->setItem(IS_ACTIVE_2_ROW, NAME_COLUMN, new TableWidgetItem("bIsActive2"));
    table->setItem(IS_ACTIVE_2_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(IS_ACTIVE_2_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(IS_ACTIVE_2_ROW, VALUE_COLUMN, bIsActive2);
    table->setItem(INVERT_ACTIVE_2_ROW, NAME_COLUMN, new TableWidgetItem("bInvertActive2"));
    table->setItem(INVERT_ACTIVE_2_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(INVERT_ACTIVE_2_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(INVERT_ACTIVE_2_ROW, VALUE_COLUMN, bInvertActive2);
    table->setItem(IS_ACTIVE_3_ROW, NAME_COLUMN, new TableWidgetItem("bIsActive3"));
    table->setItem(IS_ACTIVE_3_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(IS_ACTIVE_3_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(IS_ACTIVE_3_ROW, VALUE_COLUMN, bIsActive3);
    table->setItem(INVERT_ACTIVE_3_ROW, NAME_COLUMN, new TableWidgetItem("bInvertActive3"));
    table->setItem(INVERT_ACTIVE_3_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(INVERT_ACTIVE_3_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(INVERT_ACTIVE_3_ROW, VALUE_COLUMN, bInvertActive3);
    table->setItem(IS_ACTIVE_4_ROW, NAME_COLUMN, new TableWidgetItem("bIsActive4"));
    table->setItem(IS_ACTIVE_4_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(IS_ACTIVE_4_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(IS_ACTIVE_4_ROW, VALUE_COLUMN, bIsActive4);
    table->setItem(INVERT_ACTIVE_4_ROW, NAME_COLUMN, new TableWidgetItem("bInvertActive4"));
    table->setItem(INVERT_ACTIVE_4_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(INVERT_ACTIVE_4_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(INVERT_ACTIVE_4_ROW, VALUE_COLUMN, bInvertActive4);
    topLyt->addWidget(table, 0, 0, 8, 3);
    setLayout(topLyt);
}

void BSIsActiveModifierUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
    connect(bIsActive0, SIGNAL(released()), this, SLOT(setIsActive0()), Qt::UniqueConnection);
    connect(bInvertActive0, SIGNAL(released()), this, SLOT(setInvertActive0()), Qt::UniqueConnection);
    connect(bIsActive1, SIGNAL(released()), this, SLOT(setIsActive1()), Qt::UniqueConnection);
    connect(bInvertActive1, SIGNAL(released()), this, SLOT(setInvertActive1()), Qt::UniqueConnection);
    connect(bIsActive2, SIGNAL(released()), this, SLOT(setIsActive2()), Qt::UniqueConnection);
    connect(bInvertActive2, SIGNAL(released()), this, SLOT(setInvertActive2()), Qt::UniqueConnection);
    connect(bIsActive3, SIGNAL(released()), this, SLOT(setIsActive3()), Qt::UniqueConnection);
    connect(bInvertActive3, SIGNAL(released()), this, SLOT(setInvertActive3()), Qt::UniqueConnection);
    connect(bIsActive4, SIGNAL(released()), this, SLOT(setIsActive4()), Qt::UniqueConnection);
    connect(bInvertActive4, SIGNAL(released()), this, SLOT(setInvertActive4()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)), Qt::UniqueConnection);
}

void BSIsActiveModifierUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    disconnect(bIsActive0, SIGNAL(released()), this, SLOT(setIsActive0()));
    disconnect(bInvertActive0, SIGNAL(released()), this, SLOT(setInvertActive0()));
    disconnect(bIsActive1, SIGNAL(released()), this, SLOT(setIsActive1()));
    disconnect(bInvertActive1, SIGNAL(released()), this, SLOT(setInvertActive1()));
    disconnect(bIsActive2, SIGNAL(released()), this, SLOT(setIsActive2()));
    disconnect(bInvertActive2, SIGNAL(released()), this, SLOT(setInvertActive2()));
    disconnect(bIsActive3, SIGNAL(released()), this, SLOT(setIsActive3()));
    disconnect(bInvertActive3, SIGNAL(released()), this, SLOT(setInvertActive3()));
    disconnect(bIsActive4, SIGNAL(released()), this, SLOT(setIsActive4()));
    disconnect(bInvertActive4, SIGNAL(released()), this, SLOT(setInvertActive4()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)));
}

void BSIsActiveModifierUI::connectToTables(GenericTableWidget *variables, GenericTableWidget *properties){
    if (variables && properties){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int,QString,QStringList)), variables, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int,QString,QStringList)), properties, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
    }else{
        CRITICAL_ERROR_MESSAGE("BSIsActiveModifierUI::connectToTables(): One or more arguments are nullptr!!");
    }
}

void BSIsActiveModifierUI::loadData(HkxObject *data){
    disconnectSignals();
    if (data){
        if (data->getSignature() == BS_IS_ACTIVE_MODIFIER){
            hkbVariableBindingSet *varBind = nullptr;
            bsData = static_cast<BSIsActiveModifier *>(data);
            name->setText(bsData->name);
            enable->setChecked(bsData->enable);
            bIsActive0->setChecked(bsData->bIsActive0);
            bInvertActive0->setChecked(bsData->bInvertActive0);
            bIsActive1->setChecked(bsData->bIsActive1);
            bInvertActive1->setChecked(bsData->bInvertActive1);
            bIsActive2->setChecked(bsData->bIsActive2);
            bInvertActive2->setChecked(bsData->bInvertActive2);
            bIsActive3->setChecked(bsData->bIsActive3);
            bInvertActive3->setChecked(bsData->bInvertActive3);
            bIsActive4->setChecked(bsData->bIsActive4);
            bInvertActive4->setChecked(bsData->bInvertActive4);
            varBind = bsData->getVariableBindingSetData();
            if (varBind){
                loadBinding(ENABLE_ROW, BINDING_COLUMN, varBind, "enable");
                loadBinding(IS_ACTIVE_0_ROW, BINDING_COLUMN, varBind, "bIsActive0");
                loadBinding(INVERT_ACTIVE_0_ROW, BINDING_COLUMN, varBind, "bInvertActive0");
                loadBinding(IS_ACTIVE_1_ROW, BINDING_COLUMN, varBind, "bIsActive1");
                loadBinding(INVERT_ACTIVE_1_ROW, BINDING_COLUMN, varBind, "bInvertActive1");
                loadBinding(IS_ACTIVE_2_ROW, BINDING_COLUMN, varBind, "bIsActive2");
                loadBinding(INVERT_ACTIVE_2_ROW, BINDING_COLUMN, varBind, "bInvertActive2");
                loadBinding(IS_ACTIVE_3_ROW, BINDING_COLUMN, varBind, "bIsActive3");
                loadBinding(INVERT_ACTIVE_3_ROW, BINDING_COLUMN, varBind, "bInvertActive3");
                loadBinding(IS_ACTIVE_4_ROW, BINDING_COLUMN, varBind, "bIsActive4");
                loadBinding(INVERT_ACTIVE_4_ROW, BINDING_COLUMN, varBind, "bInvertActive4");
            }else{
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(IS_ACTIVE_0_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(INVERT_ACTIVE_0_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(IS_ACTIVE_1_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(INVERT_ACTIVE_1_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(IS_ACTIVE_2_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(INVERT_ACTIVE_2_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(IS_ACTIVE_3_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(INVERT_ACTIVE_3_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(IS_ACTIVE_4_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(INVERT_ACTIVE_4_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
        }else{
            CRITICAL_ERROR_MESSAGE("BSIsActiveModifierUI::loadData(): The data is an incorrect type!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSIsActiveModifierUI::loadData(): The data is nullptr!!");
    }
    connectSignals();
}

void BSIsActiveModifierUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            bsData->setIsFileChanged(true);
            emit modifierNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData));
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSIsActiveModifierUI::setName(): The data is nullptr!!");
    }
}

void BSIsActiveModifierUI::setEnable(){
    if (bsData){
        bsData->enable = enable->isChecked();
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("BSIsActiveModifierUI::setEnable(): The data is nullptr!!");
    }
}

void BSIsActiveModifierUI::setIsActive0(){
    if (bsData){
        bsData->bIsActive0 = bIsActive0->isChecked();
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("BSIsActiveModifierUI::setIsActive0(): The data is nullptr!!");
    }
}

void BSIsActiveModifierUI::setInvertActive0(){
    if (bsData){
        bsData->bInvertActive0 = bInvertActive0->isChecked();
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("BSIsActiveModifierUI::setInvertActive0(): The data is nullptr!!");
    }
}

void BSIsActiveModifierUI::setIsActive1(){
    if (bsData){
        bsData->bIsActive1 = bIsActive1->isChecked();
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("BSIsActiveModifierUI::setIsActive1(): The data is nullptr!!");
    }
}

void BSIsActiveModifierUI::setInvertActive1(){
    if (bsData){
        bsData->bInvertActive1 = bInvertActive1->isChecked();
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("BSIsActiveModifierUI::setInvertActive1(): The data is nullptr!!");
    }
}

void BSIsActiveModifierUI::setIsActive2(){
    if (bsData){
        bsData->bIsActive2 = bIsActive2->isChecked();
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("BSIsActiveModifierUI::setIsActive2(): The data is nullptr!!");
    }
}

void BSIsActiveModifierUI::setInvertActive2(){
    if (bsData){
        bsData->bInvertActive2 = bInvertActive2->isChecked();
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("BSIsActiveModifierUI::setInvertActive2(): The data is nullptr!!");
    }
}

void BSIsActiveModifierUI::setIsActive3(){
    if (bsData){
        bsData->bIsActive3 = bIsActive3->isChecked();
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("BSIsActiveModifierUI::setIsActive3(): The data is nullptr!!");
    }
}

void BSIsActiveModifierUI::setInvertActive3(){
    if (bsData){
        bsData->bInvertActive3 = bInvertActive3->isChecked();
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("BSIsActiveModifierUI::setInvertActive3(): The data is nullptr!!");
    }
}

void BSIsActiveModifierUI::setIsActive4(){
    if (bsData){
        bsData->bIsActive4 = bIsActive4->isChecked();
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("BSIsActiveModifierUI::setIsActive4(): The data is nullptr!!");
    }
}

void BSIsActiveModifierUI::setInvertActive4(){
    if (bsData){
        bsData->bInvertActive4 = bInvertActive4->isChecked();
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("BSIsActiveModifierUI::setInvertActive4(): The data is nullptr!!");
    }
}

void BSIsActiveModifierUI::viewSelected(int row, int column){
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
            case IS_ACTIVE_0_ROW:
                if (table->item(IS_ACTIVE_0_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "bIsActive0");
                break;
            case INVERT_ACTIVE_0_ROW:
                if (table->item(INVERT_ACTIVE_0_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "bInvertActive0");
                break;
            case IS_ACTIVE_1_ROW:
                if (table->item(IS_ACTIVE_1_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "bIsActive1");
                break;
            case INVERT_ACTIVE_1_ROW:
                if (table->item(INVERT_ACTIVE_1_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "bInvertActive1");
                break;
            case IS_ACTIVE_2_ROW:
                if (table->item(IS_ACTIVE_2_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "bIsActive2");
                break;
            case INVERT_ACTIVE_2_ROW:
                if (table->item(INVERT_ACTIVE_2_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "bInvertActive2");
                break;
            case IS_ACTIVE_3_ROW:
                if (table->item(IS_ACTIVE_3_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "bIsActive3");
                break;
            case INVERT_ACTIVE_3_ROW:
                if (table->item(INVERT_ACTIVE_3_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "bInvertActive3");
                break;
            case IS_ACTIVE_4_ROW:
                if (table->item(IS_ACTIVE_4_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "bIsActive4");
                break;
            case INVERT_ACTIVE_4_ROW:
                if (table->item(INVERT_ACTIVE_4_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "bInvertActive4");
                break;
            default:
                return;
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSIsActiveModifierUI::viewSelected(): The 'bsData' pointer is nullptr!!");
    }
}

void BSIsActiveModifierUI::selectTableToView(bool viewisProperty, const QString & path){
    if (bsData){
        if (viewisProperty){
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
        CRITICAL_ERROR_MESSAGE("BSIsActiveModifierUI::selectTableToView(): The data is nullptr!!");
    }
}

void BSIsActiveModifierUI::variableRenamed(const QString & name, int index){
    if (bsData){
        index--;
        hkbVariableBindingSet *bind = bsData->getVariableBindingSetData();
        if (bind){
            int bindIndex = bind->getVariableIndexOfBinding("enable");
            if (bindIndex == index){
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("bIsActive0");
            if (bindIndex == index){
                table->item(IS_ACTIVE_0_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("bInvertActive0");
            if (bindIndex == index){
                table->item(INVERT_ACTIVE_0_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("bIsActive1");
            if (bindIndex == index){
                table->item(IS_ACTIVE_1_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("bInvertActive1");
            if (bindIndex == index){
                table->item(INVERT_ACTIVE_1_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("bIsActive2");
            if (bindIndex == index){
                table->item(IS_ACTIVE_2_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("bInvertActive2");
            if (bindIndex == index){
                table->item(INVERT_ACTIVE_2_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("bIsActive3");
            if (bindIndex == index){
                table->item(IS_ACTIVE_3_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("bInvertActive3");
            if (bindIndex == index){
                table->item(INVERT_ACTIVE_3_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("bIsActive4");
            if (bindIndex == index){
                table->item(IS_ACTIVE_4_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("bInvertActive4");
            if (bindIndex == index){
                table->item(INVERT_ACTIVE_4_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSIsActiveModifierUI::variableRenamed(): The 'bsData' pointer is nullptr!!");
    }
}

bool BSIsActiveModifierUI::setBinding(int index, int row, const QString &variableName, const QString &path, hkVariableType type, bool isProperty){
    hkbVariableBindingSet *varBind = bsData->getVariableBindingSetData();
    if (bsData){
        if (index == 0){
            varBind->removeBinding(path);if (varBind->getNumberOfBindings() == 0){static_cast<HkDynamicObject *>(bsData)->getVariableBindingSet() = HkxSharedPtr(); static_cast<BehaviorFile *>(bsData->getParentFile())->removeOtherData();}
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
        }else if ((!isProperty && areVariableTypesCompatible(static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableTypeAt(index - 1), type)) ||
                  (isProperty && areVariableTypesCompatible(static_cast<BehaviorFile *>(bsData->getParentFile())->getCharacterPropertyTypeAt(index - 1), type))){
            if (!varBind){
                varBind = new hkbVariableBindingSet(bsData->getParentFile());
                bsData->getVariableBindingSet() = HkxSharedPtr(varBind);
            }
            if (isProperty){
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY)){
                    CRITICAL_ERROR_MESSAGE("BSIsActiveModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }else{
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    CRITICAL_ERROR_MESSAGE("BSIsActiveModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->setIsFileChanged(true);
        }else{
            WARNING_MESSAGE("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSIsActiveModifierUI::setBinding(): The data is nullptr!!");
    }
    return true;
}

void BSIsActiveModifierUI::setBindingVariable(int index, const QString &name){
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
        case IS_ACTIVE_0_ROW:
            if (table->item(IS_ACTIVE_0_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "bIsActive0", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case INVERT_ACTIVE_0_ROW:
            if (table->item(INVERT_ACTIVE_0_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "bInvertActive0", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case IS_ACTIVE_1_ROW:
            if (table->item(IS_ACTIVE_1_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "bIsActive1", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case INVERT_ACTIVE_1_ROW:
            if (table->item(INVERT_ACTIVE_1_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "bInvertActive1", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case IS_ACTIVE_2_ROW:
            if (table->item(IS_ACTIVE_2_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "bIsActive2", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case INVERT_ACTIVE_2_ROW:
            if (table->item(INVERT_ACTIVE_2_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "bInvertActive2", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case IS_ACTIVE_3_ROW:
            if (table->item(IS_ACTIVE_3_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "bIsActive3", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case INVERT_ACTIVE_3_ROW:
            if (table->item(INVERT_ACTIVE_3_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "bInvertActive3", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case IS_ACTIVE_4_ROW:
            if (table->item(IS_ACTIVE_4_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "bIsActive4", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case INVERT_ACTIVE_4_ROW:
            if (table->item(INVERT_ACTIVE_4_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "bInvertActive4", VARIABLE_TYPE_BOOL, isProperty);
            break;
        default:
            return;
        }
        bsData->setIsFileChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("BSIsActiveModifierUI::setBindingVariable(): The data is nullptr!!");
    }
}

void BSIsActiveModifierUI::loadBinding(int row, int column, hkbVariableBindingSet *varBind, const QString &path){
    if (bsData){
        if (varBind){
            int index = varBind->getVariableIndexOfBinding(path);
            QString varName;
            if (index != -1){
                if (varBind->getBindingType(path) == hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY){
                    varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getCharacterPropertyNameAt(index, true);
                    table->item(row, column)->setCheckState(Qt::Checked);
                }else{
                    varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getVariableNameAt(index);
                }
            }
            if (varName == ""){
                varName = "NONE";
            }
            table->item(row, column)->setText(BINDING_ITEM_LABEL+varName);
        }else{
            CRITICAL_ERROR_MESSAGE("BSIsActiveModifierUI::loadBinding(): The variable binding set is nullptr!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("BSIsActiveModifierUI::loadBinding(): The data is nullptr!!");
    }
}
