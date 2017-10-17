#pragma once
#include "util.h"
#include "coin_sequence.h"

class BaumWelch {
public:
	BaumWelch(size_t length, std::vector<int>& observed_sequence);
	void Estimate();

	size_t length_;
	std::vector<int> observed_sequence_;
	CoinSequence estimated_para_;

private:
	void CalForwardProb();
	void CalBackwardProb();
	double UpdatePara();

	std::vector<std::vector<double>> forward_prob_;
	std::vector<std::vector<double>> backward_prob_;
};