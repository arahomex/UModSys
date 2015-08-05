#include "umodsys.basesys.rsystem.h"
#include "umodsys.basesys.rmodule.h"

#if defined(_DEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;
using namespace UModSys::base::rsystem;

//***************************************
// RSystem::
//***************************************

bool RSystem::get_var(const core::DCString& name, core::DCString& value) const
{
  return false;
}

bool RSystem::get_shell(const core::DCString& name, IShell::P& shell) const
{
  return false;
}

bool RSystem::execute(const core::DCString& text)
{
  return false;
}

bool RSystem::set_var(const core::DCString& name, const core::DCString& value, bool can_new)
{
  return false;
}

bool RSystem::set_shell(const core::DCString& name, IShell* shell)
{
  shells[name] = shell;
  return true;
}

//***************************************
// ::
//***************************************
