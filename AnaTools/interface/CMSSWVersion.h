#ifndef CMSSWVERSION_H
#define CMSSWVERSION_H

// CMSSW_X_Y_Z
// [:16]  X
// [15:8] Y
// [7:0]  Z
// Currently ignores patch numbers, and regards unusual releases (IB's etc) as X_Y_0

#define CMSSW_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
#define CMSSW_VERSION_CODE 787463

#if CMSSW_VERSION_CODE < CMSSW_VERSION(7,0,0)
  #error "Only CMSSW_7_0_0 and higher are supported. If this is already the case, you must run setupFramework.py before compiling."
#endif

#endif
