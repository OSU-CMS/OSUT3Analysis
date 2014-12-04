#include <iostream>
#include <algorithm>

#include "Reflex/Base.h"
#include "Reflex/Member.h"
#include "Reflex/Object.h"
#include "Reflex/Type.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "OSUT3Analysis/AnaTools/interface/ValueLookupTree.h"

ValueLookupTree::ValueLookupTree () :
  root_ (NULL),
  evaluationError_ (false)
{
}

ValueLookupTree::ValueLookupTree (const Cut &cut) :
  root_ (insert_ (cut.cutString, NULL)),
  inputCollections_ (cut.inputCollections),
  evaluationError_ (false)
{
  pruneCommas (root_);
  pruneParentheses (root_);

  vector<string> inferredInputCollections;
  inferInputCollections (root_, inferredInputCollections);
  if (inferredInputCollections.size ())
    inputCollections_ = inferredInputCollections;
  sort (inputCollections_.begin (), inputCollections_.end ());
}

ValueLookupTree::ValueLookupTree (const string &expression, const vector<string> &inputCollections) :
  root_ (insert_ (expression, NULL)),
  inputCollections_ (inputCollections),
  evaluationError_ (false)
{
  pruneCommas (root_);
  pruneParentheses (root_);

  vector<string> inferredInputCollections;
  inferInputCollections (root_, inferredInputCollections);
  if (inferredInputCollections.size ())
    inputCollections_ = inferredInputCollections;
  sort (inputCollections_.begin (), inputCollections_.end ());
}

ValueLookupTree::~ValueLookupTree ()
{
  destroy (root_);
}

string
ValueLookupTree::capitalize (string input)
{
  input.front () = toupper (input.front ());
  return input;
}

string
ValueLookupTree::singular (string input)
{
  if (tolower (input.back ()) == 's')
    return input.substr (0, input.size () - 1);
  else
    return input;
}

string
ValueLookupTree::plural (string input)
{
  if (tolower (input.back ()) == 's')
    return input;
  else
    return input + "s";
}

void
ValueLookupTree::pruneCommas (Node * const tree) const
{
  bool foundComma;
  do
    {
      foundComma = false;
      for (auto branch = tree->branches.begin (); branch != tree->branches.end (); branch++)
        {
          if ((*branch)->value == ",")
            {
              foundComma = true;

              size_t comma = branch - tree->branches.begin ();
              tree->branches.insert (branch + 1, (*branch)->branches.begin (), (*branch)->branches.end ());
              branch = tree->branches.begin () + comma;
              tree->branches.erase (branch);
              branch = tree->branches.begin () + comma + 1;
            }
        }
    }
  while (foundComma);

  for (const auto &branch : tree->branches)
    pruneCommas (branch);
}

void
ValueLookupTree::pruneParentheses (Node * const tree) const
{
  bool foundParenthesis;
  do
    {
      foundParenthesis = false;
      for (auto branch = tree->branches.begin (); branch != tree->branches.end (); branch++)
        {
          if ((*branch)->value == "()")
            {
              foundParenthesis = true;

              size_t parenthesis = branch - tree->branches.begin ();
              tree->branches.insert (branch + 1, (*branch)->branches.begin (), (*branch)->branches.end ());
              branch = tree->branches.begin () + parenthesis;
              tree->branches.erase (branch);
              branch = tree->branches.begin () + parenthesis;
            }
        }
    }
  while (foundParenthesis);

  for (const auto &branch : tree->branches)
    pruneParentheses (branch);
}

void
ValueLookupTree::inferInputCollections (const Node * const tree, vector<string> &inputCollections) const
{
  if (tree->branches.size ())
    {
      for (const auto &branch : tree->branches)
        inferInputCollections (branch, inputCollections);
    }
  else if (isCollection (tree->value + "s"))
    inputCollections.push_back (tree->value + "s");
}

const Collections * const
ValueLookupTree::setCollections (Collections * const handles)
{
  handles_ = handles;
  values_.clear ();
  nCombinations_.clear ();
  collectionSizes_.clear ();
  nCombinations_.assign (inputCollections_.size (), 1);
  for (auto collection = inputCollections_.begin (); collection != inputCollections_.end (); collection++)
    {
      unsigned currentSize = getCollectionSize (*collection);
      for (unsigned i = 0; i < (collection - inputCollections_.begin () + 1); i++)
        nCombinations_[i] *= currentSize;
      collectionSizes_.push_back (currentSize);
    }

  return handles_;
}

