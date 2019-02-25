## Sorting Scripts

This folder provides basic scripts in aiding sorting `.lst` files. Both scripts should be moved to the `processed/scripts` folder of the analysis directory.

### Parallel Sorter

To run the parallel sorter,

`DATA_DIR=/place/of_lst ./scripts/ProcessLstFilesParallel.sh ./txt/file/containing/runs.txt`

root files will be exported to `./rootfiles/`.

### lst2root

This is a slightly modified `c2root.sh` taken from the original experiment format. The settings have been updated to be relevent to 118Sn data.

The location of lst2root128 needs to be set in the `c2root.sh` script.
