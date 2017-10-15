#include "delayedmodifierui.h"

#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/modifiers/hkbDelayedModifier.h"
#include "src/ui/genericdatawidgets.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/TreeGraphicsView.h"
#include "src/ui/behaviorgraphview.h"
#include "src/ui/treegraphicsitem.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"


#include <QHeaderView>
#include <QGroupBox>

#define BASE_NUMBER_OF_ROWS 6

#define NAME_ROW 0
#define ENABLE_ROW 1
#define DELAY_SECONDS_ROW 2
#define DURATION_SECONDS_ROW 3
#define SECONDS_ELAPSED_ROW 4
#define MODIFIER_ROW 5

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList DelayedModifierUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

DelayedModifierUI::DelayedModifierUI()
    : behaviorView(NULL),
      bsData(NULL),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      enable(new CheckBox),
      delaySeconds(new DoubleSpinBox),
      durationSeconds(new DoubleSpinBox),
      secondsElapsed(new DoubleSpinBox)
{
    setTitle("hkbDelayedModifier");
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(NAME_ROW, NAME_COLUMN, new TableWidgetItem("name"));
    table->setItem(NAME_ROW, TYPE_COLUMN, new TableWidgetItem("hkStringPtr", Qt::AlignCenter));
    table->setItem(NAME_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignVCenter));
    table->setCellWidget(NAME_ROW, VALUE_COLUMN, name);
    table->setItem(ENABLE_ROW, NAME_COLUMN, new TableWidgetItem("enable"));
    table->setItem(ENABLE_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(ENABLE_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ENABLE_ROW, VALUE_COLUMN, enable);
    table->setItem(DELAY_SECONDS_ROW, NAME_COLUMN, new TableWidgetItem("delaySeconds"));
    table->setItem(DELAY_SECONDS_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(DELAY_SECONDS_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(DELAY_SECONDS_ROW, VALUE_COLUMN, delaySeconds);
    table->setItem(DURATION_SECONDS_ROW, NAME_COLUMN, new TableWidgetItem("durationSeconds"));
    table->setItem(DURATION_SECONDS_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(DURATION_SECONDS_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(DURATION_SECONDS_ROW, VALUE_COLUMN, durationSeconds);
    table->setItem(SECONDS_ELAPSED_ROW, NAME_COLUMN, new TableWidgetItem("secondsElapsed"));
    table->setItem(SECONDS_ELAPSED_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(SECONDS_ELAPSED_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(SECONDS_ELAPSED_ROW, VALUE_COLUMN, secondsElapsed);
    table->setItem(MODIFIER_ROW, NAME_COLUMN, new TableWidgetItem("modifier"));
    table->setItem(MODIFIER_ROW, TYPE_COLUMN, new TableWidgetItem("hkbModifier", Qt::AlignCenter));
    table->setItem(MODIFIER_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(MODIFIER_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_MODIFIERS_TABLE_TIP));
    topLyt->addWidget(table, 0, 0, 8, 3);
    setLayout(topLyt);
    connectSignals();
}

void DelayedModifierUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
    connect(delaySeconds, SIGNAL(editingFinished()), this, SLOT(setDelaySeconds()), Qt::UniqueConnection);
    connect(durationSeconds, SIGNAL(editingFinished()), this, SLOT(setDurationSeconds()), Qt::UniqueConnection);
    connect(secondsElapsed, SIGNAL(editingFinished()), this, SLOT(setSecondsElapsed()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)), Qt::UniqueConnection);
}

void DelayedModifierUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    disconnect(delaySeconds, SIGNAL(editingFinished()), this, SLOT(setDelaySeconds()));
    disconnect(durationSeconds, SIGNAL(editingFinished()), this, SLOT(setDurationSeconds()));
    disconnect(secondsElapsed, SIGNAL(editingFinished()), this, SLOT(setSecondsElapsed()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)));
}

void DelayedModifierUI::connectToTables(GenericTableWidget *modifiers, GenericTableWidget *variables, GenericTableWidget *properties){
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
        (qFatal("DelayedModifierUI::connectToTables(): One or more arguments are NULL!!"));
    }
}

void DelayedModifierUI::loadData(HkxObject *data){
    disconnectSignals();
    hkbVariableBindingSet *varBind = NULL;
    if (data){
        if (data->getSignature() == HKB_DELAYED_MODIFIER){
            bsData = static_cast<hkbDelayedModifier *>(data);
            name->setText(bsData->name);
            enable->setChecked(bsData->enable);
            delaySeconds->setValue(bsData->delaySeconds);
            durationSeconds->setValue(bsData->durationSeconds);
            secondsElapsed->setValue(bsData->secondsElapsed);
            if (bsData->modifier.data()){
                table->item(MODIFIER_ROW, VALUE_COLUMN)->setText(static_cast<hkbModifier *>(bsData->modifier.data())->getName());
            }else{
                table->item(MODIFIER_ROW, VALUE_COLUMN)->setText("NONE");
            }
            varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (varBind){
                loadBinding(ENABLE_ROW, BINDING_COLUMN, varBind, "enable");
                loadBinding(DELAY_SECONDS_ROW, BINDING_COLUMN, varBind, "delaySeconds");
                loadBinding(DURATION_SECONDS_ROW, BINDING_COLUMN, varBind, "durationSeconds");
                loadBinding(SECONDS_ELAPSED_ROW, BINDING_COLUMN, varBind, "secondsElapsed");
            }else{
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(DELAY_SECONDS_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(DURATION_SECONDS_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(SECONDS_ELAPSED_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
        }else{
            (qFatal(QString("DelayedModifierUI::loadData(): The data passed to the UI is the wrong type!\nSIGNATURE: "+QString::number(data->getSignature(), 16)).toLocal8Bit().data()));
        }
    }else{
        (qFatal("DelayedModifierUI::loadData(): The data passed to the UI is NULL!!!"));
    }
    connectSignals();
}

void DelayedModifierUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            bsData->getParentFile()->toggleChanged(true);
            emit modifierNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData));
        }
    }
}

