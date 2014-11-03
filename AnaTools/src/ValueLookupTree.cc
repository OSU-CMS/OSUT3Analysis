#include <iostream>
#include <algorithm>

#include "OSUT3Analysis/AnaTools/interface/ValueLookupTree.h"

ValueLookupTree::ValueLookupTree () :
  root_ (NULL),
  vl_ (NULL),
  evaluationError_ (false)
{
}

ValueLookupTree::ValueLookupTree (const string &cut, ValueLookup *vl) :
  root_ (insert_ (cut)),
  vl_ (vl),
  evaluationError_ (false)
{
}

ValueLookupTree::~ValueLookupTree ()
{
  destroy (root_);
}

ValueLookup *
ValueLookupTree::assignValueLookup (ValueLookup *vl)
{
  return (vl_ = vl);
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
  root_ = insert_ (cut);
}

void
ValueLookupTree::destroy (node *x)
{
  for (vector<node *>::const_iterator branch = x->branches.begin (); branch != x->branches.end (); branch++)
    destroy (*branch);
  delete x;
}

node *
ValueLookupTree::insert_ (const string &cut)
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
  node *tree = new node;
  if (!(insertBinaryInfixOperator   (cut,  tree,  {","})                   ||
        insertBinaryInfixOperator   (cut,  tree,  {"||", "|"})             ||
        insertBinaryInfixOperator   (cut,  tree,  {"&&", "&"})             ||
        insertBinaryInfixOperator   (cut,  tree,  {"==", "!="})            ||
        insertBinaryInfixOperator   (cut,  tree,  {"<", "<=", ">", ">="})  ||
        insertBinaryInfixOperator   (cut,  tree,  {"+", "-"})              ||
        insertBinaryInfixOperator   (cut,  tree,  {"*", "/", "%"})         ||
        insertUnaryPrefixOperator   (cut,  tree,  {"!"})                   ||
        insertUnaryPrefixOperator   (cut,  tree,  {"cos", "sin", "tan", "acos", "asin", "atan", "atan2",
                                                   "cosh", "sinh", "tanh", "acosh", "asinh", "atanh",
                                                   "exp", "ldexp", "log", "log10", "exp2", "expm1", "ilogb", "log1p", "log2", "logb",
                                                   "pow", "sqrt", "cbrt", "hypot",
                                                   "erf", "erfc", "tgamma", "lgamma",
                                                   "ceil", "floor", "fmod", "trunc", "round", "rint", "nearbyint", "remainder", "abs", "fabs",
                                                   "copysign", "nextafter",
                                                   "fdim", "fmax", "fmin", "max", "min"}) ||
        insertParentheses           (cut,  tree)))
    tree->value = cut;
  //////////////////////////////////////////////////////////////////////////////

  return tree;
}

