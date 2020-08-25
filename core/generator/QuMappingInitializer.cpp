//
// Created by YUSHAH on 7/24/2020.
//

#include <util/Util.h>
#include "QuMappingInitializer.h"

const int QuMappingInitializer::TOTAL_PERM = 3; // # of qubits
int QuMappingInitializer::n = 0; // # of qubits
int QuMappingInitializer::count = 0; // permutation count
QuMapping QuMappingInitializer::defaultMapping;
vector<string> QuMappingInitializer::perms;



void QuMappingInitializer::generateMappings() {
    string str = "0123456789";
    int n = str.size();
    Util::permute(str, 0, n-1, QuMappingInitializer::perms);
}

QuMapping QuMappingInitializer::getNextMapping() {
    QuMapping nextMapping;
    for(int i=0; i<perms[count].length(); i++){
        int val = perms[count][i] - 48;
        nextMapping.setValueAt(i, val);
    }
    count++;
    return nextMapping;
}

void QuMappingInitializer::initGenerator(int n) {
    QuMappingInitializer::n = (n);
    QuMappingInitializer::count = 0;

}

const int QuMappingInitializer::getPermCount() {
    return perms.size();
}
