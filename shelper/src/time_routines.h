#ifndef _TIME_ROUTINES_H_
#define _TIME_ROUTINES_H_

#include <defs.h>
#include <chrono>
#include <cmath>

namespace shelper {
namespace time {

struct time_info {
    std::chrono::milliseconds ms = std::chrono::milliseconds(0);
    std::chrono::seconds secs = std::chrono::seconds(0);
    std::chrono::minutes mins = std::chrono::minutes(0);
    std::chrono::hours hours = std::chrono::hours(0);
    
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
	time_info operator + (const time_info& t) const {
		time_info result;
		auto v = t.ms.count() + ms.count();
		if (v >= 1000) {
			result.ms = std::chrono::milliseconds(static_cast<long>(v - 1000));
			//t.secs
		}
		
	}
	
	time_info operator / (double val) const {
//		1/2
//		= 0,5
//
//		0.5×60+25/2
//		= 42,5
//
//		0.5×60+0/2
//		= 30
		time_info t;
		double v = hours.count() / val;
		t.hours = std::chrono::hours(static_cast<long>(std::floor(v)));
		
		v = (v - std::floor(v)) * 60 + mins.count() / val;
		t.mins = std::chrono::minutes(static_cast<long>(std::floor(v)));

		v = (v - std::floor(v)) * 60 + secs.count() / val;
		t.secs = std::chrono::seconds(static_cast<long>(std::floor(v)));

		v = (v - std::floor(v)) * 1000 + ms.count() / val;
		t.ms = std::chrono::milliseconds(static_cast<long>(std::floor(v)));
		return t;
	}
	
	std::string print() const {
		std::stringstream s;
		s << hours.count() << ':'
		<< mins.count() << ':'
		<< secs.count() << ':'
		<< ms.count();
		return s.str();
	}
};

}}

#endif // _TIME_ROUTINES_H_
