import xlwings as xw
import bondMathLib as bml

@xw.func
def double_sum(x, y):
    """Returns twice the sum of the two arguments"""
    return 2 * (x + y)

@xw.func
def computeDuration(years, couponRate, bondPrice, compounding):
    return bml.computeDuration(years, couponRate, bondPrice * 100, int(compounding))

