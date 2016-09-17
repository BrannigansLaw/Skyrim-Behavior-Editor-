#ifndef EVENTSUI_H
#define EVENTSUI_H

#include <QGroupBox>

class HkDataUI;
class QVBoxLayout;
class QHBoxLayout;
class QTableWidget;
class QPushButton;
class QSignalMapper;
class hkbBehaviorGraphData;
class HkxObject;

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
    void setBoolVariableValue(int index);
    void renameSelectedEvent(int index);
signals:
    void eventNameChanged(const QString & newName, int index);
    void eventAdded(const QString & name);
    void eventRemoved(int index);
private:
    EventsUI& operator=(const EventsUI&);
    EventsUI(const EventsUI &);
    void addEventToTable(const QString & name, const QString &flags);
    void removeEventFromTable(int row);
private:
    static QStringList headerLabels;
    HkDataUI *dataUI;
    QVBoxLayout *verLyt;
    hkbBehaviorGraphData *loadedData;
    QTableWidget *table;
    QPushButton *addObjectPB;
    QPushButton *removeObjectPB;
    QHBoxLayout *buttonLyt;
    QSignalMapper *flagsMapper;
    QSignalMapper *nameMapper;
};

#endif // EVENTSUI_H
