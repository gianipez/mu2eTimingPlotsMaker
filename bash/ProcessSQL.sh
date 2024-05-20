#!/bin/bash


for dd in $1/*
do
    dir=$dd
    echo "Processing directory $dir"
    for file in ${dd}/*csv #"$1"/*.csv
    do 
        echo "--> Processing $file"
        
        tag=(${file//_/ })
        tag=${tag[7]}
        echo "Found tag ${tag}"
        if [[ -d "$dir/csv_${tag}" ]]; then
            continue
        else
            echo "--> Skipping $file"
        fi
        mkdir $dir/csv_${tag}
        echo "---> Processing batch ${tag}"
        for mod in "TTmakeSTH" "TTmakePH" "TTflagBkgHits" "CaloClusterFast" "TTtimeClusterFinder" "TThelixFinder" "TTKSFDe" "TTCalTimePeakFinder" "TTCalHelixFinderDe" "TTCalSeedFitDe" "subsystemOutput" "makeSD" "CaloHitMakerFast" "FastCaloHitMaker" "tprDeHighPStopTargSDCountFilter" "tprDeHighPStopTargTCFilter" "TTHelixMergerDe" "tprDeHighPStopTargHSFilter" "tprDeHighPStopTargTSFilter" "tprDeHighPStopTargPS" "tprLowPSeedDeSDCountFilter" "tprLowPSeedDeTCFilter" "tprLowPSeedDeHSFilter" "tprLowPSeedDeTSFilter" "tprLowPSeedDePS" "cprDeHighPStopTargSDCountFilter" "cprDeHighPStopTargTCFilter" "TTCalHelixMergerDe" "cprDeHighPStopTargHSFilter" "cprDeHighPStopTargTSFilter" "cprDeHighPStopTargPS" "cprLowPSeedDeSDCountFilter" "cprLowPSeedDeTCFilter" "cprLowPSeedDeHSFilter" "cprLowPSeedDeTSFilter" "tprHelixIPADeSDCountFilter" "tprHelixIPADeTCFilter" "tprHelixIPADeHSFilter" "tprHelixIPADePS" "tprHelixCalibIPADeSDCountFilter" "tprHelixCalibIPADeTCFilter" "tprHelixCalibIPADeHSFilter" "tprHelixCalibIPADePS" "TTTZClusterFinder" "aprHighPStopTargTCFilter" "TTAprHelixFinder" "TTAprHelixMerger" "aprHighPStopTargHSFilter" "TTAprKSF" "aprHighPStopTargKSFilter" "aprHighPStopTargTriggerInfoMerger" "aprLowPStopTargTCFilter" "aprLowPStopTargHSFilter" "aprLowPStopTargKSFilter" "aprLowPStopTargTriggerInfoMerger"; do
	    echo Processing module $mod
	    sqlite3  -separator "," $file "SELECT Event, Time FROM TimeModule WHERE ModuleLabel=\"${mod}\";" >| $dir/csv_${tag}/$mod.csv
        done
        
        sqlite3  -separator "," $file  "SELECT Event, Time FROM TimeSource" >| $dir/csv_${tag}/OfflineFragmentReader.csv
        
        sqlite3  -separator "," $file  "SELECT Event, Time FROM TimeEvent" >| $dir/csv_${tag}/tot_event.csv
        
        sed -n 'n;p' $dir/csv_${tag}/subsystemOutput.csv >| $dir/csv_${tag}/subsystemOutput_write.csv
        sed -n 'p;n' $dir/csv_${tag}/subsystemOutput.csv >| $dir/csv_${tag}/subsystemOutput_init.csv
    done
done

#sed -n 'n;p' $1/subsystemOutput.csv >| $1/csv_${tag}/subsystemOutput_write.csv
#sed -n 'p;n' $1/subsystemOutput.csv >| $1/csv_${tag}/subsystemOutput_init.csv
