#ifndef FOOTIKDRIVERINFOLEGUI_H
#define FOOTIKDRIVERINFOLEGUI_H

#include <QGroupBox>

#include "src/hkxclasses/behavior/hkbfootikdriverinfo.h"

class QuadVariableWidget;
class TableWidget;
class hkbFootIkDriverInfoLeg;
class QVBoxLayout;
class HkxObject;
class DoubleSpinBox;
class CheckBox;
class QStackedLayout;
class ComboBox;
class QPushButton;

class FootIkDriverInfoLegUI: public QGroupBox
{
    Q_OBJECT
    //friend class HkDataUI;
public:
    FootIkDriverInfoLegUI();
    virtual ~FootIkDriverInfoLegUI(){}
    void loadData(hkbFootIkDriverInfoLeg *data);
    void loadBoneList(QStringList &bones);
signals:
    void returnToParent();
private slots:
    void setKneeAxisLS();
    void setFootEndLS();
    void setFootPlantedAnkleHeightMS();
    void setFootRaisedAnkleHeightMS();
    void setMaxAnkleHeightMS();
    void setMinAnkleHeightMS();
    void setMaxKneeAngleDegrees();
    void setMinKneeAngleDegrees();
    void setMaxAnkleAngleDegrees();
    void setHipIndex(int index);
    void setKneeIndex(int index);
    void setAnkleIndex(int index);
private:
    static QStringList headerLabels1;
    hkbFootIkDriverInfo::hkbFootIkDriverInfoLeg *bsData;
    QVBoxLayout *lyt;
    QStackedLayout *stackLyt;
    QPushButton *returnPB;
    TableWidget *table;
    QuadVariableWidget *kneeAxisLS;
    QuadVariableWidget *footEndLS;
    DoubleSpinBox *footPlantedAnkleHeightMS;
    DoubleSpinBox *footRaisedAnkleHeightMS;
    DoubleSpinBox *maxAnkleHeightMS;
    DoubleSpinBox *minAnkleHeightMS;
    DoubleSpinBox *maxKneeAngleDegrees;
    DoubleSpinBox *minKneeAngleDegrees;
    DoubleSpinBox *maxAnkleAngleDegrees;
    ComboBox *hipIndex;
    ComboBox *kneeIndex;
    ComboBox *ankleIndex;
};

#endif // FOOTIKDRIVERINFOLEGUI_H
