#include <stdio.h>
#include "election.h"
#include <assert.h>
#include <stdlib.h>

struct election_t {
    Map tribe_head;
    Map area_name_id;
    Map tribe_name_id;
};

Election electionCreate() {
    Election election = malloc(sizeof(*election));
    election->tribe_head = mapCreate();
    election->area_name_id = mapCreate();
    election->tribe_name_id = mapCreate();
    return election;
}

void electionDestroy(Election election) {
}

static char *intPointerToString(int *pointer) {
    char *tribe_string_id = malloc(sizeof(pointer));
    sprintf(tribe_string_id, "%p", (void *) pointer);
    return tribe_string_id;
}

static char *mapPointerToString(Map pointer) {
    char *tribe_string_id = malloc(sizeof(pointer));
    sprintf(tribe_string_id, "%p", (void *) pointer);
    return tribe_string_id;
}

ElectionResult electionAddTribe(Election election, int tribe_id, const char *tribe_name) {
    Map map = mapCreate();
    int *p_tribe_id = &tribe_id;
    mapPut(election->tribe_head, intPointerToString(p_tribe_id), mapPointerToString(map));
    mapPut(election->tribe_name_id, intPointerToString(p_tribe_id), tribe_name);
    return ELECTION_SUCCESS;
}

ElectionResult electionAddArea(Election election, int area_id, const char *area_name) {
    return ELECTION_SUCCESS;
}

const char *electionGetTribeName(Election election, int tribe_id) {
    return false;
}

ElectionResult electionAddVote(Election election, int area_id, int tribe_id, int num_of_votes) {
    return ELECTION_SUCCESS;
}

ElectionResult electionRemoveVote(Election election, int area_id, int tribe_id, int num_of_votes) {
    return ELECTION_SUCCESS;
}

ElectionResult electionSetTribeName(Election election, int tribe_id, const char *tribe_name) {
    return ELECTION_SUCCESS;
}

ElectionResult electionRemoveTribe(Election election, int tribe_id) {
    return ELECTION_SUCCESS;
}

ElectionResult electionRemoveAreas(Election election, AreaConditionFunction should_delete_area) {
    return ELECTION_SUCCESS;
}

Map electionComputeAreasToTribesMapping(Election election) {
    return false;
}


