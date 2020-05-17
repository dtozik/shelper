#include "app.h"
#include <media_center/media_center.h>
#include <media_center/adapters/kodi_adapter.h>
#include <interop_mgr.h>
#include <subtitles/fetcher.h>

using namespace shelper;

app* g_app = nullptr;
app* shelper::app_instance() {
	return g_app;
}


app::app(const downloader_ptr& downloader, const fs::file_system_ptr& fs):
m_downloader(downloader),
m_fs(fs)
{
	g_app = this;
}

void app::init() {
	m_fetcher = std::make_shared<sub::fetcher>();
	
	m_interop = std::make_shared<interop_mgr>();
	m_interop->init();

	auto mc = std::make_shared<media_center::kodi_adapter>();
	m_interop->set_media_center(mc);

	mc->set_host("192.168.1.213:1234");
}

void app::handle_timer(long time_ms) {
	m_interop->handle_timer(time_ms);
}
