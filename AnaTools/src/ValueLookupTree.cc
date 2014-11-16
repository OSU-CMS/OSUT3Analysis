#include <iostream>
#include <algorithm>

#include "Reflex/Base.h"
#include "Reflex/Member.h"
#include "Reflex/Object.h"
#include "Reflex/Type.h"

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
  vector<string> trimmedInputCollections;
  trimInputCollections (root_, trimmedInputCollections);
  if (trimmedInputCollections.size ())
    inputCollections_ = trimmedInputCollections;
  sort (inputCollections_.begin (), inputCollections_.end ());
}

ValueLookupTree::ValueLookupTree (const string &expression, const vector<string> &inputCollections) :
  root_ (insert_ (expression, NULL)),
  inputCollections_ (inputCollections),
  evaluationError_ (false)
{
  vector<string> trimmedInputCollections;
  trimInputCollections (root_, trimmedInputCollections);
  sort (trimmedInputCollections.begin (), trimmedInputCollections.end ());
  trimmedInputCollections.erase (unique (trimmedInputCollections.begin (), trimmedInputCollections.end ()), trimmedInputCollections.end ());
  if (trimmedInputCollections.size ())
    inputCollections_ = trimmedInputCollections;
  sort (inputCollections_.begin (), inputCollections_.end ());
}

ValueLookupTree::~ValueLookupTree ()
{
  destroy (root_);
}

void
ValueLookupTree::trimInputCollections (const Node *tree, vector<string> &inputCollections)
{
  if (tree->branches.size ())
    {
      for (vector<Node *>::const_iterator branch = tree->branches.begin (); branch != tree->branches.end (); branch++)
        trimInputCollections (*branch, inputCollections);
    }
  else if (isCollection (tree->value + "s"))
    inputCollections.push_back (tree->value + "s");
}

Collections *
ValueLookupTree::setCollections (Collections *handles)
{
  handles_ = handles;
  values_.clear ();
  nCombinations_.clear ();
  collectionSizes_.clear ();
  nCombinations_.assign (inputCollections_.size (), 1);
  for (vector<string>::const_iterator collection = inputCollections_.begin (); collection != inputCollections_.end (); collection++)
    {
      unsigned currentSize = getCollectionSize (*collection);
      for (unsigned i = 0; i < (collection - inputCollections_.begin () + 1); i++)
        nCombinations_[i] *= currentSize;
      collectionSizes_.push_back (currentSize);
    }

  return handles_;
}

unsigned
ValueLookupTree::getLocalIndex (unsigned globalIndex, unsigned collectionIndex)
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
      singleObjects.push_back (inputLabel.substr (0, hyphen));
      inputLabel = inputLabel.substr (hyphen + 1);
    }
  singleObjects.push_back (inputLabel);
  sort (singleObjects.begin (), singleObjects.end ());
  //////////////////////////////////////////////////////////////////////////////

  return singleObjects;
}

vector<unsigned>
ValueLookupTree::getGlobalIndices (unsigned localIndex, const string &singleObjectCollection, string inputLabel)
{
  vector<string> singleObjects = getSingleObjects (inputLabel);
  vector<unsigned> globalIndices, nCombinations (singleObjects.size (), 1), collectionSizes;
  unsigned singleObjectIndex = singleObjects.size ();
  for (vector<string>::const_iterator collection = singleObjects.begin (); collection != singleObjects.end (); collection++)
    {
      unsigned currentSize = getCollectionSize (*collection);
      for (unsigned i = 0; i < (collection - singleObjects.begin () + 1); i++)
        nCombinations[i] *= currentSize;
      collectionSizes.push_back (currentSize);
      if (*collection == singleObjectCollection)
        singleObjectIndex = (collection - singleObjects.begin ());
    }
  if (singleObjectIndex < singleObjects.size ())
    {
      for (unsigned i = 0; i < nCombinations.at (0); i++)
        {
          if (singleObjectIndex + 1 < singleObjects.size ())
            {
              if ((i / nCombinations.at (singleObjectIndex + 1)) % collectionSizes.at (singleObjectIndex) == localIndex)
                globalIndices.push_back (i);
            }
          else
            {
              if (i % collectionSizes.at (singleObjectIndex) == localIndex)
                globalIndices.push_back (i);
            }
        }
    }

  return globalIndices;
}

bool
ValueLookupTree::isValid ()
{
  return (root_ != NULL);
}

bool
ValueLookupTree::evaluationError ()
{
  return evaluationError_;
}

void
ValueLookupTree::insert (const string &cut)
{
  root_ = insert_ (cut, NULL);
}

void
ValueLookupTree::destroy (Node *x)
{
  for (vector<Node *>::const_iterator branch = x->branches.begin (); branch != x->branches.end (); branch++)
    destroy (*branch);
  delete x;
}

