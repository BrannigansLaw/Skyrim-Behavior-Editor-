#ifndef EVENTSUI_H
#define EVENTSUI_H

#include <QGroupBox>

class HkDataUI;
class QVBoxLayout;
class QHBoxLayout;
class TableWidget;
class QPushButton;
class QSignalMapper;
class hkbBehaviorGraphData;
class HkxObject;
class QStackedLayout;
class QLineEdit;
class QCheckBox;

class EventsUI: public QGroupBox
{
    Q_OBJECT
public:
    EventsUI(const QString & title);
    virtual ~EventsUI(){}
    void setHkDataUI(HkDataUI *ui);
    void loadData(HkxObject *data);
private slots:
    void addEvent();
    void removeEvent();
    void setBoolVariableValue();
    void renameSelectedEvent();
    void viewEvent(int row, int column);
    void returnToTable();
signals:
    void eventNameChanged(const QString & newName, int index);
    void eventAdded(const QString & name);
    void eventRemoved(int index);
private:
    EventsUI& operator=(const EventsUI&);
    EventsUI(const EventsUI &);
private:
    enum View {
        TABLE_WIDGET = 0,
        EVENT_WIDGET = 1
    };
    static QStringList headerLabels;
    HkDataUI *dataUI;
    QVBoxLayout *verLyt;
    hkbBehaviorGraphData *loadedData;
    TableWidget *table;
    QPushButton *addObjectPB;
    QPushButton *removeObjectPB;
    QHBoxLayout *buttonLyt;
    QLineEdit *eventName;
    QCheckBox *flag;
    TableWidget *eventWidget;
    QStackedLayout *stackLyt;
    QPushButton *returnPB;
};

#endif // EVENTSUI_H
