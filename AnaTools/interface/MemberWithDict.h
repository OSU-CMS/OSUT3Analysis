#ifndef OSUT3Analysis_AnaTools_MemberWithDict_h
#define OSUT3Analysis_AnaTools_MemberWithDict_h

/*----------------------------------------------------------------------

MemberWithDict:  A holder for a class member

----------------------------------------------------------------------*/


#include <string>

class TDataMember;

namespace anatools {

class ObjectWithDict;
class TypeWithDict;

class MemberWithDict {
private:
  TDataMember* dataMember_;
public:
  MemberWithDict();
  explicit MemberWithDict(TDataMember*);
  explicit operator bool() const;
  std::string name() const;
  bool isArray() const;
  bool isConst() const;
  bool isPublic() const;
  bool isStatic() const;
  bool isTransient() const;
  size_t offset() const;
  TypeWithDict declaringType() const;
  TypeWithDict typeOf() const;
  ObjectWithDict get() const;
  ObjectWithDict get(ObjectWithDict const&) const;
};

} // namespace anatools

#include "OSUT3Analysis/AnaTools/interface/ObjectWithDict.h"
#include "OSUT3Analysis/AnaTools/interface/TypeWithDict.h"

#endif // OSUT3Analysis_AnaTools_MemberWithDict_h
