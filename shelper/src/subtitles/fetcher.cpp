#include "fetcher.h"
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>
#include <json.hpp>
#include <downloader.h>
#include <app.h>
#include <zstr.hpp>

using namespace shelper::sub;
using namespace nlohmann;

std::shared_ptr<fetcher::subtitles_list> fetcher::get_subtitles_list() {
//	if (m_mtx.try_lock()) {
		return m_subtitles;
//	}
//	return std::shared_ptr<fetcher::subtitles_list>();
}

void fetcher::request_subtitles_list(const std::string& str, const request_callbacks& clbs) {
	// curl -A 'TemporaryUserAgent'  https://rest.opensubtitles.org/search/query-shawshank/sublanguageid-eng
	if (m_subtitles)
		m_subtitles->clear();
	
	net::request_params params;
	params.url = "https://rest.opensubtitles.org/search";
	params.url += "/query-" + str;
	params.url += "/sublanguageid-eng";
	
	params.user_agent = "TemporaryUserAgent";
	params.clbs.error = [] (auto) {
		assert(false);
	};
	
	params.clbs.complete = [this, clbs] (auto raw) {
		json arr = json::parse(raw);
		subtitle_list_element element;
		m_subtitles = std::make_shared<subtitles_list>();
		for (auto i : arr) {
			element.name = i["MovieName"];
			element.url = i["SubDownloadLink"];
			element.score = i["Score"];
			m_subtitles->push_back(element);
		}
		clbs.complete();
	};
	app_instance()->downloader()->request(params);

}

void fetcher::download_subtitle_data(const std::string& url, const request_callbacks& clbs) {
	net::request_params params;
	params.url = url;
	params.user_agent = "TemporaryUserAgent";
	params.clbs.error = [] (auto) {
		assert(false);
	};
	
	params.clbs.complete = [clbs] (const std::string& raw) {
		std::stringstream s;
		s << raw;
		auto size = raw.size();
		zstr::istreambuf zsbuf(s.rdbuf(), 1<<16, true);
		std::istream is(&zsbuf);
		is.exceptions(std::ios_base::badbit);
		const std::streamsize buff_size = 1 << 16;
		char * buff = new char [buff_size];
		while (true) {
			is.read(buff, buff_size);
			std::streamsize cnt = is.gcount();
			if (cnt == 0)
				break;
			std::cout.write(buff, cnt);
		}
		delete [] buff;
		
		clbs.complete();
	};
	app_instance()->downloader()->request(params);
}