unsigned
ValueLookupTree::getLocalIndex (unsigned globalIndex, unsigned collectionIndex) const
{
  if (collectionIndex + 1 != inputCollections_.size ())
    return ((globalIndex / nCombinations_.at (collectionIndex + 1)) % collectionSizes_.at (collectionIndex));
  else
    return (globalIndex % collectionSizes_.at (collectionIndex));
}

vector<string>
ValueLookupTree::getSingleObjects (string inputLabel)
{
  //////////////////////////////////////////////////////////////////////////////
  // Extracts the names of the two object types from the name of a pair.
  //////////////////////////////////////////////////////////////////////////////
  vector<string> singleObjects;
  size_t hyphen;
  while ((hyphen = inputLabel.find ('-')) != string::npos)
    {
      singleObjects.push_back (plural (inputLabel.substr (0, hyphen)));
      inputLabel = inputLabel.substr (hyphen + 1);
    }
  singleObjects.push_back (plural (inputLabel));
  sort (singleObjects.begin (), singleObjects.end ());
  //////////////////////////////////////////////////////////////////////////////

  return singleObjects;
}

set<unsigned>
ValueLookupTree::getGlobalIndices (unsigned localIndex, const string &singleObjectCollection, string inputLabel) const
{
  vector<string> singleObjects = getSingleObjects (inputLabel);
  set<unsigned> globalIndices;
  vector<unsigned> nCombinations (singleObjects.size (), 1), collectionSizes;
  unordered_set<unsigned> singleObjectIndices;
  for (auto collection = singleObjects.begin (); collection != singleObjects.end (); collection++)
    {
      unsigned currentSize = getCollectionSize (*collection);
      for (unsigned i = 0; i < (collection - singleObjects.begin () + 1); i++)
        nCombinations[i] *= currentSize;
      collectionSizes.push_back (currentSize);
      if (*collection == singleObjectCollection)
        singleObjectIndices.insert (collection - singleObjects.begin ());
    }
  for (const auto &singleObjectIndex : singleObjectIndices)
    {
      for (unsigned i = 0; i < nCombinations.at (0); i++)
        {
          if (singleObjectIndex + 1 < singleObjects.size ())
            {
              if ((i / nCombinations.at (singleObjectIndex + 1)) % collectionSizes.at (singleObjectIndex) == localIndex)
                globalIndices.insert (i);
            }
          else
            {
              if (i % collectionSizes.at (singleObjectIndex) == localIndex)
                globalIndices.insert (i);
            }
        }
    }

  return globalIndices;
}

bool
ValueLookupTree::isValid () const
{
  return (root_ != NULL);
}

bool
ValueLookupTree::evaluationError () const
{
  return evaluationError_;
}

void
ValueLookupTree::insert (const string &cut)
{
  root_ = insert_ (cut, NULL);
}

void
ValueLookupTree::destroy (Node * const x) const
{
  for (const auto &branch : x->branches)
    destroy (branch);
  delete x;
}

Node *
ValueLookupTree::insert_ (const string &cut, Node * const parent) const
{
  //////////////////////////////////////////////////////////////////////////////
  // If the given string is missing parentheses, simply print an error and
  // return NULL. The user can then use the isValid() method to see if
  // inserting the expression into the tree was successful.
  //////////////////////////////////////////////////////////////////////////////
  if (splitParentheses (cut))
    {
      clog << "ERROR: missing parentheses in \"" << cut << "\"" << endl;
      return NULL;
    }
  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  // Try inserting nodes for each possible operator, in reverse order of
  // operations. If none of the operators can be inserted, then it means we
  // have hit a leaf.
  //////////////////////////////////////////////////////////////////////////////
  Node *tree = new Node;
  tree->parent = parent;
  if (!(insertBinaryInfixOperator  (cut,  tree,  {","})                           ||
        insertBinaryInfixOperator  (cut,  tree,  {"||", "|"})                     ||
        insertBinaryInfixOperator  (cut,  tree,  {"&&", "&"})                     ||
        insertBinaryInfixOperator  (cut,  tree,  {"==", "!=", "="}, {"<=", ">="}) ||
        insertBinaryInfixOperator  (cut,  tree,  {"<", "<=", ">", ">="})          ||
        insertBinaryInfixOperator  (cut,  tree,  {"+", "-"})                      ||
        insertBinaryInfixOperator  (cut,  tree,  {"*", "/", "%"})                 ||
        insertUnaryPrefixOperator  (cut,  tree,  {"!"})                           ||
        insertUnaryPrefixOperator  (cut,  tree,  {"cos", "sin", "tan", "acos", "asin", "atan", "atan2",
                                                  "cosh", "sinh", "tanh", "acosh", "asinh", "atanh",
                                                  "exp", "ldexp", "log", "log10", "exp2", "expm1", "ilogb", "log1p", "log2", "logb",
                                                  "pow", "sqrt", "cbrt", "hypot",
                                                  "erf", "erfc", "tgamma", "lgamma",
                                                  "ceil", "floor", "fmod", "trunc", "round", "rint", "nearbyint", "remainder", "abs", "fabs",
                                                  "copysign", "nextafter",
                                                  "fdim", "fmax", "fmin", "max", "min"}) ||
        insertUnaryPrefixOperator  (cut,  tree,  {"deltaPhi", "deltaR", "invMass"}) ||
        insertBinaryInfixOperator  (cut,  tree,  {"."})                           ||
        insertParentheses          (cut,  tree)))
    tree->value = cut;
  //////////////////////////////////////////////////////////////////////////////

  return tree;
}

