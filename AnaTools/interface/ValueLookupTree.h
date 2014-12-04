#ifndef VALUE_LOOKUP_TREE
#define VALUE_LOOKUP_TREE

#include <set>
#include <unordered_map>
#include <unordered_set>

// Return whether obj is contained in vec.
#define VEC_CONTAINS(vec,obj) (find (vec.begin (), vec.end (), obj) != vec.end ())

#include "OSUT3Analysis/AnaTools/interface/AnalysisTypes.h"

/*
A ValueLookupTree object contains all the information needed to
evaluate an expression or inequality for a given object.

An example for an applied cut, i.e., an inequality:
   ValueLookupTree myvaltree("abs(eta) < 2.5", valueLookupMuons);
This will create a tree with the following structure:
        <
      /   \
    abs   2.5
     |
    ( )
     |
    eta

The nodes "eta" and "2.5" in this tree are leaves, i.e., they have no branches.
The nodes "<", "abs", and "( )" are not leaves, i.e., they have at least one
branch.
When the evaluate function is called, e.g., myvaltree.evalutate(muons.at(0)),
each node of the tree will be evaluated recursively.  The nodes that are leaves
return the value they correspond to; the nodes that are not leaves apply an operator
to their branch(es).
In this example, the evaluate function will always return 0 or 1.

Another example is an expression, e.g., "2 * abs(eta)", represented as:
      *
    /   \
   abs   2
    |
   ( )
    |
   eta
In this case the evaluate function would return a continuous value.
*/

typedef unordered_multimap<string, tuple<unsigned, unsigned, void *> > ObjMap;

class ValueLookupTree
{
  public:
    ValueLookupTree ();
    ValueLookupTree (const Cut &);
    ValueLookupTree (const string &, const vector<string> &);
    ~ValueLookupTree ();

    // Method for assigning a ValueLookup object which is used to evaluate the
    // expression.
    const Collections * const setCollections (Collections * const);

    ////////////////////////////////////////////////////////////////////////////
    // Error checking methods: isValid() returns false if the tree has not been
    // built, i.e., an expression has not been inserted, and evaluationError()
    // returns true if there was any exception during the evaluation of the
    // expression.
    ////////////////////////////////////////////////////////////////////////////
    bool isValid () const;
    bool evaluationError () const;
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // Methods for inserting an expression into the tree and for evaluating the
    // expression.
    ////////////////////////////////////////////////////////////////////////////
    void insert (const string &);
    void *getObject (const string &name, const unsigned i) const;
    void deleteObject (const string &, void * const) const;
    const vector<Leaf> &evaluate ();
    unsigned getLocalIndex (unsigned, unsigned) const;
    static vector<string> getSingleObjects (string);
    set<unsigned> getGlobalIndices (unsigned, const string &, string) const;
    unsigned getCollectionSize (const string &name) const;
    ////////////////////////////////////////////////////////////////////////////

    static string catInputCollection (const vector<string> &);

    static string capitalize (string);
    static string singular (string);
    static string plural (string);

    ////////////////////////////////////////////////////////////////////////////
    // Methods for removing whitespace from a string, either from the left, the
    // right, or both sides.
    ////////////////////////////////////////////////////////////////////////////
    static string &ltrim (string &);
    static string &rtrim (string &);
    static string &trim (string &);
    ////////////////////////////////////////////////////////////////////////////

  private:
    // Method for destroying an entire tree, including all of its children.
    void destroy (Node * const) const;

    void pruneCommas (Node * const) const;
    void pruneParentheses (Node * const) const;

    ////////////////////////////////////////////////////////////////////////////
    // Recursive methods for inserting an expression into the tree and then
    // evaluating it.
    ////////////////////////////////////////////////////////////////////////////
    Node *insert_ (const string &, Node * const) const;
    string getCollectionType (const string &name) const;
    Leaf evaluateOperator (const string &, const vector<Leaf> &, const ObjMap &);
    Leaf evaluate_ (const Node * const, const ObjMap &);
    ////////////////////////////////////////////////////////////////////////////

    // Method which returns true if the parentheses are not balanced, i.e., if
    // the number of left parentheses does not equal the number of right
    // parentheses.
    bool splitParentheses (string) const;

    // Method which returns true if the first argument looks like a number. The
    // second argument receives the converted value.
    bool isnumber (const string &, double &) const;

    ////////////////////////////////////////////////////////////////////////////
    // Methods for finding the first instance within a string of any one of a
    // vector of target strings.
    ////////////////////////////////////////////////////////////////////////////
    static bool firstOfPairAscending (pair<size_t, string>, pair<size_t, string>);
    pair<size_t, string> findFirstOf (const string &, const vector<string> &, const vector<string> &, const size_t = 0) const;
    bool vetoMatch (const string &, const string &, const size_t, const vector<string> &) const;
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // Methods for inserting different types of operators into the tree.
    ////////////////////////////////////////////////////////////////////////////
    bool insertBinaryInfixOperator (const string &, Node * const, const vector<string> &, const vector<string> & = {}) const;
    bool insertUnaryPrefixOperator (const string &, Node * const, const vector<string> &, const vector<string> & = {}) const;
    bool insertParentheses (const string &, Node * const) const;
    ////////////////////////////////////////////////////////////////////////////

    Node *root_;
    vector<string> inputCollections_;
    Collections *handles_;
    vector<Leaf> values_;
    bool evaluationError_;
    vector<unsigned> nCombinations_;
    vector<unsigned> collectionSizes_;

    ////////////////////////////////////////////////////////////////////////////
    // valueLookup methods for each type of object.
    ////////////////////////////////////////////////////////////////////////////
    double  valueLookup  (const  BNbxlumi             * const,  const string &) const;
    double  valueLookup  (const  BNelectron           * const,  const string &) const;
    double  valueLookup  (const  BNevent              * const,  const string &) const;
    double  valueLookup  (const  BNgenjet             * const,  const string &) const;
    double  valueLookup  (const  BNjet                * const,  const string &) const;
    double  valueLookup  (const  BNmcparticle         * const,  const string &) const;
    double  valueLookup  (const  BNmet                * const,  const string &) const;
    double  valueLookup  (const  BNmuon               * const,  const string &) const;
    double  valueLookup  (const  BNphoton             * const,  const string &) const;
    double  valueLookup  (const  BNprimaryvertex      * const,  const string &) const;
    double  valueLookup  (const  BNsupercluster       * const,  const string &) const;
    double  valueLookup  (const  BNtau                * const,  const string &) const;
    double  valueLookup  (const  BNtrack              * const,  const string &) const;
    double  valueLookup  (const  BNtrigobj            * const,  const string &) const;
    ////////////////////////////////////////////////////////////////////////////

    bool isCollection (const string &) const;
    void inferInputCollections (const Node * const, vector<string> &) const;

    double getMember (const string &, void * const, const string &) const;
    double valueLookup (const string &, const ObjMap &, const string &, const bool = true);

    bool isUniqueCase (const ObjMap &, const unordered_set<string> &) const;

    unordered_map<string, ObjMap::const_iterator> objIterators_;
    unordered_map<string, bool> shouldIterate_;

    static bool collectionIndexAscending (pair<string, tuple<unsigned, unsigned, void *> >, pair<string, tuple<unsigned, unsigned, void *> >);
};

#endif
