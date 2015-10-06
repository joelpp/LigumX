#include "osm_element.h"

using namespace std;

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
        PRINT("exception!");
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


OSMElement::ElementType OSMElement::typeFromStrings(string key, string value){
    if (key.compare("highway") == 0){
        if (value.compare("trunk") == 0) return OSMElement::HIGHWAY_TRUNK;
        else if (value.compare("primary") == 0) return OSMElement::HIGHWAY_PRIMARY;
        else if (value.compare("secondary") == 0) return OSMElement::HIGHWAY_SECONDARY;
        else if (value.compare("tertiary") == 0) return OSMElement::HIGHWAY_TERTIARY;
        else if (value.compare("residential") == 0) return OSMElement::HIGHWAY_RESIDENTIAL;
        else if (value.compare("service") == 0) return OSMElement::HIGHWAY_SERVICE;
        else if (value.compare("unclassified") == 0) return OSMElement::HIGHWAY_UNCLASSIFIED;
        else return OSMElement::NOT_IMPLEMENTED;

    }
    else if (key.compare("natural") == 0){
        if (value.compare("wood") == 0) return OSMElement::NATURAL_WOOD;
        if (value.compare("water") == 0) return OSMElement::NATURAL_WATER;
        else return OSMElement::NOT_IMPLEMENTED;

    }
    else if (key.compare("building") == 0){
        if (value.compare("yes") == 0) return OSMElement::BUILDING_UNMARKED;
        else if (value.compare("school") == 0) return OSMElement::BUILDING_SCHOOL;
        else return OSMElement::NOT_IMPLEMENTED;

    }
    else if (key.compare("contour") == 0) return OSMElement::CONTOUR;
    else if (key.compare("leisure") == 0){
        if (value.compare("park") == 0) return OSMElement::LEISURE_PARK;
        else return OSMElement::NOT_IMPLEMENTED;

    }
    else if (key.compare("addr:interpolation") == 0){ return OSMElement::ADDR_INTERPOLATION; }
    else if (key.compare("landuse") == 0){ return OSMElement::LANDUSE; }
    else if (key.compare("boundary") == 0){ return OSMElement::BOUNDARY; }

    else return OSMElement::NOT_IMPLEMENTED;
}


// 