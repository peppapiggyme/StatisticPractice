#include "ExDef.h"
#include "Utils.h"

/**
 * 本程序旨在为高能物理专业的同学提供统计学练习，一开始它是为
 * 《粒子物理与核物理实验方法》这门课设计的。面向本科生高年级
 * 以及研究生同学。
 *
 * 本程序的优点是老师或助教可以较容易的添加练习用例，同时学生
 * 的编程练习可以完全在一个cpp文件中进行。
 *
 * 用到的第三方库:
 *  - Gnu Scientific Library
 *  - CERN ROOT
 *
 * 用法:
 *  ./HEPStatisticPractise
 *
 * 现有的例子:
 *  - E1: 线性拟合测试 GSL 和 ROOT
 *  - E2: 空白
 *
 * @file main.cpp
 * @author Bowen Zhang
 * @date 2020/10/03
 *
 * @todo 打印所有可供选择的例子 -> printEx()
 */

// prototypes
void test();


// main function
int main()
{
    try {
        test();
    } catch (std::exception& e) {
        std::cerr << e.what() << '\n';

    }
    return 0;
}


// do excercises here
void test()
{
    SP::ExFactoryCollection ExCol;
    ExCol.setExFacts();
    const auto& ExFacts = ExCol.getExFacts();

    int cont = true;

    while (cont) {
        int i = SP::IO::getInteger();
        if (i > (int)ExFacts.size() || i < 1)
        {
            std::cerr << "练习" << i << "不存在， 请重试！\n";
            cont = SP::IO::doContinue();
            continue;
        }

        const auto e = ExFacts[i-1]->Create();
        e->test();
        delete e;

        cont = SP::IO::doContinue();
    }
}
