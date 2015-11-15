#ifndef OSU_GEN_MATCHABLE
#define OSU_GEN_MATCHABLE

#include "DataFormats/Common/interface/Handle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "OSUT3Analysis/Collections/interface/Mcparticle.h"

namespace osu
{
  template<class T, int PdgId>
  class GenMatchable : public T
    {
      public:
        struct GenMatchedParticle
          {
            ////////////////////////////////////////////////////////////////////
            // Names taken from pat::PackedGenParticle
            // https://github.com/cms-sw/cmssw/blob/CMSSW_8_0_X/DataFormats/PatCandidates/interface/PackedGenParticle.h#L307-L336
            ////////////////////////////////////////////////////////////////////

            //is particle prompt (not from hadron, muon, or tau decay) and final state
            edm::Ref<vector<osu::Mcparticle> > promptFinalState;

            //this particle is a direct decay product of a prompt tau and is final state
            //(eg an electron or muon from a leptonic decay of a prompt tau)
            edm::Ref<vector<osu::Mcparticle> > directPromptTauDecayProductFinalState;

            //this particle is the final state direct descendant of a hard process particle
            edm::Ref<vector<osu::Mcparticle> > hardProcessFinalState;

            //this particle is a direct decay product of a hardprocess tau and is final state
            //(eg an electron or muon from a leptonic decay of a tau from the hard process)
            edm::Ref<vector<osu::Mcparticle> > directHardProcessTauDecayProductFinalState;

            ////////////////////////////////////////////////////////////////////
          };
        struct DRToGenMatchedParticle
          {
            double promptFinalState;
            double directPromptTauDecayProductFinalState;
            double hardProcessFinalState;
            double directHardProcessTauDecayProductFinalState;

            DRToGenMatchedParticle () :
              promptFinalState (numeric_limits<int>::min ()),
              directPromptTauDecayProductFinalState (numeric_limits<int>::min ()),
              hardProcessFinalState (numeric_limits<int>::min ()),
              directHardProcessTauDecayProductFinalState (numeric_limits<int>::min ())
            {
            }
          };

        GenMatchable ();
        GenMatchable (const T &);
        GenMatchable (const T &, const edm::Handle<vector<osu::Mcparticle> > &);
        GenMatchable (const T &, const edm::Handle<vector<osu::Mcparticle> > &, const edm::ParameterSet &);
        ~GenMatchable ();

        const GenMatchedParticle genMatchedParticle () const;
        const DRToGenMatchedParticle dRToGenMatchedParticle () const;

        const GenMatchedParticle genMatchedParticleOfSameType () const;
        const DRToGenMatchedParticle dRToGenMatchedParticleOfSameType () const;

      private:
        GenMatchedParticle genMatchedParticle_;
        DRToGenMatchedParticle dRToGenMatchedParticle_;

        GenMatchedParticle genMatchedParticleOfSameType_;
        DRToGenMatchedParticle dRToGenMatchedParticleOfSameType_;

        double maxDeltaR_;

        const GenMatchedParticle findGenMatchedParticle (const edm::Handle<vector<osu::Mcparticle> > &, GenMatchedParticle &, DRToGenMatchedParticle &, const bool = false);
    };
}

template<class T, int PdgId>
osu::GenMatchable<T, PdgId>::GenMatchable () :
  genMatchedParticle_ (),
  dRToGenMatchedParticle_ (),
  genMatchedParticleOfSameType_ (),
  dRToGenMatchedParticleOfSameType_ (),
  maxDeltaR_ (-1.0)
{
}

template<class T, int PdgId>
osu::GenMatchable<T, PdgId>::GenMatchable (const T &object) :
  T (object),
  genMatchedParticle_ (),
  dRToGenMatchedParticle_ (),
  genMatchedParticleOfSameType_ (),
  dRToGenMatchedParticleOfSameType_ (),
  maxDeltaR_ (-1.0)
{
}

template<class T, int PdgId>
osu::GenMatchable<T, PdgId>::GenMatchable (const T &object, const edm::Handle<vector<osu::Mcparticle> > &particles) :
  GenMatchable<T, PdgId> (object)
{
  if (particles.isValid ())
    {
      findGenMatchedParticle (particles, genMatchedParticle_, dRToGenMatchedParticle_);
      findGenMatchedParticle (particles, genMatchedParticleOfSameType_, dRToGenMatchedParticleOfSameType_, true);
    }
}

template<class T, int PdgId>
osu::GenMatchable<T, PdgId>::GenMatchable (const T &object, const edm::Handle<vector<osu::Mcparticle> > &particles, const edm::ParameterSet &cfg) :
  GenMatchable<T, PdgId> (object)
{
  maxDeltaR_ = cfg.getParameter<double> ("maxDeltaRForGenMatching");
  if (particles.isValid ())
    {
      findGenMatchedParticle (particles, genMatchedParticle_, dRToGenMatchedParticle_);
      findGenMatchedParticle (particles, genMatchedParticleOfSameType_, dRToGenMatchedParticleOfSameType_, true);
    }
}

