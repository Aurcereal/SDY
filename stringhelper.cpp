#include "stringhelper.h"

void split(const string &s, const string &delim, vector<string>* outputVec) {
    outputVec->clear();

    size_t prevIndex = 0;
    size_t currIndex = s.find(delim, prevIndex);
    while (currIndex != string::npos) {
        outputVec->push_back(s.substr(prevIndex, currIndex - prevIndex));

        prevIndex = currIndex + 1;
        currIndex = s.find(delim, prevIndex);
    }

    // Add the last segment of the string if there's anything left to add
    if (prevIndex < s.length()) {
        outputVec->push_back(s.substr(prevIndex, s.length() - prevIndex));
    }

}