#ifndef _INTEROP_MGR_H_
#define _INTEROP_MGR_H_

#include <defs.h>

namespace shelper {

namespace media_center { struct track_info; }
namespace sub { struct subtitles_entry; }



class interop_mgr final {
    output::output_ptr                      m_output;
    media_center::media_center_adapter_ptr  m_mc_adapter;
    sub::subtitles_ptr                      m_subtitles;
    translator::translator_base_ptr         m_translator;
public:
    void init();
    void handle_timer(long time_ms);
    void set_output(const output::output_ptr& output) {
        m_output = output;
    }
    
    void set_media_center(const media_center::media_center_adapter_ptr& adapter) {
        m_mc_adapter = adapter;
    }
    
    void set_translator(const translator::translator_base_ptr& translator) {
        m_translator = translator;
    }

    sub::subtitles_entry_ptr find_sub(const media_center::track_info& ti) const;
    void load_subtitles(const std::string& file);
    
    // view events
    void on_select_text(const std::string& text);
    void on_pause();
    void on_resume();
    void on_seek();
};
using interop_mgr_ptr = std::shared_ptr<interop_mgr>;

}

#endif // _INTEROP_MGR_H_
