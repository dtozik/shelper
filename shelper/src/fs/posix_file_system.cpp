#include "posix_file_system.h"
#include <cstdio>
#include <sys/stat.h>

using namespace shelper::fs;

class posix_read_data_stream : public read_data_stream {
	FILE* m_file = nullptr;
public:
	posix_read_data_stream(FILE* f) : m_file(f)
	{}

	~posix_read_data_stream() {
		if (m_file) {
			::fclose(m_file);
			m_file = nullptr;
		}
	}

	unsigned int read(unsigned char* data, unsigned int size) override {
		return ::fread(data, 1, size, m_file);
	}
	unsigned int tell() const override {
		return ::ftell(m_file);
	}
	bool seek(int offset, seek_offset_type type) override {
		int dir = 0;
		switch (type){
			case seek_offset_type::from_current:
			dir = SEEK_CUR;
			break;
		case seek_offset_type::from_end:
			dir = SEEK_END;
			break;
		case seek_offset_type::from_begin:
			dir = SEEK_SET;
			break;
		default: return false;
		}
		return ::fseek(m_file,offset,dir) == 0;
	}
	bool eof() const override {
		return ::feof(m_file) != 0;
	}
};

class posix_write_data_stream : public write_data_stream {
	FILE* m_file = nullptr;
public:
	posix_write_data_stream(FILE* f) : m_file(f)
	{}
	~posix_write_data_stream() {
		if (m_file) {
			fclose(m_file);
			m_file = nullptr;
		}
	}
	void flush() override {
	}
	unsigned int write(const unsigned char* data, unsigned int size) override {
		if (!m_file)
			return 0;
		return ::fwrite(data, size, 1, m_file);
	}
};
using write_data_stream_ptr = std::shared_ptr<write_data_stream>;


std::string posix_file_system::get_dir(dir_type type) const {
   if (type == dir_type::data) {
	   return m_data_dir.c_str();
   }
   if (type == dir_type::user_profile) {
	   return m_docs_dir.c_str();
   }
   if (type == dir_type::cache) {
	   return m_cache_dir.c_str();
   }
   return "/";
}

bool posix_file_system::remove_file(const std::string& file) {
	return !::remove(file.c_str());
}

 bool posix_file_system::is_file_exists(const std::string& file) const {
	 typedef struct stat stat_t;
	 stat_t s;
	 if (::stat(file.c_str(), (stat_t*)(&s)) == 0) {
		 return (s.st_mode & S_IFREG)!=0;
	 }
	 return false;
 }

data_stream_ptr posix_file_system::open_read_file(const std::string& file) {
	if (file.empty())
		return nullptr;
	
	if (!is_file_exists(file))
		return 0;
	FILE* f = ::fopen(file.c_str(), "rb");
	if (f)
		return std::make_shared<posix_read_data_stream>(f);
	return nullptr;
}

static void create_dir(const char* filename) {
	std::string dir;
	const char* pos = ::strchr(filename,'/');
	while (pos) {
		dir.append(filename,pos-filename);
		::mkdir(dir.c_str(),0777);
		filename = pos;
		pos = ::strchr(pos+1,'/');
	}
}

write_data_stream_ptr posix_file_system::open_write_file(const std::string& file) {
	if (file.empty())
		return nullptr;
	
	create_dir(file.c_str());
	FILE* f = ::fopen(file.c_str(), "wb");
	if (f)
		return std::make_shared<posix_write_data_stream>(f);
	return nullptr;
};
