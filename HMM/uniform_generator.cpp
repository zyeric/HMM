#include "uniform_generator.h"

UniformGenerator::UniformGenerator(double low, double high) {
	gen = std::mt19937(rd());
	dis = std::uniform_real_distribution<double>(low, high);
}

double UniformGenerator::NextNumber() {
	return dis(gen);
}