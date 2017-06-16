#ifndef SKELETONUI_H
#define SKELETONUI_H

#include <QGroupBox>

class hkaSkeleton;
class QVBoxLayout;
class DoubleSpinBox;
class TableWidget;
class HkxObject;
class QPushButton;
class QSignalMapper;

class SkeletonUI: public QGroupBox
{
    Q_OBJECT
public:
    SkeletonUI(const QString & title);
    virtual ~SkeletonUI();
    void loadData(HkxObject *data, bool isRagdoll = false);
protected:
signals:
    void returnToParent();
private slots:
    void setLocalFrame(int row);
private:
    hkaSkeleton *bsData;
    QVBoxLayout *lyt;
    //QPushButton *backPB;
    TableWidget *bones;
    QSignalMapper *mapper;
};

#endif // SKELETONUI_H
