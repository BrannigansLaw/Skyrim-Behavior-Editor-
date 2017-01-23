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
protected:
    bool parse();
    bool link();
private:
    HkxObjectExpSharedPtr * findSkeleton(long ref);
    HkxObjectExpSharedPtr * findLocalFrame(long ref);
private:
    QList <HkxObjectExpSharedPtr> skeletons;
    QList <HkxObjectExpSharedPtr> localFrames;
    long largestRef;
};

#endif // SKELETONFILE_H
