#ifndef _APP_H_
#define _APP_H_

#include <defs.h>

namespace shelper {
using namespace net;
using namespace media_center;
using namespace sub;

class app final {
	downloader_ptr m_downloader;
    interop_mgr_ptr m_interop;
	fs::file_system_ptr	m_fs;
	sub::fetcher_ptr m_fetcher;
public:
	explicit app(const downloader_ptr& downloader, const fs::file_system_ptr& fs);
	void init();
	void handle_timer(long time_ms);
	interop_mgr_ptr interop() { return m_interop; }
	downloader_ptr downloader() { return m_downloader; }
	sub::fetcher_ptr fetcher() { return m_fetcher; }
	fs::file_system_ptr fs() { return m_fs; }
	
};
using app_ptr = std::shared_ptr<app>;

app* app_instance();


}

#endif // _APP_H_
