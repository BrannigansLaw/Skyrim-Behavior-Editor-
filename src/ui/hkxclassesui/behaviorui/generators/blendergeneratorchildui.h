#ifndef BLENDERGENERATORCHILDUI_H
#define BLENDERGENERATORCHILDUI_H

#include <QGroupBox>

class QuadVariableWidget;
class TableWidget;
class hkbBlenderGeneratorChild;
class QVBoxLayout;
class HkxObject;
class DoubleSpinBox;
class QCheckBox;
class QStackedLayout;
class ComboBox;
class QPushButton;

class BlenderGeneratorChildUI: public QGroupBox
{
    Q_OBJECT
    //friend class HkDataUI;
public:
    BlenderGeneratorChildUI();
    virtual ~BlenderGeneratorChildUI(){}
    void loadData(hkbFootIkDriverInfoLeg *data);
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
    hkbBlenderGeneratorChild *bsData;
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

#endif // BLENDERGENERATORCHILDUI_H
