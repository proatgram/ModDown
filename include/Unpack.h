/*
 * Unpack.h
 *
 *  Created on: Jun 15, 2022
 *      Author: thetimbrick
 */

#ifndef INCLUDE_UNPACK_H_
#define INCLUDE_UNPACK_H_

#include <cstdio>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include <filesystem>
#include <zip.h>
#include <sysexits.h>
#include <fstream>
#include <indicators/progress_bar.hpp>

#include "SafeData.hpp"
#include "Utils.hpp"
#include "Progress.h"

struct Unpack {
	public:

	Unpack() :
		m_progress(0)
	{

	}

	std::string operator()(std::filesystem::path path);

	SafeData<size_t> m_progress;
};



#endif /* INCLUDE_UNPACK_H_ */
