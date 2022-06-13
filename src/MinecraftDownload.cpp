/*
 * MinecraftDownload.cpp
 *
 *  Created on: Jun 12, 2022
 *      Author: thetimbrick
 */

#include "MinecraftDownload.h"

void MinecraftDownload::operator()() {
	int size = m_json["files"].size();
	if (m_json["files"][0]["downloadUrl"].dump().compare("null") == 0) {
		m_request.setHeader(std::pair<std::string, std::string>("Accept", "application/json"));
		m_request.setHeader(std::pair<std::string, std::string>("x-api-key", m_key));
		std::string requestBASE("https://api.curseforge.com");
		for (int times = 0; times < size; times++) {
			std::string requestURL("/v1/mods/{modId}/files/{fileId}");
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
			std::printf("Mod Display Name: %s\n", url["data"]["displayName"].dump().c_str());
			std::string modFileName(url["data"]["fileName"].dump());
			modFileName.erase(modFileName.cbegin());
			modFileName.erase(modFileName.cend() - 1);
			std::printf("Mod Filename: %s\n\n", modFileName.c_str());
			if (url["data"]["downloadUrl"].dump().compare("null") != 0) {
				std::printf("Mod data contains a download url. Downloading...\n");
				std::string downloadURL(url["data"]["downloadUrl"].dump());
				downloadURL.erase(downloadURL.cbegin());
				downloadURL.erase(downloadURL.cend() - 1);
				m_request.download(downloadURL, modFileName);
				std::printf("Successfully downloaded mod as %s!\n\n", modFileName.c_str());
			}
			else {
				std::printf("Mod data does not contain a download url.\n");
				std::printf("Trying to guess url based on the ID...\n");
				std::string id(url["data"]["id"].dump());
				std::string baseURL("https://edge.forgecdn.net/files/");
				id.insert(4, "/");
				id.insert(id.size(), "/");
				baseURL.append(id);
				baseURL.append(modFileName);
				std::printf("Guessed download url as: %s\n", baseURL.c_str());
				std::printf("Trying to download...\n");
				m_request.download(baseURL, modFileName);
				std::printf("Successfully downloaded mod as %s!\n\n", modFileName.c_str());
			}
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
