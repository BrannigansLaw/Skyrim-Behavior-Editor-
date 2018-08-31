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

class SkeletonUI final: public QGroupBox
{
    Q_OBJECT
public:
    SkeletonUI(const QString & title);
    SkeletonUI& operator=(const SkeletonUI&) = delete;
    SkeletonUI(const SkeletonUI &) = delete;
    ~SkeletonUI() = default;
public:
    void loadData(HkxObject *data, bool isRagdoll = false);
signals:
    void returnToParent();
private:
    hkaSkeleton *bsData;
    QVBoxLayout *lyt;
    TableWidget *bones;
};

#endif // SKELETONUI_H
