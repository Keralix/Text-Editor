#include "Text.h"
#include <string>

Text::Text():data(""){}
Text::Text(std::string input) : data(input){}
Text::~Text(){}

std::string Text::getText() const{
    return data;
}

void Text::changeText(const std::string& newText){
    data = newText;
}
void Text::append(char c){
    data +=c;
}
void Text::deleteLast(){
    if(!data.empty()) data.pop_back();
}
void Text::clear(){
    data.clear();
}

int Text::length() const {
    return data.length();
}
void Text::insertAt(int pos, char c){
    data.insert(pos,1,c);
}
void Text::deleteAt(int pos){
    if(pos >=0 &&pos<data.length()){
        data.erase(pos,1);
    }
}
void Text::deleteBefore(int pos){
    if(pos>=0){
        data.erase(0,pos);
    }
}
void Text::deleteAfter(int pos){
        if(pos>=0){
        data.erase(pos,data.length()-pos);
    }
}
char Text::getCharAt(int pos){
    return data[pos];
}