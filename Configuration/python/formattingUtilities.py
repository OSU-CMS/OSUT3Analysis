#!/usr/bin/env python

import math
import decimal
def roundingNumbers(central,sta,sys):
        roundingDic = {}
        sys = float(sys)
        sta = float(sta)
        sysOut = 0
        staOut = 0
        centralOut = 0
        if sys > sta:
                sysRounded = round_sigfigs(sys,2)
		sysOut = modifyByPrecision(sys,sys,sysRounded)
		centralOut = modifyByPrecision(central,sys,sysRounded)
		staOut = modifyByPrecision(sta,sys,sysRounded)
        else:
                staRounded = round_sigfigs(sta,2)
		staOut = modifyByPrecision(sta,sta,staRounded)
		centralOut = modifyByPrecision(central,sta,staRounded)
		sysOut = modifyByPrecision(sys,sta,staRounded)
                
        roundingDic['central'] = centralOut
        roundingDic['sys'] = sysOut
        roundingDic['sta'] = staOut

        print roundingDic['central']
        print roundingDic['sta']
        print roundingDic['sys']
        return roundingDic

def modifyByPrecision(input, origin, originRounded):
	output = 0
	if input is origin:
		if not origin < 10:
			output = int(input)
		elif not origin < 1:
			output = originRounded
		else:
			for n in range(1,100):
                                    if int(int(originRounded*math.pow(10,n))/(originRounded*math.pow(10,n))) is 1 and int(originRounded*math.pow(10,n)) is not 0:
                                                if int(originRounded*math.pow(10,n)) is 0:
                                                        output = decimal.Decimal(originRounded).quantize(decimal.Decimal(str(base/10)))
						else:
							output = originRounded
			 
	if originRounded - int(originRounded) > 0:
		if originRounded > 1:
				output = round(input,1)
		else:
        		for n in range(1,100):
                                    if int(int(originRounded*math.pow(10,n))/(originRounded*math.pow(10,n))) is 1 and int(originRounded*math.pow(10,n)) is not 0:
                                                if int(originRounded*math.pow(10,n)) is not 0:
							base = math.pow(10,-n)
							output = decimal.Decimal(input).quantize(decimal.Decimal(str(base)))
                                                else:
							output = decimal.Decimal(input).quantize(decimal.Decimal(str(base/10)))
                                                break	
	else:
		if originRounded < 10:
			output = round(input,1)
		else:
                	for n in range(0,100):
				precision = 0
                                if int(originRounded/math.pow(10,n)) is 0:
           				precision = math.pow(10,n-2)
                                        output = int((int(input/precision))*precision)
					break

	return output
#function to replace special characters in a cut name
def plainTextString(inputString):

    inputString.replace('<','lt')
    inputString.replace('>','gt')
    inputString.replace('(','')
    inputString.replace(')','')
    inputString.replace('=','eq')
    
    # we have to handle whitespace separately
    # there can be special characters that don't get recognized by just replacing " "
    stringList = list(inputString)
    inputString = ""
    for char in stringList:
        if char.isspace():
            inputString = inputString + "_"
        else:
            inputString = inputString + char

    return inputString
  

#function to put spaces between every 3 digits in a number
def formatNumber(inputNumber):
    inputList = list(inputNumber)
    decimalIndex = inputNumber.find(".")
    if decimalIndex is not -1:# found decimal point
        numDigitsAboveOne = decimalIndex
        numDigitsBelowOne = len(inputList) - numDigitsAboveOne - 1
    else: # didn't find a decimal point
        numDigitsAboveOne = len(inputList)
        numDigitsBelowOne = 0
        
    outputString = "$"

    for index in range(numDigitsAboveOne): #print digits > 1
        outputString = outputString + inputList[index]
        if (numDigitsAboveOne - index - 1) % 3 is 0 and (numDigitsAboveOne - index - 1) is not 0:
            outputString = outputString + "\;"
    if decimalIndex is not -1: #print "." if needed
        outputString = outputString + "."
    for index in range(numDigitsBelowOne): #print digits < 1
        outputString = outputString + inputList[index+numDigitsAboveOne+1]
        if (index+1) % 3 is 0 and (numDigitsBelowOne - index -1) is not 0:
            outputString = outputString + "\;"

    outputString = outputString + "$"

    if outputString.find('$$') is not -1:
        outputString = '$0$'

    return outputString


#function to round to a certain number of significant digits
def round_sigfigs(num, sig_figs):
    if num != 0:
        return round(num, -int(math.floor(math.log10(abs(num))) - (sig_figs - 1)))
    else:
        return 0  # Can't take the log of 0   
