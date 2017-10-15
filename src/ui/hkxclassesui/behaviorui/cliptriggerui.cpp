#include "cliptriggerui.h"

#include "src/utility.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/behavior/hkbstringeventpayload.h"
#include "src/hkxclasses/hkxobject.h"
#include "src/ui/genericdatawidgets.h"

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

#define BASE_NUMBER_OF_ROWS 6

#define EVENT_ROW 0
#define PAYLOAD_ROW 1
#define LOCAL_TIME_ROW 2
#define RELATIVE_TO_END_ROW 3
#define ACYCLIC_ROW 4
#define ISANNOTATION_ROW 5

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define VALUE_COLUMN 2

QStringList ClipTriggerUI::headerLabels = {
    "Name",
    "Type",
    "Value"
};

ClipTriggerUI::ClipTriggerUI()
    : file(NULL),
      bsData(NULL),
      topLyt(new QGridLayout),
      returnPB(new QPushButton("Return")),
      table(new TableWidget),
      payload(new QLineEdit),
      localTime(new DoubleSpinBox),
      relativeToEndOfClip(new CheckBox),
      acyclic(new CheckBox),
      isAnnotation(new CheckBox)
{
    setTitle("Clip Trigger Editor");
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(EVENT_ROW, NAME_COLUMN, new TableWidgetItem("event"));
    table->setItem(EVENT_ROW, TYPE_COLUMN, new TableWidgetItem("hkEvent", Qt::AlignCenter));
    table->setItem(EVENT_ROW, VALUE_COLUMN, new TableWidgetItem("NONE", Qt::AlignCenter, QColor(Qt::lightGray), QBrush(Qt::black), VIEW_EVENTS_TABLE_TIP));
    table->setItem(PAYLOAD_ROW, NAME_COLUMN, new TableWidgetItem("payload"));
    table->setItem(PAYLOAD_ROW, TYPE_COLUMN, new TableWidgetItem("hkbStringEventPayload", Qt::AlignCenter));
    table->setCellWidget(PAYLOAD_ROW, VALUE_COLUMN, payload);
    table->setItem(LOCAL_TIME_ROW, NAME_COLUMN, new TableWidgetItem("localTime"));
    table->setItem(LOCAL_TIME_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setCellWidget(LOCAL_TIME_ROW, VALUE_COLUMN, localTime);
    table->setItem(RELATIVE_TO_END_ROW, NAME_COLUMN, new TableWidgetItem("relativeToEndOfClip"));
    table->setItem(RELATIVE_TO_END_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(RELATIVE_TO_END_ROW, VALUE_COLUMN, new TableWidgetItem("", Qt::AlignCenter, QColor(Qt::lightGray)));
    table->setCellWidget(RELATIVE_TO_END_ROW, VALUE_COLUMN, relativeToEndOfClip);
    table->setItem(ACYCLIC_ROW, NAME_COLUMN, new TableWidgetItem("acyclic"));
    table->setItem(ACYCLIC_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(ACYCLIC_ROW, VALUE_COLUMN, new TableWidgetItem("", Qt::AlignCenter, QColor(Qt::lightGray)));
    table->setCellWidget(ACYCLIC_ROW, VALUE_COLUMN, acyclic);
    table->setItem(ISANNOTATION_ROW, NAME_COLUMN, new TableWidgetItem("isAnnotation"));
    table->setItem(ISANNOTATION_ROW, TYPE_COLUMN, new TableWidgetItem("hkBool", Qt::AlignCenter));
    table->setItem(ISANNOTATION_ROW, VALUE_COLUMN, new TableWidgetItem("", Qt::AlignCenter, QColor(Qt::lightGray)));
    table->setCellWidget(ISANNOTATION_ROW, VALUE_COLUMN, isAnnotation);
    topLyt->addWidget(returnPB, 0, 1, 1, 1);
    topLyt->addWidget(table, 1, 0, 6, 3);
    setLayout(topLyt);
    connectSignals();
}

void ClipTriggerUI::connectSignals(){
    connect(returnPB, SIGNAL(released()), this, SIGNAL(returnToParent()), Qt::UniqueConnection);
    connect(localTime, SIGNAL(editingFinished()), this, SLOT(setLocalTime()), Qt::UniqueConnection);
    connect(relativeToEndOfClip, SIGNAL(released()), this, SLOT(setRelativeToEndOfClip()), Qt::UniqueConnection);
    connect(acyclic, SIGNAL(released()), this, SLOT(setAcyclic()), Qt::UniqueConnection);
    connect(isAnnotation, SIGNAL(released()), this, SLOT(setIsAnnotation()), Qt::UniqueConnection);
    connect(payload, SIGNAL(editingFinished()), this, SLOT(setEventPayload()), Qt::UniqueConnection);
    connect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)), Qt::UniqueConnection);
}

void ClipTriggerUI::disconnectSignals(){
    disconnect(returnPB, SIGNAL(released()), this, SIGNAL(returnToParent()));
    disconnect(localTime, SIGNAL(editingFinished()), this, SLOT(setLocalTime()));
    disconnect(relativeToEndOfClip, SIGNAL(released()), this, SLOT(setRelativeToEndOfClip()));
    disconnect(acyclic, SIGNAL(released()), this, SLOT(setAcyclic()));
    disconnect(isAnnotation, SIGNAL(released()), this, SLOT(setIsAnnotation()));
    disconnect(payload, SIGNAL(editingFinished()), this, SLOT(setEventPayload()));
    disconnect(table, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewSelectedChild(int,int)));
}

