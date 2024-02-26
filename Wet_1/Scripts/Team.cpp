#include "Team.h"

Team::Team(int teamId, Country * country, Sport sport): teamId(teamId), country(country), sport(sport), numContestants(0),austerity(0), s1(), s2(), s3(), st1(), st2(), st3() {
    s1 = new avl<int, Contestant*>;
    try{
        s2 = new avl<int, Contestant*>;
    }catch(std::bad_alloc& err){
        delete s1;
        throw err;
    }
    try{
        s3 = new avl<int, Contestant*>;
    }catch(std::bad_alloc& err){
        delete s1;
        delete s2;
        throw err;
    }
    try{
        st1 = new avl<pair<int, int>, int>;
    }catch(std::bad_alloc& err){
        delete s1;
        delete s2;
        delete s3;
        throw err;
    }
    try{
        st2 = new avl<pair<int, int>, int>;
    }catch(std::bad_alloc& err){
        delete s1;
        delete s2;
        delete s3;
        delete st1;
        throw err;
    }
    try{
        st3 = new avl<pair<int, int>, int>;
    }catch(std::bad_alloc& err){
        delete s1;
        delete s2;
        delete s3;
        delete st1;
        delete st2;
        throw err;
    }
    country->addTeam();
}

int Team::getStrength() {
    return numContestants % 3? 0: st1->getMax().getP1() + st2->getMax().getP1() + st3->getMax().getP1();
}

int Team::getCountryId() {
    return country->getId();
}

template <typename T, typename S>
StatusType switchTree(T id, avl<T, S>* tree1, avl<T, S>* tree2){
    S value = *tree1->find(id);
    StatusType status = tree1->remove(id);
    if(status != StatusType::SUCCESS) return status;
    status = tree2->insert(id, value);
    return status;
}

StatusType Team::addContestant(int id, int strength,Contestant* contestant, bool fromUpdate){
    StatusType status = StatusType::SUCCESS;
    pair<int, int> strengthPair(strength, id);
    if(id < s1->getMax()){ //insert to s1
        status = s1->insert(id, contestant);
        if(status != StatusType::SUCCESS) return status;
        status = st1->insert(strengthPair, id);
        if(status != StatusType::SUCCESS) return status;
    }
    else if(id < s2->getMax()){ //insert to s2
        status = s2->insert(id, contestant);
        if(status != StatusType::SUCCESS) return status;
        status = st2->insert(strengthPair, id);
        if(status != StatusType::SUCCESS) return status;
    }
    else{ //insert to s3
        status = s3->insert(id, contestant);
        if(status != StatusType::SUCCESS) return status;
        status = st3->insert(strengthPair, id);
        if(status != StatusType::SUCCESS) return status;
    }
    numContestants++;
    status = balanceTeam();
    if (!fromUpdate) updateAusterity();
    return status;
}
StatusType Team::removeContestant(int id, bool fromUpdate) {
    StatusType status;
    if (!s1->find(id) && !s2->find(id) && !s3->find(id)) return StatusType::FAILURE;
    pair<int, int> strengthPair;
    if (id <= s1->getMax()){
        int strength = (*s1->find(id))->getStrength();
        strengthPair = pair<int, int>(strength, id);
        status = s1->remove(id);
        if (status != StatusType::SUCCESS) return status;
        status = st1->remove(strengthPair);
        if (status != StatusType::SUCCESS) return status;
    }else if (id <= s2->getMax()){
        int strength = (*s2->find(id))->getStrength();
        strengthPair = pair<int, int>(strength, id);
        status = s2->remove(id);
        if (status != StatusType::SUCCESS) return status;
        status = st2->remove(strengthPair);
        if (status != StatusType::SUCCESS) return status;
    }else{
        int strength = (*s3->find(id))->getStrength();
        strengthPair = pair<int, int>(strength, id);
        status = s3->remove(id);
        if (status != StatusType::SUCCESS) return status;
        status = st3->remove(strengthPair);
        if (status != StatusType::SUCCESS) return status;
    }
    numContestants--;
    status = balanceTeam();
    if (!fromUpdate) updateAusterity();
    return status;
}

