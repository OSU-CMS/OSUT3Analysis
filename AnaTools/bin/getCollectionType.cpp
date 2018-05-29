#include <iostream>

#include "OSUT3Analysis/AnaTools/interface/DataFormat.h"

using namespace std;

int
main (int argc, char *argv[])
{
  for (int i = 0; i < argc - 1; i++)
    {
      string cppType (argv[i + 1]);

      if (cppType == "INVALID_TYPE");
#if IS_VALID(beamspots)
      else if (cppType == TYPE_STR(beamspots))
        cout << "beamspots" << endl;
#endif
#if IS_VALID(bxlumis)
      else if (cppType == "vector<" TYPE_STR(bxlumis) ">")
        cout << "bxlumis" << endl;
#endif
#if IS_VALID(electrons)
      else if (cppType == "vector<" TYPE_STR(electrons) ">")
        cout << "electrons" << endl;
#endif
#if IS_VALID(events)
      else if (cppType == "vector<" TYPE_STR(events) ">")
        cout << "events" << endl;
#endif
#if IS_VALID(genjets)
      else if (cppType == "vector<" TYPE_STR(genjets) ">")
        cout << "genjets" << endl;
#endif
#if IS_VALID(jets)
      else if (cppType == "vector<" TYPE_STR(jets) ">")
        cout << "jets" << endl;
#endif
#if IS_VALID(bjets)
      else if (cppType == "vector<" TYPE_STR(bjets) ">")
        cout << "bjets" << endl;
#endif
#if IS_VALID(mcparticles)
      else if (cppType == "vector<" TYPE_STR(mcparticles) ">")
        cout << "mcparticles" << endl;
#endif
#if IS_VALID(mets)
      else if (cppType == "vector<" TYPE_STR(mets) ">")
        cout << "mets" << endl;
#endif
#if IS_VALID(muons)
      else if (cppType == "vector<" TYPE_STR(muons) ">")
        cout << "muons" << endl;
#endif
#if IS_VALID(photons)
      else if (cppType == "vector<" TYPE_STR(photons) ">")
        cout << "photons" << endl;
#endif
#if IS_VALID(primaryvertexs)
      else if (cppType == "vector<" TYPE_STR(primaryvertexs) ">")
        cout << "primaryvertexs" << endl;
#endif
#if IS_VALID(superclusters)
      else if (cppType == "vector<" TYPE_STR(superclusters) ">")
        cout << "superclusters" << endl;
#endif
#if IS_VALID(taus)
      else if (cppType == "vector<" TYPE_STR(taus) ">")
        cout << "taus" << endl;
#endif
#if IS_VALID(tracks)
      else if (cppType == "vector<" TYPE_STR(tracks) ">")
        cout << "tracks" << endl;
#endif
#if IS_VALID(secondaryTracks)
      else if (cppType == "vector<" TYPE_STR(secondaryTracks) ">")
        cout << "secondaryTracks" << endl;
#endif
#if IS_VALID(pileupinfos)
      else if (cppType == "vector<" TYPE_STR(pileupinfos) ">")
        cout << "pileupinfos" << endl;
#endif
#if IS_VALID(trigobjs)
      else if (cppType == "vector<" TYPE_STR(trigobjs) ">")
        cout << "trigobjs" << endl;
#endif
#if IS_VALID(uservariables)
      else if (cppType == "vector<" TYPE_STR(uservariables) ">")
        cout << "uservariables" << endl;
#endif
#if IS_VALID(eventvariables)
      else if (cppType == "vector<" TYPE_STR(eventvariables) ">")
        cout << "eventvariables" << endl;
#endif
      else
        cout << "INVALID_TYPE" << endl;
    }

  return 0;
}
