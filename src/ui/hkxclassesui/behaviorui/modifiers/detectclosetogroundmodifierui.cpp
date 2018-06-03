#include "detectclosetogroundmodifierui.h"

#include "src/hkxclasses/hkxobject.h"
#include "src/hkxclasses/behavior/hkbstringeventpayload.h"
#include "src/hkxclasses/behavior/modifiers/hkbDetectCloseToGroundModifier.h"
#include "src/hkxclasses/behavior/hkbvariablebindingset.h"
#include "src/filetypes/behaviorfile.h"
#include "src/ui/genericdatawidgets.h"

#include <QGridLayout>
#include <QHeaderView>

#define BASE_NUMBER_OF_ROWS 9

#define NAME_ROW 0
#define ENABLE_ROW 1
#define CLOSE_TO_GROUND_HEIGHT_ROW 2
#define RAYCAST_DISTANCE_DOWN_ROW 3
#define COLLISION_FILTER_INFO_ROW 4
#define BONE_INDEX_ROW 5
#define ANIM_BONE_INDEX_ROW 6
#define CLOSE_TO_GROUND_EVENT_ID_ROW 7
#define CLOSE_TO_GROUND_EVENT_PAYLOAD_ROW 8

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define BINDING_COLUMN 2
#define VALUE_COLUMN 3

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList DetectCloseToGroundModifierUI::collisionLayers = {
    "NONE",
    "L_UNIDENTIFIED",
    "L_STATIC",
    "L_ANIMSTATIC",
    "L_TRANSPARENT",
    "L_CLUTTER",
    "L_WEAPON",
    "L_PROJECTILE",
    "L_SPELL",
    "L_BIPED",
    "L_TREES",
    "L_PROPS",
    "L_WATER",
    "L_TRIGGER",
    "L_TERRAIN",
    "L_TRAP",
    "L_NONCOLLIDABLE",
    "L_CLOUDTRAP",
    "L_GROUND",
    "L_PORTAL",
    "L_DEBRIS_SMALL",
    "L_DEBRIS_LARGE",
    "L_ACOUSTIC_SPACE",
    "L_ACTOR_ZONE",
    "L_PROJECTILEZONE",
    "L_GASTRAP",
    "L_SHELLCASING",
    "L_TRANSPARENT_SMALL",
    "L_INVISABLE_WALL",
    "L_TRANSPARENT_SMALL_ANIM",
    "L_WARD",
    "L_CHARCONTROLLER",
    "L_STAIRHELPER",
    "L_DEADBIP",
    "L_BIPED_NO_CC",
    "L_AVOIDBOX",
    "L_COLLISIONBOX",
    "L_CAMERASPHERE",
    "L_DOORDETECTION",
    "L_CONEPROJECTILE",
    "L_CAMERA",
    "L_ITEMPICKER",
    "L_LOS",
    "L_PATHINGPICK",
    "L_CUSTOMPICK1",
    "L_CUSTOMPICK2",
    "L_SPELLEXPLOSION",
    "L_DROPPINGPICK",
    "L_DEADACTORZONE",
    "L_CRITTER",
    "L_TRIGGERFALLINGTRAP",
    "L_NAVCUT",
    "L_SPELLTRIGGER",
    "L_LIVING_AND_DEAD_ACTORS",
    "L_DETECTION",
    "L_TRAP_TRIGGER"
};

QStringList DetectCloseToGroundModifierUI::headerLabels = {
    "Name",
    "Type",
    "Bound Variable",
    "Value"
};

