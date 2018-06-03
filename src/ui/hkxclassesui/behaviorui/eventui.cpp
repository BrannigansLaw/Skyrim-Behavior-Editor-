#include "eventui.h"
#include "src/utility.h"
#include "src/filetypes/behaviorfile.h"
#include "src/hkxclasses/behavior/hkbstringeventpayload.h"
#include "src/hkxclasses/hkxobject.h"

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

EventUI::EventUI()
    : file(nullptr),
      topLyt(new QGridLayout),
      returnPB(new QPushButton("Return")),
      nameL(new QLabel("Event Name:")),
      selectEvent(new QPushButton("NONE")),
      payloadL(new QLabel("Payload String:")),
      eventPayload(new QLineEdit),
      eventData(nullptr)
{
    setMinimumHeight(400);
    setMaximumHeight(400);
    setMinimumWidth(600);
    setMaximumWidth(600);
    //topLyt->setSizeConstraint(QLayout::SetMaximumSize);
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    setTitle("Event Payload Editor");
    topLyt->addWidget(returnPB, 0, 2, 1, 1);
    topLyt->addWidget(nameL, 2, 0, 1, 1);
    topLyt->addWidget(selectEvent, 2, 2, 1, 2);
    topLyt->addWidget(payloadL, 4, 0, 1, 1);
    topLyt->addWidget(eventPayload, 4, 2, 1, 2);
    setLayout(topLyt);
    connectSignals();
}

void EventUI::connectSignals(){
    connect(returnPB, SIGNAL(released()), this, SIGNAL(returnToParent()), Qt::UniqueConnection);
    connect(selectEvent, SIGNAL(released()), this, SLOT(emitViewEvent()), Qt::UniqueConnection);
    connect(eventPayload, SIGNAL(editingFinished()), this, SLOT(setEventPayload()), Qt::UniqueConnection);
}

void EventUI::disconnectSignals(){
    disconnect(returnPB, SIGNAL(released()), this, SIGNAL(returnToParent()));
    disconnect(selectEvent, SIGNAL(released()), this, SLOT(emitViewEvent()));
    disconnect(eventPayload, SIGNAL(editingFinished()), this, SLOT(setEventPayload()));
}

void EventUI::loadData(BehaviorFile *parentFile, hkEventPayload * event){
    disconnectSignals();
    QString text;
    if (parentFile && event){
        file = parentFile;
        eventData = event;
        text = file->getEventNameAt(event->id);
        if (text == ""){
            if (event->id != -1){
                WARNING_MESSAGE("EventUI::loadData(): Invalid event id!!!");
            }
            text = "NONE";
        }
        selectEvent->setText(text);
        if (event->payload.data()){
            eventPayload->setText(static_cast<hkbStringEventPayload *>(event->payload.data())->data);
        }else{
            eventPayload->setText("");
        }
    }else{
        CRITICAL_ERROR_MESSAGE("EventUI::loadData(): Behavior file or event data is null!!!");
    }
    connectSignals();
}

QSize EventUI::sizeHint() const{
    return QSize(1600, 800);
}

QSize EventUI::minimumSizeHint() const{
    return QSize(1200, 600);
}

void EventUI::setEvent(int index, const QString & name){
    if (eventData && file){
        eventData->id = index - 1;
        selectEvent->setText(name);
        file->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("EventUI::setEvent(): Behavior file or event data is null!!!");
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
                eventData->payload = HkxSharedPtr(payload);
            }
        }else{
            eventData->payload = HkxSharedPtr();
        }
        file->setIsChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("EventUI::setEventPayload(): Behavior file or event data is null!!!");
    }
}

void EventUI::emitViewEvent(){
    if (eventData){
        emit viewEvents(eventData->id + 1, QString(), QStringList());
    }else{
        CRITICAL_ERROR_MESSAGE("EventUI::emitViewEvent(): Event data is null!!!");
    }
}

void EventUI::eventRenamed(const QString & name, int index){
    if (eventData){
        if (index == eventData->id){
            selectEvent->setText(name);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("EventUI::eventRenamed(): The data is nullptr!!");
    }
}
