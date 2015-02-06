#ifndef ProductArea_BNPFChgHad_h
#define ProductArea_BNPFChgHad_h

#include <vector>

// This class has been copied from BNtrack.h.
// Not all member values may be available for BNPFChgHad.

// a simple class
struct BNPFChgHad
{
  explicit BNPFChgHad(double v, int c):pt(v),px(v),py(v),pz(v),phi(v),eta(v),theta(v),normChi2(v),dZ(v),d0(v),d0err(v),vx(v),vy(v),vz(v),charge(c),numValidHits(c),caloEMDeltaRp3(v),caloHadDeltaRp3(v),caloEMDeltaRp5(v),caloHadDeltaRp5(v),energy(v),ecalEnergy(v),rawEcalEnergy(v),hcalEnergy(v),rawHcalEnergy(v) { }
BNPFChgHad():pt(-99),px(-99),py(-99),pz(-99),phi(-99),eta(-99),theta(-99),normChi2(-99),dZ(-99),d0(-99),d0err(-99),vx(-99),vy(-99),vz(-99),charge(-99),numValidHits(-99),caloEMDeltaRp3(-99),caloHadDeltaRp3(-99),caloEMDeltaRp5(-99),caloHadDeltaRp5(-99),energy(-99),ecalEnergy(-99),rawEcalEnergy(-99),hcalEnergy(-99),rawHcalEnergy(-99) { }
  double pt,px,py,pz,phi,eta,theta,normChi2,dZ,d0,d0err,vx,vy,vz;
  int    charge,numValidHits,isHighPurity;

  double caloEMDeltaRp3;
  double caloHadDeltaRp3;
  double caloEMDeltaRp5;
  double caloHadDeltaRp5;

  double energy;
  double ecalEnergy;
  double rawEcalEnergy;
  double hcalEnergy;
  double rawHcalEnergy;

  std::vector<int> subDetIdHits;
  std::vector<int> rawDetIdHits;
  std::vector<bool> isValidHits;
  std::vector<double> modulePerpHits;
  std::vector<double> moduleZHits;
  std::vector<double> modulePhiHits;

};

// this is our new product, it is simply a
// collection of BNPFChgHad held in an std::vector
typedef std::vector<BNPFChgHad> BNPFChgHadCollection;

#endif