DetectCloseToGroundModifierUI::DetectCloseToGroundModifierUI()
    : bsData(nullptr),
      topLyt(new QGridLayout),
      table(new TableWidget(QColor(Qt::white))),
      name(new LineEdit),
      enable(new CheckBox),
      closeToGroundEventPayload(new QLineEdit),
      closeToGroundHeight(new DoubleSpinBox),
      raycastDistanceDown(new DoubleSpinBox),
      collisionFilterInfo(new ComboBox),
      boneIndex(new ComboBox),
      animBoneIndex(new ComboBox)
{
    collisionFilterInfo->insertItems(0, collisionLayers);
    setTitle("hkbDetectCloseToGroundModifier");
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
    table->setItem(CLOSE_TO_GROUND_EVENT_ID_ROW, NAME_COLUMN, new TableWidgetItem("closeToGroundEventId"));
    table->setItem(CLOSE_TO_GROUND_EVENT_ID_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(CLOSE_TO_GROUND_EVENT_ID_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setItem(CLOSE_TO_GROUND_EVENT_ID_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_EVENTS_TABLE_TIP));
    table->setItem(CLOSE_TO_GROUND_EVENT_PAYLOAD_ROW, NAME_COLUMN, new TableWidgetItem("closeToGroundEventPayload"));
    table->setItem(CLOSE_TO_GROUND_EVENT_PAYLOAD_ROW, TYPE_COLUMN, new TableWidgetItem("hkbStringEventPayload", Qt::AlignCenter));
    table->setItem(CLOSE_TO_GROUND_EVENT_PAYLOAD_ROW, BINDING_COLUMN, new TableWidgetItem("N/A", Qt::AlignCenter));
    table->setCellWidget(CLOSE_TO_GROUND_EVENT_PAYLOAD_ROW, VALUE_COLUMN, closeToGroundEventPayload);
    table->setItem(CLOSE_TO_GROUND_HEIGHT_ROW, NAME_COLUMN, new TableWidgetItem("closeToGroundHeight"));
    table->setItem(CLOSE_TO_GROUND_HEIGHT_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(CLOSE_TO_GROUND_HEIGHT_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(CLOSE_TO_GROUND_HEIGHT_ROW, VALUE_COLUMN, closeToGroundHeight);
    table->setItem(RAYCAST_DISTANCE_DOWN_ROW, NAME_COLUMN, new TableWidgetItem("raycastDistanceDown"));
    table->setItem(RAYCAST_DISTANCE_DOWN_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setItem(RAYCAST_DISTANCE_DOWN_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(RAYCAST_DISTANCE_DOWN_ROW, VALUE_COLUMN, raycastDistanceDown);
    table->setItem(COLLISION_FILTER_INFO_ROW, NAME_COLUMN, new TableWidgetItem("collisionFilterInfo"));
    table->setItem(COLLISION_FILTER_INFO_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(COLLISION_FILTER_INFO_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(COLLISION_FILTER_INFO_ROW, VALUE_COLUMN, collisionFilterInfo);
    table->setItem(BONE_INDEX_ROW, NAME_COLUMN, new TableWidgetItem("boneIndex"));
    table->setItem(BONE_INDEX_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(BONE_INDEX_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(BONE_INDEX_ROW, VALUE_COLUMN, boneIndex);
    table->setItem(ANIM_BONE_INDEX_ROW, NAME_COLUMN, new TableWidgetItem("animBoneIndex"));
    table->setItem(ANIM_BONE_INDEX_ROW, TYPE_COLUMN, new TableWidgetItem("hkInt32", Qt::AlignCenter));
    table->setItem(ANIM_BONE_INDEX_ROW, BINDING_COLUMN, new TableWidgetItem(BINDING_ITEM_LABEL+"NONE", Qt::AlignLeft | Qt::AlignVCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_VARIABLES_TABLE_TIP, true));
    table->setCellWidget(ANIM_BONE_INDEX_ROW, VALUE_COLUMN, animBoneIndex);
    topLyt->addWidget(table, 0, 0, 8, 3);
    setLayout(topLyt);
}

void DetectCloseToGroundModifierUI::connectSignals(){
    connect(name, SIGNAL(editingFinished()), this, SLOT(setName()), Qt::UniqueConnection);
    connect(enable, SIGNAL(released()), this, SLOT(setEnable()), Qt::UniqueConnection);
    connect(closeToGroundHeight, SIGNAL(editingFinished()), this, SLOT(setCloseToGroundHeight()), Qt::UniqueConnection);
    connect(raycastDistanceDown, SIGNAL(editingFinished()), this, SLOT(setRaycastDistanceDown()), Qt::UniqueConnection);
    connect(collisionFilterInfo, SIGNAL(currentIndexChanged(int)), this, SLOT(setCollisionFilterInfo(int)), Qt::UniqueConnection);
    connect(boneIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setBoneIndex(int)), Qt::UniqueConnection);
    connect(animBoneIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setAnimBoneIndex(int)), Qt::UniqueConnection);
    connect(closeToGroundEventPayload, SIGNAL(editingFinished()), this, SLOT(setCloseToGroundEventPayload()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)), Qt::UniqueConnection);
}

void DetectCloseToGroundModifierUI::disconnectSignals(){
    disconnect(name, SIGNAL(editingFinished()), this, SLOT(setName()));
    disconnect(enable, SIGNAL(released()), this, SLOT(setEnable()));
    disconnect(closeToGroundHeight, SIGNAL(editingFinished()), this, SLOT(setCloseToGroundHeight()));
    disconnect(raycastDistanceDown, SIGNAL(editingFinished()), this, SLOT(setRaycastDistanceDown()));
    disconnect(collisionFilterInfo, SIGNAL(currentIndexChanged(int)), this, SLOT(setCollisionFilterInfo(int)));
    disconnect(boneIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setBoneIndex(int)));
    disconnect(animBoneIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(setAnimBoneIndex(int)));
    disconnect(closeToGroundEventPayload, SIGNAL(editingFinished()), this, SLOT(setCloseToGroundEventPayload()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelected(int,int)));
}

void DetectCloseToGroundModifierUI::connectToTables(GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *events){
    if (variables && properties && events){
        disconnect(variables, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(properties, SIGNAL(elementSelected(int,QString)), 0, 0);
        disconnect(events, SIGNAL(elementSelected(int,QString)), 0, 0);
        connect(variables, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(properties, SIGNAL(elementSelected(int,QString)), this, SLOT(setBindingVariable(int,QString)), Qt::UniqueConnection);
        connect(events, SIGNAL(elementSelected(int,QString)), this, SLOT(setCloseToGroundEventId(int,QString)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewVariables(int,QString,QStringList)), variables, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewProperties(int,QString,QStringList)), properties, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
        connect(this, SIGNAL(viewEvents(int,QString,QStringList)), events, SLOT(showTable(int,QString,QStringList)), Qt::UniqueConnection);
    }else{
        CRITICAL_ERROR_MESSAGE("DetectCloseToGroundModifierUI::connectToTables(): One or more arguments are nullptr!!");
    }
}

void DetectCloseToGroundModifierUI::loadData(HkxObject *data){
    disconnectSignals();
    if (data){
        if (data->getSignature() == HKB_DETECT_CLOSE_TO_GROUND_MODIFIER){
            bsData = static_cast<hkbDetectCloseToGroundModifier *>(data);
            QStringList boneNames("None");
            hkbVariableBindingSet *varBind = nullptr;
            hkbStringEventPayload *payload = static_cast<hkbStringEventPayload *>(bsData->closeToGroundEvent.payload.data());
            name->setText(bsData->name);
            enable->setChecked(bsData->enable);
            QString text = static_cast<BehaviorFile *>(bsData->getParentFile())->getEventNameAt(bsData->closeToGroundEvent.id);
            if (text != ""){
                table->item(CLOSE_TO_GROUND_EVENT_ID_ROW, VALUE_COLUMN)->setText(text);
            }else{
                table->item(CLOSE_TO_GROUND_EVENT_ID_ROW, VALUE_COLUMN)->setText("None");
            }
            if (payload){
                closeToGroundEventPayload->setText(payload->data);
            }else{
                closeToGroundEventPayload->setText("");
            }
            closeToGroundHeight->setValue(bsData->closeToGroundHeight);
            raycastDistanceDown->setValue(bsData->raycastDistanceDown);
            if (bsData->collisionFilterInfo + 1 >= collisionFilterInfo->count() || bsData->collisionFilterInfo + 1 < 0){
                CRITICAL_ERROR_MESSAGE("DetectCloseToGroundModifierUI::loadData(): Invalid collisionFilterInfo!!!");
                return;
            }
            collisionFilterInfo->setCurrentIndex(bsData->collisionFilterInfo + 1);
            if (boneIndex->count() == 0){
                boneNames = boneNames + static_cast<BehaviorFile *>(bsData->getParentFile())->getRigBoneNames();
                boneIndex->insertItems(0, boneNames);
            }
            boneIndex->setCurrentIndex(bsData->boneIndex + 1);
            if (animBoneIndex->count() == 0){
                boneNames = boneNames + static_cast<BehaviorFile *>(bsData->getParentFile())->getRigBoneNames();
                animBoneIndex->insertItems(0, boneNames);
            }
            animBoneIndex->setCurrentIndex(bsData->animBoneIndex + 1);
            varBind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
            if (varBind){
                loadBinding(ENABLE_ROW, BINDING_COLUMN, varBind, "enable");
                loadBinding(CLOSE_TO_GROUND_HEIGHT_ROW, BINDING_COLUMN, varBind, "closeToGroundHeight");
                loadBinding(RAYCAST_DISTANCE_DOWN_ROW, BINDING_COLUMN, varBind, "raycastDistanceDown");
                loadBinding(COLLISION_FILTER_INFO_ROW, BINDING_COLUMN, varBind, "collisionFilterInfo");
                loadBinding(BONE_INDEX_ROW, BINDING_COLUMN, varBind, "boneIndex");
                loadBinding(ANIM_BONE_INDEX_ROW, BINDING_COLUMN, varBind, "animBoneIndex");
            }else{
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(CLOSE_TO_GROUND_HEIGHT_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(RAYCAST_DISTANCE_DOWN_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(COLLISION_FILTER_INFO_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(BONE_INDEX_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
                table->item(ANIM_BONE_INDEX_ROW, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+"NONE");
            }
        }else{
            CRITICAL_ERROR_MESSAGE("DetectCloseToGroundModifierUI::loadData(): The data is an incorrect type!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("DetectCloseToGroundModifierUI::loadData(): The data is nullptr!!");
    }
    connectSignals();
}

void DetectCloseToGroundModifierUI::setName(){
    if (bsData){
        if (bsData->name != name->text()){
            bsData->name = name->text();
            static_cast<DataIconManager*>((bsData))->updateIconNames();
            bsData->getParentFile()->setIsChanged(true);
            emit modifierNameChanged(name->text(), static_cast<BehaviorFile *>(bsData->getParentFile())->getIndexOfModifier(bsData));
        }
    }else{
        CRITICAL_ERROR_MESSAGE("DetectCloseToGroundModifierUI::setName(): The data is nullptr!!");
    }
}

void DetectCloseToGroundModifierUI::setEnable(){
    if (bsData){
        bsData->enable = enable->isChecked();
        bsData->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("DetectCloseToGroundModifierUI::setEnable(): The data is nullptr!!");
    }
}

void DetectCloseToGroundModifierUI::setCloseToGroundEventId(int index, const QString & name){
    if (bsData){
        index--;
        if (bsData->closeToGroundEvent.id != index){
            bsData->closeToGroundEvent.id = index;
            table->item(CLOSE_TO_GROUND_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("DetectCloseToGroundModifierUI::setcloseToGroundEventId(): The data is nullptr!!");
    }
}

void DetectCloseToGroundModifierUI::setCloseToGroundEventPayload(){
    hkbStringEventPayload *payload;
    if (bsData){
        payload = static_cast<hkbStringEventPayload *>(bsData->closeToGroundEvent.payload.data());
        if (closeToGroundEventPayload->text() != ""){
            if (payload){
                payload->data = closeToGroundEventPayload->text();
            }else{
                payload = new hkbStringEventPayload(bsData->getParentFile(), closeToGroundEventPayload->text());
                //bsData->getParentFile()->addObjectToFile(payload, -1);
                bsData->closeToGroundEvent.payload = HkxSharedPtr(payload);
            }
        }else{
            bsData->closeToGroundEvent.payload = HkxSharedPtr();
        }
        bsData->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("DetectCloseToGroundModifierUI::setcloseToGroundEventPayload(): The data is nullptr!!");
    }
}

void DetectCloseToGroundModifierUI::setCloseToGroundHeight(){
    if (bsData){
        if (bsData->closeToGroundHeight != closeToGroundHeight->value()){
            bsData->closeToGroundHeight = closeToGroundHeight->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("DetectCloseToGroundModifierUI::setcloseToGroundHeight(): The data is nullptr!!");
    }
}

void DetectCloseToGroundModifierUI::setRaycastDistanceDown(){
    if (bsData){
        if (bsData->raycastDistanceDown != raycastDistanceDown->value()){
            bsData->raycastDistanceDown = raycastDistanceDown->value();
            bsData->getParentFile()->setIsChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("DetectCloseToGroundModifierUI::setdistance(): The data is nullptr!!");
    }
}

void DetectCloseToGroundModifierUI::setCollisionFilterInfo(int index){
    if (bsData){
        bsData->collisionFilterInfo = index - 1;
        bsData->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("DetectCloseToGroundModifierUI::setcollisionFilterInfo(): The data is nullptr!!");
    }
}

void DetectCloseToGroundModifierUI::setBoneIndex(int index){
    if (bsData){
        bsData->boneIndex = index - 1;
        if (bsData->boneIndex > -1){   //boneIndex and animBoneIndex cannot simultaneously have nonnegative values!!!
            setAnimBoneIndex(0);
        }
        bsData->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("DetectCloseToGroundModifierUI::setBoneIndex(): The data is nullptr!!");
    }
}

void DetectCloseToGroundModifierUI::setAnimBoneIndex(int index){
    if (bsData){
        bsData->animBoneIndex = index - 1;
        if (bsData->animBoneIndex > -1){   //boneIndex and animBoneIndex cannot simultaneously have nonnegative values!!!
            setBoneIndex(0);
        }
        bsData->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("DetectCloseToGroundModifierUI::setAnimBoneIndex(): The data is nullptr!!");
    }
}

void DetectCloseToGroundModifierUI::viewSelected(int row, int column){
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
            case CLOSE_TO_GROUND_HEIGHT_ROW:
                if (table->item(CLOSE_TO_GROUND_HEIGHT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "closeToGroundHeight");
                break;
            case RAYCAST_DISTANCE_DOWN_ROW:
                if (table->item(RAYCAST_DISTANCE_DOWN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "raycastDistanceDown");
                break;
            case COLLISION_FILTER_INFO_ROW:
                if (table->item(COLLISION_FILTER_INFO_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "collisionFilterInfo");
                break;
            case BONE_INDEX_ROW:
                if (table->item(BONE_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "boneIndex");
                break;
            case ANIM_BONE_INDEX_ROW:
                if (table->item(ANIM_BONE_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                    isProperty = true;
                }
                selectTableToView(isProperty, "animBoneIndex");
                break;
            default:
                return;
            }
        }else if (column == VALUE_COLUMN && row == CLOSE_TO_GROUND_EVENT_ID_ROW){
            emit viewEvents(bsData->closeToGroundEvent.id + 1, QString(), QStringList());
        }
    }else{
        CRITICAL_ERROR_MESSAGE("DetectCloseToGroundModifierUI::viewSelected(): The 'bsData' pointer is nullptr!!");
    }
}

void DetectCloseToGroundModifierUI::selectTableToView(bool viewisProperty, const QString & path){
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
        CRITICAL_ERROR_MESSAGE("DetectCloseToGroundModifierUI::selectTableToView(): The data is nullptr!!");
    }
}

void DetectCloseToGroundModifierUI::eventRenamed(const QString & name, int index){
    if (bsData){
        index--;
        if (index == bsData->closeToGroundEvent.id){
            table->item(CLOSE_TO_GROUND_EVENT_ID_ROW, VALUE_COLUMN)->setText(name);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("DetectCloseToGroundModifierUI::eventRenamed(): The data is nullptr!!");
    }
}

void DetectCloseToGroundModifierUI::variableRenamed(const QString & name, int index){
    if (bsData){
        index--;
        hkbVariableBindingSet *bind = static_cast<hkbVariableBindingSet *>(bsData->variableBindingSet.data());
        if (bind){
            int bindIndex = bind->getVariableIndexOfBinding("enable");
            if (bindIndex == index){
                table->item(ENABLE_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("closeToGroundHeight");
            if (bindIndex == index){
                table->item(CLOSE_TO_GROUND_HEIGHT_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("raycastDistanceDown");
            if (bindIndex == index){
                table->item(RAYCAST_DISTANCE_DOWN_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("collisionFilterInfo");
            if (bindIndex == index){
                table->item(COLLISION_FILTER_INFO_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("boneIndex");
            if (bindIndex == index){
                table->item(BONE_INDEX_ROW, BINDING_COLUMN)->setText(name);
            }
            bindIndex = bind->getVariableIndexOfBinding("animBoneIndex");
            if (bindIndex == index){
                table->item(ANIM_BONE_INDEX_ROW, BINDING_COLUMN)->setText(name);
            }
        }
    }else{
        CRITICAL_ERROR_MESSAGE("DetectCloseToGroundModifierUI::variableRenamed(): The 'bsData' pointer is nullptr!!");
    }
}

bool DetectCloseToGroundModifierUI::setBinding(int index, int row, const QString &variableName, const QString &path, hkVariableType type, bool isProperty){
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
                    CRITICAL_ERROR_MESSAGE("DetectCloseToGroundModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }else{
                if (!varBind->addBinding(path, index - 1, hkbVariableBindingSet::hkBinding::BINDING_TYPE_VARIABLE)){
                    CRITICAL_ERROR_MESSAGE("DetectCloseToGroundModifierUI::setBinding(): The attempt to add a binding to this object's hkbVariableBindingSet failed!!");
                }
            }
            table->item(row, BINDING_COLUMN)->setText(BINDING_ITEM_LABEL+variableName);
            bsData->getParentFile()->setIsChanged(true);
        }else{
            WARNING_MESSAGE("I'M SORRY HAL BUT I CAN'T LET YOU DO THAT.\n\nYou are attempting to bind a variable of an invalid type for this data field!!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("DetectCloseToGroundModifierUI::setBinding(): The data is nullptr!!");
    }
    return true;
}

void DetectCloseToGroundModifierUI::setBindingVariable(int index, const QString &name){
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
        case CLOSE_TO_GROUND_HEIGHT_ROW:
            if (table->item(CLOSE_TO_GROUND_HEIGHT_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "closeToGroundHeight", VARIABLE_TYPE_REAL, isProperty);
            break;
        case RAYCAST_DISTANCE_DOWN_ROW:
            if (table->item(RAYCAST_DISTANCE_DOWN_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "raycastDistanceDown", VARIABLE_TYPE_REAL, isProperty);
            break;
        case COLLISION_FILTER_INFO_ROW:
            if (table->item(COLLISION_FILTER_INFO_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "collisionFilterInfo", VARIABLE_TYPE_INT32, isProperty);
            break;
        case BONE_INDEX_ROW:
            if (table->item(BONE_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "boneIndex", VARIABLE_TYPE_INT32, isProperty);
            break;
        case ANIM_BONE_INDEX_ROW:
            if (table->item(ANIM_BONE_INDEX_ROW, BINDING_COLUMN)->checkState() != Qt::Unchecked){
                isProperty = true;
            }
            setBinding(index, row, name, "animBoneIndex", VARIABLE_TYPE_INT32, isProperty);
            break;
        default:
            return;
        }
        bsData->getParentFile()->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("DetectCloseToGroundModifierUI::setBindingVariable(): The data is nullptr!!");
    }
}

void DetectCloseToGroundModifierUI::loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path){
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
            CRITICAL_ERROR_MESSAGE("DetectCloseToGroundModifierUI::loadBinding(): The variable binding set is nullptr!!");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("DetectCloseToGroundModifierUI::loadBinding(): The data is nullptr!!");
    }
}

