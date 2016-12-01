#-------------------------------------------------
#
# Project created by QtCreator 2016-02-09T01:56:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Skyrim_Behavior_Tool
TEMPLATE = app

# remove possible other optimization flags
#QMAKE_CXXFLAGS_DEBUG -= -O
#QMAKE_CXXFLAGS_DEBUG -= -O1
#QMAKE_CXXFLAGS_DEBUG -= -O2

# add the desired -O3 if not present
#QMAKE_CXXFLAGS_DEBUG *= -O3

SOURCES += \
    src/main.cpp \
    src/filetypes/hkxfile.cpp \
    src/ui/fileselectwindow.cpp \
    src/ui/mainwindow.cpp \
    src/xml/hkxxmlreader.cpp \
    src/hkxclasses/hkbbehaviorgraphstringdata.cpp \
    src/hkxclasses/hkxobject.cpp \
    src/hkxclasses/hkbvariablevalueset.cpp \
    src/hkxclasses/hkbbehaviorgraphdata.cpp \
    src/hkxclasses/generators/hkbbehaviorgraph.cpp \
    src/hkxclasses/generators/hkbgenerator.cpp \
    src/ui/behaviorgraphui/behaviorgraphicons.cpp \
    src/ui/behaviorgraphui/behaviorgraphui.cpp \
    src/hkxclasses/hkrootlevelcontainer.cpp \
    src/hkxclasses/generators/hkbbehaviorreferencegenerator.cpp \
    src/hkxclasses/generators/hkbstatemachine.cpp \
    src/hkxclasses/generators/hkbstatemachinestateinfo.cpp \
    src/hkxclasses/generators/hkbclipgenerator.cpp \
    src/hkxclasses/generators/bscyclicblendtransitiongenerator.cpp \
    src/hkxclasses/generators/hkbblendergenerator.cpp \
    src/hkxclasses/generators/hkbblendergeneratorchild.cpp \
    src/hkxclasses/generators/hkbmanualselectorgenerator.cpp \
    src/hkxclasses/generators/hkbmodifiergenerator.cpp \
    src/hkxclasses/generators/bsboneswitchgenerator.cpp \
    src/hkxclasses/generators/bsboneswitchgeneratorbonedata.cpp \
    src/hkxclasses/generators/bsistatetagginggenerator.cpp \
    src/hkxclasses/generators/bssynchronizedclipgenerator.cpp \
    src/hkxclasses/generators/hkbposematchinggenerator.cpp \
    src/hkxclasses/generators/bsoffsetanimationgenerator.cpp \
    src/hkxclasses/hkbstringeventpayload.cpp \
    src/hkxclasses/hkbboneweightarray.cpp \
    src/hkxclasses/hkbstatemachineeventpropertyarray.cpp \
    src/hkxclasses/hkbstatemachinetransitioninfoarray.cpp \
    src/hkxclasses/modifiers/hkbmodifier.cpp \
    src/ui/hkdataui.cpp \
    src/ui/objectdataui/generators/bsistatetagginggeneratorui.cpp \
    src/ui/objectdataui/generators/modifiergeneratorui.cpp \
    src/ui/objectdataui/generators/manualselectorgeneratorui.cpp \
    src/ui/behaviorvariablesui.cpp \
    src/ui/eventsui.cpp \
    src/ui/objectdataui/generators/statemachineui.cpp \
    src/ui/objectdataui/generators/stateui.cpp \
    src/hkxclasses/hkbvariablebindingset.cpp \
    src/ui/transitionsui.cpp \
    src/ui/blendingtransitioneffectui.cpp \
    src/hkxclasses/hkbexpressioncondition.cpp \
    src/ui/genericdatawidgets.cpp \
    src/xml/hkxxmlwriter.cpp \
    src/hkxclasses/hkbcliptriggerarray.cpp \
    src/hkxclasses/hkbblendingtransitioneffect.cpp \
    src/hkxclasses/modifiers/hkbmodifierlist.cpp \
    src/hkxclasses/modifiers/hkbtwistmodifier.cpp \
    src/hkxclasses/modifiers/hkbeventdrivenmodifier.cpp \
    src/hkxclasses/modifiers/bsisactivemodifier.cpp \
    src/hkxclasses/modifiers/bslimbikmodifier.cpp \
    src/hkxclasses/modifiers/bsspeedsamplermodifier.cpp \
    src/hkxclasses/modifiers/bslookatmodifier.cpp \
    src/hkxclasses/modifiers/hkbfootikcontrolsmodifier.cpp \
    src/hkxclasses/modifiers/hkbevaluateexpressionmodifier.cpp \
    src/hkxclasses/hkbexpressiondataarray.cpp \
    src/hkxclasses/modifiers/bsmodifyoncemodifier.cpp \
    src/hkxclasses/modifiers/hkbrotatecharactermodifier.cpp \
    src/hkxclasses/modifiers/bseventondeactivatemodifier.cpp \
    src/hkxclasses/modifiers/hkbdampingmodifier.cpp \
    src/hkxclasses/modifiers/bseventonfalsetotruemodifier.cpp \
    src/hkxclasses/modifiers/hkbkeyframebonesmodifier.cpp \
    src/hkxclasses/hkbboneindexarray.cpp \
    src/hkxclasses/modifiers/bsragdollcontactlistenermodifier.cpp \
    src/hkxclasses/modifiers/hkbpoweredragdollcontrolsmodifier.cpp \
    src/hkxclasses/modifiers/hkbtimermodifier.cpp \
    src/hkxclasses/modifiers/hkbrigidbodyragdollcontrolsmodifier.cpp \
    src/hkxclasses/modifiers/hkbgetupmodifier.cpp \
    src/hkxclasses/modifiers/bsdirectatmodifier.cpp \
    src/hkxclasses/modifiers/hkbcomputedirectionmodifier.cpp \
    src/hkxclasses/modifiers/hkbevaluatehandlemodifier.cpp \
    src/hkxclasses/modifiers/hkbgethandleonbonemodifier.cpp \
    src/hkxclasses/modifiers/hkbsensehandlemodifier.cpp

