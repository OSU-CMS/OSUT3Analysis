#include <iostream>
#include <algorithm>

#include "DataFormats/Math/interface/deltaR.h"

#include "OSUT3Analysis/AnaTools/interface/CommonUtils.h"
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
  pruneDots (root_);

  sort (inputCollections_.begin (), inputCollections_.end ());
}

ValueLookupTree::ValueLookupTree (const ValueToPrint &value) :
  root_ (insert_ (value.valueToPrint, NULL)),
  inputCollections_ (value.inputCollections),
  evaluationError_ (false)
{
  pruneCommas (root_);
  pruneParentheses (root_);
  pruneDots (root_);

  sort (inputCollections_.begin (), inputCollections_.end ());
}

ValueLookupTree::ValueLookupTree (const string &expression, const vector<string> &inputCollections) :
  root_ (insert_ (expression, NULL)),
  inputCollections_ (inputCollections),
  evaluationError_ (false)
{
  pruneCommas (root_);
  pruneParentheses (root_);
  pruneDots (root_);

  sort (inputCollections_.begin (), inputCollections_.end ());
}

ValueLookupTree::~ValueLookupTree ()
{
  destroy (root_);
}

const Collections * const
ValueLookupTree::setCollections (Collections * const handles)
{
  //////////////////////////////////////////////////////////////////////////////
  // Assigns the given collections to the private class member, clears values_,
  // and recalculates nCombinations_ and collectionSizes_. If there are N input
  // collections, the i-th element of nCombinations_ is the product of the
  // sizes of the first (N - i) collections. This is used later when
  // calculating indices of objects. The i-th element of collectionSizes_ is
  // just the size of the i-th collection.
  //////////////////////////////////////////////////////////////////////////////
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
  //////////////////////////////////////////////////////////////////////////////

  return handles_;
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

const vector<Leaf> &
ValueLookupTree::evaluate ()
{
  //////////////////////////////////////////////////////////////////////////////
  // The values_ vector contains the expression stored in the tree evaluated
  // for each object. If it is empty when this method is called, it is filled.
  // Then the method returns it as a reference.
  //////////////////////////////////////////////////////////////////////////////
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
        }
    }

  return values_;
  //////////////////////////////////////////////////////////////////////////////
}

unsigned
ValueLookupTree::getLocalIndex (unsigned globalIndex, unsigned collectionIndex) const
{
  //////////////////////////////////////////////////////////////////////////////
  // Returns the local index within the primitive collection indexed by the
  // second argument, given the global index within the composite collection of
  // this tree.
  //////////////////////////////////////////////////////////////////////////////
  if (collectionIndex + 1 != inputCollections_.size ())
    return ((globalIndex / nCombinations_.at (collectionIndex + 1)) % collectionSizes_.at (collectionIndex));
  else
    return (globalIndex % collectionSizes_.at (collectionIndex));
  //////////////////////////////////////////////////////////////////////////////
}

set<unsigned>
ValueLookupTree::getGlobalIndices (unsigned localIndex, const string &singleObjectCollection, string inputLabel) const
{
  //////////////////////////////////////////////////////////////////////////////
  // Returns the global indices within the composite collection named by the
  // third argument, given a local index within the primitive collection named
  // by the second argument.
  //////////////////////////////////////////////////////////////////////////////
  vector<string> singleObjects = anatools::getSingleObjects (inputLabel);
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
  //////////////////////////////////////////////////////////////////////////////

  return globalIndices;
}

