#include "bscyclicblendtransitiongeneratorui.h"

#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/generators/bscyclicblendtransitiongenerator.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/ui/hkxclassesui/behaviorui/eventui.h"
#include "src/ui/genericdatawidgets.h"
#include "src/ui/behaviorgraphview.h"
#include "src/ui/treegraphicsitem.h"
#include "src/ui/mainwindow.h"

#include <QGridLayout>

#include "src/ui/genericdatawidgets.h"
#include <QStackedLayout>
#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 7

#define NAME_ROW 0
#define EVENT_TO_FREEZE_BLEND_VALUE_ROW 1
#define EVENT_TO_CROSS_BLEND_ROW 2
#define BLEND_PARAMETER_ROW 3
#define TRANSITION_DURATION_ROW 4
#define BLEND_CURVE_ROW 5
#define BLENDER_GENERATOR_ROW 6

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList BSCyclicBlendTransitionGeneratorUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

BSCyclicBlendTransitionGeneratorUI::BSCyclicBlendTransitionGeneratorUI()
    : behaviorView(nullptr),
      bsData(nullptr),
      groupBox(new QGroupBox("BSCyclicBlendTransitionGenerator")),
      eventUI(new EventUI),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      eventToFreezeBlendValue(new CheckButtonCombo("Edit")),
      eventToCrossBlend(new CheckButtonCombo("Edit")),
      fBlendParameter(new DoubleSpinBox),
      fTransitionDuration(new DoubleSpinBox),
      eBlendCurve(new ComboBox)
{
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(NAME_ROW, NAME_COLUMN, new TableWidgetItem("name"));
    table->setItem(NAME_ROW, TYPE_COLUMN, new TableWidgetItem("hkStringPtr", Qt::AlignCenter));
    table->setItem(NAME_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(NAME_ROW, VALUE_COLUMN, name);
    table->setItem(EVENT_TO_FREEZE_BLEND_VALUE_ROW, NAME_COLUMN, new TableWidgetItem("eventToFreezeBlendValue"));
    table->setItem(EVENT_TO_FREEZE_BLEND_VALUE_ROW, TYPE_COLUMN, new TableWidgetItem("hkEvent", Qt::AlignCenter));
    table->setItem(EVENT_TO_FREEZE_BLEND_VALUE_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(EVENT_TO_FREEZE_BLEND_VALUE_ROW, VALUE_COLUMN, new TableWidgetItem("", Qt::AlignCenter, QColor(Qt::gray)));
    table->setCellWidget(EVENT_TO_FREEZE_BLEND_VALUE_ROW, VALUE_COLUMN, eventToFreezeBlendValue);
    table->setItem(EVENT_TO_CROSS_BLEND_ROW, NAME_COLUMN, new TableWidgetItem("eventToCrossBlend"));
    table->setItem(EVENT_TO_CROSS_BLEND_ROW, TYPE_COLUMN, new TableWidgetItem("hkEvent", Qt::AlignCenter));
    table->setItem(EVENT_TO_CROSS_BLEND_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(EVENT_TO_CROSS_BLEND_ROW, VALUE_COLUMN, new TableWidgetItem("", Qt::AlignCenter, QColor(Qt::gray)));
    table->setCellWidget(EVENT_TO_CROSS_BLEND_ROW, VALUE_COLUMN, eventToCrossBlend);
    table->setItem(BLEND_PARAMETER_ROW, NAME_COLUMN, new TableWidgetItem("fBlendParameter"));
    table->setItem(BLEND_PARAMETER_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(BLEND_PARAMETER_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(BLEND_PARAMETER_ROW, VALUE_COLUMN, fBlendParameter);
    table->setItem(TRANSITION_DURATION_ROW, NAME_COLUMN, new TableWidgetItem("fTransitionDuration"));
    table->setItem(TRANSITION_DURATION_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(TRANSITION_DURATION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(TRANSITION_DURATION_ROW, VALUE_COLUMN, fTransitionDuration);
    table->setItem(BLEND_CURVE_ROW, NAME_COLUMN, new TableWidgetItem("eBlendCurve"));
    table->setItem(BLEND_CURVE_ROW, TYPE_COLUMN, new TableWidgetItem("BlendCurve", Qt::AlignCenter));
    table->setItem(BLEND_CURVE_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(BLEND_CURVE_ROW, VALUE_COLUMN, eBlendCurve);
    table->setItem(BLENDER_GENERATOR_ROW, NAME_COLUMN, new TableWidgetItem("pBlenderGenerator"));
    table->setItem(BLENDER_GENERATOR_ROW, TYPE_COLUMN, new TableWidgetItem("hkbBlenderGenerator", Qt::AlignCenter));
    table->setItem(BLENDER_GENERATOR_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(BLENDER_GENERATOR_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_GENERATORS_TABLE_TIP));
    topLyt->addWidget(table, 1, 0, 8, 3);
    groupBox->setLayout(topLyt);
    //Order here must correspond with the ACTIVE_WIDGET Enumerated type!!!
    addWidget(groupBox);
    addWidget(eventUI);
    connectSignals();
}

void BSCyclicBlendTransitionGeneratorUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(fBlendParameter, SIGNAL(editingFinished()), this, SLOT(setBlendParameter()), Qt::UniqueConnection);
    connect(fTransitionDuration, SIGNAL(editingFinished()), this, SLOT(setTransitionDuration()), Qt::UniqueConnection);
    connect(eventToFreezeBlendValue, SIGNAL(pressed()), this, SLOT(viewEventToFreezeBlendValue()), Qt::UniqueConnection);
    connect(eventToFreezeBlendValue, SIGNAL(enabled(bool)), this, SLOT(toggleEventToFreezeBlendValue(bool)), Qt::UniqueConnection);
    connect(eventToCrossBlend, SIGNAL(pressed()), this, SLOT(viewEventToCrossBlend()), Qt::UniqueConnection);
    connect(eventToCrossBlend, SIGNAL(enabled(bool)), this, SLOT(toggleEventToCrossBlend(bool)), Qt::UniqueConnection);
    connect(eBlendCurve, SIGNAL(currentIndexChanged(int)), this, SLOT(setBlendCurve(int)), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)), Qt::UniqueConnection);
    connect(eventUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()), Qt::UniqueConnection);
    connect(eventUI, SIGNAL(viewEvents(int)), this, SIGNAL(viewEvents(int)), Qt::UniqueConnection);
}

void BSCyclicBlendTransitionGeneratorUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(fBlendParameter, SIGNAL(editingFinished()), this, SLOT(setBlendParameter()));
    disconnect(fTransitionDuration, SIGNAL(editingFinished()), this, SLOT(setTransitionDuration()));
    disconnect(eventToFreezeBlendValue, SIGNAL(pressed()), this, SLOT(viewEventToFreezeBlendValue()));
    disconnect(eventToFreezeBlendValue, SIGNAL(enabled(bool)), this, SLOT(toggleEventToFreezeBlendValue(bool)));
    disconnect(eventToCrossBlend, SIGNAL(pressed()), this, SLOT(viewEventToCrossBlend()));
    disconnect(eventToCrossBlend, SIGNAL(enabled(bool)), this, SLOT(toggleEventToCrossBlend(bool)));
    disconnect(eBlendCurve, SIGNAL(currentIndexChanged(int)), this, SLOT(setBlendCurve(int)));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)));
    disconnect(eventUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()));
    disconnect(eventUI, SIGNAL(viewEvents(int)), this, SIGNAL(viewEvents(int)));
}

