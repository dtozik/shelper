#ifndef _KODI_ADAPTER_H_
#define _KODI_ADAPTER_H_

#include "media_center.h"

namespace shelper {
namespace media_center {


class kodi_adapter : public media_center_adapter {
    mutable int m_cached_player_id = -1;
public:
    ~kodi_adapter() override {}
    
    bool play() const override;
    bool pause() const override;
    bool stop() const override;
    bool seek() const override;
    bool is_playing() const override;
    bool get_player_info(player_info& info) const override;
    bool get_current_track_info(track_info& info) const override;
    
private:
    bool play_pause_impl() const;
    bool get_player_info_impl(player_info& info) const;
};

}
}


#endif // _KODI_ADAPTER_H_
