#ifndef HKBATTACHMENTMODIFIER_H
#define HKBATTACHMENTMODIFIER_H

#include "hkbmodifier.h"

class hkbAttachmentModifier: public hkbModifier
{
    friend class BehaviorGraphView;
public:
    hkbAttachmentModifier(BehaviorFile *parent, long ref = 0);
    virtual ~hkbAttachmentModifier();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    void unlink();
    QString getName() const;
    bool evaulateDataValidity();
    static QString getClassname();
    bool write(HkxXMLWriter *writer);
private:
    hkbAttachmentModifier& operator=(const hkbAttachmentModifier&);
    hkbAttachmentModifier(const hkbAttachmentModifier &);
private:
    struct hkEvent{
        hkEvent(): id(-1){}
        int id;
        HkxObjectExpSharedPtr payload;
    };

    static uint refCount;
    static QString classname;
    long userData;
    QString name;
    bool enable;
    hkEvent sendToAttacherOnAttach;
    hkEvent sendToAttacheeOnAttach;
    hkEvent sendToAttacherOnDetach;
    hkEvent sendToAttacheeOnDetach;
    HkxObjectExpSharedPtr attachmentSetup;
    HkxObjectExpSharedPtr attacherHandle;
    HkxObjectExpSharedPtr attacheeHandle;
    int attacheeLayer;
};

#endif // HKBATTACHMENTMODIFIER_H
