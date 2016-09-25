#ifndef HKBGENERATOR_H
#define HKBGENERATOR_H

#include "src/hkxclasses/hkxobject.h"

class GeneratorIcon;

/**
 * Adding new generator classes:
 *
 * Need to add to hkbGenerator::link()
 * Need to add to MainWindow::drawBehaviorGraph()
 * Need to add to BehaviorFile::parse()
 */

class hkbGenerator: public HkDynamicObject
{
    friend class BehaviorGraphView;
    friend class GeneratorIcon;
public:
    virtual ~hkbGenerator();
    bool link();
    virtual QString getName() const;
    QString getClassname() const;
    void updateIconNames();
protected:
    hkbGenerator(BehaviorFile *parent/*, long ref = 0*/);
    void appendIcon(GeneratorIcon * icon);
private:
    QList <GeneratorIcon *> icons;
};


#endif // HKBGENERATOR_H
