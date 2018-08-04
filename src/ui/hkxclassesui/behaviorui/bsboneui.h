#ifndef BSBONEUI_H
#define BSBONEUI_H

#include <QGroupBox>

#include "src/hkxclasses/behavior/modifiers/bslookatmodifier.h"

class QGridLayout;
class QLabel;
class QPushButton;
class QLineEdit;
class BehaviorFile;
class DoubleSpinBox;
class CheckBox;
class TableWidget;
class hkbVariableBindingSet;
class QuadVariableWidget;
class ComboBox;

class BSBoneUI: public QGroupBox
{
    Q_OBJECT
    friend class BSLookAtModifierUI;
public:
    BSBoneUI();
    void loadData(BehaviorFile *parentFile, BSLookAtModifier::BsBone *bon, BSLookAtModifier *par, int ind, bool isEyeBone);
signals:
    void viewProperties(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewVariables(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void returnToParent();
private slots:
    void setIndex(int index);
    void setFwdAxisLS();
    void setLimitAngleDegrees();
    void setOnGain();
    void setOffGain();
    void setEnabled();
    void viewSelectedChild(int row, int column);
private:
    void connectSignals();
    void disconnectSignals();
    bool setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty);
    void setBindingVariable(int index, const QString & name);
    void loadBinding(int row, int column, hkbVariableBindingSet *varBind, const QString &path);
    void selectTableToView(bool viewproperties, const QString & path);
    void variableRenamed(const QString & name, int index);
private:
    static QStringList headerLabels;
    BehaviorFile *file;
    BSLookAtModifier::BsBone *bsData;
    BSLookAtModifier *parent;
    int bsBoneIndex;
    QString parameterName;
    QGridLayout *topLyt;
    QPushButton *returnPB;
    TableWidget *table;
    ComboBox *index;
    QuadVariableWidget *fwdAxisLS;
    DoubleSpinBox *limitAngleDegrees;
    DoubleSpinBox *onGain;
    DoubleSpinBox *offGain;
    CheckBox *enabled;
};

#endif // BSBONEUI_H
