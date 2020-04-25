
#include "interop_mgr.h"
#include <media_center.h>
#include <subtitles/subtitles.h>
#include <output.h>
#include <translator.h>
#include <subtitles/fetcher.h>

namespace shelper {



void interop_mgr::init() {
    m_subtitles = std::make_shared<sub::subtitles>();
    m_translator = std::make_shared<translator::lingualeo_translator>();
}

void interop_mgr::on_play() {
    m_mc_adapter->play();
}

void interop_mgr::on_pause() {
    m_mc_adapter->pause();
}

void interop_mgr::on_stop() {
    m_mc_adapter->stop();
}

void interop_mgr::on_seek(const time::time_info& time) {
    
}

void interop_mgr::on_backward() {
	media_center::track_info ti;
	if (m_mc_adapter->get_current_track_info(ti)) {
		sub::subtitles_entry_ptr sub = find_sub(ti);
		if (sub) {
//			auto range = (sub->end.mins.count() + sub->end.secs.count()) -
//				(sub->start.mins.count() + sub->start.secs.count());
//			auto t = ti.current_time.secs.count() - sub->start.secs.count();
//
//			sub::subtitles_entry_wptr p;
//			if (t / static_cast<double>(range) < 0.5) {
//				p = p.lock()->prev;
//			} else {
//				p = sub;
//			}
			sub::subtitles_entry_wptr p = sub;
			if (!sub->prev.expired())
				p = sub->prev;
			//if (m_output)
			//	m_output->set_text(p.lock()->text);
			time::time_info t = p.lock()->start;
			t.secs = t.secs - std::chrono::seconds(2);
			m_mc_adapter->seek(t);
		} else {
			if (!m_last_sub.expired()) {
				auto last = m_last_sub.lock();
				//if (m_output)
				//	m_output->set_text(last->text);
				time::time_info t = last->start;
				t.secs = t.secs - std::chrono::seconds(2);
				m_mc_adapter->seek(t);
			}
		}
	}
}


void interop_mgr::on_forward() {
}


void interop_mgr::on_select_text(const std::string& text) {
    //on_pause();
    if (m_translator && m_output && text.size()) {
        translator::translation_cfg cfg;
        cfg.word = text;
        
        translator::translation_callbacks clbs;
        clbs.complete = [&](const translator::translation_result& result) {
            m_output->set_translation(result.base_json["translate"].at(0)["translate_value"]);
        };
        clbs.error = [](int err){};
        m_translator->translate(cfg, clbs);
    }
}

void interop_mgr::load_subtitles(const std::string& file) {
    if (!m_subtitles || !m_subtitles->load_srt(file))
        throw std::runtime_error("subtitles is null");
}

sub::subtitles_entry_ptr interop_mgr::find_sub(const media_center::track_info& ti) const {
    
    if (!m_subtitles)
        return sub::subtitles_entry_ptr();
        

    unsigned start = 0;
    unsigned end = m_subtitles->count();
    if (end == 0)
        return nullptr;
    
    unsigned cur = 0;
    unsigned last_cur = std::numeric_limits<unsigned>::max();

    auto val = ti.current_time;
    sub::subtitles_entry_ptr result;
    
    // binary search
    while (1) {
        cur = start + (end - start) / 2;
        if (cur == last_cur) {
            result = m_subtitles->get_entry(cur);
            break;
        }
        last_cur = cur;

        auto curr_val = m_subtitles->get_entry(cur)->start;
        if (curr_val < val) { //turn right
            start = cur;
        }
        else
        if (curr_val > val) { //turn left
            end = cur;
        } else {
            result = m_subtitles->get_entry(cur);
        }
    }
    
    if (result && ti.current_time >= result->start && ti.current_time <= result->end)
        return result;
    
    return nullptr;
}

void interop_mgr::handle_timer(long time_ms) {
    
    static int time_counter = 0;
    time_counter += time_ms == 0 ? 1 : time_ms;
    if (time_counter >= 50) {
        time_counter = 0;
    
        media_center::track_info ti;
        if (m_mc_adapter->get_current_track_info(ti)) {
            
            sub::subtitles_entry_ptr sub = find_sub(ti);
            if (sub) {
                if (m_last_sub.expired() || sub->s_id != m_last_sub.lock()->s_id) {
                    if (m_output)
                        m_output->set_text(sub->text);
                    m_last_sub = sub;
                }
            }
        }
    }
}

}
