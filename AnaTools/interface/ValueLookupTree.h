#ifndef VALUE_LOOKUP_TREE
#define VALUE_LOOKUP_TREE

#include <set>
#include <unordered_map>
#include <unordered_set>

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

Note that the prunedParentheses function will convert this tree to:
      *
    /   \
   abs   2
    |
   (eta)

For a complex string such as innerTrack.hitPattern_.numberOfValidPixelHits > 0, the tree will have the following structure:
               < 
             /   \
            .     0
         /     \   
    innerTrack  . 
               / \
    hitPattern_  numberOfValidPixelHits

Right after the tree is built, this tree will get pruned by the pruneDots() function
to be the following:
                                                  < 
                                                /   \
   innerTrack.hitPattern_.numberOfValidPixelHits     0

Here is an example using commas:
               < 
             /   \
         invMass   0
            |
            ()
            |
            ,
          /   \
       muon   muon

This will be converted to the following, by the prunedCommas() and prunedParentheses() functions: 
               < 
             /   \
         invMass   0
          /   \
       muon   muon

*/

typedef unordered_multimap<string, DressedObject> ObjMap;

class ValueLookupTree
{
  public:
    ValueLookupTree ();
    ValueLookupTree (const Cut &);
    ValueLookupTree (const ValueToPrint &);
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
    // expression.  The evaluate() function returns values for each of the 
    // objects in the event; that is why it returns a vector.  
    // FIXME:  Check whether evaluate() should return vector<Leaf> or vector<double>.
    ////////////////////////////////////////////////////////////////////////////
    void insert (const string &);
    const vector<Leaf> &evaluate ();
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // Methods for retrieving various information about a collection.
    ////////////////////////////////////////////////////////////////////////////
    unsigned getLocalIndex (unsigned globalIndex, unsigned collectionIndex) const;
    set<unsigned> getGlobalIndices (unsigned localIndex, const string &singleObjectCollection, string inputLabel) const;
    unsigned getCollectionSize (const string &name) const;
    bool collectionIsFound (const string &name) const;
    ////////////////////////////////////////////////////////////////////////////

  private:
    // Method for destroying an entire tree, including all of its children.
    void destroy (Node * const) const;

    ////////////////////////////////////////////////////////////////////////////
    // Methods for removing commas and parentheses from a tree.
    ////////////////////////////////////////////////////////////////////////////
    void pruneCommas (Node * const) const;
    void pruneParentheses (Node * &) const;
    void pruneParentheses_ (Node * const) const;
    void pruneDots (Node * &) const;
    void pruneDots_ (Node * const) const;
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // Recursive methods for inserting an expression into the tree and then
    // evaluating it.
    ////////////////////////////////////////////////////////////////////////////
    Node *insert_ (const string &, Node * const) const;
    Leaf evaluate_ (const Node * const, const ObjMap &);
    ////////////////////////////////////////////////////////////////////////////

    // Mainly for debugging:
    string printNode(Node* node) const;
    string printValue(Node* node) const;

    // Returns the result of an operator acting on its operands.
    Leaf evaluateOperator (const string &op, const vector<Leaf> &operands, const ObjMap &objs);

    ////////////////////////////////////////////////////////////////////////////
    // Methods for retrieving and deleting an object from a collection.
    // i is the local index
    ////////////////////////////////////////////////////////////////////////////
    void *getObject (const string &name, const unsigned i);
    ////////////////////////////////////////////////////////////////////////////

    // Returns the C++ type associated with the collection named in the first
    // argument.
    string getCollectionType (const string &name) const;

    ////////////////////////////////////////////////////////////////////////////
    // Methods which returns true if the first argument looks like a collection
    // name or a number, respectively. The second argument of isNumber receives
    // the converted value.
    ////////////////////////////////////////////////////////////////////////////
    bool isCollection (const string &) const;
    bool isnumber (const string &, double &) const;
    ////////////////////////////////////////////////////////////////////////////

    // Method which returns true if the parentheses are not balanced, i.e., if
    // the number of left parentheses does not equal the number of right
    // parentheses.
    bool splitParentheses (string) const;

    ////////////////////////////////////////////////////////////////////////////
    // Methods for finding the first instance within a string of any one of a
    // vector of target strings.
    ////////////////////////////////////////////////////////////////////////////
    pair<size_t, string> findFirstOf (const string &s, const vector<string> &targets, const vector<string> &vetoTargets, const size_t pos = 0) const;
    pair<size_t, string> findLastOf (const string &s, const vector<string> &targets, const vector<string> &vetoTargets, const size_t pos = string::npos) const;
    bool vetoMatch (const string &, const string &, const size_t, const vector<string> &) const;
    ////////////////////////////////////////////////////////////////////////////

    // To avoid double counting. For a given set of objects, returns true only
    // if they are all unique and in a specific order.
    bool isUniqueCase (const ObjMap &, const unordered_set<string> &) const;

    ////////////////////////////////////////////////////////////////////////////
    // Methods for inserting different types of operators into the tree.
    ////////////////////////////////////////////////////////////////////////////
    bool insertBinaryInfixOperator (const string &, Node * const, const vector<string> &, const vector<string> & = {}) const;
    bool insertUnaryPrefixOperator (const string &, Node * const, const vector<string> &, const vector<string> & = {}) const;
    bool insertDots (const string &, Node * const) const;
    bool insertParentheses (const string &, Node * const) const;
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // Methods for retrieving values from objects.
    ////////////////////////////////////////////////////////////////////////////
    double valueLookup (const string &collection, const ObjMap &objs, const string &variable, const bool iterateObj = true);
    ////////////////////////////////////////////////////////////////////////////

    Node            *root_;
    vector<string>  inputCollections_;
    bool            evaluationError_;

    Collections                                    *handles_;
    unordered_map<string, ObjMap::const_iterator>  objIterators_;  // defined for each collection
    unordered_map<string, bool>                    shouldIterate_; // defined for each collection 
    vector<Leaf>                                   values_;
    vector<unsigned>                               collectionSizes_; // vector index corresponds to collection index
    vector<unsigned>                               nCombinations_;   // vector index corresponds to collection index
    bool                                           allCollectionsNonEmpty_;
    // nCombinations[i] specifies the number of combinations that can be formed from objects 
    // in collections i to N, where N is the number of collections 

    vector<void *> uservariablesToDelete_;
    vector<void *> eventvariablesToDelete_;

    const int                                      verbose_ = 0;  // verbosity levels:  0, 1, ... 
    // Typically you want to use verbosity of 1 when running over a single event.  
    

};

#endif
