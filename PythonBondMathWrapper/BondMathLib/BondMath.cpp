#include<vector>
#include<string>
#include<iostream>
#include<cmath>
#include<stdlib.h>
#include<algorithm>

#include "BondMath.h"

double compoundedToContRate(double rate, int compounding) 
{
    return compounding*log(1 + (rate/compounding));
}

double contRateToCompounded(double rate, int compounding) 
{
    return compounding*(exp(rate/compounding)-1);
}

double firstOrderPrice(double yield, std::vector<double> maturitySchedule, std::vector<double> cashflowSchedule) 
{
    double result = 0.0;
    
    for(int i = 0; i < cashflowSchedule.size(); i++) {
        result += cashflowSchedule[i]*maturitySchedule[i]*exp(-yield * maturitySchedule[i]);
    }
    
    return result;
}

double price(double yield, std::vector<double> maturitySchedule, std::vector<double> cashflowSchedule) 
{
    double result = 0.0;
    
    for(int i = 0; i < cashflowSchedule.size(); i++) {
        result += cashflowSchedule[i]*exp(-yield * maturitySchedule[i]);
    }    
 
    return result;
}

// Here we are computing the yield of the bond recursively
double computeYieldHelper(double yield, double bondPrice, std::vector<double> maturitySchedule, std::vector<double> cashflowSchedule, double threshold = 0.00000000001) 
{
    double newYield = yield + ((price(yield,maturitySchedule,cashflowSchedule) - bondPrice)/ firstOrderPrice(yield,maturitySchedule,cashflowSchedule));
    
    if(abs(newYield - yield) > threshold){
        return computeYieldHelper(newYield, bondPrice,maturitySchedule,cashflowSchedule, threshold);
    }
    
    else{
        return newYield;
    }
}

// Here the bond yield is computed via newton method and returned as the continusous rate
double computeYield(std::vector<double> maturitySchedule, std::vector<double> cashflowSchedule, double bondPrice)
{

    return computeYieldHelper(0.0,bondPrice,maturitySchedule,cashflowSchedule);
}

//created an independent function to convert string payment to int fomr the enum in BondMath.h 
int convertPaymentStringToEnum(std::string paymentFreqStr)
{
    try
    {
        if (paymentFreqStr == "PA")
        {
            return PerAnum;
        }
        else if (paymentFreqStr == "SA")
        {
            return SemiAnum;

        }
        else if (paymentFreqStr == "Q")
        {
            return Quarterly;
        }
        else if (paymentFreqStr == "M")
        {
            return Monthly;
        }
        else
        {
            throw "Not a proper input";
        }
    }
    catch (const std::exception&)
    {
        //thinking of creating a log file to record exceptions not clear as of right now how to even do that
    }

}

//functions with coups Frequancy as raw int 
std::vector<double> createTimeMaturitySchedule(int paymentFreq, double years) 
{
    
    int numberOfMaturities = paymentFreq*years;
    
    double unitOfTime = 1.0/paymentFreq;

	//if the time span is less than a year default to one so the vector will have one spot
	if (numberOfMaturities < 1) 
    {
		numberOfMaturities = 1;
		unitOfTime = years;
	}
    
    std::vector<double> maturitySchedule(numberOfMaturities);
    maturitySchedule[0] = unitOfTime;
    
    for(int i = 1; i < numberOfMaturities; i++) 
    {
        maturitySchedule[i] = unitOfTime + maturitySchedule[i- 1];
    }
    
    return maturitySchedule;
}

std::vector<double> createCashFlowGen(double years, double principle, int paymentFreq, double couponRate) 
{
    //Generating coupon payments and the numbers of payments
    
    //here we assume the interest rate is given as continous yearly rate
    double cashFlow = (couponRate/paymentFreq) * principle;
    int numberOfCashfows = int (years*paymentFreq);    

    //if the bond is less than a year default the number of cashflows to one 
	if (numberOfCashfows < 1) {
		numberOfCashfows = 1;
	}

    //generating the vector of coupon payments
    std::vector<double> couponPaymentsSchedule(numberOfCashfows);
    
    for(int i = 0; i < numberOfCashfows; i++) {
        couponPaymentsSchedule[i] = cashFlow;
    }
    
    //add the principle payment to the last payment schedule
    couponPaymentsSchedule[numberOfCashfows-1] = principle + couponPaymentsSchedule[numberOfCashfows - 1];
    
    return couponPaymentsSchedule;   
}

//this is an intermediate function is used to create present value cash flows
std::vector<double> computeDiscountedCashflow(double yield, std::vector<double> maturitySchedule, std::vector<double> cashflowSchedule ) 
{
    std::vector<double> presentValueCashflow(maturitySchedule.size());

    for (int i  = 0; i < presentValueCashflow.size(); i++)  {
        //computing the discounted Cash flow
        presentValueCashflow[i] = cashflowSchedule[i] * exp(-yield * maturitySchedule[i]);
    }

    return presentValueCashflow;
} 

double computeDuration(double yield, double bondPrice, std::vector<double> maturitySchedule, std::vector<double> cashflowSchedule) 
{
    std::vector<double> presentCashflow = computeDiscountedCashflow(yield, maturitySchedule, cashflowSchedule);
    
    double duration = 0.0;
    for(int i = 0; i < presentCashflow.size(); i++) {
        duration += (maturitySchedule[i] * presentCashflow[i]) / bondPrice; 
    }
    
    return duration;
}

inline double computeFiniteDifference(double bondPrice, double bondDuration, double difference) 
{
    return -1*bondPrice*bondDuration*difference;
}

double computeDV01(double bondPrice, double bondDuration) 
{
    return computeFiniteDifference(bondPrice, bondDuration, .0001);
}

double getYield(double years, double couponRate, double bondPrice, int compounding) 
{
	std::vector<double> maturitySchedule = createTimeMaturitySchedule(compounding, years);
	std::vector<double> cashflowSchedule = createCashFlowGen(years, 100, compounding, couponRate);
	double yield = computeYield(maturitySchedule, cashflowSchedule, bondPrice);

	return yield;
}

double getDuration(double years, double couponRate, double bondPrice, int compounding) 
{
	std::vector<double> maturitySchedule = createTimeMaturitySchedule(compounding, years);
	std::vector<double> cashflowSchedule = createCashFlowGen(years, 100, compounding, couponRate);
	double yield = computeYield(maturitySchedule, cashflowSchedule, bondPrice);

	return computeDuration(yield, bondPrice, maturitySchedule, cashflowSchedule);
}

double getYield(double years, double couponRate, double bondPrice, std::string compoundingStr)
{
    int paymentFreqInt = convertPaymentStringToEnum(compoundingStr);
    getYield(years, couponRate, bondPrice, paymentFreqInt);
}