unsigned
ValueLookupTree::getCollectionSize (const string &name) const
{

  if (!collectionIsFound(name)) {
    clog << "ERROR [ValueLookupTree::getCollectionSize]:  Could not find collection named " << name
         << " for expression: " << printNode(root_) << endl
         << "List of input collections: " << endl;
    for (uint i=0; i<inputCollections_.size(); i++) clog << "  " << inputCollections_.at(i) << endl;
    clog << "Please modify your configuration file.  Exiting..." << endl << endl << endl;
    exit(0);
  }

  if (EQ_VALID(name,beamspots))
    return 1;
  else if (EQ_VALID(name,bxlumis))
    return handles_->bxlumis->size ();
  else if (EQ_VALID(name,electrons))
    return handles_->electrons->size ();
  else if (EQ_VALID(name,events))
    return handles_->events->size ();
  else if (EQ_VALID(name,genjets))
    return handles_->genjets->size ();
  else if (EQ_VALID(name,jets))
    return handles_->jets->size ();
  else if (EQ_VALID(name,mcparticles))
    return handles_->mcparticles->size ();
  else if (EQ_VALID(name,mets))
    return handles_->mets->size ();
  else if (EQ_VALID(name,muons))
    return handles_->muons->size ();
  else if (EQ_VALID(name,photons))
    return handles_->photons->size ();
  else if (EQ_VALID(name,primaryvertexs))
    return handles_->primaryvertexs->size ();
  else if (EQ_VALID(name,superclusters))
    return handles_->superclusters->size ();
  else if (EQ_VALID(name,taus))
    return handles_->taus->size ();
  else if (EQ_VALID(name,tracks))
    return handles_->tracks->size ();
  else if (EQ_VALID(name,trigobjs))
    return handles_->trigobjs->size ();
  else if (EQ_VALID(name,uservariables)){
    // !!!
    //    return handles_->uservariables[printValue(root_)].size ();
    //    return handles_->uservariables["electronPt"].size ();
    //cout << handles_->uservariables.at(printValue(root_)).size () << endl;
    //    auto isFound = handles_->uservariables.size();
    //    cout << printValue(root_) << endl;
    //    cout << "\t" << isFound << endl;
    return 1;  // FIXME
  }
  return 0;
}


bool
  ValueLookupTree::collectionIsFound (const string &name) const
{
  bool isFound = false;
  if      (EQ_VALID(name,beamspots))       isFound = handles_->beamspots.isValid();
  else if (EQ_VALID(name,bxlumis))         isFound = handles_->bxlumis.isValid();
  else if (EQ_VALID(name,electrons))       isFound = handles_->electrons.isValid();
  else if (EQ_VALID(name,events))          isFound = handles_->events.isValid();
  else if (EQ_VALID(name,genjets))         isFound = handles_->genjets.isValid();
  else if (EQ_VALID(name,jets))            isFound = handles_->jets.isValid();
  else if (EQ_VALID(name,mcparticles))     isFound = handles_->mcparticles.isValid();
  else if (EQ_VALID(name,mets))            isFound = handles_->mets.isValid();
  else if (EQ_VALID(name,muons))           isFound = handles_->muons.isValid();
  else if (EQ_VALID(name,photons))         isFound = handles_->photons.isValid();
  else if (EQ_VALID(name,primaryvertexs))  isFound = handles_->primaryvertexs.isValid();
  else if (EQ_VALID(name,superclusters))   isFound = handles_->superclusters.isValid();
  else if (EQ_VALID(name,taus))            isFound = handles_->taus.isValid();
  else if (EQ_VALID(name,tracks))          isFound = handles_->tracks.isValid();
  else if (EQ_VALID(name,trigobjs))        isFound = handles_->trigobjs.isValid();
  else if (EQ_VALID(name,uservariables))   isFound = true; // This vector is always present, even if its size is 0.
  return isFound;

}


void
ValueLookupTree::destroy (Node * const x) const
{
  for (const auto &branch : x->branches)
    destroy (branch);
  delete x;
}

void
ValueLookupTree::pruneCommas (Node * const tree) const
{
  //////////////////////////////////////////////////////////////////////////////
  // Recursively replaces any commas in the tree with the daughters.
  //////////////////////////////////////////////////////////////////////////////
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
              (*branch)->branches.at (0)->parent = tree;
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
  //////////////////////////////////////////////////////////////////////////////
}

void
ValueLookupTree::pruneParentheses (Node * &tree) const
{
  pruneParentheses_ (tree);
  if (!tree->parent)
    {
      if (tree->value == "()")
        {
          tree = tree->branches.at (0);
          tree->parent = NULL;
        }
    }
}

void
ValueLookupTree::pruneParentheses_ (Node * const tree) const
{
  //////////////////////////////////////////////////////////////////////////////
  // Recursively replaces any parentheses in the tree with the daughter.
  //////////////////////////////////////////////////////////////////////////////
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
              (*branch)->branches.at (0)->parent = tree;
              tree->branches.insert (branch + 1, (*branch)->branches.begin (), (*branch)->branches.end ());
              branch = tree->branches.begin () + parenthesis;
              tree->branches.erase (branch);
              branch = tree->branches.begin () + parenthesis;
            }
        }
    }
  while (foundParenthesis);

  for (const auto &branch : tree->branches)
    pruneParentheses_ (branch);
  //////////////////////////////////////////////////////////////////////////////
}

