#pragma once
#include "util.h"

class ParaEstimator {
public:
	ParaEstimator(size_t length, std::vector<int> observed_sequence) :
		length_(length), observed_sequence_(observed_sequence) {};
	virtual void Estimate() = 0;

	size_t length_;
	std::vector<int> observed_sequence_;
};