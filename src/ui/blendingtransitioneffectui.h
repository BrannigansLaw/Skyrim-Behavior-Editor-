#ifndef BLENDINGTRANSITIONEFFECTUI_H
#define BLENDINGTRANSITIONEFFECTUI_H

#include <QGroupBox>

class BehaviorGraphView;
class QTableWidget;
class QLineEdit;
class QComboBox;
class hkbBlendingTransitionEffect;
class QVBoxLayout;
class HkxObject;
class QSpinBox;
class QDoubleSpinBox;
class QCheckBox;

class BlendingTransitionEffectUI: public QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    BlendingTransitionEffectUI();
    virtual ~BlendingTransitionEffectUI(){}
    void loadData(HkxObject *data){}
private slots:
    void setName(){}
private:
    //void addEventToLists(const QString & name);
    //void removeEventFromLists(int index);
    //void renameEventInLists(const QString & newName, int index);
    void addVariableToLists(const QString & name){}
    void removeVariableFromLists(int index){}
    void renameVariableInLists(const QString & newName, int index){}
    //void addGeneratorToLists(const QString & name);
    //void removeGeneratorFromLists(int index);
    //void renameGeneratorInLists(const QString & name, int index);
    //void addModifierToLists(const QString & name);
    //void removeModifierFromLists(int index);
    //void renameModifierInLists(const QString & name, int index);
private:
    static QStringList headerLabels1;
    BehaviorGraphView *behaviorView;
    hkbBlendingTransitionEffect *bsData;
    QVBoxLayout *lyt;
    QTableWidget *table;
    QLineEdit *name;
    QComboBox *selfTransitionMode;
    QComboBox *eventMode;
    QDoubleSpinBox *duration;
    QDoubleSpinBox *toGeneratorStartTimeFraction;
    QCheckBox *flagSync;
    QCheckBox *flagIgnoreFromWorldFromModel;
    QComboBox *endMode;
    QComboBox *blendCurve;
};

#endif // BLENDINGTRANSITIONEFFECTUI_H
