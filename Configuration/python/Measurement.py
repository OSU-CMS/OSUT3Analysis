#!/usr/bin/env python

import math

# a SimpleMeasurement is a real number with an uncertainty (possibly asymmetric)
class SimpleMeasurement:
    _nan = float ("nan")
    _centralValue = _nan
    _uncertaintyDown = _nan
    _uncertaintyUp = _nan
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

    def centralValueAndUncertaintyDownAndUp (self):
        return (self._centralValue, self._uncertaintyDown, self._uncertaintyUp)

    def maxSigFigsInUncertainty (self):
        return self._maxSigFigsInUncertainty

    ############################################################################
    # Miscellaneous.
    ############################################################################

    def roundAccordingToUncertainty (self, uncertainty):
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
            (centralValue, uncertaintyDown, uncertaintyUp) = self.centralValueAndUncertaintyDownAndUp ()
        else:
            (centralValue, uncertaintyDown, uncertaintyUp) = self.roundAccordingToUncertainty (self.maxUncertainty ())
        if not self._printTeX:
            if uncertaintyDown == uncertaintyUp:
                return str (centralValue) + " +- " + str (uncertaintyDown)
            else:
                return str (centralValue) + " - " + str (uncertaintyDown) + " + " + str (uncertaintyUp)
        else:
            if uncertaintyDown == uncertaintyUp:
                return str (centralValue) + " \pm " + str (uncertaintyDown)
            else:
                return str (centralValue) + " {}_{-" + str (uncertaintyDown) + "}^{+" + str (uncertaintyUp) + "}"

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
        return SimpleMeasurement (-self._centralValue, self._uncertaintyDown, self._uncertaintyUp)

    def __pos__ (self):
        return SimpleMeasurement (self._centralValue, self._uncertaintyDown, self._uncertaintyUp)

    def __abs__ (self):
        return SimpleMeasurement (abs (self._centralValue), self._uncertaintyDown, self._uncertaintyUp)

    def __add__ (self, other):
        if hasattr (other, "centralValue"):
            return SimpleMeasurement (self._centralValue + other.centralValue (), math.hypot (self._uncertaintyDown, other.uncertaintyDown ()), math.hypot (self._uncertaintyUp, other.uncertaintyUp ()))
        else:
            return SimpleMeasurement (self._centralValue + float (other), self._uncertaintyDown, self._uncertaintyUp)

    def __sub__ (self, other):
        return self.__add__ (-other)

    def __mul__ (self, other):
        if hasattr (other, "centralValue"):
            return SimpleMeasurement (self._centralValue * other.centralValue (), math.hypot (self._uncertaintyDown * other.centralValue (), self._centralValue * other.uncertaintyDown ()), math.hypot (self._uncertaintyUp * other.centralValue (), self._centralValue * other.uncertaintyUp ()))
        else:
            return SimpleMeasurement (self._centralValue * float (other), self._uncertaintyDown * float (other), self._uncertaintyUp * float (other))

    def __truediv__ (self, other):
        if hasattr (other, "centralValue"):
            return SimpleMeasurement (self._centralValue / other.centralValue (), math.hypot (self._uncertaintyDown * other.centralValue (), self._centralValue * other.uncertaintyDown ()) / (other.centralValue () * other.centralValue ()), math.hypot (self._uncertaintyUp * other.centralValue (), self._centralValue * other.uncertaintyUp ()) / (other.centralValue () * other.centralValue ()))
        else:
            return SimpleMeasurement (self._centralValue / float (other), self._uncertaintyDown / float (other), self._uncertaintyUp / float (other))

    def __div__ (self, other):
        return self.__truediv__ (other)

    def __pow__ (self, other):
        return SimpleMeasurement (pow (self._centralValue, other), abs (other) * self._uncertaintyDown * pow (self._centralValue, other - 1.0), abs (other) * self._uncertaintyUp * pow (self._centralValue, other - 1.0))

    def __radd__ (self, other):
        return self.__add__ (other)

    def __rsub__ (self, other):
        return (-self).__add__ (other)

    def __rmul__ (self, other):
        return self.__mul__ (other)

    def __rtruediv__ (self, other):
        return SimpleMeasurement (other / self._centralValue, (abs (other) * self._uncertaintyDown) / (self._centralValue * self._centralValue), (abs (other) * self._uncertaintyUp) / (self._centralValue * self._centralValue))

    def __rdiv__ (self, other):
        return self.__rtruediv__ (other)

    ############################################################################
    # Methods to overload those in numpy.
    ############################################################################

    def sqrt (self):
        return self.__pow__ (0.5)

    def exp (self):
        return SimpleMeasurement (math.exp (self._centralValue), self._uncertaintyDown * math.exp (self._centralValue), self._uncertaintyUp * math.exp (self._centralValue))

    def log (self):
        return SimpleMeasurement (math.log (self._centralValue), self._uncertaintyDown / abs (self._centralValue), self._uncertaintyUp / abs (self._centralValue))

    def log10 (self):
        return self.log () / math.log (10.0)

    def fabs (self):
        return self.__abs__ ()

    ############################################################################

