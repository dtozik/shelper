#ifndef _KODI_ADAPTER_H_
#define _KODI_ADAPTER_H_

#include "media_center.h"

namespace shelper {
namespace media_center {


class kodi_adapter : public media_center_adapter {
    int m_cached_player_id;
public:
    kodi_adapter(): m_cached_player_id(-1) {
    }
    ~kodi_adapter() override {
    }
    
    void play() override;
    void pause() override;
    void stop() override;
    void seek() override;
    bool get_player_info(player_info& info) override;
    bool get_current_track_info(track_info& info, unsigned player_id) override;
    
};

}
}


#endif // _KODI_ADAPTER_H_
