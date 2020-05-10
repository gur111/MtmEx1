
set -e
zip -r tmp_submission.zip makefile dry.pdf augmented_map.c augmented_map.h election.c mtm_map/map.c test_utils.h
# scp $csl:/home/mtm/public/1920b/ex1/finalCheck ./
if ~mtm/public/1920b/ex1/finalCheck tmp_submission.zip | grep -e "^" -e "Congratulations! Final check passed." ; then
	mv tmp_submission.zip Submission_passed.zip
	echo "finalCheck Passed Successfully!"
else
	~mtm/public/1920b/ex1/finalCheck
	rm tmp_submission.zip
	echo "finalCheck Failed!"
fi
