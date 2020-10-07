#include "ExDef.h"
#include "Utils.h"
#include "TROOT.h"
#include "TStyle.h"

#include <thread>
#include <mutex>

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
 *  ./HEPStatisticPractice
 *
 * 现有的例子:
 *  - E1: 线性拟合测试 GSL 和 ROOT
 *  - E2: Chi-square 拟合，最小化
 *  - E3: 计数实验，PLR，显著性
 *  - E4: 计数实验，PLR，上限
 *  - E5: RooStats 创建 Workspace
 *
 *
 * 可能会添加：
 *  - Confidence Interval
 *  - Maximum likelihood fit
 *  - 计数实验，添加误差项
 *  - RooStats 例子，需要创建 workspace
 *
 *
 * @file main.cpp
 * @author Bowen Zhang
 * @date 2020/10/03
 *
 * @todo 打印所有可供选择的例子 -> printEx()
 */



/// prototypes
void Test();
void PrintInfo();


/// fields
// mutex for Testing, no overlap if run multi tests
// this feature is only for developer
std::mutex gMutexTest;
std::mutex gMutexPrint;
const std::size_t gNTests = 1;

// main function
int main()
{
    std::thread tPrint(PrintInfo);
    tPrint.join();

    std::vector<std::thread> vThreads;

    for (std::size_t i = 0; i < gNTests; ++i)
        vThreads.emplace_back(Test);

    std::for_each(vThreads.begin(), vThreads.end(), [](std::thread& t){ t.join(); });
}

// print info, also set up ROOT
void PrintInfo()
{
    auto printFrameLine = []()
    { SP::IO::println(" *--------------------------------------------* "); };
    auto printEmptyLine = []()
    { SP::IO::println(" |                                            | "); };

    printFrameLine();                                                       printEmptyLine();
    SP::IO::println(" | Welcome to HEP Statistics Practice Program | ");    printEmptyLine();
    SP::IO::println(" |                        Author: Bowen Zhang | ");    printEmptyLine();
    printFrameLine();

    gMutexPrint.lock();

    // Paintings are not thread safe?
    gROOT->SetBatch(true);

    // Set ATLAS style
    gROOT->SetStyle("ATLAS");
    gStyle->SetErrorX(0.5);

    gMutexPrint.unlock();
}

// do excercises here
void Test()
{
    gMutexTest.lock();

    SP::ExFactoryCollection ExCol;
    ExCol.setExFacts();
    const auto& ExFacts = ExCol.getExFacts();

    int cont = true;

    while (cont) {
        try {
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
        } catch (const std::exception& e) {
            gMutexTest.unlock();
            std::cerr << e.what() << '\n';
        }
    }
    gMutexTest.unlock();
}
