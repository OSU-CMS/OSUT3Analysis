#include <iostream>

#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookup.h"

//!trigobj valueLookup
double
ValueLookup::valueLookup (const BNtrigobj &object, string variable){

  double value = numeric_limits<int>::min ();
  BNtrigobj *obj = new BNtrigobj (object);

  try
    {
      value = getMember ("BNtrigobj", obj, variable);
    }
  catch (...)
    {
      if(variable == "ptWithForwardJets") value = (object.pt + getSumForwardJetPt(jets.product()));
      else if(variable == "metPt") {  // allow making 2D plots of jet quantities vs. Met
        if (const BNmet *met = chosenMET ()) {
          value = met->pt;
        } else value = numeric_limits<int>::min ();
      }
      else if(variable == "trackPt") {  // allow making 2D plots of jet quantities vs. Met
        if (const BNtrack *track = chosenTrack ()) {
          value = track->pt;
        } else value = numeric_limits<int>::min ();
      }
      else if(variable == "jetPt") {  // allow making 2D plots of jet quantities vs. Met
        if (const BNjet *jet = chosenJet ()) {
          value = jet->pt;
        } else value = numeric_limits<int>::min ();
      }
      /*else if(variable == "filter") {
        if ((stringValue = object.filter) == "")
          stringValue = "none";  // stringValue should only be empty if value is filled
      }*/

      else
        clog << "WARNING: invalid trigobj variable '" << variable << "'\n";
    }

  delete obj;
  return value;
}
