#!/usr/bin/python

"""
Generate files

usage:

  ./tools/GenFiles.py

@author Bowen Zhang
@date 2020/10/05
"""

import random
from ROOT import TFile, TH1D, TF1


def GenEx2():
    h = TH1D("flat", "y = 10", 100, 0, 100)
    for i in range(0, h.GetNbinsX()):
        for _ in range(10 + random.randint(-4, 4)):
            h.Fill(i)

    out = TFile("res/Ex2.root", "recreate")

    h.Write()

    out.Close()


def GenEx2_exp():
    f = TF1("f", "[0] * exp ([1] * x) + [2]", 0, 200)
    f.SetParameter(0, 10)
    f.SetParameter(1, -0.01)
    f.SetParameter(2, 5)

    h = f.CreateHistogram()

    out = TFile("res/Ex2_exp.root", "recreate")
    
    h.SetName("exp")
    h.SetTitle("10 * exp (-0.01 * x) + 5")
    
    h.Write()
    
    out.Close()


GenEx2()
