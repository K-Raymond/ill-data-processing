#!/bin/bash
#script usage: ./c2root.sh <filename>.lst <multiplicity>
#usage : <file> <coincidence dT> <cleanDT> <mult> <detectoradc.txt> <XPConfig.txt> [blockSize=8192] 
#[outputDirectory=.] [isVerbose=false] [isSingle=false]  
mkdir -pv rootfiles
PROGR=/home/kraymond/data_analysis/lst-data-process-128/build/bin/lst2root128
INFILE=$1
COINCDT=4000
CLEANDT=100
MULT=1
DETECTOR_FILE=configs/detectoradc.txt
CONFIG_FILE=configs/XPConfig.txt
BLOCKSIZE=67108864
OUTDIR=./rootfiles
isVerbose=true
isSingle=false

${PROGR} ${INFILE} ${COINCDT} ${CLEANDT} ${MULT} ${DETECTOR_FILE} ${CONFIG_FILE} ${BLOCKSIZE} ${OUTDIR} ${isVerbose} ${isSingle}
#lst2root128 001150.lst 30 40 1 Configs/detectoradc.txt Configs/XPConfig.txt 8192 RootFiles false false
