/*
 * Download.cpp
 *
 *  Created on: Jun 9, 2022
 *      Author: thetimbrick
 */

#include "Download.h"

int Download::progress_callback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
	progress = std::pair<double, double>(dlnow, dltotal);
	return 0;
}

static int cb(void* clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
	Download* ptr = (Download*)clientp;
	ptr->progress = std::pair<long int, long int>(dlnow, dltotal);
	return CURL_PROGRESSFUNC_CONTINUE;
}

int Download::operator()(std::string url, std::string output) {
	CURL *handle = curl_easy_init();
	std::string dUrl = url;
	CURLcode res;
	FILE* fp = std::fopen(output.c_str(), "wb");
	curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(handle, CURLOPT_URL, dUrl.c_str());
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp);
	curl_easy_setopt(handle, CURLOPT_XFERINFODATA, this);
	curl_easy_setopt(handle, CURLOPT_XFERINFOFUNCTION, &cb);
	curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 0L);
	curl_easy_setopt(handle, CURLOPT_VERBOSE, 0L);
	curl_easy_setopt(handle, CURLOPT_STDERR, std::fopen("/dev/null", "ro"));
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
