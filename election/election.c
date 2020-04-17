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
    if (election->tribe_name_id == NULL || election->tribe_head == NULL || election->area_name_id == NULL) {
        return NULL;
    }
    return election;
}

static Map stringToPointerMap(char *string) {
    return false;
}

void electionDestroy(Election election) {
    MAP_FOREACH(area_id, election->tribe_head) {
        mapDestroy(stringToPointerMap(mapGet(election->tribe_head, area_id)));
    }
    mapDestroy(election->tribe_head);
    mapDestroy(election->area_name_id);
    mapDestroy(election->tribe_name_id);
    free(election);
}

static bool lowerCaseCheck(const char *string) {
    unsigned int length = strlen(string);
    for (unsigned int i = 0; i < length; i++) {
        if ((string[i] < 'a' || string[i] > 'z') && string[i] != ' ') {
            return false;
        }
    }
    return true;
}

static char *intPointerToString(int *pointer) {
    char *string = malloc(sizeof(pointer));
    if (string == NULL) {
        return NULL;
    }
    sprintf(string, "%p", (void *) pointer);
    return string;
}

static char *mapPointerToString(Map pointer) {
    char *string = malloc(sizeof(pointer));
    if (string == NULL) {
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
    if (lowerCaseCheck(tribe_name) == false) {
        return ELECTION_INVALID_NAME;
    }
    int *p_tribe_id = &tribe_id;
    char *string_tribe_id = intPointerToString(p_tribe_id);
    if (string_tribe_id == NULL) {
        free(string_tribe_id);
        electionDestroy(election);
        return ELECTION_OUT_OF_MEMORY;
    }
    if (mapContains(election->tribe_name_id, string_tribe_id) == true) {
        return ELECTION_TRIBE_ALREADY_EXIST;
    }
    if (mapPut(election->tribe_name_id, string_tribe_id, tribe_name) == MAP_OUT_OF_MEMORY) {
        electionDestroy(election);
        free(string_tribe_id);
        return ELECTION_OUT_OF_MEMORY;
    }
    free(string_tribe_id);
    return ELECTION_SUCCESS;
}

ElectionResult electionAddArea(Election election, int area_id, const char *area_name) {
    if (election == NULL || area_name == NULL) {
        return ELECTION_NULL_ARGUMENT;
    }
    if (area_id < 0) {
        return ELECTION_INVALID_ID;
    }
    int *p_area_id = &area_id;
    char *string_area_id = intPointerToString(p_area_id);
    if (string_area_id == NULL) {
        electionDestroy(election);
        return ELECTION_OUT_OF_MEMORY;
    }
    if (mapContains(election->area_name_id, string_area_id) == true) {
        return ELECTION_TRIBE_ALREADY_EXIST;
    }
    if (lowerCaseCheck(area_name) == false) {
        return ELECTION_INVALID_NAME;
    }
    if (mapPut(election->area_name_id, string_area_id, area_name) == MAP_OUT_OF_MEMORY) {
        electionDestroy(election);
        free(string_area_id);
        return ELECTION_OUT_OF_MEMORY;
    }
    Map map_tribe_votes = mapCreate();
    char *string_map_tribe_votes = mapPointerToString(map_tribe_votes);
    if (string_map_tribe_votes == NULL) {
        free(string_area_id);
        electionDestroy(election);
        return ELECTION_OUT_OF_MEMORY;
    }
    if (mapPut(election->tribe_head, string_area_id, string_map_tribe_votes) == MAP_OUT_OF_MEMORY) {
        free(string_area_id);
        free(string_map_tribe_votes);
        electionDestroy(election);
        return ELECTION_OUT_OF_MEMORY;
    }
    free(string_area_id);
    free(string_map_tribe_votes);
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

int main() {
    Election elec = electionCreate();
    electionAddArea(elec, 4, "dani");
    MAP_FOREACH(item, elec->area_name_id) {
        char *a = mapGet(elec->area_name_id, item);
        a = NULL;
    }
    return 0;
}
