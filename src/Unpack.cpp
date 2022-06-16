/*
 * Unpack.cpp
 *
 *  Created on: Jun 15, 2022
 *      Author: thetimbrick
 */

#include "Unpack.h"


std::string Unpack::operator()(std::string filename) {
	std::filesystem::path path(filename);
	zip_t* file;
	zip_file_t* zipFile;
	zip_stat_t stat;
	zip_error_t* error;
	int* errorno;
	int sum = 0;
	int len = 0;

	std::fstream stream;

	if ((file = zip_open(filename.c_str(), 0, errorno)) == NULL) {
		error->zip_err = *errorno;
		std::fprintf(stderr, "Error extracting zip file. Error code %d. %s", error->zip_err, zip_error_strerror(error));
	}
	if (std::filesystem::exists("/tmp/ModDown")) {
		if (std::filesystem::exists("/tmp/ModDown/extracts")) {
			if (std::filesystem::exists("/tmp/ModDown/extracts/" + path.filename().string())) {

			}
			else {
				std::filesystem::create_directory("/tmp/ModDown/extracts/" + path.filename().string());
			}
		}
		else {
			std::filesystem::create_directory("/tmp/ModDown/extracts/");
			std::filesystem::create_directory("/tmp/ModDown/extracts/" + path.filename().string());
		}
	}
	else {
		std::filesystem::create_directory("/tmp/ModDown/");
		std::filesystem::create_directory("/tmp/ModDown/extracts/");
		std::filesystem::create_directory("/tmp/ModDown/extracts/" + path.filename().string());
	}
	std::string dir("/tmp/ModDown/extracts/" + path.filename().string());
	for (int times = 0; times < zip_get_num_entries(file, ZIP_FL_UNCHANGED); times++) {
		if (zip_stat_index(file, times, ZIP_FL_ENC_GUESS, &stat) == 0) {
			int len = strlen(stat.name);
			if (stat.name[len - 1] == '/') {
				std::filesystem::create_directory(dir + '/' + stat.name);
			}
			else {
				zipFile = zip_fopen_index(file, times, 0);
				if (!zipFile) {
					fprintf(stderr, "Error: Bad file.\n");
					exit(EX_DATAERR);
				}
				stream.open(dir + '/' + stat.name, std::fstream::in | std::fstream::out | std::fstream::trunc | std::fstream::binary);
				if (!stream.is_open()) {
					stream.open(dir + '/' + stat.name, std::fstream::out | std::fstream::in | std::fstream::app | std::fstream::binary);
					stream.close();
					stream.open(dir + '/' + stat.name, std::fstream::in | std::fstream::out | std::fstream::trunc | std::fstream::binary);
				}
				char buffer[stat.size];
				std::printf("%ld", stat.size);
				len = zip_fread(zipFile, buffer, stat.size);
				if (len < 0) {
					std::printf("Error: Can't read from file.\n");
					exit(EX_IOERR);
				}
				if (len != stat.size) {
					std::printf("Error reading from file.\n");
					exit(EX_IOERR);
				}
				std::printf("%s\n", buffer);
				stream.write(buffer, stat.size);
				stream.close();
				zip_fclose(zipFile);
			}
		}
	}
	return dir + '/';
}