void BSCyclicBlendTransitionGeneratorUI::loadData(HkxObject *data){
    disconnectSignals();
    setCurrentIndex(MAIN_WIDGET);
    if (data){
        if (data->getSignature() == BS_CYCLIC_BLEND_TRANSITION_GENERATOR){
            bsData = static_cast<BSCyclicBlendTransitionGenerator *>(data);
            name->setText(bsData->name);
            if (bsData->eventToFreezeBlendValue.id > -1){
                eventToFreezeBlendValue->setChecked(true);
            }else{
                eventToFreezeBlendValue->setChecked(false);
            }
            if (bsData->eventToCrossBlend.id > -1){
                eventToCrossBlend->setChecked(true);
            }else{
                eventToCrossBlend->setChecked(false);
            }
            if (eBlendCurve->count() == 0){
                eBlendCurve->insertItems(0, bsData->BlendCurve);
            }
            eBlendCurve->setCurrentIndex(bsData->BlendCurve.indexOf(bsData->eBlendCurve));
            if (bsData->pBlenderGenerator.data()){
                table->item(BLENDER_GENERATOR_ROW, VALUE_COLUMN)->setText(static_cast<hkbGenerator *>(bsData->pBlenderGenerator.data())->getName());
            }else{
                table->item(BLENDER_GENERATOR_ROW, VALUE_COLUMN)->setText("NONE");
            }
            hkbVariableBindingSet *varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (varBind){
                loadBinding(BLEND_PARAMETER_ROW, BINDING_COLUMN, varBind, "fBlendParameter");
                loadBinding(TRANSITION_DURATION_ROW, BINDING_COLUMN, varBind, "fTransitionDuration");
            }else{
                table->item(BLEND_PARAMETER_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(TRANSITION_DURATION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
        }else{
            FATAL_RUNTIME_ERROR(QString("BSCyclicBlendTransitionGeneratorUI::loadData(): The data passed to the UI is the wrong type!\nSIGNATURE: "+QString::number(data->getSignature(), 16)).toLocal8Bit().data());
        }
    }else{
        FATAL_RUNTIME_ERROR("BSCyclicBlendTransitionGeneratorUI::loadData(): Attempting to load a null pointer!!");
    }
    connectSignals();
}

void BSCyclicBlendTransitionGeneratorUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            emit generatorNameChanged(bsData->name, static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData));
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("BSCyclicBlendTransitionGeneratorUI::setName(): The data is nullptr!!");
    }
}

