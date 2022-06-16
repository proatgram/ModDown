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


struct Unpack {
	public:

	std::string operator()(std::string filename);

	private:
};



#endif /* INCLUDE_UNPACK_H_ */
