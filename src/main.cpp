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
#include <zip.h>
#include <sysexits.h>

#include "nlohmann/json.hpp"
#include "indicators/cursor_control.hpp"
#include "indicators/progress_bar.hpp"

#include "Download.h"

int changeRange(int OldValue, int OldMax, int OldMin, int NewMax, int NewMin) {
	return ((((OldValue - OldMin) * (NewMax - NewMin)) / (OldMax - OldMin)) + NewMin);
}



int main(int argc, char** argv) {
	if (argc < 2) {
		std::printf("Invalid number of arguments. Try ModDown --help for usage.\n");
		exit(EX_USAGE);
	}
	nlohmann::json modList;
	unsigned long int args = 0;
	for (int times = 1; times < argc; times++) {
		if (std::string(argv[times]).find("--help") != std::string::npos || std::string(argv[times]).find("-h") != std::string::npos) {
			std::printf("ModDown [OPTIONS...] (MODPACK.ZIP / MANIFEST.JSON)\n\n");
			std::printf("	Options:\n");
			std::printf("		-h, --help				Displays this page.\n");
			std::printf("		-z, --zip				Indicates that you are using a zip file.\n");
			std::printf("		-j, --json				Indicates that you are using a manifest file.\n");
			std::printf("		-d, --download			Downloads all the mods in the ModPack.\n");

			std::printf("\n");
		}

		if (std::string(argv[times]).find("--zip") != std::string::npos || std::string(argv[times]).find("-z") != std::string::npos) {
			args |= 0b1;
		}
		else if (std::string(argv[times]).find("--json") != std::string::npos || std::string(argv[times]).find("-j") != std::string::npos) {
			args |= 0b10;
		}
		if (std::string(argv[times]).find("--pipe") != std::string::npos || std::string(argv[times]).find("-p") != std::string::npos) {
			args |= 0b100;
		}
		if (std::string(argv[times]).find("--download") != std::string::npos || std::string(argv[times]).find("-d") != std::string::npos) {
			args |= 0b1000;
		}
	}

	if ((args & 0b11) == 0) {
		std::fprintf(stderr, "Error: Can't determine file type.\n");
		exit(EX_USAGE);
	}

	if ((args & 0b1) == 0b1) {
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
	}
	if ((args & 0b100) == 0b100) {
		std::printf("%s\n", modList.dump(4).c_str());
		exit(0);
	}
}

