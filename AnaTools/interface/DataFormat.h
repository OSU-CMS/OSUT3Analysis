#ifndef DATA_FORMAT

#define  BEAN             0
#define  MINI_AOD         1
#define  AOD              2
#define  MINI_AOD_CUSTOM  3

#define DATA_FORMAT MINI_AOD

#define INVALID_TYPE void *

#if DATA_FORMAT == BEAN

  #include "OSUT3Analysis/AnaTools/interface/DataFormatBEAN.h"

#elif DATA_FORMAT == MINI_AOD

  #include "OSUT3Analysis/AnaTools/interface/DataFormatMiniAOD.h"

#elif DATA_FORMAT == AOD

  #include "OSUT3Analysis/AnaTools/interface/DataFormatAOD.h"

#elif DATA_FORMAT == MINI_AOD_CUSTOM

  #include "CustomDataFormat.h"  // Define collections in an external header file.  

#else
  #error "Data format is not valid."
#endif

#define XSTR(x) STR(x)
#define STR(x) #x
#define IS_VALID(x) !(defined (x##_INVALID))
#define EQ_VALID(s,x) (string (XSTR(x##_TYPE)) != XSTR(INVALID_TYPE) && string (s) == XSTR(x))

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
  #warning "ROOT 6 is currently barely supported and many features may not work. Proceed at your own risk."
#elif ROOT_VERSION_CODE >= ROOT_VERSION(5,0,0)
  #define ROOT5
#else
  #error "Only ROOT 5 and 6 are supported."
#endif

#define SUPPORTED_VERSION "CMSSW_7_4_5_ROOT5"
#if !defined(BOOST_NO_CXX11_STATIC_ASSERT)
  static_assert (strcmp (PROJECT_VERSION, SUPPORTED_VERSION) == 0, SUPPORTED_VERSION " is the currently supported release. Please switch.");
#endif

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
};

// VariableProducerPayload type:
//   string - user-defined name of variable
//   vector - one UserVariable per combination of input objects
typedef map<string, vector<UserVariable> > VariableProducerPayload;

// EventVariableProducerPayload type:
//   string - user-defined name of variable
//   double - value of variable for the event
typedef map<string, double > EventVariableProducerPayload;

#endif
