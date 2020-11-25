#ifndef EXCERCISEBASE_H
#define EXCERCISEBASE_H

/**
 * @brief The base Exercise class
 * new Exercises must inherite from this class
 * and use index 1, 2, 3 as their postfix
 *
 * @file ExerciseBase.h
 * @author Bowen Zhang
 * @date 2020/10/03
 */

class ExerciseBase
{
public:
    ExerciseBase() = default;
    virtual ~ExerciseBase() = default;
    virtual void test() const = 0;
};

#define DEFINE_NEW_EXERCISE(I) \
    class Exercise_##I : public ExerciseBase \
    { \
    public: \
        Exercise_##I() : ExerciseBase() {} \
        void test() const override; \
    };

#endif // EXCERCISEBASE_H
