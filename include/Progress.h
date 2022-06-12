/*
 * Progress.h
 *
 *  Created on: Jun 11, 2022
 *      Author: thetimbrick
 */

#ifndef INCLUDE_PROGRESS_H_
#define INCLUDE_PROGRESS_H_

#include <utility>
#include <thread>
#include <chrono>

#include "Download.h"
#include "SafeData.hpp"

#include <indicators/cursor_control.hpp>
#include <indicators/progress_bar.hpp>

struct Progress {
	public:
		Progress(SafeData<Download>& dl, SafeData<std::pair<int, int>>& totalDownloadProgress) :
			m_dl(dl),
			m_totalDownloadProgress(totalDownloadProgress),
			m_downloadProgress(std::pair<long, long>(m_dl.get().progress))
		{
			indicators::show_console_cursor(false);
			m_running.set(true);
		}

		int start();

		void setCurrentDownload(std::string cur);
	private:
		SafeData<Download>& m_dl;
		SafeData<std::pair<int, int>>& m_totalDownloadProgress;
		SafeData<std::pair<long, long>> m_downloadProgress;

		void m_update();
	protected:
		std::thread m_thread;
		std::thread m_updateThread;
		SafeData<std::string> m_currentlyDownloading;
		SafeData<bool> m_running;
};



#endif /* INCLUDE_PROGRESS_H_ */
