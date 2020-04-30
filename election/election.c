#include "election.h"

#include <assert.h>
#include <stdlib.h>

#ifndef NDEBUG

#include "../MtmEx1Tester/utils.h"
// Allow malloc to be unstable
#define malloc xmalloc
#endif

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
    //creating sub maps
    election->votes_by_area = augMapCreate(MAP_TYPE);
    election->areas = augMapCreate(STR_TYPE);
    election->tribes = augMapCreate(STR_TYPE);
    if (election->tribes == NULL || election->votes_by_area == NULL ||
        election->areas == NULL) {
        electionDestroy(election);
        return NULL;
    }
    return election;
}

void electionDestroy(Election election) {
    if (election != NULL) {
        augMapDestroy(election->votes_by_area);
        augMapDestroy(election->areas);
        augMapDestroy(election->tribes);
        free(election);
    }
}
/**
 * isLowerCase - checks if the string consist of lower case characters and space key
 * @param string  - the string that needs to be checked
 * @return
 *      true - in case the string consist of lower case characters and space key
 *      false - if the string has invalid characters
 */
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
/**
 * augMapResultToElectionResultTribe - converts the eNUM from the augmented_map.h to ElectionResult -
 * specific conversion to tribe Enums
 * @param result - a eNum of AugMapResult - only eNums that exists in election.h are valid
 * @return
 *      eNuns ElectionResult
 */
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
            //shouldn`t get to here
            assert(false);
            return ELECTION_SUCCESS;
    }
}
/**
 * augMapResultToElectionResultArea - converts the eNUM from the
 *                                  augmented_map.h to ElectionResult - specific conversion to area Enums
 * @param result - a eNum of AugMapResult - only eNums that exists in election.h are valid
 * @return
 *      eNuns ElectionResult
 */
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
            //shouldn`t get to here
            assert(false);
            return ELECTION_SUCCESS;
    }
}
/**
 * electionSetName - inserts or updates the name of tribe or area. validate the inputs of the input.
 * @param aug_map - the tribe or the area map
 * @param element_id - the tribe id or the area id - cant be negative
 * @param name - the new name of the area or tribe
 * @param should_exist - indicates if the user wants to change the name of the tribe or just enter a new tribe to the
 *                      election
 * @param augMapResultToElectionResult - the function that converts the eNum
 * @return
 *      ELECTION_NULL_ARGUMENT - in case there`s a null argument
 *      ELECTION_INVALID_ID - if the id negative
 *      ELECTION_TRIBE_DOSE_NOT_EXIST - in case the user wants to update the name of the tribe, but it dosen`t exist
 *      ELECTION_INVALID_NAME - invalid char in the name string
 *      ELECTION_SUCCESS - if the name was successfully inserted
 */
static ElectionResult electionSetName(
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
    //checks if the tribe/area id already exits in election
    AugMapResult status = augMapContains(aug_map, element_id, &is_present);
    if (status != AUG_MAP_SUCCESS) {
        return augMapResultToElectionResult(status);
    }
    //checks if the user wants to update the tribe name but the tribe doesnt exists
    if (is_present != should_exist) {
        return should_exist
               ? augMapResultToElectionResult(AUG_MAP_ITEM_DOES_NOT_EXIST)
               : augMapResultToElectionResult(AUG_MAP_ITEM_ALREADY_EXISTS);
    }

    if (!isLowerCase(name)) {
        return ELECTION_INVALID_NAME;
    }
    //inserts the new name to and id to election / or just updates the value of the key
    AugMapResult res = augMapPutStr(aug_map, element_id, name);
    assert(res != AUG_MAP_INVALID_TYPE);
    return augMapResultToElectionResult(res);
}

