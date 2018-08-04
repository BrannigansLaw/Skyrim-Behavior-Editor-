#ifndef BLENDINGTRANSITIONEFFECTUI_H
#define BLENDINGTRANSITIONEFFECTUI_H

#include <QGroupBox>

#include "src/utility.h"

class QGroupBox;
class TableWidget;
class LineEdit;
class ComboBox;
class hkbBlendingTransitionEffect;
class QGridLayout;
class HkxObject;
class SpinBox;
class DoubleSpinBox;
class CheckBox;
class QPushButton;
class hkbVariableBindingSet;

class BlendingTransitionEffectUI: public QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
    friend class TransitionsUI;
public:
    BlendingTransitionEffectUI();
    virtual ~BlendingTransitionEffectUI(){}
    void loadData(HkxObject *data);
signals:
    void viewVariables(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewProperties(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void transitionEffectRenamed(const QString & name);
    void returnToParent();
private slots:
    void setName();
    void setBindingVariable(int index, const QString & name);
    void setSelfTransitionMode(int index);
    void setEventMode(int index);
    void setDuration();
    void setToGeneratorStartTimeFraction();
    void setEndMode(int index);
    void setBlendCurve(int index);
    void toggleSyncFlag();
    void toggleIgnoreFromWorldFromModelFlag();
    void toggleIgnoreToWorldFromModelFlag();
    void viewSelectedChild(int row, int column);
private:
    void connectSignals();
    void disconnectSignals();
    void loadBinding(int row, int column, hkbVariableBindingSet *varBind, const QString & path);
    void selectTableToView(bool viewproperties, const QString & path);
    void variableRenamed(const QString & name, int index);
    void setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty);
private:
    static QStringList headerLabels;
    hkbBlendingTransitionEffect *bsData;
    QGridLayout *topLyt;
    QPushButton *returnPB;
    TableWidget *table;
    LineEdit *name;
    ComboBox *selfTransitionMode;
    ComboBox *eventMode;
    DoubleSpinBox *duration;
    DoubleSpinBox *toGeneratorStartTimeFraction;
    CheckBox *flagSync;
    CheckBox *flagIgnoreFromWorldFromModel;
    CheckBox *flagIgnoreToWorldFromModel;
    ComboBox *endMode;
    ComboBox *blendCurve;
};

#endif // BLENDINGTRANSITIONEFFECTUI_H
