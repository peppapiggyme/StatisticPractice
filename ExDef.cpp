#ifndef EXDEF_CXX
#define EXDEF_CXX

#include "ExDef.h"

using namespace SP;

void SP::ExFactoryCollection::setExFacts()
{
    ExFacts.push_back(INSTANCE_EXFACTORY(1));
    ExFacts.push_back(INSTANCE_EXFACTORY(2));
    ExFacts.push_back(INSTANCE_EXFACTORY(3));
    ExFacts.push_back(INSTANCE_EXFACTORY(4));
    ExFacts.push_back(INSTANCE_EXFACTORY(5));
    ExFacts.push_back(INSTANCE_EXFACTORY(6));
}


std::vector<SP::ExFactory*> SP::ExFactoryCollection::getExFacts() const
{
    return ExFacts;
}

#endif // EXDEF_CXX