void BSCyclicBlendTransitionGeneratorUI::setBlendParameter(){
    if (bsData){
        if (bsData->fBlendParameter != fBlendParameter->value()){
            bsData->fBlendParameter = fBlendParameter->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("BSOffsetAnimationGeneratorUI::setBlendParameter(): The data is nullptr!!");
    }
}

void BSCyclicBlendTransitionGeneratorUI::setTransitionDuration(){
    if (bsData){
        if (bsData->fTransitionDuration != fTransitionDuration->value()){
            bsData->fTransitionDuration = fTransitionDuration->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("BSOffsetAnimationGeneratorUI::setTransitionDuration(): The data is nullptr!!");
    }
}

void BSCyclicBlendTransitionGeneratorUI::setBlendCurve(int index){
    if (bsData){
        bsData->eBlendCurve = bsData->BlendCurve.at(index);
        bsData->getParentFile()->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("BSCyclicBlendTransitionGeneratorUI::setBlendCurve(): The data is nullptr!!");
    }
}

void BSCyclicBlendTransitionGeneratorUI::viewEventToFreezeBlendValue(){
    if (bsData){
        eventUI->loadData(static_cast<BehaviorFile *>(bsData->getParentFile()), &bsData->eventToFreezeBlendValue);
        setCurrentIndex(EVENT_WIDGET);
    }else{
        FATAL_RUNTIME_ERROR("BSCyclicBlendTransitionGeneratorUI::viewEventToFreezeBlendValue(): The data is nullptr!!");
    }
}

void BSCyclicBlendTransitionGeneratorUI::toggleEventToFreezeBlendValue(bool enable){
    if (bsData){
        if (!enable){
            bsData->eventToFreezeBlendValue.id = -1;
            bsData->eventToFreezeBlendValue.payload = HkxSharedPtr();
            static_cast<BehaviorFile *>(bsData->getParentFile())->removeOtherData();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("BSCyclicBlendTransitionGeneratorUI::toggleEventToFreezeBlendValue(): The data is nullptr!!");
    }
}

void BSCyclicBlendTransitionGeneratorUI::viewEventToCrossBlend(){
    if (bsData){
        eventUI->loadData(static_cast<BehaviorFile *>(bsData->getParentFile()), &bsData->eventToCrossBlend);
        setCurrentIndex(EVENT_WIDGET);
    }else{
        FATAL_RUNTIME_ERROR("BSCyclicBlendTransitionGeneratorUI::viewEventToCrossBlend(): The data is nullptr!!");
    }
}

void BSCyclicBlendTransitionGeneratorUI::toggleEventToCrossBlend(bool enable){
    if (bsData){
        if (!enable){
            bsData->eventToCrossBlend.id = -1;
            bsData->eventToCrossBlend.payload = HkxSharedPtr();
            static_cast<BehaviorFile *>(bsData->getParentFile())->removeOtherData();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("BSCyclicBlendTransitionGeneratorUI::toggleEventToCrossBlend(): The data is nullptr!!");
    }
}

void BSCyclicBlendTransitionGeneratorUI::selectTableToView(bool viewproperties, const QString & path){
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
        FATAL_RUNTIME_ERROR("BSOffsetAnimationGeneratorUI::selectTableToView(): The data is nullptr!!");
    }
}

void BSCyclicBlendTransitionGeneratorUI::viewSelectedChild(int row, int column){
    if (bsData){
        bool properties = false;
        if (column == BINDING_COLUMN){
            switch (row){
            case BLEND_PARAMETER_ROW:
                if (table->item(BLEND_PARAMETER_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "fBlendParameter");
                break;
            case TRANSITION_DURATION_ROW:
                if (table->item(TRANSITION_DURATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "fTransitionDuration");
                break;
            }
        }else if (column == VALUE_COLUMN){
            if (row == BLENDER_GENERATOR_ROW){
                emit viewGenerators(static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData->pBlenderGenerator) + 1);
            }
        }
    }else{
        FATAL_RUNTIME_ERROR("BSCyclicBlendTransitionGeneratorUI::viewSelectedChild(): The data is nullptr!!");
    }
}

void BSCyclicBlendTransitionGeneratorUI::setBlenderGenerator(int index, const QString & name){
    DataIconManager *ptr = nullptr;
    int indexOfGenerator = -1;
    if (bsData){
        if (behaviorView){
            ptr = static_cast<BehaviorFile *>(bsData->getParentFile())->getGeneratorDataAt(index - 1);
            indexOfGenerator = bsData->getIndexOfObj(static_cast<DataIconManager*>(bsData->pBlenderGenerator.data()));
            if (ptr){
                if (name != ptr->getName()){
                    FATAL_RUNTIME_ERROR("::setDefaultGenerator():The name of the selected object does not match it's name in the object selection table!!!");
                    return;
                }else if (ptr->getSignature() != HKB_BLENDER_GENERATOR){
                    WARNING_MESSAGE("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\nInvalid object type selected! You must select a blender generator for the 'pBlenderGenerator' data field!!!");
                    return;
                }else if (ptr == bsData || !behaviorView->reconnectIcon(behaviorView->getSelectedItem(), static_cast<DataIconManager*>(bsData->pBlenderGenerator.data()), ptr, false)){
                    WARNING_MESSAGE("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\nYou are attempting to create a circular branch or dead end!!!");
                    return;
                }
            }else{
                if (behaviorView->getSelectedItem()){
                    behaviorView->removeItemFromGraph(behaviorView->getSelectedItem()->getChildWithData(static_cast<DataIconManager*>(bsData->pBlenderGenerator.data())), indexOfGenerator);
                }else{
                    FATAL_RUNTIME_ERROR("BSCyclicBlendTransitionGeneratorUI::setBlenderGenerator(): The selected icon is nullptr!!");
                    return;
                }
            }
            behaviorView->removeGeneratorData();
            table->item(BLENDER_GENERATOR_ROW, VALUE_COLUMN)->setText(name);
            bsData->getParentFile()->setIsChanged(true);
        }else{
            FATAL_RUNTIME_ERROR("BSCyclicBlendTransitionGeneratorUI::setBlenderGenerator(): The 'behaviorView' pointer is nullptr!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("BSCyclicBlendTransitionGeneratorUI::setBlenderGenerator(): The 'bsData' pointer is nullptr!!");
    }
}

void BSCyclicBlendTransitionGeneratorUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
    if (bsData){
        if (varBind){
            int index = varBind->getVariableIndexOfBinding(path);
            QString varName;
            if (index != -1){
                if (varBind->getBindingType(path) == hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY){
                    varName = static_cast<BehaviorFile *>(bsData->getParentFile())->getCharacterPropertyNameAt(index, false);
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
            FATAL_RUNTIME_ERROR("BSCyclicBlendTransitionGeneratorUI::loadBinding(): The variable binding set is nullptr!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("BSCyclicBlendTransitionGeneratorUI::loadBinding(): The data is nullptr!!");
    }
}

bool BSCyclicBlendTransitionGeneratorUI::setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty){
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
                varBind->addBinding(path, index - 1,hkbVariableBindingSet::hkBinding::BINDING_TYPE_CHARACTER_PROPERTY);
            }else{
                varBind->addBinding(path, index - 1,hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE);
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->setIsChanged(true);
        }else{
            WARNING_MESSAGE("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\nYou are attempting to bind a variable of an invalid type for this data field!!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("BSCyclicBlendTransitionGeneratorUI::setBinding(): The 'bsData' pointer is nullptr!!");
        return false;
    }
    return true;
}

void BSCyclicBlendTransitionGeneratorUI::setBindingVariable(int index, const QString & name){
    if (bsData){
        bool isProperty = false;
        int row = table->currentRow();
        switch (row){
        case BLEND_PARAMETER_ROW:
            if (table->item(BLEND_PARAMETER_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "fBlendParameter", VARIABLE_TYPE_REAL, isProperty);
            break;
        case TRANSITION_DURATION_ROW:
            if (table->item(TRANSITION_DURATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "fTransitionDuration", VARIABLE_TYPE_REAL, isProperty);
            break;
        default:
            return;
        }
        bsData->getParentFile()->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("BSCyclicBlendTransitionGeneratorUI::setBindingVariable(): The 'bsData' pointer is nullptr!!");
    }
}

void BSCyclicBlendTransitionGeneratorUI::returnToWidget(){
    setCurrentIndex(MAIN_WIDGET);
}

void BSCyclicBlendTransitionGeneratorUI::connectToTables(GenericTableWidget *generators, GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *events){
    if (generators && variables && events && properties){
        disconnect(events, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(generators, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(events, SIGNAL(elementSelected(int,QString)), eventUI, SLOT(setEvent(int,QString)), Qt::UniqueConnection);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(generators, SIGNAL(elementSelected(int,QString)), this, SLOT(setBlenderGenerator(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewGenerators(int)), generators, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int)), variables, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int)), properties, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewEvents(int)), events, SLOT(showTable(int)), Qt::UniqueConnection);
    }else{
        FATAL_RUNTIME_ERROR("BSCyclicBlendTransitionGeneratorUI::connectToTables(): One or more arguments are nullptr!!");
    }
}

void BSCyclicBlendTransitionGeneratorUI::eventRenamed(const QString & name, int index){
    if (bsData){
        if (currentIndex() == EVENT_WIDGET){
            eventUI->eventRenamed(name, index);
        }
    }else{
        FATAL_RUNTIME_ERROR("BSCyclicBlendTransitionGeneratorUI::eventRenamed(): The data is nullptr!!");
    }
}

void BSCyclicBlendTransitionGeneratorUI::variableRenamed(const QString & name, int index){
    int bindIndex = -1;
    hkbVariableBindingSet *bind = nullptr;
    if (name == ""){
        WARNING_MESSAGE("BSCyclicBlendTransitionGeneratorUI::variableRenamed(): The new variable name is the empty string!!");
    }
    if (bsData){
        index--;
        bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (bind){
            bindIndex = bind->getVariableIndexOfBinding("fBlendParameter");
            if (bindIndex == index){
                table->item(BLEND_PARAMETER_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+name);
            }
            bindIndex = bind->getVariableIndexOfBinding("fTransitionDuration");
            if (bindIndex == index){
                table->item(TRANSITION_DURATION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+name);
            }
        }
    }else{
        FATAL_RUNTIME_ERROR("BSCyclicBlendTransitionGeneratorUI::variableRenamed(): The data is nullptr!!");
    }
}

void BSCyclicBlendTransitionGeneratorUI::generatorRenamed(const QString &name, int index){
    if (name == ""){
        WARNING_MESSAGE("BSCyclicBlendTransitionGeneratorUI::generatorRenamed(): The new variable name is the empty string!!");
    }
    index--;
    if (index == static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData->pBlenderGenerator)){
        table->item(BLENDER_GENERATOR_ROW, VALUE_COLUMN)->setText(name);
    }
}

void BSCyclicBlendTransitionGeneratorUI::setBehaviorView(BehaviorGraphView *view){
    behaviorView = view;
    setCurrentIndex(MAIN_WIDGET);
}

