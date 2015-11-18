#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"

/**
 * Splits the concatenated object label into a vector of individual labels.
 *
 * @param  inputLabel string giving the concatenation of multiple object labels
 *         with hyphens between
 * @return vector of strings corresponding to the individual object labels
 */
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

/**
 * Concatenates the input strings with hyphens between.
 *
 * @param  inputCollections vector of strings to concatenate
 * @return concatenation of input strings with hyphens between
 */
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

/**
 * Capitalizes the input string without modifying the original string.
 *
 * @param  input string to capitalize
 * @return capitalized string
 */
string
anatools::capitalize (string input)
{
  input.front () = toupper (input.front ());
  return input;
}

/**
 * Removes a trailing 's' from a string without modifying the original string.
 *
 * @param  input string from which to remove the 's'
 * @return string with a trailing 's' removed
 */
string
anatools::singular (string input)
{
  if (tolower (input.back ()) == 's')
    return input.substr (0, input.size () - 1);
  else
    return input;
}

/**
 * Appends an 's' to a string without modifying the original string.
 *
 * @param  input string to which to append the 's'
 * @return string with the appended 's'
 */
string
anatools::plural (string input)
{
  if (tolower (input.back ()) == 's')
    return input;
  else
    return input + "s";
}

/**
 * Removes whitespace from the left of a string, modifying original string.
 *
 * @param  s string from which to remove whitespace
 * @return reference to the original string
 */
string &
anatools::ltrim (string &s)
{
  return s.erase (0, s.find_first_not_of (" \t\f\n\r"));
}

/**
 * Removes whitespace from the right of a string, modifying original string.
 *
 * @param  s string from which to remove whitespace
 * @return reference to the original string
 */
string &
anatools::rtrim (string &s)
{
  return s.erase (s.find_last_not_of (" \t\f\n\r") + 1);
}

/**
 * Removes whitespace from both sides of a string, modifying original string.
 *
 * @param  s string from which to remove whitespace
 * @return reference to the original string
 */
string &
anatools::trim (string &s)
{
  return ltrim (rtrim (s));
}

/**
 * Returns whether the first members of the tuples are in ascending order.
 *
 * This function is meant to be used with std::sort to sort a vector of tuples
 * so that the first members of the tuples are in ascending order.
 *
 * @param  a first of two tuples to compare
 * @param  b second of two tuples to compare
 * @return boolean representing whether the first members of the two tuples are
 *         in ascending order
 */
bool
anatools::firstOfTupleAscending (tuple<size_t, size_t, string> a, tuple<size_t, size_t, string> b)
{
  return (get<0> (a) < get<0> (b));
}

/**
 * Returns whether the first members of the tuples are in descending order.
 *
 * This function is meant to be used with std::sort to sort a vector of tuples
 * so that the first members of the tuples are in descending order.
 *
 * @param  a first of two tuples to compare
 * @param  b second of two tuples to compare
 * @return boolean representing whether the first members of the two tuples are
 *         in descending order
 */
bool
anatools::firstOfTupleDescending (tuple<size_t, size_t, string> a, tuple<size_t, size_t, string> b)
{
  return (get<0> (a) > get<0> (b));
}

/**
 * Returns whether the collection indices are in ascending order.
 *
 * This function is meant to be used with std::sort to sort a vector so that
 * the collection indices are in ascending order. The collection index is the
 * first member of the tuple in the input pairs. This sorting is used in
 * determining which combinations of objects should be ignored to avoid double
 * counting in ValueLookupTree.
 *
 * @param  a first of two objects to compare
 * @param  b second of two objects to compare
 * @return boolean representing whether the collection indices of the two
 *         objects are in ascending order
 */
bool
anatools::collectionIndexAscending (pair<string, DressedObject> a, pair<string, DressedObject> b)
{
  return (a.second.collectionIndex < b.second.collectionIndex);
}

