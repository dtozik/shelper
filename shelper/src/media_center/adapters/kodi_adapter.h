#ifndef _KODI_ADAPTER_H_
#define _KODI_ADAPTER_H_

#include "media_center.h"

namespace shelper {
namespace media_center {


class kodi_adapter : public media_center_adapter {
public:
    ~kodi_adapter() override {
    }
    
    void play() override;
    void pause() override;
    void stop() override;
    void seek() override;
    bool get_player_info(player_info& info) override;
};

}
}


#endif // _KODI_ADAPTER_H_
