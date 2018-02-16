#ifndef HKBEXPRESSIONDATAARRAY_H
#define HKBEXPRESSIONDATAARRAY_H

#include "src/hkxclasses/hkxobject.h"

class hkbExpressionDataArray: public HkxObject
{
    friend class ExpressionDataArrayUI;
    friend class hkbEvaluateExpressionModifier;
    friend class EvaluateExpressionModifierUI;
private:
    enum ExpressionEventMode{
        EVENT_MODE_SEND_ONCE=0,
        EVENT_MODE_SEND_ON_TRUE=1,
        EVENT_MODE_SEND_ON_FALSE_TO_TRUE=2,
        EVENT_MODE_SEND_EVERY_FRAME_ONCE_TRUE=3
    };
public:
    hkbExpressionDataArray(HkxFile *parent, long ref = -1);
    virtual ~hkbExpressionDataArray();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    bool evaulateDataValidity();
    static QString getClassname();
    void addExpression(const QString & exp = "", ExpressionEventMode mode = EVENT_MODE_SEND_ONCE);
    void removeExpression(const QString & exp);
    void removeExpression(int index);
    bool write(HkxXMLWriter *writer);
    bool isEventReferenced(int eventindex) const;
    void updateEventIndices(int eventindex);
protected:
private:
    hkbExpressionDataArray& operator=(const hkbExpressionDataArray&);
    hkbExpressionDataArray(const hkbExpressionDataArray &);
private:
    struct hkExpression
    {
        hkExpression(const QString & exp = "", ExpressionEventMode mode = EVENT_MODE_SEND_ONCE)
            : expression(exp),
              assignmentVariableIndex(-1),
              assignmentEventIndex(-1),
              eventMode(EventMode.at(mode))
        {
            //
        }

        QString expression;
        int assignmentVariableIndex;
        int assignmentEventIndex;
        QString eventMode;
    };

    static QStringList EventMode;   //ExpressionEventMode (EVENT_MODE_SEND_ONCE=0;EVENT_MODE_SEND_ON_TRUE=1;EVENT_MODE_SEND_ON_FALSE_TO_TRUE=2;EVENT_MODE_SEND_EVERY_FRAME_ONCE_TRUE=3)
    static uint refCount;
    static QString classname;
    QList <hkExpression> expressionsData;
};

#endif // HKBEXPRESSIONDATAARRAY_H
