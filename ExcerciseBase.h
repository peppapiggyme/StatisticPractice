#ifndef EXCERCISEBASE_H
#define EXCERCISEBASE_H

/**
 * @brief The base Excercise class
 * new Excercises must inherite from this class
 * and use index 1, 2, 3 as their postfix
 *
 * @file ExcerciseBase.h
 * @author Bowen Zhang
 * @date 2020/10/03
 */

class ExcerciseBase
{
public:
    ExcerciseBase() = default;
    virtual ~ExcerciseBase() = default;
    virtual void test() const = 0;
};

#define DEFINENEWEXCERCISE(I) \
    class Excercise_##I : public ExcerciseBase \
    { \
    public: \
        Excercise_##I() : ExcerciseBase() {} \
        void test() const override; \
    };

#endif // EXCERCISEBASE_H
