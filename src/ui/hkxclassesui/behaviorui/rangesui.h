#ifndef RANGESUI_H
#define RANGESUI_H

#include <QGroupBox>

#include "src/hkxclasses/behavior/modifiers/hkbsensehandlemodifier.h"

class QGridLayout;
class QLabel;
class QPushButton;
class QLineEdit;
class BehaviorFile;
class DoubleSpinBox;
class CheckBox;
class TableWidget;
class hkbVariableBindingSet;

class RangesUI: public QGroupBox
{
    Q_OBJECT
    friend class SenseHandleModifierUI;
public:
    RangesUI();
    void loadData(BehaviorFile *parentFile, hkbSenseHandleModifier::hkRanges *ranges, hkbSenseHandleModifier *par, int index);
signals:
    void viewEvents(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewProperties(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewVariables(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void returnToParent();
private slots:
    void setEventId(int index, const QString &name);
    void setEventPayload();
    void setMinDistance();
    void setMaxDistance();
    void setIgnoreHandle();
    void viewSelectedChild(int row, int column);
private:
    void connectSignals();
    void disconnectSignals();
    bool setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty);
    void setBindingVariable(int index, const QString & name);
    void loadBinding(int row, int column, hkbVariableBindingSet *varBind, const QString &path);
    void selectTableToView(bool viewproperties, const QString & path);
    void eventRenamed(const QString & name, int index);
    void variableRenamed(const QString & name, int index);
private:
    static QStringList headerLabels;
    BehaviorFile *file;
    hkbSenseHandleModifier::hkRanges *bsData;
    hkbSenseHandleModifier *parent;
    int rangeIndex;
    QGridLayout *topLyt;
    QPushButton *returnPB;
    TableWidget *table;
    QLineEdit *payload;
    DoubleSpinBox *minDistance;
    DoubleSpinBox *maxDistance;
    CheckBox *ignoreHandle;
};

#endif // RANGESUI_H
