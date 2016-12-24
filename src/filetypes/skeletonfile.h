#ifndef SKELETONFILE_H
#define SKELETONFILE_H

#include "src/filetypes/hkxfile.h"

class SkeletonFile: public HkxFile
{
    friend class MainWindow;
    friend class ProjectUI;
public:
    SkeletonFile(MainWindow *window, const QString & name);
    virtual ~SkeletonFile();
    bool addObjectToFile(HkxObject *obj, long ref = -1);
    void write();
    QString getRootObjectReferenceString();
    HkxObjectExpSharedPtr * findSkeleton(long ref);
    HkxObjectExpSharedPtr * findLocalFrame(long ref);
    QStringList getBonesFromSkeletonAt(int index) const;
protected:
    bool parse();
    bool link();
private:
    QList <HkxObjectExpSharedPtr> skeletons;
    QList <HkxObjectExpSharedPtr> localFrames;
    long largestRef;
};

#endif // SKELETONFILE_H
