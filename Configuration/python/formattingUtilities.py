#!/usr/bin/env python

import math
import decimal

def roundingNumbers(central_value,stat_error,syst_error):
        roundingDic = {}
        syst_error = float(syst_error)
        stat_error = float(stat_error)
        syst_errorOut = 0
        stat_errorOut = 0
        central_valueOut = 0

	#take the larger error (syst or stat) and round it to 2 sig figs
	
        if syst_error > stat_error:
                syst_errorRounded = round_sigfigs(syst_error,2)
		syst_errorOut = modifyByPrecision(syst_error,syst_error,syst_errorRounded)
		central_valueOut = modifyByPrecision(central_value,syst_error,syst_errorRounded)
		stat_errorOut = modifyByPrecision(stat_error,syst_error,syst_errorRounded)
        else:
                if stat_error <= 0:
			syst_errorOut = decimal.Decimal("0").quantize(decimal.Decimal("0"))
			stat_errorOut = decimal.Decimal("0").quantize(decimal.Decimal("0"))
			central_valueOut = decimal.Decimal(str(central_value)).quantize(decimal.Decimal("0"))
		else:
			stat_errorRounded = round_sigfigs(stat_error,2)
			stat_errorOut = modifyByPrecision(stat_error,stat_error,stat_errorRounded)
			central_valueOut = modifyByPrecision(central_value,stat_error,stat_errorRounded)
			syst_errorOut = modifyByPrecision(syst_error,stat_error,stat_errorRounded)
                
        roundingDic['central_value'] = central_valueOut
        roundingDic['syst_error'] = syst_errorOut
        roundingDic['stat_error'] = stat_errorOut

        return roundingDic

def modifyByPrecision(input, origin, originRounded):
	output = 0
	if input is origin:
		if not origin < 10:
			output = int(input)
		elif not origin < 1:
			output = originRounded
		elif not input > 0:
			output = decimal.Decimal(str(input)).quantize(decimal.Decimal("0"))
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
                                if int(originRounded*math.pow(10,n)) is not 0:
                                                base = math.pow(10,-n-1)
						output = decimal.Decimal(str(input)).quantize(decimal.Decimal(str(base)))  
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
    if num != 0 and not math.isnan(num):
        return round(num, -int(math.floor(math.log10(abs(num))) - (sig_figs - 1)))
    else:
        return 0  # Can't take the log of 0   
