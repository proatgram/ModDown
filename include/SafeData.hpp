/*
   This is a project that is dedicated to downloading Minecraft Mods from CurseForge using their API
    Copyright (C) <2022>  Timothy Hutchins

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

/*
 * SafeData.hpp
 *
 *  Created on: Jun 11, 2022
 *      Author: thetimbrick
 */

#ifndef INCLUDE_SAFEDATA_HPP_
#define INCLUDE_SAFEDATA_HPP_


#include <mutex>
#include <functional>

/**
 * @brief This data will be thread safe using this class
 * <p> This can be used if you want to get data from one thread to another
 *
 * @tparam T
 */
template <typename T>
class SafeData {
    public:
		SafeData(T data) {
			m_data = data;
		}

		SafeData() {

		}
		~SafeData() {

		}

		T operator=(const T& data) {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_data = data;
            return m_data;
		}

		T operator+(const T& data) {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_data += data;
            return m_data;
		}

		T operator()() {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_data;
		}

		T operator()(T data) {
			std::lock_guard<std::mutex> lock(m_mutex);
			m_data = data;
			return m_data;
		}

        /**
         * @brief Sets the data to be read by another thread
         *
         * @param data The data
         */
        void set(T data) {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_data = data;
        }
        /**
         * @brief Returns the data
         *
         * @return T The data type
         */
        T get() {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_data;
        }

        const T& get() const {
			std::lock_guard<std::mutex> lock(m_mutex);
			return m_data;
		}

        /**
         * @brief A function used to get and modify `T`
         *
         * @param func A function that takes in a `T` reference
         * <p> The funtion then gets called with the `T` type
         * <p> In the function you would program what you wanted to do the `T`
         * <p> This is usefull if you need to get `T` in order to modify it or for any other reason
         */
        void processLocked(std::function<void(T&)> func) {
            std::lock_guard<std::mutex> lock(m_mutex);
            func(m_data);
        }
        /**
         * @brief A function used to get and modify `T`
         *
         * @param func A function that takes in a `T` reference
         * <p> The funtion then gets called with the `T` type
         * <p> In the function you would program what you wanted to do the `T`
         * <p> This is usefull if you need to get `T` in order to modify it or for any other reason
         */
        void processLocked(std::function<void(const T&)> func) const {
            std::lock_guard<std::mutex> lock(m_mutex);
            func(m_data);
        }
    private:
        mutable std::mutex m_mutex;
        T m_data;
};



#endif /* INCLUDE_SAFEDATA_HPP_ */
