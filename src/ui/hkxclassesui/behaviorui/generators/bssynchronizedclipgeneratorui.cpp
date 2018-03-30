#include "bssynchronizedclipgeneratorui.h"

#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/generators/bssynchronizedclipgenerator.h"
#include "src/hkxclasses/behavior/generators/hkbclipgenerator.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/ui/hkxclassesui/behaviorui/eventui.h"
#include "src/ui/genericdatawidgets.h"
#include "src/ui/behaviorgraphview.h"
#include "src/ui/treegraphicsitem.h"
#include "src/ui/mainwindow.h"

#include <QGridLayout>

#include "src/ui/genericdatawidgets.h"
#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 10

#define NAME_ROW 0
#define CLIP_GENERATOR_ROW 1
#define SYNC_ANIM_PREFIX_ROW 2
#define SYNC_CLIP_IGNORE_MARK_PLACEMENT_ROW 3
#define GET_TO_MARK_TIME_ROW 4
#define MARK_ERROR_THRESHOLD_ROW 5
#define LEAD_CHARACTER_ROW 6
#define REORIENT_SUPPORT_CHAR_ROW 7
#define APPLY_MOTION_FROM_ROOT_ROW 8
#define ANIMATION_BINDING_INDEX_ROW 9

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList BSSynchronizedClipGeneratorUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

