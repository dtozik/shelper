#include "downloader.h"
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>
#include <future>

using namespace shelper::net;

#define MAX_FILE_LENGTH 20000
class WriterMemoryClass
{
public:
	// Helper Class for reading result from remote host
	WriterMemoryClass()
	{
		this->m_pBuffer = NULL;
		this->m_pBuffer = (char*) malloc(MAX_FILE_LENGTH * sizeof(char));
		this->m_Size = 0;
	};

	~WriterMemoryClass()
	{
		if (this->m_pBuffer)
			free(this->m_pBuffer);
	};

	void* Realloc(void* ptr, size_t size)
	{
		if(ptr)
			return realloc(ptr, size);
		else
			return malloc(size);
	};

	// Callback must be declared static, otherwise it won't link...
	size_t WriteMemoryCallback(char* ptr, size_t size, size_t nmemb)
	{
		// Calculate the real size of the incoming buffer
		size_t realsize = size * nmemb;

		// (Re)Allocate memory for the buffer
		m_pBuffer = (char*) Realloc(m_pBuffer, m_Size + realsize);

		// Test if Buffer is initialized correctly & copy memory
		if (m_pBuffer == NULL) {
			realsize = 0;
		}

		memcpy(&(m_pBuffer[m_Size]), ptr, realsize);
		m_Size += realsize;


		// return the real size of the buffer...
		return realsize;
	};


	void print()
	{
		std::cout << "Size: " << m_Size << std::endl;
		std::cout << "Content: " << std::endl << m_pBuffer << std::endl;
	}

	// Public member vars
	char* m_pBuffer;
	size_t m_Size;
};

downloader::downloader(const main_thread_callback_t& mt_func) : m_mt_func(mt_func) {
}

bool downloader::request(const request_params& params) {
	std::async(std::launch::async, [this, params] {
		try {
			curlpp::Cleanup cleaner;
			curlpp::Easy request;
			
			request.setOpt(new curlpp::options::Url(params.url));
			request.setOpt(new curlpp::options::Verbose(true));
			if (!params.user_agent.empty())
				request.setOpt(new curlpp::options::UserAgent(params.user_agent));
			request.setOpt(new curlpp::options::SslVerifyPeer(false));

			std::ostringstream os;
			request.setOpt(new curlpp::options::WriteStream(&os));
			request.perform();
			
			if (m_mt_func)
				m_mt_func([params, s = os.str()]() {
					if (params.clbs.complete) {
						params.clbs.complete(s);
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

