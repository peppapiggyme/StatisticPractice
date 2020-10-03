#ifndef EXDEF_H
#define EXDEF_H

/**
 * @brief Whenever a new Excercise is added
 * One should also modify the ExDef.h and ExDef.cxx
 * To make sure they can be created by the factories
 *
 * @file ExDef.h
 * @author Bowen Zhang
 * @date 2020/10/03
 */

#include "Excercise_1.h"
#include "Excercise_2.h"
#include "Utils.h"

#include <vector>

namespace SP
{
    /**
     * 用来产生练习
     */
    class ExFactory
    {
    public:
        ExFactory() = default;
        virtual ~ExFactory() {}
        virtual ExcerciseBase* Create() = 0;
    };

    class ExFactoryCollection
    {
    public:
        ExFactoryCollection() = default;
        // RAII
        ~ExFactoryCollection() { SP::STL::ClearUp<SP::ExFactory*>(ExFacts); }
        // non-copiable or movable
        ExFactoryCollection(const ExFactoryCollection& other) = delete;
        ExFactoryCollection& operator=(const ExFactoryCollection& other) = delete;

        void setExFacts();
        std::vector<SP::ExFactory*> getExFacts() const;
    private:
        std::vector<SP::ExFactory*> ExFacts;
    };
}

#define ADDNEWEXFACTORY(I) \
    namespace SP { \
    class ExFactory_##I : public ExFactory \
    { \
    public: \
        virtual ExcerciseBase * Create() override { \
            return new Excercise_##I(); \
        } \
    }; \
    }

#define INSTANCE_EXFACTORY(I) new SP::ExFactory_##I()

ADDNEWEXFACTORY(1); //vExFacts.push_back(new INSTANCE_FACTORY(1));
ADDNEWEXFACTORY(2);// vExFacts.push_back(new INSTANCE_FACTORY(2));

#endif // EXDEF_H
