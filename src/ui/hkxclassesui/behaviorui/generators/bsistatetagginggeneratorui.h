#ifndef BSISTATETAGGINGGENERATORUI_H
#define BSISTATETAGGINGGENERATORUI_H

#include <QGroupBox>

#include "src/utility.h"

class BehaviorGraphView;
class TableWidget;
class LineEdit;
class BSiStateTaggingGenerator;
class QGridLayout;
class HkxObject;
class SpinBox;
class GenericTableWidget;
class hkbVariableBindingSet;

class BSiStateTaggingGeneratorUI: public QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    BSiStateTaggingGeneratorUI();
    virtual ~BSiStateTaggingGeneratorUI(){}
    void loadData(HkxObject *data);
signals:
    void viewVariables(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewGenerators(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewProperties(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void generatorNameChanged(const QString & newName, int index);
private slots:
    void setName();
    void setIStateToSetAs();
    void setIPriority();
    void setBindingVariable(int index, const QString & name);
    void viewSelected(int row, int column);
    void setDefaultGenerator(int index, const QString & name);
private:
    void connectSignals();
    void disconnectSignals();
    void connectToTables(GenericTableWidget *variables, GenericTableWidget *properties, GenericTableWidget *generators);
    void loadBinding(int row, int column, hkbVariableBindingSet *varBind, const QString &path);
    void variableRenamed(const QString & name, int index);
    void generatorRenamed(const QString & name, int index);
    void setBehaviorView(BehaviorGraphView *view);
    void selectTableToView(bool viewproperties, const QString & path);
    void setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty);
private:
    static QStringList headerLabels;
    BehaviorGraphView *behaviorView;
    BSiStateTaggingGenerator *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    LineEdit *name;
    SpinBox *iStateToSetAs;
    SpinBox *iPriority;
};

#endif // BSISTATETAGGINGGENERATORUI_H
