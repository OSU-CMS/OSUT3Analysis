#ifndef DATA_FORMAT

// DATA_FORMAT format
// bit 0: datatier flag (0=AOD, 1=MINIAOD)
// bit 1: 2017 flag (0=not 2017, 1=2017 -- only relevant for MINIAOD in 2017)
// bit 2: custom flag (0=not custom, 1=custom)

#define AOD                  0b0000 // 0
#define MINI_AOD             0b0001 // 1
#define AOD_2017             0b0010 // 2 (unused)
#define MINI_AOD_2017        0b0011 // 3
#define AOD_CUSTOM           0b0100 // 4
#define MINI_AOD_CUSTOM      0b0101 // 5
#define AOD_2017_CUSTOM      0b0110 // 6 (unused)
#define MINI_AOD_2017_CUSTOM 0b0111 // 7
#define AOD_2022             0b1000 // 8
#define MINI_AOD_2022        0b1001 // 9 
#define AOD_2022_CUSTOM      0b1100 // 12
#define MINI_AOD_2022_CUSTOM 0b1101 // 13

#define DATA_FORMAT MINI_AOD_2017_CUSTOM

#define DATA_FORMAT_FROM_MINIAOD (DATA_FORMAT & 0b1) == 1
#define DATA_FORMAT_FROM_AOD (DATA_FORMAT & 0b1) == 0
#define DATA_FORMAT_IS_2017 ((DATA_FORMAT >> 1) & 0b1) == 1
#define DATA_FORMAT_IS_CUSTOM ((DATA_FORMAT >> 2) & 0b1) == 1
#define DATA_FORMAT_IS_2022 ((DATA_FORMAT >> 3) & 0b1) == 1

#define DATA_FORMAT_IS_VALID (DATA_FORMAT == AOD || DATA_FORMAT == MINI_AOD || DATA_FORMAT == AOD_2017 || DATA_FORMAT == MINI_AOD_2017 || DATA_FORMAT == AOD_CUSTOM || DATA_FORMAT == MINI_AOD_CUSTOM || DATA_FORMAT == AOD_2017_CUSTOM || DATA_FORMAT == MINI_AOD_2017_CUSTOM || DATA_FORMAT == MINI_AOD_2022)

#define INVALID_TYPE void *

#define ORIGINAL_FORMAT "originalFormat"  // Must match definition used in processingUtilities.py

#if DATA_FORMAT == MINI_AOD

  #include "OSUT3Analysis/AnaTools/interface/DataFormatMiniAOD.h"

#elif DATA_FORMAT == MINI_AOD_2017

  #include "OSUT3Analysis/AnaTools/interface/DataFormatMiniAOD2017.h" 

#elif DATA_FORMAT == MINI_AOD_2022

  #include "OSUT3Analysis/AnaTools/interface/DataFormatMiniAOD2022.h"

#elif DATA_FORMAT == AOD

  #include "OSUT3Analysis/AnaTools/interface/DataFormatAOD.h"

#elif DATA_FORMAT_IS_CUSTOM

  #include "DisappTrks/StandardAnalysis/interface/CustomDataFormat.h"

#else
  #error "Data format is not valid."
#endif

#define XSTR(x) STR(x)
#define STR(x) #x
#define IS_VALID(x) (x##_INVALID != 1)
#define EQ_VALID(s,x) (strcmp (XSTR(x##_TYPE), XSTR(INVALID_TYPE)) && !strcmp (s.c_str (), XSTR(x)))

#define TYPE(x) x##_TYPE
#define TYPE_STR(x) XSTR(x##_TYPE)

#define INVALID_VALUE (numeric_limits<int>::min ())
#define IS_INVALID(x) (x <= INVALID_VALUE + 1)

// Return whether obj is contained in vec.
#define VEC_CONTAINS(vec, obj) (find (vec.begin (), vec.end (), obj) != vec.end ())

#include <map>
#include <string>
#include <vector>
#include "boost/config.hpp"
#include "RVersion.h"

#if ROOT_VERSION_CODE >= ROOT_VERSION(6,0,0)
  #define ROOT6
#elif ROOT_VERSION_CODE >= ROOT_VERSION(5,0,0)
  #define ROOT5
#else
  #error "Only ROOT 5 and 6 are supported."
#endif

#include "CMSSWVersion.h"

using namespace std;

// type to hold list of hashed indices corresponding to objects
// string - object type
// int - object hash
typedef multimap <string, int>  ObjectList;

// struct to connect objects to the calculated variable's value
struct UserVariable
{
  double value;
  ObjectList objects;

  UserVariable ()
    {
    }

  UserVariable (double val, const ObjectList &objs) :
    value (val),
    objects (objs)
    {
    }
};

// VariableProducerPayload type:
//   string - user-defined name of variable
//   vector - one UserVariable per combination of input objects
typedef map<string, vector<UserVariable> > VariableProducerPayload;

// EventVariableProducerPayload type:
//   string - user-defined name of variable
//   double - value of variable for the event
typedef map<string, double > EventVariableProducerPayload;

// define some macros with meaningful names for the ANSI color codes
// https://en.wikipedia.org/wiki/ANSI_escape_code#Colors

#define  A_BLACK    ("\033[30m")
#define  A_RED      ("\033[31m")
#define  A_GREEN    ("\033[32m")
#define  A_YELLOW   ("\033[33m")
#define  A_BLUE     ("\033[34m")
#define  A_MAGENTA  ("\033[35m")
#define  A_CYAN     ("\033[36m")
#define  A_WHITE    ("\033[37m")

#define  A_BRIGHT_BLACK    ("\033[1;30m")
#define  A_BRIGHT_RED      ("\033[1;31m")
#define  A_BRIGHT_GREEN    ("\033[1;32m")
#define  A_BRIGHT_YELLOW   ("\033[1;33m")
#define  A_BRIGHT_BLUE     ("\033[1;34m")
#define  A_BRIGHT_MAGENTA  ("\033[1;35m")
#define  A_BRIGHT_CYAN     ("\033[1;36m")
#define  A_BRIGHT_WHITE    ("\033[1;37m")

#define  A_RESET    ("\033[0m")

#if !DATA_FORMAT_IS_VALID
  #error "Data format is not valid. Check AnaTools/interface/DataFormat.h for allowed formats."
#endif

#endif
