/*
 * MinecraftDownload.cpp
 *
 *  Created on: Jun 12, 2022
 *      Author: thetimbrick
 */

#include "MinecraftDownload.h"

int MinecraftDownload::operator()() {
	int size = m_json["files"].size();
	if (m_json["files"][0]["downloadUrl"].dump().compare("null") == 0) {
		m_request.setHeader(std::pair<std::string, std::string>("Accept", "application/json"));
		m_request.setHeader(std::pair<std::string, std::string>("x-api-key", "$2a$10$oEmLmQHTPAE5KgK8DlvqHebQPweHgcIfL5fDsvCfSDOmZhOQuGOre"));
		std::string requestBASE("https://api.curseforge.com");
		for (int times = 0; times < size; times++) {
			std::string requestURL("/v1/mods/{modId}/files/{fileId}/download-url");
			std::string modId(m_json["files"][times]["projectID"].dump());
			std::string fileId(m_json["files"][times]["fileID"].dump());
			size_t index = requestURL.find("{modId}");
			requestURL.replace(index, 7, modId);
			index = requestURL.find("{fileId}");
			requestURL.replace(index, 8, fileId);
			m_request.setBase(requestBASE);
			std::stringstream mdURL(m_request.sendGET(requestURL));
			nlohmann::json url;
			mdURL >> url;
			std::string downloadURL = url["data"].dump();
			downloadURL.erase(downloadURL.cbegin());
			downloadURL.erase(downloadURL.cend() - 1);
			std::string fileName(downloadURL);
			size_t del = fileName.find_last_of("/");
			fileName.erase(0, del + 1);
			std::printf("%s: %s\n", downloadURL.c_str(), fileName.c_str());
			m_request.download(downloadURL, fileName);
		}
	}
	else {
		for (int times = 0; times < size; times++) {
			std::string downloadURL(m_json["files"][times]["downloadUrl"].dump());
			downloadURL.erase(downloadURL.cbegin());
			downloadURL.erase(downloadURL.cend() - 1);
			std::string fileName(downloadURL);
			size_t del = fileName.find_last_of("/");
			fileName.erase(0, del + 1);
			std::printf("%s: %s\n", downloadURL.c_str(), fileName.c_str());
			m_request.download(downloadURL, fileName);
		}
	}
}