void ClipTriggerUI::loadData(BehaviorFile *parentFile, hkbClipTriggerArray::HkTrigger *trigger){
    disconnectSignals();
    QString text;
    if (parentFile && trigger){
        file = parentFile;
        bsData = trigger;
        text = file->getEventNameAt(trigger->event.id);
        if (text == ""){
            if (trigger->event.id != -1){
                (qWarning("ClipTriggerUI::loadData(): Invalid event id!!!"));
            }
            text = "NONE";
        }
        QString eventName = file->getEventNameAt(bsData->event.id);
        if (eventName != ""){
            table->item(EVENT_ROW, VALUE_COLUMN)->setText(eventName);
        }else{
            table->item(EVENT_ROW, VALUE_COLUMN)->setText("NONE");
        }
        if (trigger->event.payload.data()){
            payload->setText(static_cast<hkbStringEventPayload *>(trigger->event.payload.data())->data);
        }else{
            payload->setText("");
        }
        localTime->setValue(bsData->localTime);
        relativeToEndOfClip->setChecked(bsData->relativeToEndOfClip);
        acyclic->setChecked(bsData->acyclic);
        isAnnotation->setChecked(bsData->isAnnotation);
    }else{
        (qFatal("ClipTriggerUI::loadData(): Behavior file or event data is null!!!"));
    }
    connectSignals();
}

/*QSize ClipTriggerUI::sizeHint() const{
    return QSize(1600, 800);
}

QSize ClipTriggerUI::minimumSizeHint() const{
    return QSize(1200, 600);
}*/

void ClipTriggerUI::setEventId(int index, const QString & name){
    if (bsData && file){
        index--;
        if (bsData->event.id != index){
            bsData->event.id = index;
            table->item(EVENT_ROW, VALUE_COLUMN)->setText(name);
            file->toggleChanged(true);
        }
    }else{
        (qFatal("ClipTriggerUI::setEventId(): Behavior file or event data is null!!!"));
    }
}

void ClipTriggerUI::setEventPayload(){
    hkbStringEventPayload *payloadData;
    if (bsData && file){
        payloadData = static_cast<hkbStringEventPayload *>(bsData->event.payload.data());
        if (payload->text() != ""){
            if (payloadData){
                if (payloadData->data != payload->text()){
                    payloadData->data = payload->text();
                }else{
                    return;
                }
            }else{
                payloadData = new hkbStringEventPayload(file, payload->text());
                file->addObjectToFile(payloadData);
                bsData->event.payload = HkxSharedPtr(payloadData);
            }
        }else{
            bsData->event.payload = HkxSharedPtr();
        }
        file->toggleChanged(true);
    }else{
        (qFatal("ClipTriggerUI::setEventPayload(): Behavior file or event data is null!!!"));
    }
}

void ClipTriggerUI::setLocalTime(){
    if (bsData && file){
        if (bsData->localTime != localTime->value()){
            bsData->localTime = localTime->value();
            file->toggleChanged(true);
        }
    }else{
        (qFatal("ClipTriggerUI::setLocalTime(): Behavior file or event data is null!!!"));
    }
}

void ClipTriggerUI::setRelativeToEndOfClip(){
    if (bsData && file){
        if (bsData->relativeToEndOfClip != relativeToEndOfClip->isChecked()){
            bsData->relativeToEndOfClip = relativeToEndOfClip->isChecked();
            file->toggleChanged(true);
        }
    }else{
        (qFatal("ClipTriggerUI::setRelativeToEndOfClip(): Behavior file or event data is null!!!"));
    }
}

void ClipTriggerUI::setAcyclic(){
    if (bsData && file){
        if (bsData->acyclic != acyclic->isChecked()){
            bsData->acyclic = acyclic->isChecked();
            file->toggleChanged(true);
        }
    }else{
        (qFatal("ClipTriggerUI::setAcyclic(): Behavior file or event data is null!!!"));
    }
}

void ClipTriggerUI::setIsAnnotation(){
    if (bsData && file){
        if (bsData->isAnnotation != isAnnotation->isChecked()){
            bsData->isAnnotation = isAnnotation->isChecked();
            file->toggleChanged(true);
        }
    }else{
        (qFatal("ClipTriggerUI::setIsAnnotation(): Behavior file or event data is null!!!"));
    }
}

void ClipTriggerUI::viewSelectedChild(int row, int column){
    if (bsData){
        if (row == EVENT_ROW && column == VALUE_COLUMN){
            emit viewEvents(bsData->event.id + 1);
        }
    }else{
        (qFatal("ClipTriggerUI::viewSelectedChild(): The data is NULL!!"));
    }
}

void ClipTriggerUI::eventRenamed(const QString & name, int index){
    if (bsData){
        if (index == bsData->event.id){
            table->item(EVENT_ROW, VALUE_COLUMN)->setText(name);
        }
    }else{
        (qFatal("ClipTriggerUI::eventRenamed(): The data is NULL!!"));
    }
}
