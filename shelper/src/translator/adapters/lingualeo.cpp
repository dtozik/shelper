#include <translator.h>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>
#include <json.hpp>

namespace shelper { namespace translator {

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
         std::cout << "couldn't get tarnsalation from lingualeo_translator, exception: " << e.what() << std::endl;
         clbs.error(-1);
         return false;
     }
     clbs.error(-1);
     return false;
}


}}
