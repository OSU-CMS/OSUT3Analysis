#!/usr/bin/env python

import math

# a measurement is a real number with an uncertainty (possibly asymmetric)
class Measurement:
    _centralValue = None
    _uncertaintyDown = None
    _uncertaintyUp = None
    _printTeX = False
    _maxSigFigsInUncertainty = 2
    _printLongFormat = False

    def __init__ (self, centralValue = None, uncertaintyDown = None, uncertaintyUp = None):
        self._centralValue = float (centralValue)
        if uncertaintyDown is not None:
            self._uncertaintyDown = float (uncertaintyDown)
        if uncertaintyUp is not None:
            self._uncertaintyUp = float (uncertaintyUp)
        if uncertaintyDown is not None and uncertaintyUp is None:
            self._uncertaintyUp = float (uncertaintyDown)
        if uncertaintyUp is not None and uncertaintyDown is None:
            self._uncertaintyDown = float (uncertaintyUp)

    ############################################################################
    # Setters.
    ############################################################################

    def setCentralValue (self, centralValue):
        self._centralValue = float (centralValue)

    def setUncertainty (self, uncertainty):
        self._uncertaintyDown = float (uncertainty)
        self._uncertaintyUp = float (uncertainty)

    def setUncertainty (self, uncertaintyDown, uncertaintyUp):
        self._uncertaintyDown = float (uncertaintyDown)
        self._uncertaintyUp = float (uncertaintyUp)

    def setUncertaintyDown (self, uncertaintyDown):
        self._uncertaintyDown = float (uncertaintyDown)

    def setUncertaintyUp (self, uncertaintyUp):
        self._uncertaintyUp = float (uncertaintyUp)

    def setCentralValueAndUncertainty (self, centralValue, uncertainty):
        self._centralValue = float (centralValue)
        self._uncertaintyDown = float (uncertainty)
        self._uncertaintyUp = float (uncertainty)

    def setCentralValueAndUncertainty (self, centralValue, uncertaintyDown, uncertaintyUp):
        self._centralValue = float (centralValue)
        self._uncertaintyDown = float (uncertaintyDown)
        self._uncertaintyUp = float (uncertaintyUp)

    def printTeX (self, printTeX = True):
        self._printTeX = bool (printTeX)

    def setMaxSigFigsInUncertainty (self, maxSigFigsInUncertainty):
        self._maxSigFigsInUncertainty = int (maxSigFigsInUncertainty)

    def isPositive (self, isPositive = True):
        if self._centralValue - self._uncertaintyDown < 0.0:
          self._uncertaintyDown = self._centralValue

    def printLongFormat (self, printLongFormat = True):
        self._printLongFormat = printLongFormat

    ############################################################################
    # Getters.
    ############################################################################

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

    def minUncertainty (self):
        return min (self._uncertaintyDown, self._uncertaintyUp)

    def maxUncertainty (self):
        return max (self._uncertaintyDown, self._uncertaintyUp)

    def centralValueAndUncertainty (self):
        if self._uncertaintyDown == self._uncertaintyUp:
          return (self._centralValue, self._uncertaintyDown)
        else:
          return (self._centralValue, self._uncertaintyDown, self._uncertaintyUp)

    def centralValueAndUncertaintyUpAndDown (self):
        return (self._centralValue, self._uncertaintyDown, self._uncertaintyUp)

    def maxSigFigsInUncertainty (self):
        return self._maxSigFigsInUncertainty

    ############################################################################
    # Miscellaneous.
    ############################################################################

    def roundAccordingToUncertainty (self):
        uncertainty = self._uncertaintyDown if self._uncertaintyDown > self._uncertaintyUp else self._uncertaintyUp
        exponent = (int (math.floor (math.log10 (uncertainty))) if uncertainty != 0.0 else -1)

        uncertaintyDown = round (self._uncertaintyDown, self._maxSigFigsInUncertainty - 1 - exponent)
        uncertaintyUp = round (self._uncertaintyUp, self._maxSigFigsInUncertainty - 1 - exponent)
        centralValue = round (self._centralValue, self._maxSigFigsInUncertainty - 1 - exponent)

        return (centralValue, uncertaintyDown, uncertaintyUp)

    ############################################################################
    # Methods to overload standard operators are defined below.
    ############################################################################

    def __str__ (self):
        if self._printLongFormat:
            (centralValue, uncertaintyDown, uncertaintyUp) = self.centralValueAndUncertaintyUpAndDown ()
        else:
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
            return (self._centralValue < float (other))

    def __eq__ (self, other):
        if hasattr (other, "centralValue"):
            return (self._centralValue == other.centralValue ())
        else:
            return (self._centralValue == float (other))

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
            return Measurement (self._centralValue + float (other), self._uncertaintyDown, self._uncertaintyUp)

    def __sub__ (self, other):
        return self.__add__ (-other)

    def __mul__ (self, other):
        if hasattr (other, "centralValue"):
            return Measurement (self._centralValue * other.centralValue (), math.hypot (self._uncertaintyDown * other.centralValue (), self._centralValue * other.uncertaintyDown ()), math.hypot (self._uncertaintyUp * other.centralValue (), self._centralValue * other.uncertaintyUp ()))
        else:
            return Measurement (self._centralValue * float (other), self._uncertaintyDown * float (other), self._uncertaintyUp * float (other))

    def __truediv__ (self, other):
        if hasattr (other, "centralValue"):
            return Measurement (self._centralValue / other.centralValue (), math.hypot (self._uncertaintyDown * other.centralValue (), self._centralValue * other.uncertaintyDown ()) / (other.centralValue () * other.centralValue ()), math.hypot (self._uncertaintyUp * other.centralValue (), self._centralValue * other.uncertaintyUp ()) / (other.centralValue () * other.centralValue ()))
        else:
            return Measurement (self._centralValue / float (other), self._uncertaintyDown / float (other), self._uncertaintyUp / float (other))

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