/**
 * Retrieves all required collections from the event.
 *
 * @param  objectsToGet set of strings specifying which collections are
 *         required
 * @param  collections edm::ParameterSet giving the input tags for the
 *         collections
 * @param  handles structure containing the edm::Handle objects in which the
 *         collections are to be stored
 * @param  event edm::Event from which to get the collections
 */
void
anatools::getRequiredCollections (const unordered_set<string> &objectsToGet, const edm::ParameterSet &collections, Collections &handles, const edm::Event &event)
{
  static bool firstEvent = true;

  //////////////////////////////////////////////////////////////////////////////
  // Retrieve each object collection which we need and print a warning if it is
  // missing.
  //////////////////////////////////////////////////////////////////////////////
  if  (VEC_CONTAINS  (objectsToGet,  "beamspots")         &&  collections.exists  ("beamspots"))         getCollection  (collections.getParameter<edm::InputTag>  ("beamspots"),         handles.beamspots,         event);
  if  (VEC_CONTAINS  (objectsToGet,  "bxlumis")           &&  collections.exists  ("bxlumis"))           getCollection  (collections.getParameter<edm::InputTag>  ("bxlumis"),           handles.bxlumis,           event);
  if  (VEC_CONTAINS  (objectsToGet,  "electrons")         &&  collections.exists  ("electrons"))         getCollection  (collections.getParameter<edm::InputTag>  ("electrons"),         handles.electrons,         event);
  if  (VEC_CONTAINS  (objectsToGet,  "events")            &&  collections.exists  ("events"))            getCollection  (collections.getParameter<edm::InputTag>  ("events"),            handles.events,            event);
  if  (VEC_CONTAINS  (objectsToGet,  "genjets")           &&  collections.exists  ("genjets"))           getCollection  (collections.getParameter<edm::InputTag>  ("genjets"),           handles.genjets,           event);
  if  (VEC_CONTAINS  (objectsToGet,  "jets")              &&  collections.exists  ("jets"))              getCollection  (collections.getParameter<edm::InputTag>  ("jets"),              handles.jets,              event);
  if  (VEC_CONTAINS  (objectsToGet,  "basicjets")         &&  collections.exists  ("basicjets"))         getCollection  (collections.getParameter<edm::InputTag>  ("basicjets"),         handles.basicjets,         event);
  if  (VEC_CONTAINS  (objectsToGet,  "candeles")          &&  collections.exists  ("candeles"))          getCollection  (collections.getParameter<edm::InputTag>  ("candeles"),          handles.candeles,          event);
  if  (VEC_CONTAINS  (objectsToGet,  "generatorweights")  &&  collections.exists  ("generatorweights"))  getCollection  (collections.getParameter<edm::InputTag>  ("generatorweights"),  handles.generatorweights,  event);
  if  (VEC_CONTAINS  (objectsToGet,  "mcparticles")       &&  collections.exists  ("mcparticles"))       getCollection  (collections.getParameter<edm::InputTag>  ("mcparticles"),       handles.mcparticles,       event);
  if  (VEC_CONTAINS  (objectsToGet,  "mets")              &&  collections.exists  ("mets"))              getCollection  (collections.getParameter<edm::InputTag>  ("mets"),              handles.mets,              event);
  if  (VEC_CONTAINS  (objectsToGet,  "muons")             &&  collections.exists  ("muons"))             getCollection  (collections.getParameter<edm::InputTag>  ("muons"),             handles.muons,             event);
  if  (VEC_CONTAINS  (objectsToGet,  "photons")           &&  collections.exists  ("photons"))           getCollection  (collections.getParameter<edm::InputTag>  ("photons"),           handles.photons,           event);
  if  (VEC_CONTAINS  (objectsToGet,  "prescales")         &&  collections.exists  ("prescales"))         getCollection  (collections.getParameter<edm::InputTag>  ("prescales"),         handles.prescales,         event);
  if  (VEC_CONTAINS  (objectsToGet,  "primaryvertexs")    &&  collections.exists  ("primaryvertexs"))    getCollection  (collections.getParameter<edm::InputTag>  ("primaryvertexs"),    handles.primaryvertexs,    event);
  if  (VEC_CONTAINS  (objectsToGet,  "superclusters")     &&  collections.exists  ("superclusters"))     getCollection  (collections.getParameter<edm::InputTag>  ("superclusters"),     handles.superclusters,     event);
  if  (VEC_CONTAINS  (objectsToGet,  "taus")              &&  collections.exists  ("taus"))              getCollection  (collections.getParameter<edm::InputTag>  ("taus"),              handles.taus,              event);
  if  (VEC_CONTAINS  (objectsToGet,  "tracks")            &&  collections.exists  ("tracks"))            getCollection  (collections.getParameter<edm::InputTag>  ("tracks"),            handles.tracks,            event);
  if  (VEC_CONTAINS  (objectsToGet,  "pileupinfos")            &&  collections.exists  ("pileupinfos"))            getCollection  (collections.getParameter<edm::InputTag>  ("pileupinfos"),            handles.pileupinfos,            event);
  if  (VEC_CONTAINS  (objectsToGet,  "triggers")          &&  collections.exists  ("triggers"))          getCollection  (collections.getParameter<edm::InputTag>  ("triggers"),          handles.triggers,          event);
  if  (VEC_CONTAINS  (objectsToGet,  "trigobjs")          &&  collections.exists  ("trigobjs"))          getCollection  (collections.getParameter<edm::InputTag>  ("trigobjs"),          handles.trigobjs,          event);
  if  (VEC_CONTAINS  (objectsToGet,  "uservariables")     &&  collections.exists  ("uservariables"))
    {
      handles.uservariables.clear ();
      for (const auto &collection : collections.getParameter<vector<edm::InputTag> >  ("uservariables"))
        {
          handles.uservariables.resize (handles.uservariables.size () + 1);
          getCollection (collection, handles.uservariables.back (), event);
        }
    }
  if  (VEC_CONTAINS  (objectsToGet,  "eventvariables")   &&  collections.exists  ("eventvariables"))
    {
      handles.eventvariables.clear ();
      for (const auto &collection : collections.getParameter<vector<edm::InputTag> >  ("eventvariables"))
        {
          handles.eventvariables.resize (handles.eventvariables.size () + 1);
          getCollection (collection, handles.eventvariables.back (), event);
        }
    }




  if (firstEvent)
    clog << "Will print any collections not retrieved.  These INFO messages may be safely ignored." << endl;
  if (firstEvent && !handles.beamspots.isValid ())
    clog << "INFO: did not retrieve beamspots collection from the event." << endl;
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
  if (firstEvent && !handles.basicjets.isValid ())
    clog << "INFO: did not retrieve basicjets collection from the event." << endl;
  if (firstEvent && !handles.candeles.isValid ())
    clog << "INFO: did not retrieve candeles collection from the event." << endl;
  if (firstEvent && !handles.generatorweights.isValid ())
    clog << "INFO: did not retrieve generatorweights collection from the event." << endl;
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
  if (firstEvent && !handles.prescales.isValid ())
    clog << "INFO: did not retrieve prescales collection from the event." << endl;
  if (firstEvent && !handles.superclusters.isValid ())
    clog << "INFO: did not retrieve superclusters collection from the event." << endl;
  if (firstEvent && !handles.taus.isValid ())
    clog << "INFO: did not retrieve taus collection from the event." << endl;
  if (firstEvent && !handles.tracks.isValid ())
    clog << "INFO: did not retrieve tracks collection from the event." << endl;
  if (firstEvent && !handles.pileupinfos.isValid ())
    clog << "INFO: did not retrieve pileupinfos collection from the event." << endl;
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
            else if (dataMemberType == "unsigned long int")
              value = dataMember.get (*o).objectCast<unsigned long int> ();
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

/**
 * Returns the value of a member of an object.
 *
 * @param  type string giving the type of the object
 * @param  obj void pointer to the object
 * @param  member string giving the member, data or function, to evaluate
 * @return value of the member of the given object
 */
  double
  anatools::getMember (const string &type, const void * const obj, const string &member)
  {
    double value = numeric_limits<int>::min ();
    Reflex::Type t = Reflex::Type::ByName (type);
    Reflex::Object *o = new Reflex::Object (t, (void *) obj);
    string memberType;

    try
      {
        const Reflex::Object * const retObj = getMember (t, *o, member, memberType);
        if (memberType == "float")
          value = Reflex::Object_Cast<float> (*retObj);
        else if (memberType == "double")
          value = Reflex::Object_Cast<double> (*retObj);
        else if (memberType == "long double")
          value = Reflex::Object_Cast<long double> (*retObj);
        else if (memberType == "char")
          value = Reflex::Object_Cast<char> (*retObj);
        else if (memberType == "int")
          value = Reflex::Object_Cast<int> (*retObj);
        else if (memberType == "unsigned")
          value = Reflex::Object_Cast<unsigned> (*retObj);
        else if (memberType == "bool")
          value = Reflex::Object_Cast<bool> (*retObj);
        else if (memberType == "unsigned int")
          value = Reflex::Object_Cast<unsigned int> (*retObj);
        else if (memberType == "unsigned short int")
          value = Reflex::Object_Cast<unsigned short int> (*retObj);
        else if (memberType == "unsigned long int")
          value = Reflex::Object_Cast<unsigned long int> (*retObj);
        else
          clog << "WARNING: \"" << member << "\" has unrecognized type \"" << memberType << "\"" << endl;
        delete retObj;
      }
    catch (...)
      {
        clog << "WARNING: unable to access member \"" << member << "\" from \"" << type << "\"" << endl;
      }
    delete o;

    return value;
  }

/**
 * Helper function that returns a member of an object as a Reflex::Object.
 *
 * @param  t Reflex::Type corresponding to the type of the object
 * @param  o Reflex::Object corresponding to the object
 * @param  member string giving the member, data or function, to evaluate
 * @param  memberType string in which the type of the retrieved member is
 *         stored
 * @return Reflex::Object corresponding to the value of the member of the given
 *         object
 */
  const Reflex::Object * const
  anatools::getMember (const Reflex::Type &t, const Reflex::Object &o, const string &member, string &memberType)
  {
    string typeName = t.Name (Reflex::FINAL | Reflex::SCOPED);
    size_t dot = member.find ('.'),
           asterisk = typeName.rfind ('*');

    if (t.IsReference ())
      {
        clog << "WARNING: unable to access members which are references" << endl;
        throw 0;
      }
    if (t.IsPointer ())
      {
        Reflex::Type derefType = Reflex::Type::ByName (typeName.substr (0, asterisk) + typeName.substr (asterisk + 1));
        void *obj = o.Address ();
        Reflex::Object derefObj (derefType, (void *) *((void **) obj));
        return getMember (derefType, derefObj, member, memberType);
      }
    if (dot != string::npos)
      {
        try
          {
            const Reflex::Object * const subObj = getMember (t, o, member.substr (0, dot), memberType);
            Reflex::Type subType = Reflex::Type::ByName (memberType);
            string subMember = member.substr (dot + 1);
            const Reflex::Object * const retObj = getMember (subType, *subObj, subMember, memberType);
            delete subObj;
            return retObj;
          }
        catch (...)
          {
            const Reflex::Object * const subObj = getMember (t, o, member.substr (0, dot), memberType);
            Reflex::Type subType = Reflex::Type::ByName (memberType);
            string subMember = (member.substr (0, dot) == "operator->" ? "" : "operator->.") + member.substr (dot + 1);
            const Reflex::Object * const retObj = getMember (subType, *subObj, subMember, memberType);
            delete subObj;
            return retObj;
          }
      }

    Reflex::Type dataMemberType, functionMemberType;
    string dataMemberTypeName, functionMemberTypeName;

    dataMemberType = t.DataMemberByName (member).TypeOf ();
    functionMemberType = t.FunctionMemberByName (member).TypeOf ().ReturnType ();
    dataMemberTypeName = dataMemberType.Name (Reflex::FINAL | Reflex::SCOPED);
    functionMemberTypeName = functionMemberType.Name (Reflex::FINAL | Reflex::SCOPED);
    if (dataMemberType.IsReference () || functionMemberType.IsReference ())
      {
        clog << "WARNING: unable to access members which are references" << endl;
        throw 0;
      }
    try
      {
        if (dataMemberTypeName != "")
          {
            memberType = dataMemberTypeName;
            Reflex::Object *retObj = new Reflex::Object (o.Get (member));
            return retObj;
          }
        else if (functionMemberTypeName != "")
          {
            memberType = functionMemberTypeName;
            return invoke (memberType, o, member);
          }
        else
          throw 0;
      }
    catch (...)
      {
        for (auto bi = t.Base_Begin (); bi != t.Base_End (); bi++)
          {
            try
              {
                return getMember (bi->ToType (), o.CastObject (bi->ToType ()), member, memberType);
              }
            catch (...)
              {
                continue;
              }
          }
        throw;
      }

    throw 0;
  }

/**
 * Invokes a function member of an object.
 *
 * @param  returnType type which the function returns
 * @param  o Reflex::Object corresponding to the object whose function member
 *         is to be called
 * @param  member string giving the name of the function member
 * @return Reflex::Object corresponding to what the function returned
 */
  const Reflex::Object * const
  anatools::invoke (const string &returnType, const Reflex::Object &o, const string &member)
  {
    Reflex::Type t = Reflex::Type::ByName (returnType);
    Reflex::Object *value = new Reflex::Object (t.Construct ());
    o.Invoke (member, value);
    return value;
  }
#endif

#if IS_VALID(beamspots)
  string  anatools::getObjectType  (const  osu::Beamspot         &obj)  {  return  "beamspot";         }
  string  anatools::getObjectClass  (const  osu::Beamspot         &obj)  {  return  "osu::Beamspot";         }
#endif
#if IS_VALID(bxlumis)
  string  anatools::getObjectType  (const  osu::Bxlumi         &obj)  {  return  "bxlumi";         }
  string  anatools::getObjectClass  (const  osu::Bxlumi         &obj)  {  return  "osu::Bxlumi";         }
#endif
#if IS_VALID(electrons)
  string  anatools::getObjectType  (const  osu::Electron       &obj)  {  return  "electron";       }
  string  anatools::getObjectClass  (const  osu::Electron       &obj)  {  return  "osu::Electron";       }
#endif
#if IS_VALID(candeles)
  string  anatools::getObjectType  (const  osu::Candele       &obj)  {  return  "candele";       }
  string  anatools::getObjectClass  (const  osu::Candele       &obj)  {  return  "osu::Candele";       }
#endif
#if IS_VALID(events)
  string  anatools::getObjectType  (const  osu::Event          &obj)  {  return  "event";          }
  string  anatools::getObjectClass  (const  osu::Event          &obj)  {  return  "osu::Event";          }
  template<> int anatools::getObjectHash<osu::Event> (const osu::Event &object){
    int run, lumi, evt;
    run = abs(int(getMember (object, "run")));
    lumi = abs(int(getMember (object, "lumi")));
    evt = abs(int(getMember (object, "evt")));
    return run + lumi + evt;
  }
#endif
#if IS_VALID(genjets)
  string  anatools::getObjectType  (const  osu::Genjet         &obj)  {  return  "genjet";         }
  string  anatools::getObjectClass  (const  osu::Genjet         &obj)  {  return  "osu::Genjet";         }
#endif
#if IS_VALID(jets)
  string  anatools::getObjectType  (const  osu::Jet            &obj)  {  return  "jet";            }
  string  anatools::getObjectClass  (const  osu::Jet            &obj)  {  return  "osu::Jet";            }
#endif
#if IS_VALID(basicjets) && DATA_FORMAT == AOD
  string  anatools::getObjectType   (const  osu::Basicjet            &obj)  {  return  "basicjet";            }
  string  anatools::getObjectClass  (const  osu::Basicjet            &obj)  {  return  "osu::Basicjet";            }
#endif
#if IS_VALID(mcparticles)
  string  anatools::getObjectType  (const  osu::Mcparticle     &obj)  {  return  "mcparticle";     }
  string  anatools::getObjectClass  (const  osu::Mcparticle     &obj)  {  return  "osu::Mcparticle";     }
#endif
#if IS_VALID(mets)
  string  anatools::getObjectType  (const  osu::Met            &obj)  {  return  "met";            }
  string  anatools::getObjectClass  (const  osu::Met            &obj)  {  return  "osu::Met";            }
  template<> int anatools::getObjectHash<osu::Met> (const osu::Met& object){
    int px_mev, py_mev;
    px_mev = fabs(int(10000 * getMember (object, "px")));
    py_mev = fabs(int(10000 * getMember (object, "py")));
    return px_mev + py_mev;
  }
#endif
#if IS_VALID(muons)
  string  anatools::getObjectType  (const  osu::Muon           &obj)  {  return  "muon";           }
  string  anatools::getObjectClass  (const  osu::Muon           &obj)  {  return  "osu::Muon";           }
#endif
#if IS_VALID(photons)
  string  anatools::getObjectType  (const  osu::Photon         &obj)  {  return  "photon";         }
  string  anatools::getObjectClass  (const  osu::Photon         &obj)  {  return  "osu::Photon";         }
#endif
#if IS_VALID(primaryvertexs)
  string  anatools::getObjectType  (const  osu::Primaryvertex  &obj)  {  return  "primaryvertex";  }
  string  anatools::getObjectClass  (const  osu::Primaryvertex  &obj)  {  return  "osu::Primaryvertex";  }
  template<> int anatools::getObjectHash<osu::Primaryvertex> (const osu::Primaryvertex& object){
    int x_mum, y_mum, z_mum;
    x_mum = abs(int(10000 * getMember (object, "x")));
    y_mum = abs(int(10000 * getMember (object, "y")));
    z_mum = abs(int(10000 * getMember (object, "z")));
    return x_mum + y_mum + z_mum;
  }
#endif
#if IS_VALID(superclusters)
  string  anatools::getObjectType  (const  osu::Supercluster   &obj)  {  return  "supercluster";   }
  string  anatools::getObjectClass  (const  osu::Supercluster   &obj)  {  return  "osu::Supercluster";   }
#endif
#if IS_VALID(taus)
  string  anatools::getObjectType  (const  osu::Tau            &obj)  {  return  "tau";            }
  string  anatools::getObjectClass  (const  osu::Tau            &obj)  {  return  "osu::Tau";            }
#endif
#if IS_VALID(tracks)
  string  anatools::getObjectType  (const  osu::Track          &obj)  {  return  "track";          }
  string  anatools::getObjectClass  (const  osu::Track          &obj)  {  return  "osu::Track";          }
#endif
#if IS_VALID(pileupinfos)
  string  anatools::getObjectType  (const  osu::PileUpInfo          &obj)  {  return  "pileupinfo";          }
  string  anatools::getObjectClass  (const  osu::PileUpInfo          &obj)  {  return  "osu::PileUpInfo";          }
#endif
#if IS_VALID(trigobjs)
  string  anatools::getObjectType  (const  osu::Trigobj        &obj)  {  return  "trigobj";        }
  string  anatools::getObjectClass  (const  osu::Trigobj        &obj)  {  return  "osu::Trigobj";        }
#endif

// user-defined cases
string anatools::getObjectType (const VariableProducerPayload& obj){ return "uservariable"; }
string anatools::getObjectClass (const VariableProducerPayload& obj){ return "VariableProducerPayload"; }

string anatools::getObjectType (const EventVariableProducerPayload& obj){ return "eventvariable"; }
string anatools::getObjectClass (const EventVariableProducerPayload& obj){ return "EventVariableProducerPayload"; }

double
anatools::getGeneratorWeight (const TYPE(generatorweights) &weights)
{
#if TYPE(generatorweights) == GenEventInfoProduct
  return (weights.weight () / fabs (weights.weight ()));
#else
  return 1.0;
#endif
}