void *
ValueLookupTree::getObject (const string &name, const unsigned i) const
{
  if (name == "bxlumis")
    {
      BNbxlumi *obj = new BNbxlumi (handles_->bxlumis->at (i));
      return obj;
    }
  else if (name == "electrons")
    {
      BNelectron *obj = new BNelectron (handles_->electrons->at (i));
      return obj;
    }
  else if (name == "events")
    {
      BNevent *obj = new BNevent (handles_->events->at (i));
      return obj;
    }
  else if (name == "genjets")
    {
      BNgenjet *obj = new BNgenjet (handles_->genjets->at (i));
      return obj;
    }
  else if (name == "jets")
    {
      BNjet *obj = new BNjet (handles_->jets->at (i));
      return obj;
    }
  else if (name == "mcparticles")
    {
      BNmcparticle *obj = new BNmcparticle (handles_->mcparticles->at (i));
      return obj;
    }
  else if (name == "mets")
    {
      BNmet *obj = new BNmet (handles_->mets->at (i));
      return obj;
    }
  else if (name == "muons")
    {
      BNmuon *obj = new BNmuon (handles_->muons->at (i));
      return obj;
    }
  else if (name == "photons")
    {
      BNphoton *obj = new BNphoton (handles_->photons->at (i));
      return obj;
    }
  else if (name == "primaryvertexs")
    {
      BNprimaryvertex *obj = new BNprimaryvertex (handles_->primaryvertexs->at (i));
      return obj;
    }
  else if (name == "superclusters")
    {
      BNsupercluster *obj = new BNsupercluster (handles_->superclusters->at (i));
      return obj;
    }
  else if (name == "taus")
    {
      BNtau *obj = new BNtau (handles_->taus->at (i));
      return obj;
    }
  else if (name == "tracks")
    {
      BNtrack *obj = new BNtrack (handles_->tracks->at (i));
      return obj;
    }
  else if (name == "trigobjs")
    {
      BNtrigobj *obj = new BNtrigobj (handles_->trigobjs->at (i));
      return obj;
    }
  else if (name == "userVariables")
    {
      map<string, double> *obj = new map<string, double> (handles_->userVariables->at (i));
      return obj;
    }
  return NULL;
}

void
ValueLookupTree::deleteObject (const string &name, void * const obj) const
{
  if (name == "bxlumis")
    delete ((BNbxlumi *) obj);
  else if (name == "electrons")
    delete ((BNelectron *) obj);
  else if (name == "events")
    delete ((BNevent *) obj);
  else if (name == "genjets")
    delete ((BNgenjet *) obj);
  else if (name == "jets")
    delete ((BNjet *) obj);
  else if (name == "mcparticles")
    delete ((BNmcparticle *) obj);
  else if (name == "mets")
    delete ((BNmet *) obj);
  else if (name == "muons")
    delete ((BNmuon *) obj);
  else if (name == "photons")
    delete ((BNphoton *) obj);
  else if (name == "primaryvertexs")
    delete ((BNprimaryvertex *) obj);
  else if (name == "superclusters")
    delete ((BNsupercluster *) obj);
  else if (name == "taus")
    delete ((BNtau *) obj);
  else if (name == "tracks")
    delete ((BNtrack *) obj);
  else if (name == "trigobjs")
    delete ((BNtrigobj *) obj);
  else if (name == "userVariables")
    delete ((map<string, double> *) obj);
}

