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
 *  - E1: 复习 ROOT 直方图 Fit 功能
 *  - E2: Chi-square 拟合，最小化
 *  - E3: 计数实验，PLR，显著性
 *  - E4: 计数实验，PLR，上限
 *  - E5: RooStats 创建 Workspace
 *  - E6: RooStats 从 Histogram 到结果
 *  - E7: OpenData 实践，MakeClass
 *
 *
 * 可能会添加：
 *  - Confidence Interval
 *  - Maximum likelihood fit
 *  - 计数实验，添加误差项
 *  - 使用 Minuit 的例子
 *  - http://personalpages.to.infn.it/~ramello/statis/expFit.cc
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
std::mutex gMutexPrint;

// main function
int main()
{
    std::thread tPrint(PrintInfo);
    tPrint.join();

    Test();

    return EXIT_SUCCESS;
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
    SP::IO::println(" |            Copyright (C) 2020  Bowen Zhang | ");    printEmptyLine();
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
                continue;
            }

            const auto e = ExFacts[i-1]->Create();
            e->test();
            delete e;

            cont = SP::IO::doContinue();
        } catch (const std::exception& e) {
            std::cerr << e.what() << '\n';
            cont = SP::IO::doContinue();
        }
    }
}
