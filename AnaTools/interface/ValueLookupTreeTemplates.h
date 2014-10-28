#ifndef VALUE_LOOKUP_TREE_TEMPLATES
#define VALUE_LOOKUP_TREE_TEMPLATES

template<class T> double
ValueLookupTree::evaluate (const T* obj)
{
  return evaluate_ (root_, obj);
}

template<class T0, class T1> double
ValueLookupTree::evaluate (const T0* obj0, const T1* obj1)
{
  return evaluate_ (root_, obj0, obj1);
}

template<class T> double
ValueLookupTree::evaluate_ (node *tree, const T* obj)
{
  if (!tree)
    return false;
  if (tree->branches.size ())
    {
      vector<double> operands;
      for (vector<node *>::iterator branch = tree->branches.begin (); branch != tree->branches.end (); branch++)
        operands.push_back (evaluate_ (*branch, obj));
      return evaluateOperator (tree->value, operands);
    }
  else
    {
      double value;
      if (isnumber (tree->value, value))
        return value;
      else
        return vl_->valueLookup (obj, tree->value);
    }
}

template<class T0, class T1> double
ValueLookupTree::evaluate_ (node *tree, const T0* obj0, const T1* obj1)
{
  if (!tree)
    return false;
  if (tree->branches.size ())
    {
      vector<double> operands;
      for (vector<node *>::iterator branch = tree->branches.begin (); branch != tree->branches.end (); branch++)
        operands.push_back (evaluate_ (*branch, obj0, obj1));
      return evaluateOperator (tree->value, operands);
    }
  else
    {
      double value;
      if (isnumber (tree->value, value))
        return value;
      else
        return vl_->valueLookup (obj0, obj1, tree->value);
    }
}

#endif
