#ifndef BLENDINGTRANSITIONEFFECTUI_H
#define BLENDINGTRANSITIONEFFECTUI_H

#include <QGroupBox>

class CustomTreeGraphicsView;
class TableWidget;
class QLineEdit;
class ComboBox;
class hkbBlendingTransitionEffect;
class QVBoxLayout;
class HkxObject;
class SpinBox;
class DoubleSpinBox;
class QCheckBox;

class BlendingTransitionEffectUI: public QGroupBox
{
    Q_OBJECT
    friend class HkDataUI;
    friend class TransitionsUI;
public:
    BlendingTransitionEffectUI();
    virtual ~BlendingTransitionEffectUI(){}
    void loadData(HkxObject *data){}
private slots:
    void setName(){}
private:
    void addVariableToLists(const QString & name){}
    void removeVariableFromLists(int index){}
    void renameVariableInLists(const QString & newName, int index){}
    void loadComboBoxes(){}
private:
    static QStringList headerLabels1;
    CustomTreeGraphicsView *behaviorView;
    hkbBlendingTransitionEffect *bsData;
    QVBoxLayout *lyt;
    TableWidget *table;
    QLineEdit *name;
    ComboBox *selfTransitionMode;
    ComboBox *eventMode;
    DoubleSpinBox *duration;
    DoubleSpinBox *toGeneratorStartTimeFraction;
    QCheckBox *flagSync;
    QCheckBox *flagIgnoreFromWorldFromModel;
    ComboBox *endMode;
    ComboBox *blendCurve;
};

#endif // BLENDINGTRANSITIONEFFECTUI_H
