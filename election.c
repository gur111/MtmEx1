#include "election.h"

#include <assert.h>
#include <stdlib.h>

#ifdef DG_TESTER

#include "MtmEx1Tester/utils.h"
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
    // creating sub maps
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
 * isLowerCase - checks if the string consist of lower case characters and space
 * key
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
 * augMapResultToElectionResultTribe - converts the eNUM from the
 * augmented_map.h to ElectionResult - specific conversion to tribe Enums
 * @param result - a eNum of AugMapResult - only eNums that exists in election.h
 * are valid
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
            // shouldn`t get to here
            assert(false);
            return ELECTION_SUCCESS;
    }
}

/**
 * augMapResultToElectionResultArea - converts the eNUM from the
 *                                  augmented_map.h to ElectionResult - specific
 * conversion to area Enums
 * @param result - a eNum of AugMapResult - only eNums that exists in election.h
 * are valid
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
            // shouldn`t get to here
            assert(false);
            return ELECTION_SUCCESS;
    }
}

/**
 * electionSetName - inserts or updates the name of tribe or area. validate the
 * inputs of the input.
 * @param aug_map - the tribe or the area map
 * @param element_id - the tribe id or the area id - cant be negative
 * @param name - the new name of the area or tribe
 * @param should_exist - indicates if the user wants to change the name of the
 * tribe or just enter a new tribe to the election
 * @param augMapResultToElectionResult - the function that converts the eNum
 * @return
 *      ELECTION_NULL_ARGUMENT - in case there`s a null argument
 *      ELECTION_INVALID_ID - if the id negative
 *      ELECTION_TRIBE_DOSE_NOT_EXIST - in case the user wants to update the
 * name of the tribe, but it dosen`t exist ELECTION_INVALID_NAME - invalid char
 * in the name string ELECTION_SUCCESS - if the name was successfully inserted
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
    // checks if the tribe/area id already exits in election
    AugMapResult status = augMapContains(aug_map, element_id, &is_present);
    if (status != AUG_MAP_SUCCESS) {
        return augMapResultToElectionResult(status);
    }
    // checks if the user wants to update the tribe name but the tribe doesnt
    // exists
    if (is_present != should_exist) {
        return should_exist
                   ? augMapResultToElectionResult(AUG_MAP_ITEM_DOES_NOT_EXIST)
                   : augMapResultToElectionResult(AUG_MAP_ITEM_ALREADY_EXISTS);
    }

    if (!isLowerCase(name)) {
        return ELECTION_INVALID_NAME;
    }
    // inserts the new name to and id to election / or just updates the value of
    // the key
    AugMapResult res = augMapPutStr(aug_map, element_id, name);
    assert(res != AUG_MAP_INVALID_TYPE);
    return augMapResultToElectionResult(res);
}

ElectionResult electionAddTribe(Election election, int tribe_id,
                                const char *tribe_name) {
    if (election == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    // inserting the new tribe to election->tribe, using the electionSetName
    // function
    return electionSetName(election->tribes, tribe_id, tribe_name, false,
                           augMapResultToElectionResultTribe);
}

ElectionResult electionAddArea(Election election, int area_id,
                               const char *area_name) {
    if (election == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    // adding the area_id and its name to election->areas
    ElectionResult result =
        electionSetName(election->areas, area_id, area_name, false,
                        augMapResultToElectionResultArea);
    if (result != ELECTION_SUCCESS) {
        return result;
    }
    // creating the map the that would store the votes to the tribes - in this
    // area.
    AugMap votes_tribe_map = augMapCreate(INT_TYPE);
    if (votes_tribe_map == NULL) {
// memory problem
#ifndef NDEBUG
        AugMapResult res = augMapRemove(election->areas, area_id);
        assert(res == AUG_MAP_SUCCESS);
#else
        augMapRemove(election->areas, area_id);
#endif

        return ELECTION_OUT_OF_MEMORY;
    }
    AugMapResult status =
        augMapPutMap(election->votes_by_area, area_id, votes_tribe_map);
    assert(status == AUG_MAP_OUT_OF_MEMORY || status == AUG_MAP_SUCCESS);

    if (status == AUG_MAP_OUT_OF_MEMORY) {
        augMapDestroy(votes_tribe_map);
#ifndef NDEBUG
        AugMapResult res = augMapRemove(election->areas, area_id);
        assert(res == AUG_MAP_SUCCESS);
#else
        augMapRemove(election->areas, area_id);
#endif

        return ELECTION_OUT_OF_MEMORY;
    }
    return augMapResultToElectionResultArea(status);
}

char *electionGetTribeName(Election election, int tribe_id) {
    if (election == NULL) {
        return NULL;
    }
    char *result;
    // getting the value of the tribe id from election->tribes
    AugMapResult status = augMapGetStr(election->tribes, tribe_id, &result);
    assert(status == AUG_MAP_OUT_OF_MEMORY || status == AUG_MAP_SUCCESS ||
           status == AUG_MAP_ITEM_DOES_NOT_EXIST);
    assert(status != AUG_MAP_NULL_ARGUMENT && status != AUG_MAP_INVALID_KEY);
    if (status != AUG_MAP_SUCCESS) {
        // if there were any problems in the input or in the function need to
        // return null
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
    // validating the arguments
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
        // going through the areas in the election and checking if there's votes
        // to the removed tribe
        AugMap area_votes;
        status = augMapGetMap(election->votes_by_area, area_key, &area_votes);
        // AUG_MAP_ITEM_DOES_NOT_EXIST also isn`t a possibility because
        // AUG_MAP_FOREACH gives us the key form areas
        assert(status == AUG_MAP_SUCCESS);
        status = augMapRemove(area_votes, tribe_id);
        assert(status != AUG_MAP_NULL_ARGUMENT);
        // dont need to check if augMapRemove returned
        // AUG_MAP_ITEM_DOES_NOT_EXIST, because the area_key was given by
        // AUG_MAP_FOREACH.
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
        // iterating until there`s no more items to delete
        AUG_MAP_FOREACH(key, election->areas) {
            if (should_delete_area(key)) {
#ifndef NDEBUG
                assert(augMapRemove(election->areas, key) == AUG_MAP_SUCCESS);
                assert(augMapRemove(election->votes_by_area, key) ==
                       AUG_MAP_SUCCESS);
#else
                augMapRemove(election->areas, key);
                augMapRemove(election->votes_by_area, key);
#endif
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

/**
 * electionChangeVotes - adding or removing votes from the election system
 * @param election - the election system
 * @param area_id
 * @param tribe_id
 * @param num_of_votes - can be positive for adding votes or negative votes but
 * cant be zero
 * @return
 *      ELECTION_NULL_ARGUMENT - null argument
 *      ELECTION_INVALID_ID - negative tribe/area id
 *      ELECTION_TRIBE_NOT_EXIST
 *      ELECTION_AREA_NOT_EXIST
 *      ELECTION_SUCCESS - in case the votes were added or removed successfully
 */
