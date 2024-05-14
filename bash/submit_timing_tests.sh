#!/bin/bash

#for ii in { 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 }; 
ii=4
while [ $ii -ne 24 ]
do 
    cp main.fcl main_${ii}.fcl
    echo "services.TimeTracker.dbOutput.filename : \"timing_test_v10_00_00_${ii}.csv\" " >> main_${ii}.fcl    
    taskset -c ${ii} mu2e -c main_${ii}.fcl -s /mu2e/DataFiles/gianipez/v09_11_03/MC_1st_fragmentAndDigi_rootOutput_20210505_fromOTS.art -n 10000 >| timing_${ii}.log 2>&1 &
    ii=$(($ii+1))
done

# ii=4
# while [ $ii -ne 24 ]
# do 
#     taskset -c ${ii} mu2e -c main.fcl -s /mu2e/DataFiles/gianipez/v09_11_03/MC_1st_fragmentAndDigi_rootOutput_20210505_fromOTS.art -n 10000 >| timing_2nd_${ii}.log 2>&1 &
#     ii=$(($ii+1))
# done