template<class T, int PdgId>
osu::GenMatchable<T, PdgId>::~GenMatchable ()
{
}

template<class T, int PdgId> const typename osu::GenMatchable<T, PdgId>::GenMatchedParticle
osu::GenMatchable<T, PdgId>::findGenMatchedParticle (const edm::Handle<vector<osu::Mcparticle> > &particles, osu::GenMatchable<T, PdgId>::GenMatchedParticle &genMatchedParticle, osu::GenMatchable<T, PdgId>::DRToGenMatchedParticle &dRToGenMatchedParticle, const bool usePdgId)
{
  dRToGenMatchedParticle.promptFinalState = numeric_limits<int>::min ();
  dRToGenMatchedParticle.directPromptTauDecayProductFinalState = numeric_limits<int>::min ();
  dRToGenMatchedParticle.hardProcessFinalState = numeric_limits<int>::min ();
  dRToGenMatchedParticle.directHardProcessTauDecayProductFinalState = numeric_limits<int>::min ();
  for (vector<osu::Mcparticle>::const_iterator particle = particles->begin (); particle != particles->end (); particle++)
    {
      int pdgId = 0;
#if DATA_FORMAT == MINI_AOD || DATA_FORMAT == AOD || DATA_FORMAT == MINI_AOD_CUSTOM
      pdgId = particle->pdgId ();
#endif
      if (usePdgId && abs (pdgId) != PdgId)
        continue;

      double dR = deltaR (*particle, *this);
      if (maxDeltaR_ >= 0.0 && dR > maxDeltaR_)
        continue;

      if (particle->isPromptFinalState ())
        {
          if (dR < dRToGenMatchedParticle.promptFinalState || dRToGenMatchedParticle.promptFinalState < 0.0)
            {
              dRToGenMatchedParticle.promptFinalState = dR;
              genMatchedParticle.promptFinalState = edm::Ref<vector<osu::Mcparticle> > (particles, particle - particles->begin ());
            }
        }
      if (particle->isDirectPromptTauDecayProductFinalState ())
        {
          if (dR < dRToGenMatchedParticle.directPromptTauDecayProductFinalState || dRToGenMatchedParticle.directPromptTauDecayProductFinalState < 0.0)
            {
              dRToGenMatchedParticle.directPromptTauDecayProductFinalState = dR;
              genMatchedParticle.directPromptTauDecayProductFinalState = edm::Ref<vector<osu::Mcparticle> > (particles, particle - particles->begin ());
            }
        }
      if (particle->fromHardProcessFinalState ())
        {
          if (dR < dRToGenMatchedParticle.hardProcessFinalState || dRToGenMatchedParticle.hardProcessFinalState < 0.0)
            {
              dRToGenMatchedParticle.hardProcessFinalState = dR;
              genMatchedParticle.hardProcessFinalState = edm::Ref<vector<osu::Mcparticle> > (particles, particle - particles->begin ());
            }
        }
      if (particle->isDirectHardProcessTauDecayProductFinalState ())
        {
          if (dR < dRToGenMatchedParticle.directHardProcessTauDecayProductFinalState || dRToGenMatchedParticle.directHardProcessTauDecayProductFinalState < 0.0)
            {
              dRToGenMatchedParticle.directHardProcessTauDecayProductFinalState = dR;
              genMatchedParticle.directHardProcessTauDecayProductFinalState = edm::Ref<vector<osu::Mcparticle> > (particles, particle - particles->begin ());
            }
        }
    }

  return genMatchedParticle;
}

template<class T, int PdgId> const typename osu::GenMatchable<T, PdgId>::GenMatchedParticle
osu::GenMatchable<T, PdgId>::genMatchedParticle () const
{
  return genMatchedParticle_;
}

template<class T, int PdgId> const typename osu::GenMatchable<T, PdgId>::DRToGenMatchedParticle
osu::GenMatchable<T, PdgId>::dRToGenMatchedParticle () const
{
  return dRToGenMatchedParticle_;
}

template<class T, int PdgId> const typename osu::GenMatchable<T, PdgId>::GenMatchedParticle
osu::GenMatchable<T, PdgId>::genMatchedParticleOfSameType () const
{
  return genMatchedParticleOfSameType_;
}

template<class T, int PdgId> const typename osu::GenMatchable<T, PdgId>::DRToGenMatchedParticle
osu::GenMatchable<T, PdgId>::dRToGenMatchedParticleOfSameType () const
{
  return dRToGenMatchedParticleOfSameType_;
}

#endif
