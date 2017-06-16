#ifndef CLIPGENERATORUI_H
#define CLIPGENERATORUI_H

#include <QGroupBox>

#include "src/utility.h"

class HkxObject;
class QGridLayout;
class TableWidget;
class hkbClipGenerator;
class LineEdit;
class DoubleSpinBox;
class QCheckBox;
class ComboBox;
class GenericTableWidget;
class hkbVariableBindingSet;

class ClipGeneratorUI: QGroupBox
{
    /*Q_OBJECT
    friend class HkDataUI;
public:
    ClipGeneratorUI();
    virtual ~ClipGeneratorUI(){}
    void loadData(HkxObject *data);
signals:
    void viewVariables(int index);
    void viewProperties(int index);
    void viewEvents(int index);
    void viewAnimations(int index);
    void generatorNameChanged(const QString & newName, int index);
private slots:
    void setName();
    void setEnable();
    void setLimitAngleDegrees();
    void setStartBoneIndex(int index);
    void setEndBoneIndex(int index);
    void setGain();
    void setBoneRadius();
    void setCastOffset();
    void viewSelected(int row, int column);
    void setBindingVariable(int index, const QString & name);
private:
    void connectSignals();
    void disconnectSignals();
    void connectToTables(GenericTableWidget *variables, GenericTableWidget *properties);
    void variableRenamed(const QString & name, int index);
    void selectTableToView(bool viewproperties, const QString & path);
    bool setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty);
    void loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString & path);
private:
    static QStringList headerLabels;
    hkbClipGenerator *bsData;
    QGridLayout *topLyt;
    TableWidget *table;
    LineEdit *name;
    QCheckBox *enable;
    DoubleSpinBox *limitAngleDegrees;
    ComboBox *startBoneIndex;
    ComboBox *endBoneIndex;
    DoubleSpinBox *gain;
    DoubleSpinBox *boneRadius;
    DoubleSpinBox *castOffset;*/
};

#endif // CLIPGENERATORUI_H
