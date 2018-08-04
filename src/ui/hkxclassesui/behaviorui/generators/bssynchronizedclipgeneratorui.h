#ifndef BSSYNCHRONIZEDCLIPGENERATORUI_H
#define BSSYNCHRONIZEDCLIPGENERATORUI_H

#include <QGroupBox>

#include "src/utility.h"

class TableWidget;
class BSSynchronizedClipGenerator;
class QGridLayout;
class HkxObject;
class DoubleSpinBox;
class LineEdit;
class BehaviorGraphView;
class GenericTableWidget;
class hkbVariableBindingSet;
class CheckBox;
class SpinBox;

class BSSynchronizedClipGeneratorUI: public QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    BSSynchronizedClipGeneratorUI();
    virtual ~BSSynchronizedClipGeneratorUI(){}
    void loadData(HkxObject *data);
signals:
    void generatorNameChanged(const QString & newName, int index);
    void viewVariables(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewGenerators(int index, const QString & typeallowed, const QStringList &typesdisallowed);
    void viewProperties(int index, const QString & typeallowed, const QStringList &typesdisallowed);
private slots:
    void setName();
    void setClipGenerator(int index, const QString & name);
    void setSyncAnimPrefix();
    void setSyncClipIgnoreMarkPlacement();
    void setGetToMarkTime();
    void setMarkErrorThreshold();
    void setLeadCharacter();
    void setReorientSupportChar();
    void setApplyMotionFromRoot();
    void setAnimationBindingIndex();
    void viewSelectedChild(int row, int column);
    void setBindingVariable(int index, const QString & name);
private:
    void connectSignals();
    void disconnectSignals();
    void selectTableToView(bool viewproperties, const QString & path);
    void loadBinding(int row, int column, hkbVariableBindingSet *varBind, const QString &path);
    bool setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty);
    void connectToTables(GenericTableWidget *generators, GenericTableWidget *variables, GenericTableWidget *properties);
    void variableRenamed(const QString & name, int index);
    void generatorRenamed(const QString & name, int index);
    void setBehaviorView(BehaviorGraphView *view);
private:
    static QStringList headerLabels;
    BehaviorGraphView *behaviorView;
    BSSynchronizedClipGenerator *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    LineEdit *name;
    LineEdit *syncAnimPrefix;
    CheckBox *bSyncClipIgnoreMarkPlacement;
    DoubleSpinBox *fGetToMarkTime;
    DoubleSpinBox *fMarkErrorThreshold;
    CheckBox *bLeadCharacter;
    CheckBox *bReorientSupportChar;
    CheckBox *bApplyMotionFromRoot;
    SpinBox *sAnimationBindingIndex;
};

#endif // BSSYNCHRONIZEDCLIPGENERATORUI_H
