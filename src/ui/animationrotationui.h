#ifndef ANIMATIONROTATIONUI_H
#define ANIMATIONROTATIONUI_H

#include <QGroupBox>

#include "src/animData/skyrimanimdata.h"

class QGridLayout;
class QPushButton;
class DoubleSpinBox;
class TableWidget;

class AnimationRotationUI final: public QGroupBox
{
    Q_OBJECT
public:
    AnimationRotationUI();
    void loadData(SkyrimAnimationRotation *quaternion, qreal maxtime);
signals:
    void returnToParent();
private slots:
    void setLocalTime();
    void setX(qreal xval);
    void setY(qreal yval);
    void setZ(qreal zval);
    void setAngle(qreal wval);
private:
    void toggleSignals(bool toggleconnections);
    SkyrimAnimationRotation convertQuaternionAxisAngle(SkyrimAnimationRotation * quaternion) const;
private:
    static const QStringList headerLabels;
    SkyrimAnimationRotation *bsData;
    QGridLayout *topLyt;
    QPushButton *returnPB;
    TableWidget *table;
    DoubleSpinBox *localTime;
    DoubleSpinBox *x;
    DoubleSpinBox *y;
    DoubleSpinBox *z;
    DoubleSpinBox *angle;
};

#endif // ANIMATIONROTATIONUI_H
