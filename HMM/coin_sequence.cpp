#include "coin_sequence.h"
#include "uniform_generator.h"

CoinSequence::CoinSequence() {
	init_prob_ = { 1.0, 0.0, 0.0 };
	head_prob_ = { 0.5, 0.5, 0.5 };
	/*
	transition_prob_ =
	{ {1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0}, 
	  {1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0},
	  {1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0} };
	*/
	transition_prob_.assign(3, std::vector<double>(3, 0));
	UniformGenerator* gen = new UniformGenerator(0, 1);
	for (int i = 0; i < 3; ++i) {
		double sum = 0;
		for (int j = 0; j < 3; ++j) {
			transition_prob_[i][j] = gen->NextNumber();
			sum += transition_prob_[i][j];
		}
		for (int j = 0; j < 3; ++j) {
			transition_prob_[i][j] /= sum;
		}
	}
}

CoinSequence::CoinSequence(std::vector<double> init_prob,
	std::vector<double> head_prob,
	std::vector<std::vector<double>> transition_prob) {
	init_prob_ = init_prob;
	head_prob_ = head_prob;
	transition_prob_ = transition_prob;
}

void CoinSequence::Generate(int length, std::vector<int>& state_sequence,
	std::vector<int>& observed_sequence) {
	UniformGenerator *gen = new UniformGenerator(0, 1);

	int cur_coin = 0;
	for (int i = 0; i < length; ++i) {
		state_sequence.push_back(cur_coin);
		if (gen->NextNumber() < head_prob_[cur_coin]) { observed_sequence.push_back(1); }
		else { observed_sequence.push_back(0); }
		double val = gen->NextNumber();
		if (val <= transition_prob_[cur_coin][0]) {
			cur_coin = 0;
		}
		else if (val <= transition_prob_[cur_coin][0] + transition_prob_[cur_coin][1]) {
			cur_coin = 1;
		}
		else {
			cur_coin = 2;
		}
	}

	delete gen;
}

void CoinSequence::PrintPara() {
	std::cout << "Head prob:\n" <<
		head_prob_[0] << "\t" << head_prob_[1] << "\t" << head_prob_[2] << std::endl;
	std::cout << "Transition prob:\n" <<
		transition_prob_[0][0] << "\t" << transition_prob_[0][1] << "\t" << transition_prob_[0][2] << std::endl <<
		transition_prob_[1][0] << "\t" << transition_prob_[1][1] << "\t" << transition_prob_[1][2] << std::endl <<
		transition_prob_[2][0] << "\t" << transition_prob_[2][1] << "\t" << transition_prob_[2][2] << std::endl <<
		std::endl;
}

std::vector<int> CoinSequence::CalHiddenState(std::vector<int>& observed_sequence) {
	size_t cur_length = observed_sequence.size();
	std::vector<std::vector<double>> prob(cur_length, std::vector<double>(3, 0));
	std::vector<std::vector<int>> pre(cur_length, std::vector<int>(3, -1));

	//TO FIX
	for (int i = 0; i < 3; ++i) {
		double val = observed_sequence[1] ? head_prob_[i] : (1 - head_prob_[i]);
		prob[1][i] = transition_prob_[0][i] * val;
		pre[1][i] = 0;
	}

	for (int i = 2; i < cur_length; ++i) {
		while (true) {
			int cnt = 0;
			for (int j = 0; j < 3; ++j) {
				if (prob[i - 1][j] < 0.25) {
					++cnt;
				}
			}
			if (cnt == 3) {
				for (int j = 0; j < 3; ++j) { prob[i - 1][j] *= 2; }
			}
			else {
				break;
			}
		}
		for (int j = 0; j < 3; ++j) {
			prob[i][j] = prob[i - 1][0] * transition_prob_[0][j];
			pre[i][j] = 0;
			double val = prob[i - 1][1] * transition_prob_[1][j];
			if (val > prob[i][j]) {
				prob[i][j] = val;
				pre[i][j] = 1;
			}
			val = prob[i - 1][2] * transition_prob_[2][j];
			if (val > prob[i][j]) {
				prob[i][j] = val;
				pre[i][j] = 2;
			}
			prob[i][j] *= observed_sequence[i] ? head_prob_[j] : (1 - head_prob_[j]);
		}
	}

	std::vector<int> ret;
	int pos = 0, index = cur_length - 1;
	if (prob[cur_length - 1][1] > prob[cur_length - 1][pos]) { pos = 1; }
	if (prob[cur_length - 1][2] > prob[cur_length - 1][pos]) { pos = 2; }
	while (true) {
		if (pos == -1) { break; }
		ret.push_back(pos);
		pos = pre[index][pos];
		--index;
	}
	std::reverse(ret.begin(), ret.end());

	return ret;
}