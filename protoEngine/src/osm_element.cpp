#include <iostream>
#include "osm_element.h"

std::string OSMElement::getValue(std::string key){
    return tags[key];
}

bool OSMElement::hasTag(std::string tag){

        std::unordered_map<std::string,std::string>::const_iterator got = tags.find(tag);

     if ( got == tags.end() )
        return false;
     else
        return true;


}

bool OSMElement::hasITag(int tag){
    try{
        std::unordered_map<int,int>::const_iterator got = itags.find(tag);

         if ( got == itags.end() )
            return false;
         else
            return true;
    }
    catch(...){
        std::cout << "exception! \n";
        return false;
    }
}

bool OSMElement::hasTagAndValue(std::string key, std::string value){

    if (hasTag(key))
        return ((tags[key].compare(value) == 0) || tags[key].compare("") == 0);
    else
        return false;
}

bool OSMElement::hasITagAndValue(int key, int value){
    try{
        if (hasITag(key))
            return (itags[key] == value);
        else
            return false;
    }
    catch(...){
        return false;
    }
}

bool OSMElement::passesFilter(Filter f){
    for (int i = 0; i < f.filter.size(); i++){
        std::pair<std::string, std::string> filterElement = f.filter[i];

        if (this->hasTagAndValue(filterElement.first, filterElement.second)) return true;
    }

    return false;
}
