#include "File.h"

#include <fstream>
#include <filesystem>
#include <iterator>

#include <vul/core/Log.h>

namespace vul::Filesystem {
	File::File(std::string path) {
		m_Path = path;
	}

	File::~File() {
		VUL_LOG_CORE_TRACE("File::~File()");
	}

	std::vector<char> File::Read() {
		std::ifstream file(m_Path, std::ios::ate | std::ios::binary);

		if (file.is_open()) {
			VUL_LOG_CORE_TRACE("Reading contents of file '" + m_Path + "'.");
			size_t fileSize = (size_t)file.tellg();
			std::vector<char> buffer(fileSize);
			file.seekg(0);
			file.read(buffer.data(), fileSize);

			file.close();

			m_Contents = buffer;
			return buffer;
		}

		VUL_LOG_CORE_WARN("Failed to open file '" + m_Path + "'!");
		
		return m_Contents;
	}

	void File::Clear() {
		m_Contents.clear();
	}

	void File::Write(std::vector<char> contents) {
		m_Contents.insert(m_Contents.end(), contents.begin(), contents.end());
	}

	void File::Write(char* contents) {
		char* p = &contents[0];
		while (*p != '\0') {
			m_Contents.push_back(*(p++));
		}
	}

	void File::Save() {
		std::ofstream file(m_Path, std::ios::out | std::ios::binary);
		std::copy(m_Contents.begin(), m_Contents.end(), std::ostreambuf_iterator<char>(file));
	}

	void File::SaveAs(std::string path) {
		m_Path = path;
		Save();
	}

	bool File::Exists() {
		struct stat buffer;
		return (stat(m_Path.c_str(), &buffer) == 0);
	}
}