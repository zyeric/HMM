#include "gibbs_sample.h"

const int kSampleStep = 1;
const double kTolerance = 1e-4;
const int kMaxIteration = 10000;

GibbsSample::GibbsSample(size_t length, std::vector<int>& observed_sequence) 
	: ParaEstimator(length, observed_sequence){
	is_converged_ = false;
	state_sequence_.resize(length);
	gen = new UniformGenerator(0, 1);
}

GibbsSample::~GibbsSample() {
	delete gen;
}

void GibbsSample::Estimate() {
	InitSequence();

	int iter_cnt = 1;
	while (true) {
		std::vector<std::vector<double>> probs;
		for (size_t i = 0; i < length_; ++i) { probs.push_back(CalProb(i)); }
		std::vector<std::vector<int>> seqs;
		for (int i = 0; i < kSampleStep; ++i) {
			seqs.push_back(RandomSampleSequence(probs));
		}
		double updt_sum = UpdatePara(seqs);
		std::swap(state_sequence_, seqs[0]);
		if (updt_sum < kTolerance) {
			is_converged_ = true;
			break;
		}
		if (iter_cnt >= kMaxIteration) {
			std::cout << "Iteration greater than " << kMaxIteration << std::endl;
			break;
		}
		++iter_cnt;
	}
	if (is_converged_) { std::cout << "Iteration cnt: " << iter_cnt << std::endl; }
}

void GibbsSample::InitSequence() {
	for (size_t i = 0; i < length_; ++i) {
		double val = gen->NextNumber();
		if (val <= 1.0 / 3.0) {
			state_sequence_[i] = 0;
		}
		else if (val <= 2.0 / 3.0) {
			state_sequence_[i] = 1;
		}
		else {
			state_sequence_[i] = 2;
		}
	}
}

int GibbsSample::RandomSelectCoin(std::vector<double>& prob) {
	double val = gen->NextNumber();
	int ret = -1;
	if (val <= prob[0]) {
		ret = 0;
	}
	else if (val <= prob[0] + prob[1]) {
		ret = 1;
	}
	else {
		ret = 2;
	}
	return ret;
}

std::vector<int> GibbsSample::RandomSampleSequence(
	std::vector<std::vector<double>>& probs) {
	std::vector<int> ret(length_, 0);
	for (size_t i = 0; i < length_; ++i) {
		ret[i] = RandomSelectCoin(probs[i]);
	}
	return ret;
}

std::vector<double> GibbsSample::CalProb(size_t index) {
	std::vector<double> prob(3, 1);
	for (int i = 0; i < 3; ++i) {
		if (index > 0) { prob[i] *= estimated_para_.transition_prob_[state_sequence_[index - 1]][i]; }
		else { prob[i] *= estimated_para_.init_prob_[i]; }
		if (index + 1 < length_) { prob[i] *= estimated_para_.transition_prob_[i][state_sequence_[index + 1]]; }
		if (observed_sequence_[index]) { prob[i] *= estimated_para_.head_prob_[i]; }
		else { prob[i] *= (1 - estimated_para_.head_prob_[i]); }
	}
	double sum = prob[0] + prob[1] + prob[2];
	for (int i = 0; i < 3; ++i) { prob[i] /= sum; }
	return prob;
}

double GibbsSample::UpdatePara(std::vector<std::vector<int>>& sequences) {
	std::vector<int> coin_cnt(3, 0);
	std::vector<int> head_cnt(3, 0);
	std::vector<std::vector<int>> transition_cnt(3, std::vector<int>(3, 0));

	for (int j = 0; j < kSampleStep; ++j) {
		for (size_t i = 0; i < length_; ++i) {
			++coin_cnt[sequences[j][i]];
			if (observed_sequence_[i]) { ++head_cnt[sequences[j][i]]; }
		}

		for (size_t i = 0; i + 1 < length_; ++i) {
			++transition_cnt[sequences[j][i]][sequences[j][i + 1]];
		}
	}
	double ret = 0;
	for (int i = 0; i < 3; ++i) {
		double val = coin_cnt[i] == 0 ? 0 : 1.0 * head_cnt[i] / coin_cnt[i];
		ret += abs(val - estimated_para_.head_prob_[i]);
		estimated_para_.head_prob_[i] = val;
	}
	for (int i = 0; i < 3; ++i) {
		int sum = 0;
		for (int j = 0; j < 3; ++j) { sum += transition_cnt[i][j]; }
		for (int j = 0; j < 3; ++j) {
			double val = sum == 0 ? 0 : 1.0 * transition_cnt[i][j] / sum;
			ret += abs(val - estimated_para_.transition_prob_[i][j]);
			estimated_para_.transition_prob_[i][j] = val;
		}
	}

	return ret;
}