# a Measurement is a SimpleMeasurement an additional systematic uncertainty (possibly asymmetric)
class Measurement (SimpleMeasurement):
    _systematicDown = SimpleMeasurement._nan
    _systematicUp = SimpleMeasurement._nan

    def __init__ (self, centralValue = None, uncertaintyDown = None, uncertaintyUp = None, systematicDown = None, systematicUp = None):
        SimpleMeasurement.__init__ (self, centralValue, uncertaintyDown, uncertaintyUp)
        if systematicDown is not None:
            self._systematicDown = float (systematicDown)
        if systematicUp is not None:
            self._systematicUp = float (systematicUp)
        if systematicDown is not None and systematicUp is None:
            self._systematicUp = float (systematicDown)
        if systematicUp is not None and systematicDown is None:
            self._systematicDown = float (systematicUp)

    ############################################################################
    # Setters.
    ############################################################################

    def setSystematic (self, systematic):
        self._systematicDown = float (systematic)
        self._systematicUp = float (systematic)

    def setSystematic (self, systematicDown, systematicUp):
        self._systematicDown = float (systematicDown)
        self._systematicUp = float (systematicUp)

    def setSystematicDown (self, systematicDown):
        self._systematicDown = float (systematicDown)

    def setSystematicUp (self, systematicUp):
        self._systematicUp = float (systematicUp)

    def setCentralValueAndSystematic (self, centralValue, systematic):
        self._centralValue = float (centralValue)
        self._systematicDown = float (systematic)
        self._systematicUp = float (systematic)

    def setCentralValueAndSystematic (self, centralValue, systematicDown, systematicUp):
        self._centralValue = float (centralValue)
        self._systematicDown = float (systematicDown)
        self._systematicUp = float (systematicUp)

    def isPositive (self, isPositive = True):
        SimpleMeasurement.isPositive (self, isPositive)
        if self._centralValue - self._systematicDown < 0.0:
            self._systematicDown = self._centralValue

    ############################################################################
    # Getters.
    ############################################################################

    def systematic (self):
        if self._systematicDown == self._systematicUp:
            return self._systematicDown
        else:
            return (self._systematicDown, self._systematicUp)

    def systematicDown (self):
        return self._systematicDown

    def systematicUp (self):
        return self._systematicUp

    def minSystematic (self):
        return min (self._systematicDown, self._systematicUp)

    def maxSystematic (self):
        return max (self._systematicDown, self._systematicUp)

    def centralValueAndSystematic (self):
        if self._systematicDown == self._systematicUp:
            return (self._centralValue, self._systematicDown)
        else:
            return (self._centralValue, self._systematicDown, self._systematicUp)

    def centralValueAndSystematicDownAndUp (self):
        return (self._centralValue, self._systematicDown, self._systematicUp)

    def centralValueAndUncertaintyDownAndUpAndSystematicDownAndUp (self):
        return (self._centralValue, self._uncertaintyDown, self._uncertaintyUp, self._systematicDown, self._systematicUp)

    ############################################################################
    # Miscellaneous.
    ############################################################################

    def roundAccordingToUncertainty (self, uncertainty):
        x = SimpleMeasurement.roundAccordingToUncertainty (self, uncertainty)
        if math.isnan (self._systematicDown) and math.isnan (self._systematicUp):
            return x

        exponent = (int (math.floor (math.log10 (uncertainty))) if uncertainty != 0.0 else -1)

        systematicDown = round (self._systematicDown, self._maxSigFigsInUncertainty - 1 - exponent)
        systematicUp = round (self._systematicUp, self._maxSigFigsInUncertainty - 1 - exponent)

        return x + (systematicDown, systematicUp)

    ############################################################################
    # Methods to overload standard operators are defined below.
    ############################################################################

    def __str__ (self):
        if math.isnan (self._systematicDown) and math.isnan (self._systematicUp):
            return SimpleMeasurement.__str__ (self)

        if self._printLongFormat:
            (centralValue, uncertaintyDown, uncertaintyUp, systematicDown, systematicUp) = self.centralValueAndUncertaintyDownAndUpAndSystematicDownAndUp ()
        else:
            (centralValue, uncertaintyDown, uncertaintyUp, systematicDown, systematicUp) = self.roundAccordingToUncertainty (max (self.maxUncertainty (), self.maxSystematic ()))

        s = ""
        if not self._printTeX:
            if uncertaintyDown == uncertaintyUp:
                s += str (centralValue) + " +- " + str (uncertaintyDown)
            else:
                s += str (centralValue) + " (- " + str (uncertaintyDown) + " + " + str (uncertaintyUp) + ")"
            if systematicDown == systematicUp:
                s += " +- " + str (systematicDown)
            else:
                s += " (- " + str (systematicDown) + " + " + str (systematicUp) + ")"
        else:
            if uncertaintyDown == uncertaintyUp:
                s += str (centralValue) + " \pm " + str (uncertaintyDown)
            else:
                s += str (centralValue) + " {}_{-" + str (uncertaintyDown) + "}^{+" + str (uncertaintyUp) + "}"
            if systematicDown == systematicUp:
                s += " \pm " + str (systematicDown)
            else:
                s += " {}_{-" + str (systematicDown) + "}^{+" + str (systematicUp) + "}"
        return s

    def __len__ (self):
        n = SimpleMeasurement (self)
        if self._systematicDown == self._systematicUp:
            return (n + 2)
        else:
            return (n + 3)

    def __neg__ (self):
        x = SimpleMeasurement.__neg__ (self)
        x.__class__ = Measurement
        x.setSystematic (self._systematicDown, self._systematicUp)
        return x

    def __pos__ (self):
        x = SimpleMeasurement.__pos__ (self)
        x.__class__ = Measurement
        x.setSystematic (self._systematicDown, self._systematicUp)
        return x

    def __abs__ (self):
        x = SimpleMeasurement.__abs__ (self)
        x.__class__ = Measurement
        x.setSystematic (self._systematicDown, self._systematicUp)
        return x

    def __add__ (self, other):
        x = SimpleMeasurement.__add__ (self, other)
        x.__class__ = Measurement
        if hasattr (other, "centralValue"):
            x.setSystematic (math.hypot (self._systematicDown, other.systematicDown ()), math.hypot (self._systematicUp, other.systematicUp ()))
        else:
            x.setSystematic (self._systematicDown, self._systematicUp)
        return x

    def __mul__ (self, other):
        x = SimpleMeasurement.__mul__ (self, other)
        x.__class__ = Measurement
        if hasattr (other, "centralValue"):
            x.setSystematic (math.hypot (self._systematicDown * other.centralValue (), self._centralValue * other.systematicDown ()), math.hypot (self._systematicUp * other.centralValue (), self._centralValue * other.systematicUp ()))
        else:
            x.setSystematic (self._systematicDown * float (other), self._systematicUp * float (other))
        return x

    def __truediv__ (self, other):
        x = SimpleMeasurement.__truediv__ (self, other)
        x.__class__ = Measurement
        if hasattr (other, "centralValue"):
            x.setSystematic (math.hypot (self._systematicDown * other.centralValue (), self._centralValue * other.systematicDown ()) / (other.centralValue () * other.centralValue ()), math.hypot (self._systematicUp * other.centralValue (), self._centralValue * other.systematicUp ()) / (other.centralValue () * other.centralValue ()))
        else:
            x.setSystematic (self._systematicDown / float (other), self._systematicUp / float (other))
        return x

    def __pow__ (self, other):
        x = SimpleMeasurement.__pow__ (self, other)
        x.__class__ = Measurement
        x.setSystematic (abs (other) * self._systematicDown * pow (self._centralValue, other - 1.0), abs (other) * self._systematicUp * pow (self._centralValue, other - 1.0))
        return x

    def __rtruediv__ (self, other):
        x = SimpleMeasurement.__rtruediv__ (self, other)
        x.__class__ = Measurement
        x.setSystematic ((abs (other) * self._systematicDown) / (self._centralValue * self._centralValue), (abs (other) * self._systematicUp) / (self._centralValue * self._centralValue))
        return x

    ############################################################################
    # Methods to overload those in numpy.
    ############################################################################

    def exp (self):
        x = SimpleMeasurement.exp (self)
        x.__class__ = Measurement
        x.setSystematic (self._systematicDown * math.exp (self._centralValue), self._systematicUp * math.exp (self._centralValue))
        return x

    def log (self):
        x = SimpleMeasurement.log (self)
        x.__class__ = Measurement
        x.setSystematic (self._systematicDown / abs (self._centralValue), self._systematicUp / abs (self._centralValue))
        return x

    ############################################################################
