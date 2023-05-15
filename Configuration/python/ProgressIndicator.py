#!/usr/bin/env python3

import sys, time
import OSUT3Analysis.Configuration.Color

# define some macros with meaningful names for the ANSI control sequences
# https://en.wikipedia.org/wiki/ANSI_escape_code#CSI_sequences

A_UP              =  "\033[{n}A"
A_DOWN            =  "\033[{n}B"
A_RIGHT           =  "\033[{n}C"
A_LEFT            =  "\033[{n}D"

A_COLUMN          =  (lambda n : "\033[" + str (n) + "G")

class ProgressIndicator:
  _title = ""
  _percentDone = 0
  _isPrinted = False
  _percentDonePadding = 6
  _timeOfLastPrint = 0.0
  _minTimeIntervalForPrinting = 0.5

  def __init__ (self, title = "", percentDone = 0):
    self._title = title
    self._percentDone = percentDone

  def printProgress (self, finished = False):
    if (time.time () - self._timeOfLastPrint) > self._minTimeIntervalForPrinting or finished:
      if not self._isPrinted:
        print(self._title, end=' ')
        if self._title:
          print(": ", end=' ')

        self._isPrinted = True
      else:
        titleLength = len (self._title) + 2
        if self._title:
          titleLength += 2
        print(A_COLUMN (titleLength), end=' ')

      print(str ("%.2f" % round (self._percentDone, 2)).rjust (self._percentDonePadding) + "%", end=' ')
      if finished:
        print("")

      sys.stdout.flush ()
      self._timeOfLastPrint = time.time ()

  def setPercentDone (self, percentDone):
    self._percentDone = percentDone

  def setTitle (self, title):
    self._title = title

  def __iadd__ (self, other):
    self._percentDone += other

  def __isub__ (self, other):
    self._percentDone -= other

  def __eq__ (self, other):
    return (self._percentDone == other)

  def __ne__ (self, other):
    return (not self == other)

  def __lt__ (self, other):
    return (self._percentDone < other)

  def __le__ (self, other):
    return (self < other or self == other)

  def __gt__ (self, other):
    return (not self <= other)

  def __ge__ (self, other):
    return (not self < other)
