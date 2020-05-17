#include "fetcher.h"
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>
#include <json.hpp>
#include <downloader.h>
#include <app.h>
#include <zstr.hpp>
#include <fs/file_system.h>

using namespace shelper::sub;
using namespace nlohmann;

static inline void ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
		return !std::isspace(ch);
	}));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
	}).base(), s.end());
}


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
	
	std::string s = str;
	ltrim(s);
	rtrim(s);
	
	net::request_params params;
	params.url = "https://rest.opensubtitles.org/search";
	params.url += "/query-" + s;
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
		clbs.complete(nullptr);
	};
	app_instance()->downloader()->request(params);

}

 unsigned int unpack(const unsigned char* raw, unsigned int size, unsigned char** b) {
	 std::stringstream s;
	 std::string ss;
	 ss.assign(reinterpret_cast<char*>(const_cast<unsigned char*>(raw)), size);
	 s << ss;
	
	zstr::istreambuf zsbuf(s.rdbuf(), 1<<16, true);
	std::istream is(&zsbuf);
	is.exceptions(std::ios_base::badbit);
	const std::streamsize buff_size = 32768;
	unsigned char * buff = new unsigned char[buff_size];
	unsigned int size_total = 0;
	while (true) {
		is.read(reinterpret_cast<char*>(buff), buff_size);
		std::streamsize cnt = is.gcount();
		if (cnt == 0)
			break;
		
		if (size_total) {
			unsigned char* tmp = new unsigned char[size_total];
			::memcpy(tmp, *b, size_total);
			delete [] *b;
			*b = new unsigned char[size_total+cnt];
			::memcpy(*b, tmp, size_total);
			::memcpy(*b+size_total, buff, cnt);
			delete [] tmp;
		} else {
			*b = new unsigned char[cnt];
			::memcpy(*b, buff, cnt);
		}
		size_total += cnt;
	}
	return size_total;
}

void fetcher::request_subtitle_data(const std::string& url, const request_callbacks& clbs) {
	
	std::hash<std::string> hash;
	auto hs = std::to_string(hash(url));	
				   
	auto dir = app_instance()->fs()->get_dir(fs::dir_type::cache);
	auto ds = app_instance()->fs()->open_read_file(dir + '/' + hs);
	if (ds) {
		ds->seek(0, fs::seek_offset_type::from_end);
		auto size = ds->tell();
		ds->seek(0, fs::seek_offset_type::from_begin);
		unsigned char* buf = new unsigned char[size];
		ds->read(buf, size);
		unsigned char* ext_buf;
		auto ret = unpack(buf, size, &ext_buf);
		if (ret) {
			clbs.complete(std::shared_ptr<char>(reinterpret_cast<char*>(ext_buf), std::default_delete<char[]>()));
		}
		else {
			clbs.error(-1);
		}
		return;
	}
	
	net::request_params params;
	params.url = url;
	params.user_agent = "TemporaryUserAgent";
	params.clbs.error = [] (auto) {
		assert(false);
	};
	
	params.clbs.complete = [=] (const std::string& raw) {
		auto ws = app_instance()->fs()->open_write_file(dir + '/' + hs);
		unsigned char buf[raw.size()];
		std::copy(raw.begin(), raw.end(), buf);
		auto size = raw.size() * sizeof(unsigned char);
		ws->write(buf, size);
		ws->flush();
			
		unsigned char* ext_buf;
		auto ret = unpack(buf, size, &ext_buf);
		if (ret) {
			clbs.complete(std::shared_ptr<char>(reinterpret_cast<char*>(ext_buf), std::default_delete<char[]>()));
		} else {
			clbs.error(-1);
		}
	};
	app_instance()->downloader()->request(params);
}
