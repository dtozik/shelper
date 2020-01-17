
#include <media_center.h>
#include <kodi_adapter.h>
#include <interop_mgr.h>
#include <chrono>

int main(int argc, const char * argv[]) {
    
    {
        shelper::media_center::media_center_adapter_ptr adapter = std::make_shared<shelper::media_center::kodi_adapter>();
        adapter->set_host("localhost", 1234);
        
        
        shelper::interop_mgr interop;
        
        auto t0 = std::chrono::high_resolution_clock::now();
        //std::chrono::duration<unsigned> last.
        while (1) {
            auto t1 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> fs = t1 - t0;
            t0 = t1;
            interop.handle_timer(std::chrono::duration_cast<std::chrono::milliseconds>(fs).count());
        }
    }
    
    
	return 0;
}
