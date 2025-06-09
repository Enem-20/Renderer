#ifndef C_IN_FlIGHT_DEPENDENT_H
#define C_IN_FlIGHT_DEPENDENT_H

#include <vector>
#include <cstddef>

template<class inFlightResource>
class InFlightDependent {
public:
    InFlightDependent(size_t count);
private:
    std::vector<inFlightResource> inFlightResources;
};

template<class inFlightResource>
InFlightDependent<inFlightResource>::InFlightDependent(size_t count)
    : inFlightResources(count) {
    
}

#endif