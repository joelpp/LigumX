#include "OSMElement.h"

bool OSMElement::hasTag(std::string tag){
    std::unordered_map<std::string,std::string>::const_iterator got = tags.find(tag);

     if ( got == tags.end() )
        return false;
     else
        return true;
}


bool OSMElement::hasTagAndValue(std::string key, std::string value){
    if (hasTag(key))
        return (tags[key].compare(value) == 0);
    else
        return false;
}
