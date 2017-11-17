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
anatools::getRequiredCollections (const unordered_set<string> &objectsToGet, Collections &handles, const edm::Event &event, const Tokens &tokens)
{
  static bool firstEvent = true;

  //////////////////////////////////////////////////////////////////////////////
  // Retrieve each object collection which we need and print a warning if it is
  // missing.
  //////////////////////////////////////////////////////////////////////////////
  if  (VEC_CONTAINS  (objectsToGet,  "beamspots") && !tokens.beamspots.isUninitialized())         event.getByToken  (tokens.beamspots,         handles.beamspots);
  if  (VEC_CONTAINS  (objectsToGet,  "bxlumis")   && !tokens.bxlumis.isUninitialized())           event.getByToken  (tokens.bxlumis,           handles.bxlumis);
  if  (VEC_CONTAINS  (objectsToGet,  "cschits")   && !tokens.cschits.isUninitialized())           event.getByToken  (tokens.cschits,           handles.cschits);
  if  (VEC_CONTAINS  (objectsToGet,  "cscsegs")   && !tokens.cscsegs.isUninitialized())           event.getByToken  (tokens.cscsegs,           handles.cscsegs);
  if  (VEC_CONTAINS  (objectsToGet,  "dtsegs")    && !tokens.dtsegs.isUninitialized())            event.getByToken  (tokens.dtsegs,            handles.dtsegs);
  if  (VEC_CONTAINS  (objectsToGet,  "electrons") && !tokens.electrons.isUninitialized())         event.getByToken  (tokens.electrons,         handles.electrons);
  if  (VEC_CONTAINS  (objectsToGet,  "events")    && !tokens.events.isUninitialized())            event.getByToken  (tokens.events,            handles.events);
  if  (VEC_CONTAINS  (objectsToGet,  "genjets")   && !tokens.genjets.isUninitialized())           event.getByToken  (tokens.genjets,           handles.genjets);
  if  (VEC_CONTAINS  (objectsToGet,  "jets")      && !tokens.jets.isUninitialized())              event.getByToken  (tokens.jets,              handles.jets);
  if  (VEC_CONTAINS  (objectsToGet,  "bjets")     && !tokens.bjets.isUninitialized())             event.getByToken  (tokens.bjets,             handles.bjets);
  if  (VEC_CONTAINS  (objectsToGet,  "basicjets") && !tokens.basicjets.isUninitialized())         event.getByToken  (tokens.basicjets,         handles.basicjets);
  if  (VEC_CONTAINS  (objectsToGet,  "generatorweights") && !tokens.generatorweights.isUninitialized())  event.getByToken  (tokens.generatorweights,  handles.generatorweights);
  if  (VEC_CONTAINS  (objectsToGet,  "mcparticles")      && !tokens.mcparticles.isUninitialized())       event.getByToken  (tokens.mcparticles,       handles.mcparticles);
  if  (VEC_CONTAINS  (objectsToGet,  "mets")             && !tokens.mets.isUninitialized())              event.getByToken  (tokens.mets,              handles.mets);
  if  (VEC_CONTAINS  (objectsToGet,  "muons")            && !tokens.muons.isUninitialized())             event.getByToken  (tokens.muons,             handles.muons);
  if  (VEC_CONTAINS  (objectsToGet,  "photons")          && !tokens.photons.isUninitialized())           event.getByToken  (tokens.photons,           handles.photons);
  if  (VEC_CONTAINS  (objectsToGet,  "prescales")        && !tokens.prescales.isUninitialized())         event.getByToken  (tokens.prescales,         handles.prescales);
  if  (VEC_CONTAINS  (objectsToGet,  "primaryvertexs")   && !tokens.primaryvertexs.isUninitialized())    event.getByToken  (tokens.primaryvertexs,    handles.primaryvertexs);
  if  (VEC_CONTAINS  (objectsToGet,  "rpchits")          && !tokens.rpchits.isUninitialized())    event.getByToken  (tokens.rpchits,    handles.rpchits);
  if  (VEC_CONTAINS  (objectsToGet,  "superclusters")    && !tokens.superclusters.isUninitialized())     event.getByToken  (tokens.superclusters,     handles.superclusters);
  if  (VEC_CONTAINS  (objectsToGet,  "taus")             && !tokens.taus.isUninitialized())              event.getByToken  (tokens.taus,              handles.taus);
  if  (VEC_CONTAINS  (objectsToGet,  "tracks")           && !tokens.tracks.isUninitialized())            event.getByToken  (tokens.tracks,            handles.tracks);
  if  (VEC_CONTAINS  (objectsToGet,  "secondaryTracks")  && !tokens.secondaryTracks.isUninitialized())   event.getByToken  (tokens.secondaryTracks,   handles.secondaryTracks);
  if  (VEC_CONTAINS  (objectsToGet,  "pileupinfos")      && !tokens.pileupinfos.isUninitialized())       event.getByToken  (tokens.pileupinfos,       handles.pileupinfos);
  if  (VEC_CONTAINS  (objectsToGet,  "triggers")         && !tokens.triggers.isUninitialized())          event.getByToken  (tokens.triggers,          handles.triggers);
  if  (VEC_CONTAINS  (objectsToGet,  "metFilters")         && !tokens.metFilters.isUninitialized())          event.getByToken  (tokens.metFilters,          handles.metFilters);
  if  (VEC_CONTAINS  (objectsToGet,  "trigobjs")         && !tokens.trigobjs.isUninitialized())          event.getByToken  (tokens.trigobjs,          handles.trigobjs);
  if  (VEC_CONTAINS  (objectsToGet,  "uservariables"))
    {
      handles.uservariables.clear ();
      for (const auto &token : tokens.uservariables)
        {
          handles.uservariables.resize (handles.uservariables.size () + 1);
          event.getByToken (token, handles.uservariables.back ());
        }
    }
  if  (VEC_CONTAINS  (objectsToGet,  "eventvariables"))
    {
      handles.eventvariables.clear ();
      for (const auto &token : tokens.eventvariables)
        {
          handles.eventvariables.resize (handles.eventvariables.size () + 1);
          event.getByToken (token, handles.eventvariables.back ());
        }
    }

  if (firstEvent)
    {
      stringstream ss;
      ss << "Will print any collections not retrieved. These INFO messages may be safely ignored.";
      if (!handles.beamspots.isValid ())
        ss << endl << "Did not retrieve beamspots collection from the event.";
      if (!handles.bxlumis.isValid ())
        ss << endl << "Did not retrieve bxlumis collection from the event.";
      if (!handles.cschits.isValid ())
        ss << endl << "Did not retrieve cschits collection from the event.";
      if (!handles.cscsegs.isValid ())
        ss << endl << "Did not retrieve cscsegs collection from the event.";
      if (!handles.dtsegs.isValid ())
        ss << endl << "Did not retrieve dtsegs collection from the event.";
      if (!handles.electrons.isValid ())
        ss << endl << "Did not retrieve electrons collection from the event.";
      if (!handles.events.isValid ())
        ss << endl << "Did not retrieve events collection from the event.";
      if (!handles.genjets.isValid ())
        ss << endl << "Did not retrieve genjets collection from the event.";
      if (!handles.jets.isValid ())
        ss << endl << "Did not retrieve jets collection from the event.";
      if (!handles.bjets.isValid ())
        ss << endl << "Did not retrieve bjets collection from the event.";
      if (!handles.basicjets.isValid ())
        ss << endl << "Did not retrieve basicjets collection from the event.";
      if (!handles.generatorweights.isValid ())
        ss << endl << "Did not retrieve generatorweights collection from the event.";
      if (!handles.mcparticles.isValid ())
        ss << endl << "Did not retrieve mcparticles collection from the event.";
      if (!handles.mets.isValid ())
        ss << endl << "Did not retrieve mets collection from the event.";
      if (!handles.muons.isValid ())
        ss << endl << "Did not retrieve muons collection from the event.";
      if (!handles.photons.isValid ())
        ss << endl << "Did not retrieve photons collection from the event.";
      if (!handles.primaryvertexs.isValid ())
        ss << endl << "Did not retrieve primaryvertexs collection from the event.";
      if (!handles.prescales.isValid ())
        ss << endl << "Did not retrieve prescales collection from the event.";
      if (!handles.rpchits.isValid ())
        ss << endl << "Did not retrieve rpchits collection from the event.";
      if (!handles.superclusters.isValid ())
        ss << endl << "Did not retrieve superclusters collection from the event.";
      if (!handles.taus.isValid ())
        ss << endl << "Did not retrieve taus collection from the event.";
      if (!handles.tracks.isValid ())
        ss << endl << "Did not retrieve tracks collection from the event.";
      if (!handles.secondaryTracks.isValid ())
        ss << endl << "Did not retrieve secondaryTracks collection from the event.";
      if (!handles.pileupinfos.isValid ())
        ss << endl << "Did not retrieve pileupinfos collection from the event.";
      if (!handles.triggers.isValid ())
        ss << endl << "Did not retrieve triggers collection from the event.";
      if (!handles.metFilters.isValid ())
        ss << endl << "Did not retrieve metFilters collection from the event.";
      if (!handles.trigobjs.isValid ())
        ss << endl << "Did not retrieve trigobjs collection from the event.";
      edm::LogInfo ("CommonUtils") << ss.str ();
    }
  //////////////////////////////////////////////////////////////////////////////

  firstEvent = false;
}

