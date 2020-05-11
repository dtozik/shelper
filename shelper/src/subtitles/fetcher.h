#ifndef _FETCHER_H_
#define _FETCHER_H_

#include <defs.h>

namespace shelper {
namespace sub {

struct subtitle_list_element {
	std::string name;
	std::string url;
	float score;
};

struct request_callbacks {
	std::function<void()> complete;
	std::function<void(int)> error;
};

class fetcher {
public:
	using subtitles_list = std::vector<subtitle_list_element>;
	std::shared_ptr<subtitles_list> m_subtitles;
	std::mutex	m_mtx;
	
public:
	void request_subtitles_list(const std::string& name, const request_callbacks& clbs);
	std::shared_ptr<subtitles_list> get_subtitles_list();
	
	void download_subtitle_data(const std::string& url, const request_callbacks& clbs);
};
using fetcher_ptr = std::shared_ptr<fetcher>;

}}


#endif // _FETCHER_H_

