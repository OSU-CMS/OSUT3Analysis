#include "DataFormats/Common/interface/Wrapper.h"

#include "OSUT3Analysis/Collections/interface/Basicjet.h"
#include "OSUT3Analysis/Collections/interface/Beamspot.h"
#include "OSUT3Analysis/Collections/interface/Bjet.h"
#include "OSUT3Analysis/Collections/interface/Bxlumi.h"
#include "OSUT3Analysis/Collections/interface/Electron.h"
#include "OSUT3Analysis/Collections/interface/Event.h"
#include "OSUT3Analysis/Collections/interface/Eventvariable.h"
#include "OSUT3Analysis/Collections/interface/Genjet.h"
#include "OSUT3Analysis/Collections/interface/Jet.h"
#include "OSUT3Analysis/Collections/interface/Mcparticle.h"
#include "OSUT3Analysis/Collections/interface/Met.h"
#include "OSUT3Analysis/Collections/interface/Muon.h"
#include "OSUT3Analysis/Collections/interface/Photon.h"
#include "OSUT3Analysis/Collections/interface/Primaryvertex.h"
#include "OSUT3Analysis/Collections/interface/Supercluster.h"
#include "OSUT3Analysis/Collections/interface/Tau.h"
#include "OSUT3Analysis/Collections/interface/Track.h"
#include "OSUT3Analysis/Collections/interface/Trigobj.h"
#include "OSUT3Analysis/Collections/interface/Uservariable.h"
#include "OSUT3Analysis/Collections/interface/Cschit.h"
#include "OSUT3Analysis/Collections/interface/Cscseg.h"
#include "OSUT3Analysis/Collections/interface/Dtseg.h"
#include "OSUT3Analysis/Collections/interface/Rpchit.h"

