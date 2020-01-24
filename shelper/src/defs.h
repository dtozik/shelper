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
}

}

#endif // _DEFS_H_
