#ifndef LEGUI_H
#define LEGUI_H

#include <QGroupBox>

#include "src/hkxclasses/behavior/modifiers/hkbfootikcontrolsmodifier.h"

class QGridLayout;
class QLabel;
class QPushButton;
class QLineEdit;
class BehaviorFile;
class DoubleSpinBox;
class CheckBox;
class TableWidget;
class hkbVariableBindingSet;
class QuadVariableWidget;

class LegUI: public QGroupBox
{
    Q_OBJECT
    friend class FootIkControlsModifierUI;
public:
    LegUI();
    void loadData(BehaviorFile *parentFile, hkbFootIkControlsModifier::hkLeg *leg, hkbFootIkControlsModifier *par, int index);
signals:
    void viewEvents(int index);
    void viewProperties(int index);
    void viewVariables(int index);
    void returnToParent();
private slots:
    void setEventId(int index, const QString &name);
    void setEventPayload();
    void setGroundPosition();
    void setVerticalError();
    void setHitSomething();
    void setIsPlantedMS();
    void viewSelectedChild(int row, int column);
private:
    void connectSignals();
    void disconnectSignals();
    bool setBinding(int index, int row, const QString & variableName, const QString & path, hkVariableType type, bool isProperty);
    void setBindingVariable(int index, const QString & name);
    void loadBinding(int row, int colunm, hkbVariableBindingSet *varBind, const QString &path);
    void selectTableToView(bool viewproperties, const QString & path);
    void eventRenamed(const QString & name, int index);
    void variableRenamed(const QString & name, int index);
private:
    static QStringList headerLabels;
    BehaviorFile *file;
    hkbFootIkControlsModifier::hkLeg *bsData;
    hkbFootIkControlsModifier *parent;
    int legIndex;
    QGridLayout *topLyt;
    QPushButton *returnPB;
    TableWidget *table;
    QuadVariableWidget *groundPosition;
    QLineEdit *payload;
    DoubleSpinBox *verticalError;
    CheckBox *hitSomething;
    CheckBox *isPlantedMS;
};

#endif // LEGUI_H
