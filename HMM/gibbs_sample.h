#pragma once
#include <vector>

class GibbsSample {
public:
	GibbsSample(int length, std::vector<std::pair<int, int>> generated_sequence);
private:
	std::vector<int> observed_sequence_;
	std::vector<int> state_sequence_;
};