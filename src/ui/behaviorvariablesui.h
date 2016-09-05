#ifndef BEHAVIORVARIABLESUI_H
#define BEHAVIORVARIABLESUI_H

#include <QGroupBox>

class HkDataUI;
class QVBoxLayout;
class QHBoxLayout;
class GenericListWidget;
class QStackedLayout;
class QLabel;
class DoubleWidget;
class QuadVariableWidget;
class IntWidget;
class BoolWidget;
class QPushButton;

class hkbBehaviorGraphData;
class HkxObject;

class BehaviorVariablesUI: public QGroupBox
{
    Q_OBJECT
public:
    BehaviorVariablesUI(const QString & title);
    virtual ~BehaviorVariablesUI(){}
public slots:
    void changeCurrentDataWidget(int index, int column);
    void loadTable();
    void setHkDataUI(HkDataUI *ui);
    void loadData(HkxObject *data);
    void addVariable(int type);
    void removeVariable(int index);
    void setBoolVariableValue();
    void setIntVariableValue();
    void setDoubleVariableValue();
    void setVector4VariableValue();
    void setQuaternionVariableValue();
    void renameSelectedVariable(const QString & name);
private:
    BehaviorVariablesUI& operator=(const BehaviorVariablesUI&);
    BehaviorVariablesUI(const BehaviorVariablesUI &);
private:
    enum WidgetType {
        TABLE_WIDGET = 0,
        BOOL_WIDGET = 1,
        INT_WIDGET = 2,
        DOUBLE_WIDGET = 3,
        VECTOR_4_WIDGET = 4,
        QUATERNION_WIDGET = 5
    };
    enum hkTypes {
        VARIABLE_TYPE_BOOL = 0,
        VARIABLE_TYPE_INT32 = 1,
        VARIABLE_TYPE_REAL = 2,
        VARIABLE_TYPE_POINTER = 3,
        VARIABLE_TYPE_VECTOR4 = 4,
        VARIABLE_TYPE_QUATERNION = 5
    };
    HkDataUI *dataUI;
    QVBoxLayout *verLyt;
    QStackedLayout *stack;
    hkbBehaviorGraphData *loadedData;
    QLabel *noDataL;
    QPushButton *backPB;
    GenericListWidget *view;
    BoolWidget *boolWidget;
    IntWidget *intWidget;
    DoubleWidget *doubleWidget;
    QuadVariableWidget *quadVariableWidget;
    QuadVariableWidget *quaternionWidget;
};

#endif // BEHAVIORVARIABLESUI_H