StatusType Team::balanceTeam() {
    StatusType status;
    int numPerS = ((numContestants - 1) / 3) + 1;
    int toMove, toMoveStrength;
    pair<int, int> toMovePair;
    if(numPerS  < s1->getSize()){ //if first tree not good
        toMove = s1->getMax();
        toMoveStrength = (*s1->find(toMove))->getStrength();
        toMovePair = pair<int, int>(toMoveStrength, toMove);
        status = switchTree<int, Contestant*>(toMove, s1, s2);
        if (status != StatusType::SUCCESS) return status;
        status = switchTree<pair<int, int>, int>(toMovePair, st1, st2);
        if (status != StatusType::SUCCESS) return status;
    }
    if(numPerS < s2->getSize()){ //if second tree not good
        toMove = s2->getMax();
        toMoveStrength = (*s2->find(toMove))->getStrength();
        toMovePair = pair<int, int>(toMoveStrength, toMove);
        status = switchTree<int, Contestant*>(toMove, s2, s3);
        //TODO if status isnt success return trees to old
        if (status != StatusType::SUCCESS) return status;
        status = switchTree<pair<int, int>, int>(toMovePair, st2, st3);
        if (status != StatusType::SUCCESS) return status;
    }
    if(numPerS < s3->getSize()){ //if third tree not good
        toMove = s3->getMin();
        toMoveStrength = (*s3->find(toMove))->getStrength();
        toMovePair = pair<int, int>(toMoveStrength, toMove);
        status = switchTree<int, Contestant*>(toMove, s3, s2);
        //TODO if status isnt success return trees to old
        if (status != StatusType::SUCCESS) return status;
        status = switchTree<pair<int, int>, int>(toMovePair, st3, st2);
        if (status != StatusType::SUCCESS) return status;
    }
    if(numPerS < s2->getSize()){ //if second tree not good
        toMove = s2->getMin();
        toMoveStrength = (*s2->find(toMove))->getStrength();
        toMovePair = pair<int, int>(toMoveStrength, toMove);
        status = switchTree<int, Contestant*>(toMove, s2, s1);
        //TODO if status isnt success return trees to old
        if (status != StatusType::SUCCESS) return status;
        status = switchTree<pair<int, int>, int>(toMovePair, st2, st1);
        if (status != StatusType::SUCCESS) return status;
    }
    return StatusType::SUCCESS;
}

template<typename T>
pair<T*, int> merge(T *arr1, int size1, T *arr2, int size2){
    T* newList = new T[size1+size2];
    int i1 = 0, i2 = 0, duplicates = 0;
    while(i1 + i2 < size1+size2){
        if (i1 == size1){
            newList[i1 + i2 - duplicates] = arr2[i2];
            i2++;
            continue;
        }
        if (i2 == size2){
            newList[i1 + i2 - duplicates] = arr1[i1];
            i1++;
            continue;
        }
        if(arr1[i1] == arr2[i2]){
            duplicates++;
            i1++;
            continue;
        }
        if (arr1[i1] < arr2[i2]){
            newList[i1 + i2 - duplicates] = arr1[i1];
            i1++;
        }else{
            newList[i1 + i2 - duplicates] = arr2[i2];
            i2++;
        }
    }
    int newSize = size1 + size2 - duplicates;
    T* noDupList = new T[newSize];
    for (int i = 0; i < newSize; ++i) {
        noDupList[i] = newList[i];
    }
    delete[] newList;
    return pair<T*, int>(noDupList, newSize);
}

