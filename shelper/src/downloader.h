#ifndef _DOWNLADER_H_
#define _DOWNLADER_H_

#include <defs.h>

namespace shelper::net {

struct request_callbacks {
	std::function<void(char*)> complete;
	std::function<void(int)> error;
};

struct request_params {
	std::string url;
	std::string user_agent;
	request_callbacks clbs;
};

using main_thread_callback_t = std::function<void(std::function<void()>)>;

class downloader final {
	main_thread_callback_t m_mt_func;
public:
	explicit downloader(const main_thread_callback_t& mt_func);
	bool request(const request_params& params);

};
using downloader_ptr = std::shared_ptr<downloader>;

}

#endif // _DOWNLADER_H_
