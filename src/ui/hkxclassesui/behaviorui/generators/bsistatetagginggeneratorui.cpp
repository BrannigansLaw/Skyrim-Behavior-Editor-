#include "bsistatetagginggeneratorui.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/generators/bsistatetagginggenerator.h"
#include "src/ui/genericdatawidgets.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/TreeGraphicsView.h"
#include "src/ui/behaviorgraphview.h"
#include "src/ui/treegraphicsitem.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"


#include <QHeaderView>
#include <QGroupBox>

#define BASE_NUMBER_OF_ROWS 4

#define NAME_ROW 0
#define I_STATE_TO_SET_AS_ROW 1
#define I_PRIORITY_ROW 2
#define P_DEFAULT_GENERATOR_ROW 3

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList BSiStateTaggingGeneratorUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

BSiStateTaggingGeneratorUI::BSiStateTaggingGeneratorUI()
    : behaviorView(NULL),
      bsData(NULL),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      iStateToSetAs(new SpinBox),
      iPriority(new SpinBox)
{
    setTitle("BSiStateTaggingGenerator");
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(NAME_ROW, NAME_COLUMN, new TableWidgetItem("name"));
    table->setItem(NAME_ROW, TYPE_COLUMN, new TableWidgetItem("hkStringPtr", Qt::AlignCenter));
    table->setItem(NAME_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignVCenter));
    table->setCellWidget(NAME_ROW, VALUE_COLUMN, name);
    table->setItem(I_STATE_TO_SET_AS_ROW, NAME_COLUMN, new TableWidgetItem("iStateToSetAs"));
    table->setItem(I_STATE_TO_SET_AS_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(I_STATE_TO_SET_AS_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(I_STATE_TO_SET_AS_ROW, VALUE_COLUMN, iStateToSetAs);
    table->setItem(I_PRIORITY_ROW, NAME_COLUMN, new TableWidgetItem("iPriority"));
    table->setItem(I_PRIORITY_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(I_PRIORITY_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(I_PRIORITY_ROW, VALUE_COLUMN, iPriority);
    table->setItem(P_DEFAULT_GENERATOR_ROW, NAME_COLUMN, new TableWidgetItem("pDefaultGenerator"));
    table->setItem(P_DEFAULT_GENERATOR_ROW, TYPE_COLUMN, new TableWidgetItem("hkbGenerator", Qt::AlignCenter));
    table->setItem(P_DEFAULT_GENERATOR_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(P_DEFAULT_GENERATOR_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_GENERATORS_TABLE_TIP));
    topLyt->addWidget(table, 0, 0, 8, 3);
    setLayout(topLyt);
    connectSignals();
}

void BSiStateTaggingGeneratorUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(iStateToSetAs, SIGNAL(editingFinished()), this, SLOT(setIStateToSetAs()), Qt::UniqueConnection);
    connect(iPriority, SIGNAL(editingFinished()), this, SLOT(setIPriority()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)), Qt::UniqueConnection);
}

void BSiStateTaggingGeneratorUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(iStateToSetAs, SIGNAL(editingFinished()), this, SLOT(setIStateToSetAs()));
    disconnect(iPriority, SIGNAL(editingFinished()), this, SLOT(setIPriority()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)));
}

void BSiStateTaggingGeneratorUI::connectToTables(GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *generators){
    if (variables && properties && generators){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(generators, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(generators, SIGNAL(elementSelected(int,QString)), this, SLOT(setDefaultGenerator(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewGenerators(int)), generators, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int)), variables, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int)), properties, SLOT(showTable(int)), Qt::UniqueConnection);
    }else{
        (qFatal("BSiStateTaggingGeneratorUI::connectToTables(): One or more arguments are NULL!!"));
    }
}

