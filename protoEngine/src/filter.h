#ifndef FILTER
#define FILTER

#include <vector>
#include <string>
#include <stdarg.h>

class Filter{
private:
public:
    Filter();
    Filter(int n_args, ...);

    std::vector<std::pair<std::string, std::string> > filter;
};

#endif // FILTER

