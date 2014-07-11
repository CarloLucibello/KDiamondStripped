#!/bin/sh

#  mb.sh
#  
# Esempio di script bash per salvare i risultati in cartelle diverse 
#  Created by paolo on 06/07/14.
#

./kdiamond-stripped -w mask -v -i 1000 -q .0
mkdir q0
mv simParam.txt ./q0
mv maskres.txt ./q0
mv maskParams.txt ./q0


./kdiamond-stripped -w mask -v -i 1000 -q 1.0
mkdir q100
mv simParam.txt ./q100
mv maskres.txt ./q100
mv maskParams.txt ./q100


