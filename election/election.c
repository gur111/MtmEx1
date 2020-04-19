#include "augmented_map.h"
#include "election.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// Supports upto 64 bit. On 99% of systems it will be 11 inc \0
#define MAX_PTR_AS_STR_SIZE 20
// Supports up to 64 bit integers
#define MAX_INT_AS_STR_SIZE 22

struct election_t {
    AugMap area_head;
    AugMap area_name_id;
    AugMap tribe_name_id;
};

Election electionCreate() {
    Election election = malloc(sizeof(*election));
    if (election == NULL) {
        return NULL;
    }
    election->area_head->type = MAP;
    election->area_head->map = mapCreate(MAP);
    election->area_name_id->type = STR;
    election->area_name_id->map = mapCreate(STR);
    election->tribe_name_id->type = STR;
    election->tribe_name_id->map = mapCreate(STR);
    if (election->tribe_name_id == NULL || election->area_head == NULL ||
        election->area_name_id == NULL) {
        return NULL;
    }
    return election;
}

void electionDestroy(Election election) {
    mapDestroy(election->area_head);
    mapDestroy(election->area_name_id);
    mapDestroy(election->tribe_name_id);
    free(election);
}

static bool isLowerCase(const char *string) {
    const unsigned int length = strlen(string);
    for (unsigned int i = 0; i < length; i++) {
        if ((string[i] < 'a' || string[i] > 'z') && string[i] != ' ') {
            return false;
        }
    }
    return true;
}

ElectionResult electionAddTribe(Election election, int tribe_id,
                                const char *tribe_name) {
    // Verify arguments
    if (election == NULL || tribe_name == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    if (tribe_id < 0) {
        return ELECTION_INVALID_ID;
    }
    if (mapContains(election->tribe_name_id, tribe_id) == true) {
        return ELECTION_TRIBE_ALREADY_EXIST;
    }
    if (isLowerCase(tribe_name) == false) {
        return ELECTION_INVALID_NAME;
    }
    if (mapPut(election->tribe_name_id, tribe_id, tribe_name) == MAP_OUT_OF_MEMORY) {
        return ELECTION_OUT_OF_MEMORY;
    }
    return ELECTION_SUCCESS;
}

ElectionResult electionAddArea(Election election, int area_id,
                               const char *area_name) {
    if (election == NULL || area_name == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    if (area_id < 0) {
        return ELECTION_INVALID_ID;
    }
    if (mapContains(election->area_name_id, area_id) == true) {
        return ELECTION_AREA_ALREADY_EXIST;
    }
    if (isLowerCase(area_name) == false) {
        return ELECTION_INVALID_NAME;
    }
    if (mapPut(election->area_name_id, area_id, area_name) == MAP_OUT_OF_MEMORY) {
        return ELECTION_OUT_OF_MEMORY;
    }
    AugMap votes_Tribe_Map = mapCreate(INT);
    if (votes_Tribe_Map == NULL) {
        return ELECTION_OUT_OF_MEMORY;
    }
    if (mapPut(election->area_head, area_id, votes_Tribe_Map) == MAP_OUT_OF_MEMORY) {
        return ELECTION_OUT_OF_MEMORY;
    }
    return ELECTION_SUCCESS;
}

char *electionGetTribeName(Election election, int tribe_id) {
    if (election == NULL) {
        return NULL;
    }
    if (mapContains(election->tribe_name_id, tribe_id) == false) {
        return NULL;
    }
    return mapGetStr(election->tribe_name_id, tribe_id);
}

ElectionResult electionSetTribeName(Election election, int tribe_id,
                                    const char *tribe_name) {
    if (election == NULL || tribe_name == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    if (tribe_id < 0) {
        return ELECTION_INVALID_ID;
    }
    if (mapContains(election->tribe_name_id, tribe_id) == false) {
        return ELECTION_TRIBE_NOT_EXIST;
    }
    if (isLowerCase(tribe_name) == false) {
        return ELECTION_INVALID_NAME;
    }
    if (mapPut(election->tribe_name_id, tribe_id, tribe_name) == MAP_OUT_OF_MEMORY) {
        return ELECTION_OUT_OF_MEMORY;
    }
    return ELECTION_SUCCESS;
}

ElectionResult electionRemoveTribe(Election election, int tribe_id) {
    if (election == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    if (tribe_id < 0) {
        return ELECTION_INVALID_ID;
    }
    if (mapContains(election->tribe_name_id, tribe_id) == false) {
        return ELECTION_TRIBE_NOT_EXIST;
    }
    mapRemove(election->tribe_name_id, tribe_id);
    //todo:check if need to return ELECTION_OUT_OF_MEMORY
    return ELECTION_SUCCESS;
}

ElectionResult electionRemoveAreas(Election election,
                                   AreaConditionFunction should_delete_area) {
    return ELECTION_SUCCESS;
}

ElectionResult electionAddVote(Election election, int area_id, int tribe_id,
                               int num_of_votes) {

    return ELECTION_SUCCESS;
}

ElectionResult electionRemoveVote(Election election, int area_id, int tribe_id,
                                  int num_of_votes) {
    return ELECTION_SUCCESS;
}

Map electionComputeAreasToTribesMapping(Election election) { return false; }
