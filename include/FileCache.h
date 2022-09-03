/*
   This is a project that is dedicated to downloading Minecraft Mods from CurseForge using their API
    Copyright (C) <2022>  Timothy Hutchins

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

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

		Cache()
		{

		}

		int writePair(std::string key, std::string value);

		int removePair(std::string key);

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
