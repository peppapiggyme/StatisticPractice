#ifndef EXDEF_CXX
#define EXDEF_CXX

#include "ExDef.h"

using namespace SP;

void SP::ExFactoryCollection::setExFacts()
{
    ExFacts.push_back(INSTANCE_EXFACTORY(0));
    ExFacts.push_back(INSTANCE_EXFACTORY(1));
    ExFacts.push_back(INSTANCE_EXFACTORY(2));
    ExFacts.push_back(INSTANCE_EXFACTORY(3));
    ExFacts.push_back(INSTANCE_EXFACTORY(4));
    ExFacts.push_back(INSTANCE_EXFACTORY(5));
    ExFacts.push_back(INSTANCE_EXFACTORY(6));
    ExFacts.push_back(INSTANCE_EXFACTORY(7));
    ExFacts.push_back(INSTANCE_EXFACTORY(8));
    ExFacts.push_back(INSTANCE_EXFACTORY(9));
    ExFacts.push_back(INSTANCE_EXFACTORY(10));
    ExFacts.push_back(INSTANCE_EXFACTORY(11));
    ExFacts.push_back(INSTANCE_EXFACTORY(12));
}


std::vector<SP::ExFactory*> SP::ExFactoryCollection::getExFacts() const
{
    return ExFacts;
}

#endif // EXDEF_CXX
