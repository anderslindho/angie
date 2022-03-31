#pragma once

#include <string>
#include <iostream>
#include <fstream>

std::string readFile(const std::string &fname) {
	std::ifstream ifs(fname, std::ios::in);
	if (!ifs.is_open()) {
		std::cerr << "Failed to load file " << fname << std::endl;
		return "";
	}

	return std::string(
		(std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>())
	);
}