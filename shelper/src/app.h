#ifndef _APP_H_
#define _APP_H_

#include <defs.h>

namespace shelper {
using namespace net;
using namespace media_center;

class app final {
	downloader_ptr m_downloader;
    interop_mgr_ptr m_interop;
    media_center_adapter_ptr m_mc;

public:
	explicit app(const downloader_ptr& downloader);
	void init();
	void handle_timer(long time_ms);
	interop_mgr_ptr interop() { return m_interop; }
	downloader_ptr downloader() { return m_downloader; }
};
using app_ptr = std::shared_ptr<app>;

app* app_instance();


}

#endif // _APP_H_
