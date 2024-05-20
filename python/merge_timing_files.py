import numpy as np
import argparse
import time
import os, sys

my_dir=os.environ['PWD']
my_python_dir=my_dir+'/python'
sys.path.append(my_python_dir)

from modules import *

def parseArgs(argv):
    parser = argparse.ArgumentParser(prog='./merge_timing_files.py')
    
    parser.add_argument("--input-dir", "-i", dest='input_dir', default="", type=str, help="Input directory")
    parser.add_argument("--max-evts", "-max", dest='max_evts', default=1e7, type=int, help="max n events")

    try:
        args = parser.parse_args(argv)
        
        checkArgs(args)
        
    except:
        parser.print_help()
        raise

    return args

#-------------------------------------------------------------------------------
def checkArgs(args):
    if not os.path.exists(args.input_dir):
        os.makedirs(args.input_dir)
    elif not os.path.isdir(args.input_dir):
        raise Exception("Cannot access directory '{}'".format(args.output_dir))
        
    return

#-------------------------------------------------------------------------------
def merge_timing_files(args):
    events = []
    timing = []
    timing_prescale = []
    timing_filters  = []
    
    #readFile(args.input_dir+'/makeSD.csv', events, timing, args)
    readFile(args.input_dir+'/CaloHitMakerFast.csv', events, timing, args)
    print("[merge_timing_files] nevents from CaloHitMakerFast = {}".format(len(events)))
    timing_noFilters = timing.copy()
    timing_prescale  = np.zeros(len(timing_noFilters))
    timing_filters   = np.zeros(len(timing_noFilters))
    timing_SDFilter  = np.zeros(len(timing_noFilters))
    timing_TCFilter  = np.zeros(len(timing_noFilters))
    timing_HSFilter  = np.zeros(len(timing_noFilters))
    timing_TSFilter  = np.zeros(len(timing_noFilters))
    
    # modules = ["TTmakeSTH" , "TTmakePH" , "TTflagBkgHits", 
    #            "CaloClusterFast" , "TTtimeClusterFinder" , "TThelixFinder" ,
    #            "TTKSFDeM" , "TTKSFDeP" , 
    #            "TTCalTimePeakFinder" , "TTCalHelixFinderDe" ,
    #            "TTCalSeedFitDem" , "TTCalSeedFitDep", 
    #            # "subsystemOutput", 
    #            #"subsystemOutput_write","subsystemOutput_init",
    #            "CaloDigiFromShower", "OfflineFragmentReader"]
    
    for mm in modules:
        tmp_events = []
        tmp_timing = []
        fname = args.input_dir+'/'+mm+'.csv'
        if not os.path.exists(fname):
            continue
        readFile(fname, tmp_events, tmp_timing, args)
        print("processing module {}...".format(mm))
        for i, evt in enumerate(events):            
            for j, new_evt in enumerate(tmp_events):
                if evt == new_evt:
                    timing[i] = timing[i] + tmp_timing[j]
                    if "Filter" not in mm and "PS" not in mm:
                        timing_noFilters[i] = timing_noFilters[i] + tmp_timing[j]
                    if "Filter" in mm:
                        timing_filters[i]   = timing_filters[i]   + tmp_timing[j]
                    if "PS" in mm:
                        timing_prescale[i]  = timing_prescale[i]  + tmp_timing[j]
                    if "HSFilter" in mm:
                        timing_HSFilter[i]  = timing_HSFilter[i]  + tmp_timing[j]
                    if "TCFilter" in mm:
                        timing_TCFilter[i]  = timing_TCFilter[i]  + tmp_timing[j]
                    if "TSFilter" in mm:
                        timing_TSFilter[i]  = timing_TSFilter[i]  + tmp_timing[j]
                    # counter = counter + 1
                    break

    with open(args.input_dir+'/tot_event_timing.csv', 'w') as txt_file:
        for i in range(len(events)):
            print("{}, {}".format(events[i], timing[i]), file=txt_file)

    with open(args.input_dir+'/tot_event_timing_noFilters.csv', 'w') as txt_file:
        for i in range(len(events)):
            print("{}, {}".format(events[i], timing_noFilters[i]), file=txt_file)

    with open(args.input_dir+'/tot_timing_filters.csv', 'w') as txt_file:
        for i in range(len(events)):
            print("{}, {}".format(events[i], timing_filters[i]), file=txt_file)

    with open(args.input_dir+'/tot_timing_prescale.csv', 'w') as txt_file:
        for i in range(len(events)):
            print("{}, {}".format(events[i], timing_prescale[i]), file=txt_file)

    with open(args.input_dir+'/tot_timing_SDFilter.csv', 'w') as txt_file:
        for i in range(len(events)):
            print("{}, {}".format(events[i], timing_SDFilter[i]), file=txt_file)

    with open(args.input_dir+'/tot_timing_TCFilter.csv', 'w') as txt_file:
        for i in range(len(events)):
            print("{}, {}".format(events[i], timing_TCFilter[i]), file=txt_file)
    with open(args.input_dir+'/tot_timing_HSFilter.csv', 'w') as txt_file:
        for i in range(len(events)):
            print("{}, {}".format(events[i], timing_HSFilter[i]), file=txt_file)
    with open(args.input_dir+'/tot_timing_TSFilter.csv', 'w') as txt_file:
        for i in range(len(events)):
            print("{}, {}".format(events[i], timing_TSFilter[i]), file=txt_file)



#--------------------------------------------------------------------------------
def readFile(fileName, events, timing, args):
    f      = open(fileName)
    lines  = f.readlines()
    counter = 0
    for x in lines:
        if counter >= args.max_evts:
            break
        events.append(x.split(',')[0])
        timing.append(float(x.split(',')[1]))
        counter = counter + 1
#--------------------------------------------------------------------------------
if __name__=="__main__":
    args = parseArgs(sys.argv[1:])
    merge_timing_files(args)