void
ValueLookupTree::pruneDots (Node * &tree) const
{
  pruneDots_ (tree);
  if (!tree->parent)
    {
      if (tree->value == "." && !isCollection (tree->branches.at (0)->value + "s"))
        {
          tree->branches.at (0)->value += "." + tree->branches.at (1)->value;
          tree->branches.at (0)->parent = NULL;
          tree = tree->branches.at (0);
        }
    }
}

void
ValueLookupTree::pruneDots_ (Node * const tree) const
{
  //////////////////////////////////////////////////////////////////////////////
  // Recursively replaces any dots in the tree with the concatentation of the
  // daughter values.
  //////////////////////////////////////////////////////////////////////////////
  bool foundDot;
  do
    {
      foundDot = false;
      for (auto branch = tree->branches.begin (); branch != tree->branches.end (); branch++)
        {
          if ((*branch)->value == "." && (((*branch)->parent && (*branch)->parent->value == ".")
                                       || !isCollection ((*branch)->branches.at (0)->value + "s")))
            {
              foundDot = true;

              size_t dot = branch - tree->branches.begin ();
              (*branch)->branches.at (0)->value += "." + (*branch)->branches.at (1)->value;
              (*branch)->branches.at (0)->parent = tree;
              tree->branches.insert (branch + 1, (*branch)->branches.at (0));
              branch = tree->branches.begin () + dot;
              tree->branches.erase (branch);
              branch = tree->branches.begin () + dot;
            }
        }
    }
  while (foundDot);

  for (const auto &branch : tree->branches)
    pruneDots_ (branch);
  //////////////////////////////////////////////////////////////////////////////
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

string
ValueLookupTree::printNode (Node* tree) const
{
  string expression = "";
  if (!tree) return expression;
  if (tree->branches.size ()) {
    for (const auto &branch : tree->branches)
      expression += branch->value + " ";
  } else {
    expression = tree->value;
  }

  return expression;

}

string
ValueLookupTree::printValue (Node* tree) const
{
  string expression = "";
  if (!tree) return expression;
  if (tree->branches.size () >= 1) {
    expression += tree->branches.at(0)->value;
  } 
  else {
    expression = tree->value;
  }
  cout << expression << endl;
  return expression;

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

void *
ValueLookupTree::getObject (const string &name, const unsigned i) const
{
  if (EQ_VALID(name,beamspots))
    return ((void *) &(*handles_->beamspots));
  else if (EQ_VALID(name,bxlumis))
    return ((void *) &handles_->bxlumis->at (i));
  else if (EQ_VALID(name,electrons))
    return ((void *) &handles_->electrons->at (i));
  else if (EQ_VALID(name,events))
    return ((void *) &handles_->events->at (i));
  else if (EQ_VALID(name,genjets))
    return ((void *) &handles_->genjets->at (i));
  else if (EQ_VALID(name,jets))
    return ((void *) &handles_->jets->at (i));
  else if (EQ_VALID(name,mcparticles))
    return ((void *) &handles_->mcparticles->at (i));
  else if (EQ_VALID(name,mets))
    return ((void *) &handles_->mets->at (i));
  else if (EQ_VALID(name,muons))
    return ((void *) &handles_->muons->at (i));
  else if (EQ_VALID(name,photons))
    return ((void *) &handles_->photons->at (i));
  else if (EQ_VALID(name,primaryvertexs))
    return ((void *) &handles_->primaryvertexs->at (i));
  else if (EQ_VALID(name,superclusters))
    return ((void *) &handles_->superclusters->at (i));
  else if (EQ_VALID(name,taus))
    return ((void *) &handles_->taus->at (i));
  else if (EQ_VALID(name,tracks))
    return ((void *) &handles_->tracks->at (i));
  else if (EQ_VALID(name,trigobjs))
    return ((void *) &handles_->trigobjs->at (i));
  else if (EQ_VALID(name,uservariables))
    {
      //!!!
      TYPE(uservariables) *obj = new TYPE(uservariables) ();
      for (const auto &handle : handles_->uservariables)
        obj->insert (handle->begin (), handle->end ());
      return obj;
    }
  return NULL;
}

string
ValueLookupTree::getCollectionType (const string &name) const
{
  if (EQ_VALID(name,beamspots))
    return TYPE_STR(beamspots);
  else if (EQ_VALID(name,bxlumis))
    return TYPE_STR(bxlumis);
  else if (EQ_VALID(name,electrons))
    return TYPE_STR(electrons);
  else if (EQ_VALID(name,events))
    return TYPE_STR(events);
  else if (EQ_VALID(name,genjets))
    return TYPE_STR(genjets);
  else if (EQ_VALID(name,jets))
    return TYPE_STR(jets);
  else if (EQ_VALID(name,mcparticles))
    return TYPE_STR(mcparticles);
  else if (EQ_VALID(name,mets))
    return TYPE_STR(mets);
  else if (EQ_VALID(name,muons))
    return TYPE_STR(muons);
  else if (EQ_VALID(name,photons))
    return TYPE_STR(photons);
  else if (EQ_VALID(name,primaryvertexs))
    return TYPE_STR(primaryvertexs);
  else if (EQ_VALID(name,superclusters))
    return TYPE_STR(superclusters);
  else if (EQ_VALID(name,taus))
    return TYPE_STR(taus);
  else if (EQ_VALID(name,tracks))
    return TYPE_STR(tracks);
  else if (EQ_VALID(name,trigobjs))
    return TYPE_STR(trigobjs);
  else if (EQ_VALID(name,uservariables))
    return TYPE_STR(uservariables);
  return "";
}

bool
ValueLookupTree::isCollection (const string &name) const
{
  if (EQ_VALID(name,beamspots))
    return true;
  else if (EQ_VALID(name,bxlumis))
    return true;
  else if (EQ_VALID(name,electrons))
    return true;
  else if (EQ_VALID(name,events))
    return true;
  else if (EQ_VALID(name,genjets))
    return true;
  else if (EQ_VALID(name,jets))
    return true;
  else if (EQ_VALID(name,mcparticles))
    return true;
  else if (EQ_VALID(name,mets))
    return true;
  else if (EQ_VALID(name,muons))
    return true;
  else if (EQ_VALID(name,photons))
    return true;
  else if (EQ_VALID(name,primaryvertexs))
    return true;
  else if (EQ_VALID(name,superclusters))
    return true;
  else if (EQ_VALID(name,taus))
    return true;
  else if (EQ_VALID(name,tracks))
    return true;
  else if (EQ_VALID(name,trigobjs))
    return true;
  else if (EQ_VALID(name,uservariables))
    return true;
  return false;
}

bool
ValueLookupTree::isnumber (const string &s, double &x) const
{
  char *p;
  x = strtod (s.c_str (), &p);
  return !(*p);
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
  sort (indices.begin (), indices.end (), anatools::firstOfPairAscending);
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
ValueLookupTree::isUniqueCase (const ObjMap &objs, const unordered_set<string> &keys) const
{
  //////////////////////////////////////////////////////////////////////////////
  // Returns true only if the given objects are unique and in a specific order.
  // This is to avoid double counting.
  //////////////////////////////////////////////////////////////////////////////
  bool pass = true;

  for (const auto &key : keys)
    {
      if (objs.count (key) <= 1)
        continue;
      else
        {
          auto range = objs.equal_range (key);
          vector<pair<string, tuple<unsigned, unsigned, void *> > > objsOfThisType (range.first, range.second);
          sort (objsOfThisType.begin (), objsOfThisType.end (), anatools::collectionIndexAscending);
          int previousLocalIndex = -1;
          for (const auto &obj : objsOfThisType)
            {
              pass = pass && ((int) get<1> (obj.second) > previousLocalIndex);
              previousLocalIndex = get<1> (obj.second);
            }
        }
    }

  return pass;
  //////////////////////////////////////////////////////////////////////////////
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
          anatools::trim (left);
          anatools::trim (right);

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
          anatools::trim (right);
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
  anatools::trim (middle);
  tree->value = "()";
  tree->branches.push_back (insert_ (middle, tree));
  return true;
  //////////////////////////////////////////////////////////////////////////////
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
      if (collection == "uservariables")
        return 1; // FIXME
        //        return (((VariableProducerPayload *) obj)->at (variable));
      return anatools::getMember (getCollectionType (collection), obj, variable);
    }
  catch (...)
    {
      return numeric_limits<int>::min ();
    }
}
