CC = gcc
COMP_FLAGS = -std=c99 -pedantic-errors -Wall -Werror
DEBUG_FLAGS = -g -DDG_TESTER
PRODUCTION_FLAGS = -DNDEBUG
RELEASE_TYPE_FLAGS = $(PRODUCTION_FLAGS) 
MAP = map.o
MAP_DIR = mtm_map
ELECTION = election.o
AUG_MAP = augmented_map.o
ELECTION_TESTER = electionTestsExample.o
OBJS = $(MAP) $(ELECTION) $(AUG_MAP)
PROG = election


$(PROG): $(ELECTION_TESTER) $(OBJS)
	$(CC) $(RELEASE_TYPE_FLAGS) $(ELECTION_TESTER) $(OBJS) -o $@

$(ELECTION_TESTER): tests/electionTestsExample.c test_utilities.h election.h
	$(CC) -c $(RELEASE_TYPE_FLAGS) $(COMP_FLAGS) tests/$*.c

$(ELECTION): election.c election.h mtm_map/map.h
	$(CC) -c $(RELEASE_TYPE_FLAGS) $(COMP_FLAGS) $*.c

$(MAP): mtm_map/map.c mtm_map/map.h
	$(CC) -c $(RELEASE_TYPE_FLAGS) $(COMP_FLAGS) $(MAP_DIR)/$*.c

$(AUG_MAP): augmented_map.c augmented_map.h mtm_map/map.h
	$(CC) -c $(RELEASE_TYPE_FLAGS) $(COMP_FLAGS) $*.c

clean:
	rm -f $(OBJS) $(PROG)
