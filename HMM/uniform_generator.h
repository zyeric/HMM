#pragma once
#include <random>
#include <numeric>

class UniformGenerator {
public:
	UniformGenerator(double low, double high);
	double NextNumber();
private:
	std::random_device rd;
	std::mt19937 gen;
	std::uniform_real_distribution<double> dis;
};