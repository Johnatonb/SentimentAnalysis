#include <iostream>
#include "Classifier.h"

int main(int argc, char** argv) {
    std::ifstream ifsTrain;
    ifsTrain.open(argv[1]);
    std::ifstream ifsTest;
    std::ifstream ifsSent;
    std::ofstream ofs;
    ifsTest.open(argv[2]);
    ifsSent.open(argv[3]);
    ofs.open(argv[4]);

    Classifier analyzer;
    std::unordered_map<DSString,int> freqResults = analyzer.readTrain(ifsTrain);
    std::cout<<"Finished Analysis"<<std::endl;


    std::unordered_map<DSString,Tweet> tweetSent = analyzer.getTestSentiment(ifsTest,freqResults,1,1,1);
    std::cout<<"Made Predictions"<<std::endl;
    analyzer.testOutput(ofs,ifsSent, tweetSent, freqResults);
    return 0;
}