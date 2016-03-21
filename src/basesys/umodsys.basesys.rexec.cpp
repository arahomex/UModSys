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
: ss(ass), executors(ee), executors_len(nee), up(u) 
{
  ss->stack_stream(stream);
}

SExecTCL::SExecTCL(IThread& ass, IExecTCL *u)
: ss(ass), executors(NULL), executors_len(0), up(u) 
{
  ss->stack_stream(stream);
}

SExecTCL::~SExecTCL(void) 
{
}

SExecTCL::IThread* SExecTCL::get_thread(void) const
{
  return ss;
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

void SExecTCL::add(char sym) 
{
//  printf("{a c}");
  stream.append(&sym, 1);
  ssync();
}

void SExecTCL::add(StringP b, StringP e) 
{
//  printf("{a %u}", e-b);
  stream.append(b, e-b);
  ssync();
}

void SExecTCL::add(const String& ss) 
{
//  printf("{a %u}", ~ss);
  stream.append(*ss, ~ss);
  ssync();
}

void SExecTCL::next_arg(void) 
{
  args.Push(detach()); 
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
  eStatus rv = tcl_cmd(args);
  execute_end();
  return sResult(rv);
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
    ss->set_error(0, "");
  else
    ss->set_error(ec, txt);
}

SExecTCL::String SExecTCL::eval(const String& code) 
{
  Self c2(ss, this);
//printf("Eval{%.*s}\n", int(~code), *code);
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
  char* dest = ss->stack_addz(~src, *src);
  if(dest==NULL)
    return String();
//  printf("{ns t %u}", ss->stack_top());
  String rv(dest, ~src);
  return rv;
}

SExecTCL::String SExecTCL::detach(void) 
{
  String rv = stream.get_s();
  stream_redo();
//  printf("{dt t %u}", ss->stack_top());
  return rv;
}

void SExecTCL::parse_start() 
{
  stream_redo();
}

void SExecTCL::stream_redo(void) 
{
//  printf("{R t %u}", ss->stack_top());
  ss.next(stream);
//  printf("{/R t %u}", ss->stack_top());
}

void SExecTCL::ssync(void) 
{
  ss->stack_movetop(stream.get_text() + ~stream - ss->stack_value() );
//  printf("{mvt %u t %u}", ~stream, ss->stack_top());
}

void SExecTCL::set_result(const String& src) 
{
  result = new_string(src);
}

size_t SExecTCL::stream_size(void) 
{ 
  return ~stream; 
}

IExecTCL::eStatus SExecTCL::tcl_cmd(Strings& args) 
{
  IExecTCL *tcl = this;
  while(tcl!=NULL) {
    for(size_t i=0; i<tcl->get_executor_count(); i++) {
      IExecutor *ex = tcl->get_executor(i);
      if(ex==NULL) continue;
      eStatus rv = ex->tcl_command(*this, ~args, args.All());
      if(sHave(rv))
        return rv;
    }
    tcl = tcl->get_up();
  }
  return sFalse;
}

SExecTCL::String SExecTCL::var_get(const String& name) 
{
  String rvx;
  IExecTCL *tcl = this;
  // always get executor vars
  while(tcl!=NULL) {
    for(size_t i=0; i<tcl->get_executor_count(); i++) {
      IExecutor *ex = tcl->get_executor(i);
      if(ex==NULL) continue;
      eStatus rv = ex->tcl_getvar(*this, name, rvx);
      if(sResult(rv))
        return rvx;
      if(sFailed(rv))
        return String(); // failed
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
      if(ex==NULL) continue;
      eStatus rv = ex->tcl_setvar(*this, name, value);
      if(sResult(rv))
        return value;
      if(sFailed(rv))
        return String(); // failed

    }
    tcl = tcl->get_up();
  }
  return String();
}

void SExecTCL::execute_begin(void) 
{
//  printf("{t %u}", ss->stack_top());
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
//  printf("{t %u}", ss->stack_top());
  args.Clear();
  ss.reset();
  ss.re(stream);
}

void SExecTCL::finish(void) 
{
//  printf("{t %u}", ss->stack_top());
  ss.reset();
}

void SExecTCL::add_result(Self &r) 
{
//  printf("{t %u}", ss->stack_top());
  add(r.result.begin(), r.result.end());
}

void SExecTCL::next_token(int token)
{
//  printf("{token %d}", token);
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

size_t SExecTCL::Thread::stack_top(void) const
{
  return stack.Len();
}

size_t SExecTCL::Thread::stack_maxtop(void) const
{
  return stack.MaxLen();
}

char* SExecTCL::Thread::stack_value(void)
{
  return stack.All();
}

void SExecTCL::Thread::stack_movetop(size_t top)
{
  stack.resize(top);
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
