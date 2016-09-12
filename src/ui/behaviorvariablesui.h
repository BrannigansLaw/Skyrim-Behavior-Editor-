#ifndef BEHAVIORVARIABLESUI_H
#define BEHAVIORVARIABLESUI_H

#include <QGroupBox>

class HkDataUI;
class QVBoxLayout;
class QHBoxLayout;
class QSignalMapper;
class QTableWidget;
class QPushButton;
class hkbBehaviorGraphData;
class HkxObject;
class QCheckBox;
class QComboBox;

class BehaviorVariablesUI: public QGroupBox
{
    Q_OBJECT
public:
    BehaviorVariablesUI(const QString & title);
    virtual ~BehaviorVariablesUI(){}
public slots:
    void setHkDataUI(HkDataUI *ui);
    void loadData(HkxObject *data);
    void addVariable();
    void removeVariable();
    void setVariableValue(int index);
    void renameSelectedVariable(int index);
private:
    BehaviorVariablesUI& operator=(const BehaviorVariablesUI&);
    BehaviorVariablesUI(const BehaviorVariablesUI &);
    template <typename T, typename W> void addVariableToTable(const QString & name, const QString & type, const T & value, W *widget);
    template <typename T> void addVariableToTable(const QString & name, const QString & type, const T & value, QCheckBox *widget);
    void removeVariableFromTable(int row);
private:
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
    QTableWidget *table;
    QPushButton *addObjectPB;
    QPushButton *removeObjectPB;
    QHBoxLayout *buttonLyt;
    QSignalMapper *valueMapper;
    QSignalMapper *nameMapper;
    QComboBox *typeSelector;
};

#endif // BEHAVIORVARIABLESUI_H
