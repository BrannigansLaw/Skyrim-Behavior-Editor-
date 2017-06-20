#ifndef CLIPTRIGGERUI_H
#define CLIPTRIGGERUI_H

#include <QGroupBox>

#include "src/hkxclasses/behavior/hkbcliptriggerarray.h"

class QGridLayout;
class QLabel;
class QPushButton;
class QLineEdit;
class BehaviorFile;
class DoubleSpinBox;
class CheckBox;
class TableWidget;

class ClipTriggerUI: public QGroupBox
{
    Q_OBJECT
    friend class StateMachineUI;
    friend class StateUI;
    friend class ClipGeneratorUI;
public:
    ClipTriggerUI();
    void loadData(BehaviorFile *parentFile, hkbClipTriggerArray::HkTrigger *trigger);
    //QSize sizeHint() const Q_DECL_OVERRIDE;
    //QSize minimumSizeHint() const Q_DECL_OVERRIDE;
signals:
    void viewEvents(int index);
    void returnToParent();
private slots:
    void setEvent(int index, const QString &name);
    void setEventPayload();
    void setLocalTime();
    void setRelativeToEndOfClip();
    void setAcyclic();
    void setIsAnnotation();
    void viewSelectedChild(int row, int column);
private:
    void connectSignals();
    void disconnectSignals();
    void eventRenamed(const QString & name, int index);
private:
    static QStringList headerLabels;
    BehaviorFile *file;
    hkbClipTriggerArray::HkTrigger *bsData;
    QGridLayout *topLyt;
    QPushButton *returnPB;
    TableWidget *table;
    QLineEdit *payload;
    DoubleSpinBox *localTime;
    CheckBox *relativeToEndOfClip;
    CheckBox *acyclic;
    CheckBox *isAnnotation;
};

#endif // CLIPTRIGGERUI_H
