//
// Created by mark on 6/2/2021.
//

#include "DSString.h"

DSString::DSString() : data{nullptr} { };

DSString::DSString(const char* c_str) {
    data = new char[strlen(c_str)+1];
    length = strlen(c_str);
    strcpy(data,c_str);
}

DSString::DSString(const DSString &str) {
    data = new char[str.getLength()+1];
    length = str.getLength();
    strcpy(data,str.data);
}

DSString::~DSString() {
    delete[] data;
}

DSString& DSString::operator= (const char* c_str){
    if(&c_str == &data){return *this;}
    if(data) delete[] data;
    data = new char[strlen(c_str)+1];
    length = strlen(c_str);
    strcpy(data,c_str);
    return *this;
}

DSString& DSString::operator= (const DSString& str){
    if(str.data == data){return *this;}
    if(data) delete[] data;
    data = new char[str.getLength()+1];
    length = str.getLength();
    strcpy(data,str.data);
    return *this;
}

bool DSString::operator==(const char* c_str) const{
    if(strlen(c_str)!=getLength()){return false;}
    return strcmp(data,c_str);

}

bool DSString::operator==(const DSString& str) const{
    if(str.getLength()!=getLength()){return false;}
    if(strcmp(data,str.data) == 0){return true;}
    return false;
}

bool DSString::operator!=(const char* c_str) const{
    return !(*this == c_str);
}

bool DSString::operator!=(const DSString& str) const{
    return !(*this == str);
}

bool DSString::operator> (const DSString& str) const{
    return getLength() > str.getLength();
}
bool DSString::operator> (const char* c_str) const{
    return getLength() > strlen(c_str);
}

bool DSString::operator< (const DSString& str) const{
    return getLength() < str.getLength();
}
bool DSString::operator< (const char* c_str) const{
    return getLength() < strlen(c_str);
}

char& DSString::operator[] (const int i){
    if(i>length){throw std::out_of_range("index out of bounds");}
    return *(data + i);
}

size_t DSString::getLength() const{
    return length;
}
#include <iostream>

DSString DSString::substring(int start, int numChars) {
    DSString temp;
    temp.data = new char[numChars + 1];
    temp.length = numChars;
    strncpy(temp.data, data + start, numChars);
    temp.data[numChars] = '\0';
    return temp;
}

DSString DSString::operator+(const DSString &arg) const{
    DSString temp;
    temp.data = new char[getLength() + arg.getLength() + 1];
    temp.length = length + arg.length;
    strcpy(temp.data,data);
    strcat(temp.data,arg.data);
    return temp;
}

char* DSString::c_str() const{
    return data;
}

std::ostream& operator<< (std::ostream& out, const DSString& arg){
    out << arg.data;
    return out;
}

size_t DSString::find(char* match) const{
    if(strstr(data,match)!= nullptr){return strstr(data,match) - data + 1;}
    return length;

}

size_t DSString::findnth(char* match, size_t n) const {
    //calls find on the string n times using substring to narrow the search
    DSString temp(data);
    int count = 0;
    size_t n_pos = 0;
    size_t n_pos_total = 0;
    while(count<n){
        n_pos = temp.find(match);
        n_pos_total += n_pos;
        temp = temp.substring(n_pos,temp.getLength() - n_pos);
        count++;
    }
    return n_pos_total;
}