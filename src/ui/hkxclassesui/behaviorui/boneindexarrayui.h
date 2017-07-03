#ifndef BONEINDEXARRAYUI_H
#define BONEINDEXARRAYUI_H

#include <QGroupBox>

#include "src/utility.h"

class HkxObject;
class BehaviorGraphView;
class GenericTableWidget;
class hkbBoneIndexArray;
class TableWidget;
class SpinBox;
class ComboBox;
class LineEdit;
class CheckBox;
class QGridLayout;
class hkbVariableBindingSet;

class BoneIndexArrayUI: public QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    BoneIndexArrayUI();
    virtual ~BoneIndexArrayUI(){}
    void loadData(HkxObject *data);
signals:
    void returnToParent();
    void viewRagdollBones(int index);
private slots:
    void setRagdollBone(int index, const QString & name);
    void viewSelectedChild(int row, int column);
private:
    void connectSignals();
    void disconnectSignals();
    void removeRagdollBone(int index);
    void setRowItems(int row, const QString & name, const QString & bind, const QString & value, const QString & tip1, const QString & tip2);
    void connectToTables(GenericTableWidget *ragdollBones);
    void addRagdollBone();
    void loadDynamicTableRows();
private:
    static QStringList types;
    static QStringList headerLabels;
    hkbBoneIndexArray *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    QPushButton *returnPB;
};

#endif // BONEINDEXARRAYUI_H