unsigned
ValueLookupTree::getCollectionSize (const string &name) const
{
  if (name == "bxlumis")
    return handles_->bxlumis->size ();
  else if (name == "electrons")
    return handles_->electrons->size ();
  else if (name == "events")
    return handles_->events->size ();
  else if (name == "genjets")
    return handles_->genjets->size ();
  else if (name == "jets")
    return handles_->jets->size ();
  else if (name == "mcparticles")
    return handles_->mcparticles->size ();
  else if (name == "mets")
    return handles_->mets->size ();
  else if (name == "muons")
    return handles_->muons->size ();
  else if (name == "photons")
    return handles_->photons->size ();
  else if (name == "primaryvertexs")
    return handles_->primaryvertexs->size ();
  else if (name == "superclusters")
    return handles_->superclusters->size ();
  else if (name == "taus")
    return handles_->taus->size ();
  else if (name == "tracks")
    return handles_->tracks->size ();
  else if (name == "trigobjs")
    return handles_->trigobjs->size ();
  else if (name == "userVariables")
    return handles_->userVariables->size ();
  return 0;
}

const vector<Leaf> &
ValueLookupTree::evaluate ()
{
  if (!values_.size ())
    {
      evaluationError_ = false;
      for (unsigned i = 0; i < nCombinations_.at (0); i++)
        {
          objIterators_.clear ();
          shouldIterate_.clear ();
          ObjMap objs;
          unordered_set<string> keys;
          for (auto collection = inputCollections_.begin (); collection != inputCollections_.end (); collection++)
            {
              unsigned j = collection - inputCollections_.begin (),
                       localIndex = getLocalIndex (i, j);
              objs.insert ({*collection, make_tuple (j, localIndex, getObject (*collection, localIndex))});
              keys.insert (*collection);
            }
          if (isUniqueCase (objs, keys))
            values_.push_back (evaluate_ (root_, objs));
          else
            values_.push_back (numeric_limits<int>::min ());
          for (const auto &obj : objs)
            deleteObject (obj.first, get<2> (obj.second));
        }
    }

  return values_;
}

bool
ValueLookupTree::collectionIndexAscending (pair<string, tuple<unsigned, unsigned, void *> > a, pair<string, tuple<unsigned, unsigned, void *> > b)
{
  return (get<0> (a.second) < get<0> (b.second));
}

bool
ValueLookupTree::isUniqueCase (const ObjMap &objs, const unordered_set<string> &keys) const
{
  bool pass = true;

  for (const auto &key : keys)
    {
      if (objs.count (key) <= 1)
        continue;
      else
        {
          auto range = objs.equal_range (key);
          vector<pair<string, tuple<unsigned, unsigned, void *> > > objsOfThisType (range.first, range.second);
          sort (objsOfThisType.begin (), objsOfThisType.end (), ValueLookupTree::collectionIndexAscending);
          int previousLocalIndex = -1;
          for (const auto &obj : objsOfThisType)
            {
              pass = pass && ((int) get<1> (obj.second) > previousLocalIndex);
              previousLocalIndex = get<1> (obj.second);
            }
        }
    }

  return pass;
}

Leaf
ValueLookupTree::evaluate_ (const Node * const tree, const ObjMap &objs)
{
  //////////////////////////////////////////////////////////////////////////////
  // Do nothing if the tree is null.
  //////////////////////////////////////////////////////////////////////////////
  if (!tree)
    return numeric_limits<int>::min ();
  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  // The node is not a leaf and its value is an operator. First, evaluate its
  // daughters, then return the result of the operator acting on the daughters.
  //////////////////////////////////////////////////////////////////////////////
  if (tree->branches.size ())
    {
      vector<Leaf> operands;
      for (const auto &branch : tree->branches)
        operands.push_back (evaluate_ (branch, objs));
      return evaluateOperator (tree->value, operands, objs);
    }
  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  // The node is a leaf and its value is either a valueLookup variable or a
  // number. Return the result of the valueLookup function or simply return the
  // number.
  //////////////////////////////////////////////////////////////////////////////
  else
    {
      double value;
      if (isnumber (tree->value, value))
        return value;
      else if (isCollection (tree->value + "s") || (tree->parent && tree->parent->value == "."))
        return tree->value;
      else
        {
          if (inputCollections_.size () == 1)
            return valueLookup (inputCollections_.at (0), objs, tree->value);
          clog << "ERROR: cannot infer ownership of \"" << tree->value << "\"" << endl;
          evaluationError_ = true;
          return numeric_limits<int>::min ();
        }
    }
  //////////////////////////////////////////////////////////////////////////////
}

