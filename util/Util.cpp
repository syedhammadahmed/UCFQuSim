//
// Created by YUSHAH on 10/21/2019.
//

#include <time.h>
#include <ctime>
#include "Util.h"

string Util::FILE_EXTENSION = ".qasm";

string Util::toLower(string str) {
    for(int i=0; i<str.length(); i++)
        str[i] = tolower(str[i]);
    return str;
}

double Util::timeIt(bool isEnd) {
    static time_t start;
    static time_t end;
    if(!isEnd) {
        time(&start);
    }
    else {
        time(&end);
        return double(end) - double(start);
    }
    return 0.0;
}



// ubuntu
//double Util::timeIt(bool isEnd) {
//    static time_t start;
//    static time_t end;
//    if(!isEnd) {
//        time(&start);
//    }
//    else {
//        time(&end);
//        return double(end) - double(start);
//    }
//    return 0.0;
//}

