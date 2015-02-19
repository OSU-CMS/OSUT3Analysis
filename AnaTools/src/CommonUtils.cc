#include "Reflex/Base.h"
#include "Reflex/Member.h"
#include "Reflex/Object.h"
#include "Reflex/Type.h"

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

vector<string>
anatools::getSingleObjects (string inputLabel)
{
  vector<string> singleObjects;
  size_t hyphen;
  while ((hyphen = inputLabel.find ('-')) != string::npos)
    {
      singleObjects.push_back (plural (inputLabel.substr (0, hyphen)));
      inputLabel = inputLabel.substr (hyphen + 1);
    }
  singleObjects.push_back (plural (inputLabel));
  sort (singleObjects.begin (), singleObjects.end ());

  return singleObjects;
}

string
anatools::concatenateInputCollection (const vector<string> &inputCollections)
{
  string catInputCollection = "";
  for (auto collection = inputCollections.begin (); collection != inputCollections.end (); collection++)
    {
      if (collection != inputCollections.begin ())
        catInputCollection += "-";
      catInputCollection += singular (*collection);
    }
  return plural (catInputCollection);
}

string
anatools::capitalize (string input)
{
  input.front () = toupper (input.front ());
  return input;
}

string
anatools::singular (string input)
{
  if (tolower (input.back ()) == 's')
    return input.substr (0, input.size () - 1);
  else
    return input;
}

string
anatools::plural (string input)
{
  if (tolower (input.back ()) == 's')
    return input;
  else
    return input + "s";
}

string &
anatools::ltrim (string &s)
{
  return s.erase (0, s.find_first_not_of (" \t\f\n\r"));
}

string &
anatools::rtrim (string &s)
{
  return s.erase (s.find_last_not_of (" \t\f\n\r") + 1);
}

string &
anatools::trim (string &s)
{
  return ltrim (rtrim (s));
}

bool
anatools::firstOfPairAscending (pair<size_t, string> a, pair<size_t, string> b)
{
  return (a.first < b.first);
}

bool
anatools::collectionIndexAscending (pair<string, tuple<unsigned, unsigned, void *> > a, pair<string, tuple<unsigned, unsigned, void *> > b)
{
  return (get<0> (a.second) < get<0> (b.second));
}

