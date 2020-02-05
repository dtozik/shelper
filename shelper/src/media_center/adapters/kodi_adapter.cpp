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

bool kodi_adapter::get_player_info(player_info& info) const {
    //{"jsonrpc": "2.0", "method": "Player.GetActivePlayers", "id": 1}
    // curl -s --data-binary '{"jsonrpc":"2.0", "method":"Player.Seek", "params": { "playerid":1, "value":{"hours":0,"milliseconds":0,"minutes":20,"seconds":0} }, "id":1}' -H 'content-type: application/json;' http://192.168.1.181:3369/jsonrpc
    
    if (m_cached_player_id >= 0) {
        info.player_id = m_cached_player_id;
        return true;
    }
    try {
        curlpp::Cleanup cleaner;
        curlpp::Easy request;
        request.setOpt(new curlpp::options::Url(m_server_path.first + "/jsonrpc"));
        request.setOpt(new curlpp::options::Port(m_server_path.second));
        //request.setOpt(new curlpp::options::Verbose(true));

        std::list<std::string> header;
        header.push_back("content-type: application/json");
        request.setOpt(new curlpp::options::HttpHeader(header));
        {
            json j;
            j["jsonrpc"] = "2.0";
            j["method"] = "Player.GetActivePlayers";
            j["id"] = "1";
            std::string field(j.dump());
            request.setOpt(new curlpp::options::PostFields(field));
            request.setOpt(new curlpp::options::PostFieldSize(field.size() + 1));
        }
        //request.setOpt(new curlpp::options::UserPwd("user:password"));
        std::ostringstream os;
        os << request;
        json j = json::parse(os.str());
        for (auto& player : j["result"]) {
            if (player["type"] == "video") {
                info.player_id = player["playerid"];
                m_cached_player_id = info.player_id;
                return true;
            }
        }
    }
    catch ( std::exception& e ) {
        std::cout << "coudn't get kodi_adapter::get_player_info, exception: " << e.what() << std::endl;
        return false;
    }
    
    return false;
};


bool kodi_adapter::get_current_track_info(track_info& info, unsigned player_id) const {

//    curl -s --data '{"jsonrpc":"2.0", "method":"Player.GetProperties", "params": { "playerid":1, "properties":["type",
//    "speed",
//    "time",
//    "percentage",
//    "totaltime",
//    "canseek",
//    "canchangespeed"] }, "id":"qq"}' -H 'content-type: application/json;' localhost:1234/jsonrpc
    
    try {
        curlpp::Cleanup cleaner;
        curlpp::Easy request;
        request.setOpt(new curlpp::options::Url(m_server_path.first + "/jsonrpc"));
        request.setOpt(new curlpp::options::Port(m_server_path.second));
        //request.setOpt(new curlpp::options::Verbose(true));

        std::list<std::string> header;
        header.push_back("content-type: application/json");
        request.setOpt(new curlpp::options::HttpHeader(header));
        {
            json j;
            j["jsonrpc"] = "2.0";
            j["method"] = "Player.GetProperties";
            json params;
            params["playerid"] = m_cached_player_id;
            params["properties"] = { "type", "speed", "time", "percentage", "totaltime", "canseek", "canchangespeed" };
            j["params"] = params;
            j["id"] = "1";
            std::string field(j.dump());
            request.setOpt(new curlpp::options::PostFields(field));
            request.setOpt(new curlpp::options::PostFieldSize(field.size() + 1));
        }
        //request.setOpt(new curlpp::options::UserPwd("user:password"));
        std::ostringstream os;
        os << request;
        json j = json::parse(os.str());
        json result = json::parse(os.str())["result"];
        info.can_seek = result["canseek"];
        info.can_change_speed = result["canchangespeed"];
        info.speed = result["speed"];
        info.percentage = result["percentage"];
          
        json time = result["time"];
        info.current_time.ms = std::chrono::milliseconds(time["milliseconds"]);
        info.current_time.secs = std::chrono::seconds(time["seconds"]);
        info.current_time.mins = std::chrono::minutes(time["minutes"]);
        info.current_time.hours = std::chrono::hours(time["hours"]);
        
        time = result["totaltime"];
        info.total_time.ms = std::chrono::milliseconds(time["milliseconds"]);
        info.total_time.secs = std::chrono::seconds(time["seconds"]);
        info.total_time.mins = std::chrono::minutes(time["minutes"]);
        info.total_time.hours = std::chrono::hours(time["hours"]);
    }
    catch ( std::exception& e ) {
        std::cout << "coudn't get kodi_adapter::get_current_track_info, exception: " << e.what() << std::endl;
        return false;
    }
    
    return true;
}


}}
