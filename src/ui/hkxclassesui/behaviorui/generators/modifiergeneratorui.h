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
class HkxObjectTableWidget;

class ModifierGeneratorUI: public QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    ModifierGeneratorUI(HkxObjectTableWidget *genTable, HkxObjectTableWidget *modTable);
    virtual ~ModifierGeneratorUI(){}
    void loadData(HkxObject *data);
    void setGeneratorTable(HkxObjectTableWidget *genTable);
    void setModifierTable(HkxObjectTableWidget *modTable);
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
    HkxObjectTableWidget *generatorTable;
    HkxObjectTableWidget *modifiersTable;
    BehaviorGraphView *behaviorView;
    hkbModifierGenerator *bsData;
    QVBoxLayout *lyt;
    TableWidget *table;
    QLineEdit *name;
    QPushButton *modifier;
    QPushButton *generator;
};

#endif // MODIFIERGENERATORUI_H