StatusType Team::unite(Team &team2) {
    //TODO - need to delete these
    pair<int, Contestant*>* s1Ordered, *s2Ordered, *s3Ordered, *team2s1Ordered, *team2s2Ordered, *team2s3Ordered, *mergeds2, *mergeds3, *mergedt2s1, *mergedt2s2, *merged;
    pair<pair<int, Contestant*>*, int> mergeds2Pair, mergeds3Pair, mergedt2s1Pair, mergedt2s2Pair, mergedPair;
    int mergeds2Size, mergeds3Size, mergedt2s1Size, mergedt2s2Size, mergedSize;

    try {
        s1Ordered = s1->getInorder();
    }catch (std::bad_alloc& err){
        return StatusType::ALLOCATION_ERROR;
    }
    try {
        s2Ordered = s2->getInorder();
    }catch (std::bad_alloc& err){
        delete[] s1Ordered;
        return StatusType::ALLOCATION_ERROR;
    }
    try {
        s3Ordered = s3->getInorder();
    }catch (std::bad_alloc& err){
        delete[] s1Ordered;
        delete[] s2Ordered;
        return StatusType::ALLOCATION_ERROR;
    }
    try {
        team2s1Ordered = team2.s1->getInorder();
    }catch (std::bad_alloc& err){
        delete[] s1Ordered;
        delete[] s2Ordered;
        delete[] s3Ordered;
        return StatusType::ALLOCATION_ERROR;
    }
    try {
        team2s2Ordered = team2.s2->getInorder();
    }catch (std::bad_alloc& err){
        delete[] s1Ordered;
        delete[] s2Ordered;
        delete[] s3Ordered;
        delete[] team2s1Ordered;
        return StatusType::ALLOCATION_ERROR;
    }
    try {
        team2s3Ordered = team2.s3->getInorder();
    }catch (std::bad_alloc& err){
        delete[] s1Ordered;
        delete[] s2Ordered;
        delete[] s3Ordered;
        delete[] team2s1Ordered;
        delete[] team2s2Ordered;
        return StatusType::ALLOCATION_ERROR;
    }
    try {
        mergeds2Pair = merge<pair<int, Contestant*>>(s1Ordered, s1->getSize(), s2Ordered, s2->getSize());
        mergeds2 = mergeds2Pair.getP1();
        mergeds2Size = mergeds2Pair.getP2();
        delete[] s1Ordered;
        delete[] s2Ordered;
    }catch (std::bad_alloc& err){
        delete[] s1Ordered;
        delete[] s2Ordered;
        delete[] s3Ordered;
        delete[] team2s1Ordered;
        delete[] team2s2Ordered;
        delete[] team2s3Ordered;
        return StatusType::ALLOCATION_ERROR;
    }
    try {
        mergeds3Pair = merge<pair<int, Contestant*>>(mergeds2, mergeds2Size, s3Ordered, s3->getSize());
        mergeds3 = mergeds3Pair.getP1();
        mergeds3Size = mergeds3Pair.getP2();
        delete[] s3Ordered;
        delete[] mergeds2;
    }catch (std::bad_alloc& err){
        delete[] s3Ordered;
        delete[] team2s1Ordered;
        delete[] team2s2Ordered;
        delete[] team2s3Ordered;
        delete[] mergeds2;
        return StatusType::ALLOCATION_ERROR;
    }
    try {
        mergedt2s1Pair = merge<pair<int, Contestant*>>(mergeds3, mergeds3Size, team2s1Ordered, team2.s1->getSize());
        mergedt2s1 = mergedt2s1Pair.getP1();
        mergedt2s1Size = mergedt2s1Pair.getP2();
        delete[] team2s1Ordered;
        delete[] mergeds3;
    }catch (std::bad_alloc& err){
        delete[] team2s1Ordered;
        delete[] team2s2Ordered;
        delete[] team2s3Ordered;
        delete[] mergeds3;
        return StatusType::ALLOCATION_ERROR;
    }try {
        mergedt2s2Pair = merge<pair<int, Contestant*>>(mergedt2s1, mergedt2s1Size, team2s2Ordered, team2.s2->getSize());
        mergedt2s2 = mergedt2s2Pair.getP1();
        mergedt2s2Size = mergedt2s2Pair.getP2();
        delete[] team2s2Ordered;
        delete[] mergedt2s1;
    }catch (std::bad_alloc& err){
        delete[] team2s2Ordered;
        delete[] team2s3Ordered;
        delete[] mergedt2s1;
        return StatusType::ALLOCATION_ERROR;
    }try {
        mergedPair = merge<pair<int, Contestant*>>(mergedt2s2, mergedt2s2Size, team2s3Ordered, team2.s3->getSize());
        merged = mergedPair.getP1();
        mergedSize = mergedPair.getP2();
        delete[] team2s3Ordered;
        delete[] mergedt2s2;
    }catch (std::bad_alloc& err){
        delete[] team2s3Ordered;
        delete[] mergedt2s2;
        return StatusType::ALLOCATION_ERROR;
    }
    int m = mergedSize%3;
    int size1 = floor(mergedSize / 3) + (m? 1: 0), size2 = floor(mergedSize / 3) + (m==2? 1: 0), size3 = floor(mergedSize / 3);
    avl<int, Contestant*> *news1 = nullptr, *news2= nullptr, *news3 = nullptr;
    try{
        news1 = new avl<int, Contestant*>();
    }catch (std::bad_alloc& err){
        delete[] merged;
        return StatusType::ALLOCATION_ERROR;
    }
    try{
        news1->createEmpty(size1);
        news1->fillEmpty(merged);  //TODO - could be memory leak if copying is problem
    }catch (std::bad_alloc& err){
        delete[] merged;
        delete news1;
        return StatusType::ALLOCATION_ERROR;
    }
    try{
        news2 = new avl<int, Contestant*>();
    }catch (std::bad_alloc& err){
        delete[] merged;
        delete news1;
        return StatusType::ALLOCATION_ERROR;
    }
    try{
        news2->createEmpty(size2);
        news2->fillEmpty(merged + size1);
    }catch (std::bad_alloc& err){
        delete[] merged;
        delete news1;
        delete news2;
        return StatusType::ALLOCATION_ERROR;
    }
    try{
        news3 = new avl<int, Contestant*>();
    }catch (std::bad_alloc& err){
        delete[] merged;
        delete news1;
        delete news2;
        return StatusType::ALLOCATION_ERROR;
    }
    try{
        news3->createEmpty(size3);
        news3->fillEmpty(merged + size1 + size2);
    }catch (std::bad_alloc& err){
        delete[] merged;
        delete news1;
        delete news2;
        delete news3;
        return StatusType::ALLOCATION_ERROR;
    }
    delete s1;
    delete s2;
    delete s3;
    delete[] merged;
    s1 = news1;
    s2 = news2;
    s3 = news3;

    //TODO - need to delete these
    pair<pair<int, int>, int>* st1Ordered, *st2Ordered, *st3Ordered, *team2st1Ordered, *team2st2Ordered, *team2st3Ordered, *mergedst2, *mergedst3, *mergedt2st1, *mergedt2st2, *mergedst;
    pair<pair<pair<int, int>, int>*, int> mergedst2Pair, mergedst3Pair, mergedt2st1Pair, mergedt2st2Pair, mergedstPair;
    int mergedst2Size, mergedst3Size, mergedt2st1Size, mergedt2st2Size, mergedstSize;

    try {
        st1Ordered = st1->getInorder();
    }catch (std::bad_alloc& err){
        return StatusType::ALLOCATION_ERROR;
    }
    try {
        st2Ordered = st2->getInorder();
    }catch (std::bad_alloc& err){
        delete[] st1Ordered;
        return StatusType::ALLOCATION_ERROR;
    }
    try {
        st3Ordered = st3->getInorder();
    }catch (std::bad_alloc& err){
        delete[] st1Ordered;
        delete[] st2Ordered;
        return StatusType::ALLOCATION_ERROR;
    }
    try {
        team2st1Ordered = team2.st1->getInorder();
    }catch (std::bad_alloc& err){
        delete[] st1Ordered;
        delete[] st2Ordered;
        delete[] st3Ordered;
        return StatusType::ALLOCATION_ERROR;
    }
    try {
        team2st2Ordered = team2.st2->getInorder();
    }catch (std::bad_alloc& err){
        delete[] st1Ordered;
        delete[] st2Ordered;
        delete[] st3Ordered;
        delete[] team2st1Ordered;
        return StatusType::ALLOCATION_ERROR;
    }
    try {
        team2st3Ordered = team2.st3->getInorder();
    }catch (std::bad_alloc& err){
        delete[] st1Ordered;
        delete[] st2Ordered;
        delete[] st3Ordered;
        delete[] team2st1Ordered;
        delete[] team2st2Ordered;
        return StatusType::ALLOCATION_ERROR;
    }
    try {
        mergedst2Pair = merge<pair<pair<int, int>, int>>(st1Ordered, st1->getSize(), st2Ordered, st2->getSize());
        mergedst2 = mergedst2Pair.getP1();
        mergedst2Size = mergedst2Pair.getP2();
        delete[] st1Ordered;
        delete[] st2Ordered;
    }catch (std::bad_alloc& err){
        delete[] st1Ordered;
        delete[] st2Ordered;
        delete[] st3Ordered;
        delete[] team2st1Ordered;
        delete[] team2st2Ordered;
        delete[] team2st3Ordered;
        return StatusType::ALLOCATION_ERROR;
    }
    try {
        mergedst3Pair = merge<pair<pair<int, int>, int>>(mergedst2, mergedst2Size, st3Ordered, st3->getSize());
        mergedst3 = mergedst3Pair.getP1();
        mergedst3Size = mergedst3Pair.getP2();
        delete[] st3Ordered;
        delete[] mergedst2;
    }catch (std::bad_alloc& err){
        delete[] st3Ordered;
        delete[] team2st1Ordered;
        delete[] team2st2Ordered;
        delete[] team2st3Ordered;
        delete[] mergedst2;
        return StatusType::ALLOCATION_ERROR;
    }
    try {
        mergedt2st1Pair = merge<pair<pair<int, int>, int>>(mergedst3, mergedst3Size, team2st1Ordered, team2.st1->getSize());
        mergedt2st1 = mergedt2st1Pair.getP1();
        mergedt2st1Size = mergedt2st1Pair.getP2();
        delete[] team2st1Ordered;
        delete[] mergedst3;
    }catch (std::bad_alloc& err){
        delete[] team2st1Ordered;
        delete[] team2st2Ordered;
        delete[] team2st3Ordered;
        delete[] mergedst3;
        return StatusType::ALLOCATION_ERROR;
    }try {
        mergedt2st2Pair = merge<pair<pair<int, int>, int>>(mergedt2st1, mergedt2st1Size, team2st2Ordered, team2.st2->getSize());
        mergedt2st2 = mergedt2st2Pair.getP1();
        mergedt2st2Size = mergedt2st2Pair.getP2();
        delete[] team2st2Ordered;
        delete[] mergedt2st1;
    }catch (std::bad_alloc& err){
        delete[] team2st2Ordered;
        delete[] team2st3Ordered;
        delete[] mergedt2st1;
        return StatusType::ALLOCATION_ERROR;
    }try {
        mergedstPair = merge<pair<pair<int, int>, int>>(mergedt2st2, mergedt2st2Size, team2st3Ordered, team2.st3->getSize());
        mergedst = mergedstPair.getP1();
        mergedstSize = mergedstPair.getP2();
        delete[] team2st3Ordered;
        delete[] mergedt2st2;
    }catch (std::bad_alloc& err){
        delete[] team2st3Ordered;
        delete[] mergedt2st2;
        return StatusType::ALLOCATION_ERROR;
    }
    int mst = mergedstSize%3;
    int sizest1 = floor(mergedstSize / 3) + (mst? 1: 0), sizest2 = floor(mergedstSize / 3) + (mst==2? 1: 0), sizest3 = floor(mergedstSize / 3);
    avl<pair<int, int>, int> *newst1 = nullptr, *newst2= nullptr, *newst3 = nullptr;
    try{
        newst1 = new avl<pair<int, int>, int>();
    }catch (std::bad_alloc& err){
        delete[] mergedst;
        return StatusType::ALLOCATION_ERROR;
    }
    try{
        newst1->createEmpty(sizest1);
    }catch (std::bad_alloc& err){
        delete[] mergedst;
        delete newst1;
        return StatusType::ALLOCATION_ERROR;
    }
    try{
        newst2 = new avl<pair<int, int>, int>();
    }catch (std::bad_alloc& err){
        delete[] mergedst;
        delete newst1;
        return StatusType::ALLOCATION_ERROR;
    }
    try{
        newst2->createEmpty(sizest2);
    }catch (std::bad_alloc& err){
        delete[] mergedst;
        delete newst1;
        delete newst2;
        return StatusType::ALLOCATION_ERROR;
    }
    try{
        newst3 = new avl<pair<int, int>, int>();
    }catch (std::bad_alloc& err){
        delete[] mergedst;
        delete newst1;
        delete newst2;
        return StatusType::ALLOCATION_ERROR;
    }
    try{
        newst3->createEmpty(sizest3);
    }catch (std::bad_alloc& err){
        delete[] mergedst;
        delete newst1;
        delete newst2;
        delete newst3;
        return StatusType::ALLOCATION_ERROR;
    }
    newst1->fillByValue(mergedst, s1->getMin(), s1->getMax());
    newst2->fillByValue(mergedst, s2->getMin(), s2->getMax());
    newst3->fillByValue(mergedst, s3->getMin(), s3->getMax());

    delete st1;
    delete st2;
    delete st3;
    delete[] mergedst;
    st1 = newst1;
    st2 = newst2;
    st3 = newst3;
    numContestants = mergedSize;
    team2.numContestants = 0;
    updateTeamIds(s1, team2.getID());
    updateTeamIds(s2, team2.getID());
    updateTeamIds(s3, team2.getID());
    return StatusType::SUCCESS;
}

