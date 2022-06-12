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
		Progress(Download& dl, SafeData<std::pair<int, int>>& totalDownloadProgress) :
			m_dl(dl),
			m_totalDownloadProgress(totalDownloadProgress),
			m_downloadProgress(m_dl.progress)
		{
			indicators::show_console_cursor(false);
			m_changeRange.set([](int OldValue, int OldMax, int OldMin, int NewMax, int NewMin) {
				return ((((OldValue - OldMin) * (NewMax - NewMin)) / (OldMax - OldMin)) + NewMin);
			});

		}

		int start();

		void setCurrentDownload(std::string cur);
	private:
		Download& m_dl;
		SafeData<std::pair<int, int>> m_totalDownloadProgress;
		SafeData<std::pair<double, double>>*  m_downloadProgress;
		SafeData<std::function<int(int, int, int, int, int)>> m_changeRange;
	protected:
		SafeData<std::string> m_currentlyDownloading;
};



#endif /* INCLUDE_PROGRESS_H_ */
