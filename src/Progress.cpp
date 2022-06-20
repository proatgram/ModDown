/*
 * Progress.cpp
 *
 *  Created on: Jun 18, 2022
 *      Author: thetimbrick
 */

#include "Progress.h"

void Progress::start() {
	m_progressThread = std::thread([this]() {
		while (this->m_isRunning.get()) {
			this->m_bar->set_progress(this->m_progress());
			sleep(1);
		}
	});
}

void Progress::stop() {
	m_isRunning = false;
}

void Progress::finish() {
	m_bar->mark_as_completed();
}

void Progress::reset() {
	m_bar->set_progress(0);
}

void Progress::setProgress(size_t progress) {
	m_bar->set_progress(progress);
}

void Progress::increment() {
	m_bar->tick();
}
