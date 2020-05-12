CC = gcc
COMP_FLAGS = -std=c99 -pedantic-errors -Wall
DEBUG_FLAGS = -g
PRODUCTION_FLAGS = -DNDEBUG
RELEASE_TYPE_FLAGS = $(DEBUG_FLAGS)
MAP = map.o
MAP_DIR = mtm_map
ELECTION = election.o
AUG_MAP = augmented_map.o
ELECTION_TESTER = electionTestsExample.o
OBJS = $(MAP) $(ELECTION) $(AUG_MAP) $(ELECTION_TESTER)
PROG = election


$(PROG): $(ELECTION_TESTER)
	$(CC) $(RELEASE_TYPE_FLAGS) $@

$(ELECTION_TESTER): electionTestsExample.c test_utilities.h election.h $(OBJS)
	$(CC) $(RELEASE_TYPE_FLAGS) $(COMP_FLAGS) $*.c

$(MAP): $(MAP_DIR)/map.c $(MAP_DIR)/map.h
	$(CC) -c $(RELEASE_TYPE_FLAGS) $(COMP_FLAGS) $(MAP_DIR)/$*.c

$(ELECTION): $*.c $*.h $(AUG_MAP) $(MAP)
	$(CC) -c $(RELEASE_TYPE_FLAGS) $(COMP_FLAGS) $*.c

$(AUG_MAP): $*.c $*.h $(MAP)
	$(CC) -c $(RELEASE_TYPE_FLAGS) $(COMP_FLAGS) $*.c


clean:
	rm -f *.o $(OBJS) $(PROG)