HEADERS  += \
    src/filetypes/hkxfile.h \
    src/ui/fileselectwindow.h \
    src/ui/mainwindow.h \
    src/xml/hkxxmlreader.h \
    src/hkxclasses/hkbbehaviorgraphstringdata.h \
    src/hkxclasses/hkxobject.h \
    src/hkxclasses/hkbvariablevalueset.h \
    src/hkxclasses/hkbbehaviorgraphdata.h \
    src/hkxclasses/generators/hkbbehaviorgraph.h \
    src/hkxclasses/generators/hkbgenerator.h \
    src/ui/behaviorgraphui/behaviorgraphicons.h \
    src/ui/behaviorgraphui/behaviorgraphui.h \
    src/hkxclasses/hkrootlevelcontainer.h \
    src/hkxclasses/generators/hkbbehaviorreferencegenerator.h \
    src/hkxclasses/generators/hkbstatemachine.h \
    src/hkxclasses/generators/hkbstatemachinestateinfo.h \
    src/hkxclasses/generators/hkbclipgenerator.h \
    src/hkxclasses/generators/bscyclicblendtransitiongenerator.h \
    src/hkxclasses/generators/hkbblendergenerator.h \
    src/hkxclasses/generators/hkbblendergeneratorchild.h \
    src/hkxclasses/generators/hkbmanualselectorgenerator.h \
    src/hkxclasses/generators/hkbmodifiergenerator.h \
    src/hkxclasses/generators/bsboneswitchgenerator.h \
    src/hkxclasses/generators/bsboneswitchgeneratorbonedata.h \
    src/hkxclasses/generators/bsistatetagginggenerator.h \
    src/hkxclasses/generators/bssynchronizedclipgenerator.h \
    src/hkxclasses/generators/hkbposematchinggenerator.h \
    src/hkxclasses/generators/bsoffsetanimationgenerator.h \
    src/hkxclasses/hkbstringeventpayload.h \
    src/hkxclasses/hkbboneweightarray.h \
    src/hkxclasses/hkbstatemachineeventpropertyarray.h \
    src/hkxclasses/hkbstatemachinetransitioninfoarray.h \
    src/ui/genericdatawidgets.h \
    src/hkxclasses/modifiers/hkbmodifier.h \
    src/ui/hkdataui.h \
    src/ui/objectdataui/generators/bsistatetagginggeneratorui.h \
    src/ui/objectdataui/generators/modifiergeneratorui.h \
    src/ui/objectdataui/generators/manualselectorgeneratorui.h \
    src/ui/behaviorvariablesui.h \
    src/utility.h \
    src/ui/eventsui.h \
    src/ui/objectdataui/generators/statemachineui.h \
    src/ui/objectdataui/generators/stateui.h \
    src/hkxclasses/hkbvariablebindingset.h \
    src/ui/transitionsui.h \
    src/ui/blendingtransitioneffectui.h \
    src/hkxclasses/hkbexpressioncondition.h \
    src/xml/hkxxmlwriter.h \
    src/hkxclasses/hkbcliptriggerarray.h \
    src/hkxclasses/hkbblendingtransitioneffect.h \
    src/hkxclasses/modifiers/hkbmodifierlist.h \
    src/hkxclasses/modifiers/hkbtwistmodifier.h \
    src/hkxclasses/modifiers/hkbeventdrivenmodifier.h \
    src/hkxclasses/modifiers/bsisactivemodifier.h \
    src/hkxclasses/modifiers/bslimbikmodifier.h \
    src/hkxclasses/modifiers/bsspeedsamplermodifier.h \
    src/hkxclasses/modifiers/bslookatmodifier.h \
    src/hkxclasses/modifiers/hkbfootikcontrolsmodifier.h \
    src/hkxclasses/modifiers/hkbevaluateexpressionmodifier.h \
    src/hkxclasses/hkbexpressiondataarray.h \
    src/hkxclasses/modifiers/bsmodifyoncemodifier.h \
    src/hkxclasses/modifiers/hkbrotatecharactermodifier.h \
    src/hkxclasses/modifiers/bseventondeactivatemodifier.h \
    src/hkxclasses/modifiers/hkbdampingmodifier.h \
    src/hkxclasses/modifiers/bseventonfalsetotruemodifier.h \
    src/hkxclasses/modifiers/hkbkeyframebonesmodifier.h \
    src/hkxclasses/hkbboneindexarray.h \
    src/hkxclasses/modifiers/bsragdollcontactlistenermodifier.h \
    src/hkxclasses/modifiers/hkbpoweredragdollcontrolsmodifier.h \
    src/hkxclasses/modifiers/hkbtimermodifier.h \
    src/hkxclasses/modifiers/hkbrigidbodyragdollcontrolsmodifier.h \
    src/hkxclasses/modifiers/hkbgetupmodifier.h \
    src/hkxclasses/modifiers/bsdirectatmodifier.h \
    src/hkxclasses/modifiers/hkbcomputedirectionmodifier.h \
    src/hkxclasses/modifiers/hkbevaluatehandlemodifier.h \
    src/hkxclasses/modifiers/hkbgethandleonbonemodifier.h \
    src/hkxclasses/modifiers/hkbsensehandlemodifier.h