void
anatools::getRequiredCollections (const unordered_set<string> &objectsToGet, const edm::ParameterSet &collections, Collections &handles, const edm::Event &event)
{
  static bool firstEvent = true;

  //////////////////////////////////////////////////////////////////////////////
  // Retrieve each object collection which we need and print a warning if it is
  // missing.
  //////////////////////////////////////////////////////////////////////////////
  if  (VEC_CONTAINS  (objectsToGet,  "bxlumis")         &&  collections.exists  ("bxlumis"))         getCollection  (collections.getParameter<edm::InputTag>  ("bxlumis"),         handles.bxlumis,         event);
  if  (VEC_CONTAINS  (objectsToGet,  "electrons")       &&  collections.exists  ("electrons"))       getCollection  (collections.getParameter<edm::InputTag>  ("electrons"),       handles.electrons,       event);
  if  (VEC_CONTAINS  (objectsToGet,  "events")          &&  collections.exists  ("events"))          getCollection  (collections.getParameter<edm::InputTag>  ("events"),          handles.events,          event);
  if  (VEC_CONTAINS  (objectsToGet,  "genjets")         &&  collections.exists  ("genjets"))         getCollection  (collections.getParameter<edm::InputTag>  ("genjets"),         handles.genjets,         event);
  if  (VEC_CONTAINS  (objectsToGet,  "jets")            &&  collections.exists  ("jets"))            getCollection  (collections.getParameter<edm::InputTag>  ("jets"),            handles.jets,            event);
  if  (VEC_CONTAINS  (objectsToGet,  "mcparticles")     &&  collections.exists  ("mcparticles"))     getCollection  (collections.getParameter<edm::InputTag>  ("mcparticles"),     handles.mcparticles,     event);
  if  (VEC_CONTAINS  (objectsToGet,  "mets")            &&  collections.exists  ("mets"))            getCollection  (collections.getParameter<edm::InputTag>  ("mets"),            handles.mets,            event);
  if  (VEC_CONTAINS  (objectsToGet,  "muons")           &&  collections.exists  ("muons"))           getCollection  (collections.getParameter<edm::InputTag>  ("muons"),           handles.muons,           event);
  if  (VEC_CONTAINS  (objectsToGet,  "photons")         &&  collections.exists  ("photons"))         getCollection  (collections.getParameter<edm::InputTag>  ("photons"),         handles.photons,         event);
  if  (VEC_CONTAINS  (objectsToGet,  "primaryvertexs")  &&  collections.exists  ("primaryvertexs"))  getCollection  (collections.getParameter<edm::InputTag>  ("primaryvertexs"),  handles.primaryvertexs,  event);
  if  (VEC_CONTAINS  (objectsToGet,  "superclusters")   &&  collections.exists  ("superclusters"))   getCollection  (collections.getParameter<edm::InputTag>  ("superclusters"),   handles.superclusters,   event);
  if  (VEC_CONTAINS  (objectsToGet,  "taus")            &&  collections.exists  ("taus"))            getCollection  (collections.getParameter<edm::InputTag>  ("taus"),            handles.taus,            event);
  if  (VEC_CONTAINS  (objectsToGet,  "tracks")          &&  collections.exists  ("tracks"))          getCollection  (collections.getParameter<edm::InputTag>  ("tracks"),          handles.tracks,          event);
  if  (VEC_CONTAINS  (objectsToGet,  "triggers")        &&  collections.exists  ("triggers"))        getCollection  (collections.getParameter<edm::InputTag>  ("triggers"),        handles.triggers,        event);
  if  (VEC_CONTAINS  (objectsToGet,  "trigobjs")        &&  collections.exists  ("trigobjs"))        getCollection  (collections.getParameter<edm::InputTag>  ("trigobjs"),        handles.trigobjs,        event);

  if  (VEC_CONTAINS  (objectsToGet,  "userVariables")   &&  collections.exists  ("userVariables"))
    {
      handles.userVariables.clear ();
      for (const auto &collection : collections.getParameter<vector<edm::InputTag> >  ("userVariables"))
        {
          handles.userVariables.resize (handles.userVariables.size () + 1);
          getCollection (collection, handles.userVariables.back (), event);
        }
    }


  if (firstEvent)
    clog << "Will print any collections not retrieved.  These INFO messages may be safely ignored." << endl;
  if (firstEvent && !handles.bxlumis.isValid ())
    clog << "INFO: did not retrieve bxlumis collection from the event." << endl;
  if (firstEvent && !handles.electrons.isValid ())
    clog << "INFO: did not retrieve electrons collection from the event." << endl;
  if (firstEvent && !handles.events.isValid ())
    clog << "INFO: did not retrieve events collection from the event." << endl;
  if (firstEvent && !handles.genjets.isValid ())
    clog << "INFO: did not retrieve genjets collection from the event." << endl;
  if (firstEvent && !handles.jets.isValid ())
    clog << "INFO: did not retrieve jets collection from the event." << endl;
  if (firstEvent && !handles.mcparticles.isValid ())
    clog << "INFO: did not retrieve mcparticles collection from the event." << endl;
  if (firstEvent && !handles.mets.isValid ())
    clog << "INFO: did not retrieve mets collection from the event." << endl;
  if (firstEvent && !handles.muons.isValid ())
    clog << "INFO: did not retrieve muons collection from the event." << endl;
  if (firstEvent && !handles.photons.isValid ())
    clog << "INFO: did not retrieve photons collection from the event." << endl;
  if (firstEvent && !handles.primaryvertexs.isValid ())
    clog << "INFO: did not retrieve primaryvertexs collection from the event." << endl;
  if (firstEvent && !handles.superclusters.isValid ())
    clog << "INFO: did not retrieve superclusters collection from the event." << endl;
  if (firstEvent && !handles.taus.isValid ())
    clog << "INFO: did not retrieve taus collection from the event." << endl;
  if (firstEvent && !handles.tracks.isValid ())
    clog << "INFO: did not retrieve tracks collection from the event." << endl;
  if (firstEvent && !handles.triggers.isValid ())
    clog << "INFO: did not retrieve triggers collection from the event." << endl;
  if (firstEvent && !handles.trigobjs.isValid ())
    clog << "INFO: did not retrieve trigobjs collection from the event." << endl;
  //////////////////////////////////////////////////////////////////////////////

  firstEvent = false;
}

#if DATA_FORMAT == BEAN
template<> int anatools::getObjectHash<BNevent> (const BNevent& object){
  int run, lumi, evt;
  run = abs(int(object.run));
  lumi = abs(int(object.lumi));
  evt = abs(int(object.evt));
  return run + lumi + evt;
}

