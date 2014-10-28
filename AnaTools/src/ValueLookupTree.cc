#include <iostream>
#include <algorithm>

#include "OSUT3Analysis/AnaTools/interface/ValueLookupTree.h"

ValueLookupTree::ValueLookupTree () :
  root_ (NULL)
{
}

ValueLookupTree::ValueLookupTree (const string &cut, ValueLookup *vl) :
  root_ (insert_ (cut)),
  vl_ (vl)
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

void
ValueLookupTree::destroy (node *x)
{
  for (vector<node *>::const_iterator branch = x->branches.begin (); branch != x->branches.end (); branch++)
    destroy (*branch);
  delete x;
}

double
ValueLookupTree::evaluateOperator (const string &op, const vector<double> &operands)
{
  if (op == "||")
    return (operands.at (0) || operands.at (1));
  else if (op == "&&")
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
  else if (op == "!")
    return (!operands.at (0));
  else if (op == "()")
    return (operands.at (0));

  return -99.0;
}

bool
ValueLookupTree::isnumber (const string &s, double &x)
{
  char *p;
  x = strtod (s.c_str (), &p);
  return !(*p);
}

void
ValueLookupTree::insert (const string &cut)
{
  root_ = insert_ (cut);
}

node *
ValueLookupTree::insert_ (const string &cut)
{
  node *tree = new node;
  if (!(insertBinaryInfixOperator  (cut,  tree,  {"||"})                  ||
        insertBinaryInfixOperator  (cut,  tree,  {"&&"})                  ||
        insertBinaryInfixOperator  (cut,  tree,  {"==", "!="})            ||
        insertBinaryInfixOperator  (cut,  tree,  {"<", "<=", ">", ">="})  ||
        insertBinaryInfixOperator  (cut,  tree,  {"+", "-"})              ||
        insertBinaryInfixOperator  (cut,  tree,  {"*", "/"})              ||
        insertUnaryPrefixOperator  (cut,  tree,  {"!"})                   ||
        insertParentheses          (cut,  tree)))
    tree->value = cut;

  return tree;
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
ValueLookupTree::firstOfPairAscending (pair<size_t, string> a, pair<size_t, string> b)
{
  return (a.first < b.first);
}

pair<size_t, string>
ValueLookupTree::findFirstOf (const string &s, const vector<string> &targets, const size_t pos)
{
  vector<pair<size_t, string> > indices;
  for (vector<string>::const_iterator target = targets.begin (); target != targets.end (); target++)
    indices.push_back (make_pair (s.find (*target, pos), *target));
  sort (indices.begin (), indices.end (), ValueLookupTree::firstOfPairAscending);
  return indices.at (0);
}

bool
ValueLookupTree::insertBinaryInfixOperator (const string &s, node *tree, const vector<string> &operators)
{
  bool foundAnOperator = false;
  double x;
  if (isnumber (s,x))
    return foundAnOperator;
  for (pair<size_t, string> i = findFirstOf (s, operators); i.first != string::npos; i = findFirstOf (s, operators, i.first + i.second.length ()))
    {
      string left, right;
      left = s.substr (0, i.first);
      right = s.substr (i.first + i.second.length ());
      if (!splitParentheses (left))
        {
          trim (left);
          trim (right);

          tree->value = i.second;
          tree->branches.push_back (insert_ (left));
          tree->branches.push_back (insert_ (right));
          foundAnOperator = true;
        }
    }

  return foundAnOperator;
}

bool
ValueLookupTree::insertUnaryPrefixOperator (const string &s, node *tree, const vector<string> &operators)
{
  bool foundAnOperator = false;
  double x;
  if (isnumber (s,x))
    return foundAnOperator;
  for (pair<size_t, string> i = findFirstOf (s, operators); i.first != string::npos; i = findFirstOf (s, operators, i.first + i.second.length ()))
    {
      string right;
      right = s.substr (i.first + i.second.length ());
      if (!splitParentheses (right))
        {
          trim (right);

          tree->value = i.second;
          tree->branches.push_back (insert_ (right));
          foundAnOperator = true;
        }
    }

  return foundAnOperator;
}

bool
ValueLookupTree::insertParentheses (const string &s, node *tree)
{
  size_t leftParenthesis = s.find ('('),
         rightParenthesis = s.rfind (')');
  if (leftParenthesis == string::npos || rightParenthesis == string::npos)
    return false;
  string middle = s.substr (leftParenthesis + 1, rightParenthesis - leftParenthesis - 1);
  trim (middle);
  tree->value = "()";
  tree->branches.push_back (insert_ (middle));
  return true;
}
