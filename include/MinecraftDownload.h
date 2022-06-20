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
#include "SafeData.hpp"
#include "Utils.hpp"

class MinecraftDownload {
	public:
		MinecraftDownload(nlohmann::json& json, std::string outputLocation, std::string key, std::string zip) :
			m_json(json),
			m_outputLocation(outputLocation),
			m_key(key),
			m_zip(zip)
		{
			std::filesystem::create_directory(m_outputLocation);
			std::filesystem::current_path(m_outputLocation);
			std::filesystem::create_directory("mods");
			std::filesystem::create_directory("resourcepacks");
		}
		void operator()();
	private:
	nlohmann::json& m_json;
	std::string m_outputLocation;
	std::string m_key;
	std::string m_zip;
	Request m_request;
	Unpack m_unpack;
};



#endif /* INCLUDE_MINECRAFTDOWNLOAD_H_ */
