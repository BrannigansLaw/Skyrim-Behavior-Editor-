#ifndef EVENTUI_H
#define EVENTUI_H

#include <QGroupBox>

class QFormLayout;
class QLabel;
class QPushButton;
class QLineEdit;
class BehaviorFile;
struct hkEventPayload;

class EventUI: public QGroupBox
{
    Q_OBJECT
    friend class StateMachineUI;
    friend class StateUI;
public:
    EventUI();
    void loadData(BehaviorFile *parentFile, hkEventPayload *event);
signals:
    void viewEvents(int index);
    void returnToParent();
private slots:
    void setEvent(int index, const QString &name);
    void setEventPayload();
    void emitViewEvent();
private:
    void eventRenamed(const QString & name, int index);
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
