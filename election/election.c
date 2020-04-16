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
    if (election == NULL) {
        return NULL;
    }
    election->tribe_head = mapCreate();
    election->area_name_id = mapCreate();
    election->tribe_name_id = mapCreate();
    if (election->tribe_name_id == NULL||election->tribe_head == NULL||election->area_name_id == NULL){
        return NULL;
    }
    return election;
}

void electionDestroy(Election election) {
}

static char *intPointerToString(int *pointer) {
    char *string = malloc(sizeof(pointer));
    if (string==NULL){
        return NULL;
    }
    sprintf(string, "%p", (void *) pointer);
    return string;
}

static char *mapPointerToString(Map pointer) {
    char *string = malloc(sizeof(pointer));
    if (string==NULL){
        return NULL;
    }
    sprintf(string, "%p", (void *) pointer);
    return string;
}

ElectionResult electionAddTribe(Election election, int tribe_id, const char *tribe_name) {
    if (election == NULL || tribe_name == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    if (tribe_id < 0) {
        return ELECTION_INVALID_ID;
    }
    int *p_tribe_id = &tribe_id;
    char *string_tribe_id = intPointerToString(p_tribe_id);
    if (string_tribe_id==NULL){
        electionDestroy(election);
        return ELECTION_OUT_OF_MEMORY;
    }
    if (mapContains(election->tribe_name_id, string_tribe_id) == true) {
        return ELECTION_TRIBE_ALREADY_EXIST;
    }
    if (mapPut(election->tribe_name_id, string_tribe_id, tribe_name) == MAP_OUT_OF_MEMORY){
        electionDestroy(election);
        return ELECTION_OUT_OF_MEMORY;
    }
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


