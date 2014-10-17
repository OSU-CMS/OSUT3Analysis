#include <iostream>

#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookup.h"

//!trigobj valueLookup
double
ValueLookup::valueLookup (const BNtrigobj* object, string variable, string function, string &stringValue){

  double value = 0.0;

  if(variable == "pt") value = object->pt;
  else if(variable == "ptWithForwardJets") value = (object->pt + getSumForwardJetPt(jets.product()));
  else if(variable == "eta") value = object->eta;
  else if(variable == "phi") value = object->phi;
  else if(variable == "px") value = object->px;
  else if(variable == "py") value = object->py;
  else if(variable == "pz") value = object->pz;
  else if(variable == "et") value = object->et;
  else if(variable == "energy") value = object->energy;
  else if(variable == "etTotal") value = object->etTotal;
  else if(variable == "id") value = object->id;
  else if(variable == "charge") value = object->charge;
  else if(variable == "isIsolated") value = object->isIsolated;
  else if(variable == "isMip") value = object->isMip;
  else if(variable == "isForward") value = object->isForward;
  else if(variable == "isRPC") value = object->isRPC;
  else if(variable == "bx") value = object->bx;
  else if(variable == "metPt") {  // allow making 2D plots of jet quantities vs. Met
    if (const BNmet *met = chosenMET ()) {
      value = met->pt;
    } else value = -999;
  }
  else if(variable == "trackPt") {  // allow making 2D plots of jet quantities vs. Met
    if (const BNtrack *track = chosenTrack ()) {
      value = track->pt;
    } else value = -999;
  }
  else if(variable == "jetPt") {  // allow making 2D plots of jet quantities vs. Met
    if (const BNjet *jet = chosenJet ()) {
      value = jet->pt;
    } else value = -999;
  }
  else if(variable == "filter") {
    if ((stringValue = object->filter) == "")
      stringValue = "none";  // stringValue should only be empty if value is filled
  }


  else{clog << "WARNING: invalid trigobj variable '" << variable << "'\n"; value = -999;}

  value = applyFunction(function, value);

  return value;
}