namespace {
  struct OSUT3Analysis_Collections {

//#if IS_VALID(basicjets)
    osu::Basicjet                             basicjet0;
    vector<osu::Basicjet>                     basicjet1;
    edm::Wrapper<osu::Basicjet>               basicjet2;
    edm::Wrapper<vector<osu::Basicjet> >      basicjet3;
    edm::Ref<vector<osu::Basicjet> >          basicjet4;
//#endif

//#if IS_VALID(beamspots)
    osu::Beamspot                             beamspot0;
    vector<osu::Beamspot>                     beamspot1;
    edm::Wrapper<osu::Beamspot>               beamspot2;
    edm::Wrapper<vector<osu::Beamspot> >      beamspot3;
    edm::Ref<vector<osu::Beamspot> >          beamspot4;
//#endif

//#if IS_VALID(bjets)
    osu::Bjet                                 bjet0;
    vector<osu::Bjet>                         bjet1;
    edm::Wrapper<osu::Bjet>                   bjet2;
    edm::Wrapper<vector<osu::Bjet> >          bjet3;
    edm::Ref<vector<osu::Bjet> >              bjet4;
//#endif

//#if IS_VALID(bxlumis)
    osu::Bxlumi                               bxlumi0;
    vector<osu::Bxlumi>                       bxlumi1;
    edm::Wrapper<osu::Bxlumi>                 bxlumi2;
    edm::Wrapper<vector<osu::Bxlumi> >        bxlumi3;
    edm::Ref<vector<osu::Bxlumi> >            bxlumi4;
//#endif

//#if IS_VALID(cschits)
    osu::Cschit                               cschit0;
    vector<osu::Cschit>                       cschit1;
    edm::Wrapper<osu::Cschit>                 cschit2;
    edm::Wrapper<vector<osu::Cschit> >        cschit3;
    edm::Ref<vector<osu::Cschit> >            cschit4;
//#endif

//#if IS_VALID(cscsegs)
    osu::Cscseg                               cscseg0;
    vector<osu::Cscseg>                       cscseg1;
    edm::Wrapper<osu::Cscseg>                 cscseg2;
    edm::Wrapper<vector<osu::Cscseg> >        cscseg3;
    edm::Ref<vector<osu::Cscseg> >            cscseg4;
//#endif

//#if IS_VALID(dtsegs)
    osu::Dtseg                                dtseg0;
    vector<osu::Dtseg>                        dtseg1;
    edm::Wrapper<osu::Dtseg>                  dtseg2;
    edm::Wrapper<vector<osu::Dtseg> >         dtseg3;
    edm::Ref<vector<osu::Dtseg> >             dtseg4;
//#endif

//#if IS_VALID(electrons)
    osu::Electron                             electron0;
    vector<osu::Electron>                     electron1;
    edm::Wrapper<osu::Electron>               electron2;
    edm::Wrapper<vector<osu::Electron> >      electron3;
    edm::Ref<vector<osu::Electron> >          electron4;
//#endif

//#if IS_VALID(events)
    osu::Event                                event0;
    vector<osu::Event>                        event1;
    edm::Wrapper<osu::Event>                  event2;
    edm::Wrapper<vector<osu::Event> >         event3;
    edm::Ref<vector<osu::Event> >             event4;
//#endif

//#if IS_VALID(eventvariables)
    osu::Eventvariable                        eventvariable0;
    vector<osu::Eventvariable>                eventvariable1;
    edm::Wrapper<osu::Eventvariable>          eventvariable2;
    edm::Wrapper<vector<osu::Eventvariable> > eventvariable3;
    edm::Ref<vector<osu::Eventvariable> >     eventvariable4;
//#endif

//#if IS_VALID(genjets)
    osu::Genjet                               genjet0;
    vector<osu::Genjet>                       genjet1;
    edm::Wrapper<osu::Genjet>                 genjet2;
    edm::Wrapper<vector<osu::Genjet> >        genjet3;
    edm::Ref<vector<osu::Genjet> >            genjet4;
//#endif

//#if IS_VALID(jets)
    osu::Jet                                  jet0;
    vector<osu::Jet>                          jet1;
    edm::Wrapper<osu::Jet>                    jet2;
    edm::Wrapper<vector<osu::Jet> >           jet3;
    edm::Ref<vector<osu::Jet> >               jet4;
//#endif

//#if IS_VALID(mcparticles)
    osu::Mcparticle                           mcparticle0;
    vector<osu::Mcparticle>                   mcparticle1;
    edm::Wrapper<osu::Mcparticle>             mcparticle2;
    edm::Wrapper<vector<osu::Mcparticle> >    mcparticle3;
    edm::Ref<vector<osu::Mcparticle> >        mcparticle4;
//#endif

//#if IS_VALID(mets)
    osu::Met                                  met0;
    vector<osu::Met>                          met1;
    edm::Wrapper<osu::Met>                    met2;
    edm::Wrapper<vector<osu::Met> >           met3;
    edm::Ref<vector<osu::Met> >               met4;
//#endif

//#if IS_VALID(muons)
    osu::Muon                                 muon0;
    vector<osu::Muon>                         muon1;
    edm::Wrapper<osu::Muon>                   muon2;
    edm::Wrapper<vector<osu::Muon> >          muon3;
    edm::Ref<vector<osu::Muon> >              muon4;
//#endif

//#if IS_VALID(photons)
    osu::Photon                               photon0;
    vector<osu::Photon>                       photon1;
    edm::Wrapper<osu::Photon>                 photon2;
    edm::Wrapper<vector<osu::Photon> >        photon3;
    edm::Ref<vector<osu::Photon> >            photon4;
//#endif

//#if IS_VALID(primaryvertexs)
    osu::Primaryvertex                        primaryvertex0;
    vector<osu::Primaryvertex>                primaryvertex1;
    edm::Wrapper<osu::Primaryvertex>          primaryvertex2;
    edm::Wrapper<vector<osu::Primaryvertex> > primaryvertex3;
    edm::Ref<vector<osu::Primaryvertex> >     primaryvertex4;
//#endif

//#if IS_VALID(rpchits)
    osu::Rpchit                               rpchit0;
    vector<osu::Rpchit>                       rpchit1;
    edm::Wrapper<osu::Rpchit>                 rpchit2;
    edm::Wrapper<vector<osu::Rpchit> >        rpchit3;
    edm::Ref<vector<osu::Rpchit> >            rpchit4;
//#endif

//#if IS_VALID(superclusters)
    osu::Supercluster                         supercluster0;
    vector<osu::Supercluster>                 supercluster1;
    edm::Wrapper<osu::Supercluster>           supercluster2;
    edm::Wrapper<vector<osu::Supercluster> >  supercluster3;
    edm::Ref<vector<osu::Supercluster> >      supercluster4;
//#endif

//#if IS_VALID(taus)
    osu::Tau                                  tau0;
    vector<osu::Tau>                          tau1;
    edm::Wrapper<osu::Tau>                    tau2;
    edm::Wrapper<vector<osu::Tau> >           tau3;
    edm::Ref<vector<osu::Tau> >               tau4;
//#endif

//#if IS_VALID(tracks)
    osu::Track                                track0;
    vector<osu::Track>                        track1;
    edm::Wrapper<osu::Track>                  track2;
    edm::Wrapper<vector<osu::Track> >         track3;
    edm::Ref<vector<osu::Track> >             track4;
//#endif

//#if IS_VALID(trigobjs)
    osu::Trigobj                              trigobj0;
    vector<osu::Trigobj>                      trigobj1;
    edm::Wrapper<osu::Trigobj>                trigobj2;
    edm::Wrapper<vector<osu::Trigobj> >       trigobj3;
    edm::Ref<vector<osu::Trigobj> >           trigobj4;
//#endif

//#if IS_VALID(uservariables)
    osu::Uservariable                         uservariable0;
    vector<osu::Uservariable>                 uservariable1;
    edm::Wrapper<osu::Uservariable>           uservariable2;
    edm::Wrapper<vector<osu::Uservariable> >  uservariable3;
    edm::Ref<vector<osu::Uservariable> >      uservariable4;
//#endif

  };
}
