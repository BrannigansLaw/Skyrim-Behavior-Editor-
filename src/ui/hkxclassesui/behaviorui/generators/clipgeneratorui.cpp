#include "clipgeneratorui.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/generators/hkbclipgenerator.h"
#include "src/hkxclasses/behavior/hkbcliptriggerarray.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/ui/hkxclassesui/behaviorui/cliptriggerui.h"
#include "src/ui/genericdatawidgets.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/behaviorgraphview.h"
#include "src/ui/treegraphicsitem.h"
#include "src/ui/mainwindow.h"

#include <QGridLayout>

#include "src/ui/genericdatawidgets.h"
#include <QStackedLayout>
#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 17

#define NAME_ROW 0
#define ANIMATION_NAME_ROW 1
#define CROP_START_AMOUNT_LOCAL_TIME_ROW 2
#define CROP_END_AMOUNT_LOCAL_TIME_ROW 3
#define START_TIME_ROW 4
#define PLAYBACK_SPEED_ROW 5
#define ENFORCED_DURATION_ROW 6
#define USER_CONTROLLED_TIME_FRACTION_ROW 7
#define ANIMATION_BINDING_INDEX_ROW 8
#define MODE_ROW 9
#define FLAG_CONTINUE_MOTION_AT_END_ROW 10
#define FLAG_SYNC_HALF_CYCLE_IN_PING_PONG_MODE_ROW 11
#define FLAG_MIRROR_ROW 12
#define FLAG_FORCE_DENSE_POSE_ROW 13
#define FLAG_DONT_CONVERT_ANNOTATIONS_TO_TRIGGERS_ROW 14
#define FLAG_IGNORE_MOTION_ROW 15
#define ADD_TRIGGER_ROW 16

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList ClipGeneratorUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

