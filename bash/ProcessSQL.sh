#!/bin/bash
# for mod in "makeSH" "FSHPreStereo" "MakeStrawHitPositions" "MakeStereoHits" "FlagStrawHits" "FlagBkgHits" "TimeClusterFinder" "PosHelixFinder" "KSFDeM" "CalPatRecFSHP" "CalPatRecMakeStrawHitPositions" "CalPatRecMakeStereoHits" "CalPatRecFlagStrawHits" "CalPatRecFlagBkgHits" "CalTimePeakFinder" "CalHelixFinder" "CalSeedFit" "CalTrkFit"; do

for file in "$1"/*.csv
do 

    for mod in "TTmakeSH" "TTmakePH" "TTflagBkgHits" "CaloClusterFast" "TTtimeClusterFinder" "TThelixFinder" "TTKSFDeM" "TTKSFDeP" "TTCalTimePeakFinder" "TTCalHelixFinderDe" "TTCalSeedFitDem" "TTCalSeedFitDep" "subsystemOutput" "makeSD" "CaloDigiMaker" "tprSeedDeMEventPrescale" "tprSeedDeMSDCountFilter" "tprSeedDeMTCFilter" "TTHelixMergerDeM" "tprSeedDeMHSFilter" "tprSeedDeMTSFilter" "tprSeedDeMPrescale"  "tprSeedDePEventPrescale" "tprSeedDePSDCountFilter" "tprSeedDePTCFilter" "TTHelixMergerDeP" "tprSeedDePHSFilter" "tprSeedDePTSFilter" "tprSeedDePPrescale" "tprLowPSeedDeMEventPrescale" "tprLowPSeedDeMSDCountFilter" "tprLowPSeedDeMTCFilter" "tprLowPSeedDeMHSFilter" "tprLowPSeedDeMTSFilter" "tprLowPSeedDeMPrescale"  "tprLowPSeedDePEventPrescale" "tprLowPSeedDePSDCountFilter" "tprLowPSeedDePTCFilter" "tprLowPSeedDePHSFilter" "tprLowPSeedDePTSFilter" "tprLowPSeedDePPrescale" "cprSeedDeMEventPrescale" "cprSeedDeMSDCountFilter" "cprSeedDeMTCFilter" "TTCalHelixMergerDeM" "cprSeedDeMHSFilter" "cprSeedDeMTSFilter" "cprSeedDeMPrescale"  "cprSeedDePEventPrescale" "cprSeedDePSDCountFilter" "cprSeedDePTCFilter" "TTCalHelixMergerDeP" "cprSeedDePHSFilter" "cprSeedDePTSFilter" "cprSeedDePPrescale" "cprLowPSeedDeMEventPrescale" "cprLowPSeedDeMSDCountFilter" "cprLowPSeedDeMTCFilter" "cprLowPSeedDeMHSFilter" "cprLowPSeedDeMTSFilter" "cprLowPSeedDeMPrescale"  "cprLowPSeedDePEventPrescale" "cprLowPSeedDePSDCountFilter" "cprLowPSeedDePTCFilter" "cprLowPSeedDePHSFilter" "cprLowPSeedDePTSFilter" "cprLowPSeedDePPrescale" "tprHelixIPADeMEventPrescale" "tprHelixIPADeMSDCountFilter" "tprHelixIPADeMTCFilter" "tprHelixIPADeMHSFilter" "tprHelixIPADeMPrescale" "tprHelixCalibIPADeMEventPrescale" "tprHelixCalibIPADeMSDCountFilter" "tprHelixCalibIPADeMTCFilter" "tprHelixCalibIPADeMHSFilter" "tprHelixCalibIPADeMPrescale" ; do
	echo Processing module $mod
	sqlite3  -separator "," $file "SELECT Event, Time FROM TimeModule WHERE ModuleLabel=\"${mod}\";" >> $1/csv/$mod.csv
    done

    sqlite3  -separator "," $file  "SELECT Event, Time FROM TimeSource" >> $1/csv/OfflineFragmentReader.csv
    
    sqlite3  -separator "," $file  "SELECT Event, Time FROM TimeEvent" >> $1/csv/tot_event.csv
done

sed -n 'n;p' $1/subsystemOutput.csv >> $1/csv/subsystemOutput_write.csv
sed -n 'p;n' $1/subsystemOutput.csv >> $1/csv/subsystemOutput_init.csv