template<> int anatools::getObjectHash<BNmet> (const BNmet& object){
  int px_mev, py_mev;
  px_mev = fabs(int(10000 * object.px));
  py_mev = fabs(int(10000 * object.py));
  return px_mev + py_mev;
}

template<> int anatools::getObjectHash<BNprimaryvertex> (const BNprimaryvertex& object){
  int x_mum, y_mum, z_mum;
  x_mum = abs(int(10000 * object.x));
  y_mum = abs(int(10000 * object.y));
  z_mum = abs(int(10000 * object.z));
  return x_mum + y_mum + z_mum;
}
#endif

double
anatools::getMember (const string &type, const void * const obj, const string &member)
{
  double value = numeric_limits<int>::min ();
  Reflex::Type t = Reflex::Type::ByName (type);
  Reflex::Object *o = new Reflex::Object (t, (void *) obj);
  string dataMemberType, functionMemberType;

  dataMemberType = t.DataMemberByName (member).TypeOf ().Name ();
  functionMemberType = t.FunctionMemberByName (member).TypeOf ().ReturnType ().Name ();
  try
    {
      if (dataMemberType != "")
        {
          if (dataMemberType == "float")
            value = Reflex::Object_Cast<float> (o->Get (member));
          else if (dataMemberType == "double")
            value = Reflex::Object_Cast<double> (o->Get (member));
          else if (dataMemberType == "long double")
            value = Reflex::Object_Cast<long double> (o->Get (member));
          else if (dataMemberType == "char")
            value = Reflex::Object_Cast<char> (o->Get (member));
          else if (dataMemberType == "int")
            value = Reflex::Object_Cast<int> (o->Get (member));
          else if (dataMemberType == "unsigned")
            value = Reflex::Object_Cast<unsigned> (o->Get (member));
          else if (dataMemberType == "bool")
            value = Reflex::Object_Cast<bool> (o->Get (member));
          else
            clog << "WARNING: \"" << member << "\" has unrecognized type \"" << dataMemberType << "\"" << endl;
        }
      else if (functionMemberType != "")
        {
          if (functionMemberType == "float")
            value = invoke<float> (functionMemberType, o, member);
          else if (functionMemberType == "double")
            value = invoke<double> (functionMemberType, o, member);
          else if (functionMemberType == "long double")
            value = invoke<long double> (functionMemberType, o, member);
          else if (functionMemberType == "char")
            value = invoke<char> (functionMemberType, o, member);
          else if (functionMemberType == "int")
            value = invoke<int> (functionMemberType, o, member);
          else if (functionMemberType == "unsigned")
            value = invoke<unsigned> (functionMemberType, o, member);
          else if (functionMemberType == "bool")
            value = invoke<bool> (functionMemberType, o, member);
          else
            clog << "WARNING: \"" << member << "()\" has unrecognized return type \"" << functionMemberType << "\"" << endl;
        }
      else
        throw 0;
    }
  catch (...)
    {
      bool found = false;
      for (auto bi = t.Base_Begin (); bi != t.Base_End (); bi++)
        {
          string baseName = bi->Name ();
          addDeclaringScope (bi->ToScope (), baseName);
          try
            {
              value = getMember (baseName, obj, member);
              found = true;
              break;
            }
          catch (...)
            {
              continue;
            }
        }
      if (!found)
        throw;
    }
  delete o;

  return value;
}

template<class T> T
anatools::invoke (const string &returnType, Reflex::Object * const o, const string &member)
{
  T mem;
  Reflex::Object *value = new Reflex::Object (Reflex::Type::ByName (returnType), &mem);
  o->Invoke (member, value);
  return Reflex::Object_Cast<T> (*value);
}

void
anatools::addDeclaringScope (const Reflex::Scope &scope, string &baseName)
{
  if (!scope.IsTopScope ())
    {
      baseName = scope.DeclaringScope ().Name () + "::" + baseName;
      addDeclaringScope (scope.DeclaringScope (), baseName);
    }
}

