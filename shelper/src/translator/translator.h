#ifndef _TRANSLATOR_H_
#define _TRANSLATOR_H_

#include <defs.h>
#include <json.hpp>

using namespace nlohmann;

namespace shelper { namespace translator {

struct translation_cfg {
    std::string word;
};

struct translation_result {
    json base_json;
};


using translation_complete_clb_t = std::function<void(const translation_result&)>;
using translation_error_clb_t = std::function<void(int)>;

struct translation_callbacks {
    translation_complete_clb_t complete;
    translation_error_clb_t error;
};

class translator_base {
public:
    virtual bool translate(const translation_cfg& cfg, const translation_callbacks& clbs) = 0;
	virtual bool store(const std::string& text, const std::string& value, const std::string& ctx) = 0;
};
using translator_base_ptr = std::shared_ptr<translator_base>;

class lingualeo_translator final : public translator_base {
	std::string m_cookie;
private:
	bool login();
public:
    bool translate(const translation_cfg& cfg,
                   const translation_callbacks& clbs) override;
	
	bool store(const std::string& text, const std::string& value, const std::string& ctx) override;
};


}}


#endif // _TRANSLATOR_H_
