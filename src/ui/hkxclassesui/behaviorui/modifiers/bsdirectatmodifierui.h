#ifndef BSDIRECTATMODIFIERUI_H
#define BSDIRECTATMODIFIERUI_H

#include <QGroupBox>

#include "src/utility.h"

class HkxObject;
class QGridLayout;
class TableWidget;
class BSDirectAtModifier;
class LineEdit;
class DoubleSpinBox;
class CheckBox;
class ComboBox;
class GenericTableWidget;
class hkbVariableBindingSet;
class QuadVariableWidget;
class SpinBox;

class BSDirectAtModifierUI: QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    BSDirectAtModifierUI();
    virtual ~BSDirectAtModifierUI(){}
    void loadData(HkxObject *data);
signals:
    void viewVariables(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewProperties(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void modifierNameChanged(const QString & newName, int index);
private slots:
    void setName();
    void setEnable();
    void setDirectAtTarget();
    void setSourceBoneIndex(int index);
    void setStartBoneIndex(int index);
    void setEndBoneIndex(int index);
    void setLimitHeadingDegrees();
    void setLimitPitchDegrees();
    void setOffsetHeadingDegrees();
    void setOffsetPitchDegrees();
    void setOnGain();
    void setOffGain();
    void setTargetLocation();
    void setUserInfo();
    void setDirectAtCamera();
    void setDirectAtCameraX();
    void setDirectAtCameraY();
    void setDirectAtCameraZ();
    void setActive();
    void setCurrentHeadingOffset();
    void setCurrentPitchOffset();
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
    BSDirectAtModifier *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    LineEdit *name;
    CheckBox *enable;
    CheckBox *directAtTarget;
    ComboBox *sourceBoneIndex;
    ComboBox *startBoneIndex;
    ComboBox *endBoneIndex;
    DoubleSpinBox *limitHeadingDegrees;
    DoubleSpinBox *limitPitchDegrees;
    DoubleSpinBox *offsetHeadingDegrees;
    DoubleSpinBox *offsetPitchDegrees;
    DoubleSpinBox *onGain;
    DoubleSpinBox *offGain;
    QuadVariableWidget *targetLocation;
    SpinBox *userInfo;
    CheckBox *directAtCamera;
    DoubleSpinBox *directAtCameraX;
    DoubleSpinBox *directAtCameraY;
    DoubleSpinBox *directAtCameraZ;
    CheckBox *active;
    DoubleSpinBox *currentHeadingOffset;
    DoubleSpinBox *currentPitchOffset;
};

#endif // BSDIRECTATMODIFIERUI_H
