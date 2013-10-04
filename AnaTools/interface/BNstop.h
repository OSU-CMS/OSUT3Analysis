#ifndef ProductArea_BNstop_h
#define ProductArea_BNstop_h

#include <vector>
#include <string>

// a simple class
struct BNstop
{
  explicit BNstop(double v, int c):pt(v),eta(v),phi(v),px(v),py(v),pz(v),energy(v),mass(v),beta(v),gamma(v),vx(v),vy(v),vz(v),decayVx(v),decayVy(v),decayVz(v),decayLength(v),ctau(v),daughter0Id(v),daughter0Status(v),daughter0PT(v),daughter0Phi(v),daughter0Eta(v),daughter1Id(v),daughter1Status(v),daughter1PT(v),daughter1Phi(v),daughter1Eta(v),charge(c),threeCharge(c),daughter0Charge(v),daughter0ThreeCharge(v) { }
  BNstop():pt(-99),eta(-99),phi(-99),px(-99),py(-99),pz(-99),energy(-99),mass(-99),beta(-99),gamma(-99),vx(-99),vy(-99),vz(-99),decayVx(-99),decayVy(-99),decayVz(-99),decayLength(-99),ctau(-99),daughter0Id(-99),daughter0Status(-99),daughter0PT(-99),daughter0Phi(-99),daughter0Eta(-99),daughter1Id(-99),daughter1Status(-99),daughter1PT(-99),daughter1Phi(-99),daughter1Eta(-99),charge(-99),threeCharge(-99),daughter0Charge(-99),daughter0ThreeCharge(-99) { }
  double pt,eta,phi,px,py,pz,energy,mass,beta,gamma,vx,vy,vz,decayVx,decayVy,decayVz,decayLength,ctau,daughter0Id,daughter0Status,daughter0PT,daughter0Phi,daughter0Eta,daughter1Id,daughter1Status,daughter1PT,daughter1Phi,daughter1Eta;
  int    charge,threeCharge,daughter0Charge,daughter0ThreeCharge,daughter1Charge,daughter1ThreeCharge;

};

// this is our new product, it is simply a 
// collection of BNstop held in an std::vector
typedef std::vector<BNstop> BNstopCollection;

#endif
