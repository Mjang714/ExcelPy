from bondMathLib import *
import numpy as np
import matplotlib.pyplot as plt
import xlwings as xw

def generateGraph(x, y, dataSheet) :
    fig = plt.figure()
    plt.plot(x, y)
    plt.xlabel('Time (years)')
    plt.ylabel('Yield (cont. rate)')
    plt.title('Yield Curve') 
    dataSheet.pictures.add(fig, name='Yield', update=True, left=dataSheet.range('A9').left, top=dataSheet.range('A9').top, width=450, height=300)


def compute() :
    wb = xw.Book.caller()

    dataSheet =  wb.sheets['BondInfo']
     
    bondData = dataSheet.range('BondData').expand().value

    timeToMaturity = []
    listOfYield = []

    for x in bondData:
         
     price = x[2] * 100
     contYield = computeYield(x[0],x[1], price , int(x[3]))
     if(len(x) > 4):
          x[4] = contYield
     else:
          x.append(contYield)     
         #x.append(computeDuration(x[0],x[1], price, int(x[3]))
         #x.append(computeDV01(price, x[5]))
     timeToMaturity.append(x[0])
     listOfYield.append(contYield)
        
    dataSheet.range('A2').value = bondData

    
    generateGraph(timeToMaturity, listOfYield, dataSheet)
