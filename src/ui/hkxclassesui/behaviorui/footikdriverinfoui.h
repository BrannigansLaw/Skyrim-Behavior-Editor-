#ifndef FOOTIKDRIVERINFOUI_H
#define FOOTIKDRIVERINFOUI_H

#include <QStackedWidget>

class QuadVariableWidget;
class TableWidget;
class hkbFootIkDriverInfo;
class QGridLayout;
class HkxObject;
class DoubleSpinBox;
class CheckBox;
class QStackedLayout;
class SpinBox;
class QPushButton;
class QGroupBox;
class FootIkDriverInfoLegUI;

class FootIkDriverInfoUI: public QStackedWidget
{
    Q_OBJECT
    //friend class HkDataUI;
public:
    FootIkDriverInfoUI();
    virtual ~FootIkDriverInfoUI(){}
    void loadData(HkxObject *data);
    void loadBoneList(QStringList &bones);
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
    void addLeg();
    void removeSelectedLeg();
    void viewSelectedLeg(int row, int column);
    void returnToWidget();
private:
    enum ACTIVE_WIDGET {
        FOOT_IK_DRIVER_INFO = 0,
        FOOT_IK_DRIVER_INFO_LEG = 1
    };

    static QStringList headerLabels1;
    hkbFootIkDriverInfo *bsData;
    QGroupBox *footDriverGB;
    FootIkDriverInfoLegUI *legUI;
    QGridLayout *footDriverLyt;
    QPushButton *addLegPB;
    QPushButton *removeLegPB;
    //QStackedLayout *stackLyt;
    TableWidget *table;
    DoubleSpinBox *raycastDistanceUp;
    DoubleSpinBox *raycastDistanceDown;
    DoubleSpinBox *originalGroundHeightMS;
    DoubleSpinBox *verticalOffset;
    SpinBox *collisionFilterInfo;
    DoubleSpinBox *forwardAlignFraction;
    DoubleSpinBox *sidewaysAlignFraction;
    DoubleSpinBox *sidewaysSampleWidth;
    CheckBox *lockFeetWhenPlanted;
    CheckBox *useCharacterUpVector;
    CheckBox *isQuadrupedNarrow;
};

#endif // FOOTIKDRIVERINFOUI_H