void BSiStateTaggingGeneratorUI::loadData(HkxObject *data){
    disconnectSignals();
    hkbVariableBindingSet *varBind = NULL;
    if (data){
        if (data->getSignature() == BS_I_STATE_TAGGING_GENERATOR){
            bsData = static_cast<BSiStateTaggingGenerator *>(data);
            name->setText(bsData->name);
            iStateToSetAs->setValue(bsData->iStateToSetAs);
            iPriority->setValue(bsData->iPriority);
            if (bsData->pDefaultGenerator.data()){
                table->item(P_DEFAULT_GENERATOR_ROW, VALUE_COLUMN)->setText(static_cast<hkbGenerator *>(bsData->pDefaultGenerator.data())->getName());
            }else{
                table->item(P_DEFAULT_GENERATOR_ROW, VALUE_COLUMN)->setText("NONE");
            }
            varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (varBind){
                loadBinding(I_STATE_TO_SET_AS_ROW, BINDING_COLUMN, varBind, "iStateToSetAs");
                loadBinding(I_PRIORITY_ROW, BINDING_COLUMN, varBind, "iPriority");
            }else{
                table->item(I_STATE_TO_SET_AS_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(I_PRIORITY_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
        }else{
            (qFatal(QString("BSiStateTaggingGeneratorUI::loadData(): The data passed to the UI is the wrong type!\nSIGNATURE: "+QString::number(data->getSignature(), 16)).toLocal8Bit().data()));
        }
    }else{
        (qFatal("BSiStateTaggingGeneratorUI::loadData(): The data passed to the UI is NULL!!!"));
    }
    connectSignals();
}

void BSiStateTaggingGeneratorUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            bsData->getParentFile()->setIsChanged(true);
            emit generatorNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData));
        }
    }
}

void BSiStateTaggingGeneratorUI::setIStateToSetAs(){
    if (bsData){
        if (bsData->iStateToSetAs != iStateToSetAs->value()){
            bsData->iStateToSetAs = iStateToSetAs->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        (qFatal("BSiStateTaggingGeneratorUI::setIStateToSetAs(): The 'bsData' pointer is NULL!!"));
    }
}

void BSiStateTaggingGeneratorUI::setIPriority(){
    if (bsData){
        if (bsData->iPriority != iPriority->value()){
            bsData->iPriority = iPriority->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        (qFatal("BSiStateTaggingGeneratorUI::setIPriority(): The 'bsData' pointer is NULL!!"));
    }
}

void BSiStateTaggingGeneratorUI::setDefaultGenerator(int index, const QString & name){
    DataIconManager *ptr = NULL;
    int indexOfGenerator = -1;
    if (bsData){
        if (behaviorView){
            ptr = static_cast<BehaviorFile *>(bsData->getParentFile())->getGeneratorDataAt(index - 1);
            indexOfGenerator = bsData->getIndexOfObj(static_cast<DataIconManager*>(bsData->pDefaultGenerator.data()));
            if (ptr){
                if (name != ptr->getName()){
                    (qFatal("The name of the selected object does not match it's name in the object selection table!!!"));
                    return;
                }else if (ptr == bsData || !behaviorView->reconnectIcon(behaviorView->getSelectedItem(), static_cast<DataIconManager*>(bsData->pDefaultGenerator.data()), ptr, false)){
                    (qWarning("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\nYou are attempting to create a circular branch or dead end!!!"));
                    return;
                }
            }else{
                if (behaviorView->getSelectedItem()){
                    behaviorView->removeItemFromGraph(behaviorView->getSelectedItem()->getChildWithData(static_cast<DataIconManager*>(bsData->pDefaultGenerator.data())), indexOfGenerator);
                }else{
                    (qFatal("BSiStateTaggingGeneratorUI::setDefaultGenerator(): The selected icon is NULL!!"));
                    return;
                }
            }
            behaviorView->removeGeneratorData();
            table->item(P_DEFAULT_GENERATOR_ROW, VALUE_COLUMN)->setText(name);
            bsData->getParentFile()->setIsChanged(true);
        }else{
            (qFatal("BSiStateTaggingGeneratorUI::setDefaultGenerator(): The 'behaviorView' pointer is NULL!!"));
        }
    }else{
        (qFatal("BSiStateTaggingGeneratorUI::setDefaultGenerator(): The 'bsData' pointer is NULL!!"));
    }
}

void BSiStateTaggingGeneratorUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
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
            (qFatal("BSiStateTaggingGeneratorUI::loadBinding(): The variable binding set is NULL!!"));
        }
    }else{
        (qFatal("BSiStateTaggingGeneratorUI::loadBinding(): The data is NULL!!"));
    }
}

