
#include <media_center.h>
#include <kodi_adapter.h>
#include <interop_mgr.h>
#include <output.h>
#include <time_routines.h>
#include <chrono>


int main(int argc, char * argv[]) {
    
    if (argc != 3) {
        std::cout << "please specify remote address of media center and path to srt file like: \'shelper localhost:1234 /path/to/example.srt\' " << std::endl;
        return -1;
    }
    
    try
    {
        shelper::media_center::media_center_adapter_ptr adapter = std::make_shared<shelper::media_center::kodi_adapter>();
        adapter->set_host(std::string(argv[1]));
        
        shelper::interop_mgr interop;
        interop.init();
        interop.set_output(std::make_shared<shelper::output::console_output>());
        interop.set_media_center(adapter);
        interop.load_subtitles(std::string(argv[2]));
        
        auto t0 = std::chrono::steady_clock::now();
        while (1) {
            auto t1 = std::chrono::steady_clock::now();
            std::chrono::duration<double> fs = t1 - t0;
            t0 = t1;
            interop.handle_timer(std::chrono::duration_cast<std::chrono::milliseconds>(fs).count());
        }
    } catch (const std::exception& e) {
        std::cout << "the program has been terminated due to exception: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
