#include <stdio.h>
#include "election.h"

Election electionCreate() {
    return false;
}

void electionDestroy(Election election) {
}

ElectionResult electionAddTribe(Election election, int tribe_id, const char *tribe_name) {
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
