#ifndef BONEWEIGHTARRAYUI_H
#define BONEWEIGHTARRAYUI_H

#include <QGroupBox>

class hkbBoneWeightArray;
class QVBoxLayout;
class DoubleSpinBox;
class TableWidget;
class HkxObject;
class QPushButton;
class QGridLayout;
class QLabel;

class BoneWeightArrayUI: public QGroupBox
{
    Q_OBJECT
public:
    BoneWeightArrayUI();
    virtual ~BoneWeightArrayUI(){}
    void loadData(HkxObject *data, bool isRagdoll = false);
signals:
    void returnToParent();
private slots:
    void setBoneWeight();
    void loadBoneWeight(int row, int);
private:
    static QStringList headerLabels;
    hkbBoneWeightArray *bsData;
    QGridLayout *topLyt;
    QPushButton *returnPB;
    TableWidget *bones;
    QLabel *label;
    DoubleSpinBox *selectedBone;
};

#endif // BONEWEIGHTARRAYUI_H
