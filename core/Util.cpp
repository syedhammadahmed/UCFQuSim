//
// Created by YUSHAH on 10/21/2019.
//

#include "Util.h"

string Util::toLower(string str) {
    for(int i=0; i<str.length(); i++)
        str[i] = tolower(str[i]);
    return str;
}
