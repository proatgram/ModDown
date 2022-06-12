/*
 * Download.cpp
 *
 *  Created on: Jun 9, 2022
 *      Author: thetimbrick
 */

#include "Download.h"

int Download::operator()(std::string url, std::string output) {
	CURL *handle = curl_easy_init();
	std::string dUrl = url;
	CURLcode res;
	FILE* fp = std::fopen(output.c_str(), "wb");
	curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(handle, CURLOPT_URL, dUrl.c_str());
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp);
	res = curl_easy_perform(handle);
	std::printf("%d\n", res);
	if (res != CURLE_OK) {
		std::printf("Error. Please see error code for more details. ");
		std::printf("Errno: %d\n", res);
		return EXIT_FAILURE;
	}
	curl_easy_cleanup(handle);
	std::fclose(fp);
	return EXIT_SUCCESS;
}
