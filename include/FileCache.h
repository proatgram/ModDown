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

class Cache {
	public:

		Cache(std::string filePath) :
			m_path(filePath)
		{
			std::fstream m_file(m_path.filename().string(), std::fstream::out | std::fstream::app);
			m_file.close();
			m_file.open(m_path.filename().string(), std::fstream::in | std::fstream::out | std::fstream::app);
		}

		int writePair(std::string key, std::string value);

		std::string getPair(std::string key);

	private:

		std::filesystem::path m_path;
		std::fstream m_file;

};



#endif /* INCLUDE_FILECACHE_H_ */