double
ValueLookupTree::evaluateOperator (const string &op, const vector<double> &operands)
{
  // Tries to return the result of operating on the operands. Prints out a
  // warning, sets evaluationError_ to true, and returns the maximum unsigned
  // integer if there is a problem.

  evaluationError_ = false;
  try
    {
      if (op == ",")
        return (operands.at (0), operands.at (1));
      else if (op == "||" || op == "|")
        return (operands.at (0) || operands.at (1));
      else if (op == "&&" || op == "&")
        return (operands.at (0) && operands.at (1));
      else if (op == "==")
        return (operands.at (0) == operands.at (1));
      else if (op == "!=")
        return (operands.at (0) != operands.at (1));
      else if (op == "<")
        return (operands.at (0) < operands.at (1));
      else if (op == "<=")
        return (operands.at (0) <= operands.at (1));
      else if (op == ">")
        return (operands.at (0) > operands.at (1));
      else if (op == ">=")
        return (operands.at (0) >= operands.at (1));
      else if (op == "+")
        return (operands.at (0) + operands.at (1));
      else if (op == "-")
        return (operands.at (0) - operands.at (1));
      else if (op == "*")
        return (operands.at (0) * operands.at (1));
      else if (op == "/")
        return (operands.at (0) / operands.at (1));
      else if (op == "%")
        return ((int) operands.at (0) % (int) operands.at (1));
      else if (op == "!")
        return (!operands.at (0));
      else if (op == "atan2")
        return (atan2 (operands.at (0), operands.at (1)));
      else if (op == "ldexp")
        return (ldexp (operands.at (0), operands.at (1)));
      else if (op == "pow")
        return (pow (operands.at (0), operands.at (1)));
      else if (op == "hypot")
        return (hypot (operands.at (0), operands.at (1)));
      else if (op == "fmod")
        return (fmod (operands.at (0), operands.at (1)));
      else if (op == "remainder")
        return (remainder (operands.at (0), operands.at (1)));
      else if (op == "copysign")
        return (copysign (operands.at (0), operands.at (1)));
      else if (op == "nextafter")
        return (nextafter (operands.at (0), operands.at (1)));
      else if (op == "fdim")
        return (fdim (operands.at (0), operands.at (1)));
      else if (op == "fmax" || op == "max")
        return (fmax (operands.at (0), operands.at (1)));
      else if (op == "fmin" || op == "min")
        return (fmin (operands.at (0), operands.at (1)));
      else if (op == "cos")
        return (cos (operands.at (0)));
      else if (op == "sin")
        return (sin (operands.at (0)));
      else if (op == "tan")
        return (tan (operands.at (0)));
      else if (op == "acos")
        return (acos (operands.at (0)));
      else if (op == "asin")
        return (asin (operands.at (0)));
      else if (op == "atan")
        return (atan (operands.at (0)));
      else if (op == "cosh")
        return (cosh (operands.at (0)));
      else if (op == "sinh")
        return (sinh (operands.at (0)));
      else if (op == "tanh")
        return (tanh (operands.at (0)));
      else if (op == "acosh")
        return (acosh (operands.at (0)));
      else if (op == "asinh")
        return (asinh (operands.at (0)));
      else if (op == "atanh")
        return (atanh (operands.at (0)));
      else if (op == "exp")
        return (exp (operands.at (0)));
      else if (op == "log")
        return (log (operands.at (0)));
      else if (op == "log10")
        return (log10 (operands.at (0)));
      else if (op == "exp2")
        return (exp2 (operands.at (0)));
      else if (op == "expm1")
        return (expm1 (operands.at (0)));
      else if (op == "ilogb")
        return (ilogb (operands.at (0)));
      else if (op == "log1p")
        return (log1p (operands.at (0)));
      else if (op == "log2")
        return (log2 (operands.at (0)));
      else if (op == "logb")
        return (logb (operands.at (0)));
      else if (op == "sqrt")
        return (sqrt (operands.at (0)));
      else if (op == "cbrt")
        return (cbrt (operands.at (0)));
      else if (op == "erf")
        return (erf (operands.at (0)));
      else if (op == "erfc")
        return (erfc (operands.at (0)));
      else if (op == "tgamma")
        return (tgamma (operands.at (0)));
      else if (op == "lgamma")
        return (lgamma (operands.at (0)));
      else if (op == "ceil")
        return (ceil (operands.at (0)));
      else if (op == "floor")
        return (floor (operands.at (0)));
      else if (op == "trunc")
        return (trunc (operands.at (0)));
      else if (op == "round")
        return (round (operands.at (0)));
      else if (op == "rint")
        return (rint (operands.at (0)));
      else if (op == "nearbyint")
        return (nearbyint (operands.at (0)));
      else if (op == "abs" || op == "fabs")
        return (fabs (operands.at (0)));
      else if (op == "()")
        return (operands.at (0));
    }
  catch (...)
    {
      clog << "WARNING: failed to evaluate \"" << op << " (";
      for (vector<double>::const_iterator operand = operands.begin (); operand != operands.end (); operand++)
        {
          if (operand != operands.begin ())
            clog << ", " << endl;
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
ValueLookupTree::pruneCommas (vector<node *> &branches)
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
  node *originalBranch = branches.at (0);
  branches.clear ();
  for (vector<node *>::const_iterator branch = originalBranch->branches.begin (); branch != originalBranch->branches.end (); branch++)
    branches.push_back (*branch);
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
ValueLookupTree::findFirstOf (const string &s, const vector<string> &targets, const size_t pos)
{
  vector<pair<size_t, string> > indices;
  pair<size_t, string> firstHit, biggestHit;

  //////////////////////////////////////////////////////////////////////////////
  // For each of the target strings, find the index of the first instance of
  // it. Then sort the indices in ascending order.
  //////////////////////////////////////////////////////////////////////////////
  for (vector<string>::const_iterator target = targets.begin (); target != targets.end (); target++)
    indices.push_back (make_pair (s.find (*target, pos), *target));
  sort (indices.begin (), indices.end (), ValueLookupTree::firstOfPairAscending);
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
ValueLookupTree::insertBinaryInfixOperator (const string &s, node *tree, const vector<string> &operators)
{
  bool foundAnOperator = false;
  double x;

  //////////////////////////////////////////////////////////////////////////////
  // If the string is a number, then simply return false;
  //////////////////////////////////////////////////////////////////////////////
  if (isnumber (s,x))
    return false;
  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  // For each operator which is found in the string, split the string into a
  // left and right substring. These substrings are inserted into the tree and
  // stored as branches for the operator's node.
  //////////////////////////////////////////////////////////////////////////////
  for (pair<size_t, string> i = findFirstOf (s, operators); !foundAnOperator && i.first != string::npos; i = findFirstOf (s, operators, i.first + i.second.length ()))
    {
      string left, right;
      left = s.substr (0, i.first);
      right = s.substr (i.first + i.second.length ());
      if (!splitParentheses (left) && !splitParentheses (right))
        {
          trim (left);
          trim (right);

          tree->value = i.second;
          tree->branches.push_back (insert_ (left));
          tree->branches.push_back (insert_ (right));
          foundAnOperator = true;
        }
    }
  //////////////////////////////////////////////////////////////////////////////

  return foundAnOperator;
}

bool
ValueLookupTree::insertUnaryPrefixOperator (const string &s, node *tree, const vector<string> &operators)
{
  bool foundAnOperator = false;
  double x;

  //////////////////////////////////////////////////////////////////////////////
  // If the string is a number, then simply return false;
  //////////////////////////////////////////////////////////////////////////////
  if (isnumber (s,x))
    return foundAnOperator;
  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  // For each operator which is found in the string, split the string into a
  // left and right substring. The right substring is inserted into the tree and
  // stored as a branch for the operator's node.
  //////////////////////////////////////////////////////////////////////////////
  for (pair<size_t, string> i = findFirstOf (s, operators); !foundAnOperator && i.first != string::npos; i = findFirstOf (s, operators, i.first + i.second.length ()))
    {
      string left, right;
      left = s.substr (0, i.first);
      right = s.substr (i.first + i.second.length ());
      if (!splitParentheses (left) && !splitParentheses (right))
        {
          trim (right);
          tree->value = i.second;
          tree->branches.push_back (insert_ (right));
          pruneCommas (tree->branches);
          foundAnOperator = true;
        }
    }
  //////////////////////////////////////////////////////////////////////////////

  return foundAnOperator;
}

bool
ValueLookupTree::insertParentheses (const string &s, node *tree)
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
  tree->branches.push_back (insert_ (middle));
  return true;
  //////////////////////////////////////////////////////////////////////////////
}
