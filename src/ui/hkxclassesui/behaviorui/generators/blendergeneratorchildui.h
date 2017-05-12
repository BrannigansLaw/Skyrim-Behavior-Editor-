#ifndef BLENDERGENERATORCHILDUI_H
#define BLENDERGENERATORCHILDUI_H

#include <QStackedWidget>

#include "src/utility.h"

class BoneWeightArrayUI;
class TableWidget;
class hkbBlenderGeneratorChild;
class QVBoxLayout;
class HkxObject;
class DoubleSpinBox;
class QCheckBox;
class QStackedLayout;
class ComboBox;
class QPushButton;
class BehaviorGraphView;
class QGroupBox;
class QGridLayout;

class BlenderGeneratorChildUI: public QStackedWidget
{
    Q_OBJECT
    friend class BlenderGeneratorUI;
public:
    BlenderGeneratorChildUI();
    virtual ~BlenderGeneratorChildUI(){}
    void loadData(HkxObject *data);
signals:
    void viewVariables(int index);
    void viewGenerators(int index);
    void viewProperties(int index);
    void returnToParent();
private slots:
    void setBindingVariable(int index, const QString & name);
    void setWeight();
    void setWorldFromModelWeight();
    void setGenerator(int index, const QString & name);
    void viewSelected(int row, int column);
    void returnToWidget();
private:
    void variableRenamed(const QString & name, int index);
    void generatorRenamed(const QString & name, int index);
    void setBehaviorView(BehaviorGraphView *view);
    bool setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type);
private:
    enum ACTIVE_WIDGET {
        MAIN_WIDGET = 0,
        BONE_WEIGHTS_ARRAY_WIDGET = 1
    };
    static QStringList headerLabels;
    BehaviorGraphView *behaviorView;
    hkbBlenderGeneratorChild *bsData;
    QGridLayout *topLyt;
    QGroupBox *groupBox;
    QPushButton *returnPB;
    TableWidget *table;
    BoneWeightArrayUI *boneWeights;
    DoubleSpinBox *weight;
    DoubleSpinBox *worldFromModelWeight;
};

#endif // BLENDERGENERATORCHILDUI_H
