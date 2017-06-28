#include "bsoffsetanimationgeneratorui.h"

#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/hkxclasses/behavior/generators/bsoffsetanimationgenerator.h"
#include "src/hkxclasses/behavior/generators/hkbgenerator.h"
#include "src/ui/genericdatawidgets.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/behaviorgraphview.h"
#include "src/ui/treegraphicsitem.h"
#include "src/ui/genericdatawidgets.h"

#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 6

#define NAME_ROW 0
#define FOFFSET_VARIABLE_ROW 1
#define FOFFSET_RANGE_START_ROW 2
#define FOFFSET_RANGE_END_ROW 3
#define PDEFAULT_GENERATOR_ROW 4
#define POFFSET_CLIP_GENERATOR_ROW 5

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList BSOffsetAnimationGeneratorUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

BSOffsetAnimationGeneratorUI::BSOffsetAnimationGeneratorUI()
    : behaviorView(NULL),
      bsData(NULL),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::cyan))),
      name(new LineEdit),
      fOffsetVariable(new DoubleSpinBox),
      fOffsetRangeStart(new DoubleSpinBox),
      fOffsetRangeEnd(new DoubleSpinBox)
{
    setTitle("BSOffsetAnimationGenerator");
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(NAME_ROW, NAME_COLUMN, new TableWidgetItem("name"));
    table->setItem(NAME_ROW, TYPE_COLUMN, new TableWidgetItem("hkStringPtr", Qt::AlignCenter));
    table->setItem(NAME_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(NAME_ROW, VALUE_COLUMN, name);
    table->setItem(FOFFSET_VARIABLE_ROW, NAME_COLUMN, new TableWidgetItem("fOffsetVariable"));
    table->setItem(FOFFSET_VARIABLE_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(FOFFSET_VARIABLE_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setItem(FOFFSET_VARIABLE_ROW, VALUE_COLUMN, new TableWidgetItem("", Qt::AlignCenter, QColor(Qt::lightGray)));
    table->setCellWidget(FOFFSET_VARIABLE_ROW, VALUE_COLUMN, fOffsetVariable);
    table->setItem(FOFFSET_RANGE_START_ROW, NAME_COLUMN, new TableWidgetItem("fOffsetRangeStart"));
    table->setItem(FOFFSET_RANGE_START_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(FOFFSET_RANGE_START_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setItem(FOFFSET_RANGE_START_ROW, VALUE_COLUMN, new TableWidgetItem("", Qt::AlignCenter, QColor(Qt::lightGray)));
    table->setCellWidget(FOFFSET_RANGE_START_ROW, VALUE_COLUMN, fOffsetRangeStart);
    table->setItem(FOFFSET_RANGE_END_ROW, NAME_COLUMN, new TableWidgetItem("fOffsetRangeEnd"));
    table->setItem(FOFFSET_RANGE_END_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(FOFFSET_RANGE_END_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setItem(FOFFSET_RANGE_END_ROW, VALUE_COLUMN, new TableWidgetItem("", Qt::AlignCenter, QColor(Qt::lightGray)));
    table->setCellWidget(FOFFSET_RANGE_END_ROW, VALUE_COLUMN, fOffsetRangeEnd);
    table->setItem(PDEFAULT_GENERATOR_ROW, NAME_COLUMN, new TableWidgetItem("pDefaultGenerator"));
    table->setItem(PDEFAULT_GENERATOR_ROW, TYPE_COLUMN, new TableWidgetItem("hkbGenerator", Qt::AlignCenter));
    table->setItem(PDEFAULT_GENERATOR_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(PDEFAULT_GENERATOR_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_GENERATORS_TABLE_TIP));
    table->setItem(POFFSET_CLIP_GENERATOR_ROW, NAME_COLUMN, new TableWidgetItem("pOffsetClipGenerator"));
    table->setItem(POFFSET_CLIP_GENERATOR_ROW, TYPE_COLUMN, new TableWidgetItem("hkbClipGenerator", Qt::AlignCenter));
    table->setItem(POFFSET_CLIP_GENERATOR_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(POFFSET_CLIP_GENERATOR_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_GENERATORS_TABLE_TIP));
    topLyt->addWidget(table, 0, 0, 8, 3);
    setLayout(topLyt);
    connectSignals();
}

void BSOffsetAnimationGeneratorUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(fOffsetVariable, SIGNAL(clicked(bool)), this, SLOT(setFOffsetVariable()), Qt::UniqueConnection);
    connect(fOffsetRangeStart, SIGNAL(editingFinished()), this, SLOT(setFOffsetRangeStart()), Qt::UniqueConnection);
    connect(fOffsetRangeEnd, SIGNAL(clicked(bool)), this, SLOT(setFOffsetRangeEnd()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)), Qt::UniqueConnection);
}

void BSOffsetAnimationGeneratorUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(fOffsetVariable, SIGNAL(clicked(bool)), this, SLOT(setFOffsetVariable()));
    disconnect(fOffsetRangeStart, SIGNAL(editingFinished()), this, SLOT(setFOffsetRangeStart()));
    disconnect(fOffsetRangeEnd, SIGNAL(clicked(bool)), this, SLOT(setFOffsetRangeEnd()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)));
}

void BSOffsetAnimationGeneratorUI::connectToTables(GenericTableWidget *generators, GenericTableWidget *variables, GenericTableWidget *properties){
    if (variables && properties && generators){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(generators, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(generators, SIGNAL(elementSelected(int,QString)), this, SLOT(setGenerator(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewGenerators(int)), generators, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int)), variables, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int)), properties, SLOT(showTable(int)), Qt::UniqueConnection);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSOffsetAnimationGeneratorUI::connectToTables(): One or more arguments are NULL!!"))
    }
}

void BSOffsetAnimationGeneratorUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            bsData->getParentFile()->toggleChanged(true);
            emit generatorNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSOffsetAnimationGeneratorUI::setName(): The data is NULL!!"))
    }
}

