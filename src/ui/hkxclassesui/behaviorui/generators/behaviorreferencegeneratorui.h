#ifndef BEHAVIORREFERENCEGENERATORUI_H
#define BEHAVIORREFERENCEGENERATORUI_H

#include <QGroupBox>

class HkxObject;
class ComboBox;
class hkbBehaviorReferenceGenerator;
class QGridLayout;
class TableWidget;
class LineEdit;

class BehaviorReferenceGeneratorUI: public QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    BehaviorReferenceGeneratorUI();
    virtual ~BehaviorReferenceGeneratorUI(){}
    void loadData(HkxObject *data);
signals:
    void generatorNameChanged(const QString & newName, int index);
private slots:
    void setName();
    void setBehaviorName(const QString &text);
private:
    void connectSignals();
    void disconnectSignals();
    void behaviorRenamed(const QString & name, int index);
private:
    static QStringList headerLabels;
    hkbBehaviorReferenceGenerator *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    LineEdit *name;
    ComboBox *behaviorName;
};

#endif // BEHAVIORREFERENCEGENERATORUI_H
