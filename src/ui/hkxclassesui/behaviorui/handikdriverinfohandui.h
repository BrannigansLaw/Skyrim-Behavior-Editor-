#ifndef HANDIKDRIVERINFOHANDUI_H
#define HANDIKDRIVERINFOHANDUI_H

#include <QGroupBox>

#include "src/hkxclasses/behavior/hkbhandikdriverinfo.h"

class QuadVariableWidget;
class TableWidget;
class hkbHandIkDriverInfoHand;
class QVBoxLayout;
class HkxObject;
class DoubleSpinBox;
class CheckBox;
class QStackedLayout;
class ComboBox;
class QPushButton;
class LineEdit;

class HandIkDriverInfoHandUI: public QGroupBox
{
    Q_OBJECT
    //friend class HkDataUI;
public:
    HandIkDriverInfoHandUI();
    virtual ~HandIkDriverInfoHandUI(){}
    void loadData(hkbHandIkDriverInfoHand *data, hkbHandIkDriverInfo *par);
    void loadBoneList(QStringList &bones);
signals:
    void returnToParent();
private slots:
    void setElbowAxisLS();
    void setBackHandNormalLS();
    void setHandOffsetLS();
    void setHandOrienationOffsetLS();
    void setMaxElbowAngleDegrees();
    void setMinElbowAngleDegrees();
    void setShoulderIndex(int index);
    void setShoulderSiblingIndex(int index);
    void setElbowIndex(int index);
    void setElbowSiblingIndex(int index);
    void setWristIndex(int index);
    void setEnforceEndPosition();
    void setEnforceEndRotation();
    void setLocalFrameName();
private:
    static QStringList headerLabels1;
    hkbHandIkDriverInfo::hkbHandIkDriverInfoHand *bsData;
    hkbHandIkDriverInfo *parent;
    QVBoxLayout *lyt;
    QStackedLayout *stackLyt;
    QPushButton *returnPB;
    TableWidget *table;
    QuadVariableWidget *elbowAxisLS;
    QuadVariableWidget *backHandNormalLS;
    QuadVariableWidget *handOffsetLS;
    QuadVariableWidget *handOrienationOffsetLS;
    DoubleSpinBox *maxElbowAngleDegrees;
    DoubleSpinBox *minElbowAngleDegrees;
    ComboBox *shoulderIndex;
    ComboBox *shoulderSiblingIndex;
    ComboBox *elbowIndex;
    ComboBox *elbowSiblingIndex;
    ComboBox *wristIndex;
    CheckBox *enforceEndPosition;
    CheckBox *enforceEndRotation;
    LineEdit *localFrameName;
};

#endif // HANDIKDRIVERINFOHANDUI_H
