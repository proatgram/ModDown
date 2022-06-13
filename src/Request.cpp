/*
 * Request.cpp
 *
 *  Created on: Jun 9, 2022
 *      Author: thetimbrick
 */

#include "Request.h"

static size_t write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
	std::stringstream* stream = (std::stringstream*)userdata;
	*stream << ptr;
	return nmemb * size;
}

int Request::download(std::string url, std::string output) {
	CURL *handle = curl_easy_init();
	std::string dUrl = url;
	CURLcode res;
	FILE* fp = std::fopen(output.c_str(), "wb");
	curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(handle, CURLOPT_URL, dUrl.c_str());
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp);
	res = curl_easy_perform(handle);
	if (res != CURLE_OK) {
		std::printf("Error. Please see error code for more details. ");
		std::printf("Errno: %d: \n", res);
		std::printf("Error: %s\n", curl_easy_strerror(res));
		return EXIT_FAILURE;
	}
	curl_easy_cleanup(handle);
	std::fclose(fp);
	return EXIT_SUCCESS;
}

int Request::download(std::string output) {
	CURL *handle = curl_easy_init();
	std::string dUrl = m_baseURL;
	CURLcode res;
	FILE* fp = std::fopen(output.c_str(), "wb");
	curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(handle, CURLOPT_URL, dUrl.c_str());
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp);
	res = curl_easy_perform(handle);
	if (res != CURLE_OK) {
		std::printf("Error. Please see error code for more details. ");
		std::printf("Errno: %d\n", res);
		std::printf("Error: %s\n", curl_easy_strerror(res));
		return EXIT_FAILURE;
	}
	curl_easy_cleanup(handle);
	std::fclose(fp);
	return EXIT_SUCCESS;
}

int Request::download() {
	CURL *handle = curl_easy_init();
	std::string dUrl = m_baseURL;
	CURLcode res;
	FILE* fp = std::fopen("Downloads", "wb");
	curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(handle, CURLOPT_URL, dUrl.c_str());
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp);
	res = curl_easy_perform(handle);
	curl_easy_cleanup(handle);
	std::fclose(fp);
	free(handle);
	free(fp);
	handle = nullptr;
	fp = nullptr;
	if (res != CURLE_OK) {
		std::printf("Error. Please see error code for more details. ");
		std::printf("Errno: %d\n", res);
		std::printf("Error: %s\n", curl_easy_strerror(res));
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}


std::stringstream Request::sendGET(std::string endpoint) {
	std::stringstream output;
	CURL* handle = curl_easy_init();
	std::string dUrl = m_baseURL;
	CURLcode res;
	curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);
	dUrl.append(endpoint);
	curl_easy_setopt(handle, CURLOPT_URL, dUrl.c_str());
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&output);
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_cb);
	curl_easy_setopt(handle, CURLOPT_HTTPGET, 1L);
	curl_easy_setopt(handle, CURLOPT_USERAGENT, "ModDown/dev");
	curl_easy_setopt(handle, CURLOPT_HTTPHEADER, m_header);
	curl_easy_setopt(handle, CURLOPT_HEADEROPT, CURLHEADER_UNIFIED);
	curl_easy_setopt(handle, CURLOPT_USE_SSL, CURLUSESSL_ALL);
	res = curl_easy_perform(handle);
	if (res != CURLE_OK) {
		std::fprintf(stderr, "Error: Please see error code for more details. ");
		std::fprintf(stderr, "Errno: %d\n", res);
		std::printf("Error: %s\n", curl_easy_strerror(res));
		return std::stringstream("LIBCURLERR");
	}
	handle = nullptr;
	free(handle);
	return output;
}

void Request::setBase(std::string baseURL) {
	m_baseURL = baseURL;
}

void Request::setHeader(std::pair<std::string, std::string> key) {
	std::stringstream st;
	st << key.first << ": " << key.second;
	m_header = curl_slist_append(m_header, st.str().c_str());
}

void Request::clearHeader() {
	curl_slist_free_all(m_header);
}
