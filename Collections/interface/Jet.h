#ifndef OSU_JET
#define OSU_JET

#include "OSUT3Analysis/Collections/interface/GenMatchable.h"

namespace osu
{

#if IS_VALID(jets)
#ifndef STOPPPED_PTLS
    class Jet : public GenMatchable<TYPE(jets), 0>
    {
      public:
        Jet ();
        Jet (const TYPE(jets) &);
        Jet (const TYPE(jets) &, const edm::Handle<vector<osu::Mcparticle> > &);
        Jet (const TYPE(jets) &, const edm::Handle<vector<osu::Mcparticle> > &, const edm::ParameterSet &);
        ~Jet ();
        const int matchedToLepton () const;
        const float pfCombinedSecondaryVertexV2BJetTags () const;
        const float pfCombinedInclusiveSecondaryVertexV2BJetTags () const;
        // const float pileupJetId () const; // Not available in Puppi jets

        const float smearedPt () const;
        const float smearedPtJesSystUp () const;
        const float smearedPtJesSystDown () const;
        const float smearedPtJerSystUp () const;
        const float smearedPtJerSystDown () const;

        const float alphamax () const;
        const float ipsig () const;
        const float log10ipsig () const;
        const float medianlog10ipsig () const;

        void set_matchedToLepton (float value) { matchedToLepton_  = value; }
        void set_pfCombinedSecondaryVertexV2BJetTags (float value) { pfCombinedSecondaryVertexV2BJetTags_ = value;}
        void set_pfCombinedInclusiveSecondaryVertexV2BJetTags (float value) { pfCombinedInclusiveSecondaryVertexV2BJetTags_ = value;}
        // void set_pileupJetId (float value) { pileupJetId_ = value;} // Not available in Puppi jets

        void set_smearedPt (float value) { smearedPt_ = value;}
        void set_smearedPtJesSystUp (float value) { smearedPtJesSystUp_ = value;}
        void set_smearedPtJesSystDown (float value) { smearedPtJesSystDown_ = value;}
        void set_smearedPtJerSystUp (float value) { smearedPtJerSystUp_ = value;}
        void set_smearedPtJerSystDown (float value) { smearedPtJerSystDown_ = value;}

        void set_alphamax (float value) { alphamax_ = value;}
        void set_ipsig (float value) { ipsig_ = value;}
        void set_log10ipsig (float value) { log10ipsig_ = value;}
        void set_medianlog10ipsig (float value) { medianlog10ipsig_ = value;}

      private:
        int matchedToLepton_;
        float pfCombinedSecondaryVertexV2BJetTags_;
        float pfCombinedInclusiveSecondaryVertexV2BJetTags_;
        // float pileupJetId_; // Not available in Puppi jets
        float smearedPt_;
        float smearedPtJesSystUp_;
        float smearedPtJesSystDown_;
        float smearedPtJerSystUp_;
        float smearedPtJerSystDown_;
        float alphamax_;
        float ipsig_;
        float log10ipsig_;
        float medianlog10ipsig_;
    };
#else // if STOPPPED_PTLS
    class Jet : public TYPE(jets)
    {
      public:
        Jet ();
        Jet (const TYPE(jets) &);
        ~Jet ();
    };
#endif // if STOPPPED_PTLS
#else // IS_VALID(jets)
    typedef TYPE(jets) Jet;
#endif // IS_VALID(jets)

    //////////////////////////////
    // osu::Bjet
    //////////////////////////////
#if IS_VALID(jets) && IS_VALID(bjets)
#ifndef STOPPPED_PTLS
    class Bjet : public Jet
    {
      public:
        Bjet();
        Bjet(const TYPE(jets) &);
        Bjet(const TYPE(jets) &, const edm::Handle<vector<osu::Mcparticle> > &);
        Bjet(const TYPE(jets) &, const edm::Handle<vector<osu::Mcparticle> > &, const edm::ParameterSet &);
        ~Bjet();
    };
#else // STOPPPED_PTLS
    class Bjet : public Jet
    {
        Bjet();
        BJet(const TYPE(jets) &);
        ~Bjet();
    };
#endif // STOPPPED_PTLS
#else // IS_VALID(jets) && IS_VALID(bjets)
    typedef TYPE(bjets) Bjet;
#endif // IS_VALID(jets) && IS_VALID(bjets)

} // namespace osu

#endif // ifndef OSU_JET
