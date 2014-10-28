#include <iostream>

#include "OSUT3Analysis/AnaTools/interface/ExternTemplates.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookup.h"

//!stop valueLookup
double
ValueLookup::valueLookup (const BNstop* object, string variable){


  double value = 0.0;

  if      (variable == "ctau") value = object->ctau;
  else if (variable == "pdgId") value = object->pdgId;
  else if (variable == "status") value = object->status;
  else if (variable == "pt") value = object->pt;
  else if (variable == "eta") value = object->eta;
  else if (variable == "phi") value = object->phi;
  else if (variable == "px") value = object->px;
  else if (variable == "py") value = object->py;
  else if (variable == "pz") value = object->pz;
  else if (variable == "energy") value = object->energy;
  else if (variable == "mass") value = object->mass;
  else if (variable == "beta") value = object->beta;
  else if (variable == "gamma") value = object->gamma;
  else if (variable == "betaAtDecay") value = object->betaAtDecay;
  else if (variable == "gammaAtDecay") value = object->gammaAtDecay;
  else if (variable == "vx") value = object->vx;
  else if (variable == "vy") value = object->vy;
  else if (variable == "vz") value = object->vz;
  else if (variable == "decayVx") value = object->decayVx;
  else if (variable == "decayVy") value = object->decayVy;
  else if (variable == "decayVz") value = object->decayVz;
  else if (variable == "decayLength")          value = object->decayLength;
  else if (variable == "daughter0Id")          value = object->daughter0Id;
  else if (variable == "daughter0Status")      value = object->daughter0Status;
  else if (variable == "daughter0PT")          value = object->daughter0PT;
  else if (variable == "daughter0Phi")         value = object->daughter0Phi;
  else if (variable == "daughter0Eta")         value = object->daughter0Eta;
  else if (variable == "daughter1Id")          value = object->daughter1Id;
  else if (variable == "daughter1Status")      value = object->daughter1Status;
  else if (variable == "daughter1PT")          value = object->daughter1PT;
  else if (variable == "daughter1Phi")         value = object->daughter1Phi;
  else if (variable == "daughter1Eta")         value = object->daughter1Eta;
  else if (variable == "charge")               value = object->charge;
  else if (variable == "threeCharge")          value = object->threeCharge;
  else if (variable == "daughter0Charge")      value = object->daughter0Charge;
  else if (variable == "daughter0ThreeCharge") value = object->daughter0ThreeCharge;
  else if (variable == "daughter1Charge")      value = object->daughter1Charge;
  else if (variable == "daughter1ThreeCharge") value = object->daughter1ThreeCharge;
  else if (variable == "rHadronCharge")        value = object->rHadronCharge;
  else if (variable == "rHadronPT")            value = object->rHadronPT;
  else if (variable == "rHadronEnergy")        value = object->rHadronEnergy;
  else if (variable == "rHadronMass")          value = object->rHadronMass;
  else if (variable == "rHadronPhi")           value = object->rHadronPhi;
  else if (variable == "rHadronEta")           value = object->rHadronEta;
  else if (variable == "rHadronBeta")          value = object->rHadronBeta;
  else if (variable == "rHadronGamma")         value = object->rHadronGamma;
  else if (variable == "rHadronId")            value = object->rHadronId;
  else if (variable == "rHadronStatus")        value = object->rHadronStatus;
  else if (variable == "rHadronThreeCharge")   value = object->rHadronThreeCharge;

  // derived quantities
  else if (variable == "betaGamma") value = object->beta * object->gamma;
  else if (variable == "vxy") value = sqrt(object->vx * object->vx + object->vy * object->vy);
  else if (variable == "decayVxy") value = sqrt(object->decayVx * object->decayVx + object->decayVy * object->decayVy);

  else if (variable == "d0"){
    double vx = object->vx - chosenVertex ()->x,
      vy = object->vy - chosenVertex ()->y,
      px = object->px,
      py = object->py,
      pt = object->pt;
    value = (-vx * py + vy * px) / pt;
  }

  else if (variable == "dz"){
    double vx = object->vx - chosenVertex ()->x,
      vy = object->vy - chosenVertex ()->y,
      vz = object->vz - chosenVertex ()->z,
      px = object->px,
      py = object->py,
      pz = object->pz,
      pt = object->pt;
    value = vz - (vx * px + vy * py)/pt * (pz/pt);
  }

  else if (variable == "minD0"){
    double minD0=999;
    for(BNprimaryvertexCollection::const_iterator vertex = primaryvertexs->begin (); vertex != primaryvertexs->end (); vertex++){
      double vx = object->vx - vertex->x,
        vy = object->vy - vertex->y,
        px = object->px,
        py = object->py,
        pt = object->pt;
      value = (-vx * py + vy * px) / pt;
      if(abs(value) < abs(minD0)) minD0 = value;
    }
    value = minD0;
  }
  else if (variable == "minDz"){
    double minDz=999;
    for(BNprimaryvertexCollection::const_iterator vertex = primaryvertexs->begin (); vertex != primaryvertexs->end (); vertex++){
      double vx = object->vx - vertex->x,
        vy = object->vy - vertex->y,
        vz = object->vz - vertex->z,
        px = object->px,
        py = object->py,
        pz = object->pz,
        pt = object->pt;
      value = vz - (vx * px + vy * py)/pt * (pz/pt);
      if(abs(value) < abs(minDz)) minDz = value;
    }
    value = minDz;
  }
  else if(variable == "distToVertex"){
    value = sqrt((object->vx-chosenVertex()->x)*(object->vx-chosenVertex()->x) + \
                 (object->vy-chosenVertex()->y)*(object->vy-chosenVertex()->y) + \
                 (object->vz-chosenVertex()->z)*(object->vz-chosenVertex()->z));
  }
  else if (variable == "minDistToVertex"){
    double minDistToVertex=999;
    for(BNprimaryvertexCollection::const_iterator vertex = primaryvertexs->begin (); vertex != primaryvertexs->end (); vertex++){
      value = sqrt((object->vx-vertex->x)*(object->vx-vertex->x) + \
                   (object->vy-vertex->y)*(object->vy-vertex->y) + \
                   (object->vz-vertex->z)*(object->vz-vertex->z));

      if(abs(value) < abs(minDistToVertex)) minDistToVertex = value;
    }
    value = minDistToVertex;
  }
  else if (variable == "distToVertexDifference"){
    double minDistToVertex=999;
    for(BNprimaryvertexCollection::const_iterator vertex = primaryvertexs->begin (); vertex != primaryvertexs->end (); vertex++){
      value = sqrt((object->vx-vertex->x)*(object->vx-vertex->x) + \
                   (object->vy-vertex->y)*(object->vy-vertex->y) + \
                   (object->vz-vertex->z)*(object->vz-vertex->z));

      if(abs(value) < abs(minDistToVertex)) minDistToVertex = value;
    }
    double distToChosenVertex = sqrt((object->vx-chosenVertex()->x)*(object->vx-chosenVertex()->x) + \
                                     (object->vy-chosenVertex()->y)*(object->vy-chosenVertex()->y) + \
                                     (object->vz-chosenVertex()->z)*(object->vz-chosenVertex()->z));

    value = distToChosenVertex - minDistToVertex;
  }

  else if (variable == "closestVertexRank"){
    double minDistToVertex=999;
    int vertex_rank = 0;
    for(BNprimaryvertexCollection::const_iterator vertex = primaryvertexs->begin (); vertex != primaryvertexs->end (); vertex++){
      vertex_rank++;
      int dist = sqrt((object->vx-vertex->x)*(object->vx-vertex->x) + \
                      (object->vy-vertex->y)*(object->vy-vertex->y) + \
                      (object->vz-vertex->z)*(object->vz-vertex->z));

      if(abs(dist) < abs(minDistToVertex)){
        value = vertex_rank;
        minDistToVertex = dist;
      }
    }
  }

  else if (variable == "decaysToTau"){
    value = abs (object->daughter0Id) == 15 || abs (object->daughter1Id) == 15;
  }




  else { clog << "WARNING: invalid stop variable '" << variable << "'\n"; value = -999; }

  return value;

} // end stop valueLookup
