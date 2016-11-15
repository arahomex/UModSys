#include <umodsys/common/base/uniq.h>
#include <umodsys/common/stdinlines.h>

using namespace UModSys;
using namespace UModSys::core;

//***************************************
// SUniquePtrList::
//***************************************

SUniquePtrList::SUniquePtrList(HUniquePointer *a, size_t n) 
: values(DSet::Base(a, n))
{
}

bool SUniquePtrList::add(HUniquePointer val)
{
  if(val==NULL)
    return false; // never add NULL
  return values.Set(val);
}

size_t SUniquePtrList::find(HUniquePointer val) const
{
  if(val==NULL)
    return array_index_none; // never add NULL
  return values.FindIndex(val);
}

//***************************************
// ::
//***************************************