Sport Team::getSport() {
    return sport;
}

int Team::getID() {
    return teamId;
}

int Team::getNumContestants() {
    return numContestants;
}

Team::~Team() {
    delete s1;
    delete s2;
    delete s3;
    delete st1;
    delete st2;
    delete st3;
}

Country* Team::getCountry() {
    return country;
}

int Team::getAusterity() {
    return austerity;
}

int Team::calculateAusterity(int toRemoveS1, int toRemoveS2, int toRemoveS3) {
    pair<int, int> tempKeysSt1[3], tempKeysSt2[3], tempKeysSt3[3];
    int tempIdSt1[3], tempIdSt2[3], tempIdSt3[3];
    Contestant* tempContestents1[3], *tempContestents2[3], *tempContestents3[3];
    int newStrength = this->getStrength();
    for(int i=0; i < toRemoveS1; i++){
        pair<int,int> minStrength = st1->getMin();
        int minId = minStrength.getP2();
        tempKeysSt1[i] = minStrength;
        tempIdSt1[i] = minId;
        tempContestents1[i] = *s1->find(minId);
        s1->remove(minId);
        st1->remove(minStrength);
        numContestants--;
    }
    for(int i=0; i < toRemoveS2; i++){
        pair<int,int> minStrength = st2->getMin();
        int minId = minStrength.getP2();
        tempKeysSt2[i] = minStrength;
        tempIdSt2[i] = minId;
        tempContestents2[i] = *s2->find(minId);
        s2->remove(minId);
        st2->remove(minStrength);
        numContestants--;
    }
    for(int i=0; i < toRemoveS3; i++){
        pair<int,int> minStrength = st3->getMin();
        int minId = minStrength.getP2();
        tempKeysSt3[i] = minStrength;
        tempIdSt3[i] = minId;
        tempContestents3[i] = *s3->find(minId);
        s3->remove(minId);
        st3->remove(minStrength);
        numContestants--;
    }
    balanceTeam();
    balanceTeam();
    balanceTeam();
    newStrength = this->getStrength();
    for(int i=0; i < toRemoveS1; i++){
        int strength = tempKeysSt1[i].getP1();
        addContestant(tempIdSt1[i], strength, tempContestents1[i], true);
    }
    for(int i=0; i < toRemoveS2; i++){
        int strength = tempKeysSt2[i].getP1();
        addContestant(tempIdSt2[i], strength, tempContestents2[i], true);
    }
    for(int i=0; i < toRemoveS3; i++){
        int strength = tempKeysSt3[i].getP1();
        addContestant(tempIdSt3[i], strength, tempContestents3[i], true);
    }
    return newStrength;
}

