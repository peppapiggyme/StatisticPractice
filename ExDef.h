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

#include "Utils.h"
#include "ExerciseBase.h"
#include "Exercise_1.h"
#include "Exercise_2.h"
#include "Exercise_3.h"
#include "Exercise_4.h"
#include "Exercise_5.h"
#include "Exercise_6.h"
#include "Exercise_7.h"
#include "Exercise_8.h"
#include "Exercise_9.h"
#include "Exercise_10.h"
#include "Exercise_11.h"
#include "Exercise_12.h"

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

#define ADD_NEW_EXFACTORY(I) \
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

ADD_NEW_EXFACTORY(1);
ADD_NEW_EXFACTORY(2);
ADD_NEW_EXFACTORY(3);
ADD_NEW_EXFACTORY(4);
ADD_NEW_EXFACTORY(5);
ADD_NEW_EXFACTORY(6);
ADD_NEW_EXFACTORY(7);
ADD_NEW_EXFACTORY(8);
ADD_NEW_EXFACTORY(9);
ADD_NEW_EXFACTORY(10);
ADD_NEW_EXFACTORY(11);
ADD_NEW_EXFACTORY(12);

#endif // EXDEF_H
