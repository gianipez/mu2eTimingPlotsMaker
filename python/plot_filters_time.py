import time
import math
import numpy as np
import matplotlib.pyplot as plt
import glob

from ROOT import TFile, TH1F
from importlib import import_module

import os, sys
import argparse

from concurrent.futures import ThreadPoolExecutor


names  = { 
    'producers' : { "makeSD",
                    "CaloClusterFast",
                    #"CaloDigiMaker",
                    "CaloHitMakerFast",
                    "FastCaloHitMaker",
                    "OfflineFragmentReader",
                    "TTCalHelixFinderDe",
                    "TTCalHelixMergerDeM",
                    "TTCalHelixMergerDeP",
                    "TTCalSeedFitDem",
                    "TTCalSeedFitDep",
                    "TTCalTimePeakFinder",
                    "TTflagBkgHits",
                    "TThelixFinder",
                    "TTHelixMergerDeM",
                    "TTHelixMergerDeP",
                    "TTKSFDeM",
                    "TTKSFDeP",
                    "TTmakePH",
                    "TTmakeSH",
                    "TTtimeClusterFinder"
                },
    'prescalers' : {"cprLowPSeedDeMEventPrescale",
                    "cprLowPSeedDeMPrescale",
                    "cprLowPSeedDePEventPrescale",
                    "cprLowPSeedDePPrescale",
                    "cprSeedDeMEventPrescale",
                    "cprSeedDeMPrescale",
                    "cprSeedDePEventPrescale",
                    "cprSeedDePPrescale",
                    "tprHelixCalibIPADeMEventPrescale",
                    "tprHelixIPADeMEventPrescale",
                    "tprHelixCalibIPADeMPrescale",
                    "tprHelixIPADeMPrescale",
                    "tprLowPSeedDeMEventPrescale",
                    "tprLowPSeedDeMPrescale",
                    "tprLowPSeedDePEventPrescale",
                    "tprLowPSeedDePPrescale",
                    "tprSeedDeMEventPrescale",
                    "tprSeedDeMPrescale",
                    "tprSeedDePEventPrescale",
                    "tprSeedDePPrescale"
                },
    
                    
    'filters_cpr0'   : {"cprLowPSeedDeMHSFilter",
                        "cprLowPSeedDeMSDCountFilter",
                        "cprLowPSeedDeMTCFilter",
                        "cprLowPSeedDeMTSFilter",
                        "cprLowPSeedDePHSFilter",
                        "cprLowPSeedDePSDCountFilter",
                        "cprLowPSeedDePTCFilter",
                        "cprLowPSeedDePTSFilter"},
    'filters_cor1'   :{ "cprSeedDeMHSFilter",
                        "cprSeedDeMSDCountFilter",
                        "cprSeedDeMTCFilter",
                        "cprSeedDeMTSFilter",
                        "cprSeedDePHSFilter",
                        "cprSeedDePSDCountFilter",
                        "cprSeedDePTCFilter",
                        "cprSeedDePTSFilter"},

    'filters_tpr0'   : {"tprHelixCalibIPADeMHSFilter",
                        "tprHelixCalibIPADeMSDCountFilter",
                        "tprHelixCalibIPADeMTCFilter",
                        "tprHelixIPADeMHSFilter",
                        "tprHelixIPADeMSDCountFilter",
                        "tprHelixIPADeMTCFilter"},

    'filters_tpr1'   : {"tprLowPSeedDeMHSFilter",
                        "tprLowPSeedDeMSDCountFilter",
                        "tprLowPSeedDeMTCFilter",
                        "tprLowPSeedDeMTSFilter",
                        "tprLowPSeedDePHSFilter",
                        "tprLowPSeedDePSDCountFilter",
                        "tprLowPSeedDePTCFilter",
                        "tprLowPSeedDePTSFilter"},

    'filters_tpr2'   : {"tprSeedDeMHSFilter",
                        "tprSeedDeMSDCountFilter",
                        "tprSeedDeMTCFilter",
                        "tprSeedDeMTSFilter",
                        "tprSeedDePHSFilter",
                        "tprSeedDePSDCountFilter",
                        "tprSeedDePTCFilter",
                        "tprSeedDePTSFilter"},

    'summary'    : {"tot_event_timing",
                    "tot_event_timing_noFilters",
                    "tot_timing_filters",
                    "tot_timing_prescale",
                    "tot_timing_SDFilter",
                    "tot_timing_TCFilter",
                    "tot_timing_HSFilter",
                    "tot_timing_TSFilter"}
}



def parseArgs(argv):
    parser = argparse.ArgumentParser(prog='./plot_filter_time.py')
    
    parser.add_argument("--filename", "-s", dest='filename', default="", type=str, help="input file")
    parser.add_argument("--out_dir", "-o", dest='out_dir', default="", type=str, help="input file")
    args = parser.parse_args(argv)
        
    return args

def extract_info(filename, tag):
    file = TFile.Open(filename)
    
    labels = []
    timing = []
    timing_err = []
    for n in names[tag]:
        h = file.Get("hm0_{}".format(n))
        if h == None: continue
        labels.append(n)
        timing.append(h.GetMean())
        timing_err.append(h.GetMean())

    return labels, timing, timing_err

def plot_module_timing(args, tag):
    
    labels, timing, timing_err = extract_info(args.filename, tag)
      
    plt.clf()
    plt.rcdefaults()
    plt.subplots_adjust(left=0.3,right=0.95, top=0.95, bottom=0.1)

    ax = plt.gca()
    # ax.set_ymargin(left=0.3,right=0)
    y_pos = np.arange(len(labels))

    ax.barh(y_pos, timing, align='center')
    ax.set_yticks(y_pos)
    ax.set_yticklabels(labels,fontsize=7)
    ax.invert_yaxis()  # labels read top-to-bottom
    if 'summary' not in tag:
        ax.set_xlabel('timing/module_visit [ms]')
    else:
        ax.set_xlabel('timing/event [ms]')
    ax.set_title('Modules {} timing chart'.format(tag))

    for i, v in enumerate(timing):
        ax.text(v + 0., i - 0., str("%2.2f"%v), color='blue', fontweight='bold')
    
    # top = len(y_pos)*1.1
    plt.ylim(-0.5, len(timing) +1.5)

    plt.text(0.05,0.97,r'$\mathbf{{Mu2e}}$ Preliminary',ha="left",va="top",family='sans-serif',transform=ax.transAxes,fontsize=13)
    # plt.text(0.05,0.92,'ots',ha="left",va="top",family='sans-serif',transform=ax.transAxes,style='italic',fontsize=8)
    # plt.text(0.05,0.88,r'$\sqrt{s}=13\,\mathrm{TeV}$',ha="left",va="top",family='sans-serif',transform=ax.transAxes)

    # if flow_etau_VBF[0]>0:
    #     plt.xscale('log')

    plt.savefig("{}/plot_timing_modules_{}.pdf".format(args.out_dir,tag))

    return

def plot_filters_time(args):

    for key in names:
        plot_module_timing(args, key)
    
#--------------------------------------------------------------------------------
if __name__=="__main__":
    args = parseArgs(sys.argv[1:])
    plot_filters_time(args)

