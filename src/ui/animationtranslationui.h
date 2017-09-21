#ifndef ANIMATIONTRANSLATIONUI_H
#define ANIMATIONTRANSLATIONUI_H

#include <QGroupBox>

#include "src/animData/skyrimanimdata.h"

class QGridLayout;
class QPushButton;
class DoubleSpinBox;
class TableWidget;

class AnimationTranslationUI: public QGroupBox
{
    Q_OBJECT
    //friend class AnimationUI;
public:
    AnimationTranslationUI();
    void loadData(SkyrimAnimationTranslation *trans, qreal maxtime);
signals:
    void returnToParent();
private slots:
    void setLocalTime();
    void setX();
    void setY();
    void setZ();
private:
    void connectSignals();
    void disconnectSignals();
private:
    static QStringList headerLabels;
    SkyrimAnimationTranslation *bsData;
    QGridLayout *topLyt;
    QPushButton *returnPB;
    TableWidget *table;
    DoubleSpinBox *localTime;
    DoubleSpinBox *x;
    DoubleSpinBox *y;
    DoubleSpinBox *z;
};

#endif // ANIMATIONTRANSLATIONUI_H