Leaf
ValueLookupTree::evaluateOperator (const string &op, const vector<Leaf> &operands, const ObjMap &objs)
{
  // Tries to return the result of operating on the operands. Prints out a
  // warning, sets evaluationError_ to true, and returns the minimum unsigned
  // integer if there is a problem.

  try
    {
      if (op == "||" || op == "|")
        return (boost::get<double> (operands.at (0)) || boost::get<double> (operands.at (1)));
      else if (op == "&&" || op == "&")
        return (boost::get<double> (operands.at (0)) && boost::get<double> (operands.at (1)));
      else if (op == "==" || op == "=")
        return (boost::get<double> (operands.at (0)) == boost::get<double> (operands.at (1)));
      else if (op == "!=")
        return (boost::get<double> (operands.at (0)) != boost::get<double> (operands.at (1)));
      else if (op == "<")
        return (boost::get<double> (operands.at (0)) < boost::get<double> (operands.at (1)));
      else if (op == "<=")
        return (boost::get<double> (operands.at (0)) <= boost::get<double> (operands.at (1)));
      else if (op == ">")
        return (boost::get<double> (operands.at (0)) > boost::get<double> (operands.at (1)));
      else if (op == ">=")
        return (boost::get<double> (operands.at (0)) >= boost::get<double> (operands.at (1)));
      else if (op == "+")
        return (boost::get<double> (operands.at (0)) + boost::get<double> (operands.at (1)));
      else if (op == "-")
        return (boost::get<double> (operands.at (0)) - boost::get<double> (operands.at (1)));
      else if (op == "*")
        return (boost::get<double> (operands.at (0)) * boost::get<double> (operands.at (1)));
      else if (op == "/")
        return (boost::get<double> (operands.at (0)) / boost::get<double> (operands.at (1)));
      else if (op == "%")
        return ((int) boost::get<double> (operands.at (0)) % (int) boost::get<double> (operands.at (1)));
      else if (op == "!")
        return (!boost::get<double> (operands.at (0)));
      else if (op == "atan2")
        return (atan2 (boost::get<double> (operands.at (0)), boost::get<double> (operands.at (1))));
      else if (op == "ldexp")
        return (ldexp (boost::get<double> (operands.at (0)), boost::get<double> (operands.at (1))));
      else if (op == "pow")
        return (pow (boost::get<double> (operands.at (0)), boost::get<double> (operands.at (1))));
      else if (op == "hypot")
        return (hypot (boost::get<double> (operands.at (0)), boost::get<double> (operands.at (1))));
      else if (op == "fmod")
        return (fmod (boost::get<double> (operands.at (0)), boost::get<double> (operands.at (1))));
      else if (op == "remainder")
        return (remainder (boost::get<double> (operands.at (0)), boost::get<double> (operands.at (1))));
      else if (op == "copysign")
        return (copysign (boost::get<double> (operands.at (0)), boost::get<double> (operands.at (1))));
      else if (op == "nextafter")
        return (nextafter (boost::get<double> (operands.at (0)), boost::get<double> (operands.at (1))));
      else if (op == "fdim")
        return (fdim (boost::get<double> (operands.at (0)), boost::get<double> (operands.at (1))));
      else if (op == "fmax" || op == "max")
        return (fmax (boost::get<double> (operands.at (0)), boost::get<double> (operands.at (1))));
      else if (op == "fmin" || op == "min")
        return (fmin (boost::get<double> (operands.at (0)), boost::get<double> (operands.at (1))));
      else if (op == "cos")
        return (cos (boost::get<double> (operands.at (0))));
      else if (op == "sin")
        return (sin (boost::get<double> (operands.at (0))));
      else if (op == "tan")
        return (tan (boost::get<double> (operands.at (0))));
      else if (op == "acos")
        return (acos (boost::get<double> (operands.at (0))));
      else if (op == "asin")
        return (asin (boost::get<double> (operands.at (0))));
      else if (op == "atan")
        return (atan (boost::get<double> (operands.at (0))));
      else if (op == "cosh")
        return (cosh (boost::get<double> (operands.at (0))));
      else if (op == "sinh")
        return (sinh (boost::get<double> (operands.at (0))));
      else if (op == "tanh")
        return (tanh (boost::get<double> (operands.at (0))));
      else if (op == "acosh")
        return (acosh (boost::get<double> (operands.at (0))));
      else if (op == "asinh")
        return (asinh (boost::get<double> (operands.at (0))));
      else if (op == "atanh")
        return (atanh (boost::get<double> (operands.at (0))));
      else if (op == "exp")
        return (exp (boost::get<double> (operands.at (0))));
      else if (op == "log")
        return (log (boost::get<double> (operands.at (0))));
      else if (op == "log10")
        return (log10 (boost::get<double> (operands.at (0))));
      else if (op == "exp2")
        return (exp2 (boost::get<double> (operands.at (0))));
      else if (op == "expm1")
        return (expm1 (boost::get<double> (operands.at (0))));
      else if (op == "ilogb")
        return (ilogb (boost::get<double> (operands.at (0))));
      else if (op == "log1p")
        return (log1p (boost::get<double> (operands.at (0))));
      else if (op == "log2")
        return (log2 (boost::get<double> (operands.at (0))));
      else if (op == "logb")
        return (logb (boost::get<double> (operands.at (0))));
      else if (op == "sqrt")
        return (sqrt (boost::get<double> (operands.at (0))));
      else if (op == "cbrt")
        return (cbrt (boost::get<double> (operands.at (0))));
      else if (op == "erf")
        return (erf (boost::get<double> (operands.at (0))));
      else if (op == "erfc")
        return (erfc (boost::get<double> (operands.at (0))));
      else if (op == "tgamma")
        return (tgamma (boost::get<double> (operands.at (0))));
      else if (op == "lgamma")
        return (lgamma (boost::get<double> (operands.at (0))));
      else if (op == "ceil")
        return (ceil (boost::get<double> (operands.at (0))));
      else if (op == "floor")
        return (floor (boost::get<double> (operands.at (0))));
      else if (op == "trunc")
        return (trunc (boost::get<double> (operands.at (0))));
      else if (op == "round")
        return (round (boost::get<double> (operands.at (0))));
      else if (op == "rint")
        return (rint (boost::get<double> (operands.at (0))));
      else if (op == "nearbyint")
        return (nearbyint (boost::get<double> (operands.at (0))));
      else if (op == "abs" || op == "fabs")
        return (fabs (boost::get<double> (operands.at (0))));
      else if (op == "deltaPhi")
        return deltaPhi (valueLookup (boost::get<string> (operands.at (0)) + "s", objs, "phi"),
                         valueLookup (boost::get<string> (operands.at (1)) + "s", objs, "phi"));
      else if (op == "deltaR")
        {
          double eta0, phi0, eta1, phi1;

          eta0 = valueLookup (boost::get<string> (operands.at (0)) + "s", objs, "eta");
          phi0 = valueLookup (boost::get<string> (operands.at (0)) + "s", objs, "phi", false);
          eta1 = valueLookup (boost::get<string> (operands.at (1)) + "s", objs, "eta");
          phi1 = valueLookup (boost::get<string> (operands.at (1)) + "s", objs, "phi", false);

          return deltaR (eta0, phi0, eta1, phi1);
        }
      else if (op == "invMass")
        {
          double energy = 0.0, px = 0.0, py = 0.0, pz = 0.0;

          for (const auto &operand : operands)
            {
              energy += valueLookup (boost::get<string> (operand) + "s", objs, "energy");
              px += valueLookup (boost::get<string> (operand) + "s", objs, "px", false);
              py += valueLookup (boost::get<string> (operand) + "s", objs, "py", false);
              pz += valueLookup (boost::get<string> (operand) + "s", objs, "pz", false);
            }

          return sqrt (energy * energy - px * px - py * py - pz * pz);
        }
      else if (op == ".")
        return valueLookup (boost::get<string> (operands.at (0)) + "s", objs, boost::get<string> (operands.at (1)));
    }
  catch (...)
    {
      clog << "WARNING: failed to evaluate \"" << op << " (";
      for (auto operand = operands.begin (); operand != operands.end (); operand++)
        {
          if (operand != operands.begin ())
            clog << ", ";
          clog << *operand;
        }
      clog << ")\"" << endl;
      evaluationError_ = true;
    }
  return numeric_limits<int>::min ();
}

