#!/usr/bin/env python

import math

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
