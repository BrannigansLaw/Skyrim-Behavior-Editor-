#ifndef HANDIKDRIVERINFOUI_H
#define HANDIKDRIVERINFOUI_H

#include <QStackedWidget>

class QuadVariableWidget;
class TableWidget;
class hkbHandIkDriverInfo;
class QGridLayout;
class HkxObject;
class DoubleSpinBox;
class QCheckBox;
class QStackedLayout;
class SpinBox;
class QPushButton;
class QGroupBox;
class HandIkDriverInfoHandUI;
class ComboBox;

class HandIkDriverInfoUI: public QStackedWidget
{
    Q_OBJECT
    //friend class HkDataUI;
public:
    HandIkDriverInfoUI();
    virtual ~HandIkDriverInfoUI(){}
    void loadData(HkxObject *data);
    void loadBoneList(QStringList &bones);
signals:
private slots:
    void setFadeInOutCurve(int index);
    void addHand();
    void removeSelectedHand();
    void viewSelectedHand(int row, int column);
    void returnToWidget();
private:
    enum ACTIVE_WIDGET {
        HAND_IK_DRIVER_INFO = 0,
        HAND_IK_DRIVER_INFO_HAND = 1
    };

    static QStringList headerLabels1;
    hkbHandIkDriverInfo *bsData;
    QGroupBox *handDriverGB;
    HandIkDriverInfoHandUI *handUI;
    QGridLayout *footDriverLyt;
    QPushButton *addHandPB;
    QPushButton *removeHandPB;
    //QStackedLayout *stackLyt;
    TableWidget *table;
    ComboBox *fadeInOutCurve;
};

#endif // HANDIKDRIVERINFOUI_H
