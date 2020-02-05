#ifndef _DEFS_H_
#define _DEFS_H_

#include <memory>
#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

namespace shelper {

namespace output {
class base_output;
using output_ptr = std::shared_ptr<base_output>;
}

namespace media_center {
class media_center_adapter;
using media_center_adapter_ptr = std::shared_ptr<media_center_adapter>;
}

namespace sub {
class subtitles;
using subtitles_ptr = std::shared_ptr<subtitles>;

struct subtitles_entry;
using subtitles_entry_ptr = std::shared_ptr<subtitles_entry>;
}

namespace translator {
class translator_base;
using translator_base_ptr = std::shared_ptr<translator_base>;
}

class interop_mgr;
using interop_mgr_ptr = std::shared_ptr<interop_mgr>;

}

#endif // _DEFS_H_
