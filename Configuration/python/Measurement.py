#!/usr/bin/env python

import math

# a measurement is a real number with an uncertainty (possibly asymmetric)
class Measurement:
    _centralValue = None
    _uncertaintyDown = None
    _uncertaintyUp = None
    _printTeX = False
    _maxSigFigsInUncertainty = 2

    def __init__ (self, centralValue = None, uncertaintyDown = None, uncertaintyUp = None):
        self._centralValue = centralValue
        self._uncertaintyDown = uncertaintyDown
        self._uncertaintyUp = uncertaintyUp
        if uncertaintyDown is not None and uncertaintyUp is None:
            self._uncertaintyUp = uncertaintyDown
        if uncertaintyUp is not None and uncertaintyDown is None:
            self._uncertaintyDown = uncertaintyUp

    def setCentralValue (self, centralValue):
        self._centralValue = centralValue

    def setUncertainty (self, uncertainty):
        self._uncertaintyDown = uncertainty
        self._uncertaintyUp = uncertainty

    def setUncertainty (self, uncertaintyDown, uncertaintyUp):
        self._uncertaintyDown = uncertaintyDown
        self._uncertaintyUp = uncertaintyUp

    def setUncertaintyDown (self, uncertaintyDown):
        self._uncertaintyDown = uncertaintyDown

    def setUncertaintyUp (self, uncertaintyUp):
        self._uncertaintyUp = uncertaintyUp

    def setCentralValueAndUncertainty (self, centralValue, uncertainty):
        self._centralValue = centralValue
        self._uncertaintyDown = uncertainty
        self._uncertaintyUp = uncertainty

    def setCentralValueAndUncertainty (self, centralValue, uncertaintyDown, uncertaintyUp):
        self._centralValue = centralValue
        self._uncertaintyDown = uncertaintyDown
        self._uncertaintyUp = uncertaintyUp

    def centralValue (self):
        return self._centralValue

    def uncertainty (self):
        if self._uncertaintyDown == self._uncertaintyUp:
          return self._uncertaintyDown
        else:
          return (self._uncertaintyDown, self._uncertaintyUp)

    def uncertaintyDown (self):
        return self._uncertaintyDown

    def uncertaintyUp (self):
        return self._uncertaintyUp

    def centralValueAndUncertainty (self):
        if self._uncertaintyDown == self._uncertaintyUp:
          return (self._centralValue, self._uncertaintyDown)
        else:
          return (self._centralValue, self._uncertaintyDown, self._uncertaintyUp)

    def printTeX (self, printTeX = True):
        self._printTeX = printTeX

    def maxSigFigsInUncertainty (self):
        return self._maxSigFigsInUncertainty

    def setMaxSigFigsInUncertainty (self, maxSigFigsInUncertainty):
        self._maxSigFigsInUncertainty = maxSigFigsInUncertainty

    def roundAccordingToUncertainty (self):
        uncertainty = self._uncertaintyDown if self._uncertaintyDown > self._uncertaintyUp else self._uncertaintyUp
        exponent = int (math.floor (math.log10 (uncertainty)))

        uncertaintyDown = round (self._uncertaintyDown, self._maxSigFigsInUncertainty - 1 - exponent)
        uncertaintyUp = round (self._uncertaintyUp, self._maxSigFigsInUncertainty - 1 - exponent)
        centralValue = round (self._centralValue, self._maxSigFigsInUncertainty - 1 - exponent)

        return (centralValue, uncertaintyDown, uncertaintyUp)

    ############################################################################
    # Methods to overload standard operators are defined below.
    ############################################################################

    def __str__ (self):
        (centralValue, uncertaintyDown, uncertaintyUp) = self.roundAccordingToUncertainty ()
        if not self._printTeX:
            if uncertaintyDown == uncertaintyUp:
                return str (centralValue) + " +- " + str (uncertaintyDown)
            else:
                return str (centralValue) + " - " + str (uncertaintyDown) + " + " + str (uncertaintyUp)
        else:
            if uncertaintyDown == uncertaintyUp:
                return str (centralValue) + " \pm " + str (uncertaintyDown)
            else:
                return str (centralValue) + "_{-" + str (uncertaintyDown) + "}^{+" + str (uncertaintyUp) + "}"

    def __lt__ (self, other):
        if hasattr (other, "centralValue"):
            return (self._centralValue < other.centralValue ())
        else:
            return (self._centralValue < other)

    def __eq__ (self, other):
        if hasattr (other, "centralValue"):
            return (self._centralValue == other.centralValue ())
        else:
            return (self._centralValue == other)

    def __le__ (self, other):
        return (self.__lt__ (other) or self.__eq__ (other))

    def __ne__ (self, other):
        return (not self.__eq__ (other))

    def __gt__ (self, other):
        return (not self.__le__ (other))

    def __ge__ (self, other):
        return (self.__gt__ (other) or self.__eq__ (other))

    def __nonzero__ (self):
        return (self._centralValue != 0.0)

    def __bool__ (self):
        return (self._centralValue != 0.0)

    def __len__ (self):
        if self._uncertaintyDown == self._uncertaintyUp:
            return 2
        else:
            return 3

    def __neg__ (self):
        return Measurement (-self._centralValue, self._uncertaintyDown, self._uncertaintyUp)

    def __pos__ (self):
        return Measurement (self._centralValue, self._uncertaintyDown, self._uncertaintyUp)

    def __abs__ (self):
        return Measurement (abs (self._centralValue), self._uncertaintyDown, self._uncertaintyUp)

    def __add__ (self, other):
        if hasattr (other, "centralValue"):
            return Measurement (self._centralValue + other.centralValue (), math.hypot (self._uncertaintyDown, other.uncertaintyDown ()), math.hypot (self._uncertaintyUp, other.uncertaintyUp ()))
        else:
            return Measurement (self._centralValue + other, self._uncertaintyDown, self._uncertaintyUp)

    def __sub__ (self, other):
        return self.__add__ (-other)

    def __mul__ (self, other):
        if hasattr (other, "centralValue"):
            return Measurement (self._centralValue * other.centralValue (), math.hypot (self._uncertaintyDown * other.centralValue (), self._centralValue * other.uncertaintyDown ()), math.hypot (self._uncertaintyUp * other.centralValue (), self._centralValue * other.uncertaintyUp ()))
        else:
            return Measurement (self._centralValue * other, self._uncertaintyDown * other, self._uncertaintyUp * other)

    def __truediv__ (self, other):
        if hasattr (other, "centralValue"):
            return Measurement (self._centralValue / other.centralValue (), math.hypot (self._uncertaintyDown * other.centralValue (), self._centralValue * other.uncertaintyDown ()) / (other.centralValue () * other.centralValue ()), math.hypot (self._uncertaintyUp * other.centralValue (), self._centralValue * other.uncertaintyUp ()) / (other.centralValue () * other.centralValue ()))
        else:
            return Measurement (self._centralValue / other, self._uncertaintyDown / other, self._uncertaintyUp / other)

    def __div__ (self, other):
        return self.__truediv__ (other)

    def __pow__ (self, other):
        return Measurement (pow (self._centralValue, other), abs (other) * self._uncertaintyDown * pow (self._centralValue, other - 1.0), abs (other) * self._uncertaintyUp * pow (self._centralValue, other - 1.0))

    def __radd__ (self, other):
        return self.__add__ (other)

    def __rsub__ (self, other):
        return (-self).__add__ (other)

    def __rmul__ (self, other):
        return self.__mul__ (other)

    def __rtruediv__ (self, other):
        return Measurement (other / self._centralValue, (abs (other) * self._uncertaintyDown) / (self._centralValue * self._centralValue), (abs (other) * self._uncertaintyUp) / (self._centralValue * self._centralValue))

    def __rdiv__ (self, other):
        return self.__rtruediv__ (other)

    ############################################################################
    # Methods to overload those in numpy.
    ############################################################################

    def sqrt (self):
        return self.__pow__ (0.5)

    def exp (self):
        return Measurement (math.exp (self._centralValue), self._uncertaintyDown * math.exp (self._centralValue), self._uncertaintyUp * math.exp (self._centralValue))

    def log (self):
        return Measurement (math.log (self._centralValue), self._uncertaintyDown / abs (self._centralValue), self._uncertaintyUp / abs (self._centralValue))

    def log10 (self):
        return self.log () / math.log (10.0)

    def fabs (self):
        return self.__abs__ ()

    ############################################################################