void DelayedModifierUI::setEnable(){
    if (bsData){
        bsData->enable = enable->isChecked();
        bsData->getParentFile()->toggleChanged(true);
    }else{
        (qFatal("DelayedModifierUI::setEnable(): The data is NULL!!"));
    }
}

void DelayedModifierUI::setDelaySeconds(){
    if (bsData){
        if (bsData->delaySeconds != delaySeconds->value()){
            bsData->delaySeconds = delaySeconds->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        (qFatal("DelayedModifierUI::setDelaySeconds(): The data is NULL!!"));
    }
}

void DelayedModifierUI::setDurationSeconds(){
    if (bsData){
        if (bsData->durationSeconds != durationSeconds->value()){
            bsData->durationSeconds = durationSeconds->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        (qFatal("DelayedModifierUI::setDurationSeconds(): The data is NULL!!"));
    }
}

void DelayedModifierUI::setSecondsElapsed(){
    if (bsData){
        if (bsData->secondsElapsed != secondsElapsed->value()){
            bsData->secondsElapsed = secondsElapsed->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        (qFatal("DelayedModifierUI::setSecondsElapsed(): The data is NULL!!"));
    }
}

void DelayedModifierUI::setModifier(int index, const QString & name){
    DataIconManager *ptr = NULL;
    int indexOfModifier = -1;
    if (bsData){
        if (behaviorView){
            ptr = static_cast<BehaviorFile *>(bsData->getParentFile())->getModifierDataAt(index - 1);
            indexOfModifier = bsData->getIndexOfObj(static_cast<DataIconManager*>(bsData->modifier.data()));
            if (ptr){
                if (name != ptr->getName()){
                    (qFatal("The name of the selected object does not match it's name in the object selection table!!!"));
                    return;
                }else if (ptr == bsData || !behaviorView->reconnectIcon(behaviorView->getSelectedItem(), static_cast<DataIconManager*>(bsData->modifier.data()), ptr, false)){
                    (qWarning("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\nYou are attempting to create a circular branch or dead end!!!"));
                    return;
                }
            }else{
                if (behaviorView->getSelectedItem()){
                    behaviorView->removeItemFromGraph(behaviorView->getSelectedItem()->getChildWithData(static_cast<DataIconManager*>(bsData->modifier.data())), indexOfModifier);
                }else{
                    (qFatal("DelayedModifierUI::setModifier(): The selected icon is NULL!!"));
                    return;
                }
            }
            behaviorView->removeModifierData();
            table->item(MODIFIER_ROW, VALUE_COLUMN)->setText(name);
            bsData->getParentFile()->toggleChanged(true);
        }else{
            (qFatal("DelayedModifierUI::setModifier(): The 'behaviorView' pointer is NULL!!"));
        }
    }else{
        (qFatal("DelayedModifierUI::setModifier(): The 'bsData' pointer is NULL!!"));
    }
}

void DelayedModifierUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
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
            (qFatal("DelayedModifierUI::loadBinding(): The variable binding set is NULL!!"));
        }
    }else{
        (qFatal("DelayedModifierUI::loadBinding(): The data is NULL!!"));
    }
}