void Team::updateAusterity() {
    std::cout << "1324" << std::endl;
    int max = 0, trial = 0;
    if(numContestants % 3 != 0){
        austerity = 0;
        return;
    }
    if(numContestants > 3){
        trial = calculateAusterity(1,1,1);
        if(trial > max) max = trial;
        trial = calculateAusterity(2,1,0);
        if(trial > max) max = trial;
        trial = calculateAusterity(2,0,1);
        if(trial > max) max = trial;
        trial = calculateAusterity(1,2,0);
        if(trial > max) max = trial;
        trial = calculateAusterity(0,2,1);
        if(trial > max) max = trial;
        trial = calculateAusterity(1,0,2);
        if(trial > max) max = trial;
        trial = calculateAusterity(0,1,2);
        if(trial > max) max = trial;
    }
    if(numContestants > 6){
        trial = calculateAusterity(3,0,0);  //TODO - this ruins the tree!!!!!!
        if(trial > max) max = trial;
        trial = calculateAusterity(0,3,0);
        if(trial > max) max = trial;
        trial = calculateAusterity(0,0,3);
        if(trial > max) max = trial;
    }
    austerity = max;
}

bool Team::operator<(const Team &toCompare) const {
    return true;
}

void Team::setContestants(int update) {
    numContestants = update;
}

void Team::updateTeamIds(avl<int, Contestant *> *tree, int oldID) {
    updateTeamIdsAux(oldID, tree->getRoot());
}

void Team::updateTeamIdsAux(int oldID, node<int, Contestant *>* cur) {
    if (!cur) return;
    updateTeamIdsAux(oldID, cur->getLeft());
    updateTeamIdsAux(oldID, cur->getRight());
    (*(cur->getValue())).updateTeam(oldID, teamId);
}
