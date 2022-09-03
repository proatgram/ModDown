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
 * FileCache.cpp
 *
 *  Created on: Jun 20, 2022
 *      Author: thetimbrick
 */

#include "FileCache.h"

const std::string Cache::STR_ENDOFFILE = "STR_ENDOFFILE";
const std::string Cache::STR_FILENOTOPEN = "STR_FILENOTOPEN";

int Cache::openFile(std::string filePath) {
	m_path = std::filesystem::path(filePath);
	m_file.open(m_path.filename().string(), std::fstream::in | std::fstream::out | std::fstream::app);
	if (!m_file) {
		return Cache::SUCCESS;
	}
	else {
		m_file.open(m_path.filename().string(), std::fstream::out | std::fstream::app);
		m_file.close();
		m_file.open(m_path.filename().string(), std::fstream::in | std::fstream::out | std::fstream::app);
		return Cache::SUCCESS;
	}
	return Cache::ERROR;
}

int Cache::closeFile() {
	m_file.close();
	return 0;
}

int Cache::writePair(std::string key, std::string value) {
	if (!m_file) {
		return Cache::FILENOTOPEN;
	}
	std::stringstream str;
	str << '{' << key << '}' << "==" << '{' << value << '}' << std::endl;
	m_file << str.str() << std::endl;
	return Cache::SUCCESS;
}

int Cache::removePair(std::string key) {
	if (!m_file) {
		return Cache::FILENOTOPEN;
	}
	int startPos = m_file.tellg();
	std::stringstream buff;
	buff << m_file.rdbuf();
	std::string str(buff.str());
	std::string findValue('{' + key + '}');
	int pos = m_file.tellg();
	size_t at(str.find(findValue));
	if (at != std::string::npos) {
		bool endFound = false;
		size_t pos;
		while (!endFound) {
			if (str.at(pos + at) == '\n') {
				endFound = true;
			}
			pos++;
		}
		str.erase(str.find(findValue), pos);
		std::filesystem::resize_file(m_path, 0);
		m_file.write(str.c_str(), str.size());
		return Cache::SUCCESS;
	}
	return Cache::ENDOFFILE;
}

std::string Cache::getPair(std::string key) {
	if (!m_file) {
		return Cache::STR_FILENOTOPEN;
	}
	int initpos = m_file.tellg();
	m_file.seekg(0);
	std::string buff;
	std::string value;
	std::string findValue('{' + key + '}');
	bool found(false);
	while (!found) {
		std::getline(m_file, buff);
		if (buff.find(findValue) != std::string::npos) {
			std::stringstream wr;
			wr << '{' << key << '}' << "==" << '{';
			int len = strlen(wr.str().c_str());
			value = buff.substr(len, buff.size() - len);
			value.erase(value.cend() - 1);
			m_file.seekg(initpos);
			found = true;
			return value;
		}
		else {
			buff = '\0';
		}
		if (m_file.eof()) {
			m_file.clear();
			m_file.seekg(0);
			return Cache::STR_ENDOFFILE;
		}
	}
	return value;
}
