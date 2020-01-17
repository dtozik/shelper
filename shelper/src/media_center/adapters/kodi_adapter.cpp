#include "kodi_adapter.h"

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>
#include <json.hpp>

using namespace nlohmann;

namespace shelper {
namespace media_center {


void kodi_adapter::play() {
    
}

void kodi_adapter::pause() {
}
void kodi_adapter::stop() {
}
void kodi_adapter::seek() {
}

bool kodi_adapter::get_player_info(player_info& info) {
    //{"jsonrpc": "2.0", "method": "Player.GetActivePlayers", "id": 1}
    // curl -s --data-binary '{"jsonrpc":"2.0", "method":"Player.Seek", "params": { "playerid":1, "value":{"hours":0,"milliseconds":0,"minutes":20,"seconds":0} }, "id":1}' -H 'content-type: application/json;' http://192.168.1.181:3369/jsonrpc
    
    try {
        curlpp::Cleanup cleaner;
        curlpp::Easy request;
      
        request.setOpt(new curlpp::options::Url(m_server_path.first + "/jsonrpc"));
        request.setOpt(new curlpp::options::Port(m_server_path.second));
        request.setOpt(new curlpp::options::Verbose(true));

        std::list<std::string> header;
        header.push_back("content-type: application/json");
        request.setOpt(new curlpp::options::HttpHeader(header));

        std::string field("{\"jsonrpc\": \"2.0\", \"method\": \"Player.GetActivePlayers\", \"id\": 1}");
        request.setOpt(new curlpp::options::PostFields(field));
        request.setOpt(new curlpp::options::PostFieldSize(field.size() + 1));

        //request.setOpt(new curlpp::options::UserPwd("user:password"));
        std::ostringstream os;
        os << request;
        json j = json::parse(os.str());
        // {"id":1,"jsonrpc":"2.0","result":[{"playerid":1,"playertype":"internal","type":"video"}]}
        for (auto& player : j["result"]) {
            if (player["type"] == "video") {
                info.player_id = player["playerid"];
                return true;
            }
        }
        
        
        std::cout << os.str();
    }
    catch ( std::exception& e ) {
        std::cout << e.what() << std::endl;
        return false;
    }
    
    return false;
};

}}