ElectionResult electionAddTribe(Election election, int tribe_id,
                                const char *tribe_name) {
    if (election == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    //inserting the new tribe to election->tribe, using the electionSetName function
    return electionSetName(election->tribes, tribe_id, tribe_name, false,
                           augMapResultToElectionResultTribe);
}

ElectionResult electionAddArea(Election election, int area_id,
                               const char *area_name) {
    if (election == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    //adding the area_id and its name to election->areas
    ElectionResult result =
            electionSetName(election->areas, area_id, area_name, false,
                            augMapResultToElectionResultArea);
    if (result != ELECTION_SUCCESS) {
        return result;
    }
    //creating the map the that would store the votes to the tribes - in this  area.
    AugMap votes_tribe_map = augMapCreate(INT_TYPE);
    if (votes_tribe_map == NULL) {
        //memory problem
        AugMapResult res = augMapRemove(election->areas, area_id);
        assert(res == AUG_MAP_SUCCESS);
        return ELECTION_OUT_OF_MEMORY;
    }
    AugMapResult status =
            augMapPutMap(election->votes_by_area, area_id, votes_tribe_map);
    assert(status == AUG_MAP_OUT_OF_MEMORY || status == AUG_MAP_SUCCESS);

    if (status == AUG_MAP_OUT_OF_MEMORY) {
        augMapDestroy(votes_tribe_map);
        AugMapResult res = augMapRemove(election->areas, area_id);
        assert(res == AUG_MAP_SUCCESS);
        return ELECTION_OUT_OF_MEMORY;
    }
    return augMapResultToElectionResultArea(status);
}

char *electionGetTribeName(Election election, int tribe_id) {
    if (election == NULL) {
        return NULL;
    }
    char *result;
    //getting the value of the tribe id from election->tribes
    AugMapResult status = augMapGetStr(election->tribes, tribe_id, &result);
    assert(status == AUG_MAP_OUT_OF_MEMORY || status == AUG_MAP_SUCCESS || status == AUG_MAP_ITEM_DOES_NOT_EXIST);
    assert(status !=AUG_MAP_NULL_ARGUMENT && status != AUG_MAP_INVALID_KEY);
    if (status != AUG_MAP_SUCCESS) {
        //if there were any problems in the input or in the function need to return null
        return NULL;
    }
    return result;
}

ElectionResult electionSetTribeName(Election election, int tribe_id,
                                    const char *tribe_name) {
    if (election == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    // setting the new name of the tribe using electionSetName function
    return electionSetName(election->tribes, tribe_id, tribe_name, true,
                           augMapResultToElectionResultTribe);
}

ElectionResult electionRemoveTribe(Election election, int tribe_id) {
    //validating the arguments
    if (election == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    if (tribe_id < 0) {
        return ELECTION_INVALID_ID;
    }
    AugMapResult status = augMapRemove(election->tribes, tribe_id);
    if (status != AUG_MAP_SUCCESS) {
        return augMapResultToElectionResultTribe(status);
    }
    AUG_MAP_FOREACH(area_key, election->votes_by_area) {
        AugMap area_votes;
        status = augMapGetMap(election->votes_by_area, area_key, &area_votes);
        //CANT be Null because we already checked it
        //AUG_MAP_ITEM_DOES_NOT_EXIST also isn`t a possibility because  AUG_MAP_FOREACH gives us the key form areas
        assert(status == AUG_MAP_SUCCESS);
        status = augMapRemove(area_votes, tribe_id);
        assert(status != AUG_MAP_NULL_ARGUMENT);
        // dont need to check if augMapRemove returned
        // AUG_MAP_ITEM_DOES_NOT_EXIST, because we already checking it in
        // "augMapRemove(election->tribes, tribe_id)"
    }
    return ELECTION_SUCCESS;
}

ElectionResult electionRemoveAreas(Election election,
                                   AreaConditionFunction should_delete_area) {
    if (election == NULL || should_delete_area == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }

    for (bool was_deleted = true; was_deleted;) {
        was_deleted = false;

        AUG_MAP_FOREACH(key, election->areas) {
            if (should_delete_area(key)) {
                AugMapResult status = augMapRemove(election->areas, key);
                assert(status == AUG_MAP_SUCCESS);
                status = augMapRemove(election->votes_by_area, key);
                assert(status == AUG_MAP_SUCCESS);
                was_deleted = true;
                break;  // To break the foreach loop
            }
        }
    }

    return ELECTION_SUCCESS;
}

static inline int max(int first, int second) {
    if (first >= second) {
        return first;
    } else {
        return second;
    }
}

static ElectionResult electionChangeVotes(Election election, int area_id,
                                          int tribe_id, int num_of_votes) {
    if (election == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    if (area_id < 0 || tribe_id < 0) {
        return ELECTION_INVALID_ID;
    }
    bool res;
    AugMapResult status = augMapContains(election->tribes, tribe_id, &res);
    assert(status == AUG_MAP_SUCCESS || status == AUG_MAP_ITEM_DOES_NOT_EXIST);
    if (res == false) {
        return ELECTION_TRIBE_NOT_EXIST;
    }
    AugMap area_votes;
    status = augMapGetMap(election->votes_by_area, area_id, &area_votes);
    assert(status != AUG_MAP_OUT_OF_MEMORY);
    if (status != AUG_MAP_SUCCESS) {
        return augMapResultToElectionResultArea(status);
    }
    assert(area_votes != NULL);
    int votes = 0;
    status = augMapGetInt(area_votes, tribe_id, &votes);
    assert(status == AUG_MAP_SUCCESS || status == AUG_MAP_ITEM_DOES_NOT_EXIST);
    if (status == AUG_MAP_ITEM_DOES_NOT_EXIST) {
        votes = 0;
    }
    votes = votes + num_of_votes;
    status = augMapPutInt(area_votes, tribe_id, max(0, votes));
    return augMapResultToElectionResultTribe(status);
}

ElectionResult electionAddVote(Election election, int area_id, int tribe_id,
                               int num_of_votes) {
    if (num_of_votes <= 0) {
        return ELECTION_INVALID_VOTES;
    }
    return electionChangeVotes(election, area_id, tribe_id, num_of_votes);
}

ElectionResult electionRemoveVote(Election election, int area_id, int tribe_id,
                                  int num_of_votes) {
    if (num_of_votes <= 0) {
        return ELECTION_INVALID_VOTES;
    }
    return electionChangeVotes(election, area_id, tribe_id, -num_of_votes);
}

Map electionComputeAreasToTribesMapping(Election election) {
    AugMap results = augMapCreate(INT_TYPE);
    if (results == NULL || election == NULL) {
        return NULL;
    }
    AUG_MAP_FOREACH(area, election->votes_by_area) {
        bool is_map_empty = true;
        int max_votes = 0;
        int wining_tribe_id = 0;
        AugMap tribe_votes_map;
        AugMapResult status =
                augMapGetMap(election->votes_by_area, area, &tribe_votes_map);
        if (status != AUG_MAP_SUCCESS) {
            return NULL;
        }
        AUG_MAP_FOREACH(area_key, tribe_votes_map) {
            int current_tribe_vote;
            status = augMapGetInt(tribe_votes_map, area, &current_tribe_vote);
            if (status != AUG_MAP_SUCCESS) {
                return NULL;
            }
            if (is_map_empty) {
                is_map_empty = false;
                max_votes = current_tribe_vote;
                wining_tribe_id = current_tribe_vote;
            }
            if (wining_tribe_id > current_tribe_vote && max_votes == current_tribe_vote) {
                wining_tribe_id = current_tribe_vote;
            }
            if (current_tribe_vote > wining_tribe_id) {
                wining_tribe_id = current_tribe_vote;
                max_votes = current_tribe_vote;
            }
        }
        status = augMapPutInt(results, area, wining_tribe_id);
        if (status != AUG_MAP_SUCCESS) {
            return NULL;
        }
    }
    return augMapConvertToMap(results);
}
