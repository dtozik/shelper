#ifndef _TIME_ROUTINES_H_
#define _TIME_ROUTINES_H_

#include <defs.h>
#include <chrono>

namespace shelper {
namespace time {

struct time_info {
    std::chrono::milliseconds ms;
    std::chrono::seconds secs;
    std::chrono::minutes mins;
    std::chrono::hours hours;
    
    bool operator == (const time_info& rhs) const {
        return hours == rhs.hours && mins == rhs.mins && secs == rhs.secs && ms == rhs.ms;
    }
    
    bool operator >= (const time_info& rhs) const {
        return *this > rhs || *this == rhs;
    }
    
    bool operator <= (const time_info& rhs) const {
        return *this < rhs || *this == rhs;
    }

    bool operator < (const time_info& rhs) const {
        if (hours < rhs.hours)
            return true;
        
        if (hours > rhs.hours)
            return false;
        
        if (mins < rhs.mins)
            return true;
        
        if (mins > rhs.mins)
            return false;
        
        if (secs < rhs.secs)
            return true;
        
        if (secs > rhs.secs)
            return false;
        
        if (ms < rhs.ms)
            return true;
        
        if (ms > rhs.ms)
            return false;
        
        return false;
    }
    
   bool operator > (const time_info& rhs) const {
        if (hours > rhs.hours)
            return true;
        
        if (hours < rhs.hours)
            return false;
        
        if (mins > rhs.mins)
            return true;
        
        if (mins < rhs.mins)
            return false;
        
        if (secs > rhs.secs)
            return true;
        
        if (secs < rhs.secs)
            return false;
        
        if (ms > rhs.ms)
            return true;
        
        if (ms < rhs.ms)
            return false;
        
        return false;
    }
};

}}

#endif // _TIME_ROUTINES_H_
