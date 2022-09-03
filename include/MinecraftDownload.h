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
 * MinecraftDownload.h
 *
 *  Created on: Jun 12, 2022
 *      Author: thetimbrick
 */

#ifndef INCLUDE_MINECRAFTDOWNLOAD_H_
#define INCLUDE_MINECRAFTDOWNLOAD_H_

#include <nlohmann/json.hpp>
#include <utility>
#include <sstream>
#include <filesystem>
#include <zip.h>

#include "Request.h"
#include "Unpack.h"
#include "FileCache.h"
#include "SafeData.hpp"
#include "Utils.hpp"

class MinecraftDownload {
	public:
		MinecraftDownload(nlohmann::json& json, std::string outputLocation, std::string key, std::filesystem::path zip) :
			m_json(json),
			m_outputLocation(outputLocation),
			m_key(key),
			m_zip(zip),
			m_cache()
		{
			std::filesystem::create_directory(m_outputLocation);
			std::filesystem::current_path(m_outputLocation);
			m_cache.openFile(m_zip.filename().string() + ".cache");
			std::filesystem::create_directory("mods");
			std::filesystem::create_directory("resourcepacks");
		}
		void operator()();
	private:
	nlohmann::json& m_json;
	std::string m_outputLocation;
	std::string m_key;
	std::filesystem::path m_zip;
	Request m_request;
	Unpack m_unpack;
	Cache m_cache;
};



#endif /* INCLUDE_MINECRAFTDOWNLOAD_H_ */