Node *
ValueLookupTree::insert_ (const string &cut, Node *parent)
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
        insertBinaryInfixOperator  (cut,  tree,  {"."})                           ||
        insertParentheses          (cut,  tree)))
    tree->value = cut;
  //////////////////////////////////////////////////////////////////////////////

  return tree;
}

void *
ValueLookupTree::getObject (const string &name, const unsigned i)
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
  return NULL;
}

unsigned
ValueLookupTree::getCollectionSize (const string &name)
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
  return 0;
}

const vector<Operand> &
ValueLookupTree::evaluate ()
{
  if (!values_.size ())
    {
      evaluationError_ = false;
      for (unsigned i = 0; i < nCombinations_.at (0); i++)
        {
          map<string, void *> objs;
          for (vector<string>::const_iterator collection = inputCollections_.begin (); collection != inputCollections_.end (); collection++)
            {
              unsigned j = collection - inputCollections_.begin ();
              objs[*collection] = getObject (*collection, getLocalIndex (i, j));
            }
          values_.push_back (evaluate_ (root_, objs));
        }
    }

  return values_;
}

Operand
ValueLookupTree::evaluate_ (Node *tree, const map<string, void *> &objs)
{
  //////////////////////////////////////////////////////////////////////////////
  // Do nothing if the tree is null.
  //////////////////////////////////////////////////////////////////////////////
  if (!tree)
    return false;
  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  // The node is not a leaf and its value is an operator. First, evaluate its
  // daughters, then return the result of the operator acting on the daughters.
  //////////////////////////////////////////////////////////////////////////////
  if (tree->branches.size ())
    {
      vector<Operand> operands;
      for (vector<Node *>::iterator branch = tree->branches.begin (); branch != tree->branches.end (); branch++)
        operands.push_back (evaluate_ (*branch, objs));
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
      else if (tree->parent && tree->parent->value == ".")
        return tree->value;
      else
        {
          if (inputCollections_.size () == 1)
            return valueLookup (inputCollections_.at (0), objs.at (inputCollections_.at (0)), tree->value);
          clog << "ERROR: cannot infer ownership of \"" << tree->value << "\"" << endl;
          evaluationError_ = true;
          return numeric_limits<unsigned>::min ();
        }
    }
  //////////////////////////////////////////////////////////////////////////////
}

