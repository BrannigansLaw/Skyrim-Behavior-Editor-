#ifndef BSRAGDOLLCONTACTLISTENERMODIFIERUI_H
#define BSRAGDOLLCONTACTLISTENERMODIFIERUI_H

#include <QStackedWidget>

#include "src/utility.h"

class HkxObject;
class QGridLayout;
class TableWidget;
class BSRagdollContactListenerModifier;
class LineEdit;
class QLineEdit;
class QGroupBox;
class CheckBox;
class BoneIndexArrayUI;
class GenericTableWidget;
class hkbVariableBindingSet;
class CheckButtonCombo;

class BSRagdollContactListenerModifierUI: QStackedWidget
{
    Q_OBJECT
    friend class HkDataUI;
public:
    BSRagdollContactListenerModifierUI();
    virtual ~BSRagdollContactListenerModifierUI(){}
    void loadData(HkxObject *data);
signals:
    void viewVariables(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewProperties(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewEvents(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewRagdollBones(int index);
    void modifierNameChanged(const QString & newName, int index);
private slots:
    void setName();
    void setEnable();
    void setContactEventId(int index, const QString & name);
    void setContactEventPayload();
    void toggleBones(bool enable);
    void viewBones();
    void viewSelected(int row, int column);
    void setBindingVariable(int index, const QString & name);
private:
    void connectSignals();
    void disconnectSignals();
    void connectToTables(GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *events, GenericTableWidget *ragdollBones);
    void variableRenamed(const QString & name, int index);
    void eventRenamed(const QString & name, int index);
    void selectTableToView(bool viewproperties, const QString & path);
    bool setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty);
    void loadBinding(int row, int column, hkbVariableBindingSet *varBind, const QString & path);
private:
    enum ACTIVE_WIDGET {
        MAIN_WIDGET = 0,
        BONE_INDEX_WIDGET = 1
    };
    static QStringList headerLabels;
    BSRagdollContactListenerModifier *bsData;
    QGridLayout *topLyt;
    QGroupBox *groupBox;
    TableWidget *table;
    BoneIndexArrayUI *boneIndexUI;
    LineEdit *name;
    CheckBox *enable;
    QLineEdit *contactEventPayload;
    CheckButtonCombo *bones;
};

#endif // BSRAGDOLLCONTACTLISTENERMODIFIERUI_H
