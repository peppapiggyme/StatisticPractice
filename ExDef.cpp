#ifndef EXDEF_CXX
#define EXDEF_CXX

#include "ExDef.h"

using namespace SP;

void SP::ExFactoryCollection::setExFacts()
{
    ExFacts.push_back(INSTANCE_EXFACTORY(1));
    ExFacts.push_back(INSTANCE_EXFACTORY(2));
}


std::vector<SP::ExFactory*> SP::ExFactoryCollection::getExFacts() const
{
    return ExFacts;
}

#endif // EXDEF_CXX
