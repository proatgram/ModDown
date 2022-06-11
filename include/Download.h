/*
 * Download.h
 *
 *  Created on: Jun 9, 2022
 *      Author: thetimbrick
 */

#ifndef INCLUDE_DOWNLOAD_H_
#define INCLUDE_DOWNLOAD_H_

#include <string>
#include <curl/curl.h>
#include <curl/easy.h>

struct Download {
public:
	int operator()(std::string url, std::string output);

private:

};



#endif /* INCLUDE_DOWNLOAD_H_ */
