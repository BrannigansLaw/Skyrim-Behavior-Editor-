#ifndef BONEWEIGHTARRAYUI_H
#define BONEWEIGHTARRAYUI_H

#include <QGroupBox>

class hkbBoneWeightArray;
class QVBoxLayout;
class DoubleSpinBox;
class TableWidget;
class HkxObject;
class QPushButton;
class QSignalMapper;

class BoneWeightArrayUI: public QGroupBox
{
    Q_OBJECT
public:
    BoneWeightArrayUI();
    virtual ~BoneWeightArrayUI();
    void loadData(HkxObject *data, bool isRagdoll = false);
protected:
signals:
    void returnToParent();
private slots:
    void setBoneWeight(int row);
private:
    hkbBoneWeightArray *bsData;
    QVBoxLayout *lyt;
    QPushButton *backPB;
    TableWidget *bones;
    QSignalMapper *mapper;
};

#endif // BONEWEIGHTARRAYUI_H
