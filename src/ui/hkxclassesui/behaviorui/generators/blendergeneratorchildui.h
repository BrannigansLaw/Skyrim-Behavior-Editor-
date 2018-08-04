#ifndef BLENDERGENERATORCHILDUI_H
#define BLENDERGENERATORCHILDUI_H

#include <QStackedWidget>

#include "src/utility.h"

class BoneWeightArrayUI;
class TableWidget;
class hkbBlenderGeneratorChild;
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

class BlenderGeneratorChildUI final: public QStackedWidget
{
    Q_OBJECT
    friend class BlenderGeneratorUI;
    friend class PoseMatchingGeneratorUI;
public:
    BlenderGeneratorChildUI();
    BlenderGeneratorChildUI& operator=(const BlenderGeneratorChildUI&) = delete;
    BlenderGeneratorChildUI(const BlenderGeneratorChildUI &) = delete;
    ~BlenderGeneratorChildUI() = default;
    void loadData(HkxObject *data, int childindex);
    void setBehaviorView(BehaviorGraphView *view);
    void connectToTables(GenericTableWidget *generators, GenericTableWidget *variables, GenericTableWidget *properties);
    void variableRenamed(const QString & name, int index);
    void generatorRenamed(const QString & name, int index);
signals:
    void viewVariables(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewGenerators(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewProperties(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void returnToParent(bool reloadData);
private slots:
    void setWeight();
    void setWorldFromModelWeight();
    void viewSelected(int row, int column);
    void viewBoneWeights();
    void toggleBoneWeights(bool enable);
    void returnToWidget();
    void generatorTableElementSelected(int index, const QString &name);
    void variableTableElementSelected(int index, const QString &name);
private:
    void toggleSignals(bool toggleconnections);
    void setGenerator(int index, const QString & name);
    void loadBinding(int row, int column, hkbVariableBindingSet *varBind, const QString &path);
    void setBindingVariable(int index, const QString & name);
    void selectTableToView(bool viewproperties, const QString & path);
    void setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty);
private:
    enum ACTIVE_WIDGET {
        MAIN_WIDGET,
        BONE_WEIGHT_ARRAY_WIDGET
    };
private:
    static const QStringList headerLabels;
    BehaviorGraphView *behaviorView;
    hkbBlenderGeneratorChild *bsData;
    int childIndex;
    QGridLayout *topLyt;
    QGroupBox *groupBox;
    QPushButton *returnPB;
    TableWidget *table;
    CheckButtonCombo *boneWeights;
    BoneWeightArrayUI *boneWeightArrayUI;
    DoubleSpinBox *weight;
    DoubleSpinBox *worldFromModelWeight;
};

#endif // BLENDERGENERATORCHILDUI_H
