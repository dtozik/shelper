#ifndef _MEDIA_CENTER_H_
#define _MEDIA_CENTER_H_

#include <defs.h>
#include <time_routines.h>
#include <regex>

namespace shelper {
namespace media_center {

struct player_info {
    unsigned player_id;
};


struct track_info {
    time::time_info     current_time;
    time::time_info     total_time;
    double              percentage;
    double              speed;
    bool                can_seek;
    bool                can_change_speed;
};

class media_center_adapter {
protected:
    std::pair<std::string, unsigned> m_server_path;
    
public:
    enum class type {
        kodi
    };
    
    virtual ~media_center_adapter() {}
    
public:
    void set_host(const std::string& host) {
        std::regex reg(":");
        using regex_iter_t = std::regex_token_iterator<std::string::const_iterator>;
        regex_iter_t iter(host.begin(), host.end(), reg, -1);
        
        m_server_path.first = *iter++;
        std::stringstream s(iter->str());
        unsigned val;
        s >> val;
        m_server_path.second = val;
    }
    
public:
    // control interface
    virtual bool get_player_info(player_info& info) const { return false; };
    virtual bool get_current_track_info(track_info& info, unsigned player_id) const { return false; };
    
    virtual void play() = 0;
    virtual void pause() = 0;
    virtual void stop() = 0;
    virtual void seek() = 0;
};
using media_center_adapter_ptr = std::shared_ptr<media_center_adapter>;

}
}

#endif // _MEDIA_CENTER_H_
