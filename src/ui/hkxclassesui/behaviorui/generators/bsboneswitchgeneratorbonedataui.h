#ifndef BSBONESWITCHGENERATORBONEDATAUI_H
#define BSBONESWITCHGENERATORBONEDATAUI_H

#include <QStackedWidget>

#include "src/utility.h"

class BoneWeightArrayUI;
class TableWidget;
class BSBoneSwitchGeneratorBoneData;
class HkxObject;
class DoubleSpinBox;
class CheckBox;
class ComboBox;
class QPushButton;
class BehaviorGraphView;
class QGroupBox;
class QGridLayout;
class CheckButtonCombo;
class hkbVariableBindingSet;
class GenericTableWidget;

class BSBoneSwitchGeneratorBoneDataUI: public QStackedWidget
{
    Q_OBJECT
    friend class BSBoneSwitchGeneratorUI;
    friend class HkDataUI;
public:
    BSBoneSwitchGeneratorBoneDataUI();
    virtual ~BSBoneSwitchGeneratorBoneDataUI(){}
    void loadData(HkxObject *data, int childindex);
signals:
    void viewVariables(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewGenerators(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewProperties(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void returnToParent(bool reloadData);
private slots:
    void viewSelected(int row, int column);
    void viewBoneWeights();
    void toggleBoneWeights(bool enable);
    void returnToWidget();
    void generatorTableElementSelected(int index, const QString &name);
    void variableTableElementSelected(int index, const QString &name);
private:
    void connectSignals();
    void disconnectSignals();
    void setGenerator(int index, const QString & name);
    void loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path);
    void setBindingVariable(int index, const QString & name);
    void variableRenamed(const QString & name, int index);
    void generatorRenamed(const QString & name, int index);
    void setBehaviorView(BehaviorGraphView *view);
    void connectToTables(GenericTableWidget *generators, GenericTableWidget *variables, GenericTableWidget *properties);
    void selectTableToView(bool viewproperties, const QString & path);
    bool setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty);
private:
    enum ACTIVE_WIDGET {
        MAIN_WIDGET = 0,
        BONE_WEIGHT_ARRAY_WIDGET = 1
    };
    static QStringList headerLabels;
    BehaviorGraphView *behaviorView;
    BSBoneSwitchGeneratorBoneData *bsData;
    int childIndex;
    QGridLayout *topLyt;
    QGroupBox *groupBox;
    QPushButton *returnPB;
    TableWidget *table;
    CheckButtonCombo *spBoneWeight;
    BoneWeightArrayUI *boneWeightArrayUI;
};

#endif // BSBONESWITCHGENERATORBONEDATAUI_H
