/*
 * Request.h
 *
 *  Created on: Jun 9, 2022
 *      Author: thetimbrick
 */

#ifndef INCLUDE_REQUEST_H_
#define INCLUDE_REQUEST_H_

#include <string>
#include <cstdlib>
#include <initializer_list>
#include <sstream>
#include <curl/curl.h>
#include <curl/easy.h>
#include <utility>
#include <cstdio>


struct Request {
public:

	Request() :
		m_baseURL("\0"),
		m_header(nullptr)
	{

	}
	~Request() {
		free(m_header);
		m_header = nullptr;
	}

	int download(std::string url, std::string output);

	int download(std::string output);

	int download();

	std::stringstream sendGET(std::string endpoint);

	void setBase(std::string baseURL);

	void setHeader(std::pair<std::string, std::string> keys);

	void clearHeader();

private:

	std::string m_baseURL;

	struct curl_slist* m_header;

};



#endif /* INCLUDE_REQUEST_H_ */
