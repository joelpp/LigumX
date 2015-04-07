#include "filter.h"

typedef std::pair<std::string, std::string> string_pair;

Filter::Filter(){
}

Filter::Filter(int n_args, ...){
    va_list arguments;                     // A place to store the list of arguments

//    va_start ( arguments, n_args );           // Initializing arguments to store all values after num
//    for ( int x = 0; x < n_args; x++ )        // Loop until all numbers are added
//        this->filter.push_back(va_arg(arguments, string_pair)); // Adds the next value in argument list to sum.
//    va_end ( arguments );                  // Cleans up the list

}
