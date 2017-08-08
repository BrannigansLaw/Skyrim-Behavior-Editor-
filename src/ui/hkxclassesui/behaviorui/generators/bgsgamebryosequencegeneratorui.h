#ifndef BGSGAMEBRYOSEQUENCEGENERATORUI_H
#define BGSGAMEBRYOSEQUENCEGENERATORUI_H

#include <QGroupBox>

class HkxObject;
class ComboBox;
class BGSGamebryoSequenceGenerator;
class QGridLayout;
class TableWidget;
class LineEdit;
class DoubleSpinBox;

class BGSGamebryoSequenceGeneratorUI: public QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
public:
    BGSGamebryoSequenceGeneratorUI();
    virtual ~BGSGamebryoSequenceGeneratorUI(){}
    void loadData(HkxObject *data);
signals:
    void generatorNameChanged(const QString & newName, int index);
private slots:
    void setName();
    void setSequence();
    void setBlendModeFunction(int index);
    void setPercent();
private:
    void connectSignals();
    void disconnectSignals();
private:
    static QStringList headerLabels;
    BGSGamebryoSequenceGenerator *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    LineEdit *name;
    LineEdit *pSequence;
    ComboBox *eBlendModeFunction;
    DoubleSpinBox *fPercent;
};

#endif // BGSGAMEBRYOSEQUENCEGENERATORUI_H
