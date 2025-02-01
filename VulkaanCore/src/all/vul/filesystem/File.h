#pragma once

#include <string>
#include <vector>

namespace vul::Filesystem {
	class File {
	public:
		File(std::string path);
		~File();

		std::vector<char> Read();
		void Clear();
		void Write(std::vector<char> contents);
		void Write(char* contents);
		void Save();
		void SaveAs(std::string path);
		bool Exists();

		inline std::vector<char> GetContents() const { return m_Contents; }

	private:
		std::string m_Path;
		std::vector<char> m_Contents;
	};
}