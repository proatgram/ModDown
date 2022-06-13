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

#include "Request.h"

class MinecraftDownload {
	public:
		MinecraftDownload(nlohmann::json& json, std::string outputLocation) :
			m_json(json),
			m_outputLocation(outputLocation)
		{
			std::filesystem::create_directory(m_outputLocation);
			std::filesystem::current_path(m_outputLocation);
		}
		void operator()();
	private:
	nlohmann::json& m_json;
	std::string m_outputLocation;
	Request m_request;
};



#endif /* INCLUDE_MINECRAFTDOWNLOAD_H_ */
