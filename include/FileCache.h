/*
 * FileCache.h
 *
 *  Created on: Jun 19, 2022
 *      Author: thetimbrick
 */

#ifndef INCLUDE_FILECACHE_H_
#define INCLUDE_FILECACHE_H_

#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <cstring>

class Cache {
	public:
		Cache(std::string path)
		{
			this->openFile(path);
		}

		int writePair(std::string key, std::string value);

		std::string getPair(std::string key);

		int openFile(std::string filePath);

		int closeFile();


		enum {
			FILENOTOPEN,
			SUCCESS,
			ERROR,
			ENDOFFILE
		};

		static const std::string STR_FILENOTOPEN;
		static const std::string STR_ENDOFFILE;
	private:

		std::filesystem::path m_path;
		std::fstream m_file;

};



#endif /* INCLUDE_FILECACHE_H_ */
