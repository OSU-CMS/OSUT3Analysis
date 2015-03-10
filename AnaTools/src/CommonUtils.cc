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
  if  (VEC_CONTAINS  (objectsToGet,  "uservariables")   &&  collections.exists  ("uservariables"))
    {
      handles.uservariables.clear ();
      for (const auto &collection : collections.getParameter<vector<edm::InputTag> >  ("uservariables"))
        {
          handles.uservariables.resize (handles.uservariables.size () + 1);
          getCollection (collection, handles.uservariables.back (), event);
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

#ifdef ROOT6
  #include "FWCore/Utilities/interface/BaseWithDict.h"
  #include "FWCore/Utilities/interface/FunctionWithDict.h"
  #include "FWCore/Utilities/interface/MemberWithDict.h"
  #include "FWCore/Utilities/interface/ObjectWithDict.h"
  #include "FWCore/Utilities/interface/TypeWithDict.h"

  double
  anatools::getMember (const string &type, const void * const obj, const string &member)
  {
    double value = numeric_limits<int>::min ();
    edm::TypeWithDict t = edm::TypeWithDict::byName (type);
    edm::ObjectWithDict *o = new edm::ObjectWithDict (t, (void *) obj);
    const edm::MemberWithDict &dataMember = t.dataMemberByName (member);
    const edm::FunctionWithDict &functionMember = t.functionMemberByName (member);
    string dataMemberType = "", functionMemberType = "";

    if (dataMember)
      dataMemberType = dataMember.typeOf ().name ();
    if (functionMember)
      functionMemberType = functionMember.finalReturnType ().name ();
    try
      {
        if (dataMemberType != "")
          {
            if (dataMemberType == "float")
              value = dataMember.get (*o).objectCast<float> ();
            else if (dataMemberType == "double")
              value = dataMember.get (*o).objectCast<double> ();
            else if (dataMemberType == "long double")
              value = dataMember.get (*o).objectCast<long double> ();
            else if (dataMemberType == "char")
              value = dataMember.get (*o).objectCast<char> ();
            else if (dataMemberType == "int")
              value = dataMember.get (*o).objectCast<int> ();
            else if (dataMemberType == "unsigned")
              value = dataMember.get (*o).objectCast<unsigned> ();
            else if (dataMemberType == "bool")
              value = dataMember.get (*o).objectCast<bool> ();
            else if (dataMemberType == "unsigned int")
              value = dataMember.get (*o).objectCast<unsigned int> ();
            else
              clog << "WARNING: \"" << member << "\" has unrecognized type \"" << dataMemberType << "\"" << endl;
          }
        else if (functionMemberType != "")
          {
            if (functionMemberType == "float")
              value = invoke<float> (functionMemberType, o, functionMember);
            else if (functionMemberType == "double")
              value = invoke<double> (functionMemberType, o, functionMember);
            else if (functionMemberType == "long double")
              value = invoke<long double> (functionMemberType, o, functionMember);
            else if (functionMemberType == "char")
              value = invoke<char> (functionMemberType, o, functionMember);
            else if (functionMemberType == "int")
              value = invoke<int> (functionMemberType, o, functionMember);
            else if (functionMemberType == "unsigned")
              value = invoke<unsigned> (functionMemberType, o, functionMember);
            else if (functionMemberType == "bool")
              value = invoke<bool> (functionMemberType, o, functionMember);
            else if (functionMemberType == "unsigned int")
              value = invoke<unsigned int> (functionMemberType, o, functionMember);
            else
              clog << "WARNING: \"" << member << "()\" has unrecognized return type \"" << functionMemberType << "\"" << endl;
          }
        else
          throw 0;
      }
    catch (...)
      {
        bool found = false;
        for (const auto &bi : edm::TypeBases (t))
          {
            edm::BaseWithDict baseClass (bi);
            string baseName = baseClass.name ();
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
  anatools::invoke (const string &returnType, edm::ObjectWithDict * const o, const edm::FunctionWithDict &functionMember)
  {
    T mem;
    edm::ObjectWithDict *value = new edm::ObjectWithDict (edm::TypeWithDict::byName (returnType), &mem);
    functionMember.invoke (*o, value);
    return value->objectCast<T> ();
  }
#else
  #include "Reflex/Base.h"
  #include "Reflex/Member.h"
  #include "Reflex/Object.h"
  #include "Reflex/Type.h"

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
            else if (dataMemberType == "unsigned int")
              value = Reflex::Object_Cast<unsigned int> (o->Get (member));
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
            else if (functionMemberType == "unsigned int")
              value = invoke<unsigned int> (functionMemberType, o, member);
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
#endif

#if IS_VALID(bxlumis)
  string  anatools::getObjectType  (const  TYPE(bxlumis)         &obj)  {  return  "bxlumi";         }
  string  anatools::getObjectClass  (const  TYPE(bxlumis)         &obj)  {  return  TYPE_STR(bxlumis);         }
#endif
#if IS_VALID(electrons)
  string  anatools::getObjectType  (const  TYPE(electrons)       &obj)  {  return  "electron";       }
  string  anatools::getObjectClass  (const  TYPE(electrons)       &obj)  {  return  TYPE_STR(electrons);       }
#endif
#if IS_VALID(events)
  string  anatools::getObjectType  (const  TYPE(events)          &obj)  {  return  "event";          }
  string  anatools::getObjectClass  (const  TYPE(events)          &obj)  {  return  TYPE_STR(events);          }
  template<> int anatools::getObjectHash<TYPE(events)> (const TYPE(events) &object){
    int run, lumi, evt;
    run = abs(int(getMember (object, "run")));
    lumi = abs(int(getMember (object, "lumi")));
    evt = abs(int(getMember (object, "evt")));
    return run + lumi + evt;
  }
#endif
#if IS_VALID(genjets)
  string  anatools::getObjectType  (const  TYPE(genjets)         &obj)  {  return  "genjet";         }
  string  anatools::getObjectClass  (const  TYPE(genjets)         &obj)  {  return  TYPE_STR(genjets);         }
#endif
#if IS_VALID(jets)
  string  anatools::getObjectType  (const  TYPE(jets)            &obj)  {  return  "jet";            }
  string  anatools::getObjectClass  (const  TYPE(jets)            &obj)  {  return  TYPE_STR(jets);            }
#endif
#if IS_VALID(mcparticles)
  string  anatools::getObjectType  (const  TYPE(mcparticles)     &obj)  {  return  "mcparticle";     }
  string  anatools::getObjectClass  (const  TYPE(mcparticles)     &obj)  {  return  TYPE_STR(mcparticles);     }
#endif
#if IS_VALID(mets)
  string  anatools::getObjectType  (const  TYPE(mets)            &obj)  {  return  "met";            }
  string  anatools::getObjectClass  (const  TYPE(mets)            &obj)  {  return  TYPE_STR(mets);            }
  template<> int anatools::getObjectHash<TYPE(mets)> (const TYPE(mets)& object){
    int px_mev, py_mev;
    px_mev = fabs(int(10000 * getMember (object, "px")));
    py_mev = fabs(int(10000 * getMember (object, "py")));
    return px_mev + py_mev;
  }
#endif
#if IS_VALID(muons)
  string  anatools::getObjectType  (const  TYPE(muons)           &obj)  {  return  "muon";           }
  string  anatools::getObjectClass  (const  TYPE(muons)           &obj)  {  return  TYPE_STR(muons);           }
#endif
#if IS_VALID(photons)
  string  anatools::getObjectType  (const  TYPE(photons)         &obj)  {  return  "photon";         }
  string  anatools::getObjectClass  (const  TYPE(photons)         &obj)  {  return  TYPE_STR(photons);         }
#endif
#if IS_VALID(primaryvertexs)
  string  anatools::getObjectType  (const  TYPE(primaryvertexs)  &obj)  {  return  "primaryvertex";  }
  string  anatools::getObjectClass  (const  TYPE(primaryvertexs)  &obj)  {  return  TYPE_STR(primaryvertexs);  }
  template<> int anatools::getObjectHash<TYPE(primaryvertexs)> (const TYPE(primaryvertexs)& object){
    int x_mum, y_mum, z_mum;
    x_mum = abs(int(10000 * getMember (object, "x")));
    y_mum = abs(int(10000 * getMember (object, "y")));
    z_mum = abs(int(10000 * getMember (object, "z")));
    return x_mum + y_mum + z_mum;
  }
#endif
#if IS_VALID(superclusters)
  string  anatools::getObjectType  (const  TYPE(superclusters)   &obj)  {  return  "supercluster";   }
  string  anatools::getObjectClass  (const  TYPE(superclusters)   &obj)  {  return  TYPE_STR(superclusters);   }
#endif
#if IS_VALID(taus)
  string  anatools::getObjectType  (const  TYPE(taus)            &obj)  {  return  "tau";            }
  string  anatools::getObjectClass  (const  TYPE(taus)            &obj)  {  return  TYPE_STR(taus);            }
#endif
#if IS_VALID(tracks)
  string  anatools::getObjectType  (const  TYPE(tracks)          &obj)  {  return  "track";          }
  string  anatools::getObjectClass  (const  TYPE(tracks)          &obj)  {  return  TYPE_STR(tracks);          }
#endif
#if IS_VALID(trigobjs)
  string  anatools::getObjectType  (const  TYPE(trigobjs)        &obj)  {  return  "trigobj";        }
  string  anatools::getObjectClass  (const  TYPE(trigobjs)        &obj)  {  return  TYPE_STR(trigobjs);        }
#endif

// user-defined cases
string anatools::getObjectType (const VariableProducerPayload& obj){ return "uservariable"; }
string anatools::getObjectClass (const VariableProducerPayload& obj){ return "VariableProducerPayload"; }
