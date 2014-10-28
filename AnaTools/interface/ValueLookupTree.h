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

    ValueLookup *assignValueLookup (ValueLookup *);
    static bool firstOfPairAscending (pair<size_t, string>, pair<size_t, string>);
    void insert (const string &);
    template<class T> double evaluate (const T*);
    template<class T0, class T1> double evaluate (const T0*, const T1*);

  private:
    void destroy (node *);
    template<class T> double evaluate_ (node *, const T*);
    template<class T0, class T1> double evaluate_ (node *, const T0*, const T1*);
    node *insert_ (const string &);
    string &ltrim (string &);
    string &rtrim (string &);
    string &trim (string &);
    bool splitParentheses (string);
    pair<size_t, string> findFirstOf (const string &, const vector<string> &, const size_t pos = 0);
    bool insertBinaryInfixOperator (const string &, node *, const vector<string> &);
    bool insertUnaryPrefixOperator (const string &, node *, const vector<string> &);
    bool insertParentheses (const string &, node *);
    double evaluateOperator (const string &, const vector<double> &);
    bool isnumber (const string &, double &);

    node *root_;
    ValueLookup *vl_;
};

#include "OSUT3Analysis/AnaTools/interface/ValueLookupTreeTemplates.h"

#endif
