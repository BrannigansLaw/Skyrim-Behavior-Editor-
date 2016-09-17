#ifndef TRANSITIONSUI_H
#define TRANSITIONSUI_H

#include <QGroupBox>

#include "src/hkxclasses/hkbstatemachinetransitioninfoarray.h"

class BehaviorGraphView;
class QTableWidget;
class QLineEdit;
class QComboBox;
class hkbStateMachineTransitionInfoArray;
class QVBoxLayout;
class HkxObject;
class QSpinBox;
class QDoubleSpinBox;
class QCheckBox;
class BlendingTransitionEffectUI;
class QStackedLayout;
class QPushButton;

class TransitionsUI: public QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    TransitionsUI();
    virtual ~TransitionsUI(){}
    void loadData(const hkbStateMachineTransitionInfoArray::HkTransition & data);
signals:
    void returnToParent();
private slots:
    void setName();
private:
    void addEventToLists(const QString & name);
    void removeEventFromLists(int index);
    void renameEventInLists(const QString & newName, int index);
    void addVariableToLists(const QString & name);
    void removeVariableFromLists(int index);
    void renameVariableInLists(const QString & newName, int index);
    //void addGeneratorToLists(const QString & name);
    //void removeGeneratorFromLists(int index);
    //void renameGeneratorInLists(const QString & name, int index);
    //void addModifierToLists(const QString & name);
    //void removeModifierFromLists(int index);
    //void renameModifierInLists(const QString & name, int index);
private:
    static QStringList headerLabels1;
    BehaviorGraphView *behaviorView;
    hkbStateMachineTransitionInfoArray::HkTransition bsData;
    QPushButton *returnToParentPB;
    QVBoxLayout *lyt;
    QTableWidget *table;
    QGroupBox *triggerIntervalGB;
    QComboBox *enterEventIdTI;
    QComboBox *exitEventIdTI;
    QDoubleSpinBox *enterTimeTI;
    QDoubleSpinBox *exitTimeTI;
    QGroupBox *initiateIntervalGB;
    QComboBox *enterEventIdII;
    QComboBox *exitEventIdII;
    QDoubleSpinBox *enterTimeII;
    QDoubleSpinBox *exitTimeII;
    BlendingTransitionEffectUI *transition;
    QLineEdit *condition;
    QComboBox *eventId;
    QComboBox *toStateId;
    QComboBox *fromNestedStateId;
    QComboBox *toNestedStateId;
    QSpinBox *priority;
    QCheckBox *flagWildcard;
    QCheckBox *flagLocal;
    QCheckBox *flagUseNestedState;
    QCheckBox *flagDisallowRandomTransition;
    QCheckBox *flagDisallowReturnToState;
    QCheckBox *flagAbutEndState;
    QStackedLayout *stackLyt;
};

#endif // TRANSITIONSUI_H
