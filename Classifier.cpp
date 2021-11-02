//
// Created by johnk on 6/2/2021.
//

#include "Classifier.h"

std::vector<DSString> Classifier::getWords(DSString tweetText) const{
    int count = 0;
    std::vector<DSString> words;

    while(tweetText[count]!='\0') {
        //Remove usernames
        if (tweetText[count] == '@') {
            int internalCount = count;
            while (tweetText[internalCount] != '\0' && !isspace(tweetText[internalCount])) {
                tweetText[internalCount] = ' ';
                internalCount++;
            }
        }
        //Remove urls
        if(tweetText.find("http")!=tweetText.getLength()){
            int internalCount = count;
            while(tweetText[internalCount] != '\0' && !isspace(tweetText[internalCount])){
                tweetText[internalCount] = ' ';
                internalCount++;
            }
        }

        tweetText[count] = tolower(tweetText[count]);

        if(!isalpha(tweetText[count])||tweetText[count]=='\''){tweetText[count]=' ';}

        count++;
    }
    count = 0;
    while(tweetText[count]!='\0'){
        if(isalpha(tweetText[count])){
            //find words by looking for next space or using the end of the tweet as default
            int wordLength = tweetText.substring(count,tweetText.getLength()-count).find(" ");
            if(wordLength<4){count+=wordLength; continue;}
            words.push_back(tweetText.substring(count,wordLength));
            count+=wordLength;
            continue;
        }
        count++;
    }

    return words;
}

int Classifier::getSentiment(const std::vector<DSString> &words, const std::unordered_map<DSString,int> &freqData, int min_threshold, int mid_threshold, int max_threshold) const {
    int sentiment = 0;
    //add total sentiment for all words in tweet using different rules based on thresholds
    for_each(words.begin(),words.end(),[&](DSString word){
        auto it = freqData.find(word);
        if(it!=freqData.end() && abs(it->second) > min_threshold){
            if(abs(it->second) > max_threshold){
                if(it->second > max_threshold){sentiment+=max_threshold;}
                else if(it->second < -max_threshold){sentiment-=max_threshold;}
            }else if(abs(it->second) > mid_threshold){
                sentiment+=it->second/words.size();
            }else{sentiment+=it->second;}
        }
    });
    return sentiment;
}

std::unordered_map<DSString,int> Classifier::readTrain(std::ifstream &ifs) {

    std::unordered_map<DSString,int> wordFreq;
    int count = 0;
    while(!ifs.eof() && count<20000){
        char tweetBuffer[800];
        if(count == 0){count++; ifs.getline(tweetBuffer,800); continue;}
        ifs.getline(tweetBuffer,800);
        DSString line(tweetBuffer);
        DSString sentiment = line.substring(0,1);
        int freqMod = 0;
        *sentiment.c_str() == '4' ? freqMod = 1 : freqMod =-1;
        size_t textBegin = line.findnth(",",5);
        DSString text = line.substring(textBegin,line.getLength()-textBegin);
        std::vector<DSString> words = getWords(text);
        //add to the count of each word or instantiate the sentiment for each word
        for_each(words.begin(),words.end(),[&](const DSString &word){auto it = wordFreq.find(word);it==wordFreq.end()?wordFreq[word]=freqMod:wordFreq[word]+=freqMod;});
        count++;
    }
    return wordFreq;
}



std::unordered_map<DSString,Tweet> Classifier::getTestSentiment(std::ifstream &ifs,const std::unordered_map<DSString,int> &freqData, int min_threshold, int mid_threshold, int max_threshold) const{
    int count=0;
    std::unordered_map<DSString,Tweet> tweets;
    while(!ifs.eof() && count<10000){
        char tweetBuffer[800];
        if(count == 0){count++; ifs.getline(tweetBuffer,800); continue;}
        ifs.getline(tweetBuffer,800);
        DSString line(tweetBuffer);
        DSString id = line.substring(0,10);
        size_t textBegin = line.findnth(",",4);
        DSString text = line.substring(textBegin,line.getLength()-textBegin);
        std::vector<DSString> words = getWords(text);
        //use insert or assign to be clear that modification is happening as opposed to the bracket operator
        tweets.insert_or_assign(id,Tweet{getSentiment(words,freqData, min_threshold, mid_threshold, max_threshold),words});
        count++;
    }
    return tweets;
}

void Classifier::testOutput(std::ofstream &ofs, std::ifstream &ifs, std::unordered_map<DSString,Tweet> &tweetSent, std::unordered_map<DSString,int> freqResults) {
    std::vector<DSString> wrongs;
    int count=0;
    while(!ifs.eof() && count <10000){
        char tweetBuffer[800];
        if(count == 0){count++; ifs.getline(tweetBuffer,800); continue;}
        ifs.getline(tweetBuffer,800);
        DSString line(tweetBuffer);
        DSString id = line.substring(2,10);
        DSString sentiment = line.substring(0,1);
        DSString tweetSentiment;
        if(tweetSent[id].first < 0){ tweetSentiment = "0";}else{tweetSentiment = "4";}
        if(sentiment != tweetSentiment){
            wrongs.push_back(id);
        }
        count++;
    }
    ofs<<1 - (float)wrongs.size()/count<<std::endl;
    std::for_each(wrongs.begin(),wrongs.end(),[&](DSString id){ofs<<id<<std::endl;});
}