#ifndef CHARACTERPROPERTIESUI_H
#define CHARACTERPROPERTIESUI_H

#include <QGroupBox>

class HkDataUI;
class QVBoxLayout;
class QHBoxLayout;
class QSignalMapper;
class TableWidget;
class QPushButton;
class hkbCharacterData;
class HkxObject;
class QCheckBox;
class ComboBox;
class QLineEdit;
class QStackedLayout;
class SpinBox;
class DoubleSpinBox;
class QuadVariableWidget;
class BoneWeightArrayUI;

class CharacterPropertiesUI: public QGroupBox
{
    Q_OBJECT
public:
    CharacterPropertiesUI(const QString & title);
    virtual ~CharacterPropertiesUI(){}
    void setHkDataUI(HkDataUI *ui);
    void loadData(HkxObject *data);
    //QSize sizeHint() const Q_DECL_OVERRIDE;
private slots:
    void addVariable();
    void removeVariable();
    void setVariableValue(int type);
    void renameSelectedVariable(int type);
    void viewVariable(int row, int column);
    void returnToTable();
signals:
    void variableNameChanged(const QString & newName, int index);
    void variableAdded(const QString & name);
    void variableRemoved(int index);
private:
    CharacterPropertiesUI& operator=(const CharacterPropertiesUI&);
    CharacterPropertiesUI(const CharacterPropertiesUI &);
    void loadVariable(QCheckBox *variableWid);
    void loadVariable(SpinBox *variableWid);
    void loadVariable(DoubleSpinBox *variableWid);
    void loadVariable(QuadVariableWidget *variableWid);
    void addVariableToTable(const QString & name, const QString & type);
    void removeVariableFromTable(int row);
    void hideOtherVariables(int indexToView);
private:
    enum View {
        TABLE_WIDGET = 0,
        VARIABLE_WIDGET = 1,
        BONE_WEIGHTS_WIDGET = 2
    };
    enum hkTypes {
        VARIABLE_TYPE_BOOL = 0,
        VARIABLE_TYPE_INT32 = 1,
        VARIABLE_TYPE_REAL = 2,
        VARIABLE_TYPE_POINTER = 3,
        VARIABLE_TYPE_VECTOR4 = 4,
        VARIABLE_TYPE_QUATERNION = 5
    };
    static QStringList types;
    static QStringList headerLabels;
    HkDataUI *dataUI;
    QVBoxLayout *verLyt;
    hkbCharacterData *loadedData;
    TableWidget *table;
    QPushButton *addObjectPB;
    QPushButton *removeObjectPB;
    QHBoxLayout *buttonLyt;
    QLineEdit *boolName;
    QLineEdit *intName;
    QLineEdit *doubleName;
    QLineEdit *quadName;
    QCheckBox *boolCB;
    SpinBox *intSB;
    DoubleSpinBox *doubleSB;
    QuadVariableWidget *quadWidget;
    TableWidget *variableWidget;
    BoneWeightArrayUI *boneWeightArrayWidget;
    QStackedLayout *stackLyt;
    QPushButton *returnBoolPB;
    QPushButton *returnIntPB;
    QPushButton *returnDoublePB;
    QPushButton *returnQuadPB;
    ComboBox *typeSelector;
    QSignalMapper *nameMapper;
    QSignalMapper *valueMapper;
};

#endif // CHARACTERPROPERTIESUI_H
