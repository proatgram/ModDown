/*
 * Progress.cpp
 *
 *  Created on: Jun 11, 2022
 *      Author: thetimbrick
 */

#include "Progress.h"

void Progress::setCurrentDownload(std::string cur) {
	m_currentlyDownloading.set(cur);
}

int Progress::start() {
	m_updateThread = std::thread([this]() {
		while(m_running.get() == true) {
			m_dl.processLocked([this](Download& dl) {
				m_downloadProgress.set(dl.progress);
				std::printf("%ld\n", dl.progress.first);
			});
		}
	});

	m_thread = std::thread([this]() {
		indicators::ProgressBar downloading{
			indicators::option::BarWidth{50},
		    indicators::option::Start{" ["},
		    indicators::option::Fill{"█"},
		    indicators::option::Lead{"█"},
		    indicators::option::Remainder{"-"},
		    indicators::option::End{"]"},
			indicators::option::PrefixText{m_currentlyDownloading.get()},
			indicators::option::ForegroundColor{indicators::Color::blue},
		    indicators::option::ShowElapsedTime{true},
		    indicators::option::ShowRemainingTime{true},
			indicators::option::FontStyles{std::vector<indicators::FontStyle>{indicators::FontStyle::bold}}
		};

		indicators::ProgressBar remaining{
			indicators::option::BarWidth{50},
		    indicators::option::Start{" ["},
		    indicators::option::Fill{"-"},
		    indicators::option::Lead{">"},
		    indicators::option::Remainder{"-"},
		    indicators::option::End{"]"},
			indicators::option::PrefixText{"Remaining Downloads"},
			indicators::option::ForegroundColor{indicators::Color::green},
		    indicators::option::ShowElapsedTime{true},
		    indicators::option::ShowRemainingTime{true},
			indicators::option::FontStyles{std::vector<indicators::FontStyle>{indicators::FontStyle::bold}}
		};


		remaining.set_progress(0);
		downloading.set_progress(0);
		for (int times = 0; times < m_totalDownloadProgress.get().second; times++) {
			while (!downloading.is_completed()) {
				long prog = m_downloadProgress.get().first;
				std::printf("\r%ld", prog);
				size_t progress = (size_t)((((prog - 0) * (100 - 0)) / (m_totalDownloadProgress.get().second - 0)) + 0);
				downloading.set_progress(progress);
			}
			//downloading.set_option(indicators::option::PrefixText{m_currentlyDownloading.get()});
			//downloading.set_progress(0);
			//remaining.set_progress(((((times - m_totalDownloadProgress.get().second) * (100 - 0)) / (m_totalDownloadProgress.get().second - 0)) + 0));
		}
		m_running.set(false);
		return 0;
	});
	return 0;
}
