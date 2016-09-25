#ifndef HKBVARIABLEBINDINGSET_H
#define HKBVARIABLEBINDINGSET_H

#include "hkxobject.h"

class hkbVariableBindingSet: public HkxObject
{
    friend class StateMachineUI;
public:
    struct hkBinding
    {
        enum BindingType {
            BINDING_TYPE_VARIABLE=0,
            BINDING_TYPE_CHARACTER_PROPERTY=1
        };
        hkBinding(const QString & path = "", int varIndex = -1, int bit = -1, BindingType type = BINDING_TYPE_VARIABLE)
            : memberPath(path), variableIndex(varIndex), bitIndex(bit), bindingType(type){}
        QString memberPath;
        int variableIndex;
        int bitIndex;
        BindingType bindingType;
    };
public:
    hkbVariableBindingSet(BehaviorFile *parent);
    virtual ~hkbVariableBindingSet();
    bool readData(const HkxXmlReader & reader, long index);
    bool link();
    bool evaulateDataValidity();
    static QString getClassname();
    void addBinding(const QString & path, int varIndex, hkBinding::BindingType type = hkBinding::BINDING_TYPE_VARIABLE);
    void removeBinding(const QString & path);
    void removeBinding(int varIndex);
    int getVariableIndexOfBinding(const QString & path) const;
protected:
private:
    hkbVariableBindingSet& operator=(const hkbVariableBindingSet&);
    hkbVariableBindingSet(const hkbVariableBindingSet &);
private:
    static uint refCount;
    static QString classname;
    QList <hkBinding> bindings;
    int indexOfBindingToEnable;
};

#endif // HKBVARIABLEBINDINGSET_H
