#pragma once
#include <chrono>
#include <string>
#include <iostream>

class Timer {

public:
	Timer(const std::string & title) {
		m_startPoint = std::chrono::high_resolution_clock::now();
		m_title = title;
	}

	~Timer() {
		std::chrono::time_point<std::chrono::high_resolution_clock> endPoint = std::chrono::high_resolution_clock::now();
		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startPoint).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endPoint).time_since_epoch().count();

		auto duration = end - start;
		auto ms = duration * 0.001;

		std::cout << m_title << " took " << duration << " us (" << ms << " ms)\n";
	}

private:
	std::string m_title;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_startPoint;
};

