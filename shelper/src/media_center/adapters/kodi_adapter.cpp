#include "kodi_adapter.h"

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>
#include <json.hpp>

#define DEFINE_REQUEST_HEADER() \
    curlpp::Cleanup cleaner; \
    curlpp::Easy request; \
    request.setOpt(new curlpp::options::Url(m_server_path.first + "/jsonrpc")); \
    request.setOpt(new curlpp::options::Port(m_server_path.second)); \
    /*request.setOpt(new curlpp::options::Verbose(true)); */ \
    std::list<std::string> header; \
    header.emplace_back("content-type: application/json"); \
    request.setOpt(new curlpp::options::HttpHeader(header));

using namespace nlohmann;

namespace shelper {
namespace media_center {

bool kodi_adapter::play_pause_impl() const {
    // {"jsonrpc": "2.0", "method": "Player.PlayPause", "params": { "playerid": 0 }, "id": 1}
    player_info pi;
    if (!get_player_info(pi)) {
        std::cout << "kodi_adapter::play_pause_impl coundn't get get_player_info" << std::endl;
        return false;
    }
    
    try {
        DEFINE_REQUEST_HEADER()
        {
            json j;
            j["jsonrpc"] = "2.0";
            j["method"] = "Player.PlayPause";
            json params;
            params["playerid"] = pi.player_id;
            j["params"] = params;
            j["id"] = "1";
            std::string field(j.dump());
            request.setOpt(new curlpp::options::PostFields(field));
            request.setOpt(new curlpp::options::PostFieldSize(field.size() + 1));
        }
        std::ostringstream os;
        os << request;
        json j = json::parse(os.str());
        if (j.contains("error")) {
            throw std::runtime_error(j["error"]["message"]);
        }
    }
    catch ( std::exception& e ) {
        std::cout << "couldn't call kodi_adapter::play_pause_impl, exception: " << e.what() << std::endl;
        return false;
    }
    
    return true;
}

bool kodi_adapter::is_playing() const {
    track_info info;
    if (!get_current_track_info(info)) {
        std::cout << "couldn't call kodi_adapter::is_playing" << std::endl;
        return false;
    }
    return info.speed != 0;
}

bool kodi_adapter::play() const {
    track_info info;
    if (!get_current_track_info(info)) {
        std::cout << "couldn't call kodi_adapter::play" << std::endl;
        return false;
    }
    
    if (info.speed > 0)
        return false;
    
    return play_pause_impl();
}

bool kodi_adapter::pause() const {
    track_info info;
    if (!get_current_track_info(info)) {
        std::cout << "couldn't call kodi_adapter::pause" << std::endl;
        return false;
    }
    
    if (info.speed == 0)
        return false;
    
    return play_pause_impl();
}

bool kodi_adapter::stop() const {
    player_info pi;
    if (!get_player_info(pi)) {
        std::cout << "kodi_adapter::stop couldn't kodi::get_player_info" << std::endl;
        return false;
    }
    
    try {
        DEFINE_REQUEST_HEADER()
        {
            json j;
            j["jsonrpc"] = "2.0";
            j["method"] = "Player.Stop";
            json params;
            params["playerid"] = pi.player_id;
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
        if (j.contains("error")) {
            throw std::runtime_error(j["error"]["message"]);
        }
    }
    catch ( std::exception& e ) {
        std::cout << "coudn't get kodi_adapter::stop, exception: " << e.what() << std::endl;
        return false;
    }
    
    return false;

}
bool kodi_adapter::seek() const {
    return false;
}

bool kodi_adapter::get_player_info_impl(player_info& info) const {
    //{"jsonrpc": "2.0", "method": "Player.GetActivePlayers", "id": 1}
    // curl -s --data-binary '{"jsonrpc":"2.0", "method":"Player.Seek", "params": { "playerid":1, "value":{"hours":0,"milliseconds":0,"minutes":20,"seconds":0} }, "id":1}' -H 'content-type: application/json;' http://192.168.1.181:3369/jsonrpc
    
    try {
        DEFINE_REQUEST_HEADER()
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
        if (j.contains("error")) {
            throw std::runtime_error(j["error"]["message"]);
        }
        
        for (auto& player : j["result"]) {
            if (player["type"] == "video") {
                info.player_id = player["playerid"];
                m_cached_player_id = info.player_id;
                return true;
            }
        }
    }
    catch ( std::exception& e ) {
        std::cout << "coudn't get kodi_adapter::get_player_info_impl, exception: " << e.what() << std::endl;
        return false;
    }
    
    return false;

}

bool kodi_adapter::get_player_info(player_info& info) const {
    if (m_cached_player_id >= 0) {
        info.player_id = m_cached_player_id;
        return true;
    } else
        return get_player_info_impl(info);
};


bool kodi_adapter::get_current_track_info(track_info& info) const {

//    curl -s --data '{"jsonrpc":"2.0", "method":"Player.GetProperties", "params": { "playerid":1, "properties":["type",
//    "speed",
//    "time",
//    "percentage",
//    "totaltime",
//    "canseek",
//    "canchangespeed"] }, "id":"qq"}' -H 'content-type: application/json;' localhost:1234/jsonrpc
    player_info pi;
    if (!get_player_info(pi)) {
       std::cout << "kodi_adapter::get_current_track_info couldn't get get_player_info" << std::endl;
       return false;
    }
    
    try {
        DEFINE_REQUEST_HEADER()
        {
            json j;
            j["jsonrpc"] = "2.0";
            j["method"] = "Player.GetProperties";
            json params;
            params["playerid"] = pi.player_id;
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
        if (j.contains("error")) {
            throw std::runtime_error(j["error"]["message"]);
        }
        
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
        std::cout << "couldn't get kodi_adapter::get_current_track_info, exception: " << e.what() << std::endl;
        return false;
    }
    
    return true;
}


}}
