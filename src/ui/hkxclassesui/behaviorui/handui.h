#ifndef HANDUI_H
#define HANDUI_H

#include <QGroupBox>

#include "src/hkxclasses/behavior/modifiers/hkbhandikcontrolsmodifier.h"

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

class HandUI: public QGroupBox
{
    Q_OBJECT
    friend class hkbHandIkControlsModifierUI;
    friend class HandIkControlsModifierUI;
public:
    HandUI();
    void loadData(BehaviorFile *parentFile, hkbHandIkControlsModifier::hkHand *bon, hkbHandIkControlsModifier *par, int ind);
signals:
    void viewProperties(int index);
    void viewVariables(int index);
    void returnToParent();
private slots:
    void setTargetPosition();
    void setTargetRotation();
    void setTargetNormal();
    void setTransformOnFraction();
    void setNormalOnFraction();
    void setFadeInDuration();
    void setFadeOutDuration();
    void setExtrapolationTimeStep();
    void setHandleChangeSpeed();
    void setHandleChangeMode(const QString & mode);
    void setFixUp();
    void setHandIndex(int index);
    void setEnable();
    void viewSelectedChild(int row, int column);
private:
    void connectSignals();
    void disconnectSignals();
    bool setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty);
    void setBindingVariable(int index, const QString & name);
    void loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path);
    void selectTableToView(bool viewproperties, const QString & path);
    void variableRenamed(const QString & name, int index);
private:
    static QStringList headerLabels;
    BehaviorFile *file;
    hkbHandIkControlsModifier::hkHand *bsData;
    hkbHandIkControlsModifier *parent;
    int bsBoneIndex;
    QGridLayout *topLyt;
    QPushButton *returnPB;
    TableWidget *table;
    QuadVariableWidget *targetPosition;
    QuadVariableWidget *targetRotation;
    QuadVariableWidget *targetNormal;
    DoubleSpinBox *transformOnFraction;
    DoubleSpinBox *normalOnFraction;
    DoubleSpinBox *fadeInDuration;
    DoubleSpinBox *fadeOutDuration;
    DoubleSpinBox *extrapolationTimeStep;
    DoubleSpinBox *handleChangeSpeed;
    ComboBox *handleChangeMode;
    CheckBox *fixUp;
    ComboBox *handIndex;
    CheckBox *enable;
};

#endif // HANDUI_H
