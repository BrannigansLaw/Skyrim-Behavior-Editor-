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

class BlenderGeneratorChildUI: public QStackedWidget
{
    Q_OBJECT
    friend class BlenderGeneratorUI;
    friend class PoseMatchingGeneratorUI;
public:
    BlenderGeneratorChildUI();
    virtual ~BlenderGeneratorChildUI(){}
    void loadData(HkxObject *data, int childindex);
signals:
    void viewVariables(int index);
    void viewGenerators(int index);
    void viewProperties(int index);
    void returnToParent(bool reloadData);
private slots:
    void setWeight();
    void setWorldFromModelWeight();
    void viewSelected(int row, int column);
    void viewBoneWeights();
    void toggleBoneWeights(bool enable);
    void returnToWidget();
private:
    void connectSignals();
    void disconnectSignals();
    void setGenerator(int index, const QString & name);
    void loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path);
    void setBindingVariable(int index, const QString & name);
    void variableRenamed(const QString & name, int index);
    void generatorRenamed(const QString & name, int index);
    void setBehaviorView(BehaviorGraphView *view);
    void selectTableToView(bool viewproperties, const QString & path);
    bool setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty);
private:
    enum ACTIVE_WIDGET {
        MAIN_WIDGET = 0,
        BONE_WEIGHT_ARRAY_WIDGET = 1
    };
    static QStringList headerLabels;
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
