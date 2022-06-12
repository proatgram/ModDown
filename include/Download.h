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
#include <utility>

#include "SafeData.hpp"

struct Download {
public:
	int operator()(std::string url, std::string output);
	int changeRange(int OldValue, int OldMax, int OldMin, int NewMax, int NewMin);

	SafeData<std::pair<double, double>>* progress;
private:
	int progress_callback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);
};



#endif /* INCLUDE_DOWNLOAD_H_ */
