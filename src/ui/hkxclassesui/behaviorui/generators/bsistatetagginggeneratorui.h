#ifndef BSISTATETAGGINGGENERATORUI_H
#define BSISTATETAGGINGGENERATORUI_H

#include <QGroupBox>

class BehaviorGraphView;
class TableWidget;
class QLineEdit;
class ComboBox;
class BSiStateTaggingGenerator;
class QVBoxLayout;
class HkxObject;
class SpinBox;

class BSiStateTaggingGeneratorUI: public QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    BSiStateTaggingGeneratorUI();
    virtual ~BSiStateTaggingGeneratorUI(){}
    void loadData(HkxObject *data);
signals:
    void generatorNameChanged(const QString & newName, int index);
private slots:
    void setName();
    void setDefaultGenerator(int index);
    void setIStateToSetAs();
    void setIPriority();
    void setIStateToSetAsBind(int index);
    void setIPriorityBind(int index);
private:
    void addVariableToLists(const QString & name);
    void removeVariableFromLists(int index);
    void renameVariableInLists(const QString & newName, int index);
    void addGeneratorToLists(const QString & name);
    void removeGeneratorFromLists(int index);
    void renameGeneratorInLists(const QString & name, int index);
    void loadComboBoxes();
private:
    static QStringList headerLabels1;
    BehaviorGraphView *behaviorView;
    BSiStateTaggingGenerator *bsData;
    QVBoxLayout *lyt;
    TableWidget *table;
    QLineEdit *name;
    ComboBox *pDefaultGenerator;
    SpinBox *iStateToSetAs;
    ComboBox *iStateToSetAsBind;
    SpinBox *iPriority;
    ComboBox *iPriorityBind;
};

#endif // BSISTATETAGGINGGENERATORUI_H
