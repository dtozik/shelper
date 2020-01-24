#ifndef _CONSOLE_OUTPUT_H_
#define _CONSOLE_OUTPUT_H_

#include <defs.h>
#include <output.h>

namespace shelper {
namespace output {

class console_output : public base_output {
public:
    void set_text(const std::string& text) {
        std::cout << text << std::endl;
        std::cout << "----------------------" << std::endl;
    }
};

}}

#endif //_CONSOLE_OUTPUT_H_