string &
ValueLookupTree::ltrim (string &s)
{
  return s.erase (0, s.find_first_not_of (" \t\f\n\r"));
}

string &
ValueLookupTree::rtrim (string &s)
{
  return s.erase (s.find_last_not_of (" \t\f\n\r") + 1);
}

string &
ValueLookupTree::trim (string &s)
{
  return ltrim (rtrim (s));
}

bool
ValueLookupTree::splitParentheses (string s) const
{
  // Returns true if the number of left parentheses does not equal the number
  // of right parentheses.

  int parentheses = 0;
  for (unsigned i = 0; i < s.length (); i++)
    {
      char c = s.at (i);
      if (c == '(')
        parentheses++;
      if (c == ')')
        parentheses--;
    }
  return parentheses;
}

bool
ValueLookupTree::isnumber (const string &s, double &x) const
{
  char *p;
  x = strtod (s.c_str (), &p);
  return !(*p);
}

bool
ValueLookupTree::firstOfPairAscending (pair<size_t, string> a, pair<size_t, string> b)
{
  return (a.first < b.first);
}

pair<size_t, string>
ValueLookupTree::findFirstOf (const string &s, const vector<string> &targets, const vector<string> &vetoTargets, const size_t pos) const
{
  vector<pair<size_t, string> > indices;
  pair<size_t, string> firstHit, biggestHit;

  //////////////////////////////////////////////////////////////////////////////
  // For each of the target strings, find the index of the first instance of
  // it. Then sort the indices in ascending order.
  //////////////////////////////////////////////////////////////////////////////
  for (const auto &target : targets)
    {
      size_t index = s.find (target, pos);
      if (!vetoMatch (s, target, index, vetoTargets))
        indices.push_back (make_pair (index, target));
    }
  sort (indices.begin (), indices.end (), ValueLookupTree::firstOfPairAscending);
  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  // If there are no matches, just return string::npos.
  //////////////////////////////////////////////////////////////////////////////
  if (!indices.size ())
    return make_pair (string::npos, "");
  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  // If there are multiple targets which match at the same position in the
  // string, then only return the one which matches the most characters.
  //////////////////////////////////////////////////////////////////////////////
  firstHit = indices.at (0);
  for (auto index = indices.begin (); index != indices.end () && index->first == firstHit.first; index++)
    {
      if (index->second.length () > biggestHit.second.length ())
        biggestHit = *index;
    }
  return biggestHit;
  //////////////////////////////////////////////////////////////////////////////
}

