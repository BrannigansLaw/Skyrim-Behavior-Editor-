#ifndef BSOFFSETANIMATIONGENERATORUI_H
#define BSOFFSETANIMATIONGENERATORUI_H

#include <QGroupBox>

#include "src/utility.h"

class HkxObject;
class BehaviorGraphView;
class BSOffsetAnimationGenerator;
class QGridLayout;
class TableWidget;
class LineEdit;
class GenericTableWidget;
class DoubleSpinBox;
class hkbVariableBindingSet;

class BSOffsetAnimationGeneratorUI: public QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    BSOffsetAnimationGeneratorUI();
    virtual ~BSOffsetAnimationGeneratorUI(){}
    void loadData(HkxObject *data);
signals:
    void generatorNameChanged(const QString & newName, int index);
    void viewVariables(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewGenerators(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewProperties(int index, const QString & typeallowed, const QStringList &typesdisallowed);
private slots:
    void setName();
    void setBindingVariable(int index, const QString & name);
    void setGenerator(int index, const QString & name);
    void setFOffsetVariable();
    void setFOffsetRangeStart();
    void setFOffsetRangeEnd();
    void viewSelected(int row, int column);
private:
    void connectSignals();
    void disconnectSignals();
    void variableRenamed(const QString & name, int index);
    void generatorRenamed(const QString & name, int index);
    void selectTableToView(bool viewproperties, const QString & path);
    bool setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty);
    void loadBinding(int row, int column, hkbVariableBindingSet *varBind, const QString &path);
    void connectToTables(GenericTableWidget *generators, GenericTableWidget *variables, GenericTableWidget *properties);
    void setBehaviorView(BehaviorGraphView *view);
private:
    static QStringList headerLabels;
    BehaviorGraphView *behaviorView;
    BSOffsetAnimationGenerator *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    LineEdit *name;
    DoubleSpinBox *fOffsetVariable;
    DoubleSpinBox *fOffsetRangeStart;
    DoubleSpinBox *fOffsetRangeEnd;
};

#endif // BSOFFSETANIMATIONGENERATORUI_H
