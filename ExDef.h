#ifndef EXDEF_H
#define EXDEF_H

/**
 * @brief Whenever a new Exercise is added
 * One should also modify the ExDef.h and ExDef.cxx
 * To make sure they can be created by the factories
 *
 * @file ExDef.h
 * @author Bowen Zhang
 * @date 2020/10/03
 */

#include "Exercise_1.h"
#include "Exercise_2.h"
#include "Exercise_3.h"
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
        virtual ExerciseBase* Create() = 0;
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
        virtual ExerciseBase * Create() override { \
            return new Exercise_##I(); \
        } \
    }; \
    }

#define INSTANCE_EXFACTORY(I) new SP::ExFactory_##I()

ADDNEWEXFACTORY(1);
ADDNEWEXFACTORY(2);
ADDNEWEXFACTORY(3);

#endif // EXDEF_H
