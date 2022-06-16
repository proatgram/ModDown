/*
 * MinecraftDownload.cpp
 *
 *  Created on: Jun 12, 2022
 *      Author: thetimbrick
 */

#include "MinecraftDownload.h"

void MinecraftDownload::operator()() {
	// Downloads the Modpacks and resource packs and puts them in their respective folder.
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

			std::string requestIdURL("/v1/mods/{modId}");
			size_t idIndex = requestIdURL.find("{modId}");
			requestIdURL.replace(idIndex, 7, modId);

			m_request.setBase(requestBASE);
			std::stringstream mdURL(m_request.sendGET(requestURL));
			std::stringstream classIdURL(m_request.sendGET(requestIdURL));
			nlohmann::json idUrl;
			nlohmann::json url;
			mdURL >> url;
			classIdURL >> idUrl;
			int classId = idUrl["data"]["classId"];

			std::string modFileName(url["data"]["fileName"].dump());
			modFileName.erase(modFileName.cbegin());
			modFileName.erase(modFileName.cend() - 1);
			bool exists = false;
			for (const auto & entry : std::filesystem::recursive_directory_iterator(std::filesystem::current_path())) {
				if (entry.path().filename().compare(modFileName) == 0) {
					exists = true;
					std::printf("File %s exists.\n", modFileName.c_str());
				}
			}
			if (!exists) {
				std::printf("Mod Display Name: %s\n", url["data"]["displayName"].dump().c_str());
				std::printf("Mod Filename: %s\n\n", modFileName.c_str());

				// If the downloadUrl element does not equal 'null'
				if (url["data"]["downloadUrl"].dump().compare("null") != 0) {
					std::printf("Mod data contains a download url. Downloading...\n");
					std::string downloadURL(url["data"]["downloadUrl"].dump());
					downloadURL.erase(downloadURL.cbegin());
					downloadURL.erase(downloadURL.cend() - 1);
					if (classId == 6) {
						std::filesystem::current_path("mods");
						m_request.download(downloadURL, modFileName);
						std::filesystem::path curPath(std::filesystem::current_path());
						std::filesystem::current_path(curPath.parent_path());
					}
					else if (classId == 12) {
						std::filesystem::current_path("resourcepacks");
						m_request.download(downloadURL, modFileName);
						std::filesystem::path curPath(std::filesystem::current_path());
						std::filesystem::current_path(curPath.parent_path());
					}
					else {
						std::printf("Unknown pack type. Putting download in top directory.\n");
						m_request.download(downloadURL, modFileName);
					}
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
					if (classId == 6) {
						std::filesystem::current_path("mods");
						m_request.download(baseURL, modFileName);
						std::filesystem::path curPath(std::filesystem::current_path());
						std::filesystem::current_path(curPath.parent_path());
					}
					else if (classId == 12) {
						std::filesystem::current_path("resourcepacks");
						m_request.download(baseURL, modFileName);
						std::filesystem::path curPath(std::filesystem::current_path());
						std::filesystem::current_path(curPath.parent_path());
					}
					else {
						std::printf("Unknown pack type. Putting download in top directory.\n");
						m_request.download(baseURL, modFileName);
					}
					std::printf("Successfully downloaded mod as %s!\n\n", modFileName.c_str());
				}
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
			bool exists = false;
			for (const auto & entry : std::filesystem::recursive_directory_iterator(std::filesystem::current_path())) {
				if (entry.path().filename().compare(fileName) == 0) {
					exists = true;
					std::printf("File %s exists.\n", fileName.c_str());
				}
			}
			if (!exists) {
				m_request.setHeader(std::pair<std::string, std::string>("Accept", "application/json"));
				m_request.setHeader(std::pair<std::string, std::string>("x-api-key", m_key));
				std::string requestBASE("https://api.curseforge.com");
				std::string modId(m_json["files"][times]["projectID"].dump());
				std::string requestIdURL("/v1/mods/{modId}");
				size_t idIndex = requestIdURL.find("{modId}");
				requestIdURL.replace(idIndex, 7, modId);
				m_request.setBase(requestBASE);

				std::stringstream url("");
				while (url.str().compare("") == 0) {
					url = m_request.sendGET(requestIdURL);
				}
				nlohmann::json jsonUrl;
				// This is to try to prevent the program from exiting in the case that
				// There was a problem sending the request
				try {
					url >> jsonUrl;
				}
				catch(nlohmann::detail::parse_error& error) {
					std::fprintf(stderr, "Network error. Re-run the program to continue.\n");
					std::printf("%s\n", url.str().c_str());
					exit(EX_UNAVAILABLE);
				}
				int classId = jsonUrl["data"]["classId"];
				std::printf("%s: %s\n", downloadURL.c_str(), fileName.c_str());
				if (classId == 6) {
					std::filesystem::current_path("mods");
					m_request.download(downloadURL, fileName);
					std::filesystem::path curPath(std::filesystem::current_path());
					std::filesystem::current_path(curPath.parent_path());
				}
				else if (classId == 12) {
					std::filesystem::current_path("resourcepacks");
					m_request.download(downloadURL, fileName);
					std::filesystem::path curPath(std::filesystem::current_path());
					std::filesystem::current_path(curPath.parent_path());
				}
				else {
					std::printf("Unknown pack type. Putting download in top directory.\n");
					m_request.download(downloadURL, fileName);
				}
			}
		}
	}
	std::filesystem::path curPath(std::filesystem::current_path());
	std::filesystem::current_path(curPath.parent_path());

	std::string path(m_unpack(m_zip));
	std::string overides(m_json["overrides"].dump());
	overides.erase(overides.cbegin());
	overides.erase(overides.cend() - 1);
	std::filesystem::copy(path + overides, m_outputLocation, std::filesystem::copy_options::recursive | std::filesystem::copy_options::skip_existing);
	std::filesystem::remove_all(path);
}