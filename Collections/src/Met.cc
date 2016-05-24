#include "TVector2.h"

#include "OSUT3Analysis/Collections/interface/Met.h"

#if IS_VALID(mets)

osu::Met::Met ()
{
}

osu::Met::Met (const TYPE(mets) &met) :
  TYPE(mets) (met),
  noMuPt_ (INVALID_VALUE),
  noMuPx_ (INVALID_VALUE),
  noMuPy_ (INVALID_VALUE),
  noMuPhi_ (INVALID_VALUE)
{
}

osu::Met::Met (const TYPE(mets) &met, const edm::Handle<vector<pat::PackedCandidate> > &pfCandidates) :
  TYPE(mets) (met),
  noMuPt_ (INVALID_VALUE),
  noMuPx_ (INVALID_VALUE),
  noMuPy_ (INVALID_VALUE),
  noMuPhi_ (INVALID_VALUE)
{
  if (pfCandidates.isValid ())
    {
      TVector2 metNoMu (met.px (), met.py ());
      for (const auto &pfCandidate : *pfCandidates)
        {
          if (abs (pfCandidate.pdgId ()) != 13)
            continue;

          TVector2 muon (pfCandidate.px (), pfCandidate.py ());
          metNoMu += muon;
        }

      noMuPt_ = metNoMu.Mod ();
      noMuPx_ = metNoMu.Px ();
      noMuPy_ = metNoMu.Py ();
      noMuPhi_ = metNoMu.Phi ();
    }
}

osu::Met::~Met ()
{
}

const double
osu::Met::noMuPt () const
{
  return noMuPt_;
}

const double
osu::Met::noMuPx () const
{
  return noMuPx_;
}

const double
osu::Met::noMuPy () const
{
  return noMuPy_;
}

const double
osu::Met::noMuPhi () const
{
  return noMuPhi_;
}

#endif