bool
ValueLookupTree::vetoMatch (const string &s, const string &target, const size_t index, const vector<string> &vetoTargets) const
{
  //////////////////////////////////////////////////////////////////////////////
  // If there was no match, just return true, indicating a veto.
  //////////////////////////////////////////////////////////////////////////////
  if (index == string::npos)
    return true;
  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  // We veto an operator match if:
  //   (1) the first character of the operator and the previous character are
  //   alphanumeric or an underscore.
  //   (2) the last character of the operator and the next character are
  //   alphanumeric or an underscore.
  //   (3) if the match is embedded in one of the veto targets passed to the
  //   function.
  //////////////////////////////////////////////////////////////////////////////
  size_t end = index + target.length () - 1;
  char first = s.at (index), last = s.at (end),
       preceding = index > 0 ? s.at (index - 1) : '\0', proceeding = end < s.length () - 1 ? s.at (end + 1) : '\0';
  if ((isalnum (first) || first == '_') && (isalnum (preceding) || preceding == '_'))
    return true;
  if ((isalnum (last) || last == '_') && (isalnum (proceeding) || proceeding == '_'))
    return true;
  for (const auto &vetoTarget : vetoTargets)
    {
      size_t i = s.find (vetoTarget, max ((size_t) 0, index - vetoTarget.length () - 1));
      if (i <= index)
        return true;
    }
  //////////////////////////////////////////////////////////////////////////////

  return false;
}

bool
ValueLookupTree::insertBinaryInfixOperator (const string &s, Node * const tree, const vector<string> &operators, const vector<string> &vetoOperators) const
{
  bool foundAnOperator = false;
  double x;

  //////////////////////////////////////////////////////////////////////////////
  // If the string is a number, then simply return false;
  //////////////////////////////////////////////////////////////////////////////
  if (isnumber (s, x))
    return false;
  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  // For each operator which is found in the string, split the string into a
  // left and right substring. These substrings are inserted into the tree and
  // stored as branches for the operator's node.
  //////////////////////////////////////////////////////////////////////////////
  for (auto i = findFirstOf (s, operators, vetoOperators); !foundAnOperator && i.first != string::npos; i = findFirstOf (s, operators, vetoOperators, i.first + i.second.length ()))
    {
      string left, right;
      left = s.substr (0, i.first);
      right = s.substr (i.first + i.second.length ());
      if (!splitParentheses (left) && !splitParentheses (right))
        {
          trim (left);
          trim (right);

          tree->value = i.second;
          tree->branches.push_back (insert_ (left, tree));
          tree->branches.push_back (insert_ (right, tree));
          foundAnOperator = true;
        }
    }
  //////////////////////////////////////////////////////////////////////////////

  return foundAnOperator;
}

bool
ValueLookupTree::insertUnaryPrefixOperator (const string &s, Node * const tree, const vector<string> &operators, const vector<string> &vetoOperators) const
{
  bool foundAnOperator = false;
  double x;

  //////////////////////////////////////////////////////////////////////////////
  // If the string is a number, then simply return false;
  //////////////////////////////////////////////////////////////////////////////
  if (isnumber (s, x))
    return foundAnOperator;
  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  // For each operator which is found in the string, split the string into a
  // left and right substring. The right substring is inserted into the tree and
  // stored as a branch for the operator's node.
  //////////////////////////////////////////////////////////////////////////////
  for (auto i = findFirstOf (s, operators, vetoOperators); !foundAnOperator && i.first != string::npos; i = findFirstOf (s, operators, vetoOperators, i.first + i.second.length ()))
    {
      string left, right;
      left = s.substr (0, i.first);
      right = s.substr (i.first + i.second.length ());
      if (!splitParentheses (left) && !splitParentheses (right))
        {
          trim (right);
          tree->value = i.second;
          tree->branches.push_back (insert_ (right, tree));
          foundAnOperator = true;
        }
    }
  //////////////////////////////////////////////////////////////////////////////

  return foundAnOperator;
}

