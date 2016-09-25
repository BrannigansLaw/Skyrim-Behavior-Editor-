#ifndef MODIFIERGENERATORUI_H
#define MODIFIERGENERATORUI_H

#include <QGroupBox>

class HkxObject;
class BehaviorGraphView;
class hkbModifierGenerator;
class QVBoxLayout;
class QTableWidget;
class QLineEdit;
class QComboBox;

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
private slots:
    void setName();
    void setModifier(int index);
    void setGenerator(int index);
private:
    //void addEventToLists(const QString & name);
    //void removeEventFromLists(int index);
    //void renameEventInLists(const QString & newName, int index);
    //void addVariableToLists(const QString & name);
    //void removeVariableFromLists(int index);
    //void renameVariableInLists(const QString & newName, int index);
    void addGeneratorToLists(const QString & name);
    void removeGeneratorFromLists(int index);
    void renameGeneratorInLists(const QString & name, int index);
    void addModifierToLists(const QString & name);
    void removeModifierFromLists(int index);
    void renameModifierInLists(const QString & name, int index);
    void loadComboBoxes();
private:
    static QStringList headerLabels1;
    BehaviorGraphView *behaviorView;
    hkbModifierGenerator *bsData;
    QVBoxLayout *lyt;
    QTableWidget *table;
    QLineEdit *name;
    QComboBox *modifier;
    QComboBox *generator;
};

#endif // MODIFIERGENERATORUI_H
