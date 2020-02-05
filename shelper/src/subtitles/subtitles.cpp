#include "subtitles.h"
#include <chrono>
#include <regex>
#include <fstream>

namespace shelper {
namespace sub {


bool subtitles::load_srt(const std::string& sub) {

    m_entries.clear();
    m_entries.reserve(2500); //average value of entries of regular movie
    
    std::regex reg("((:|,)+| --> )");
    std::ifstream input(sub);
    std::string line;
    while (std::getline(input, line)) {
        auto entry = std::make_shared<subtitles_entry>();
        { std::stringstream s(line); s >> entry->s_id;  }
        
        std::getline(input, line);
        {
            std::regex_token_iterator<std::string::iterator> iter(line.begin(), line.end(), reg, -1);
            std::regex_token_iterator<std::string::iterator> end;
            
            { std::stringstream s((iter++)->str()); unsigned val; s >> val; entry->start.hours = std::chrono::hours(val); }
            { std::stringstream s((iter++)->str()); unsigned val; s >> val; entry->start.mins = std::chrono::minutes(val); }
            { std::stringstream s((iter++)->str()); unsigned val; s >> val; entry->start.secs = std::chrono::seconds(val); }
            { std::stringstream s((iter++)->str()); unsigned val; s >> val; entry->start.ms = std::chrono::milliseconds(val); }
            
            { std::stringstream s((iter++)->str()); unsigned val; s >> val; entry->end.hours = std::chrono::hours(val); }
            { std::stringstream s((iter++)->str()); unsigned val; s >> val; entry->end.mins = std::chrono::minutes(val); }
            { std::stringstream s((iter++)->str()); unsigned val; s >> val; entry->end.secs = std::chrono::seconds(val); }
            { std::stringstream s((iter++)->str()); unsigned val; s >> val; entry->end.ms = std::chrono::milliseconds(val); }
        }
        
        while (std::getline(input, line) && !line.empty() && line.at(0) != '\r' ) {
            entry->text += line;
            line = "";
        }
        
        m_entries.push_back(std::move(entry));
    }
    
    return m_entries.size() != 0;
}

}
}