bool
ValueLookupTree::insertParentheses (const string &s, Node * const tree) const
{
  //////////////////////////////////////////////////////////////////////////////
  // Look for the first left parenthesis and the last right parenthesis and
  // simply return false if both are not found.
  //////////////////////////////////////////////////////////////////////////////
  size_t leftParenthesis = s.find ('('),
         rightParenthesis = s.rfind (')');
  if (leftParenthesis == string::npos || rightParenthesis == string::npos)
    return false;
  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  // Insert the substring between the parentheses into the tree and return
  // true.
  //////////////////////////////////////////////////////////////////////////////
  string middle = s.substr (leftParenthesis + 1, rightParenthesis - leftParenthesis - 1);
  trim (middle);
  tree->value = "()";
  tree->branches.push_back (insert_ (middle, tree));
  return true;
  //////////////////////////////////////////////////////////////////////////////
}

string
ValueLookupTree::catInputCollection (const vector<string> &inputCollections)
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

double
ValueLookupTree::getMember (const string &type, void * const obj, const string &member) const
{
  double value = numeric_limits<int>::min ();
  Reflex::Type t = Reflex::Type::ByName (type);
  Reflex::Object *o = new Reflex::Object (t, obj);
  try
    {
      string typeName = t.DataMemberByName (member).TypeOf ().Name ();
      if (typeName == "double")
        value = Reflex::Object_Cast<double> (o->Get (member));
      else if (typeName == "int")
        value = Reflex::Object_Cast<int> (o->Get (member));
      else if (typeName != "")
        clog << "WARNING: \"" << member << "\" has unrecognized type \"" << typeName << "\"" << endl;
      else
        value = Reflex::Object_Cast<double> (o->Get (member));
    }
  catch (...)
    {
      bool found = false;
      for (auto bi = t.Base_Begin (); bi != t.Base_End (); bi++)
        {
          try
            {
              value = getMember (bi->Name (), obj, member);
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

bool
ValueLookupTree::isCollection (const string &name) const
{
  if (name == "bxlumis")
    return true;
  else if (name == "electrons")
    return true;
  else if (name == "events")
    return true;
  else if (name == "genjets")
    return true;
  else if (name == "jets")
    return true;
  else if (name == "mcparticles")
    return true;
  else if (name == "mets")
    return true;
  else if (name == "muons")
    return true;
  else if (name == "photons")
    return true;
  else if (name == "primaryvertexs")
    return true;
  else if (name == "superclusters")
    return true;
  else if (name == "taus")
    return true;
  else if (name == "tracks")
    return true;
  else if (name == "trigobjs")
    return true;
  else if (name == "userVariables")
    return true;
  return false;
}

string
ValueLookupTree::getCollectionType (const string &name) const
{
  if (name == "bxlumis")
    return "BNbxlumi";
  else if (name == "electrons")
    return "BNelectron";
  else if (name == "events")
    return "BNevent";
  else if (name == "genjets")
    return "BNgenjet";
  else if (name == "jets")
    return "BNjet";
  else if (name == "mcparticles")
    return "BNmcparticle";
  else if (name == "mets")
    return "BNmet";
  else if (name == "muons")
    return "BNmuon";
  else if (name == "photons")
    return "BNphoton";
  else if (name == "primaryvertexs")
    return "BNprimaryvertex";
  else if (name == "superclusters")
    return "BNsupercluster";
  else if (name == "taus")
    return "BNtau";
  else if (name == "tracks")
    return "BNtrack";
  else if (name == "trigobjs")
    return "BNtrigobj";
  return "";
}

double
ValueLookupTree::valueLookup (const string &collection, const ObjMap &objs, const string &variable, const bool iterateObj)
{
  if (!objIterators_.count (collection))
    {
      auto range = objs.equal_range (collection);
      objIterators_[collection] = range.first;
      shouldIterate_[collection] = (objs.count (collection) > 1);
    }
  else if (shouldIterate_.at (collection) && iterateObj)
    objIterators_.at (collection)++;
  void *obj = get<2> (objIterators_.at (collection)->second);

  try
    {
      if (collection == "userVariables")
        return (((map<string, double> *) obj)->at (variable));
      return getMember (getCollectionType (collection), obj, variable);
    }
  catch (...)
    {
      return numeric_limits<int>::min ();
    }

}
