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
#include <indicators/cursor_control.hpp>

#include "SafeData.hpp"
#include "Utils.hpp"

struct Unpack {
	public:

	Unpack() :
		m_progress(0),
		m_bar()
	{
		m_bar.set_option(indicators::option::BarWidth{50});
		m_bar.set_option(indicators::option::Start{"["});
		m_bar.set_option(indicators::option::Fill{"-"});
		m_bar.set_option(indicators::option::Lead{">"});
		m_bar.set_option(indicators::option::Remainder{" "});
		m_bar.set_option(indicators::option::End{"]"});
		m_bar.set_option(indicators::option::ShowPercentage{true});
		m_bar.set_option(indicators::option::PostfixText{"Extracting Archive"});
		m_bar.set_option(indicators::option::ForegroundColor{indicators::Color::green});
		m_bar.set_option(indicators::option::FontStyles{std::vector<indicators::FontStyle>{indicators::FontStyle::bold}});
	}

	std::string operator()(std::filesystem::path path);

	SafeData<size_t> m_progress;

	indicators::ProgressBar m_bar;
};



#endif /* INCLUDE_UNPACK_H_ */