static ElectionResult electionChangeVotes(Election election, int area_id,
                                          int tribe_id, int num_of_votes,
                                          bool add_votes) {
    // validating arguments
    if (election == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    if (area_id < 0 || tribe_id < 0) {
        return ELECTION_INVALID_ID;
    }
    if (num_of_votes <= 0) {
        return ELECTION_INVALID_VOTES;
    }
    if (!add_votes) {
        num_of_votes = -num_of_votes;
    }

    AugMap area_votes;
    // tries to get the map of the tribes-votes of the specific area
    AugMapResult status =
        augMapGetMap(election->votes_by_area, area_id, &area_votes);
    assert(status == AUG_MAP_SUCCESS || status == AUG_MAP_ITEM_DOES_NOT_EXIST);
    if (status != AUG_MAP_SUCCESS) {
        return augMapResultToElectionResultArea(status);
    }

    bool res;
    // checks if the tribe already exits in the election
    status = augMapContains(election->tribes, tribe_id, &res);
    assert(status == AUG_MAP_SUCCESS || status == AUG_MAP_ITEM_DOES_NOT_EXIST);
    if (res == false) {
        return ELECTION_TRIBE_NOT_EXIST;
    }

    assert(area_votes != NULL);
    int votes = 0;
    // gets the current votes to the tribe in the area
    status = augMapGetInt(area_votes, tribe_id, &votes);
    assert(status == AUG_MAP_SUCCESS || status == AUG_MAP_ITEM_DOES_NOT_EXIST);
    if (status == AUG_MAP_ITEM_DOES_NOT_EXIST) {
        votes = 0;
    }
    votes = votes + num_of_votes;
    // inserts the new votes to the map of tribe-votes
    status = augMapPutInt(area_votes, tribe_id, max(0, votes));
    return augMapResultToElectionResultTribe(status);
}

ElectionResult electionAddVote(Election election, int area_id, int tribe_id,
                               int num_of_votes) {
    // adding the votes using the electionChangeVotes function
    return electionChangeVotes(election, area_id, tribe_id, num_of_votes, true);
}

ElectionResult electionRemoveVote(Election election, int area_id, int tribe_id,
                                  int num_of_votes) {
    // removing the votes using the electionChangeVotes function
    return electionChangeVotes(election, area_id, tribe_id, num_of_votes,
                               false);
}

// Get the winning tribe for the current area
static int computeWinningTribe(AugMap area_votes) {
    assert(area_votes);
    assert(augMapGetType(area_votes) == INT_TYPE);
#ifndef NDBEUG
    AugMapResult status;
#endif

    bool is_first_iteration = true;
    int winning_tribe_votes = 0;
    int winning_tribe_id;

    AUG_MAP_FOREACH(tribe_key, area_votes) {
        // going through the tribes in the area with votes
        int current_tribe_vote;
#ifndef NDBEUG
        status = augMapGetInt(area_votes, tribe_key, &current_tribe_vote);
#else
        augMapGetInt(area_votes, tribe_key, &current_tribe_vote);
#endif
        // There are no possible errors in this special (when iterating and
        // getting the int value)
        assert(status == AUG_MAP_SUCCESS);

        if (is_first_iteration) {
            // Setting for the first time
            is_first_iteration = false;
            winning_tribe_votes = current_tribe_vote;
            winning_tribe_id = tribe_key;
        } else if (current_tribe_vote > winning_tribe_votes) {
            winning_tribe_id = tribe_key;
            winning_tribe_votes = current_tribe_vote;
        } else if (current_tribe_vote == winning_tribe_votes &&
                   tribe_key < winning_tribe_id) {
            winning_tribe_id = tribe_key;
        }
    }

    // if no vote was added in the area
    if (winning_tribe_votes == 0) {
        return -1;
    }

    return winning_tribe_id;
}

Map electionComputeAreasToTribesMapping(Election election) {
    if (election == NULL) {
        return NULL;
    }

    AugMap results = augMapCreate(INT_TYPE);
    if (results == NULL) {
        return NULL;
    }

    // Getting the min tribe id, if there`s no tribes in the system returns an
    // empty map
    int min_tribe_id = augMapGetMinKey(election->tribes);
    if (min_tribe_id == -1) {
        return augMapConvertToMap(results);
    }

    // going through the areas in election
    AUG_MAP_FOREACH(area_key, election->votes_by_area) {
        AugMap cur_area_votes;
        // getting the map of the tribe-votes in the area
        AugMapResult status =
            augMapGetMap(election->votes_by_area, area_key, &cur_area_votes);
        if (status != AUG_MAP_SUCCESS) {
            augMapDestroy(results);
            return NULL;
        }
        int winning_tribe_id = computeWinningTribe(cur_area_votes);

        // if no vote was added in the area
        if (winning_tribe_id == -1) {
            winning_tribe_id = min_tribe_id;
        }

        // inserting the most votes
        status = augMapPutInt(results, area_key, winning_tribe_id);
        if (status != AUG_MAP_SUCCESS) {
            augMapDestroy(results);
            return NULL;
        }
    }
    return augMapConvertToMap(results);
}
