#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include <defs.h>

namespace shelper {
namespace output {

class base_output {
public:
    virtual ~base_output() = default;
    virtual void set_text(const std::string& text) = 0;
    virtual void set_translation(const std::string& text, const std::string& translation, const std::string& context) = 0;
};
using output_ptr = std::shared_ptr<base_output>;


class console_output : public base_output {
public:
    virtual ~console_output() = default;
    void set_text(const std::string& text) override {
        std::cout << ">>>>> subtitles" << std::endl;
        std::cout << text << std::endl;
        std::cout << "<<<<< subtitles" << std::endl;
    }

    void set_translation(const std::string& text, const std::string& translation, const std::string& context) override {
        std::cout << ">>>>> translation" << std::endl;
        std::cout << text << " " << translation << " " << context << std::endl;
        std::cout << "<<<<< translation" << std::endl;
    }
};

}}


#endif // _OUTPUT_H_
