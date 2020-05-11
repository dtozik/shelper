#ifndef _FILE_SYSTEM_H_
#define _FILE_SYSTEM_H_

#include <defs.h>

namespace shelper::fs {

enum class seek_offset_type {
	from_begin,
	from_current,
	from_end
};

enum class dir_type {
	data,
	user_profile,
	cache
};

struct read_data_stream {
public:
	virtual unsigned int read(unsigned char* data, unsigned int size) = 0;
	virtual unsigned int tell() const = 0;
	virtual bool seek(int offset, seek_offset_type type) = 0;
	virtual bool eof() const = 0;
};
using data_stream_ptr = std::shared_ptr<read_data_stream>;

class write_data_stream {
public:
	virtual void flush() = 0;
	virtual unsigned int write(const unsigned char* data, unsigned int size) = 0;
};
using write_data_stream_ptr = std::shared_ptr<write_data_stream>;


class file_system {
public:
	virtual std::string get_dir(dir_type type) const = 0;
	virtual bool is_file_exists(const std::string& file) const = 0;
	virtual bool remove_file(const std::string& file) = 0;
	virtual data_stream_ptr open_read_file(const std::string& file) = 0;
	virtual write_data_stream_ptr open_write_file(const std::string& file) = 0;
};
using file_system_ptr = std::shared_ptr<file_system>;

}

#endif // _FILE_SYSTEM_H_

