#ifndef EVENTUI_H
#define EVENTUI_H

#include <QGroupBox>

class QGridLayout;
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
    friend class BSCyclicBlendTransitionGeneratorUI;
public:
    EventUI();
    void loadData(BehaviorFile *parentFile, hkEventPayload *event);
    QSize sizeHint() const Q_DECL_OVERRIDE;
    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
signals:
    void viewEvents(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void returnToParent();
private slots:
    void setEvent(int index, const QString &name);
    void setEventPayload();
    void emitViewEvent();
private:
    void connectSignals();
    void disconnectSignals();
    void eventRenamed(const QString & name, int index);
private:
    BehaviorFile *file;
    hkEventPayload *eventData;
    QGridLayout *topLyt;
    QPushButton *returnPB;
    QLabel *nameL;
    QPushButton *selectEvent;
    QLabel *payloadL;
    QLineEdit *eventPayload;
};

#endif // EVENTUI_H
