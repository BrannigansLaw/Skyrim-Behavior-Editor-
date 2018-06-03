#ifndef HKBATTACHMENTMODIFIER_H
#define HKBATTACHMENTMODIFIER_H

#include "hkbmodifier.h"

class hkbAttachmentModifier: public hkbModifier
{
    friend class BehaviorGraphView;
public:
    hkbAttachmentModifier(HkxFile *parent, long ref = 0);
    virtual ~hkbAttachmentModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaluateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
    bool isEventReferenced(int eventindex) const;
    void updateEventIndices(int eventindex);
    void mergeEventIndex(int oldindex, int newindex);
    void fixMergedEventIndices(BehaviorFile *dominantfile);
    void updateReferences(long &ref);
private:
    /*QList <DataIconManager *> getChildren() const;
    int getIndexOfObj(DataIconManager *obj) const;
    bool insertObjectAt(int index, DataIconManager *obj);
    bool removeObjectAt(int index);*/
    hkbAttachmentModifier& operator=(const hkbAttachmentModifier&);
    hkbAttachmentModifier(const hkbAttachmentModifier &);
private:
    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    hkEventPayload sendToAttacherOnAttach;
    hkEventPayload sendToAttacheeOnAttach;
    hkEventPayload sendToAttacherOnDetach;
    hkEventPayload sendToAttacheeOnDetach;
    HkxSharedPtr attachmentSetup;
    HkxSharedPtr attacherHandle;
    HkxSharedPtr attacheeHandle;
    int attacheeLayer;
};

#endif // HKBATTACHMENTMODIFIER_H
