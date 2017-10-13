#include "gibbs_sample.h"
#include <random>
#include <numeric>
#include <algorithm>
#include <iostream>

const int kSampleStep = 1;

GibbsSample::GibbsSample(int length, std::vector<int> observed_sequence) {
	length_ = length;
	for (auto item : observed_sequence) {
		observed_sequence_.push_back(item);
	}
	state_sequence_.resize(length);
	estimated_para_ = CoinSequence();
	gen = new UniformGenerator(0, 1);
}

GibbsSample::~GibbsSample() {
	delete gen;
}

void GibbsSample::Estimate() {
	InitSequence();

	double eps = 1e-4;
	int iter_cnt = 1;
	while (true) {
		std::vector<std::vector<double>> probs;
		for (int i = 0; i < length_; ++i) { probs.push_back(CalProb(i)); }
		std::vector<std::vector<int>> seqs;
		for (int i = 0; i < kSampleStep; ++i) {
			seqs.push_back(RandomSampleSequence(probs));
		}
		double updt_sum = UpdatePara(seqs);
		printf("Finish %d iteration, update sum: %.6lf.\n", iter_cnt, updt_sum);
		std::swap(state_sequence_, seqs[0]);
		if (updt_sum < eps) {
			break;
		}
		++iter_cnt;
	}
}

void GibbsSample::InitSequence() {
	for (int i = 0; i < length_; ++i) {
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

int GibbsSample::RandomSelectCoin(std::vector<double> prob) {
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
	std::vector<std::vector<double>> probs) {
	std::vector<int> ret(length_, 0);
	for (int i = 0; i < length_; ++i) {
		ret[i] = RandomSelectCoin(probs[i]);
	}
	return ret;
}

std::vector<double> GibbsSample::CalProb(int index) {
	std::vector<double> prob(3, 1);
	for (int i = 0; i < 3; ++i) {
		if (index > 0) { prob[i] *= estimated_para_.transition_prob_[state_sequence_[index - 1]][i]; }
		if (index + 1 < length_) { prob[i] *= estimated_para_.transition_prob_[i][state_sequence_[index + 1]]; }
		if (observed_sequence_[index]) { prob[i] *= estimated_para_.head_prob_[i]; }
		else { prob[i] *= (1 - estimated_para_.head_prob_[i]); }
	}
	double sum = prob[0] + prob[1] + prob[2];
	for (int i = 0; i < 3; ++i) { prob[i] /= sum; }
	return prob;
}

double GibbsSample::UpdatePara(std::vector<std::vector<int>> sequences) {
	std::vector<int> coin_cnt(3, 0);
	std::vector<int> head_cnt(3, 0);
	std::vector<std::vector<int>> transition_cnt(3, std::vector<int>(3, 0));

	for (int j = 0; j < kSampleStep; ++j) {
		for (int i = 0; i < length_; ++i) {
			++coin_cnt[sequences[j][i]];
			if (observed_sequence_[i]) { ++head_cnt[sequences[j][i]]; }
		}

		for (int i = 0; i + 1 < length_; ++i) {
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