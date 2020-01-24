#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include <defs.h>

namespace shelper {
namespace output {

class base_output {
public:
    virtual void set_text(const std::string& text) = 0;
};
using output_ptr = std::shared_ptr<base_output>;

}}

#endif _OUTPUT_H_
