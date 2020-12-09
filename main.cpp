#include "ExDef.h"
#include "Utils.h"
#include "TROOT.h"
#include "TStyle.h"

#include <thread>
#include <mutex>
#include <list>

/**
 * 本程序:
 *
 * 老师或助教可以较容易的添加练习用例，同时学生
 * 的编程练习可以完全在一个cpp文件中进行。
 *
 * 用到的第三方库:
 *  - Gnu Scientific Library (x)
 *  - CERN ROOT (v)
 *
 * 用法:
 *  ./DoExercise
 * 
 * Statistical analysis examples are mostly took from this wonderful ATL:
 * https://indico.cern.ch/event/545212/
 *
 * all credits to L.Moneta and L. Lyons
 * 
 * And thanks to the amazing Atlas open data
 * https://github.com/atlas-outreach-data-tools/atlas-outreach-cpp-framework-13tev
 * 
 * all credits to the developers 
 *
 * @file main.cpp
 * @author Bowen Zhang
 * @date 2020/10/03
 *
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
    { SP::IO::println(" *---------------------------------* "); };
    auto printEmptyLine = []()
    { SP::IO::println(" |                                 | "); };
    auto printExercises = []()
    { SP::IO::println("\nChoose from:\n"
                      " (1) Method of Maximum Likelihood\n"
                      " (2) Method of Least Square\n"
                      " (3) Goodness of fit\n"
                      " (4) Combination of measurements\n"
                      " (5) Atlas physics analysis (Atlas Open Data: H->ZZ->4l)\n"
                      " (6) Discovery significance\n"
                      " (7) Exclusion limits\n"); };

    printFrameLine();                                            printEmptyLine();
    SP::IO::println(" | Welcome to the Practice Program | ");    printEmptyLine();
    SP::IO::println(" | Copyright (C) 2020  Bowen Zhang | ");    printEmptyLine();
    printFrameLine();
    printExercises();

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