BSSynchronizedClipGeneratorUI::BSSynchronizedClipGeneratorUI()
    : behaviorView(nullptr),
      bsData(nullptr),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      syncAnimPrefix(new LineEdit),
      bSyncClipIgnoreMarkPlacement(new CheckBox),
      fGetToMarkTime(new DoubleSpinBox),
      fMarkErrorThreshold(new DoubleSpinBox),
      bLeadCharacter(new CheckBox),
      bReorientSupportChar(new CheckBox),
      bApplyMotionFromRoot(new CheckBox),
      sAnimationBindingIndex(new SpinBox)
{
    setTitle("BSSynchronizedClipGeneratorUI");
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(NAME_ROW, NAME_COLUMN, new TableWidgetItem("name"));
    table->setItem(NAME_ROW, TYPE_COLUMN, new TableWidgetItem("hkStringPtr", Qt::AlignCenter));
    table->setItem(NAME_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(NAME_ROW, VALUE_COLUMN, name);
    table->setItem(CLIP_GENERATOR_ROW, NAME_COLUMN, new TableWidgetItem("pClipGenerator"));
    table->setItem(CLIP_GENERATOR_ROW, TYPE_COLUMN, new TableWidgetItem("hkbClipGenerator", Qt::AlignCenter));
    table->setItem(CLIP_GENERATOR_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(CLIP_GENERATOR_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_GENERATORS_TABLE_TIP));
    table->setItem(SYNC_ANIM_PREFIX_ROW, NAME_COLUMN, new TableWidgetItem("syncAnimPrefix"));
    table->setItem(SYNC_ANIM_PREFIX_ROW, TYPE_COLUMN, new TableWidgetItem("hkStringPtr", Qt::AlignCenter));
    table->setItem(SYNC_ANIM_PREFIX_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(SYNC_ANIM_PREFIX_ROW, VALUE_COLUMN, syncAnimPrefix);
    table->setItem(SYNC_CLIP_IGNORE_MARK_PLACEMENT_ROW, NAME_COLUMN, new TableWidgetItem("bSyncClipIgnoreMarkPlacement"));
    table->setItem(SYNC_CLIP_IGNORE_MARK_PLACEMENT_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(SYNC_CLIP_IGNORE_MARK_PLACEMENT_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setItem(SYNC_CLIP_IGNORE_MARK_PLACEMENT_ROW, VALUE_COLUMN, new TableWidgetItem("", Qt::AlignCenter));
    table->setCellWidget(SYNC_CLIP_IGNORE_MARK_PLACEMENT_ROW, VALUE_COLUMN, bSyncClipIgnoreMarkPlacement);
    table->setItem(GET_TO_MARK_TIME_ROW, NAME_COLUMN, new TableWidgetItem("fGetToMarkTime"));
    table->setItem(GET_TO_MARK_TIME_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(GET_TO_MARK_TIME_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(GET_TO_MARK_TIME_ROW, VALUE_COLUMN, fGetToMarkTime);
    table->setItem(MARK_ERROR_THRESHOLD_ROW, NAME_COLUMN, new TableWidgetItem("fMarkErrorThreshold"));
    table->setItem(MARK_ERROR_THRESHOLD_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(MARK_ERROR_THRESHOLD_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(MARK_ERROR_THRESHOLD_ROW, VALUE_COLUMN, fMarkErrorThreshold);
    table->setItem(LEAD_CHARACTER_ROW, NAME_COLUMN, new TableWidgetItem("bLeadCharacter"));
    table->setItem(LEAD_CHARACTER_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(LEAD_CHARACTER_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setItem(LEAD_CHARACTER_ROW, VALUE_COLUMN, new TableWidgetItem("", Qt::AlignCenter));
    table->setCellWidget(LEAD_CHARACTER_ROW, VALUE_COLUMN, bLeadCharacter);
    table->setItem(REORIENT_SUPPORT_CHAR_ROW, NAME_COLUMN, new TableWidgetItem("bReorientSupportChar"));
    table->setItem(REORIENT_SUPPORT_CHAR_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(REORIENT_SUPPORT_CHAR_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setItem(REORIENT_SUPPORT_CHAR_ROW, VALUE_COLUMN, new TableWidgetItem("", Qt::AlignCenter));
    table->setCellWidget(REORIENT_SUPPORT_CHAR_ROW, VALUE_COLUMN, bReorientSupportChar);
    table->setItem(APPLY_MOTION_FROM_ROOT_ROW, NAME_COLUMN, new TableWidgetItem("bApplyMotionFromRoot"));
    table->setItem(APPLY_MOTION_FROM_ROOT_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(APPLY_MOTION_FROM_ROOT_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setItem(APPLY_MOTION_FROM_ROOT_ROW, VALUE_COLUMN, new TableWidgetItem("", Qt::AlignCenter));
    table->setCellWidget(APPLY_MOTION_FROM_ROOT_ROW, VALUE_COLUMN, bApplyMotionFromRoot);
    table->setItem(ANIMATION_BINDING_INDEX_ROW, NAME_COLUMN, new TableWidgetItem("sAnimationBindingIndex"));
    table->setItem(ANIMATION_BINDING_INDEX_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(ANIMATION_BINDING_INDEX_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ANIMATION_BINDING_INDEX_ROW, VALUE_COLUMN, sAnimationBindingIndex);
    topLyt->addWidget(table, 1, 0, 8, 3);
    setLayout(topLyt);
    connectSignals();
}

void BSSynchronizedClipGeneratorUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(syncAnimPrefix, SIGNAL(editingFinished()), this, SLOT(setSyncAnimPrefix()), Qt::UniqueConnection);
    connect(bSyncClipIgnoreMarkPlacement, SIGNAL(released()), this, SLOT(setSyncClipIgnoreMarkPlacement()), Qt::UniqueConnection);
    connect(fGetToMarkTime, SIGNAL(editingFinished()), this, SLOT(setGetToMarkTime()), Qt::UniqueConnection);
    connect(fMarkErrorThreshold, SIGNAL(editingFinished()), this, SLOT(setMarkErrorThreshold()), Qt::UniqueConnection);
    connect(bLeadCharacter, SIGNAL(released()), this, SLOT(setLeadCharacter()), Qt::UniqueConnection);
    connect(bReorientSupportChar, SIGNAL(released()), this, SLOT(setReorientSupportChar()), Qt::UniqueConnection);
    connect(bApplyMotionFromRoot, SIGNAL(released()), this, SLOT(setApplyMotionFromRoot()), Qt::UniqueConnection);
    connect(sAnimationBindingIndex, SIGNAL(editingFinished()), this, SLOT(setAnimationBindingIndex()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)), Qt::UniqueConnection);
}

void BSSynchronizedClipGeneratorUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(syncAnimPrefix, SIGNAL(editingFinished()), this, SLOT(setSyncAnimPrefix()));
    disconnect(bSyncClipIgnoreMarkPlacement, SIGNAL(released()), this, SLOT(setSyncClipIgnoreMarkPlacement()));
    disconnect(fGetToMarkTime, SIGNAL(editingFinished()), this, SLOT(setGetToMarkTime()));
    disconnect(fMarkErrorThreshold, SIGNAL(editingFinished()), this, SLOT(setMarkErrorThreshold()));
    disconnect(bLeadCharacter, SIGNAL(released()), this, SLOT(setLeadCharacter()));
    disconnect(bReorientSupportChar, SIGNAL(released()), this, SLOT(setReorientSupportChar()));
    disconnect(bApplyMotionFromRoot, SIGNAL(released()), this, SLOT(setApplyMotionFromRoot()));
    disconnect(sAnimationBindingIndex, SIGNAL(editingFinished()), this, SLOT(setAnimationBindingIndex()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)));
}

void BSSynchronizedClipGeneratorUI::loadData(HkxObject *data){
    disconnectSignals();
    if (data){
        if (data->getSignature() == BS_SYNCHRONIZED_CLIP_GENERATOR){
            bsData = static_cast<BSSynchronizedClipGenerator *>(data);
            name->setText(bsData->name);
            syncAnimPrefix->setText(bsData->syncAnimPrefix);
            bSyncClipIgnoreMarkPlacement->setChecked(bsData->bSyncClipIgnoreMarkPlacement);
            fGetToMarkTime->setValue(bsData->fGetToMarkTime);
            fMarkErrorThreshold->setValue(bsData->fMarkErrorThreshold);
            bLeadCharacter->setChecked(bsData->bLeadCharacter);
            bReorientSupportChar->setChecked(bsData->bReorientSupportChar);
            bApplyMotionFromRoot->setChecked(bsData->bApplyMotionFromRoot);
            sAnimationBindingIndex->setValue(bsData->sAnimationBindingIndex);
            if (bsData->pClipGenerator.data()){
                table->item(CLIP_GENERATOR_ROW, VALUE_COLUMN)->setText(static_cast<hkbGenerator *>(bsData->pClipGenerator.data())->getName());
            }else{
                table->item(CLIP_GENERATOR_ROW, VALUE_COLUMN)->setText("NONE");
            }
            hkbVariableBindingSet *varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (varBind){
                loadBinding(SYNC_CLIP_IGNORE_MARK_PLACEMENT_ROW, BINDING_COLUMN, varBind, "bSyncClipIgnoreMarkPlacement");
                loadBinding(GET_TO_MARK_TIME_ROW, BINDING_COLUMN, varBind, "fGetToMarkTime");
                loadBinding(MARK_ERROR_THRESHOLD_ROW, BINDING_COLUMN, varBind, "fMarkErrorThreshold");
                loadBinding(LEAD_CHARACTER_ROW, BINDING_COLUMN, varBind, "bLeadCharacter");
                loadBinding(REORIENT_SUPPORT_CHAR_ROW, BINDING_COLUMN, varBind, "bReorientSupportChar");
                loadBinding(APPLY_MOTION_FROM_ROOT_ROW, BINDING_COLUMN, varBind, "bApplyMotionFromRoot");
                loadBinding(ANIMATION_BINDING_INDEX_ROW, BINDING_COLUMN, varBind, "sAnimationBindingIndex");
            }else{
                table->item(SYNC_CLIP_IGNORE_MARK_PLACEMENT_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(GET_TO_MARK_TIME_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(MARK_ERROR_THRESHOLD_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(LEAD_CHARACTER_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(REORIENT_SUPPORT_CHAR_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(APPLY_MOTION_FROM_ROOT_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(ANIMATION_BINDING_INDEX_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
        }else{
            FATAL_RUNTIME_ERROR(QString("BSSynchronizedClipGeneratorUI::loadData(): The data passed to the UI is the wrong type!\nSIGNATURE: "+QString::number(data->getSignature(), 16)).toLocal8Bit().data());
        }
    }else{
        FATAL_RUNTIME_ERROR("BSSynchronizedClipGeneratorUI::loadData(): Attempting to load a null pointer!!");
    }
    connectSignals();
}

void BSSynchronizedClipGeneratorUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            emit generatorNameChanged(bsData->name, static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData));
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("BSSynchronizedClipGeneratorUI::setName(): The data is nullptr!!");
    }
}

void BSSynchronizedClipGeneratorUI::setSyncAnimPrefix(){
    if (bsData){
        if (bsData->syncAnimPrefix != syncAnimPrefix->text()){
            bsData->syncAnimPrefix = syncAnimPrefix->text();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("BSSynchronizedClipGeneratorUI::setSyncAnimPrefix(): The data is nullptr!!");
    }
}

void BSSynchronizedClipGeneratorUI::setSyncClipIgnoreMarkPlacement(){
    if (bsData){
        if (bsData->bSyncClipIgnoreMarkPlacement != bSyncClipIgnoreMarkPlacement->isChecked()){
            bsData->bSyncClipIgnoreMarkPlacement = bSyncClipIgnoreMarkPlacement->isChecked();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("BSOffsetAnimationGeneratorUI::setSyncClipIgnoreMarkPlacement(): The data is nullptr!!");
    }
}

void BSSynchronizedClipGeneratorUI::setGetToMarkTime(){
    if (bsData){
        if (bsData->fGetToMarkTime != fGetToMarkTime->value()){
            bsData->fGetToMarkTime = fGetToMarkTime->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("BSOffsetAnimationGeneratorUI::setGetToMarkTime(): The data is nullptr!!");
    }
}

void BSSynchronizedClipGeneratorUI::setMarkErrorThreshold(){
    if (bsData){
        if (bsData->fMarkErrorThreshold != fMarkErrorThreshold->value()){
            bsData->fMarkErrorThreshold = fMarkErrorThreshold->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("BSOffsetAnimationGeneratorUI::setMarkErrorThreshold(): The data is nullptr!!");
    }
}

void BSSynchronizedClipGeneratorUI::setLeadCharacter(){
    if (bsData){
        if (bsData->bLeadCharacter != bLeadCharacter->isChecked()){
            bsData->bLeadCharacter = bLeadCharacter->isChecked();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("BSOffsetAnimationGeneratorUI::setLeadCharacter(): The data is nullptr!!");
    }
}

void BSSynchronizedClipGeneratorUI::setReorientSupportChar(){
    if (bsData){
        if (bsData->bReorientSupportChar != bReorientSupportChar->isChecked()){
            bsData->bReorientSupportChar = bReorientSupportChar->isChecked();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("BSOffsetAnimationGeneratorUI::setReorientSupportChar(): The data is nullptr!!");
    }
}

void BSSynchronizedClipGeneratorUI::setApplyMotionFromRoot(){
    if (bsData){
        if (bsData->bApplyMotionFromRoot != bApplyMotionFromRoot->isChecked()){
            bsData->bApplyMotionFromRoot = bApplyMotionFromRoot->isChecked();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("BSOffsetAnimationGeneratorUI::setApplyMotionFromRoot(): The data is nullptr!!");
    }
}

void BSSynchronizedClipGeneratorUI::setAnimationBindingIndex(){
    if (bsData){
        if (bsData->sAnimationBindingIndex != sAnimationBindingIndex->value()){
            bsData->sAnimationBindingIndex = sAnimationBindingIndex->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        FATAL_RUNTIME_ERROR("BSOffsetAnimationGeneratorUI::setAnimationBindingIndex(): The data is nullptr!!");
    }
}

void BSSynchronizedClipGeneratorUI::selectTableToView(bool viewproperties, const QString & path){
    if (bsData){
        if (viewproperties){
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
        FATAL_RUNTIME_ERROR("BSOffsetAnimationGeneratorUI::selectTableToView(): The data is nullptr!!");
    }
}

void BSSynchronizedClipGeneratorUI::viewSelectedChild(int row, int column){
    if (bsData){
        bool properties = false;
        if (column == BINDING_COLUMN){
            switch (row){
            case SYNC_CLIP_IGNORE_MARK_PLACEMENT_ROW:
                if (table->item(SYNC_CLIP_IGNORE_MARK_PLACEMENT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "bSyncClipIgnoreMarkPlacement");
                break;
            case GET_TO_MARK_TIME_ROW:
                if (table->item(GET_TO_MARK_TIME_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "fGetToMarkTime");
                break;
            case MARK_ERROR_THRESHOLD_ROW:
                if (table->item(MARK_ERROR_THRESHOLD_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "fMarkErrorThreshold");
                break;
            case LEAD_CHARACTER_ROW:
                if (table->item(LEAD_CHARACTER_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "bLeadCharacter");
                break;
            case REORIENT_SUPPORT_CHAR_ROW:
                if (table->item(REORIENT_SUPPORT_CHAR_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "bReorientSupportChar");
                break;
            case APPLY_MOTION_FROM_ROOT_ROW:
                if (table->item(APPLY_MOTION_FROM_ROOT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "bApplyMotionFromRoot");
                break;
            case ANIMATION_BINDING_INDEX_ROW:
                if (table->item(ANIMATION_BINDING_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    properties = true;
                }
                selectTableToView(properties, "sAnimationBindingIndex");
                break;
            }
        }else if (column == VALUE_COLUMN){
            if (row == CLIP_GENERATOR_ROW){
                emit viewGenerators(static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData->pClipGenerator) + 1, hkbClipGenerator::getClassname(), QStringList());
            }
        }
    }else{
        FATAL_RUNTIME_ERROR("BSSynchronizedClipGeneratorUI::viewSelectedChild(): The data is nullptr!!");
    }
}

void BSSynchronizedClipGeneratorUI::setClipGenerator(int index, const QString & name){
    DataIconManager *ptr = nullptr;
    int indexOfGenerator = -1;
    if (bsData){
        if (behaviorView){
            ptr = static_cast<BehaviorFile *>(bsData->getParentFile())->getGeneratorDataAt(index - 1);
            indexOfGenerator = bsData->getIndexOfObj(static_cast<DataIconManager*>(bsData->pClipGenerator.data()));
            if (ptr){
                if (name != ptr->getName()){
                    FATAL_RUNTIME_ERROR("::setDefaultGenerator():The name of the selected object does not match it's name in the object selection table!!!");
                    return;
                }else if (ptr->getSignature() != HKB_CLIP_GENERATOR){
                    WARNING_MESSAGE("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\nInvalid object type selected! You must select a clip generator for the 'pClipGenerator' data field!!!");
                    return;
                }else if (ptr == bsData || !behaviorView->reconnectIcon(behaviorView->getSelectedItem(), static_cast<DataIconManager*>(bsData->pClipGenerator.data()), 0, ptr, false)){
                    WARNING_MESSAGE("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\nYou are attempting to create a circular branch or dead end!!!");
                    return;
                }
            }else{
                if (behaviorView->getSelectedItem()){
                    behaviorView->removeItemFromGraph(behaviorView->getSelectedItem()->getChildWithData(static_cast<DataIconManager*>(bsData->pClipGenerator.data())), indexOfGenerator);
                }else{
                    FATAL_RUNTIME_ERROR("BSSynchronizedClipGeneratorUI::setClipGenerator(): The selected icon is nullptr!!");
                    return;
                }
            }
            behaviorView->removeGeneratorData();
            table->item(CLIP_GENERATOR_ROW, VALUE_COLUMN)->setText(name);
            bsData->getParentFile()->setIsChanged(true);
        }else{
            FATAL_RUNTIME_ERROR("BSSynchronizedClipGeneratorUI::setClipGenerator(): The 'behaviorView' pointer is nullptr!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("BSSynchronizedClipGeneratorUI::setClipGenerator(): The 'bsData' pointer is nullptr!!");
    }
}

void BSSynchronizedClipGeneratorUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
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
            FATAL_RUNTIME_ERROR("BSSynchronizedClipGeneratorUI::loadBinding(): The variable binding set is nullptr!!");
        }
    }else{
        FATAL_RUNTIME_ERROR("BSSynchronizedClipGeneratorUI::loadBinding(): The data is nullptr!!");
    }
}

bool BSSynchronizedClipGeneratorUI::setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty){
    hkbVariableBindingSet *varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
    if (bsData){
        if (index == 0){
            varBind->removeBinding(path);if (varBind->getNumberOfBindings() == 0){static_cast<HkDynamicObject *>(bsData)->variableBindingSet = HkxSharedPtr(); static_cast<BehaviorFile *>(bsData->getParentFile())->removeOtherData();}
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
        FATAL_RUNTIME_ERROR("BSSynchronizedClipGeneratorUI::setBinding(): The 'bsData' pointer is nullptr!!");
        return false;
    }
    return true;
}

void BSSynchronizedClipGeneratorUI::setBindingVariable(int index, const QString & name){
    if (bsData){
        bool isProperty = false;
        int row = table->currentRow();
        switch (row){
        case SYNC_CLIP_IGNORE_MARK_PLACEMENT_ROW:
            if (table->item(SYNC_CLIP_IGNORE_MARK_PLACEMENT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "bSyncClipIgnoreMarkPlacement", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case GET_TO_MARK_TIME_ROW:
            if (table->item(GET_TO_MARK_TIME_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "fGetToMarkTime", VARIABLE_TYPE_REAL, isProperty);
            break;
        case MARK_ERROR_THRESHOLD_ROW:
            if (table->item(MARK_ERROR_THRESHOLD_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "fMarkErrorThreshold", VARIABLE_TYPE_REAL, isProperty);
            break;
        case LEAD_CHARACTER_ROW:
            if (table->item(LEAD_CHARACTER_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "bLeadCharacter", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case REORIENT_SUPPORT_CHAR_ROW:
            if (table->item(REORIENT_SUPPORT_CHAR_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "bReorientSupportChar", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case APPLY_MOTION_FROM_ROOT_ROW:
            if (table->item(APPLY_MOTION_FROM_ROOT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "bApplyMotionFromRoot", VARIABLE_TYPE_BOOL, isProperty);
            break;
        case ANIMATION_BINDING_INDEX_ROW:
            if (table->item(ANIMATION_BINDING_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "sAnimationBindingIndex", VARIABLE_TYPE_INT32, isProperty);
            break;
        default:
            return;
        }
        bsData->getParentFile()->setIsChanged(true);
    }else{
        FATAL_RUNTIME_ERROR("BSSynchronizedClipGeneratorUI::setBindingVariable(): The 'bsData' pointer is nullptr!!");
    }
}

void BSSynchronizedClipGeneratorUI::connectToTables(GenericTableWidget *generators, GenericTableWidget *variables, GenericTableWidget *properties){
    if (generators && variables && properties){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(generators, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(generators, SIGNAL(elementSelected(int,QString)), this, SLOT(setClipGenerator(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewGenerators(int,QString,QStringList)), generators, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int,QString,QStringList)), variables, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int,QString,QStringList)), properties, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
    }else{
        FATAL_RUNTIME_ERROR("BSSynchronizedClipGeneratorUI::connectToTables(): One or more arguments are nullptr!!");
    }
}

void BSSynchronizedClipGeneratorUI::variableRenamed(const QString & name, int index){
    int bindIndex = -1;
    hkbVariableBindingSet *bind = nullptr;
    if (name == ""){
        WARNING_MESSAGE("BSSynchronizedClipGeneratorUI::variableRenamed(): The new variable name is the empty string!!");
    }
    if (bsData){
        index--;
        bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (bind){
            bindIndex = bind->getVariableIndexOfBinding("bSyncClipIgnoreMarkPlacement");
            if (bindIndex == index){
                table->item(SYNC_CLIP_IGNORE_MARK_PLACEMENT_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+name);
            }
            bindIndex = bind->getVariableIndexOfBinding("fGetToMarkTime");
            if (bindIndex == index){
                table->item(GET_TO_MARK_TIME_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+name);
            }
            bindIndex = bind->getVariableIndexOfBinding("fMarkErrorThreshold");
            if (bindIndex == index){
                table->item(MARK_ERROR_THRESHOLD_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+name);
            }
            bindIndex = bind->getVariableIndexOfBinding("bLeadCharacter");
            if (bindIndex == index){
                table->item(LEAD_CHARACTER_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+name);
            }
            bindIndex = bind->getVariableIndexOfBinding("bReorientSupportChar");
            if (bindIndex == index){
                table->item(REORIENT_SUPPORT_CHAR_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+name);
            }
            bindIndex = bind->getVariableIndexOfBinding("bApplyMotionFromRoot");
            if (bindIndex == index){
                table->item(APPLY_MOTION_FROM_ROOT_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+name);
            }
            bindIndex = bind->getVariableIndexOfBinding("sAnimationBindingIndex");
            if (bindIndex == index){
                table->item(ANIMATION_BINDING_INDEX_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+name);
            }
        }
    }else{
        FATAL_RUNTIME_ERROR("BSSynchronizedClipGeneratorUI::variableRenamed(): The data is nullptr!!");
    }
}

void BSSynchronizedClipGeneratorUI::generatorRenamed(const QString &name, int index){
    if (name == ""){
        WARNING_MESSAGE("BSSynchronizedClipGeneratorUI::generatorRenamed(): The new variable name is the empty string!!");
    }
    index--;
    if (index == static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData->pClipGenerator)){
        table->item(CLIP_GENERATOR_ROW, VALUE_COLUMN)->setText(name);
    }
}

void BSSynchronizedClipGeneratorUI::setBehaviorView(BehaviorGraphView *view){
    behaviorView = view;
}

