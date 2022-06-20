/*
 * Progress.h
 *
 *  Created on: Jun 18, 2022
 *      Author: thetimbrick
 */

#ifndef INCLUDE_PROGRESS_H_
#define INCLUDE_PROGRESS_H_

#include <indicators/progress_bar.hpp>
#include <indicators/cursor_control.hpp>
#include <thread>
#include <chrono>
#include <vector>

#include "SafeData.hpp"

class Progress {
	public:

	Progress(SafeData<size_t>& progress, std::shared_ptr<indicators::ProgressBar> bar) :
		m_bar(bar),
		m_isRunning(false),
		m_progress(progress)
	{
		indicators::show_console_cursor(false);
	}

	void start();

	void stop();

	void finish();

	void reset();

	void setProgress(size_t progress);

	void increment();

	private:

	std::shared_ptr<indicators::ProgressBar> m_bar;

	SafeData<bool> m_isRunning;

	SafeData<size_t>& m_progress;

	std::thread m_progressThread;
};



#endif /* INCLUDE_PROGRESS_H_ */
