#ifndef _POSIX_FILE_SYSTEM_H_
#define _POSIX_FILE_SYSTEM_H_

#include <defs.h>
#include "file_system.h"

namespace shelper::fs {

class posix_file_system : public file_system {
public:
	posix_file_system(const std::string& cache_dir,
					  const std::string& data_dir,
					  const std::string& doc_dir) :
	m_cache_dir(cache_dir),
	m_data_dir(data_dir),
	m_docs_dir(doc_dir)
	{}
	~posix_file_system() {};
	
	std::string get_dir(dir_type type) const override;
	bool is_file_exists(const std::string& file) const override;
	bool remove_file(const std::string& file) override;
	data_stream_ptr open_read_file(const std::string& file) override;
	write_data_stream_ptr open_write_file(const std::string& file) override;
private:
	std::string m_cache_dir;
	std::string m_data_dir;
    std::string m_docs_dir;
};

}

#endif // _POSIX_FILE_SYSTEM_H_
