#!/usr/bin/env python

import sys, time

# define some macros with meaningful names for the ANSI color codes
# https://en.wikipedia.org/wiki/ANSI_escape_code#Colors

A_BLACK           =  "\033[30m"
A_RED             =  "\033[31m"
A_GREEN           =  "\033[32m"
A_YELLOW          =  "\033[33m"
A_BLUE            =  "\033[34m"
A_MAGENTA         =  "\033[35m"
A_CYAN            =  "\033[36m"
A_WHITE           =  "\033[37m"

A_BRIGHT_BLACK    =  "\033[1;30m"
A_BRIGHT_RED      =  "\033[1;31m"
A_BRIGHT_GREEN    =  "\033[1;32m"
A_BRIGHT_YELLOW   =  "\033[1;33m"
A_BRIGHT_BLUE     =  "\033[1;34m"
A_BRIGHT_MAGENTA  =  "\033[1;35m"
A_BRIGHT_CYAN     =  "\033[1;36m"
A_BRIGHT_WHITE    =  "\033[1;37m"

A_RESET           =  "\033[0m"

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
        print self._title,
        if self._title:
          print ": ",

        self._isPrinted = True
      else:
        titleLength = len (self._title) + 2
        if self._title:
          titleLength += 2
        print A_COLUMN (titleLength),

      print str ("%.2f" % round (self._percentDone, 2)).rjust (self._percentDonePadding) + "%",
      if finished:
        print ""

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
