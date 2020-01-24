
#include "interop_mgr.h"
#include <media_center.h>
#include <subtitles/subtitles.h>
#include <output/output.h>

namespace shelper {


interop_mgr::interop_mgr() {
    m_subtitles.reset(new sub::subtitles());
}

void interop_mgr::load_subtitles() {
    m_subtitles->load_srt("");
}

bool interop_mgr::find_sub(const media_center::track_info& ti, sub::subtitles_entry& entry) {
    
    unsigned start = 0;
    unsigned end = m_subtitles->m_entries.size();
    unsigned cur = 0;
    unsigned last_cur = std::numeric_limits<unsigned>::max();

    auto val = ti.current_time;
    while (1) {
        cur = start + (end - start) / 2;
        if (cur == last_cur) {
            entry = m_subtitles->m_entries.at(cur);
            break;
        }
        last_cur = cur;

        auto curr_val = m_subtitles->m_entries.at(cur).start;
        if (curr_val < val) { //turn right
            start = cur;
        }
        else
        if (curr_val > val) { //turn left
            end = cur;
        } else {
            entry = m_subtitles->m_entries.at(cur);
        }
    }
    
    return ti.current_time >= entry.start && ti.current_time <= entry.end;
}

void interop_mgr::handle_timer(long time_ms) {
    
    
    static int time_counter = 0;
    time_counter += time_ms == 0 ? 1 : time_ms;
    if (time_counter >= 50) {
        time_counter = 0;
    
        media_center::player_info i;
        if (m_mc_adapter->get_player_info(i)) {
            
            media_center::track_info ti;
            if (m_mc_adapter->get_current_track_info(ti, i.player_id)) {
                
                sub::subtitles_entry sub;
                if (find_sub(ti, sub)) {
                    static unsigned last_sub_id = std::numeric_limits<unsigned>::max();
                    if (sub.s_id != last_sub_id) {
                     
                        m_output->set_text(sub.text);
                        last_sub_id = sub.s_id;
                    }
                }
            }
        }
    }
}

}
