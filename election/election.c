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
        // TODO: Free whatever didn't fail
        return NULL;
    }
    return election;
}

void electionDestroy(Election election) {
    if(election != NULL){
        augMapDestroy(election->votes_by_area);
        augMapDestroy(election->areas);
        augMapDestroy(election->tribes);
        free(election);
    }
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

typedef ElectionResult (*ConvertFunc)(AugMapResult);

static ElectionResult augMapResultToElectionResultTribe(AugMapResult result) {
    assert(result != AUG_MAP_ERROR);
    assert(result != AUG_MAP_TOO_MANY_LAYERS);
    assert(result != AUG_MAP_INVALID_TYPE);

    switch (result) {
        case AUG_MAP_SUCCESS:
            return ELECTION_SUCCESS;
        case AUG_MAP_OUT_OF_MEMORY:
            return ELECTION_OUT_OF_MEMORY;
        case AUG_MAP_NULL_ARGUMENT:
            return ELECTION_NULL_ARGUMENT;
        case AUG_MAP_ITEM_ALREADY_EXISTS:
            return ELECTION_TRIBE_ALREADY_EXIST;
        case AUG_MAP_ITEM_DOES_NOT_EXIST:
            return ELECTION_TRIBE_NOT_EXIST;
        default:
            assert(false);
            return ELECTION_SUCCESS;
    }
}

static ElectionResult augMapResultToElectionResultArea(AugMapResult result) {
    assert(result != AUG_MAP_ERROR);
    assert(result != AUG_MAP_TOO_MANY_LAYERS);
    assert(result != AUG_MAP_INVALID_TYPE);

    switch (result) {
        case AUG_MAP_SUCCESS:
            return ELECTION_SUCCESS;
        case AUG_MAP_OUT_OF_MEMORY:
            return ELECTION_OUT_OF_MEMORY;
        case AUG_MAP_NULL_ARGUMENT:
            return ELECTION_NULL_ARGUMENT;
        case AUG_MAP_ITEM_ALREADY_EXISTS:
            return ELECTION_AREA_ALREADY_EXIST;
        case AUG_MAP_ITEM_DOES_NOT_EXIST:
            return ELECTION_AREA_NOT_EXIST;
        default:
            assert(false);
            return ELECTION_SUCCESS;
    }
}

static ElectionResult electionSetNameLocal(
        AugMap aug_map, int element_id, const char *name, bool should_exist,
        ConvertFunc augMapResultToElectionResult) {
    // Verify arguments
    assert(aug_map != NULL);
    assert(augMapGetType(aug_map) == STR_TYPE);
    if (name == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    if (element_id < 0) {
        return ELECTION_INVALID_ID;
    }

    bool is_present;
    AugMapResult status = augMapContains(aug_map, element_id, &is_present);
    if (status != AUG_MAP_SUCCESS) {
        return augMapResultToElectionResult(status);
    }

    if (is_present != should_exist) {
        return should_exist
               ? augMapResultToElectionResult(AUG_MAP_ITEM_DOES_NOT_EXIST)
               : augMapResultToElectionResult(AUG_MAP_ITEM_ALREADY_EXISTS);
    }

    if (isLowerCase(name) == false) {
        return ELECTION_INVALID_NAME;
    }
    AugMapResult res = augMapPutStr(aug_map, element_id, name);
    assert(res != AUG_MAP_INVALID_TYPE);
    return augMapResultToElectionResult(res);
}

ElectionResult electionAddTribe(Election election, int tribe_id,
                                const char *tribe_name) {
    if (election == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }

    return electionSetNameLocal(election->tribes, tribe_id, tribe_name, false,
                                augMapResultToElectionResultTribe);
}

ElectionResult electionAddArea(Election election, int area_id,
                               const char *area_name) {
    if (election == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }

    ElectionResult result =
            electionSetNameLocal(election->areas, area_id, area_name, false,
                                 augMapResultToElectionResultArea);
    if (result != ELECTION_SUCCESS) {
        return result;
    }
    AugMap votes_tribe_map = augMapCreate(INT_TYPE);
    if (votes_tribe_map == NULL) {
        AugMapResult res = augMapRemove(election->areas, area_id);
        assert(res == AUG_MAP_SUCCESS);
        return ELECTION_OUT_OF_MEMORY;
    }
    AugMapResult status =
            augMapPutMap(election->votes_by_area, area_id, votes_tribe_map);
    assert(status != AUG_MAP_NULL_ARGUMENT);
    if (status == AUG_MAP_OUT_OF_MEMORY) {
        augMapDestroy(votes_tribe_map);
        AugMapResult res = augMapRemove(election->areas, area_id);
        assert(res == AUG_MAP_SUCCESS);
        return augMapResultToElectionResultArea(status);
    }
    return augMapResultToElectionResultArea(status);
}

char *electionGetTribeName(Election election, int tribe_id) {
    if (election == NULL) {
        return NULL;
    }
    assert(augMapGetType(election->tribes) == STR_TYPE);
    char *result;
    AugMapResult status = augMapGetStr(election->tribes, tribe_id, &result);
    if (status != AUG_MAP_SUCCESS) {
        return NULL;
    }
    return result;
}

ElectionResult electionSetTribeName(Election election, int tribe_id,
                                    const char *tribe_name) {
    if (election == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    return electionSetNameLocal(election->tribes, tribe_id, tribe_name, true,
                                augMapResultToElectionResultTribe);
}

ElectionResult electionRemoveTribe(Election election, int tribe_id) {
    if (election == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    if (tribe_id < 0) {
        return ELECTION_INVALID_ID;
    }
    AugMapResult status = augMapRemove(election->tribes, tribe_id);
    if (status == AUG_MAP_ITEM_DOES_NOT_EXIST) {
        return augMapResultToElectionResultTribe(status);
    }
    AUG_MAP_FOREACH(area_key, election->votes_by_area) {
        AugMap area_map;
        status = augMapGetMap(election->votes_by_area, area_key, &area_map);
        if (status != AUG_MAP_SUCCESS) {
            return augMapResultToElectionResultTribe(status);
        }
        status = augMapRemove(area_map, tribe_id);
        assert(status != AUG_MAP_NULL_ARGUMENT);
        //dont need to check if augMapRemove returned AUG_MAP_ITEM_DOES_NOT_EXIST, because we already checking it in "augMapRemove(election->tribes, tribe_id)"
    }
    return ELECTION_SUCCESS;
}

ElectionResult electionRemoveAreas(Election election,
                                   AreaConditionFunction should_delete_area) {
    if (election == NULL || should_delete_area == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    int size = augMapGetSize(election->areas);
    int *candidate_array = (int *) malloc(sizeof(int) * size);
    if (candidate_array == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    for (int i = 0; i < size; i++) {
        candidate_array[i] = -1;
    }

    int j = 0;
    AUG_MAP_FOREACH(key, election->areas) {
        if (should_delete_area(key)) {
            candidate_array[j] = key;
            j++;
        }
    }
    for (int i = 0; i < size; i++) {
        if (candidate_array[i] == -1) {
            free(candidate_array);
            return ELECTION_SUCCESS;
        }
        AugMapResult status = augMapRemove(election->areas, candidate_array[i]);
        assert(status == AUG_MAP_SUCCESS);
        status = augMapRemove(election->votes_by_area, candidate_array[i]);
        assert(status == AUG_MAP_SUCCESS);
    }
    free(candidate_array);
    return ELECTION_SUCCESS;
}

static ElectionResult electionManageVotes(Election election, int area_id,
                                          int tribe_id, int num_of_votes,
                                          bool add_votes) {
    if (election == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    if (area_id < 0 || tribe_id < 0) {
        return ELECTION_INVALID_ID;
    }
    if (num_of_votes <= 0) {
        return ELECTION_INVALID_VOTES;
    }
    bool res;
    AugMapResult status = augMapContains(election->tribes, tribe_id, &res);
    assert(status != AUG_MAP_OUT_OF_MEMORY);
    if (res == false) {
        return ELECTION_TRIBE_NOT_EXIST;
    }
    AugMap temp_map;
    status = augMapGetMap(election->votes_by_area, area_id, &temp_map);
    assert(status != AUG_MAP_OUT_OF_MEMORY);
    if (status != AUG_MAP_SUCCESS) {
        return augMapResultToElectionResultArea(status);
    }
    assert(temp_map != NULL);
    int votes = 0;
    status = augMapGetInt(temp_map, tribe_id, &votes);
    assert(status != AUG_MAP_OUT_OF_MEMORY);
    if (status != AUG_MAP_SUCCESS) {
        if (status == AUG_MAP_ITEM_DOES_NOT_EXIST) {
            votes = 0;
        } else {
            return augMapResultToElectionResultTribe(status);
        }
    }
    if (add_votes == true) {
        status = augMapPutInt(temp_map, tribe_id, votes + num_of_votes);
        if (status != AUG_MAP_SUCCESS) {
            return augMapResultToElectionResultTribe(status);
        }
    } else {
        if (votes - num_of_votes < 0) {
            votes = 0;
        }
        status = augMapPutInt(temp_map, tribe_id, votes);
        if (status != AUG_MAP_SUCCESS) {
            return augMapResultToElectionResultTribe(status);
        }
    }

    return ELECTION_SUCCESS;
}

ElectionResult electionAddVote(Election election, int area_id, int tribe_id,
                               int num_of_votes) {
    return electionManageVotes(election, area_id, tribe_id, num_of_votes, true);
}

ElectionResult electionRemoveVote(Election election, int area_id, int tribe_id,
                                  int num_of_votes) {
    return electionManageVotes(election, area_id, tribe_id, num_of_votes,
                               false);
}

Map electionComputeAreasToTribesMapping(Election election) {
    AugMap map = augMapCreate(INT_TYPE);
    if (map == NULL || election == NULL) {
        return NULL;
    }
    AUG_MAP_FOREACH(area, election->votes_by_area) {
        bool is_map_empty = true;
        int wining_votes = 0;
        int wining_tribe_id = 0;
        AugMap tribe_votes_map;
        AugMapResult status =
                augMapGetMap(election->votes_by_area, area, &tribe_votes_map);
        if (status != AUG_MAP_SUCCESS) {
            return NULL;
        }
        AUG_MAP_FOREACH(tribe, tribe_votes_map) {
            int t_votes;
            status = augMapGetInt(tribe_votes_map, area, &t_votes);
            if (status != AUG_MAP_SUCCESS) {
                return NULL;
            }
            if (is_map_empty) {
                is_map_empty = false;
                wining_votes = t_votes;
                wining_tribe_id = tribe;
            }
            if (wining_tribe_id > tribe && wining_votes == t_votes) {
                wining_tribe_id = tribe;
            }
            if (t_votes > wining_tribe_id) {
                wining_tribe_id = tribe;
                wining_votes = t_votes;
            }
        }
        status = augMapPutInt(map, area, wining_tribe_id);
        if (status != AUG_MAP_SUCCESS) {
            return NULL;
        }
    }
    return augMapConvertToMap(map);
}
