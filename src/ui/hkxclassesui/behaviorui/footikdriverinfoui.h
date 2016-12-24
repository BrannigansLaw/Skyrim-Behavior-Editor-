#ifndef FOOTIKDRIVERINFOUI_H
#define FOOTIKDRIVERINFOUI_H

#include <QGroupBox>

class QuadVariableWidget;
class TableWidget;
class hkbFootIkDriverInfo;
class QVBoxLayout;
class HkxObject;
class DoubleSpinBox;
class QCheckBox;
class QStackedLayout;
class SpinBox;

class FootIkDriverInfoUI: public QGroupBox
{
    Q_OBJECT
    //friend class HkDataUI;
public:
    FootIkDriverInfoUI();
    virtual ~FootIkDriverInfoUI(){}
    void loadData(HkxObject *data);
signals:
private slots:
    void setRaycastDistanceUp();
    void setRaycastDistanceDown();
    void setOriginalGroundHeightMS();
    void setVerticalOffset();
    void setCollisionFilterInfo();
    void setForwardAlignFraction();
    void setSidewaysAlignFraction();
    void setSidewaysSampleWidth();
    void setLockFeetWhenPlanted();
    void setUseCharacterUpVector();
    void setIsQuadrupedNarrow();
private:
    static QStringList headerLabels1;
    hkbFootIkDriverInfo *bsData;
    QVBoxLayout *lyt;
    QStackedLayout *stackLyt;
    TableWidget *table;
    DoubleSpinBox *raycastDistanceUp;
    DoubleSpinBox *raycastDistanceDown;
    DoubleSpinBox *originalGroundHeightMS;
    DoubleSpinBox *verticalOffset;
    SpinBox *collisionFilterInfo;
    DoubleSpinBox *forwardAlignFraction;
    DoubleSpinBox *sidewaysAlignFraction;
    DoubleSpinBox *sidewaysSampleWidth;
    QCheckBox *lockFeetWhenPlanted;
    QCheckBox *useCharacterUpVector;
    QCheckBox *isQuadrupedNarrow;
};

#endif // FOOTIKDRIVERINFOUI_H
