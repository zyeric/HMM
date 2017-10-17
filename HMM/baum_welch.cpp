#include "baum_welch.h"

BaumWelch::BaumWelch(size_t length, std::vector<int>& observed_sequence) : 
	length_(length), observed_sequence_(observed_sequence) {
	forward_prob_.assign(length, std::vector<double>(3, 0));
	backward_prob_.assign(length, std::vector<double>(3, 0));
}

void BaumWelch::Estimate() {
	int iter_cnt = 0;
	double eps = 1e-4;
	for (;;) {
		++iter_cnt;
		CalForwardProb();
		CalBackwardProb();
		if (UpdatePara() <= eps) { break; }
	}
	std::cout << "Iteration cnt: " << iter_cnt << std::endl;
}

void BaumWelch::CalForwardProb() {
	forward_prob_[0][0] = observed_sequence_[0] ?
		estimated_para_.head_prob_[0] : (1 - estimated_para_.head_prob_[0]);
	forward_prob_[0][1] = 0;
	forward_prob_[0][2] = 0;

	for (size_t i = 1; i < length_; ++i) {
		for (int j = 0; j < 3; ++j) {
			forward_prob_[i][j] = 0;
			for (int k = 0; k < 3; ++k) {
				forward_prob_[i][j] += 
					forward_prob_[i - 1][k] * estimated_para_.transition_prob_[k][j];
			}
			double val = 
				observed_sequence_[i] ? estimated_para_.head_prob_[j] : (1 - estimated_para_.head_prob_[j]);
			forward_prob_[i][j] *= val;
		}
		NormalizeProb(forward_prob_[i]);
	}
}

void BaumWelch::CalBackwardProb() {
	backward_prob_[length_ - 1][0] = 1;
	backward_prob_[length_ - 1][1] = 1;
	backward_prob_[length_ - 1][2] = 1;

	for (int i = length_ - 2; i >= 0; --i) {
		for (int j = 0; j < 3; ++j) {
			backward_prob_[i][j] = 0;
			for (int k = 0; k < 3; ++k) {
				double head_prob =
					observed_sequence_[i + 1] ? estimated_para_.head_prob_[k] : (1 - estimated_para_.head_prob_[k]);
				double transition_prob = estimated_para_.transition_prob_[j][k];
				backward_prob_[i][j] += head_prob * transition_prob * backward_prob_[i + 1][k];
			}
		}
		NormalizeProb(backward_prob_[i]);
	}
}

double BaumWelch::UpdatePara() {
	std::vector<std::vector<std::vector<double>>> xi;
	xi.assign(length_ - 1, std::vector<std::vector<double>>(3, std::vector<double>(3, 0)));
	
	for (size_t i = 0; i + 1 < length_; ++i) {
		double sum = 0;
		for (int j = 0; j < 3; ++j) {
			for (int k = 0; k < 3; ++k) {
				double val =
					observed_sequence_[i + 1] ? estimated_para_.head_prob_[k] : (1 - estimated_para_.head_prob_[k]);
				xi[i][j][k] += forward_prob_[i][j] * estimated_para_.transition_prob_[j][k] *
					val * backward_prob_[i + 1][k];
				sum += xi[i][j][k];
			}
		}
		for (int j = 0; j < 3; ++j) {
			for (int k = 0; k < 3; ++k) {
				xi[i][j][k] /= sum;
			}
		}
	}

	std::vector<double> gamma_acc;
	gamma_acc.assign(3, 0);

	for (size_t i = 0; i + 1 < length_; ++i) {
		for (int j = 0; j < 3; ++j) {
			for (int k = 0; k < 3; ++k) {
				gamma_acc[j] += xi[i][j][k];
			}
		}
	}

	double ret = 0;

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			double tmp = 0;
			for (size_t k = 0; k + 1 < length_; ++k) {
				tmp += xi[k][i][j];
			}
			tmp /= gamma_acc[i];
			ret += abs(tmp - estimated_para_.transition_prob_[i][j]);
			estimated_para_.transition_prob_[i][j] = tmp;
		}
	}

	for (int i = 0; i < 3; ++i) {
		double tmp = 0;
		for (size_t j = 0; j + 1 < length_; ++j) {
			if (observed_sequence_[j]) {
				for (int k = 0; k < 3; ++k) {
					tmp += xi[j][i][k];
				}
			}
		}
		tmp /= gamma_acc[i];
		ret += abs(tmp - estimated_para_.head_prob_[i]);
		estimated_para_.head_prob_[i] = tmp;
	}

	return ret;
}

void BaumWelch::NormalizeProb(std::vector<double>& prob) {
	double sum = 0;
	std::for_each(prob.begin(), prob.end(), [&sum](double val) { sum += val; });
	std::for_each(prob.begin(), prob.end(), [sum](double &val) { val /= sum; });
}