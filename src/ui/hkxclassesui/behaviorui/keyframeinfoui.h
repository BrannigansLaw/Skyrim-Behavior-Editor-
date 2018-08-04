#ifndef KEYFRAMEINFOUI_H
#define KEYFRAMEINFOUI_H

#include <QGroupBox>

#include "src/hkxclasses/behavior/modifiers/hkbkeyframebonesmodifier.h"

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

class KeyframeInfoUI: public QGroupBox
{
    Q_OBJECT
    friend class KeyframeBonesModifierUI;
public:
    KeyframeInfoUI();
    void loadData(BehaviorFile *parentFile, hkbKeyframeBonesModifier::hkKeyframeInfo *key, hkbKeyframeBonesModifier *par, int index);
signals:
    void viewProperties(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewVariables(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void returnToParent();
private slots:
    void setKeyframedPosition();
    void setKeyframedRotation();
    void setBoneIndex(int index);
    void setIsValid();
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
    hkbKeyframeBonesModifier::hkKeyframeInfo *bsData;
    hkbKeyframeBonesModifier *parent;
    int keyframeIndex;
    QGridLayout *topLyt;
    QPushButton *returnPB;
    TableWidget *table;
    QuadVariableWidget *keyframedPosition;
    QuadVariableWidget *keyframedRotation;
    ComboBox *boneIndex;
    CheckBox *isValid;
};

#endif // KEYFRAMEINFOUI_H
