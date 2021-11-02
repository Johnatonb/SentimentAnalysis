//
// Created by johnk on 6/2/2021.
//

#ifndef PA01_SENTIMENT_CLASSIFIER_H
#define PA01_SENTIMENT_CLASSIFIER_H

#include <fstream>
#include <unordered_map>
#include <algorithm>
#include "DSString.h"

using Tweet = std::pair<int,std::vector<DSString>>;

class Classifier {
private:
    std::vector<DSString> getWords(DSString tweetText) const;
    int getSentiment(const std::vector<DSString> &words, const std::unordered_map<DSString,int> &freqData, int min_threshold, int mid_threshold, int max_threshold) const;
public:
    std::unordered_map<DSString,int> readTrain(std::ifstream &ifs);
    std::unordered_map<DSString,Tweet> getTestSentiment(std::ifstream &ifs,const std::unordered_map<DSString,int> &freqData, int min_threshold, int mid_threshold, int max_threshold) const;
    void testOutput(std::ofstream &ofs, std::ifstream &ifs, std::unordered_map<DSString,Tweet> &tweetSent,std::unordered_map<DSString,int> freqResults);
};



#endif //PA01_SENTIMENT_CLASSIFIER_H
