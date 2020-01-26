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
    unsigned s_id;
};

using subtitles_entries_t = std::vector<subtitles_entry>;


class subtitles {
public:
    subtitles_entries_t m_entries;
public:
    void load_srt(const std::string& sub);
};
using subtitles_ptr = std::shared_ptr<subtitles>;


}
}

#endif // _SUBTITLES_H_
