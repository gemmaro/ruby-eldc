#include "eldc.h"

VALUE rb_mEldc;

RUBY_FUNC_EXPORTED void
Init_eldc(void)
{
  rb_mEldc = rb_define_module("Eldc");
}
