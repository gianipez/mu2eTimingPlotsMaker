import time
import math
#import numpy as np
import matplotlib.pyplot as plt
import glob

from ROOT import TFile
from importlib import import_module

import os, sys
import argparse

from concurrent.futures import ThreadPoolExecutor


names  = { 
    'producers' : { "makeSD",
                    "CaloClusterFast",
                    #"CaloDigiMaker",
                    "CaloHitMakerFast",
                   # "FastCaloHitMaker",
                    "OfflineFragmentReader",
                    "TTCalHelixFinderDe",
                    "TTCalHelixMergerDe",
                    "TTCalSeedFitDe",
                   # "TTCalSeedFitDep",
                    "TTCalTimePeakFinder",
                    "TTflagBkgHits",
                    "TThelixFinder",
                    "TTHelixMergerDe",
                    "TTKSFDe",
                    "TTTZClusterFinder",
                    "TTAprHelixFinder",
                    "TTAprHelixMerger",
                    "TTAprKSF",
                    "TTmakePH",
                    "TTmakeSH",
                    "TTmakeSTH",
                    "TTtimeClusterFinder"
                },
    'prescalers' : {"cprLowPSeedDeEventPrescale",
                    "cprSeedDeEventPrescale",
                    "tprHelixCalibIPADeEventPrescale",
                    "tprHelixIPADeEventPrescale",
                    "tprHelixCalibIPADePrescale",
                    "tprHelixIPADePrescale",
                    "tprLowPSeedDeEventPrescale",
                    "tprLowPSeedDePrescale",
                    "tprLowPSeedDePEventPrescale",
                    "tprSeedDeEventPrescale",
                    "tprSeedDePrescale",
                    "tprSeedDePEventPrescale",
                },
    
                    
    'filters_cpr0'   : {"cprLowPSeedDeHSFilter",
                        "cprLowPSeedDeSDCountFilter",
                        "cprLowPSeedDeTCFilter",
                        "cprLowPSeedDeTSFilter"},
    'filters_cor1'   :{ "cprSeedDeHSFilter",
                        "cprSeedDeSDCountFilter",
                        "cprSeedDeTCFilter",
                        "cprSeedDeTSFilter"},

    'filters_tpr0'   : {"tprHelixDeIpaPhiScaledHSFilter",
                        "tprHelixCalibIPADeSDCountFilter",
                        "tprHelixDeIpaPhiScaledTCFilter",
                        "tprHelixDeIpaHSFilter",
                        "tprHelixIPADeSDCountFilter",
                        "tprHelixDeIpaTCFilter"},

    'filters_tpr1'   : {"tprLowPSeedDeHSFilter",
                        "tprLowPSeedDeSDCountFilter",
                        "tprLowPSeedDeTCFilter",
                        "tprLowPSeedDeTSFilter"},

    'filters_tpr2'   : {"tprSeedDeHSFilter",
                        "tprSeedDeSDCountFilter",
                        "tprSeedDeTCFilter",
                        "tprSeedDeTSFilter"},

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
    for n in sorted(names[tag]):
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
    #y_pos = np.arange(len(labels))
    y_pos = [] #np.arange(len(labels))

    for i in range(len(labels)):
        y_pos.append(i)
    
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

    for key in sorted(names):
        plot_module_timing(args, key)
    
#--------------------------------------------------------------------------------
if __name__=="__main__":
    args = parseArgs(sys.argv[1:])
    plot_filters_time(args)

