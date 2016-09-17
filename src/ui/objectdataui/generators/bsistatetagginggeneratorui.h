#ifndef BSISTATETAGGINGGENERATORUI_H
#define BSISTATETAGGINGGENERATORUI_H

#include <QGroupBox>

class BehaviorGraphView;
class QTableWidget;
class QLineEdit;
class QComboBox;
class BSiStateTaggingGenerator;
class QVBoxLayout;
class HkxObject;
class QSpinBox;

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
    //void addEventToLists(const QString & name);
    //void removeEventFromLists(int index);
    //void renameEventInLists(const QString & newName, int index);
    void addVariableToLists(const QString & name);
    void removeVariableFromLists(int index);
    void renameVariableInLists(const QString & newName, int index);
    void addGeneratorToLists(const QString & name);
    void removeGeneratorFromLists(int index);
    void renameGeneratorInLists(const QString & name, int index);
    //void addModifierToLists(const QString & name);
    //void removeModifierFromLists(int index);
    //void renameModifierInLists(const QString & name, int index);
private:
    static QStringList headerLabels1;
    BehaviorGraphView *behaviorView;
    BSiStateTaggingGenerator *bsData;
    QVBoxLayout *lyt;
    QTableWidget *table;
    QLineEdit *name;
    QComboBox *pDefaultGenerator;
    QSpinBox *iStateToSetAs;
    QComboBox *iStateToSetAsBind;
    QSpinBox *iPriority;
    QComboBox *iPriorityBind;
};

#endif // BSISTATETAGGINGGENERATORUI_H
