#!/usr/bin/python

"""
A tool that download all the root files
that are needed for Ex5 - Atlas Open Data
H->ZZ->4l exercise

Usage:

    tool/DownloadOpenData.py

Warning: will not add if file exists!

@author: Bowen Zhang
@date: 2020/12/04

"""

import os
import subprocess
import requests


PATH = "https://atlas-opendata.web.cern.ch/atlas-opendata/samples/2020/";


class MC(object): 
    def __init__(self, dsid, proc, chan):
        self.filename = "mc_{}.{}.{}.root".format(dsid, proc, chan)
        self.url = "{}{}/MC/{}".format(PATH, chan, self.filename)


class Data(object):
    def __init__(self, label, chan):
        self.filename = "data_{}.{}.root".format(label, chan);
        self.url = "{}{}/Data/{}".format(PATH, chan, self.filename)


class Resources(object):
    def __init__(self):
        self.dataObjects = [Data(s, "4lep") for s in ["A", "B", "C", "D"]]
        self.mcObjects = [MC("363356", "ZqqZll", "4lep"), 
                          MC("363358", "WqqZll", "2lep"), 
                          MC("363359", "WpqqWmlv", "2lep"), 
                          MC("363360", "WplvWmqq", "2lep"), 
                          MC("363489", "WlvZqq", "2lep"), 
                          MC("363490", "llll", "4lep"), 
                          MC("363491", "lllv", "4lep"), 
                          MC("363492", "llvv", "4lep"), 
                          MC("363493", "lvvv", "2lep"), 
                          MC("361106", "Zee", "4lep"), 
                          MC("361107", "Zmumu", "4lep"), 
                          MC("361108", "Ztautau", "4lep"), 
                          MC("410011", "single_top_tchan", "4lep"), 
                          MC("410012", "single_antitop_tchan", "4lep"), 
                          MC("410025", "single_top_schan", "4lep"), 
                          MC("410026", "single_antitop_schan", "4lep"), 
                          MC("410013", "single_top_wtchan", "4lep"), 
                          MC("410014", "single_antitop_wtchan", "4lep"), 
                          MC("410000", "ttbar_lep", "4lep"), 
                          MC("361100", "Wplusenu", "2lep"), 
                          MC("361101", "Wplusmunu", "2lep"), 
                          MC("361102", "Wplustaunu", "2lep"),
                          MC("361103", "Wminusenu", "2lep"),
                          MC("361104", "Wminusmunu", "2lep"),
                          MC("361105", "Wminustaunu", "2lep"), 
                          MC("345060", "ggH125_ZZ4lep", "4lep"), 
                          MC("341947", "ZH125_ZZ4lep", "4lep"), 
                          MC("341964", "WH125_ZZ4lep", "4lep"), 
                          MC("344235", "VBFH125_ZZ4lep", "4lep"),
                          MC("307431", "RS_G_ZZ_llll_c10_m0200", "4lep"),]
        

def download_from_url(url, dest):
    print "Download file from {}".format(url)
    r = requests.get(url, allow_redirects=True)
    if r.status_code != 200:
        print " --> {} does not exist!".format(url)
    else:
        with open(dest, 'wb') as f:
            f.write(r.content)


def download():
    res = Resources()

    for obj in res.dataObjects:
        download_from_url(obj.url, os.environ["RES_PATH"] + obj.filename)

    for obj in res.mcObjects:
        download_from_url(obj.url, os.environ["RES_PATH"] + obj.filename)

download()
