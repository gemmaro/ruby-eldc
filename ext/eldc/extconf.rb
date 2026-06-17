# frozen_string_literal: true

require "mkmf"

VENDOR_RELPATH = File.join('eldc', 'src', 'eldc')
$INCFLAGS << " -I$(srcdir)/#{VENDOR_RELPATH}"
$VPATH << "$(srcdir)/#{VENDOR_RELPATH}"
$defs << "-DELD_BUILD_DLL"
$srcs = ['rb_eldc.c', 'eldc_lib.c']

# Makes all symbols private by default to avoid unintended conflict
# with other gems. To explicitly export symbols you can use RUBY_FUNC_EXPORTED
# selectively, or entirely remove this flag.
append_cflags("-fvisibility=hidden")

create_makefile("eldc/eldc")
