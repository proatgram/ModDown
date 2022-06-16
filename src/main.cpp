/*
 * main.cpp
 *
 *  Created on: Jun 9, 2022
 *      Author: thetimbrick
 */
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>
#include <zip.h>
#include <sysexits.h>
#include <filesystem>
#include <unistd.h>
#include <nlohmann/json.hpp>

#include "Request.h"
#include "MinecraftDownload.h"
#include "Unpack.h"

int main(int argc, char** argv) {
	if (argc < 2) {
		std::printf("Invalid number of arguments. Try ModDown --help for usage.\n");
		exit(EX_USAGE);
	}

	nlohmann::json modList;
	std::string outputDir("Download");
	std::string APIKEY("NULL");
	unsigned long int args = 0;
	for (int times = 1; times < argc; times++) {
		if (std::string(argv[times]).find("--help") != std::string::npos || std::string(argv[times]).find("-h") != std::string::npos) {
			std::printf("ModDown [OPTIONS...] (MODPACK.ZIP / MANIFEST.JSON)\n\n");

			std::printf("	Options:\n");
			std::printf("		-h, --help			Displays this page.\n");
			std::printf("		-d, --download			Downloads all the mods in the ModPack.\n");
			std::printf("		-o, --output [OUT]		Sets the output folder to store all the downloads.\n");
			std::printf("		-i, --packinfo			Displays the information about the ModPack.\n");
			std::printf("		-k, --key [KEY]			Sets the CF Core API key. You need this in order to download stuff.\n");
			std::printf("		-s, --storekey			Stores the API key so that you don't have to input it every time.\n");

			std::printf("\n");
			exit(0);
		}
		if (std::string(argv[times]).find("--pipe") != std::string::npos || std::string(argv[times]).find("-p") != std::string::npos) {
			args |= 0b100;
		}
		if (std::string(argv[times]).find("--download") != std::string::npos || std::string(argv[times]).find("-d") != std::string::npos) {
			args |= 0b1000;
		}
		if (std::string(argv[times]).find("--output") != std::string::npos || std::string(argv[times]).find("-o") != std::string::npos) {
			outputDir = argv[times + 1];
		}
		if (std::string(argv[times]).find("--packinfo") != std::string::npos || std::string(argv[times]).find("-i") != std::string::npos) {
			args |= 0b10000;
		}
		if (std::string(argv[times]).find("--key") != std::string::npos || std::string(argv[times]).find("-k") != std::string::npos) {
			args |= 0b100000;
			APIKEY = argv[times + 1];
		}
		if (std::string(argv[times]).find("--storekey") != std::string::npos || std::string(argv[times]).find("-s") != std::string::npos) {
			args |= 0b1000000;
		}
	}

	if ((args & 0b1000000) == 0b1000000 && (args & 0b100000) != 0b100000) {
		std::printf("Can't store an API Key when you don't input one!\n");
		std::printf("Use ModDown --help for more information.\n");
		exit(EX_USAGE);
	}

	if ((args & 0b1000000) == 0b1000000) {
		if (geteuid() == 0) {
			std::fstream file("/etc/ModDown.d/APIKEY.conf", std::fstream::in | std::fstream::out);
			if (file.fail()) {
				file.open("/etc/ModDown.d/APIKEY.conf", std::fstream::in | std::fstream::out | std::fstream::app);
				if (file.fail()) {
					std::filesystem::create_directory("/etc/ModDown.d");
					file.open("/etc/ModDown.d/APIKEY.conf", std::fstream::in | std::fstream::out | std::fstream::app);
				}
			}
			std::stringstream filecontents;
			filecontents << file.rdbuf();
			if (filecontents.str().find("API-KEY: ") != std::string::npos) {
				std::string uin;
				std::cout << "Config file already has an API Key entry." << std::endl;
				std::cout << "Would you like to reset it to the new API Key? (y / n)" << std::endl;
				std::cin >> uin;
				if (uin.find("y") == 0) {
					file.close();
					std::filesystem::remove("/etc/ModDown.d/APIKEY.conf");
					file.open("/etc/ModDown.d/APIKEY.conf", std::fstream::in | std::fstream::out | std::fstream::app);
					file << "";
					file << "API-KEY: " << APIKEY;
					file << "";
					file.close();
				}
				else {
					std::printf("Okay...\n");
				}
			}
			else {
				file << "API-KEY: " << APIKEY << std::endl;
				file.close();
			}
		}
		// Stores the inputed API KEY
		std::string user(getlogin());
		std::string APIFILE("/home/" + user + "/.config/ModDown/APIKEY.conf");
		std::string CONFIGFOLDER("/home/" + user + "/.config/ModDown/");
		std::fstream file(APIFILE, std::fstream::in | std::fstream::out);
		if (file.fail()) {
			file.open(APIFILE, std::fstream::in | std::fstream::out | std::fstream::app);
			if (file.fail()) {
				std::filesystem::create_directory(CONFIGFOLDER);
				file.open(APIFILE, std::fstream::in | std::fstream::out | std::fstream::app);
			}
		}
		std::stringstream filecontents;
		filecontents << file.rdbuf();
		if (filecontents.str().find("API-KEY: ") != std::string::npos) {
			std::string uin;
			std::cout << "API Key config file already has an API Key entry." << std::endl;
			std::cout << "Would you like to reset it to the new API Key? (y / n)" << std::endl;
			std::cin >> uin;
			if (uin.find("y") == 0) {
				file.close();
				std::filesystem::remove(APIFILE);
				file.open(APIFILE, std::fstream::in | std::fstream::out | std::fstream::app);
				file << "";
				file << "API-KEY: " << APIKEY;
				file << "";
				file.close();
			}
			else {
				std::printf("Okay...\n");
			}
		}
		else {
			file << "API-KEY: " << APIKEY << std::endl;
			file.close();
		}
	}

	zip_stat_t stat;
	int* errorp;
	zip_t* zfile = zip_open(argv[argc - 1], ZIP_RDONLY, errorp);
	zip_int64_t location = zip_name_locate(zfile, "manifest.json", ZIP_FL_ENC_GUESS);
	zip_file_t* man = zip_fopen_index(zfile, location, ZIP_FL_UNCHANGED);
	zip_stat_index(zfile, location, ZIP_FL_ENC_GUESS, &stat);
	char buffer[stat.size];
	int sum = 0;
	int len = zip_fread(man, buffer, stat.size);
	if (len < 0) {
		std::fprintf(stderr, "Error.\n");
	}
	std::stringstream manFile(buffer);
	manFile >> modList;


	if ((args & 0b100) == 0b100) {
		std::printf("%s\n", modList.dump(4).c_str());
		exit(0);
	}

	if (modList["manifestType"].dump() != "\"minecraftModpack\"") {
		std::fprintf(stderr, "Error: ModPack type not supported.\n");
		exit(1);
	}

	if ((args & 0b10000) == 0b10000) {
		// Gives the info about modspacks
		if (modList["manifestType"].dump() == "\"minecraftModpack\"") {
			std::printf("Name: %s\n", modList["name"].dump().c_str());
			std::printf("ModPack Version: %s\n", modList["version"].dump().c_str());
			std::printf("ModPack Author: %s\n", modList["author"].dump().c_str());
			std::printf("Minecraft Version: %s\n", modList["minecraft"]["version"].dump().c_str());
		}
	}
	if ((args & 0b1000) == 0b1000) {
		// Downloads stuff
		if (APIKEY.compare("NULL") == 0) {
			std::fstream file;
			if (geteuid() == 0) {
				file.open("/etc/ModDown.d/APIKEY.conf", std::fstream::in);
			}
			else {
				std::string APIFILE("/home/");
				APIFILE.append(getlogin());
				APIFILE.append("/.config/ModDown/APIKEY.conf");
				file.open(APIFILE, std::fstream::in);
			}
			if (!file) {
				std::printf("API Key config file does not exists. Please provide an API Key with --key.\n");
				std::printf("You can store this API Key in a config file using --storekey.\n");
				exit(EX_CONFIG);
			}
			std::stringstream filecontents;
			filecontents << file.rdbuf();
			APIKEY = filecontents.str().erase(0, 9);
			APIKEY.erase(std::remove(APIKEY.begin(), APIKEY.end(), '\n'), APIKEY.end());
		}
		if (modList["manifestType"].dump() == "\"minecraftModpack\"") {
			//Minecraft
			MinecraftDownload md(modList, outputDir, APIKEY, argv[argc - 1]);
			md();
		}
	}
}

