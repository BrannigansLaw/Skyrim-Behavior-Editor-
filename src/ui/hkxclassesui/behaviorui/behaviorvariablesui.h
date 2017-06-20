#ifndef BEHAVIORVARIABLESUI_H
#define BEHAVIORVARIABLESUI_H

#include <QGroupBox>

class HkDataUI;
class QVBoxLayout;
class QHBoxLayout;
class QSignalMapper;
class TableWidget;
class QPushButton;
class hkbBehaviorGraphData;
class HkxObject;
class CheckBox;
class ComboBox;
class LineEdit;
class QStackedLayout;
class SpinBox;
class DoubleSpinBox;
class QuadVariableWidget;
class BoneWeightArrayUI;

class BehaviorVariablesUI: public QGroupBox
{
    Q_OBJECT
public:
    BehaviorVariablesUI(const QString & title);
    virtual ~BehaviorVariablesUI(){}
    void setHkDataUI(HkDataUI *ui);
    void loadData(HkxObject *data);
    void clear();
private slots:
    void addVariable();
    void removeVariable();
    void setVariableValue(int type);
    void renameSelectedVariable(int type);
    void viewVariable(int row, int column);
    void returnToTable();
signals:
    void variableNameChanged(const QString & newName, int index);
    void variableAdded(const QString & name, const QString & type);
    void variableRemoved(int index);
private:
    BehaviorVariablesUI& operator=(const BehaviorVariablesUI&);
    BehaviorVariablesUI(const BehaviorVariablesUI &);
    void loadVariable(CheckBox *variableWid);
    void loadVariable(SpinBox *variableWid);
    void loadVariable(DoubleSpinBox *variableWid);
    void loadVariable(QuadVariableWidget *variableWid);
    void addVariableToTable(const QString & name, const QString & type);
    void removeVariableFromTable(int row);
    void hideOtherVariables(int indexToView);
private:
    enum View {
        TABLE_WIDGET = 0,
        VARIABLE_WIDGET = 1,
        BONE_WEIGHTS_WIDGET = 2
    };
    enum hkTypes {
        VARIABLE_TYPE_BOOL = 0,
        VARIABLE_TYPE_INT32 = 1,
        VARIABLE_TYPE_REAL = 2,
        VARIABLE_TYPE_POINTER = 3,
        VARIABLE_TYPE_VECTOR4 = 4,
        VARIABLE_TYPE_QUATERNION = 5
    };
    static QStringList types;
    static QStringList headerLabels;
    HkDataUI *dataUI;
    QVBoxLayout *verLyt;
    hkbBehaviorGraphData *loadedData;
    TableWidget *table;
    QPushButton *addObjectPB;
    QPushButton *removeObjectPB;
    QHBoxLayout *buttonLyt;
    LineEdit *boolName;
    LineEdit *intName;
    LineEdit *doubleName;
    LineEdit *quadName;
    CheckBox *boolCB;
    SpinBox *intSB;
    DoubleSpinBox *doubleSB;
    QuadVariableWidget *quadWidget;
    TableWidget *variableWidget;
    BoneWeightArrayUI *boneWeightArrayWidget;
    QStackedLayout *stackLyt;
    QPushButton *returnBoolPB;
    QPushButton *returnIntPB;
    QPushButton *returnDoublePB;
    QPushButton *returnQuadPB;
    ComboBox *typeSelector;
    QSignalMapper *nameMapper;
    QSignalMapper *valueMapper;
};

#endif // BEHAVIORVARIABLESUI_H
