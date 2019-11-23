#include "BondMath.h"
#include <pybind11/pybind11.h>
#include <vector>

using namespace pybind11::literals;

PYBIND11_MODULE(bondMathLib, m) {
	//Do not define the macro Py_LIMITED_API;

	//optional module docstring 
	m.doc() = "pybind11 BondMath Wrapper";
	//m.def("pythonName", &functionReference, "function description", "args name so when you call help(func_name) within python you can see what the name of the args"_a)
	m.def("computeYield", &getYield, "finds the yield of a bond when given price, coupone rate, time till maturity, and compounding", "years"_a ,"couponRate"_a, "bondPrice"_a, "compound"_a);
	m.def("computeDuration", &getDuration, "finds the Duration of a bond when given price, coupone rate as percent, time till maturity, and compounding", "years"_a, "couponRate"_a, "bondPrice"_a, "compound"_a);
	m.def("computeDV01", &computeDV01, "computes DV01 with input of price and Duration which can be found from the computeDuration", "bondPrice"_a, "bondDuration"_a);
	m.def("computeFiniteDifference", &computeFiniteDifference, "computes the fintie delta of a bond given bond price, bond duration, and delta", "bondPrice"_a, "bondDuration"_a,"difference"_a);
	m.def("compoundedToContRate", &compoundedToContRate, "converts a given compounded rate to a cont rate given the compounded rate and the compounding", "rate"_a, "compounding"_a);
	m.def("contRateToCompounded", &contRateToCompounded, "converts a cont. rate to a comounded rate given the cont rate and the compounded of the new compounded rate", "rate"_a, "compounding"_a);
}