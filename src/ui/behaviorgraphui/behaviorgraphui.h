#ifndef BEHAVIORGRAPHUI_H
#define BEHAVIORGRAPHUI_H

#include <QWidget>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QGraphicsItem>

#include "src/hkxclasses/hkxobject.h"

class FileSelectWindow;
class QVBoxLayout;
class QMenuBar;
class QMenu;
class QFile;
class QGraphicsScene;
class QGroupBox;
class QHBoxLayout;
class BehaviorFile;
class hkbManualSelectorGenerator;
class hkbBlenderGenerator;
class hkbBlenderGeneratorChild;
class hkbStateMachine;
class hkbStateMachineStateInfo;
class BSBoneSwitchGenerator;
class BSBoneSwitchGeneratorBoneData;
class hkbGenerator;
class HkDataUI;
class GeneratorIcon;

class BehaviorGraphView: public QGraphicsView
{
    Q_OBJECT
    friend class GeneratorIcon;
    friend class HkDataUI;
    friend class ManualSelectorGeneratorUI;
    friend class ModifierGeneratorUI;
    friend class BSiStateTaggingGeneratorUI;
    friend class StateMachineUI;
    friend class StateUI;
    friend class TransitionsUI;
    friend class MainWindow;
public:
    BehaviorGraphView(HkDataUI *mainUI, BehaviorFile * file);
    virtual ~BehaviorGraphView();
    void writeToLog(const QString & message, bool isError = false);
    bool drawBehaviorGraph();
    bool reconnectBranch(HkxObject *oldChild, HkxObject *newChild, GeneratorIcon *icon);
    QSize sizeHint() const Q_DECL_OVERRIDE{
        return QSize(500, 400);
    }
    GeneratorIcon* getSelectedItem() const{
        return selectedIcon;
    }
    QStringList getEventNames() const;
    void removeGeneratorData();
    void removeModifierData();
    void removeOtherData();
protected:
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
signals:
    void iconSelected(GeneratorIcon *icon);
    void addedGenerator(const QString & name);
    void addedModifier(const QString & name);
    void removedGenerator(int index);
    void removedModifier(int index);
private slots:
    void appendStateMachine();
    void appendManualSelectorGenerator();
    void appendModifierGenerator();
    void appendIStateTaggingGenerator();
    void appendSynchronizedClipGenerator();
    void appendOffsetAnimationGenerator();
    void appendCyclicBlendTransitionGenerator();
    void appendPoseMatchingGenerator();
    void appendBlenderGenerator();
    void appendBoneSwitchGenerator();
    void appendClipGenerator();
    void appendBehaviorReferenceGenerator();
    void wrapStateMachine();
    void wrapBlenderGenerator();
    void wrapPoseMatchingGenerator();

    void wrapManualSelectorGenerator();
    void wrapModifierGenerator();
    void wrapIStateTaggingGenerator();
    void wrapSynchronizedClipGenerator();
    void wrapOffsetAnimationGenerator();
    void wrapCyclicBlendTransitionGenerator();
    void wrapBoneSwitchGenerator();
    void removeSelectedObjectBranchSlot();
private:
    void removeSelectedObjectBranch(GeneratorIcon *icon, GeneratorIcon *iconToKeep = NULL, bool removeAll = true);
    void removeIconFromGraph(GeneratorIcon *iconToRemove);
private:
    HkDataUI *ui;
    BehaviorFile *behavior;
    QGraphicsScene *behaviorGS;
    GeneratorIcon *selectedIcon;
    GeneratorIcon *rootIcon;
    QMenu *contextMenu;
    QMenu *appendGeneratorMenu;
    QAction *appendSMAct;
    QAction *appendMSGAct;
    QAction *appendMGAct;
    QAction *appendBSISTGAct;
    QAction *appendBSSCGAct;
    QAction *appendBSOAGAct;
    QAction *appendBSCBTGAct;
    QAction *appendPMGAct;
    QAction *appendCGAct;
    QAction *appendBRGAct;
    QMenu *appendBlenderMenu;
    QAction *appendBGAct;
    QAction *appendBSBSGAct;
    QMenu *wrapGeneratorMenu;
    QAction *wrapSMAct;
    QAction *wrapMSGAct;
    QAction *wrapMGAct;
    QAction *wrapBSISTGAct;
    QAction *wrapBSSCGAct;
    QAction *wrapBSOAGAct;
    QAction *wrapBSCBTGAct;
    QAction *wrapPMGAct;
    QMenu *wrapBlenderMenu;
    QAction *wrapBGAct;
    QAction *wrapBSBSGAct;
    QAction *removeObjBranchAct;
    const qreal minScale;
    const qreal maxScale;
    const qreal initScale;
    const qreal iconFocusScale;
    qreal currentScale;
    const qreal scaleUpFactor;
    const qreal scaleDownFactor;
private:
    void expandBranchForIcon(GeneratorIcon *icon);
    void removeChildIcons(GeneratorIcon *parent, GeneratorIcon *iconToKeep = NULL);
    void repositionIcons(GeneratorIcon * icon, bool updateNonVisable = false);
    void popUpMenuRequested(const QPoint &pos, GeneratorIcon *icon);
    void expandBranch(GeneratorIcon * icon, bool expandAll = false);
    void contractBranch(GeneratorIcon * icon, bool contractAll = false);
    int manageIcons();
    bool positionIcon(GeneratorIcon * icon);
    void createIconBranch(QList <GeneratorIcon *> & icons, GeneratorIcon *parent);
    void deleteSelectedBranch(GeneratorIcon *icon, GeneratorIcon *iconToKeep = NULL);

    bool appendObject(DataIconManager * ptr, GeneratorIcon *parentObjIcon);
    void wrapObject(DataIconManager *obj, GeneratorIcon *parentObjIcon);
    int initializeIconsForNewBranch(DataIconManager *ptr, QList<HkxObjectExpSharedPtr> & objects, QList<GeneratorIcon *> & parentIcons, QVector <short> & objectChildCount);
    int initializeIcons(DataIconManager *ptr, QList<HkxObjectExpSharedPtr> & objects, QList<GeneratorIcon *> & parentIcons, QVector <short> & objectChildCount);
    GeneratorIcon * addIconToGraph(DataIconManager *obj, GeneratorIcon * parentIcon);
    GeneratorIcon * initalizeInjectedIcon(DataIconManager *obj, GeneratorIcon * parentIcon, GeneratorIcon * selected);
    GeneratorIcon * initalizeAppendedIcon(DataIconManager *obj, GeneratorIcon * parentIcon);
};

#endif // BEHAVIORGRAPHUI_H
