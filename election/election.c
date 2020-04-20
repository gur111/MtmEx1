#include "election.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "augmented_map.h"

struct election_t {
    AugMap votes_by_area;
    AugMap areas;
    AugMap tribes;
};

typedef enum {
    INNER_SUCESS,
    INNER_ALREADY_EXIST,
    INNER_DOES_NOT_EXIST,
    INNER_INVALID_NAME
} InnerStatus;

Election electionCreate() {
    Election election = malloc(sizeof(*election));
    if (election == NULL) {
        return NULL;
    }
    election->votes_by_area = augMapCreate(MAP_TYPE);
    election->areas = augMapCreate(STR_TYPE);
    election->tribes = augMapCreate(STR_TYPE);
    if (election->tribes == NULL || election->votes_by_area == NULL ||
        election->areas == NULL) {
        free(election);
        return NULL;
    }
    return election;
}

void electionDestroy(Election election) {
    augMapDestroy(election->votes_by_area);
    augMapDestroy(election->areas);
    augMapDestroy(election->tribes);
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

ElectionResult augMapResultToElectionResult(AugMapResult result) {
    // TODO:
    return ELECTION_SUCCESS;
}

static ElectionResult electionSetTribeNameLocal(AugMap tribes, int tribe_id,
                                                char *name, bool should_exist) {
    // Verify arguments
    assert(tribes != NULL);
    assert(augMapGetType(tribes) == STR_TYPE);
    if (name == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    if (tribe_id < 0) {
        return ELECTION_INVALID_ID;
    }

    bool is_present;
    AugMapResult status = augMapContains(tribes, tribe_id, &is_present);
    if (status != AUG_MAP_SUCCESS) {
        return augMapResultToElectionResult(status);
    }
    if (is_present != should_exist) {
        return should_exist ? ELECTION_TRIBE_NOT_EXIST
                            : ELECTION_TRIBE_ALREADY_EXIST;
    }
    if (isLowerCase(name) == false) {
        return ELECTION_INVALID_NAME;
    }
    return augMapResultToElectionResult(augMapPutStr(tribes, tribe_id, name));
}

// Yes, it's duplicated code due to the different error code
static ElectionResult electionSetAreaNameLocal(AugMap areas, int area_id,
                                               char *name, bool should_exist) {
    assert(areas != NULL);
    if (name == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    if (area_id < 0) {
        return ELECTION_INVALID_ID;
    }
    bool is_present;
    AugMapResult status = augMapContains(areas, area_id, &is_present);
    if (status != AUG_MAP_SUCCESS) {
        return augMapResultToElectionResult(status);
    }
    if (is_present != should_exist) {
        return should_exist ? ELECTION_AREA_NOT_EXIST
                            : ELECTION_AREA_ALREADY_EXIST;
    }
    if (isLowerCase(name) == false) {
        return ELECTION_INVALID_NAME;
    }
    return augMapResultToElectionResult(augMapPutStr(areas, area_id, name));
}

ElectionResult electionAddTribe(Election election, int tribe_id,
                                const char *tribe_name) {
    if (election == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }

    return electionSetTribeNameLocal(election->tribes, tribe_id, tribe_name,
                                     false);
}

ElectionResult electionAddArea(Election election, int area_id,
                               const char *area_name) {
    if (election == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }

    ElectionResult result =
        electionSetAreaNameLocal(election->areas, area_id, area_name, false);

    AugMap votes_tribe_map = augMapCreate(INT_TYPE);
    if (votes_tribe_map == NULL) {
        // TODO: Free added area
        return ELECTION_OUT_OF_MEMORY;
    }
    AugMapResult status =
        augMapPutMap(election->votes_by_area, area_id, votes_tribe_map);
    if (status != AUG_MAP_SUCCESS) {
        // TODO: Free added area
        return augMapResultToElectionResult(status);
    }
    return ELECTION_SUCCESS;
}

static char *electionGetComponentName(AugMap component_map, int id) {
    if (component_map == NULL) {
        return NULL;
    }
    assert(augMapGetType(component_map) == STR_TYPE);
    char *result;
    AugMapResult status = augMapGetStr(component_map, id, &result);
    if (status != AUG_MAP_SUCCESS) {
        return NULL;
    }
    return result;
}

char *electionGetTribeName(Election election, int tribe_id) {
    return electionGetComponentName(election->tribes, tribe_id);
}

// I know it's not needed but it's so fitting
char *electionGetAreaName(Election election, int area_id) {
    return electionGetComponentName(election->areas, area_id);
}

ElectionResult electionSetTribeName(Election election, int tribe_id,
                                    const char *tribe_name) {
    if (election == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    return electionSetTribeNameLocal(election->tribes, tribe_id, tribe_name,
                                     true);
}

ElectionResult electionRemoveTribe(Election election, int tribe_id) {
    if (election == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    if (tribe_id < 0) {
        return ELECTION_INVALID_ID;
    }
    bool is_present;
    AugMapResult status = augMapRemove(election->tribes, tribe_id);
    if (status != AUG_MAP_SUCCESS) {
        return augMapResultToElectionResult(status);
    }
    // TODO: Remove tribe from all areas
    // todo:check if need to return ELECTION_OUT_OF_MEMORY
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
