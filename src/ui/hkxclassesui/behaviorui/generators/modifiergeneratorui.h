#ifndef MODIFIERGENERATORUI_H
#define MODIFIERGENERATORUI_H

#include <QGroupBox>

class HkxObject;
class BehaviorGraphView;
class hkbModifierGenerator;
class QVBoxLayout;
class TableWidget;
class QLineEdit;
class ComboBox;
class QPushButton;
class GenericTableWidget;

class ModifierGeneratorUI: public QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    ModifierGeneratorUI(GenericTableWidget *genTable, GenericTableWidget *modTable);
    virtual ~ModifierGeneratorUI(){}
    void loadData(HkxObject *data);
    void setGeneratorTable(GenericTableWidget *genTable);
    void setModifierTable(GenericTableWidget *modTable);
signals:
    void generatorNameChanged(const QString & newName, int index);
    void viewGeneratorTable(int index);
private slots:
    void setName();
    void setModifier(int index);
    void setGenerator(int index);
    void viewGenerators();
    void viewModifiers();
private:
    static QStringList headerLabels1;
    GenericTableWidget *generatorTable;
    GenericTableWidget *modifiersTable;
    BehaviorGraphView *behaviorView;
    hkbModifierGenerator *bsData;
    QVBoxLayout *lyt;
    TableWidget *table;
    QLineEdit *name;
    QPushButton *modifier;
    QPushButton *generator;
};

#endif // MODIFIERGENERATORUI_H
