#include "hksimplelocalframe.h"
#include "src/xml/hkxxmlreader.h"
#include "src/filetypes/skeletonfile.h"

/**
 * hkSimpleLocalFrame
 */

uint hkSimpleLocalFrame::refCount = 0;

QString hkSimpleLocalFrame::classname = "hkSimpleLocalFrame";

hkSimpleLocalFrame::hkSimpleLocalFrame(HkxFile *parent, const QString &string, long ref)
    : HkxObject(parent, ref),
      name(string)
{
    setType(HK_SIMPLE_LOCAL_FRAME, TYPE_OTHER);
    getParentFile()->addObjectToFile(this, ref);
    refCount++;
}

QString hkSimpleLocalFrame::getClassname(){
    return classname;
}

QString hkSimpleLocalFrame::getName() const{
    return name;
}

bool hkSimpleLocalFrame::readData(const HkxXmlReader & reader, long index){
    QByteArray ref = reader.getNthAttributeValueAt(index - 1, 0);
    QByteArray text;
    while (index < reader.getNumElements() && reader.getNthAttributeNameAt(index, 1) != "class"){
        text = reader.getNthAttributeValueAt(index, 0);
        if (text == "name"){
            name = reader.getElementValueAt(index);
            if (name == ""){
                WRITE_TO_LOG(getClassname()+": readData()!\nFailed to properly read 'name' data field!\nObject Reference: "+ref);
            }
        }
        index++;
    }
    return true;
}

bool hkSimpleLocalFrame::write(HkxXMLWriter *writer){
    if (!writer){
        return false;
    }
    if (!getIsWritten()){
        QStringList list1 = {writer->name, writer->clas, writer->signature};
        QStringList list2 = {getReferenceString(), getClassname(), "0x"+QString::number(getSignature(), 16)};
        writer->writeLine(writer->object, list1, list2, "");
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("transform"), "(0.000000 0.000000 0.000000)(0.000000 0.000000 0.000000)(0.000000 0.000000 0.000000)(0.000000 0.000000 0.000000)");
        list1 = {writer->name, writer->numelements};
        list2 = {"children", "0"};
        writer->writeLine(writer->parameter, list1, list2, " ");
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("parentFrame"), "null");
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("group"), "null");
        writer->writeLine(writer->parameter, QStringList(writer->name), QStringList("name"), name);
        writer->writeLine(writer->object, false);
        setIsWritten();
        writer->writeLine("\n");
        /*for (int i = 0; i < children.size(); i++){
            if (children.at(i).data()){
                children.at(i).data()->write(writer);
            }
        }
        if (parentFrame.data()){
            parentFrame.data()->write(writer);
        }
        if (group.data()){
            group.data()->write(writer);
        }*/
    }
    return true;
}

bool hkSimpleLocalFrame::evaluateDataValidity(){
    if (name == ""){
        setDataValidity(false);
        return false;
    }
    setDataValidity(true);
    return true;
}

hkSimpleLocalFrame::~hkSimpleLocalFrame(){
    refCount--;
}
