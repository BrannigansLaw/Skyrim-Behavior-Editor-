#ifndef GETUPMODIFIERUI_H
#define GETUPMODIFIERUI_H

#include <QGroupBox>

#include "src/utility.h"

class HkxObject;
class QGridLayout;
class TableWidget;
class hkbGetUpModifier;
class LineEdit;
class DoubleSpinBox;
class CheckBox;
class ComboBox;
class GenericTableWidget;
class hkbVariableBindingSet;
class QuadVariableWidget;

class GetUpModifierUI: QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    GetUpModifierUI();
    virtual ~GetUpModifierUI(){}
    void loadData(HkxObject *data);
signals:
    void viewVariables(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewProperties(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void modifierNameChanged(const QString & newName, int index);
private slots:
    void setName();
    void setEnable();
    void setGroundNormal();
    void setDuration();
    void setAlignWithGroundDuration();
    void setRootBoneIndex(int index);
    void setOtherBoneIndex(int index);
    void setAnotherBoneIndex(int index);
    void viewSelected(int row, int column);
    void setBindingVariable(int index, const QString & name);
private:
    void connectSignals();
    void disconnectSignals();
    void connectToTables(GenericTableWidget *variables, GenericTableWidget *properties);
    void variableRenamed(const QString & name, int index);
    void selectTableToView(bool viewproperties, const QString & path);
    bool setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty);
    void loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString & path);
private:
    static QStringList headerLabels;
    hkbGetUpModifier *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    LineEdit *name;
    CheckBox *enable;
    QuadVariableWidget *groundNormal;
    DoubleSpinBox *duration;
    DoubleSpinBox *alignWithGroundDuration;
    ComboBox *rootBoneIndex;
    ComboBox *otherBoneIndex;
    ComboBox *anotherBoneIndex;
};

#endif // GETUPMODIFIERUI_H
