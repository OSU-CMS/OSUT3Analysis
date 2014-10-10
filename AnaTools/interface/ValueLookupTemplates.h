#ifndef VALUE_LOOKUP_TEMPLATES
#define VALUE_LOOKUP_TEMPLATES

////////////////////////////////////////////////////////////////////////////////
// This file contains the definitions of the template methods in the ValueLookup
// class. These definitions need to be included by any file which calls these
// functions so that the compiler knows which instances of the methods to
// create.
////////////////////////////////////////////////////////////////////////////////

template<typename T0, typename T1> const T1 *
ValueLookup::closest (const T0 *object0, const edm::Handle<vector<T1> > &object1Collection, const string &name)
{
  const T1 *closestObject1 = 0;
  double smallestDeltaR;
  if(cumulativeFlags.find (name) != cumulativeFlags.end ()){
    flagPair flags;
    for (int i = cumulativeFlags.at(name).size() - 1; i >= 0; i--){
      if (cumulativeFlags.at(name).at(i).size()){
        flags = cumulativeFlags.at(name).at(i);
        break;
      }
    }
    for (uint index = 0; index != flags.size(); index++){
      if(!flags.at(index).first) continue;
      const T1 *object1 = &object1Collection->at(index);
      double dR;
      if(!closestObject1 || (dR = reco::deltaR (object0->eta, object0->phi, object1->eta, object1->phi)) < smallestDeltaR){
        closestObject1 = object1;
        smallestDeltaR = dR;
      }
    }
  }
  else if (find (objectsToGet.begin (), objectsToGet.end (), name) != objectsToGet.end ())
    closestObject1 = & object1Collection->at(0);

  return closestObject1;
}


// Returns the smallest DeltaR between the object and any generated true particle in the event.
template <class InputObject>
double ValueLookup::getGenDeltaRLowest(InputObject object){
  double genDeltaRLowest = 999.;
  for(BNmcparticleCollection::const_iterator mcparticle = mcparticles->begin (); mcparticle != mcparticles->end (); mcparticle++){
    double deltaRtemp = deltaR(mcparticle->eta, mcparticle->phi, object->eta, object->phi);
    if (deltaRtemp < genDeltaRLowest) genDeltaRLowest = deltaRtemp;
  }
  return genDeltaRLowest;
}


template <class InputObject>
int ValueLookup::getGenMatchedParticleIndex(InputObject object){

  int bestMatchIndex = -1;
  double bestMatchDeltaR = 999;

  for(BNmcparticleCollection::const_iterator mcparticle = mcparticles->begin (); mcparticle != mcparticles->end (); mcparticle++){

    double currentDeltaR = deltaR(object->eta,object->phi,mcparticle->eta,mcparticle->phi);
    if(currentDeltaR > 0.05) continue;
    //     clog << setprecision(3) << setw(20)
    //          << "\tcurrentParticle:  eta = " << mcparticles->at(mcparticle - mcparticles->begin()).eta
    //          << setw(20)
    //          << "\tphi = " << mcparticles->at(mcparticle - mcparticles->begin()).phi
    //          << setw(20)
    //          << "\tdeltaR = " << currentDeltaR
    //          << setprecision(1)
    //          << setw(20)
    //          << "\tid = " << mcparticles->at(mcparticle - mcparticles->begin()).id
    //          << setw(20)
    //          << "\tmotherId = " << mcparticles->at(mcparticle - mcparticles->begin()).motherId
    //          << setw(20)
    //          << "\tstatus = " << mcparticles->at(mcparticle - mcparticles->begin()).status<< endl;
    if(currentDeltaR < bestMatchDeltaR && mcparticles->at(mcparticle - mcparticles->begin()).id != mcparticles->at(mcparticle - mcparticles->begin()).motherId){
      bestMatchIndex = mcparticle - mcparticles->begin();
      bestMatchDeltaR = currentDeltaR;
    }

  }
  //   if(bestMatchDeltaR != 999)  clog << "bestMatch:  deltaR = " << bestMatchDeltaR << "   id = " << mcparticles->at(bestMatchIndex).id << "   motherId = " << mcparticles->at(bestMatchIndex).motherId << endl;
  //   else clog << "no match found..." << endl;
  return bestMatchIndex;

}

#endif