void BSOffsetAnimationGeneratorUI::setFOffsetVariable(){
    if (bsData){
        bsData->fOffsetVariable = fOffsetVariable->value();
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSOffsetAnimationGeneratorUI::setFOffsetVariable(): The data is NULL!!"))
    }
}

void BSOffsetAnimationGeneratorUI::setFOffsetRangeStart(){
    if (bsData){
        bsData->fOffsetRangeStart = fOffsetRangeStart->value();
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSOffsetAnimationGeneratorUI::setFOffsetRangeStart(): The data is NULL!!"))
    }
}

void BSOffsetAnimationGeneratorUI::setFOffsetRangeEnd(){
    if (bsData){
        bsData->fOffsetRangeEnd = fOffsetRangeEnd->value();
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSOffsetAnimationGeneratorUI::setFOffsetRangeEnd(): The data is NULL!!"))
    }
}

bool BSOffsetAnimationGeneratorUI::setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty){
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
                    CRITICAL_ERROR_MESSAGE(QString("BSOffsetAnimationGeneratorUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }else{
                if (!varBind->addBinding(path, variableName, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    CRITICAL_ERROR_MESSAGE(QString("BSOffsetAnimationGeneratorUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->toggleChanged(true);
        }else{
            WARNING_MESSAGE(QString("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSOffsetAnimationGeneratorUI::setBinding(): The data is NULL!!"));
    }
    return true;
}

void BSOffsetAnimationGeneratorUI::setBindingVariable(int index, const QString & name){
    if (bsData){
        bool isProperty = false;
        int row = table->currentRow();
        switch (row){
        case FOFFSET_VARIABLE_ROW:
            if (table->item(FOFFSET_VARIABLE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "fOffsetVariable", VARIABLE_TYPE_REAL, isProperty);
            break;
        case FOFFSET_RANGE_START_ROW:
            if (table->item(FOFFSET_RANGE_START_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "fOffsetRangeStart", VARIABLE_TYPE_REAL, isProperty);
            break;
        case FOFFSET_RANGE_END_ROW:
            if (table->item(FOFFSET_RANGE_END_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "fOffsetRangeEnd", VARIABLE_TYPE_REAL, isProperty);
            break;
        default:
            return;
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSOffsetAnimationGeneratorUI::setBindingVariable(): The data is NULL!!"))
    }
}

void BSOffsetAnimationGeneratorUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
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
            CRITICAL_ERROR_MESSAGE(QString("BSOffsetAnimationGeneratorUI::loadBinding(): The variable binding set is NULL!!"));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSOffsetAnimationGeneratorUI::loadBinding(): The data is NULL!!"));
    }
}

void BSOffsetAnimationGeneratorUI::loadData(HkxObject *data){
    disconnectSignals();
    if (data){
        if (data->getSignature() == BS_OFFSET_ANIMATION_GENERATOR){
            bsData = static_cast<BSOffsetAnimationGenerator *>(data);
            name->setText(bsData->name);
            fOffsetVariable->setValue(bsData->fOffsetVariable);
            fOffsetRangeStart->setValue(bsData->fOffsetRangeStart);
            fOffsetRangeEnd->setValue(bsData->fOffsetRangeEnd);
            if (bsData->pDefaultGenerator.data()){
                table->item(PDEFAULT_GENERATOR_ROW, VALUE_COLUMN)->setText(static_cast<hkbGenerator *>(bsData->pDefaultGenerator.data())->getName());
            }else{
                table->item(PDEFAULT_GENERATOR_ROW, VALUE_COLUMN)->setText("NONE");
            }
            if (bsData->pOffsetClipGenerator.data()){
                table->item(POFFSET_CLIP_GENERATOR_ROW, VALUE_COLUMN)->setText(static_cast<hkbGenerator *>(bsData->pOffsetClipGenerator.data())->getName());
            }else{
                table->item(POFFSET_CLIP_GENERATOR_ROW, VALUE_COLUMN)->setText("NONE");
            }
            hkbVariableBindingSet *varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (varBind){
                loadBinding(FOFFSET_VARIABLE_ROW, BINDING_COLUMN, varBind, "fOffsetVariable");
                loadBinding(FOFFSET_RANGE_START_ROW, BINDING_COLUMN, varBind, "fOffsetRangeStart");
                loadBinding(FOFFSET_RANGE_END_ROW, BINDING_COLUMN, varBind, "fOffsetRangeEnd");
            }else{
                table->item(FOFFSET_VARIABLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(FOFFSET_RANGE_START_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(FOFFSET_RANGE_END_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
        }else{
            CRITICAL_ERROR_MESSAGE(QString("BSOffsetAnimationGeneratorUI::loadData(): The data passed to the UI is the wrong type!\nSIGNATURE: "+QString::number(data->getSignature(), 16)));
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSOffsetAnimationGeneratorUI::loadData(): The data passed to the UI is NULL!!!"));
    }
    connectSignals();
}

void BSOffsetAnimationGeneratorUI::setGenerator(int index, const QString & name){
    DataIconManager *ptr = NULL;
    int indexOfGenerator = -1;
    DataIconManager *gen = NULL;
    int row = -1;
    if (bsData){
        if (behaviorView){
            if (table->currentRow() == PDEFAULT_GENERATOR_ROW){
                gen = static_cast<DataIconManager*>(bsData->pDefaultGenerator.data());
                row = PDEFAULT_GENERATOR_ROW;
            }else if (table->currentRow() == POFFSET_CLIP_GENERATOR_ROW){
                gen = static_cast<DataIconManager*>(bsData->pOffsetClipGenerator.data());
                row = POFFSET_CLIP_GENERATOR_ROW;
            }
            ptr = static_cast<BehaviorFile *>(bsData->getParentFile())->getGeneratorDataAt(index - 1);//Check if the target data is already referenced? Must check if target is clipgen!!!
            indexOfGenerator = bsData->getIndexOfObj(gen);
            if (ptr){
                if (name != ptr->getName()){
                    CRITICAL_ERROR_MESSAGE(QString("The name of the selected object does not match it's name in the object selection table!!!"));
                    return;
                }else if (row == POFFSET_CLIP_GENERATOR_ROW && ptr->getSignature() != HKB_CLIP_GENERATOR){
                    WARNING_MESSAGE(QString("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\nInvalid object type selected! You must select a clip generator for the 'pOffsetClipGenerator' data field!!!"));
                    return;
                }else if (ptr == bsData || !behaviorView->reconnectIcon(behaviorView->getSelectedItem(), gen, ptr, false)){
                    WARNING_MESSAGE(QString("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\nYou are attempting to create a circular branch or dead end!!!"));
                    return;
                }
            }else{
                if (behaviorView->getSelectedItem()){
                    behaviorView->removeItemFromGraph(behaviorView->getSelectedItem()->getChildWithData(gen), indexOfGenerator);
                }else{
                    CRITICAL_ERROR_MESSAGE(QString("BSOffsetAnimationGeneratorUI::setGenerator(): The selected icon is NULL!!"));
                    return;
                }
            }
            behaviorView->removeGeneratorData();
            table->item(row, VALUE_COLUMN)->setText(name);
            bsData->getParentFile()->toggleChanged(true);
        }else{
            CRITICAL_ERROR_MESSAGE(QString("BSOffsetAnimationGeneratorUI::setGenerator(): The 'behaviorView' pointer is NULL!!"))
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSOffsetAnimationGeneratorUI::setGenerator(): The 'bsData' pointer is NULL!!"))
    }
}

void BSOffsetAnimationGeneratorUI::viewSelected(int row, int column){
    if (bsData){
        bool properties = false;
        if (column == BINDING_COLUMN){
            switch (row){
            case FOFFSET_VARIABLE_ROW:
                if (table->item(FOFFSET_VARIABLE_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "fOffsetVariable");
                break;
            case FOFFSET_RANGE_START_ROW:
                if (table->item(FOFFSET_RANGE_START_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "fOffsetRangeStart");
                break;
            case FOFFSET_RANGE_END_ROW:
                if (table->item(FOFFSET_RANGE_END_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "fOffsetRangeEnd");
                break;
            }
        }else if (column == VALUE_COLUMN){
            if (row == PDEFAULT_GENERATOR_ROW){
                emit viewGenerators(static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData->pDefaultGenerator) + 1);
            }else if (row == POFFSET_CLIP_GENERATOR_ROW){
                emit viewGenerators(static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData->pOffsetClipGenerator) + 1);
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSOffsetAnimationGeneratorUI::viewSelected(): The 'bsData' pointer is NULL!!"))
    }
}

void BSOffsetAnimationGeneratorUI::selectTableToView(bool viewproperties, const QString & path){
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
        CRITICAL_ERROR_MESSAGE(QString("BSOffsetAnimationGeneratorUI::selectTableToView(): The data is NULL!!"));
    }
}

void BSOffsetAnimationGeneratorUI::variableRenamed(const QString & name, int index){
    if (bsData){
        index--;
        hkbVariableBindingSet *bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (bind){
            int bindIndex = bind->getVariableIndexOfBinding("fOffsetVariable");
            if (bindIndex == index){
                table->item(FOFFSET_VARIABLE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("fOffsetRangeStart");
            if (bindIndex == index){
                table->item(FOFFSET_RANGE_START_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("fOffsetRangeEnd");
            if (bindIndex == index){
                table->item(FOFFSET_RANGE_END_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSOffsetAnimationGeneratorUI::variableRenamed(): The 'bsData' pointer is NULL!!"))
    }
}

void BSOffsetAnimationGeneratorUI::generatorRenamed(const QString & name, int index){
    if (bsData){
        index--;
        if (index == static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData->pDefaultGenerator)){
            table->item(PDEFAULT_GENERATOR_ROW, VALUE_COLUMN)->setText(name);
        }
        if (index == static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData->pOffsetClipGenerator)){
            table->item(POFFSET_CLIP_GENERATOR_ROW, VALUE_COLUMN)->setText(name);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("BSOffsetAnimationGeneratorUI::generatorRenamed(): The 'bsData' pointer is NULL!!"));
    }
}

void BSOffsetAnimationGeneratorUI::setBehaviorView(BehaviorGraphView *view){
    behaviorView = view;
}
