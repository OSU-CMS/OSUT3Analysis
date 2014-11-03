#ifndef VALUE_LOOKUP_TREE_TEMPLATES
#define VALUE_LOOKUP_TREE_TEMPLATES

// This file contains the definitions of the template methods in the
// ValueLookupTree class. These definitions need to be included by any file
// which calls these functions so that the compiler knows which instances of
// the methods to create.

template<class T> double
ValueLookupTree::evaluate (const T &obj)
{
  return evaluate_ (root_, obj);
}

template<class T0, class T1> double
ValueLookupTree::evaluate (const T0 &obj0, const T1 &obj1)
{
  return evaluate_ (root_, obj0, obj1);
}

template<class T> double
ValueLookupTree::evaluate_ (node *tree, const T &obj)
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
      vector<double> operands;
      for (vector<node *>::iterator branch = tree->branches.begin (); branch != tree->branches.end (); branch++)
        operands.push_back (evaluate_ (*branch, obj));
      return evaluateOperator (tree->value, operands);
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
      else
        return vl_->valueLookup (obj, tree->value);
    }
  //////////////////////////////////////////////////////////////////////////////
}

template<class T0, class T1> double
ValueLookupTree::evaluate_ (node *tree, const T0 &obj0, const T1 &obj1)
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
      vector<double> operands;
      for (vector<node *>::iterator branch = tree->branches.begin (); branch != tree->branches.end (); branch++)
        operands.push_back (evaluate_ (*branch, obj0, obj1));
      return evaluateOperator (tree->value, operands);
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
      else
        return vl_->valueLookup (obj0, obj1, tree->value);
    }
  //////////////////////////////////////////////////////////////////////////////
}

#endif