Operand
ValueLookupTree::evaluateOperator (const string &op, const vector<Operand> &operands, const map<string, void *> &objs)
{
  // Tries to return the result of operating on the operands. Prints out a
  // warning, sets evaluationError_ to true, and returns the maximum unsigned
  // integer if there is a problem.

  try
    {
      if (op == ",")
        return (boost::get<double> (operands.at (0)), boost::get<double> (operands.at (1)));
      else if (op == "||" || op == "|")
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
      else if (op == ".")
        return valueLookup (boost::get<string> (operands.at (0)) + "s", objs.at (boost::get<string> (operands.at (0)) + "s"), boost::get<string> (operands.at (1)));
      else if (op == "()")
        return (operands.at (0));
    }
  catch (...)
    {
      clog << "WARNING: failed to evaluate \"" << op << " (";
      for (vector<Operand>::const_iterator operand = operands.begin (); operand != operands.end (); operand++)
        {
          if (operand != operands.begin ())
            clog << ", ";
          clog << *operand;
        }
      clog << ")\"" << endl;
      evaluationError_ = true;
    }
  return numeric_limits<unsigned>::min ();
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
ValueLookupTree::splitParentheses (string s)
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
ValueLookupTree::isnumber (const string &s, double &x)
{
  char *p;
  x = strtod (s.c_str (), &p);
  return !(*p);
}

void
ValueLookupTree::pruneCommas (vector<Node *> &branches)
{
  //////////////////////////////////////////////////////////////////////////////
  // Do nothing if there is not exactly one branch whose value is either a
  // comma or parentheses.
  //////////////////////////////////////////////////////////////////////////////
  if (branches.size () != 1)
    return;
  if (branches.at (0)->value != "," && branches.at (0)->value != "()")
    return;
  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  // Replace the branches with those of the daughter, then prune the result
  // again.
  //////////////////////////////////////////////////////////////////////////////
  Node *originalBranch = branches.at (0);
  branches.clear ();
  for (vector<Node *>::const_iterator branch = originalBranch->branches.begin (); branch != originalBranch->branches.end (); branch++)
    {
      branches.push_back (*branch);
      branches.back ()->parent = originalBranch->parent;
    }
  delete originalBranch;
  pruneCommas (branches);
  //////////////////////////////////////////////////////////////////////////////
}

bool
ValueLookupTree::firstOfPairAscending (pair<size_t, string> a, pair<size_t, string> b)
{
  return (a.first < b.first);
}

pair<size_t, string>
ValueLookupTree::findFirstOf (const string &s, const vector<string> &targets, const vector<string> &vetoTargets, const size_t pos)
{
  vector<pair<size_t, string> > indices;
  pair<size_t, string> firstHit, biggestHit;

  //////////////////////////////////////////////////////////////////////////////
  // For each of the target strings, find the index of the first instance of
  // it. Then sort the indices in ascending order.
  //////////////////////////////////////////////////////////////////////////////
  for (vector<string>::const_iterator target = targets.begin (); target != targets.end (); target++)
    {
      size_t index = s.find (*target, pos);
      if (!vetoMatch (s, *target, index, vetoTargets))
        indices.push_back (make_pair (index, *target));
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
  for (vector<pair<size_t, string> >::const_iterator index = indices.begin (); index != indices.end () && index->first == firstHit.first; index++)
    {
      if (index->second.length () > biggestHit.second.length ())
        biggestHit = *index;
    }
  return biggestHit;
  //////////////////////////////////////////////////////////////////////////////
}

bool
ValueLookupTree::vetoMatch (const string &s, const string &target, const size_t index, const vector<string> &vetoTargets)
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
  for (vector<string>::const_iterator vetoTarget = vetoTargets.begin (); vetoTarget != vetoTargets.end (); vetoTarget++)
    {
      size_t i = s.find (*vetoTarget, max ((size_t) 0, index - vetoTarget->length () - 1));
      if (i <= index)
        return true;
    }
  //////////////////////////////////////////////////////////////////////////////

  return false;
}

bool
ValueLookupTree::insertBinaryInfixOperator (const string &s, Node *tree, const vector<string> &operators, const vector<string> &vetoOperators)
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
  for (pair<size_t, string> i = findFirstOf (s, operators, vetoOperators); !foundAnOperator && i.first != string::npos; i = findFirstOf (s, operators, vetoOperators, i.first + i.second.length ()))
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
ValueLookupTree::insertUnaryPrefixOperator (const string &s, Node *tree, const vector<string> &operators, const vector<string> &vetoOperators)
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
  for (pair<size_t, string> i = findFirstOf (s, operators, vetoOperators); !foundAnOperator && i.first != string::npos; i = findFirstOf (s, operators, vetoOperators, i.first + i.second.length ()))
    {
      string left, right;
      left = s.substr (0, i.first);
      right = s.substr (i.first + i.second.length ());
      if (!splitParentheses (left) && !splitParentheses (right))
        {
          trim (right);
          tree->value = i.second;
          tree->branches.push_back (insert_ (right, tree));
          pruneCommas (tree->branches);
          foundAnOperator = true;
        }
    }
  //////////////////////////////////////////////////////////////////////////////

  return foundAnOperator;
}

bool
ValueLookupTree::insertParentheses (const string &s, Node *tree)
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
  for (vector<string>::const_iterator collection = inputCollections.begin (); collection != inputCollections.end (); collection++)
    {
      if (collection != inputCollections.begin ())
        catInputCollection += "-";
      catInputCollection += *collection;
    }
  return catInputCollection;
}

double
ValueLookupTree::getMember (const string &type, void *obj, const string &member)
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
      for (Reflex::Base_Iterator bi = t.Base_Begin (); bi != t.Base_End (); bi++)
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
ValueLookupTree::isCollection (const string &name)
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
  return false;
}

string
ValueLookupTree::getCollectionType (const string &name)
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
ValueLookupTree::valueLookup (const string &collection, void *obj, const string &variable)
{
  try
    {
      return getMember (getCollectionType (collection), obj, variable);
    }
  catch (...)
    {
      if (collection == "bxlumis")
        return valueLookup ((BNbxlumi *) obj, variable);
      else if (collection == "electrons")
        return valueLookup ((BNelectron *) obj, variable);
      else if (collection == "events")
        return valueLookup ((BNevent *) obj, variable);
      else if (collection == "genjets")
        return valueLookup ((BNgenjet *) obj, variable);
      else if (collection == "jets")
        return valueLookup ((BNjet *) obj, variable);
      else if (collection == "mcparticles")
        return valueLookup ((BNmcparticle *) obj, variable);
      else if (collection == "mets")
        return valueLookup ((BNmet *) obj, variable);
      else if (collection == "muons")
        return valueLookup ((BNmuon *) obj, variable);
      else if (collection == "photons")
        return valueLookup ((BNphoton *) obj, variable);
      else if (collection == "primaryvertexs")
        return valueLookup ((BNprimaryvertex *) obj, variable);
      else if (collection == "superclusters")
        return valueLookup ((BNsupercluster *) obj, variable);
      else if (collection == "taus")
        return valueLookup ((BNtau *) obj, variable);
      else if (collection == "tracks")
        return valueLookup ((BNtrack *) obj, variable);
      else if (collection == "trigobjs")
        return valueLookup ((BNtrigobj *) obj, variable);
    }

  return numeric_limits<int>::min ();
}
