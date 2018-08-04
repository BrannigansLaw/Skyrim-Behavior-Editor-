#ifndef DETECTCLOSETOGROUNDMODIFIERUI_H
#define DETECTCLOSETOGROUNDMODIFIERUI_H

#include <QGroupBox>

#include "src/utility.h"

class HkxObject;
class QGridLayout;
class TableWidget;
class hkbDetectCloseToGroundModifier;
class LineEdit;
class QLineEdit;
class DoubleSpinBox;
class CheckBox;
class ComboBox;
class GenericTableWidget;
class hkbVariableBindingSet;
class QuadVariableWidget;

class DetectCloseToGroundModifierUI: QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    DetectCloseToGroundModifierUI();
    virtual ~DetectCloseToGroundModifierUI(){}
    void loadData(HkxObject *data);
signals:
    void viewVariables(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewProperties(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewEvents(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void modifierNameChanged(const QString & newName, int index);
private slots:
    void setName();
    void setEnable();
    void setCloseToGroundEventId(int index, const QString & name);
    void setCloseToGroundEventPayload();
    void setCloseToGroundHeight();
    void setRaycastDistanceDown();
    void setCollisionFilterInfo(int index);
    void setBoneIndex(int index);
    void setAnimBoneIndex(int index);
    void viewSelected(int row, int column);
    void setBindingVariable(int index, const QString & name);
private:
    void connectSignals();
    void disconnectSignals();
    void connectToTables(GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *events);
    void variableRenamed(const QString & name, int index);
    void eventRenamed(const QString & name, int index);
    void selectTableToView(bool viewproperties, const QString & path);
    bool setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty);
    void loadBinding(int row, int column, hkbVariableBindingSet *varBind, const QString & path);
private:
    static QStringList collisionLayers;
    static QStringList headerLabels;
    hkbDetectCloseToGroundModifier *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    LineEdit *name;
    CheckBox *enable;
    QLineEdit *closeToGroundEventPayload;
    DoubleSpinBox *closeToGroundHeight;
    DoubleSpinBox *raycastDistanceDown;
    ComboBox *collisionFilterInfo;
    ComboBox *boneIndex;
    ComboBox *animBoneIndex;
};

#endif // DETECTCLOSETOGROUNDMODIFIERUI_H
