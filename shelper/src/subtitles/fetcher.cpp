#include "fetcher.h"
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>
#include <json.hpp>
#include <future>

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

	std::async(std::launch::async, [this, str, clbs] {
		try {
			std::scoped_lock<std::mutex> lock(m_mtx);
			
			curlpp::Cleanup cleaner;
			curlpp::Easy request;
			
			std::string url = "https://rest.opensubtitles.org/search";
			url += "/query-" + curlpp::escape(str);
			url += "/sublanguageid-eng";
	
			request.setOpt(new curlpp::options::Url(url));
			request.setOpt(new curlpp::options::Verbose(true));
			request.setOpt(new curlpp::options::UserAgent("TemporaryUserAgent"));
			request.setOpt(new curlpp::options::SslVerifyPeer(false));

			std::ostringstream os;
			os << request;
			json arr = json::parse(os.str());
			subtitle_list_element element;
			m_subtitles = std::make_shared<subtitles_list>();
			for (auto i : arr) {
				element.name = i["MovieName"];
				element.url = i["ZipDownloadLink"];
				element.score = i["Score"];
				m_subtitles->push_back(element);
			}
			
			clbs.complete();
		} catch (const std::exception& e) {
			// todo:
			clbs.error(-1);
		}
	});
}