bool BSiStateTaggingGeneratorUI::setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty){
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
                    (qFatal("BSiStateTaggingGeneratorUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }else{
                if (!varBind->addBinding(path, variableName, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    (qFatal("BSiStateTaggingGeneratorUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->setIsChanged(true);
        }else{
            (qWarning("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\nYou are attempting to bind a variable of an invalid type for this data field!!!"));
        }
    }else{
        (qFatal("BSiStateTaggingGeneratorUI::setBinding(): The 'bsData' pointer is NULL!!"));
        return false;
    }
    return true;
}

void BSiStateTaggingGeneratorUI::setBindingVariable(int index, const QString & name){
    if (bsData){
        bool isProperty = false;
        int row = table->currentRow();
        switch (row){
        case I_STATE_TO_SET_AS_ROW:
            if (table->item(I_STATE_TO_SET_AS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "iStateToSetAs", VARIABLE_TYPE_INT32, isProperty);
            break;
        case I_PRIORITY_ROW:
            if (table->item(I_PRIORITY_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "iPriority", VARIABLE_TYPE_INT32, isProperty);
            break;
        default:
            return;
        }
        bsData->getParentFile()->setIsChanged(true);
    }else{
        (qFatal("BSiStateTaggingGeneratorUI::setBindingVariable(): The 'bsData' pointer is NULL!!"));
    }
}

void BSiStateTaggingGeneratorUI::selectTableToView(bool viewproperties, const QString & path){
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
        (qFatal("BSiStateTaggingGeneratorUI::selectTableToView(): The data is NULL!!"));
    }
}

void BSiStateTaggingGeneratorUI::viewSelected(int row, int column){
    if (bsData){
        bool properties = false;
        if (column == BINDING_COLUMN){
            switch (row){
            case I_STATE_TO_SET_AS_ROW:
                if (table->item(I_STATE_TO_SET_AS_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "iStateToSetAs");
                break;
            case I_PRIORITY_ROW:
                if (table->item(I_PRIORITY_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "iPriority");
                break;
            }
        }else if (row == P_DEFAULT_GENERATOR_ROW && column == VALUE_COLUMN){
            emit viewGenerators(static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData->pDefaultGenerator) + 1);
        }
    }else{
        (qFatal("BSiStateTaggingGeneratorUI::viewSelected(): The 'bsData' pointer is NULL!!"));
    }
}

void BSiStateTaggingGeneratorUI::variableRenamed(const QString & name, int index){
    if (bsData){
        index--;
        hkbVariableBindingSet *bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (bind){
            int bindIndex = bind->getVariableIndexOfBinding("iStateToSetAs");
            if (bindIndex == index){
                table->item(I_STATE_TO_SET_AS_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("iPriority");
            if (bindIndex == index){
                table->item(I_PRIORITY_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        (qFatal("BSiStateTaggingGeneratorUI::variableRenamed(): The 'bsData' pointer is NULL!!"));
    }
}

void BSiStateTaggingGeneratorUI::generatorRenamed(const QString &name, int index){
    if (bsData){
        index--;
        if (index == static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData->pDefaultGenerator)){
            table->item(P_DEFAULT_GENERATOR_ROW, VALUE_COLUMN)->setText(name);
        }
    }else{
        (qFatal("BSiStateTaggingGeneratorUI::generatorRenamed(): The 'bsData' pointer is NULL!!"));
    }
}

void BSiStateTaggingGeneratorUI::setBehaviorView(BehaviorGraphView *view){
    behaviorView = view;
}