#if DATA_FORMAT == BEAN
string anatools::getObjectType (const BNbxlumi& obj){ return "bxlumi"; }
string anatools::getObjectType (const BNelectron& obj){ return "electron"; }
string anatools::getObjectType (const BNevent& obj){ return "event"; }
string anatools::getObjectType (const BNgenjet& obj){ return "genjet"; }
string anatools::getObjectType (const BNjet& obj){ return "jet"; }
string anatools::getObjectType (const BNmcparticle& obj){ return "mcparticle"; }
string anatools::getObjectType (const BNmet& obj){ return "met"; }
string anatools::getObjectType (const BNmuon& obj){ return "muon"; }
string anatools::getObjectType (const BNphoton& obj){ return "photon"; }
string anatools::getObjectType (const BNprimaryvertex& obj){ return "primaryvertex"; }
string anatools::getObjectType (const BNsupercluster& obj){ return "supercluster"; }
string anatools::getObjectType (const BNtau& obj){ return "tau"; }
string anatools::getObjectType (const BNtrack& obj){ return "track"; }
string anatools::getObjectType (const BNtrigobj& obj){ return "trigobj"; }

string anatools::getObjectClass (const BNbxlumi& obj){ return "BNbxlumi"; }
string anatools::getObjectClass (const BNelectron& obj){ return "BNelectron"; }
string anatools::getObjectClass (const BNevent& obj){ return "BNevent"; }
string anatools::getObjectClass (const BNgenjet& obj){ return "BNgenjet"; }
string anatools::getObjectClass (const BNjet& obj){ return "BNjet"; }
string anatools::getObjectClass (const BNmcparticle& obj){ return "BNmcparticle"; }
string anatools::getObjectClass (const BNmet& obj){ return "BNmet"; }
string anatools::getObjectClass (const BNmuon& obj){ return "BNmuon"; }
string anatools::getObjectClass (const BNphoton& obj){ return "BNphoton"; }
string anatools::getObjectClass (const BNprimaryvertex& obj){ return "BNprimaryvertex"; }
string anatools::getObjectClass (const BNsupercluster& obj){ return "BNsupercluster"; }
string anatools::getObjectClass (const BNtau& obj){ return "BNtau"; }
string anatools::getObjectClass (const BNtrack& obj){ return "BNtrack"; }
string anatools::getObjectClass (const BNtrigobj& obj){ return "BNtrigobj"; }

#elif DATA_FORMAT == MINI_AOD
string anatools::getObjectType (const pat::Electron& obj){ return "electron"; }
string anatools::getObjectType (const reco::GenJet& obj){ return "genjet"; }
string anatools::getObjectType (const pat::Jet& obj){ return "jet"; }
string anatools::getObjectType (const pat::PackedGenParticle& obj){ return "mcparticle"; }
string anatools::getObjectType (const pat::MET& obj){ return "met"; }
string anatools::getObjectType (const pat::Muon& obj){ return "muon"; }
string anatools::getObjectType (const pat::Photon& obj){ return "photon"; }
string anatools::getObjectType (const reco::Vertex& obj){ return "vertex"; }
string anatools::getObjectType (const reco::SuperCluster& obj){ return "supercluster"; }
string anatools::getObjectType (const pat::Tau& obj){ return "tau"; }
string anatools::getObjectType (const pat::TriggerObjectStandAlone& obj){ return "trigobj"; }
string anatools::getObjectType (const edm::TriggerResults& obj){ return "trigger"; }

string anatools::getObjectClass (const pat::Electron& obj){ return "pat::Electron"; }
string anatools::getObjectClass (const reco::GenJet& obj){ return "reco::GenJet"; }
string anatools::getObjectClass (const pat::Jet& obj){ return "pat::Jet"; }
string anatools::getObjectClass (const pat::PackedGenParticle& obj){ return "pat::PackedGenParticle"; }
string anatools::getObjectClass (const pat::MET& obj){ return "pat::MET"; }
string anatools::getObjectClass (const pat::Muon& obj){ return "pat::Muon"; }
string anatools::getObjectClass (const pat::Photon& obj){ return "pat::Photon"; }
string anatools::getObjectClass (const reco::Vertex& obj){ return "reco::Vertex"; }
string anatools::getObjectClass (const reco::SuperCluster& obj){ return "reco::SuperCluster"; }
string anatools::getObjectClass (const pat::Tau& obj){ return "pat::Tau"; }
string anatools::getObjectClass (const pat::TriggerObjectStandAlone& obj){ return "pat::TriggerObjectStandAlone"; }
string anatools::getObjectClass (const edm::TriggerResults& obj){ return "edm::TriggerResults"; }
#endif

// user-defined cases
string anatools::getObjectType (const VariableProducerPayload& obj){ return "uservariable"; }
string anatools::getObjectClass (const VariableProducerPayload& obj){ return "VariableProducerPayload"; }
