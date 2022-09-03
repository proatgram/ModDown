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
 * Unpack.cpp
 *
 *  Created on: Jun 15, 2022
 *      Author: thetimbrick
 */

#include "Unpack.h"


std::string Unpack::operator()(std::filesystem::path path) {
	zip_t* file;
	zip_file_t* zipFile;
	zip_stat_t stat;
	zip_error_t* error;
	int* errorno;
	int sum = 0;
	int len = 0;

	std::fstream stream;

	if ((file = zip_open(std::filesystem::absolute(path).string().c_str(), 0, errorno)) == NULL) {
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
	long int numEntries = zip_get_num_entries(file, ZIP_FL_UNCHANGED);
	unsigned long int uNumEntries = numEntries;
	for (unsigned int times = 0; times < numEntries; times++) {
		unsigned long int prog = times;
		m_progress = Utils<size_t>::changeRange(prog, uNumEntries, 0, 100, 0);
		m_bar.set_progress(m_progress());

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
				len = zip_fread(zipFile, buffer, stat.size);
				if (len < 0) {
					std::printf("Error: Can't read from file.\n");
					exit(EX_IOERR);
				}
				if (len != stat.size) {
					std::printf("Error reading from file.\n");
					exit(EX_IOERR);
				}
				stream.write(buffer, stat.size);
				stream.close();
				zip_fclose(zipFile);
			}
		}
	}
	m_bar.set_progress(100);
	return dir + '/';
}
