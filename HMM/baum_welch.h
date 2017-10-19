#pragma once
#include "para_estimator.h"

class BaumWelch : public ParaEstimator{
public:
	BaumWelch(size_t length, std::vector<int>& observed_sequence);
	void Estimate() override;

private:
	void CalForwardProb();
	void CalBackwardProb();
	double UpdatePara();
	void NormalizeProb(std::vector<double>& prob);

	std::vector<std::vector<double>> forward_prob_;
	std::vector<std::vector<double>> backward_prob_;
};