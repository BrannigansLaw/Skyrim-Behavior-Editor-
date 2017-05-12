#include "eventui.h"
#include "src/utility.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/behavior/hkbstringeventpayload.h"
#include "src/hkxclasses/hkxobject.h"

#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

EventUI::EventUI()
    : file(NULL),
      topLyt(new QFormLayout),
      returnPB(new QPushButton("Return")),
      nameL(new QLabel("Event Name:")),
      selectEvent(new QPushButton("NONE")),
      payloadL(new QLabel("Payload String:")),
      eventPayload(new QLineEdit)
{
    setTitle("Event Payload Editor");
    topLyt->addWidget(returnPB);
    topLyt->addRow(nameL, selectEvent);
    topLyt->addRow(payloadL, eventPayload);
    setLayout(topLyt);
    connect(returnPB, SIGNAL(released()), this, SIGNAL(returnToParent()), Qt::UniqueConnection);
    connect(selectEvent, SIGNAL(released()), this, SLOT(emitViewEvent()), Qt::UniqueConnection);
    connect(eventPayload, SIGNAL(editingFinished()), this, SLOT(setEventPayload()), Qt::UniqueConnection);
}

void EventUI::loadData(BehaviorFile *parentFile, hkEventPayload * event){
    blockSignals(true);
    QString text;
    if (parentFile && event){
        file = parentFile;
        eventData = event;
        text = file->getEventNameAt(event->id);
        selectEvent->setText(text);
        if (text == "" && event->id != -1){
            WARNING_MESSAGE(QString("EventUI::loadData(): Invalid event id!!!"));
        }
        if (event->payload.data()){
            eventPayload->setText(static_cast<hkbStringEventPayload *>(event->payload.data())->data);
        }else{
            eventPayload->setText("");
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EventUI::loadData(): Behavior file or event data is null!!!"));
    }
    blockSignals(false);
}

void EventUI::setEvent(int index, const QString & name){
    if (eventData && file){
        eventData->id = index - 1;
        selectEvent->setText(name);
        file->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EventUI::setEvent(): Behavior file or event data is null!!!"));
    }
}

void EventUI::setEventPayload(){
    hkbStringEventPayload *payload;
    if (eventData && file){
        payload = static_cast<hkbStringEventPayload *>(eventData->payload.data());
        if (eventPayload->text() != ""){
            if (payload){
                payload->data = eventPayload->text();
            }else{
                payload = new hkbStringEventPayload(file, eventPayload->text());
                file->addObjectToFile(payload);
                eventData->payload = HkxSharedPtr(payload);
            }
        }else{
            eventData->payload = HkxSharedPtr();
        }
        file->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EventUI::setEventPayload(): Behavior file or event data is null!!!"));
    }
}

void EventUI::emitViewEvent(){
    if (eventData){
        emit viewEvents(eventData->id + 1);
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EventUI::emitViewEvent(): Event data is null!!!"));
    }
}

void EventUI::eventRenamed(const QString & name, int index){
    if (eventData){
        if (index == eventData->id){
            selectEvent->setText(name);
        }
    }else{
        CRITICAL_ERROR_MESSAGE(QString("EventUI::eventRenamed(): The data is NULL!!"))
    }
}
