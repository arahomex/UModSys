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
// SExecTCL::
//***************************************

SExecTCL::SExecTCL(IThread& ass, IExecutor* ee[], size_t nee, IExecTCL *u)
: ss(ass), stream(ass.stack_top(), ass.stack_left()), executors(ee), executors_len(nee), up(u) 
{
  stack_top = ss.stack_pos();
}

SExecTCL::SExecTCL(IThread& ass, IExecTCL *u)
: ss(ass), stream(ass.stack_top(), ass.stack_left()), executors(NULL), executors_len(0), up(u) 
{
  stack_top = ss.stack_pos();
}

SExecTCL::~SExecTCL(void) 
{
}

SExecTCL::IThread* SExecTCL::get_thread(void) const
{
  return &ss;
}

IExecTCL* SExecTCL::get_up(void) const 
{ 
  return up; 
}

SExecTCL::IExecutor* SExecTCL::get_executor(size_t id) const 
{ 
  return id<executors_len ? executors[id] : NULL; 
}

size_t SExecTCL::get_executor_count(void) const 
{
  return executors_len; 
}

/*
const IExecTCL* SExecTCL::get_other(TypeId type) const 
{
  if(type==get_tinfo())
    return this;
  return NULL;
}
IExecTCL* SExecTCL::get_other(TypeId type) 
{
  if(type==get_tinfo())
    return this;
  return NULL;
}
*/

bool SExecTCL::add_esc(StringP psym, int &idx) 
{
  switch(*psym) {
    case 'a': add(0x07); return false;
    case 'b': add(0x08); return false;
    case 'f': add(0x0c); return false;
    case 'n': add(0x0a); return false;
    case 'r': add(0x0d); return false;
    case 't': add(0x09); return false;
    case 'v': add(0x0b); return false;
    default: add(*psym); return false;
    //
    case 'o':
      return false;
    case 'x':
      return false;
    case 'u':
      return false;
    case 'U':
      return false;
    case '\n':
      return false;
  }
}

void SExecTCL::add_var(StringP b, StringP e) 
{ 
  add(var_get(string(b,e))); 
}

void SExecTCL::add_cmt(StringP b, StringP e) 
{
}

bool SExecTCL::exec_command(Parser& ps) 
{
  Self c2(ss, this);
  Parser ps2(ps.p, ps.e, true);
  ps2.Parse(c2);
  ps.p = ps2.p; // sync
  c2.finish();
  if(ps2.token>=Parser::tError)
    return false;
  add_result(c2);
  return true;
}

bool SExecTCL::execute(void) 
{
  if(args.size()==0)
    return true;
  execute_begin();
  bool rv = tcl_cmd(args);
  execute_end();
  return rv;
}

int SExecTCL::eval_expr(const String& expr) 
{
  if(~expr==0)
    return 0;
  int rv;
  if(!string_to_int(expr, rv))
    return -1;
  return rv;
}

void SExecTCL::eval_check(int ec, const String& txt) 
{
  if(ec==Parser::tEnd)
    ss.set_error(0, "");
  else
    ss.set_error(ec, txt);
}

SExecTCL::String SExecTCL::eval(const String& code) 
{
  Self c2(ss, this);
  Parser ps2(*code, code + ~code);
  eval_check(ps2.Parse(c2), code);
  c2.finish();
  return new_string(c2.result);
}

SExecTCL::String SExecTCL::eval(const String& code, IExecutor *ex2) 
{
  Self c2(ss, &ex2, 1, this);
  Parser ps2(*code, code + ~code);
  eval_check(ps2.Parse(c2), code);
  c2.finish();
  return new_string(c2.result);
}

int SExecTCL::eval_live_expr(const String& expr) 
{
  int rv = eval_expr(eval(expr));
  return rv;
}

bool SExecTCL::string_to_int(const String& src, int& dest) 
{
  StringName buf(src);
  return sscanf(*buf, "%d", &dest)==1;
}

SExecTCL::String SExecTCL::new_string(const String& src) 
{
  size_t p = ss.stack_pos();
  if(!ss.stack_add(~src+1))
      return String();
  char* dest = ss.stack_get(p);
  tl::su::smemcpy(dest, *src, ~src);
  dest[~src] = 0;
  String rv(dest, ~src);
  return rv;
}

SExecTCL::String SExecTCL::detach(void) 
{
  String rv = stream.get_s();
  stream_redo();
  return rv;
}

