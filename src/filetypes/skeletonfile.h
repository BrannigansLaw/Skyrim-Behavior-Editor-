#ifndef SKELETONFILE_H
#define SKELETONFILE_H

#include "src/filetypes/hkxfile.h"

class SkeletonFile: public HkxFile
{
    friend class MainWindow;
    friend class ProjectUI;
    friend class hkaSkeleton;
    friend class hkaAnimationContainer;
    friend class hkRootLevelContainer;
public:
    SkeletonFile(MainWindow *window, const QString & name);
    virtual ~SkeletonFile();
    bool addObjectToFile(HkxObject *obj, long ref = -1);
    void write();
    QString getRootObjectReferenceString();
    QStringList getBonesFromSkeletonAt(int index) const;
    int getNumberOfBones(bool ragdoll = false) const;
protected:
    bool parse();
    bool link();
private:
    HkxSharedPtr * findSkeleton(long ref);
    HkxSharedPtr * findLocalFrame(long ref);
private:
    QList <HkxSharedPtr> skeletons;
    QList <HkxSharedPtr> localFrames;
    long largestRef;
};

#endif // SKELETONFILE_H
