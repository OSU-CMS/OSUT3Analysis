#ifndef OSUT3Analysis_AnaTools_BaseWithDict_h
#define OSUT3Analysis_AnaTools_BaseWithDict_h

/*----------------------------------------------------------------------

BaseWithDict:  A holder for a base class

----------------------------------------------------------------------*/


#include <string>

class TBaseClass;

namespace anatools {

class TypeWithDict;

class BaseWithDict {
private:
  TBaseClass* baseClass_;
public:
  BaseWithDict();
  explicit BaseWithDict(TBaseClass*);
  bool isPublic() const;
  std::string name() const;
  TypeWithDict typeOf() const;
  size_t offset() const;
};

} // namespace anatools

#include "OSUT3Analysis/AnaTools/interface/TypeWithDict.h"

#endif // OSUT3Analysis_AnaTools_BaseWithDict_h
