#include "app.h"
#include <media_center/media_center.h>
#include <media_center/adapters/kodi_adapter.h>
#include <interop_mgr.h>

using namespace shelper;

app* g_app = nullptr;
app* shelper::app_instance() {
	return g_app;
}


app::app(const downloader_ptr& downloader):
m_downloader(downloader)
{
	g_app = this;
}

void app::init() {
	m_interop = std::make_shared<interop_mgr>();
	m_interop->init();

	m_mc = std::make_shared<media_center::kodi_adapter>();
	m_interop->set_media_center(m_mc);

	m_mc->set_host("192.168.1.181:1234");
	
//	#ifdef TEST_SUB
//		NSBundle* bundle = [NSBundle mainBundle];
//		NSURL* pathToExample = [bundle URLForResource:@TEST_SUB withExtension:@""];
//		if (pathToExample != nil)
//			m_interop_ptr->load_subtitles([pathToExample.path UTF8String]);
//	#endif

}

void app::handle_timer(long time_ms) {
	m_interop->handle_timer(time_ms);
}
