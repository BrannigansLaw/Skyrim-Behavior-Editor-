#ifndef MODIFIERGENERATORUI_H
#define MODIFIERGENERATORUI_H

#include <QGroupBox>

class HkxObject;
class BehaviorGraphView;
class hkbModifierGenerator;
class QGridLayout;
class TableWidget;
class LineEdit;
class GenericTableWidget;

class ModifierGeneratorUI: public QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    ModifierGeneratorUI();
    virtual ~ModifierGeneratorUI(){}
    void loadData(HkxObject *data);
signals:
    void generatorNameChanged(const QString & newName, int index);
    void viewGenerators(int index);
    void viewModifiers(int index);
private slots:
    void setName();
    void setModifier(int index, const QString & name);
    void setGenerator(int index, const QString & name);
    void viewSelected(int row, int column);
private:
    void connectSignals();
    void disconnectSignals();
    void generatorRenamed(const QString & name, int index);
    void modifierRenamed(const QString & name, int index);
    void connectToTables(GenericTableWidget *modifiers, GenericTableWidget *generators);
    void setBehaviorView(BehaviorGraphView *view);
private:
    static QStringList headerLabels;
    BehaviorGraphView *behaviorView;
    hkbModifierGenerator *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    LineEdit *name;
};

#endif // MODIFIERGENERATORUI_H
