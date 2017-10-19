#pragma once
#include "para_estimator.h"
#include "uniform_generator.h"

class GibbsSample : public ParaEstimator{
public:
	GibbsSample(size_t length, std::vector<int>& observerd_sequence);
	~GibbsSample();
	void Estimate() override;

private:
	void InitSequence();
	std::vector<double> CalProb(size_t index);
	double UpdatePara(std::vector<std::vector<int>>& sequences);
	int RandomSelectCoin(std::vector<double>& prob);
	std::vector<int> RandomSampleSequence(std::vector<std::vector<double>>& probs);
	UniformGenerator* gen;

private:
	bool is_converged_;
	std::vector<int> state_sequence_;
};