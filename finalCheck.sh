
set -e
zip -r tmp_submission.zip makefile dry.pdf augmented_map.c augmented_map.h election.c mtm_map/map.c test_utils.h
# scp $csl:/home/mtm/public/1920b/ex1/finalCheck ./
~mtm/public/1920b/ex1/finalCheck tmp_submission.zip
rm tmp_submission.zip