void SExecTCL::parse_start() 
{
  stream_redo();
}

void SExecTCL::stream_redo(void) 
{
  stream.setup(ss.stack_top(), ss.stack_left(), 0);
//  stream.length = 0;
//  stream.maxlength = ss.stack.MaxLen() - ss.stack.Len();
}

void SExecTCL::ssync(void) 
{
  ss.stack_reset( ~stream + (stream.get_text() - ss.stack_get(0)) );
//  ss.stack.Resize(~stream + (stream.get_text() - ss.stack.All()));
}

void SExecTCL::set_result(const String& src) 
{
  result = new_string(src);
}

void SExecTCL::add(char sym) 
{
  stream.append(&sym, 1);
  ssync();
}

void SExecTCL::add(StringP b, StringP e) 
{
  stream.append(b, e-b);
  ssync();
}

void SExecTCL::add(const String& ss) 
{
  stream.append(*ss, ~ss);
  ssync();
}

void SExecTCL::next_arg(void) 
{
  args.Push(detach()); 
}

size_t SExecTCL::stream_size(void) 
{ 
  return ~stream; 
}

bool SExecTCL::tcl_cmd(Strings& args) 
{
  IExecTCL *tcl = this;
  while(tcl!=NULL) {
    for(size_t i=0; i<tcl->get_executor_count(); i++) {
      IExecutor *ex = tcl->get_executor(i);
      if(ex!=NULL && ex->tcl_command(*this, ~args, args.All()))
        return true;
    }
    tcl = tcl->get_up();
  }
  return false;
}

SExecTCL::String SExecTCL::var_get(const String& name) 
{
  String rvx;
  IExecTCL *tcl = this;
  // always get executor vars
  while(tcl!=NULL) {
    for(size_t i=0; i<tcl->get_executor_count(); i++) {
      IExecutor *ex = tcl->get_executor(i);
      if(ex!=NULL && ex->tcl_getvar(*this, name, rvx))
        return rvx;
    }
    tcl = tcl->get_up();
  }
  return String();
}

SExecTCL::String SExecTCL::var_set(const String& name, const String& value) 
{
  // always set executor vars
  IExecTCL *tcl = this;
  while(tcl!=NULL) {
    for(size_t i=0; i<tcl->get_executor_count(); i++) {
      IExecutor *ex = tcl->get_executor(i);
      if(ex!=NULL && ex->tcl_setvar(*this, name, value))
        return value;
    }
    tcl = tcl->get_up();
  }
  return String();
}

void SExecTCL::execute_begin(void) 
{
/*
  printf("(");
  for(int i=0; i<args.size(); i++) {
      printf(" %d:'%.*s'", i, int(~args[i]), args[i].c_str());
  }
  printf(" )\n");
*/
}

void SExecTCL::execute_end(void) 
{
  args.Clear();
  ss.stack_reset(stack_top);
  stream.text = ss.stack_top();
  stream.length = 0;
//printf("{exec-end top=%d}", ss.stack.count);
}

void SExecTCL::finish(void) 
{
  ss.stack_reset(stack_top);
//printf("{finish top=%d}", ss.stack.count);
}

void SExecTCL::add_result(Self &r) 
{
//printf("add_result{%.*s}\n", int(~r.result), *r.result);
  add(r.result.begin(), r.result.end());
}

//***************************************
// SExecTCL::Thread::
//***************************************

SExecTCL::Thread::Thread(void)
{
  error_code = 0;
}

SExecTCL::Thread::~Thread(void)
{
}

int SExecTCL::Thread::get_error(void) const
{
  return error_code;
}

SExecTCL::String SExecTCL::Thread::get_error_text(void) const
{
  return error_text.get_s();
}

void SExecTCL::Thread::set_error(int err, const String &text)
{
  error_code = err;
  error_text = text;
}



void SExecTCL::Thread::print_s(const String& val)
{
  M.con().put(cl_Info, "%.*s", int(~val), val.c_str());
}

void SExecTCL::Thread::print_s(StringP val)
{
  M.con().put(cl_Info, "%s", val);
}

void SExecTCL::Thread::print_f(StringP val, ...)
{
  va_list va;
  va_start(va, val);
  M.con().vput(cl_Info, val, va);
  va_end(va);
}


//***************************************
// ::
//***************************************