ClipGeneratorUI::ClipGeneratorUI()
    : bsData(NULL),
      triggerUI(new ClipTriggerUI),
      groupBox(new QGroupBox),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      cropStartAmountLocalTime(new DoubleSpinBox),
      cropEndAmountLocalTime(new DoubleSpinBox),
      startTime(new DoubleSpinBox),
      playbackSpeed(new DoubleSpinBox),
      enforcedDuration(new DoubleSpinBox),
      userControlledTimeFraction(new DoubleSpinBox),
      animationBindingIndex(new SpinBox),
      mode(new ComboBox),
      flagContinueMotionAtEnd(new CheckBox),
      flagSyncHalfCycleInPingPongMode(new CheckBox),
      flagMirror(new CheckBox),
      flagForceDensePose(new CheckBox),
      flagDontConvertAnnotationsToTriggers(new CheckBox),
      flagIgnoreMotion(new CheckBox)
{
    groupBox->setTitle("hkbClipGenerator");
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(NAME_ROW, NAME_COLUMN, new TableWidgetItem("name"));
    table->setItem(NAME_ROW, TYPE_COLUMN, new TableWidgetItem("hkStringPtr", Qt::AlignCenter));
    table->setItem(NAME_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(NAME_ROW, VALUE_COLUMN, name);
    table->setItem(ANIMATION_NAME_ROW, NAME_COLUMN, new TableWidgetItem("animationName"));
    table->setItem(ANIMATION_NAME_ROW, TYPE_COLUMN, new TableWidgetItem("hkStringPtr", Qt::AlignCenter));
    table->setItem(ANIMATION_NAME_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(ANIMATION_NAME_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QColor(Qt::black), VIEW_ANIMATIONS_TABLE_TIP));
    table->setItem(CROP_START_AMOUNT_LOCAL_TIME_ROW, NAME_COLUMN, new TableWidgetItem("cropStartAmountLocalTime"));
    table->setItem(CROP_START_AMOUNT_LOCAL_TIME_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(CROP_START_AMOUNT_LOCAL_TIME_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(CROP_START_AMOUNT_LOCAL_TIME_ROW, VALUE_COLUMN, cropStartAmountLocalTime);
    table->setItem(CROP_END_AMOUNT_LOCAL_TIME_ROW, NAME_COLUMN, new TableWidgetItem("cropEndAmountLocalTime"));
    table->setItem(CROP_END_AMOUNT_LOCAL_TIME_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(CROP_END_AMOUNT_LOCAL_TIME_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(CROP_END_AMOUNT_LOCAL_TIME_ROW, VALUE_COLUMN, cropEndAmountLocalTime);
    table->setItem(START_TIME_ROW, NAME_COLUMN, new TableWidgetItem("startTime"));
    table->setItem(START_TIME_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(START_TIME_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(START_TIME_ROW, VALUE_COLUMN, startTime);
    table->setItem(PLAYBACK_SPEED_ROW, NAME_COLUMN, new TableWidgetItem("playbackSpeed"));
    table->setItem(PLAYBACK_SPEED_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(PLAYBACK_SPEED_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(PLAYBACK_SPEED_ROW, VALUE_COLUMN, playbackSpeed);
    table->setItem(ENFORCED_DURATION_ROW, NAME_COLUMN, new TableWidgetItem("enforcedDuration"));
    table->setItem(ENFORCED_DURATION_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(ENFORCED_DURATION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ENFORCED_DURATION_ROW, VALUE_COLUMN, enforcedDuration);
    table->setItem(USER_CONTROLLED_TIME_FRACTION_ROW, NAME_COLUMN, new TableWidgetItem("userControlledTimeFraction"));
    table->setItem(USER_CONTROLLED_TIME_FRACTION_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(USER_CONTROLLED_TIME_FRACTION_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(USER_CONTROLLED_TIME_FRACTION_ROW, VALUE_COLUMN, userControlledTimeFraction);
    table->setItem(ANIMATION_BINDING_INDEX_ROW, NAME_COLUMN, new TableWidgetItem("animationBindingIndex"));
    table->setItem(ANIMATION_BINDING_INDEX_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(ANIMATION_BINDING_INDEX_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ANIMATION_BINDING_INDEX_ROW, VALUE_COLUMN, animationBindingIndex);
    table->setItem(MODE_ROW, NAME_COLUMN, new TableWidgetItem("mode"));
    table->setItem(MODE_ROW, TYPE_COLUMN, new TableWidgetItem("Mode", Qt::AlignCenter));
    table->setItem(MODE_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(MODE_ROW, VALUE_COLUMN, mode);
    table->setItem(FLAG_CONTINUE_MOTION_AT_END_ROW, NAME_COLUMN, new TableWidgetItem("flagContinueMotionAtEnd"));
    table->setItem(FLAG_CONTINUE_MOTION_AT_END_ROW, TYPE_COLUMN, new TableWidgetItem("Flags", Qt::AlignCenter));
    table->setItem(FLAG_CONTINUE_MOTION_AT_END_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(FLAG_CONTINUE_MOTION_AT_END_ROW, VALUE_COLUMN, new TableWidgetItem("", Qt::AlignCenter, QColor(Qt::lightGray)));
    table->setCellWidget(FLAG_CONTINUE_MOTION_AT_END_ROW, VALUE_COLUMN, flagContinueMotionAtEnd);
    table->setItem(FLAG_SYNC_HALF_CYCLE_IN_PING_PONG_MODE_ROW, NAME_COLUMN, new TableWidgetItem("flagSyncHalfCycleInPingPongMode"));
    table->setItem(FLAG_SYNC_HALF_CYCLE_IN_PING_PONG_MODE_ROW, TYPE_COLUMN, new TableWidgetItem("Flags", Qt::AlignCenter));
    table->setItem(FLAG_SYNC_HALF_CYCLE_IN_PING_PONG_MODE_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(FLAG_SYNC_HALF_CYCLE_IN_PING_PONG_MODE_ROW, VALUE_COLUMN, new TableWidgetItem("", Qt::AlignCenter, QColor(Qt::lightGray)));
    table->setCellWidget(FLAG_SYNC_HALF_CYCLE_IN_PING_PONG_MODE_ROW, VALUE_COLUMN, flagSyncHalfCycleInPingPongMode);
    table->setItem(FLAG_MIRROR_ROW, NAME_COLUMN, new TableWidgetItem("flagMirror"));
    table->setItem(FLAG_MIRROR_ROW, TYPE_COLUMN, new TableWidgetItem("Flags", Qt::AlignCenter));
    table->setItem(FLAG_MIRROR_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(FLAG_MIRROR_ROW, VALUE_COLUMN, new TableWidgetItem("", Qt::AlignCenter, QColor(Qt::lightGray)));
    table->setCellWidget(FLAG_MIRROR_ROW, VALUE_COLUMN, flagMirror);
    table->setItem(FLAG_FORCE_DENSE_POSE_ROW, NAME_COLUMN, new TableWidgetItem("flagForceDensePose"));
    table->setItem(FLAG_FORCE_DENSE_POSE_ROW, TYPE_COLUMN, new TableWidgetItem("Flags", Qt::AlignCenter));
    table->setItem(FLAG_FORCE_DENSE_POSE_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(FLAG_FORCE_DENSE_POSE_ROW, VALUE_COLUMN, new TableWidgetItem("", Qt::AlignCenter, QColor(Qt::lightGray)));
    table->setCellWidget(FLAG_FORCE_DENSE_POSE_ROW, VALUE_COLUMN, flagForceDensePose);
    table->setItem(FLAG_DONT_CONVERT_ANNOTATIONS_TO_TRIGGERS_ROW, NAME_COLUMN, new TableWidgetItem("flagDontConvertAnnotationsToTriggers"));
    table->setItem(FLAG_DONT_CONVERT_ANNOTATIONS_TO_TRIGGERS_ROW, TYPE_COLUMN, new TableWidgetItem("Flags", Qt::AlignCenter));
    table->setItem(FLAG_DONT_CONVERT_ANNOTATIONS_TO_TRIGGERS_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(FLAG_DONT_CONVERT_ANNOTATIONS_TO_TRIGGERS_ROW, VALUE_COLUMN, new TableWidgetItem("", Qt::AlignCenter, QColor(Qt::lightGray)));
    table->setCellWidget(FLAG_DONT_CONVERT_ANNOTATIONS_TO_TRIGGERS_ROW, VALUE_COLUMN, flagDontConvertAnnotationsToTriggers);
    table->setItem(FLAG_IGNORE_MOTION_ROW, NAME_COLUMN, new TableWidgetItem("flagIgnoreMotion"));
    table->setItem(FLAG_IGNORE_MOTION_ROW, TYPE_COLUMN, new TableWidgetItem("Flags", Qt::AlignCenter));
    table->setItem(FLAG_IGNORE_MOTION_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(FLAG_IGNORE_MOTION_ROW, VALUE_COLUMN, new TableWidgetItem("", Qt::AlignCenter, QColor(Qt::lightGray)));
    table->setCellWidget(FLAG_IGNORE_MOTION_ROW, VALUE_COLUMN, flagIgnoreMotion);
    table->setItem(ADD_TRIGGER_ROW, NAME_COLUMN, new TableWidgetItem("Add Trigger", Qt::AlignCenter, QColor(Qt::green), QBrush(Qt::black), "Double click to add a new clip trigger"));
    table->setItem(ADD_TRIGGER_ROW, TYPE_COLUMN, new TableWidgetItem("hkTrigger", Qt::AlignCenter));
    table->setItem(ADD_TRIGGER_ROW, BINDING_COLUMN, new TableWidgetItem("Remove Selected Trigger", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black), "Double click to remove the selected trigger"));
    table->setItem(ADD_TRIGGER_ROW, VALUE_COLUMN, new TableWidgetItem("Edit Selected Trigger", Qt::AlignCenter, QColor(Qt::gray), QBrush(Qt::black), "Double click to edit the selected trigger"));
    topLyt->addWidget(table, 1, 0, 8, 3);
    groupBox->setLayout(topLyt);
    //Order here must correspond with the ACTIVE_WIDGET Enumerated type!!!
    addWidget(groupBox);
    addWidget(triggerUI);
    connectSignals();
}

void ClipGeneratorUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(cropStartAmountLocalTime, SIGNAL(editingFinished()), this, SLOT(setCropStartAmountLocalTime()), Qt::UniqueConnection);
    connect(cropEndAmountLocalTime, SIGNAL(editingFinished()), this, SLOT(setCropEndAmountLocalTime()), Qt::UniqueConnection);
    connect(startTime, SIGNAL(editingFinished()), this, SLOT(setStartTime()), Qt::UniqueConnection);
    connect(playbackSpeed, SIGNAL(editingFinished()), this, SLOT(setPlaybackSpeed()), Qt::UniqueConnection);
    connect(enforcedDuration, SIGNAL(editingFinished()), this, SLOT(setEnforcedDuration()), Qt::UniqueConnection);
    connect(userControlledTimeFraction, SIGNAL(editingFinished()), this, SLOT(setUserControlledTimeFraction()), Qt::UniqueConnection);
    connect(animationBindingIndex, SIGNAL(editingFinished()), this, SLOT(setAnimationBindingIndex()), Qt::UniqueConnection);
    connect(mode, SIGNAL(currentIndexChanged(int)), this, SLOT(setMode(int)), Qt::UniqueConnection);
    connect(flagContinueMotionAtEnd, SIGNAL(released()), this, SLOT(setFlagContinueMotionAtEnd()), Qt::UniqueConnection);
    connect(flagSyncHalfCycleInPingPongMode, SIGNAL(released()), this, SLOT(setFlagSyncHalfCycleInPingPongMode()), Qt::UniqueConnection);
    connect(flagMirror, SIGNAL(released()), this, SLOT(setFlagMirror()), Qt::UniqueConnection);
    connect(flagForceDensePose, SIGNAL(released()), this, SLOT(setFlagForceDensePose()), Qt::UniqueConnection);
    connect(flagDontConvertAnnotationsToTriggers, SIGNAL(released()), this, SLOT(setFlagDontConvertAnnotationsToTriggers()), Qt::UniqueConnection);
    connect(flagIgnoreMotion, SIGNAL(released()), this, SLOT(setFlagIgnoreMotion()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)), Qt::UniqueConnection);
    connect(triggerUI, SIGNAL(viewEvents(int)), this, SIGNAL(viewEvents(int)), Qt::UniqueConnection);
    connect(triggerUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()), Qt::UniqueConnection);
}

void ClipGeneratorUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(cropStartAmountLocalTime, SIGNAL(editingFinished()), this, SLOT(setCropStartAmountLocalTime()));
    disconnect(cropEndAmountLocalTime, SIGNAL(editingFinished()), this, SLOT(setCropEndAmountLocalTime()));
    disconnect(startTime, SIGNAL(editingFinished()), this, SLOT(setStartTime()));
    disconnect(playbackSpeed, SIGNAL(editingFinished()), this, SLOT(setPlaybackSpeed()));
    disconnect(enforcedDuration, SIGNAL(editingFinished()), this, SLOT(setEnforcedDuration()));
    disconnect(userControlledTimeFraction, SIGNAL(editingFinished()), this, SLOT(setUserControlledTimeFraction()));
    disconnect(animationBindingIndex, SIGNAL(editingFinished()), this, SLOT(setAnimationBindingIndex()));
    disconnect(mode, SIGNAL(currentIndexChanged(int)), this, SLOT(setMode(int)));
    disconnect(flagContinueMotionAtEnd, SIGNAL(released()), this, SLOT(setFlagContinueMotionAtEnd()));
    disconnect(flagSyncHalfCycleInPingPongMode, SIGNAL(released()), this, SLOT(setFlagSyncHalfCycleInPingPongMode()));
    disconnect(flagMirror, SIGNAL(released()), this, SLOT(setFlagMirror()));
    disconnect(flagForceDensePose, SIGNAL(released()), this, SLOT(setFlagForceDensePose()));
    disconnect(flagDontConvertAnnotationsToTriggers, SIGNAL(released()), this, SLOT(setFlagDontConvertAnnotationsToTriggers()));
    disconnect(flagIgnoreMotion, SIGNAL(released()), this, SLOT(setFlagIgnoreMotion()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)));
    disconnect(triggerUI, SIGNAL(viewEvents(int)), this, SIGNAL(viewEvents(int)));
    disconnect(triggerUI, SIGNAL(returnToParent()), this, SLOT(returnToWidget()));
}

void ClipGeneratorUI::addTrigger(){
    hkbClipTriggerArray *triggers = NULL;
    if (bsData){
        triggers = static_cast<hkbClipTriggerArray *>(bsData->triggers.data());
        if (!triggers){
            triggers = new hkbClipTriggerArray(bsData->getParentFile(), -1);
            bsData->triggers = HkxSharedPtr(triggers);
        }
        triggers->addTrigger();
        triggers->triggers.last().event.id = 0;
        static_cast<BehaviorFile *>(bsData->getParentFile())->appendClipTriggerToAnimData(bsData->name);
        bsData->getParentFile()->toggleChanged(true);
        loadDynamicTableRows();
    }else{
        (qFatal("ClipGeneratorUI::addTrigger(): The data is NULL!!"));
    }
}

void ClipGeneratorUI::removeTrigger(int index){
    hkbClipTriggerArray *triggers = NULL;
    if (bsData){
        triggers = static_cast<hkbClipTriggerArray *>(bsData->triggers.data());
        if (triggers){
            if (index < triggers->triggers.size() && index >= 0){
                triggers->removeTrigger(index);
                static_cast<BehaviorFile *>(bsData->getParentFile())->removeClipTriggerToAnimDataAt(bsData->name, index);
            }else{
                (qWarning("ClipGeneratorUI::removeTrigger(): Invalid row index selected!!"));
                return;
            }
            bsData->getParentFile()->toggleChanged(true);
            loadDynamicTableRows();
        }else{
            (qWarning("ClipGeneratorUI::removeTrigger(): Event data is NULL!!"));
            return;
        }
    }else{
        (qFatal("ClipGeneratorUI::removeTrigger(): The data is NULL!!"));
    }
}

void ClipGeneratorUI::loadData(HkxObject *data){
    disconnectSignals();
    setCurrentIndex(MAIN_WIDGET);
    hkbVariableBindingSet *varBind = NULL;
    if (data){
        if (data->getSignature() == HKB_CLIP_GENERATOR){
            bsData = static_cast<hkbClipGenerator *>(data);
            name->setText(bsData->name);
            table->item(ANIMATION_NAME_ROW, VALUE_COLUMN)->setText(bsData->animationName);
            cropStartAmountLocalTime->setValue(bsData->cropStartAmountLocalTime);
            cropEndAmountLocalTime->setValue(bsData->cropEndAmountLocalTime);
            startTime->setValue(bsData->startTime);
            playbackSpeed->setValue(bsData->playbackSpeed);
            enforcedDuration->setValue(bsData->enforcedDuration);
            userControlledTimeFraction->setValue(bsData->userControlledTimeFraction);
            animationBindingIndex->setValue(bsData->animationBindingIndex);
            enforcedDuration->setValue(bsData->enforcedDuration);
            bool ok = true;
            hkbClipGenerator::ClipFlags flags(bsData->flags.toInt(&ok));
            if (ok){
                if (flags.testFlag(hkbClipGenerator::FLAG_CONTINUE_MOTION_AT_END)){
                    flagContinueMotionAtEnd->setChecked(true);
                }else{
                    flagContinueMotionAtEnd->setChecked(false);
                }
                if (flags.testFlag(hkbClipGenerator::FLAG_SYNC_HALF_CYCLE_IN_PING_PONG_MODE)){
                    flagSyncHalfCycleInPingPongMode->setChecked(true);
                }else{
                    flagSyncHalfCycleInPingPongMode->setChecked(false);
                }
                if (flags.testFlag(hkbClipGenerator::FLAG_MIRROR)){
                    flagMirror->setChecked(true);
                }else{
                    flagMirror->setChecked(false);
                }
                if (flags.testFlag(hkbClipGenerator::FLAG_FORCE_DENSE_POSE)){
                    flagForceDensePose->setChecked(true);
                }else{
                    flagForceDensePose->setChecked(false);
                }
                if (flags.testFlag(hkbClipGenerator::FLAG_DONT_CONVERT_ANNOTATIONS_TO_TRIGGERS)){
                    flagDontConvertAnnotationsToTriggers->setChecked(true);
                }else{
                    flagDontConvertAnnotationsToTriggers->setChecked(false);
                }
                if (flags.testFlag(hkbClipGenerator::FLAG_IGNORE_MOTION)){
                    flagIgnoreMotion->setChecked(true);
                }else{
                    flagIgnoreMotion->setChecked(false);
                }
                if (mode->count() == 0){
                    mode->insertItems(0, bsData->PlaybackMode);
                }
                mode->setCurrentIndex(bsData->PlaybackMode.indexOf(bsData->mode));
                varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
                if (varBind){
                    loadBinding(CROP_START_AMOUNT_LOCAL_TIME_ROW, BINDING_COLUMN, varBind, "cropStartAmountLocalTime");
                    loadBinding(CROP_END_AMOUNT_LOCAL_TIME_ROW, BINDING_COLUMN, varBind, "cropEndAmountLocalTime");
                    loadBinding(START_TIME_ROW, BINDING_COLUMN, varBind, "startTime");
                    loadBinding(PLAYBACK_SPEED_ROW, BINDING_COLUMN, varBind, "playbackSpeed");
                    loadBinding(ENFORCED_DURATION_ROW, BINDING_COLUMN, varBind, "enforcedDuration");
                    loadBinding(USER_CONTROLLED_TIME_FRACTION_ROW, BINDING_COLUMN, varBind, "userControlledTimeFraction");
                    loadBinding(ANIMATION_BINDING_INDEX_ROW, BINDING_COLUMN, varBind, "animationBindingIndex");
                }else{
                    table->item(CROP_START_AMOUNT_LOCAL_TIME_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                    table->item(CROP_END_AMOUNT_LOCAL_TIME_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                    table->item(START_TIME_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                    table->item(PLAYBACK_SPEED_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                    table->item(ENFORCED_DURATION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                    table->item(USER_CONTROLLED_TIME_FRACTION_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                    table->item(ANIMATION_BINDING_INDEX_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                }
                loadDynamicTableRows();
            }else{
                (qFatal(QString("ClipGeneratorUI::loadData(): The flags string is invalid!!!\nString: "+bsData->flags).toLocal8Bit().data()));
            }
        }else{
            (qFatal(QString("ClipGeneratorUI::loadData(): The data passed to the UI is the wrong type!\nSIGNATURE: "+QString::number(data->getSignature(), 16)).toLocal8Bit().data()));
        }
    }else{
        (qFatal("ClipGeneratorUI::loadData(): Attempting to load a null pointer!!"));
    }
    connectSignals();
}

void ClipGeneratorUI::loadDynamicTableRows(){
    //table->setSortingEnabled(false);//Not sure...
    if (bsData){
        int temp = ADD_TRIGGER_ROW + bsData->getNumberOfTriggers() + 1;
        if (table->rowCount() != temp){
            table->setRowCount(temp);
        }
        hkbClipTriggerArray *triggers = static_cast<hkbClipTriggerArray *>(bsData->triggers.data());
        if (triggers){
            for (int i = ADD_TRIGGER_ROW + 1, j = 0; j < bsData->getNumberOfTriggers(); i++, j++){
                setRowItems(i, static_cast<BehaviorFile *>(bsData->getParentFile())->getEventNameAt(triggers->triggers.at(j).event.id), triggers->getClassname(), "Remove", "Edit", "Double click to remove this trigger", "Double click to edit this trigger");
            }
        }
    }else{
        (qFatal("ClipGeneratorUI::loadDynamicTableRows(): The data is NULL!!"));
    }
    //table->setSortingEnabled(true);
}

void ClipGeneratorUI::setRowItems(int row, const QString & name, const QString & classname, const QString & bind, const QString & value, const QString & tip1, const QString & tip2){
    if (table->item(row, NAME_COLUMN)){
        table->item(row, NAME_COLUMN)->setText(name);
    }else{
        table->setItem(row, NAME_COLUMN, new TableWidgetItem(name));
    }
    if (table->item(row, TYPE_COLUMN)){
        table->item(row, TYPE_COLUMN)->setText(classname);
    }else{
        table->setItem(row, TYPE_COLUMN, new TableWidgetItem(classname, Qt::AlignCenter));
    }
    if (table->item(row, BINDING_COLUMN)){
        table->item(row, BINDING_COLUMN)->setText(bind);
    }else{
        table->setItem(row, BINDING_COLUMN, new TableWidgetItem(bind, Qt::AlignCenter, QColor(Qt::red), QBrush(Qt::black), tip1));
    }
    if (table->item(row, VALUE_COLUMN)){
        table->item(row, VALUE_COLUMN)->setText(value);
    }else{
        table->setItem(row, VALUE_COLUMN, new TableWidgetItem(value, Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), tip2));
    }
}

bool ClipGeneratorUI::setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty){
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
                    (qFatal("ClipGeneratorUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }else{
                if (!varBind->addBinding(path, variableName, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    (qFatal("ClipGeneratorUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!"));
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->toggleChanged(true);
        }else{
            (qWarning("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!"));
        }
    }else{
        (qFatal("ClipGeneratorUI::setBinding(): The data is NULL!!"));
    }
    return true;
}

void ClipGeneratorUI::setBindingVariable(int index, const QString & name){
    if (bsData){
        bool isProperty = false;
        int row = table->currentRow();
        switch (row){
        case CROP_START_AMOUNT_LOCAL_TIME_ROW:
            if (table->item(CROP_START_AMOUNT_LOCAL_TIME_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "cropStartAmountLocalTime", VARIABLE_TYPE_REAL, isProperty);
            break;
        case CROP_END_AMOUNT_LOCAL_TIME_ROW:
            if (table->item(CROP_END_AMOUNT_LOCAL_TIME_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "cropEndAmountLocalTime", VARIABLE_TYPE_REAL, isProperty);
            break;
        case START_TIME_ROW:
            if (table->item(START_TIME_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "startTime", VARIABLE_TYPE_REAL, isProperty);
            break;
        case PLAYBACK_SPEED_ROW:
            if (table->item(PLAYBACK_SPEED_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "playbackSpeed", VARIABLE_TYPE_REAL, isProperty);
            break;
        case ENFORCED_DURATION_ROW:
            if (table->item(ENFORCED_DURATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "enforcedDuration", VARIABLE_TYPE_REAL, isProperty);
            break;
        case USER_CONTROLLED_TIME_FRACTION_ROW:
            if (table->item(USER_CONTROLLED_TIME_FRACTION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "userControlledTimeFraction", VARIABLE_TYPE_REAL, isProperty);
            break;
        case ANIMATION_BINDING_INDEX_ROW:
            if (table->item(ANIMATION_BINDING_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "animationBindingIndex", VARIABLE_TYPE_INT32, isProperty);
            break;
        default:
            return;
        }
        bsData->getParentFile()->toggleChanged(true);
    }else{
        (qFatal("ClipGeneratorUI::setBindingVariable(): The data is NULL!!"));
    }
}

void ClipGeneratorUI::returnToWidget(){
    loadDynamicTableRows();
    setCurrentIndex(MAIN_WIDGET);
}

void ClipGeneratorUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            if (static_cast<BehaviorFile *>(bsData->getParentFile())->isClipGenNameAvailable(name->text())){
                bsData->setName(bsData->name, name->text());
                static_cast<DataIconManager*>((bsData))->updateIconNames();
                emit generatorNameChanged(bsData->name, static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfGenerator(bsData));
                bsData->getParentFile()->toggleChanged(true);
            }else{
                disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
                name->setText(bsData->name);
                connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
                (qWarning("ClipGeneratorUI::setName(): This clip generator name is already is use elsewhere in the project!!"));
            }
        }
    }else{
        (qFatal("ClipGeneratorUI::setName(): The data is NULL!!"));
    }
}

void ClipGeneratorUI::setAnimationName(int index, const QString &name){
    if (bsData){
        if (bsData->animationName != name){
            bsData->setAnimationName(index, name);
            table->item(ANIMATION_NAME_ROW, VALUE_COLUMN)->setText(name);
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        (qFatal("ClipGeneratorUI::setAnimationName(): The data is NULL!!"));
    }
}

void ClipGeneratorUI::setCropStartAmountLocalTime(){
    if (bsData){
        if (bsData->cropStartAmountLocalTime != cropStartAmountLocalTime->value()){
            bsData->setCropStartAmountLocalTime(cropStartAmountLocalTime->value());
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        (qFatal("ClipGeneratorUI::setCropStartAmountLocalTime(): The data is NULL!!"));
    }
}

void ClipGeneratorUI::setCropEndAmountLocalTime(){
    if (bsData){
        if (bsData->cropEndAmountLocalTime != cropEndAmountLocalTime->value()){
            bsData->setCropEndAmountLocalTime(cropEndAmountLocalTime->value());
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        (qFatal("ClipGeneratorUI::setCropEndAmountLocalTime(): The data is NULL!!"));
    }
}

void ClipGeneratorUI::setStartTime(){
    if (bsData){
        if (bsData->startTime != startTime->value()){
            bsData->startTime = startTime->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        (qFatal("ClipGeneratorUI::setStartTime(): The data is NULL!!"));
    }
}

void ClipGeneratorUI::setPlaybackSpeed(){
    if (bsData){
        if (bsData->playbackSpeed != playbackSpeed->value()){
            bsData->setPlaybackSpeed(playbackSpeed->value());
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        (qFatal("ClipGeneratorUI::setPlaybackSpeed(): The data is NULL!!"));
    }
}

void ClipGeneratorUI::setEnforcedDuration(){
    if (bsData){
        if (bsData->enforcedDuration != enforcedDuration->value()){
            bsData->enforcedDuration = enforcedDuration->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        (qFatal("ClipGeneratorUI::setEnforcedDuration(): The data is NULL!!"));
    }
}

void ClipGeneratorUI::setUserControlledTimeFraction(){
    if (bsData){
        if (bsData->userControlledTimeFraction != userControlledTimeFraction->value()){
            bsData->userControlledTimeFraction = userControlledTimeFraction->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        (qFatal("ClipGeneratorUI::setUserControlledTimeFraction(): The data is NULL!!"));
    }
}

void ClipGeneratorUI::setAnimationBindingIndex(){
    if (bsData){
        if (bsData->animationBindingIndex != animationBindingIndex->value()){
            bsData->animationBindingIndex = animationBindingIndex->value();
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        (qFatal("ClipGeneratorUI::setAnimationBindingIndex(): The data is NULL!!"));
    }
}

void ClipGeneratorUI::setMode(int index){
    if (bsData){
        if (bsData->mode != bsData->PlaybackMode.at(index)){
            bsData->mode = bsData->PlaybackMode.at(index);
            bsData->getParentFile()->toggleChanged(true);
        }
    }else{
        (qFatal("ClipGeneratorUI::setMode(): The data is NULL!!"));
    }
}

void ClipGeneratorUI::setFlagContinueMotionAtEnd(){
    if (bsData){
        bool ok = true;
        hkbClipGenerator::ClipFlags flags(bsData->flags.toInt(&ok));
        if (ok){
            if (flagContinueMotionAtEnd->isChecked()){
                flags |= hkbClipGenerator::FLAG_CONTINUE_MOTION_AT_END;
            }else{
                flags &= ~(hkbClipGenerator::FLAG_CONTINUE_MOTION_AT_END);
            }
            bsData->flags = QString::number(flags);
            bsData->getParentFile()->toggleChanged(true);
        }else{
            (qFatal(QString("ClipGeneratorUI::setFlagContinueMotionAtEnd(): The flags string is invalid!!!\nString: "+bsData->flags).toLocal8Bit().data()));
        }
    }else{
        (qFatal("ClipGeneratorUI::setFlagContinueMotionAtEnd(): The data is NULL!!"));
    }
}

void ClipGeneratorUI::setFlagSyncHalfCycleInPingPongMode(){
    if (bsData){
        bool ok = true;
        hkbClipGenerator::ClipFlags flags(bsData->flags.toInt(&ok));
        if (ok){
            if (flagSyncHalfCycleInPingPongMode->isChecked()){
                flags |= hkbClipGenerator::FLAG_SYNC_HALF_CYCLE_IN_PING_PONG_MODE;
            }else{
                flags &= ~(hkbClipGenerator::FLAG_SYNC_HALF_CYCLE_IN_PING_PONG_MODE);
            }
            bsData->flags = QString::number(flags);
            bsData->getParentFile()->toggleChanged(true);
        }else{
            (qFatal(QString("ClipGeneratorUI::setFlagSyncHalfCycleInPingPongMode(): The flags string is invalid!!!\nString: "+bsData->flags).toLocal8Bit().data()));
        }
    }else{
        (qFatal("ClipGeneratorUI::setFlagSyncHalfCycleInPingPongMode(): The data is NULL!!"));
    }
}

void ClipGeneratorUI::setFlagMirror(){
    if (bsData){
        bool ok = true;
        hkbClipGenerator::ClipFlags flags(bsData->flags.toInt(&ok));
        if (ok){
            if (flagMirror->isChecked()){
                flags |= hkbClipGenerator::FLAG_MIRROR;
            }else{
                flags &= ~(hkbClipGenerator::FLAG_MIRROR);
            }
            bsData->flags = QString::number(flags);
            bsData->getParentFile()->toggleChanged(true);
        }else{
            (qFatal(QString("ClipGeneratorUI::setFlagMirror(): The flags string is invalid!!!\nString: "+bsData->flags).toLocal8Bit().data()));
        }
    }else{
        (qFatal("ClipGeneratorUI::setFlagMirror(): The data is NULL!!"));
    }
}

void ClipGeneratorUI::setFlagForceDensePose(){
    if (bsData){
        bool ok = true;
        hkbClipGenerator::ClipFlags flags(bsData->flags.toInt(&ok));
        if (ok){
            if (flagForceDensePose->isChecked()){
                flags |= hkbClipGenerator::FLAG_FORCE_DENSE_POSE;
            }else{
                flags &= ~(hkbClipGenerator::FLAG_FORCE_DENSE_POSE);
            }
            bsData->flags = QString::number(flags);
            bsData->getParentFile()->toggleChanged(true);
        }else{
            (qFatal(QString("ClipGeneratorUI::setFlagForceDensePose(): The flags string is invalid!!!\nString: "+bsData->flags).toLocal8Bit().data()));
        }
    }else{
        (qFatal("ClipGeneratorUI::setFlagForceDensePose(): The data is NULL!!"));
    }
}

void ClipGeneratorUI::setFlagDontConvertAnnotationsToTriggers(){
    if (bsData){
        bool ok = true;
        hkbClipGenerator::ClipFlags flags(bsData->flags.toInt(&ok));
        if (ok){
            if (flagDontConvertAnnotationsToTriggers->isChecked()){
                flags |= hkbClipGenerator::FLAG_DONT_CONVERT_ANNOTATIONS_TO_TRIGGERS;
            }else{
                flags &= ~(hkbClipGenerator::FLAG_DONT_CONVERT_ANNOTATIONS_TO_TRIGGERS);
            }
            bsData->flags = QString::number(flags);
            bsData->getParentFile()->toggleChanged(true);
        }else{
            (qFatal(QString("ClipGeneratorUI::setFlagDontConvertAnnotationsToTriggers(): The flags string is invalid!!!\nString: "+bsData->flags).toLocal8Bit().data()));
        }
    }else{
        (qFatal("ClipGeneratorUI::setFlagDontConvertAnnotationsToTriggers(): The data is NULL!!"));
    }
}

void ClipGeneratorUI::setFlagIgnoreMotion(){
    if (bsData){
        bool ok = true;
        hkbClipGenerator::ClipFlags flags(bsData->flags.toInt(&ok));
        if (ok){
            if (flagIgnoreMotion->isChecked()){
                flags |= hkbClipGenerator::FLAG_IGNORE_MOTION;
            }else{
                flags &= ~(hkbClipGenerator::FLAG_IGNORE_MOTION);
            }
            bsData->flags = QString::number(flags);
            bsData->getParentFile()->toggleChanged(true);
        }else{
            (qFatal(QString("ClipGeneratorUI::setFlagIgnoreMotion(): The flags string is invalid!!!\nString: "+bsData->flags).toLocal8Bit().data()));
        }
    }else{
        (qFatal("ClipGeneratorUI::setFlagIgnoreMotion(): The data is NULL!!"));
    }
}

void ClipGeneratorUI::viewSelectedChild(int row, int column){
    int result = -1;
    bool properties = false;
    if (bsData){
        if (row < ADD_TRIGGER_ROW && row >= 0){
            if (column == BINDING_COLUMN){
                switch (row){
                case CROP_START_AMOUNT_LOCAL_TIME_ROW:
                    if (table->item(CROP_START_AMOUNT_LOCAL_TIME_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "cropStartAmountLocalTime");
                    break;
                case CROP_END_AMOUNT_LOCAL_TIME_ROW:
                    if (table->item(CROP_END_AMOUNT_LOCAL_TIME_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "cropEndAmountLocalTime");
                    break;
                case START_TIME_ROW:
                    if (table->item(START_TIME_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "startTime");
                case PLAYBACK_SPEED_ROW:
                    if (table->item(PLAYBACK_SPEED_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "playbackSpeed");
                    break;
                case ENFORCED_DURATION_ROW:
                    if (table->item(ENFORCED_DURATION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "enforcedDuration");
                    break;
                case USER_CONTROLLED_TIME_FRACTION_ROW:
                    if (table->item(USER_CONTROLLED_TIME_FRACTION_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "userControlledTimeFraction");
                case ANIMATION_BINDING_INDEX_ROW:
                    if (table->item(ANIMATION_BINDING_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                        properties = true;
                    }
                    selectTableToView(properties, "animationBindingIndex");
                }
            }else if (row == ANIMATION_NAME_ROW && column == VALUE_COLUMN){
                emit viewAnimations(bsData->animationName);
            }
        }else if (row == ADD_TRIGGER_ROW && column == NAME_COLUMN){
            addTrigger();
        }else if (row > ADD_TRIGGER_ROW && row < ADD_TRIGGER_ROW + bsData->getNumberOfTriggers() + 1){
            result = row - BASE_NUMBER_OF_ROWS;
            if (bsData->getNumberOfTriggers() > result && result >= 0){
                if (column == VALUE_COLUMN){
                    triggerUI->loadData((BehaviorFile *)(bsData->triggers.data()->getParentFile()), bsData, result, &static_cast<hkbClipTriggerArray *>(bsData->triggers.data())->triggers[result]);
                    setCurrentIndex(CHILD_WIDGET);
                }else if (column == BINDING_COLUMN){
                    if (MainWindow::yesNoDialogue("Are you sure you want to remove the trigger \""+table->item(row, NAME_COLUMN)->text()+"\"?") == QMessageBox::Yes){
                        removeTrigger(result);
                    }
                }
            }else{
                (qFatal("ClipGeneratorUI::viewSelectedChild(): Invalid index of child to view!!"));
            }
        }
    }else{
        (qFatal("ClipGeneratorUI::viewSelectedChild(): The data is NULL!!"));
    }
}

void ClipGeneratorUI::connectToTables(GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *events, GenericTableWidget *animations){
    if (variables && events && properties && animations){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(events, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(animations, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(events, SIGNAL(elementSelected(int,QString)), triggerUI, SLOT(setEventId(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(animations, SIGNAL(elementSelected(int,QString)), this, SLOT(setAnimationName(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewEvents(int)), events, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int)), variables, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int)), properties, SLOT(showTable(int)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewAnimations(QString)), animations, SLOT(showTable(QString)), Qt::UniqueConnection);
    }else{
        (qFatal("ClipGeneratorUI::connectToTables(): One or more arguments are NULL!!"));
    }
}

void ClipGeneratorUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
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
            (qFatal("ClipGeneratorUI::loadBinding(): The variable binding set is NULL!!"));
        }
    }else{
        (qFatal("ClipGeneratorUI::loadBinding(): The data is NULL!!"));
    }
}

void ClipGeneratorUI::selectTableToView(bool viewproperties, const QString & path){
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
        (qFatal("ClipGeneratorUI::selectTableToView(): The data is NULL!!"));
    }
}

void ClipGeneratorUI::eventRenamed(const QString & name, int index){
    if (bsData){
        triggerUI->eventRenamed(name, index);
    }else{
        (qFatal("ClipGeneratorUI::eventRenamed(): The data is NULL!!"));
    }
}

void ClipGeneratorUI::animationRenamed(const QString &name, int index){
    if (bsData){
        //if ()
    }else{
        (qFatal("ClipGeneratorUI::animationRenamed(): The data is NULL!!"));
    }
}

void ClipGeneratorUI::variableRenamed(const QString & name, int index){
    int bindIndex = -1;
    hkbVariableBindingSet *bind = NULL;
    if (name == ""){
        (qWarning("ClipGeneratorUI::variableRenamed(): The new variable name is the empty string!!"));
    }
    if (bsData){
        index--;
        bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (bind){
            bindIndex = bind->getVariableIndexOfBinding("cropStartAmountLocalTime");
            if (bindIndex == index){
                table->item(CROP_START_AMOUNT_LOCAL_TIME_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("cropEndAmountLocalTime");
            if (bindIndex == index){
                table->item(CROP_END_AMOUNT_LOCAL_TIME_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("startTime");
            if (bindIndex == index){
                table->item(START_TIME_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("playbackSpeed");
            if (bindIndex == index){
                table->item(PLAYBACK_SPEED_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("enforcedDuration");
            if (bindIndex == index){
                table->item(ENFORCED_DURATION_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("userControlledTimeFraction");
            if (bindIndex == index){
                table->item(USER_CONTROLLED_TIME_FRACTION_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("animationBindingIndex");
            if (bindIndex == index){
                table->item(ANIMATION_BINDING_INDEX_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        (qFatal("ClipGeneratorUI::variableRenamed(): The data is NULL!!"));
    }
}

