#include <translator.h>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>
#include <curlpp/Infos.hpp>
#include <json.hpp>
#include <user_config.h>

namespace shelper { namespace translator {


bool lingualeo_translator::login() {
	//	{
	//		CURL *curl = curl_easy_init();
	//		if(curl) {
	//		  curl_easy_setopt(curl, CURLOPT_URL, "https://api.lingualeo.com/login");
	//
	//		  /* enable the cookie engine */
	//			curl_easy_setopt(curl, CURLOPT_POST, 1L);
	//		  curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
	//
	//
	//
	//		  struct curl_slist *list = NULL;
	//		  list = curl_slist_append(list, "content-type: application/json;");
	//		  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
	//
	//
	//			  json j;
	//			  j["email"] = "";
	//			  j["password"] = "";
	//			  std::string field(j.dump());
	//			  //std::cout << j.dump();
	//
	//			   /* size of the POST data */
	//			  curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, field.size());
	//
	//			   /* pass in a pointer to the data - libcurl will not copy */
	//			  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, field.c_str());
	//
	//
	//			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	//		  auto res = curl_easy_perform(curl);
	//
	//		  if(!res) {
	//			/* extract all known cookies */
	//			struct curl_slist *cookies = NULL;
	//			res = curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &cookies);
	//			if(!res && cookies) {
	//			  /* a linked list of cookies in cookie file format */
	//			  struct curl_slist *each = cookies;
	//			  while(each) {
	//				printf("%s\n", each->data);
	//				each = each->next;
	//			  }
	//			  /* we must free these cookies when we're done */
	//			  curl_slist_free_all(cookies);
	//			}
	//		  }
	//		  curl_easy_cleanup(curl);
	//		}
	//
	//	}
	//
				
		// curl --cookie-jar testcookie --data '{"email":"dtozik.cr@gmail.com", "password":""}' -H 'content-type: application/json;' https://api.lingualeo.com/login
	try {
		curlpp::Cleanup cleaner;
		curlpp::Easy request;

		request.setOpt(new curlpp::options::Url("https://api.lingualeo.com/login"));
		request.setOpt(new curlpp::options::Post(true));
		request.setOpt(curlpp::options::CookieFile(""));
		request.setOpt(new curlpp::options::Verbose(true));
		request.setOpt(new curlpp::options::SslVerifyPeer(false));
		

		std::list<std::string> header;
		header.push_back("content-type: application/json;");
		request.setOpt(new curlpp::options::HttpHeader(header));
		json j;
		j["email"] = EMAIL;
		j["password"] = PASSWORD;
		std::string field(j.dump());
		//std::cout << j.dump();
		request.setOpt(new curlpp::options::PostFields(field));
		request.setOpt(new curlpp::options::PostFieldSize(field.size()));
		
		request.perform();
		
		std::list<std::string> cookies;
		curlpp::infos::CookieList::get(request, cookies);
		
		if (cookies.size()) {
			m_cookie = cookies.front();
			return true;
		}

		return false;
	 }
	 catch ( std::exception& e ) {
		 std::cout << "couldn't get login to lingualeo_translator, exception: " << e.what() << std::endl;
		 return false;
	 }
	
	return false;
}

bool lingualeo_translator::store(const std::string& text, const std::string& value, const std::string& ctx)  {
	bool logged = true;
	if (m_cookie.empty()) {
		logged = login();
	}
	if (!logged)
		return false;
	
	//curl --cookie testcookie --data '{"word":"uioop", "tword":"ss", "context":"jopa"}' -H 'content-type: application/json;' https://api.lingualeo.com/addword
	try {
		curlpp::Cleanup cleaner;
		curlpp::Easy request;

		request.setOpt(new curlpp::options::Url("https://api.lingualeo.com/addword"));
		request.setOpt(new curlpp::options::Post(true));
		//request.setOpt(curlpp::options::CookieFile(""));
		//request.setOpt(curlpp::options::Cookie(m_cookie));
		request.setOpt(new curlpp::options::Verbose(true));
		request.setOpt(new curlpp::options::SslVerifyPeer(false));
		

		std::list<std::string> header;
		header.push_back("content-type: application/json;");
		request.setOpt(new curlpp::options::HttpHeader(header));
		json j;
		j["word"] = text;
		j["tword"] = value;
		j["context"] = ctx;
		std::string field(j.dump());
		//std::cout << j.dump();
		request.setOpt(new curlpp::options::PostFields(field));
		request.setOpt(new curlpp::options::PostFieldSize(field.size()));
		request.setOpt(curlpp::options::CookieList(m_cookie));
		request.perform();
	
		return true;
	 }
	 catch ( std::exception& e ) {
		 std::cout << "couldn't add word to lingualeo_translator, exception: " << e.what() << std::endl;
		 return false;
	 }
	
	return false;

}

bool lingualeo_translator::translate(const translation_cfg& cfg,
                                      const translation_callbacks& clbs) {
	//https://api.lingualeo.com/gettranslates?word=tables
	try {
		curlpp::Cleanup myCleanup;

		// Creation of the URL option.
		curlpp::Easy c;
		std::string request("https://api.lingualeo.com/gettranslates?word=");
		request += curlpp::escape(cfg.word);
		c.setOpt(new curlpp::options::Url(request));
		c.setOpt(new curlpp::options::SslVerifyPeer(false));

		translation_result result;

		std::ostringstream os;
		os << c;
		result.base_json = json::parse(os.str());
		clbs.complete(result);
		return true;
	}
	catch ( std::exception& e ) {
		std::cout << "couldn't get translation from lingualeo_translator, exception: " << e.what() << std::endl;
		clbs.error(-1);
		return false;
	}
	clbs.error(-1);
	return false;
}


}}
