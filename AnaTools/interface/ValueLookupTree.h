#ifndef VALUE_LOOKUP_TREE
#define VALUE_LOOKUP_TREE

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"
#include "OSUT3Analysis/AnaTools/interface/ValueLookup.h"

class ValueLookupTree
{
  public:
    ValueLookupTree ();
    ValueLookupTree (const string &, ValueLookup *);
    ~ValueLookupTree ();

    // Method for assigning a ValueLookup object which is used to evaluate the
    // expression.
    ValueLookup *assignValueLookup (ValueLookup *);

    ////////////////////////////////////////////////////////////////////////////
    // Error checking methods: isValid() returns false if the tree has not been
    // built, i.e., an expression has not been inserted, and evaluationError()
    // returns true if there was any exception during the evaluation of the
    // expression.
    ////////////////////////////////////////////////////////////////////////////
    bool isValid ();
    bool evaluationError ();
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // Methods for inserting an expression into the tree and for evaluating the
    // expression.
    ////////////////////////////////////////////////////////////////////////////
    void insert (const string &);
    template<class T> double evaluate (const T &);
    template<class T0, class T1> double evaluate (const T0 &, const T1 &);
    ////////////////////////////////////////////////////////////////////////////

  private:
    // Method for destroying an entire tree, including all of its children.
    void destroy (node *);

    ////////////////////////////////////////////////////////////////////////////
    // Recursive methods for inserting an expression into the tree and then
    // evaluating it.
    ////////////////////////////////////////////////////////////////////////////
    node *insert_ (const string &);
    double evaluateOperator (const string &, const vector<double> &);
    template<class T> double evaluate_ (node *, const T &);
    template<class T0, class T1> double evaluate_ (node *, const T0 &, const T1 &);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // Methods for removing whitespace from a string, either from the left, the
    // right, or both sides.
    ////////////////////////////////////////////////////////////////////////////
    string &ltrim (string &);
    string &rtrim (string &);
    string &trim (string &);
    ////////////////////////////////////////////////////////////////////////////

    // Method which returns true if the parentheses are not balanced, i.e., if
    // the number of left parentheses does not equal the number of right
    // parentheses.
    bool splitParentheses (string);

    // Method which returns true if the first argument looks like a number. The
    // second argument receives the converted value.
    bool isnumber (const string &, double &);

    // Method which removes commas and parentheses from the immediate daughters
    // of a unary prefix operator.
    void pruneCommas (vector<node *> &);

    ////////////////////////////////////////////////////////////////////////////
    // Methods for finding the first instance within a string of any one of a
    // vector of target strings.
    ////////////////////////////////////////////////////////////////////////////
    static bool firstOfPairAscending (pair<size_t, string>, pair<size_t, string>);
    pair<size_t, string> findFirstOf (const string &, const vector<string> &, const vector<string> &, const size_t = 0);
    bool vetoMatch (const string &, const string &, const size_t, const vector<string> &);
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // Methods for inserting different types of operators into the tree.
    ////////////////////////////////////////////////////////////////////////////
    bool insertBinaryInfixOperator (const string &, node *, const vector<string> &, const vector<string> & = {});
    bool insertUnaryPrefixOperator (const string &, node *, const vector<string> &, const vector<string> & = {});
    bool insertParentheses (const string &, node *);
    ////////////////////////////////////////////////////////////////////////////

    node *root_;
    ValueLookup *vl_;
    bool evaluationError_;
};

#include "OSUT3Analysis/AnaTools/interface/ValueLookupTreeTemplates.h"

#endif
