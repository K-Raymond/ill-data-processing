#!/bin/bash
#usage ProcessLstFiles.sh <daq time> <list_of_run_file>
echo "Going to read data from $DATA_DIR"
mkdir -pv rootfiles
minimumsize=1050000000
if [ -e "scripts/c2root.sh" ];
then
echo " running scripts/c2root.sh script "
else
echo " scripts/c2root.sh script not found "
exit
fi

function processLst {
    echo "Running: $filename"
    filename=$1

    if [ -f "${DATA_DIR}/${filename}.lst" ];
    then
    actualsize=$(wc -c < "${DATA_DIR}/${filename}.lst")
    if [ -f "./rootfiles/${filename}.root" ];
    then 
      echo "./rootfiles/${filename}.root already existing"
    else 
      if [ $actualsize -le $minimumsize ];
      then
        echo " ${DATA_DIR}/${filename}.lst size is $actualsize " >> discardedfilesiEB.txt
      else
        echo "Processing run # ${filename}"
        ./scripts/c2root.sh ${DATA_DIR}/${filename}.lst > logs/log_lst2root_${filename}.txt
      fi
    fi
    else
    echo " ${DATA_DIR}/${filename}.lst not existing "
    fi

    return 0
}
export -f processLst
echo "Begin Parallel processing"
# run in parallel making sure not to overload the system
cat $1 | parallel --eta --bar --load 80% --noswap --progress processLst
