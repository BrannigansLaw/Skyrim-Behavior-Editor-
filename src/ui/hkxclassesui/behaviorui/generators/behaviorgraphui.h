#ifndef BEHAVIORGRAPHUI_H
#define BEHAVIORGRAPHUI_H

#include <QGroupBox>

class HkxObject;
class QGridLayout;
class TableWidget;
class BehaviorGraphView;
class hkbBehaviorGraph;
class LineEdit;
class ComboBox;
class GenericTableWidget;

class BehaviorGraphUI: QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    BehaviorGraphUI();
    void loadData(HkxObject *data);
signals:
    void generatorNameChanged(const QString & newName, int index);
    void viewGenerators(int index);
private slots:
    void setName();
    void setVariableMode(int index);
    void viewSelectedChild(int row, int column);
    void setRootGenerator(int index, const QString &name);
private:
    void setBehaviorView(BehaviorGraphView *view);
    void connectToTables(GenericTableWidget *generators);
private:
    static QStringList headerLabels;
    BehaviorGraphView *behaviorView;
    hkbBehaviorGraph *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    LineEdit *name;
    ComboBox *variableMode;
};

#endif // BEHAVIORGRAPHUI_H
