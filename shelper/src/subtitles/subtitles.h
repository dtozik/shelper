#ifndef _SUBTITLES_H_
#define _SUBTITLES_H_

#include <defs.h>
#include <time_routines.h>

namespace shelper {
namespace sub {

using subtitles_time_range_t = std::pair<time::time_info, time::time_info>;

struct subtitles_entry {
    time::time_info start;
    time::time_info end;
    std::string text;
	subtitles_entry_wptr prev;
    unsigned s_id;
};
using subtitles_entry_ptr = std::shared_ptr<subtitles_entry>;
using subtitles_entry_wptr = std::weak_ptr<subtitles_entry>;
using subtitles_entries_t = std::vector<subtitles_entry_ptr>;


class subtitles final {
    subtitles_entries_t m_entries;
public:
    bool load_srt(const std::string& sub);
    subtitles_entry_ptr get_entry(size_t ind) const {
        return m_entries[ind];
    }
    
    size_t count() const {
        return m_entries.size();
    }

};
using subtitles_ptr = std::shared_ptr<subtitles>;


}
}

#endif // _SUBTITLES_H_