#ifdef ROOT6
  double
  anatools::getMember (const string &type, void *obj, const string &member, map<pair<string, string>, pair<string, void (*) (void *, int, void **, void *)> > * functionLookupTable)
  {
    const pair<string, string> typeAndMember (type, member);
    double value = INVALID_VALUE;
    string memberType = "";
    anatools::ObjectWithDict * retObj = NULL;
    void *retObjAdd = NULL;
    if (functionLookupTable && functionLookupTable->count (typeAndMember))
      {
        memberType = functionLookupTable->at (typeAndMember).first;
        if (memberType == "float")
          retObjAdd = (void *) new float[1];
        else if (memberType == "double")
          retObjAdd = (void *) new double[1];
        else if (memberType == "long double")
          retObjAdd = (void *) new long double[1];
        else if (memberType == "char")
          retObjAdd = (void *) new char[1];
        else if (memberType == "int")
          retObjAdd = (void *) new int[1];
        else if (memberType == "unsigned")
          retObjAdd = (void *) new unsigned[1];
        else if (memberType == "unsigned short")
          retObjAdd = (void *) new unsigned short[1];
        else if (memberType == "unsigned long")
          retObjAdd = (void *) new unsigned long[1];
        else if (memberType == "bool")
          retObjAdd = (void *) new bool[1];
        else if (memberType == "unsigned int")
          retObjAdd = (void *) new unsigned int[1];
        else if (memberType == "unsigned short int")
          retObjAdd = (void *) new unsigned short int[1];
        else if (memberType == "unsigned long int")
          retObjAdd = (void *) new unsigned long int[1];
        else if (memberType == "signed char")
          retObjAdd = (void *) new signed char[1];
        else if (memberType == "unsigned char")
          retObjAdd = (void *) new unsigned char[1];
        (*functionLookupTable->at (typeAndMember).second) (obj, 0, NULL, retObjAdd);
      }
    else
      {
        anatools::TypeWithDict t = anatools::TypeWithDict::byName (type);
        anatools::ObjectWithDict *o = new anatools::ObjectWithDict (t, obj);
        try
          {
            retObj = getMember (t, t, *o, member, memberType, functionLookupTable);
            if (retObj)
              retObjAdd = retObj->address ();
          }
        catch (...)
          {
            edm::LogInfo ("CommonUtils") << "Unable to access member \"" << member << "\" from \"" << type << "\".";
            if (o)
              delete o;
            if (retObj)
              delete retObj;
            return value;
          }
        if (o)
          delete o;
      }

    if (retObjAdd)
      {
        if (memberType == "float")
          {
            value = *((float *) retObjAdd);
            if (!retObj)
              delete[] ((float *) retObjAdd);
          }
        else if (memberType == "double")
          {
            value = *((double *) retObjAdd);
            if (!retObj)
              delete[] ((double *) retObjAdd);
          }
        else if (memberType == "long double")
          {
            value = *((long double *) retObjAdd);
            if (!retObj)
              delete[] ((long double *) retObjAdd);
          }
        else if (memberType == "char")
          {
            value = *((char *) retObjAdd);
            if (!retObj)
              delete[] ((char *) retObjAdd);
          }
        else if (memberType == "int")
          {
            value = *((int *) retObjAdd);
            if (!retObj)
              delete[] ((int *) retObjAdd);
          }
        else if (memberType == "unsigned")
          {
            value = *((unsigned *) retObjAdd);
            if (!retObj)
              delete[] ((unsigned *) retObjAdd);
          }
        else if (memberType == "unsigned short")
          {
            value = *((unsigned short *) retObjAdd);
            if (!retObj)
              delete[] ((unsigned short *) retObjAdd);
          }
        else if (memberType == "unsigned long")
          {
            value = *((unsigned long *) retObjAdd);
            if (!retObj)
              delete[] ((unsigned long *) retObjAdd);
          }
        else if (memberType == "bool")
          {
            value = *((bool *) retObjAdd);
            if (!retObj)
              delete[] ((bool *) retObjAdd);
          }
        else if (memberType == "unsigned int")
          {
            value = *((unsigned int *) retObjAdd);
            if (!retObj)
              delete[] ((unsigned int *) retObjAdd);
          }
        else if (memberType == "unsigned short int")
          {
            value = *((unsigned short int *) retObjAdd);
            if (!retObj)
              delete[] ((unsigned short int *) retObjAdd);
          }
        else if (memberType == "unsigned long int")
          {
            value = *((unsigned long int *) retObjAdd);
            if (!retObj)
              delete[] ((unsigned long int *) retObjAdd);
          }
        else if (memberType == "signed char")
          {
            value = *((signed char *) retObjAdd);
            if (!retObj)
              delete[] ((signed char *) retObjAdd);
          }
        else if (memberType == "unsigned char")
          {
            value = *((unsigned char *) retObjAdd);
            if (!retObj)
              delete[] ((unsigned char *) retObjAdd);
          }
        else
          edm::LogWarning ("CommonUtils") << "\"" << member << "\" has unrecognized type \"" << memberType << "\".";
        if (retObj)
          delete retObj;
      }
    else
      edm::LogInfo ("CommonUtils") << "Unable to access member \"" << member << "\" from \"" << type << "\".";

    return value;
  }

  anatools::ObjectWithDict *
  anatools::getMember (const anatools::TypeWithDict &tDerived, const anatools::TypeWithDict &t, const anatools::ObjectWithDict &o, const string &member, string &memberType, map<pair<string, string>, pair<string, void (*) (void *, int, void **, void *)> > * functionLookupTable)
  {
    string typeName = t.name ();
    size_t dot = member.find ('.'),
           asterisk = typeName.rfind ('*');

    if (t.isReference ())
      {
        edm::LogWarning ("CommonUtils") << "Unable to access members which are references.";
        throw 0;
      }
    if (t.isPointer ())
      {
        anatools::TypeWithDict derefType = anatools::TypeWithDict::byName (typeName.substr (0, asterisk) + typeName.substr (asterisk + 1));
        void *obj = o.address ();
        anatools::ObjectWithDict derefObj (derefType, (void *) *((void **) obj));
        return getMember (tDerived, derefType, derefObj, member, memberType, NULL);
      }
    if (t.name ().find ("edm::Ref") == 0 && member == "operator->")
      {
        anatools::ObjectWithDict * isValid = invoke ("bool", o, t.functionMemberByName ("isNonnull"));
        functionLookupTable = NULL;
        if (!isValid->objectCast<bool> ())
          {
            delete isValid;
            return NULL;
          }
        delete isValid;
      }
    if (dot != string::npos)
      {
        anatools::ObjectWithDict * subObj = getMember (tDerived, t, o, member.substr (0, dot), memberType, NULL);
        if (!subObj)
          return NULL;
        anatools::TypeWithDict subType = anatools::TypeWithDict::byName (memberType);
        string subMember = member.substr (dot + 1);
        anatools::ObjectWithDict * retObj = getMember (tDerived, subType, *subObj, subMember, memberType, NULL);
        if (retObj)
          {
            delete subObj;
            return retObj;
          }

        subMember = (member.substr (0, dot) == "operator->" ? "" : "operator->.") + member.substr (dot + 1);
        retObj = getMember (tDerived, subType, *subObj, subMember, memberType, NULL);
        delete subObj;
        return retObj;
      }

    anatools::MemberWithDict dataMember;
    anatools::FunctionWithDict functionMember;
    bool isReference = false;

    dataMember = t.dataMemberByName (member);
    functionMember = t.functionMemberByName (member);
    if (dataMember)
      {
        memberType = dataMember.typeOf ().name ();
        isReference = dataMember.typeOf ().isReference ();
      }
    else if (functionMember)
      {
        memberType = functionMember.finalReturnType ().name ();
        isReference = functionMember.finalReturnType ().isReference ();
      }
    if (isReference)
      {
        edm::LogWarning ("CommonUtils") << "Unable to access members which are references.";
        throw 0;
      }
    if (dataMember)
      return (new anatools::ObjectWithDict (o.get (member)));
    else if (functionMember)
      {
        if (functionLookupTable)
          (*functionLookupTable)[make_pair (tDerived.name (), member)] = make_pair (memberType, functionMember.address ());
        return invoke (memberType, o, functionMember);
      }
    else
      {
        anatools::TypeBases bases (t);
        for (auto bi = bases.begin (); bi != bases.end (); ++bi)
          {
            anatools::BaseWithDict base (*bi);
            anatools::ObjectWithDict * retObj = getMember (tDerived, base.typeOf (), o.castObject (base.typeOf ()), member, memberType, functionLookupTable);
            if (retObj)
              return retObj;
            continue;
          }
      }

    return NULL;
  }

  anatools::ObjectWithDict *
  anatools::invoke (const string &returnType, const anatools::ObjectWithDict &o, const anatools::FunctionWithDict &f)
  {
    anatools::TypeWithDict t = anatools::TypeWithDict::byName (returnType);
    anatools::ObjectWithDict * value = new anatools::ObjectWithDict (t.construct ());
    f.invoke (o, value);
    return value;
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
    double value = INVALID_VALUE;
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
        else if (memberType == "signed char")
          value = Reflex::Object_Cast<signed char> (*retObj);
        else if (memberType == "unsigned char")
          value = Reflex::Object_Cast<unsigned char> (*retObj);
        else
          edm::LogWarning ("CommonUtils") << "\"" << member << "\" has unrecognized type \"" << memberType << "\".";
        delete retObj;
      }
    catch (...)
      {
        edm::LogInfo ("CommonUtils") << "Unable to access member \"" << member << "\" from \"" << type << "\".";
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
        edm::LogWarning ("CommonUtils") << "Unable to access members which are references.";
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
        edm::LogWarning ("CommonUtils") << "Unable to access members which are references.";
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
#if IS_VALID(cschits)
  string  anatools::getObjectType  (const  osu::Cschit         &obj)  {  return  "cschit";         }
  string  anatools::getObjectClass  (const  osu::Cschit         &obj)  {  return  "osu::Cschit";         }
#endif
#if IS_VALID(cscsegs)
  string  anatools::getObjectType  (const  osu::Cscseg         &obj)  {  return  "cscseg";         }
  string  anatools::getObjectClass  (const  osu::Cscseg         &obj)  {  return  "osu::Cscseg";         }
#endif
#if IS_VALID(dtsegs)
  string  anatools::getObjectType  (const  osu::Dtseg         &obj)  {  return  "dtseg";         }
  string  anatools::getObjectClass  (const  osu::Dtseg         &obj)  {  return  "osu::Dtseg";         }
#endif
#if IS_VALID(electrons)
  string  anatools::getObjectType  (const  osu::Electron       &obj)  {  return  "electron";       }
  string  anatools::getObjectClass  (const  osu::Electron       &obj)  {  return  "osu::Electron";       }
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
#if IS_VALID(bjets)
  string  anatools::getObjectType  (const  osu::Bjet            &obj)  {  return  "bjet";            }
  string  anatools::getObjectClass  (const  osu::Bjet            &obj)  {  return  "osu::Bjet";            }
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
#if IS_VALID(rpchits)
  string  anatools::getObjectType  (const  osu::Rpchit   &obj)  {  return  "rpchit";   }
  string  anatools::getObjectClass  (const  osu::Rpchit   &obj)  {  return  "osu::Rpchit";   }
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
#if IS_VALID(secondaryTracks)
  string  anatools::getObjectType  (const  osu::SecondaryTrack          &obj)  {  return  "secondaryTrack";          }
  string  anatools::getObjectClass  (const  osu::SecondaryTrack          &obj)  {  return  "osu::SecondaryTrack";          }
#endif
#if IS_VALID(pileupinfos)
  string  anatools::getObjectType  (const  osu::PileUpInfo          &obj)  {  return  "pileupinfo";          }
  string  anatools::getObjectClass  (const  osu::PileUpInfo          &obj)  {  return  "osu::PileUpInfo";          }
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

void
anatools::getAllTokens (const edm::ParameterSet &collections, edm::ConsumesCollector &&cc, Tokens &tokens)
{
  if (collections.exists ("bxlumis"))
    tokens.bxlumis = cc.consumes<vector<osu::Bxlumi> > (collections.getParameter<edm::InputTag> ("bxlumis"));
  if (collections.exists ("cschits"))
    tokens.cschits = cc.consumes<vector<osu::Cschit> > (collections.getParameter<edm::InputTag> ("cschits"));
  if (collections.exists ("cscsegs"))
    tokens.cscsegs = cc.consumes<vector<osu::Cscseg> > (collections.getParameter<edm::InputTag> ("cscsegs"));
  if (collections.exists ("dtsegs"))
    tokens.dtsegs = cc.consumes<vector<osu::Dtseg> > (collections.getParameter<edm::InputTag> ("dtsegs"));
  if (collections.exists ("electrons"))
    tokens.electrons = cc.consumes<vector<osu::Electron> > (collections.getParameter<edm::InputTag> ("electrons"));
  if (collections.exists ("events"))
    tokens.events = cc.consumes<vector<osu::Event> > (collections.getParameter<edm::InputTag> ("events"));
  if (collections.exists ("genjets"))
    tokens.genjets = cc.consumes<vector<osu::Genjet> > (collections.getParameter<edm::InputTag> ("genjets"));
  if (collections.exists ("jets"))
    tokens.jets = cc.consumes<vector<osu::Jet> > (collections.getParameter<edm::InputTag> ("jets"));
  if (collections.exists ("bjets"))
    tokens.bjets = cc.consumes<vector<osu::Bjet> > (collections.getParameter<edm::InputTag> ("bjets"));
  if (collections.exists ("basicjets"))
    tokens.basicjets = cc.consumes<vector<osu::Basicjet> > (collections.getParameter<edm::InputTag> ("basicjets"));
  if (collections.exists ("mcparticles"))
    tokens.mcparticles = cc.consumes<vector<osu::Mcparticle> > (collections.getParameter<edm::InputTag> ("mcparticles"));
  if (collections.exists ("mets"))
    tokens.mets = cc.consumes<vector<osu::Met> > (collections.getParameter<edm::InputTag> ("mets"));
  if (collections.exists ("muons"))
    tokens.muons = cc.consumes<vector<osu::Muon> > (collections.getParameter<edm::InputTag> ("muons"));
  if (collections.exists ("photons"))
    tokens.photons = cc.consumes<vector<osu::Photon> > (collections.getParameter<edm::InputTag> ("photons"));
  if (collections.exists ("primaryvertexs"))
    tokens.primaryvertexs = cc.consumes<vector<osu::Primaryvertex> > (collections.getParameter<edm::InputTag> ("primaryvertexs"));
  if (collections.exists ("rpchits"))
    tokens.rpchits = cc.consumes<vector<osu::Rpchit> > (collections.getParameter<edm::InputTag> ("rpchits"));
  if (collections.exists ("superclusters"))
    tokens.superclusters = cc.consumes<vector<osu::Supercluster> > (collections.getParameter<edm::InputTag> ("superclusters"));
  if (collections.exists ("taus"))
    tokens.taus = cc.consumes<vector<osu::Tau> > (collections.getParameter<edm::InputTag> ("taus"));
  if (collections.exists ("tracks"))
    tokens.tracks = cc.consumes<vector<osu::Track> > (collections.getParameter<edm::InputTag> ("tracks"));
  if (collections.exists ("secondaryTracks"))
    tokens.secondaryTracks = cc.consumes<vector<osu::SecondaryTrack> > (collections.getParameter<edm::InputTag> ("secondaryTracks"));
  if (collections.exists ("pileupinfos"))
    tokens.pileupinfos = cc.consumes<vector<osu::PileUpInfo> > (collections.getParameter<edm::InputTag> ("pileupinfos"));

  if (collections.exists ("beamspots"))
    tokens.beamspots = cc.consumes<osu::Beamspot> (collections.getParameter<edm::InputTag> ("beamspots"));
  if (collections.exists ("generatorweights"))
    tokens.generatorweights = cc.consumes<TYPE(generatorweights)> (collections.getParameter<edm::InputTag> ("generatorweights"));
  if (collections.exists ("prescales"))
    tokens.prescales = cc.consumes<TYPE(prescales)> (collections.getParameter<edm::InputTag> ("prescales"));
  if (collections.exists ("triggers"))
    tokens.triggers = cc.consumes<TYPE(triggers)> (collections.getParameter<edm::InputTag> ("triggers"));
  if (collections.exists ("trigobjs"))
    tokens.trigobjs = cc.consumes<vector<TYPE(trigobjs)> > (collections.getParameter<edm::InputTag> ("trigobjs"));
  if (collections.exists ("metFilters"))
    tokens.metFilters = cc.consumes<TYPE(triggers)> (collections.getParameter<edm::InputTag> ("metFilters"));

  if (collections.exists ("uservariables"))
    {
      tokens.uservariables.clear ();
      for (const auto &collection : collections.getParameter<vector<edm::InputTag> > ("uservariables"))
        tokens.uservariables.push_back (cc.consumes<osu::Uservariable> (collection));
    }
  if (collections.exists ("eventvariables"))
    {
      tokens.eventvariables.clear ();
      for (const auto &collection : collections.getParameter<vector<edm::InputTag> > ("eventvariables"))
        tokens.eventvariables.push_back (cc.consumes<osu::Eventvariable> (collection));
    }
}

bool
anatools::getTriggerObjects (const edm::Event &event, const edm::TriggerResults &triggers, const vector<pat::TriggerObjectStandAlone> &trigObjs, const string &collection, const string &filter, vector<const pat::TriggerObjectStandAlone *> &selectedTrigObjs)
{
  if (collection == "")
    {
      selectedTrigObjs.push_back (NULL);
      return false;
    }
  vector<const pat::TriggerObjectStandAlone *> trigObjsToAdd;
  int i = -1;
  for (auto trigObj : trigObjs)
    {
      i++;
#if CMSSW_VERSION_CODE >= CMSSW_VERSION(9,2,0)
      trigObj.unpackNamesAndLabels(event, triggers);
#else
      trigObj.unpackPathNames(event.triggerNames(triggers));
#endif
      if (trigObj.collection () != collection)
        continue;
      if (filter != "")
        {
          bool flag = false;
          for (const auto &filterLabel : trigObj.filterLabels ())
            if (filterLabel == filter)
              {
                flag = true;
                break;
              }
          if (!flag)
            continue;
        }

      trigObjsToAdd.push_back (&trigObjs.at (i));
    }
  if (!trigObjsToAdd.empty ())
    selectedTrigObjs.insert (selectedTrigObjs.end (), trigObjsToAdd.begin (), trigObjsToAdd.end ());
  else
    selectedTrigObjs.push_back (NULL);
  return (!trigObjsToAdd.empty ());
}

bool
anatools::getTriggerObjectsByFilterPrefix (const edm::Event &event, const edm::TriggerResults &triggers, const vector<pat::TriggerObjectStandAlone> &trigObjs, const string &collection, const string &filterPrefix, vector<const pat::TriggerObjectStandAlone *> &selectedTrigObjs)
{
  if (collection == "")
    {
      selectedTrigObjs.push_back (NULL);
      return false;
    }
  vector<const pat::TriggerObjectStandAlone *> trigObjsToAdd;
  int i = -1;
  for (auto trigObj : trigObjs)
    {
      i++;
#if CMSSW_VERSION_CODE >= CMSSW_VERSION(9,2,0)
      trigObj.unpackNamesAndLabels(event, triggers);
#else
      trigObj.unpackPathNames(event.triggerNames(triggers));
#endif
      if (trigObj.collection () != collection)
        continue;
      if (filterPrefix != "")
        {
          bool flag = false;
          for (const auto &filterLabel : trigObj.filterLabels ())
            if (filterLabel.find (filterPrefix) == 0)
              {
                flag = true;
                break;
              }
          if (!flag)
            continue;
        }

      trigObjsToAdd.push_back (&trigObjs.at (i));
    }
  if (!trigObjsToAdd.empty ())
    selectedTrigObjs.insert (selectedTrigObjs.end (), trigObjsToAdd.begin (), trigObjsToAdd.end ());
  else
    selectedTrigObjs.push_back (NULL);
  return (!trigObjsToAdd.empty ());
}

bool
anatools::triggerObjectExists (const edm::Event &event, const edm::TriggerResults &triggers, const vector<pat::TriggerObjectStandAlone> &trigObjs, const string &collection, const string &filter)
{
  if (collection == "")
    return false;
  for (auto trigObj : trigObjs)
    {
#if CMSSW_VERSION_CODE >= CMSSW_VERSION(9,2,0)
      trigObj.unpackNamesAndLabels(event, triggers);
#else
      trigObj.unpackPathNames(event.triggerNames(triggers));
#endif
      if (trigObj.collection () != collection)
        continue;
      if (filter != "")
        {
          bool flag = false;
          for (const auto &filterLabel : trigObj.filterLabels ())
            if (filterLabel == filter)
              {
                flag = true;
                break;
              }
          if (!flag)
            continue;
        }

      return true;
    }
  return false;
}

bool
anatools::passesL1ETM (const edm::Event &event, const edm::TriggerResults &triggers, const vector<pat::TriggerObjectStandAlone> &trigObjs)
{
  for (auto trigObj : trigObjs)
    {
#if CMSSW_VERSION_CODE >= CMSSW_VERSION(9,2,0)
      trigObj.unpackNamesAndLabels(event, triggers);
#else
      trigObj.unpackPathNames(event.triggerNames(triggers));
#endif
      for (const auto &filterLabel : trigObj.filterLabels ())
        if (filterLabel.find ("hltL1sETM") == 0 && filterLabel.find ("Jet") == string::npos)
          return true;
    }
  return false;
}

void
anatools::logSpace (const double a, const double b, const unsigned n, vector<double> &bins)
{
  double step = (b - a) / ((double) n);

  bins.clear ();
  for (double i = a; i < b + 0.5 * step; i += step)
    bins.push_back (pow (10.0, i));
}

void
anatools::linSpace (const double a, const double b, const unsigned n, vector<double> &bins)
{
  double step = (b - a) / ((double) n);

  bins.clear ();
  for (double i = a; i < b + 0.5 * step; i += step)
    bins.push_back (i);
}
