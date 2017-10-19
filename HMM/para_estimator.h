#pragma once
#include "util.h"
#include "coin_sequence.h"

class ParaEstimator {
public:
	ParaEstimator(size_t length, std::vector<int>& observed_sequence) :
		length_(length), observed_sequence_(observed_sequence) { 
		estimated_para_ = CoinSequence();
	}
	virtual void Estimate() = 0;
	void InitPara();

	size_t length_;
	std::vector<int> observed_sequence_;
	CoinSequence estimated_para_;
};