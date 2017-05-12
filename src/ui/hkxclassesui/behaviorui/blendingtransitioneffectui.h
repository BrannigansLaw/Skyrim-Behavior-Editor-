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
class QCheckBox;
class QPushButton;

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
    void viewVariables(int index);
    void viewProperties(int index);
    void returnToParent();
private slots:
    void setName();
    void setBindingVariable(int index, const QString & name);
    void setSelfTransitionMode();
    void setEventMode();
    void setDuration();
    void setToGeneratorStartTimeFraction();
    void setEndMode();
    void setBlendCurve();
    void setFlagSync();
    void setFlagIgnoreFromWorldFromModel();
    void setFlagIgnoreToWorldFromModel();
private:
    void variableRenamed(const QString & name, int index);
    bool setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type);
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
    QCheckBox *flagSync;
    QCheckBox *flagIgnoreFromWorldFromModel;
    QCheckBox *flagIgnoreToWorldFromModel;
    ComboBox *endMode;
    ComboBox *blendCurve;
};

#endif // BLENDINGTRANSITIONEFFECTUI_H
