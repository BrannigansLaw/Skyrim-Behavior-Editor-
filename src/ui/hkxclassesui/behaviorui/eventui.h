#ifndef EVENTUI_H
#define EVENTUI_H

#include <QGroupBox>

class QFormLayout;
class QLabel;
class QPushButton;
class QLineEdit;
class BehaviorFile;
class hkEventPayload;

class EventUI: public QGroupBox
{
public:
    EventUI();
    void loadData(BehaviorFile *parentFile, hkEventPayload *event);
signals:
    void viewEvents(int index);
    void returnToParent();
private slots:
    void setEvent(int index);
    void setEventPayload();
    void emitViewEvent();
private:
    BehaviorFile *file;
    hkEventPayload *eventData;
    QFormLayout *topLyt;
    QPushButton *returnPB;
    QLabel *nameL;
    QPushButton *selectEvent;
    QLabel *payloadL;
    QLineEdit *eventPayload;
};

#endif // EVENTUI_H
