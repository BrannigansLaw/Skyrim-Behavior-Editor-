#include "animationrotationui.h"

#include "src/utility.h"
#include "src/ui/genericdatawidgets.h"

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QQuaternion>

#define BASE_NUMBER_OF_ROWS 5

#define LOCAL_TIME_ROW 0
#define X_ROW 1
#define Y_ROW 2
#define Z_ROW 3
#define ANGLE_ROW 4

#define NAME_COLUMN 0
#define TYPE_COLUMN 1
#define VALUE_COLUMN 2

#define BINDING_ITEM_LABEL QString("Use Property     ")

QStringList AnimationRotationUI::headerLabels = {
    "Name",
    "Type",
    "Value"
};

AnimationRotationUI::AnimationRotationUI()
    : bsData(nullptr),
      topLyt(new QGridLayout),
      returnPB(new QPushButton("Return")),
      table(new TableWidget),
      localTime(new DoubleSpinBox),
      x(new DoubleSpinBox),
      y(new DoubleSpinBox),
      z(new DoubleSpinBox),
      angle(new DoubleSpinBox)
{
    localTime->setMinimum(0);
    setTitle("Animation Rotation");
    table->setRowCount(BASE_NUMBER_OF_ROWS);
    table->setColumnCount(headerLabels.size());
    table->setHorizontalHeaderLabels(headerLabels);
    table->setItem(LOCAL_TIME_ROW, NAME_COLUMN, new TableWidgetItem("localTime"));
    table->setItem(LOCAL_TIME_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setCellWidget(LOCAL_TIME_ROW, VALUE_COLUMN, localTime);
    table->setItem(X_ROW, NAME_COLUMN, new TableWidgetItem("x"));
    table->setItem(X_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setCellWidget(X_ROW, VALUE_COLUMN, x);
    table->setItem(Y_ROW, NAME_COLUMN, new TableWidgetItem("y"));
    table->setItem(Y_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setCellWidget(Y_ROW, VALUE_COLUMN, y);
    table->setItem(Z_ROW, NAME_COLUMN, new TableWidgetItem("z"));
    table->setItem(Z_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setCellWidget(Z_ROW, VALUE_COLUMN, z);
    table->setItem(ANGLE_ROW, NAME_COLUMN, new TableWidgetItem("angle"));
    table->setItem(ANGLE_ROW, TYPE_COLUMN, new TableWidgetItem("hkReal", Qt::AlignCenter));
    table->setCellWidget(ANGLE_ROW, VALUE_COLUMN, angle);
    topLyt->addWidget(returnPB, 0, 1, 1, 1);
    topLyt->addWidget(table, 1, 0, 6, 3);
    setLayout(topLyt);
    connectSignals();
}

void AnimationRotationUI::connectSignals(){
    connect(returnPB, SIGNAL(released()), this, SIGNAL(returnToParent()), Qt::UniqueConnection);
    connect(localTime, SIGNAL(editingFinished()), this, SLOT(setLocalTime()), Qt::UniqueConnection);
    connect(x, SIGNAL(valueChanged(double)), this, SLOT(setX(double)), Qt::UniqueConnection);
    connect(y, SIGNAL(valueChanged(double)), this, SLOT(setY(double)), Qt::UniqueConnection);
    connect(z, SIGNAL(valueChanged(double)), this, SLOT(setZ(double)), Qt::UniqueConnection);
    connect(angle, SIGNAL(valueChanged(double)), this, SLOT(setAngle(double)), Qt::UniqueConnection);
}

void AnimationRotationUI::disconnectSignals(){
    disconnect(returnPB, SIGNAL(released()), this, SIGNAL(returnToParent()));
    disconnect(localTime, SIGNAL(editingFinished()), this, SLOT(setLocalTime()));
    disconnect(x, SIGNAL(valueChanged(double)), this, SLOT(setX(double)));
    disconnect(y, SIGNAL(valueChanged(double)), this, SLOT(setY(double)));
    disconnect(z, SIGNAL(valueChanged(double)), this, SLOT(setZ(double)));
    disconnect(angle, SIGNAL(valueChanged(double)), this, SLOT(setAngle(double)));
}

SkyrimAnimationRotation AnimationRotationUI::convertQuaternionAxisAngle(SkyrimAnimationRotation *quaternion) const{
    SkyrimAnimationRotation axisAngleRot;
    QQuaternion quat(quaternion->w, quaternion->x, quaternion->y, quaternion->z);
    /*qreal angle = acos(quaternion->w)/2;
    axisAngleRot.w = angle;
    axisAngleRot.x = quaternion->x/sin(angle/2);
    axisAngleRot.y = quaternion->y/sin(angle/2);
    axisAngleRot.z = quaternion->z/sin(angle/2);*/
    quat.getAxisAndAngle(&axisAngleRot.x, &axisAngleRot.y, &axisAngleRot.z, &axisAngleRot.w);
    return axisAngleRot;
}

/*SkyrimAnimData::Rotation AnimationRotationUI::convertAxisAngletoQuaternion(SkyrimAnimData::Rotation * axisAngleRot) const{
    SkyrimAnimData::Rotation rot;
    rot.w = cos(axisAngleRot->w/2);
    rot.x = axisAngleRot->x*sin(axisAngleRot->w/2);
    rot.y = axisAngleRot->y*sin(axisAngleRot->w/2);
    rot.z = axisAngleRot->z*sin(axisAngleRot->w/2);
    return rot;
}*/

void AnimationRotationUI::loadData(SkyrimAnimationRotation *quaternion, qreal maxtime){
    disconnectSignals();
    if (quaternion){
        localTime->setMaximum(maxtime);
        bsData = quaternion;
        localTime->setValue(bsData->localTime);
        SkyrimAnimationRotation axisAngleRot = convertQuaternionAxisAngle(bsData);
        x->setValue(axisAngleRot.x);
        y->setValue(axisAngleRot.y);
        z->setValue(axisAngleRot.z);
        angle->setValue(axisAngleRot.w);
    }else{
        CRITICAL_ERROR_MESSAGE("AnimationRotationUI::loadData(): Data is null!!!");
    }
    connectSignals();
}

void AnimationRotationUI::setLocalTime(){
    if (bsData){
        if (bsData->localTime != localTime->value()){
            bsData->localTime = localTime->value();
            //file->toggleChanged(true);
        }
    }else{
        CRITICAL_ERROR_MESSAGE("AnimationRotationUI::setlocalTime(): Data is null!!!");
    }
}

void AnimationRotationUI::setX(qreal xval){
    if (bsData){
        bsData->x = xval*sin(angle->value()/2);
        //file->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("AnimationRotationUI::setx(): Data is null!!!");
    }
}

void AnimationRotationUI::setY(qreal yval){
    if (bsData){
        bsData->y = yval*sin(angle->value()/2);
        //file->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("AnimationRotationUI::sety(): Data is null!!!");
    }
}

void AnimationRotationUI::setZ(qreal zval){
    if (bsData){
        bsData->z = zval*sin(angle->value()/2);
        //file->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("AnimationRotationUI::setz(): Data is null!!!");
    }
}

void AnimationRotationUI::setAngle(qreal wval){
    if (bsData){
        bsData->w = cos(wval/2);
        //file->toggleChanged(true);
    }else{
        CRITICAL_ERROR_MESSAGE("AnimationRotationUI::setz(): Data is null!!!");
    }
}