bool DelayedModifierUI::setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty){
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
                    (qFatal("DelayedModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }else{
                if (!varBind->addBinding(path, variableName, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    (qFatal("DelayedModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->toggleChanged(true);
        }else{
            (qWarning("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\nYou are attempting to bind a variable of an invalid type for this data field!!!"));
        }
    }else{
        (qFatal("DelayedModifierUI::setBinding(): The 'bsData' pointer is NULL!!"));
        return false;
    }
    return true;
}

void DelayedModifierUI::setBindingVariable(int index, const QString & name){
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
        case DELAY_SECONDS_ROW:
            if (table->item(DELAY_SECONDS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "delaySeconds", VARIABLE_TYPE_REAL, isProperty);
            break;
        case DURATION_SECONDS_ROW:
            if (table->item(DURATION_SECONDS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "durationSeconds", VARIABLE_TYPE_REAL, isProperty);
            break;
        case SECONDS_ELAPSED_ROW:
            if (table->item(SECONDS_ELAPSED_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "secondsElapsed", VARIABLE_TYPE_REAL, isProperty);
            break;
        default:
            return;
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        (qFatal("DelayedModifierUI::setBindingVariable(): The 'bsData' pointer is NULL!!"));
    }
}

void DelayedModifierUI::selectTableToView(bool viewproperties, const QString & path){
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
        (qFatal("DelayedModifierUI::selectTableToView(): The data is NULL!!"));
    }
}

void DelayedModifierUI::viewSelected(int row, int column){
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
            case DELAY_SECONDS_ROW:
                if (table->item(DELAY_SECONDS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "delaySeconds");
                break;
            case DURATION_SECONDS_ROW:
                if (table->item(DURATION_SECONDS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "durationSeconds");
                break;
            case SECONDS_ELAPSED_ROW:
                if (table->item(SECONDS_ELAPSED_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "secondsElapsed");
                break;
            }
        }else if (row == MODIFIER_ROW && column == VALUE_COLUMN){
            emit viewModifiers(static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData->modifier) + 1);
        }
    }else{
        (qFatal("DelayedModifierUI::viewSelected(): The 'bsData' pointer is NULL!!"));
    }
}

void DelayedModifierUI::variableRenamed(const QString & name, int index){
    if (bsData){
        index--;
        hkbVariableBindingSet *bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (bind){
            int bindIndex = bind->getVariableIndexOfBinding("enable");
            if (bindIndex == index){
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("delaySeconds");
            if (bindIndex == index){
                table->item(DELAY_SECONDS_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("durationSeconds");
            if (bindIndex == index){
                table->item(DURATION_SECONDS_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("secondsElapsed");
            if (bindIndex == index){
                table->item(SECONDS_ELAPSED_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        (qFatal("DelayedModifierUI::variableRenamed(): The 'bsData' pointer is NULL!!"));
    }
}

void DelayedModifierUI::modifierRenamed(const QString &name, int index){
    if (bsData){
        index--;
        if (index == static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData->modifier)){
            table->item(MODIFIER_ROW, VALUE_COLUMN)->setText(name);
        }
    }else{
        (qFatal("DelayedModifierUI::generatorRenamed(): The 'bsData' pointer is NULL!!"));
    }
}

void DelayedModifierUI::setBehaviorView(BehaviorGraphView *view){
    behaviorView = view;
}
