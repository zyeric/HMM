#include "gibbs_sample.h"
#include <random>
#include <numeric>
#include <algorithm>

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

	std::vector<int> tmp_state_sequence_(length_, 0);
	double eps = 1e-4;
	int iter_cnt = 1;
	for (;;) {
		tmp_state_sequence_[0] = UpdateFront();
		for (int i = 1; i + 1 < length_; ++i) {
			tmp_state_sequence_[i] = UpdateMid(i);
		}
		tmp_state_sequence_[length_ - 1] = UpdateBack();
		std::swap(state_sequence_, tmp_state_sequence_);
		double updt_sum = UpdatePara();
		printf("Finish %d iteration, update sum: %.6lf.\n", iter_cnt, updt_sum);
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

int GibbsSample::RandomSelectCoin(double *prob) {
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

int GibbsSample::UpdateFront() {
	double *prob = new double[3];
	for (int i = 0; i < 3; ++i) {
		prob[i] = estimated_para_.transition_prob_[i][state_sequence_[1]];
		if (observed_sequence_[0]) { prob[i] *= estimated_para_.head_prob_[i]; }
		else { prob[i] *= (1 - estimated_para_.head_prob_[i]); }
	}
	int ret = RandomSelectCoin(prob);
	delete prob;
	return ret;
}

int GibbsSample::UpdateMid(int index) {
	double *prob = new double[3];
	for (int i = 0; i < 3; ++i) {
		prob[i] = estimated_para_.transition_prob_[state_sequence_[index - 1]][i]
			* estimated_para_.transition_prob_[i][state_sequence_[index + 1]];
		if (observed_sequence_[index]) { prob[i] *= estimated_para_.head_prob_[i]; }
		else { prob[i] *= (1 - estimated_para_.head_prob_[i]); }
	}
	int ret = RandomSelectCoin(prob);
	delete prob;
	return ret;
}

int GibbsSample::UpdateBack() {
	double *prob = new double[3];
	for (int i = 0; i < 3; ++i) {
		prob[i] = estimated_para_.transition_prob_[state_sequence_[length_ - 2]][i];
		if (observed_sequence_[length_ - 1]) { prob[i] *= estimated_para_.head_prob_[i]; }
		else { prob[i] *= (1 - estimated_para_.head_prob_[i]); }
	}
	int ret = RandomSelectCoin(prob);
	delete prob;
	return ret;
}

double GibbsSample::UpdatePara() {
	std::vector<int> coin_cnt(3, 0);
	std::vector<int> head_cnt(3, 0);
	std::vector<std::vector<int>> transition_cnt(3, std::vector<int>(3, 0));

	for (int i = 0; i < length_; ++i) {
		++coin_cnt[state_sequence_[i]];
		if (observed_sequence_[i]) { ++head_cnt[state_sequence_[i]]; }
	}

	for (int i = 0; i + 1 < length_; ++i) {
		++transition_cnt[state_sequence_[i]][state_sequence_[i + 1]];
	}

	double ret = 0;
	for (int i = 0; i < 3; ++i) {
		double val = 1.0 * head_cnt[i] / coin_cnt[i];
		ret += abs(val - estimated_para_.head_prob_[i]);
		estimated_para_.head_prob_[i] = val;
	}
	for (int i = 0; i < 3; ++i) {
		int sum = 0;
		for (int j = 0; j < 3; ++j) { sum += transition_cnt[i][j]; }
		for (int j = 0; j < 3; ++j) {
			double val = 1.0 * transition_cnt[i][j] / sum;
			ret += abs(val - estimated_para_.transition_prob_[i][j]);
			estimated_para_.transition_prob_[i][j] = val;
		}
	}

	return ret;
}