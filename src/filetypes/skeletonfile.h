#ifndef SKELETONFILE_H
#define SKELETONFILE_H

#include "src/filetypes/hkxfile.h"

class hkaSkeleton;

class SkeletonFile final: public HkxFile
{
    friend class hkaSkeleton;
public:
    SkeletonFile(MainWindow *window, const QString & name);
    SkeletonFile& operator=(const SkeletonFile&) = delete;
    SkeletonFile(const SkeletonFile &) = delete;
    ~SkeletonFile() = default;
    bool addObjectToFile(HkxObject *obj, long ref = -1);
    void write();
    QString getRootObjectReferenceString();
    QStringList getBonesFromSkeletonAt(int index) const;
    int getNumberOfBones(bool ragdoll = false) const;
    QStringList getLocalFrameNames() const;
    hkaSkeleton *getSkeleton(bool isragdoll) const;
    bool parse();
    HkxSharedPtr * findSkeleton(long ref);
protected:
    bool link();
private:
    HkxSharedPtr * findLocalFrame(long ref);
private:
    QVector <HkxSharedPtr> skeletons;
    QVector <HkxSharedPtr> localFrames;
    long largestRef;
    mutable std::mutex mutex;
};

#endif // SKELETONFILE_H
