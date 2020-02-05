#ifndef _VIEW_H_
#define _VIEW_H_

#include <defs.h>


namespace shelper {
namespace view {


class view_delegate {
public:
    void on_select_text(const std::string& text);
};


class base_view {
    
};


}

}}



#endif // _VIEW_H_
