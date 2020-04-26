#include "downloader.h"
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>
#include <future>

using namespace shelper::net;

downloader::downloader(const main_thread_callback_t& mt_func) : m_mt_func(mt_func) {
}

bool downloader::request(const request_params& params) {
	std::async(std::launch::async, [this, params] {
		try {
			curlpp::Cleanup cleaner;
			curlpp::Easy request;
			
			request.setOpt(new curlpp::options::Url("https://dl.opensubtitles.org/en/download/src-api/vrf-19bc0c53/filead/1955363451.gz"));
			request.setOpt(new curlpp::options::Verbose(true));
			if (!params.user_agent.empty())
				request.setOpt(new curlpp::options::UserAgent(params.user_agent));
			request.setOpt(new curlpp::options::SslVerifyPeer(false));

			std::ostringstream os;
			request.setOpt(new curlpp::options::WriteStream(&os));
			request.perform();
						
			std::string s = os.str();
			auto p = s.size();
			if (m_mt_func)
				m_mt_func([params, s]() {
					if (params.clbs.complete) {
					
						params.clbs.complete((char*)s.c_str());
					}
				});
			
		} catch (const std::exception& e) {
			if (m_mt_func)
				m_mt_func([params]() {
					if (params.clbs.error)
						params.clbs.error(-1);
				});
		}
	});
	return true;
}

