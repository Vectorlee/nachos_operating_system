# NOTE: this is a GNU Makefile.  You must use "gmake" rather than "make".
#
# Makefile for the threads assignment.  The threads assignment must
#  be done first!
#
# Copyright (c) 1992 The Regents of the University of California.
# All rights reserved.  See copyright.h for copyright notice and limitation 
# of liability and disclaimer of warranty provisions.

DEFINES = -DTHREADS
INCPATH = -I../threads -I../machine
HFILES = $(THREAD_H)
CFILES = $(THREAD_C)
C_OFILES = $(THREAD_O)

include ../Makefile.common
include ../Makefile.dep
#-----------------------------------------------------------------
# DO NOT DELETE THIS LINE -- make depend uses it
# DEPENDENCIES MUST END AT END OF FILE
main.o: ../threads/main.cc ../threads/copyright.h ../threads/utility.h \
 ../threads/bool.h ../machine/sysdep.h ../threads/copyright.h \
 /usr/include/stdio.h /usr/include/features.h /usr/include/bits/predefs.h \
 /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
 /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
 /usr/lib/gcc/i486-linux-gnu/4.4.3/include/stddef.h \
 /usr/include/bits/types.h /usr/include/bits/typesizes.h \
 /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
 ../threads/stdarg.h /usr/include/bits/stdio_lim.h \
 /usr/include/bits/sys_errlist.h /usr/include/string.h \
 /usr/include/xlocale.h ../threads/system.h ../threads/thread.h \
 ../threads/scheduler.h ../threads/list.h ../machine/interrupt.h \
 ../threads/list.h ../machine/stats.h ../machine/timer.h \
 ../threads/utility.h ../threads/threadmanager.h ../threads/synch.h \
 ../threads/messagequeue.h /usr/include/c++/4.4/map \
 /usr/include/c++/4.4/bits/stl_tree.h \
 /usr/include/c++/4.4/bits/stl_algobase.h \
 /usr/include/c++/4.4/i486-linux-gnu/bits/c++config.h \
 /usr/include/c++/4.4/i486-linux-gnu/bits/os_defines.h \
 /usr/include/c++/4.4/i486-linux-gnu/bits/cpu_defines.h \
 /usr/include/c++/4.4/cstddef /usr/include/c++/4.4/bits/functexcept.h \
 /usr/include/c++/4.4/exception_defines.h \
 /usr/include/c++/4.4/bits/cpp_type_traits.h \
 /usr/include/c++/4.4/ext/type_traits.h \
 /usr/include/c++/4.4/ext/numeric_traits.h \
 /usr/include/c++/4.4/bits/stl_pair.h /usr/include/c++/4.4/bits/move.h \
 /usr/include/c++/4.4/bits/concept_check.h \
 /usr/include/c++/4.4/bits/stl_iterator_base_types.h \
 /usr/include/c++/4.4/bits/stl_iterator_base_funcs.h \
 /usr/include/c++/4.4/bits/stl_iterator.h \
 /usr/include/c++/4.4/debug/debug.h /usr/include/c++/4.4/bits/allocator.h \
 /usr/include/c++/4.4/i486-linux-gnu/bits/c++allocator.h \
 /usr/include/c++/4.4/ext/new_allocator.h /usr/include/c++/4.4/new \
 /usr/include/c++/4.4/exception /usr/include/c++/4.4/bits/stl_function.h \
 /usr/include/c++/4.4/backward/binders.h \
 /usr/include/c++/4.4/bits/stl_map.h \
 /usr/include/c++/4.4/initializer_list \
 /usr/include/c++/4.4/bits/stl_multimap.h ../threads/synchlist.h
list.o: ../threads/list.cc ../threads/copyright.h ../threads/list.h \
 ../threads/utility.h ../threads/bool.h ../machine/sysdep.h \
 ../threads/copyright.h /usr/include/stdio.h /usr/include/features.h \
 /usr/include/bits/predefs.h /usr/include/sys/cdefs.h \
 /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h \
 /usr/include/gnu/stubs-32.h \
 /usr/lib/gcc/i486-linux-gnu/4.4.3/include/stddef.h \
 /usr/include/bits/types.h /usr/include/bits/typesizes.h \
 /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
 ../threads/stdarg.h /usr/include/bits/stdio_lim.h \
 /usr/include/bits/sys_errlist.h /usr/include/string.h \
 /usr/include/xlocale.h
scheduler.o: ../threads/scheduler.cc ../threads/copyright.h \
 ../threads/scheduler.h ../threads/list.h ../threads/utility.h \
 ../threads/bool.h ../machine/sysdep.h ../threads/copyright.h \
 /usr/include/stdio.h /usr/include/features.h /usr/include/bits/predefs.h \
 /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
 /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
 /usr/lib/gcc/i486-linux-gnu/4.4.3/include/stddef.h \
 /usr/include/bits/types.h /usr/include/bits/typesizes.h \
 /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
 ../threads/stdarg.h /usr/include/bits/stdio_lim.h \
 /usr/include/bits/sys_errlist.h /usr/include/string.h \
 /usr/include/xlocale.h ../threads/thread.h ../threads/system.h \
 ../machine/interrupt.h ../threads/list.h ../machine/stats.h \
 ../machine/timer.h ../threads/utility.h ../threads/threadmanager.h \
 ../threads/synch.h ../threads/messagequeue.h /usr/include/c++/4.4/map \
 /usr/include/c++/4.4/bits/stl_tree.h \
 /usr/include/c++/4.4/bits/stl_algobase.h \
 /usr/include/c++/4.4/i486-linux-gnu/bits/c++config.h \
 /usr/include/c++/4.4/i486-linux-gnu/bits/os_defines.h \
 /usr/include/c++/4.4/i486-linux-gnu/bits/cpu_defines.h \
 /usr/include/c++/4.4/cstddef /usr/include/c++/4.4/bits/functexcept.h \
 /usr/include/c++/4.4/exception_defines.h \
 /usr/include/c++/4.4/bits/cpp_type_traits.h \
 /usr/include/c++/4.4/ext/type_traits.h \
 /usr/include/c++/4.4/ext/numeric_traits.h \
 /usr/include/c++/4.4/bits/stl_pair.h /usr/include/c++/4.4/bits/move.h \
 /usr/include/c++/4.4/bits/concept_check.h \
 /usr/include/c++/4.4/bits/stl_iterator_base_types.h \
 /usr/include/c++/4.4/bits/stl_iterator_base_funcs.h \
 /usr/include/c++/4.4/bits/stl_iterator.h \
 /usr/include/c++/4.4/debug/debug.h /usr/include/c++/4.4/bits/allocator.h \
 /usr/include/c++/4.4/i486-linux-gnu/bits/c++allocator.h \
 /usr/include/c++/4.4/ext/new_allocator.h /usr/include/c++/4.4/new \
 /usr/include/c++/4.4/exception /usr/include/c++/4.4/bits/stl_function.h \
 /usr/include/c++/4.4/backward/binders.h \
 /usr/include/c++/4.4/bits/stl_map.h \
 /usr/include/c++/4.4/initializer_list \
 /usr/include/c++/4.4/bits/stl_multimap.h ../threads/synchlist.h
synch.o: ../threads/synch.cc ../threads/copyright.h ../threads/synch.h \
 ../threads/thread.h ../threads/utility.h ../threads/bool.h \
 ../machine/sysdep.h ../threads/copyright.h /usr/include/stdio.h \
 /usr/include/features.h /usr/include/bits/predefs.h \
 /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
 /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
 /usr/lib/gcc/i486-linux-gnu/4.4.3/include/stddef.h \
 /usr/include/bits/types.h /usr/include/bits/typesizes.h \
 /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
 ../threads/stdarg.h /usr/include/bits/stdio_lim.h \
 /usr/include/bits/sys_errlist.h /usr/include/string.h \
 /usr/include/xlocale.h ../threads/list.h ../threads/system.h \
 ../threads/scheduler.h ../machine/interrupt.h ../threads/list.h \
 ../machine/stats.h ../machine/timer.h ../threads/utility.h \
 ../threads/threadmanager.h ../threads/messagequeue.h \
 /usr/include/c++/4.4/map /usr/include/c++/4.4/bits/stl_tree.h \
 /usr/include/c++/4.4/bits/stl_algobase.h \
 /usr/include/c++/4.4/i486-linux-gnu/bits/c++config.h \
 /usr/include/c++/4.4/i486-linux-gnu/bits/os_defines.h \
 /usr/include/c++/4.4/i486-linux-gnu/bits/cpu_defines.h \
 /usr/include/c++/4.4/cstddef /usr/include/c++/4.4/bits/functexcept.h \
 /usr/include/c++/4.4/exception_defines.h \
 /usr/include/c++/4.4/bits/cpp_type_traits.h \
 /usr/include/c++/4.4/ext/type_traits.h \
 /usr/include/c++/4.4/ext/numeric_traits.h \
 /usr/include/c++/4.4/bits/stl_pair.h /usr/include/c++/4.4/bits/move.h \
 /usr/include/c++/4.4/bits/concept_check.h \
 /usr/include/c++/4.4/bits/stl_iterator_base_types.h \
 /usr/include/c++/4.4/bits/stl_iterator_base_funcs.h \
 /usr/include/c++/4.4/bits/stl_iterator.h \
 /usr/include/c++/4.4/debug/debug.h /usr/include/c++/4.4/bits/allocator.h \
 /usr/include/c++/4.4/i486-linux-gnu/bits/c++allocator.h \
 /usr/include/c++/4.4/ext/new_allocator.h /usr/include/c++/4.4/new \
 /usr/include/c++/4.4/exception /usr/include/c++/4.4/bits/stl_function.h \
 /usr/include/c++/4.4/backward/binders.h \
 /usr/include/c++/4.4/bits/stl_map.h \
 /usr/include/c++/4.4/initializer_list \
 /usr/include/c++/4.4/bits/stl_multimap.h ../threads/synchlist.h
synchlist.o: ../threads/synchlist.cc ../threads/copyright.h \
 ../threads/synchlist.h ../threads/list.h ../threads/utility.h \
 ../threads/bool.h ../machine/sysdep.h ../threads/copyright.h \
 /usr/include/stdio.h /usr/include/features.h /usr/include/bits/predefs.h \
 /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
 /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
 /usr/lib/gcc/i486-linux-gnu/4.4.3/include/stddef.h \
 /usr/include/bits/types.h /usr/include/bits/typesizes.h \
 /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
 ../threads/stdarg.h /usr/include/bits/stdio_lim.h \
 /usr/include/bits/sys_errlist.h /usr/include/string.h \
 /usr/include/xlocale.h ../threads/synch.h ../threads/thread.h
system.o: ../threads/system.cc ../threads/copyright.h ../threads/system.h \
 ../threads/utility.h ../threads/bool.h ../machine/sysdep.h \
 ../threads/copyright.h /usr/include/stdio.h /usr/include/features.h \
 /usr/include/bits/predefs.h /usr/include/sys/cdefs.h \
 /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h \
 /usr/include/gnu/stubs-32.h \
 /usr/lib/gcc/i486-linux-gnu/4.4.3/include/stddef.h \
 /usr/include/bits/types.h /usr/include/bits/typesizes.h \
 /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
 ../threads/stdarg.h /usr/include/bits/stdio_lim.h \
 /usr/include/bits/sys_errlist.h /usr/include/string.h \
 /usr/include/xlocale.h ../threads/thread.h ../threads/scheduler.h \
 ../threads/list.h ../machine/interrupt.h ../threads/list.h \
 ../machine/stats.h ../machine/timer.h ../threads/utility.h \
 ../threads/threadmanager.h ../threads/synch.h ../threads/messagequeue.h \
 /usr/include/c++/4.4/map /usr/include/c++/4.4/bits/stl_tree.h \
 /usr/include/c++/4.4/bits/stl_algobase.h \
 /usr/include/c++/4.4/i486-linux-gnu/bits/c++config.h \
 /usr/include/c++/4.4/i486-linux-gnu/bits/os_defines.h \
 /usr/include/c++/4.4/i486-linux-gnu/bits/cpu_defines.h \
 /usr/include/c++/4.4/cstddef /usr/include/c++/4.4/bits/functexcept.h \
 /usr/include/c++/4.4/exception_defines.h \
 /usr/include/c++/4.4/bits/cpp_type_traits.h \
 /usr/include/c++/4.4/ext/type_traits.h \
 /usr/include/c++/4.4/ext/numeric_traits.h \
 /usr/include/c++/4.4/bits/stl_pair.h /usr/include/c++/4.4/bits/move.h \
 /usr/include/c++/4.4/bits/concept_check.h \
 /usr/include/c++/4.4/bits/stl_iterator_base_types.h \
 /usr/include/c++/4.4/bits/stl_iterator_base_funcs.h \
 /usr/include/c++/4.4/bits/stl_iterator.h \
 /usr/include/c++/4.4/debug/debug.h /usr/include/c++/4.4/bits/allocator.h \
 /usr/include/c++/4.4/i486-linux-gnu/bits/c++allocator.h \
 /usr/include/c++/4.4/ext/new_allocator.h /usr/include/c++/4.4/new \
 /usr/include/c++/4.4/exception /usr/include/c++/4.4/bits/stl_function.h \
 /usr/include/c++/4.4/backward/binders.h \
 /usr/include/c++/4.4/bits/stl_map.h \
 /usr/include/c++/4.4/initializer_list \
 /usr/include/c++/4.4/bits/stl_multimap.h ../threads/synchlist.h
thread.o: ../threads/thread.cc ../threads/copyright.h ../threads/thread.h \
 ../threads/utility.h ../threads/bool.h ../machine/sysdep.h \
 ../threads/copyright.h /usr/include/stdio.h /usr/include/features.h \
 /usr/include/bits/predefs.h /usr/include/sys/cdefs.h \
 /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h \
 /usr/include/gnu/stubs-32.h \
 /usr/lib/gcc/i486-linux-gnu/4.4.3/include/stddef.h \
 /usr/include/bits/types.h /usr/include/bits/typesizes.h \
 /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
 ../threads/stdarg.h /usr/include/bits/stdio_lim.h \
 /usr/include/bits/sys_errlist.h /usr/include/string.h \
 /usr/include/xlocale.h ../threads/switch.h ../threads/synch.h \
 ../threads/list.h ../threads/system.h ../threads/scheduler.h \
 ../machine/interrupt.h ../threads/list.h ../machine/stats.h \
 ../machine/timer.h ../threads/utility.h ../threads/threadmanager.h \
 ../threads/messagequeue.h /usr/include/c++/4.4/map \
 /usr/include/c++/4.4/bits/stl_tree.h \
 /usr/include/c++/4.4/bits/stl_algobase.h \
 /usr/include/c++/4.4/i486-linux-gnu/bits/c++config.h \
 /usr/include/c++/4.4/i486-linux-gnu/bits/os_defines.h \
 /usr/include/c++/4.4/i486-linux-gnu/bits/cpu_defines.h \
 /usr/include/c++/4.4/cstddef /usr/include/c++/4.4/bits/functexcept.h \
 /usr/include/c++/4.4/exception_defines.h \
 /usr/include/c++/4.4/bits/cpp_type_traits.h \
 /usr/include/c++/4.4/ext/type_traits.h \
 /usr/include/c++/4.4/ext/numeric_traits.h \
 /usr/include/c++/4.4/bits/stl_pair.h /usr/include/c++/4.4/bits/move.h \
 /usr/include/c++/4.4/bits/concept_check.h \
 /usr/include/c++/4.4/bits/stl_iterator_base_types.h \
 /usr/include/c++/4.4/bits/stl_iterator_base_funcs.h \
 /usr/include/c++/4.4/bits/stl_iterator.h \
 /usr/include/c++/4.4/debug/debug.h /usr/include/c++/4.4/bits/allocator.h \
 /usr/include/c++/4.4/i486-linux-gnu/bits/c++allocator.h \
 /usr/include/c++/4.4/ext/new_allocator.h /usr/include/c++/4.4/new \
 /usr/include/c++/4.4/exception /usr/include/c++/4.4/bits/stl_function.h \
 /usr/include/c++/4.4/backward/binders.h \
 /usr/include/c++/4.4/bits/stl_map.h \
 /usr/include/c++/4.4/initializer_list \
 /usr/include/c++/4.4/bits/stl_multimap.h ../threads/synchlist.h
utility.o: ../threads/utility.cc ../threads/copyright.h \
 ../threads/utility.h ../threads/bool.h ../machine/sysdep.h \
 ../threads/copyright.h /usr/include/stdio.h /usr/include/features.h \
 /usr/include/bits/predefs.h /usr/include/sys/cdefs.h \
 /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h \
 /usr/include/gnu/stubs-32.h \
 /usr/lib/gcc/i486-linux-gnu/4.4.3/include/stddef.h \
 /usr/include/bits/types.h /usr/include/bits/typesizes.h \
 /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
 ../threads/stdarg.h /usr/include/bits/stdio_lim.h \
 /usr/include/bits/sys_errlist.h /usr/include/string.h \
 /usr/include/xlocale.h ../threads/stdarg.h
threadtest.o: ../threads/threadtest.cc ../threads/copyright.h \
 ../threads/system.h ../threads/utility.h ../threads/bool.h \
 ../machine/sysdep.h ../threads/copyright.h /usr/include/stdio.h \
 /usr/include/features.h /usr/include/bits/predefs.h \
 /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
 /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
 /usr/lib/gcc/i486-linux-gnu/4.4.3/include/stddef.h \
 /usr/include/bits/types.h /usr/include/bits/typesizes.h \
 /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
 ../threads/stdarg.h /usr/include/bits/stdio_lim.h \
 /usr/include/bits/sys_errlist.h /usr/include/string.h \
 /usr/include/xlocale.h ../threads/thread.h ../threads/scheduler.h \
 ../threads/list.h ../machine/interrupt.h ../threads/list.h \
 ../machine/stats.h ../machine/timer.h ../threads/utility.h \
 ../threads/threadmanager.h ../threads/synch.h ../threads/messagequeue.h \
 /usr/include/c++/4.4/map /usr/include/c++/4.4/bits/stl_tree.h \
 /usr/include/c++/4.4/bits/stl_algobase.h \
 /usr/include/c++/4.4/i486-linux-gnu/bits/c++config.h \
 /usr/include/c++/4.4/i486-linux-gnu/bits/os_defines.h \
 /usr/include/c++/4.4/i486-linux-gnu/bits/cpu_defines.h \
 /usr/include/c++/4.4/cstddef /usr/include/c++/4.4/bits/functexcept.h \
 /usr/include/c++/4.4/exception_defines.h \
 /usr/include/c++/4.4/bits/cpp_type_traits.h \
 /usr/include/c++/4.4/ext/type_traits.h \
 /usr/include/c++/4.4/ext/numeric_traits.h \
 /usr/include/c++/4.4/bits/stl_pair.h /usr/include/c++/4.4/bits/move.h \
 /usr/include/c++/4.4/bits/concept_check.h \
 /usr/include/c++/4.4/bits/stl_iterator_base_types.h \
 /usr/include/c++/4.4/bits/stl_iterator_base_funcs.h \
 /usr/include/c++/4.4/bits/stl_iterator.h \
 /usr/include/c++/4.4/debug/debug.h /usr/include/c++/4.4/bits/allocator.h \
 /usr/include/c++/4.4/i486-linux-gnu/bits/c++allocator.h \
 /usr/include/c++/4.4/ext/new_allocator.h /usr/include/c++/4.4/new \
 /usr/include/c++/4.4/exception /usr/include/c++/4.4/bits/stl_function.h \
 /usr/include/c++/4.4/backward/binders.h \
 /usr/include/c++/4.4/bits/stl_map.h \
 /usr/include/c++/4.4/initializer_list \
 /usr/include/c++/4.4/bits/stl_multimap.h ../threads/synchlist.h
interrupt.o: ../machine/interrupt.cc ../threads/copyright.h \
 ../machine/interrupt.h ../threads/list.h ../threads/copyright.h \
 ../threads/utility.h ../threads/bool.h ../machine/sysdep.h \
 /usr/include/stdio.h /usr/include/features.h /usr/include/bits/predefs.h \
 /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
 /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
 /usr/lib/gcc/i486-linux-gnu/4.4.3/include/stddef.h \
 /usr/include/bits/types.h /usr/include/bits/typesizes.h \
 /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
 ../threads/stdarg.h /usr/include/bits/stdio_lim.h \
 /usr/include/bits/sys_errlist.h /usr/include/string.h \
 /usr/include/xlocale.h ../threads/system.h ../threads/thread.h \
 ../threads/scheduler.h ../threads/list.h ../machine/interrupt.h \
 ../machine/stats.h ../machine/timer.h ../threads/utility.h \
 ../threads/threadmanager.h ../threads/synch.h ../threads/messagequeue.h \
 /usr/include/c++/4.4/map /usr/include/c++/4.4/bits/stl_tree.h \
 /usr/include/c++/4.4/bits/stl_algobase.h \
 /usr/include/c++/4.4/i486-linux-gnu/bits/c++config.h \
 /usr/include/c++/4.4/i486-linux-gnu/bits/os_defines.h \
 /usr/include/c++/4.4/i486-linux-gnu/bits/cpu_defines.h \
 /usr/include/c++/4.4/cstddef /usr/include/c++/4.4/bits/functexcept.h \
 /usr/include/c++/4.4/exception_defines.h \
 /usr/include/c++/4.4/bits/cpp_type_traits.h \
 /usr/include/c++/4.4/ext/type_traits.h \
 /usr/include/c++/4.4/ext/numeric_traits.h \
 /usr/include/c++/4.4/bits/stl_pair.h /usr/include/c++/4.4/bits/move.h \
 /usr/include/c++/4.4/bits/concept_check.h \
 /usr/include/c++/4.4/bits/stl_iterator_base_types.h \
 /usr/include/c++/4.4/bits/stl_iterator_base_funcs.h \
 /usr/include/c++/4.4/bits/stl_iterator.h \
 /usr/include/c++/4.4/debug/debug.h /usr/include/c++/4.4/bits/allocator.h \
 /usr/include/c++/4.4/i486-linux-gnu/bits/c++allocator.h \
 /usr/include/c++/4.4/ext/new_allocator.h /usr/include/c++/4.4/new \
 /usr/include/c++/4.4/exception /usr/include/c++/4.4/bits/stl_function.h \
 /usr/include/c++/4.4/backward/binders.h \
 /usr/include/c++/4.4/bits/stl_map.h \
 /usr/include/c++/4.4/initializer_list \
 /usr/include/c++/4.4/bits/stl_multimap.h ../threads/synchlist.h
sysdep.o: ../machine/sysdep.cc ../threads/copyright.h \
 /usr/include/stdio.h /usr/include/features.h /usr/include/bits/predefs.h \
 /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
 /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
 /usr/lib/gcc/i486-linux-gnu/4.4.3/include/stddef.h \
 /usr/include/bits/types.h /usr/include/bits/typesizes.h \
 /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
 ../threads/stdarg.h /usr/include/bits/stdio_lim.h \
 /usr/include/bits/sys_errlist.h /usr/include/string.h \
 /usr/include/xlocale.h /usr/include/signal.h /usr/include/bits/sigset.h \
 /usr/include/bits/signum.h /usr/include/time.h \
 /usr/include/bits/siginfo.h /usr/include/bits/sigaction.h \
 /usr/include/bits/sigcontext.h /usr/include/bits/sigstack.h \
 /usr/include/sys/ucontext.h /usr/include/bits/pthreadtypes.h \
 /usr/include/bits/sigthread.h /usr/include/sys/types.h \
 /usr/include/endian.h /usr/include/bits/endian.h \
 /usr/include/bits/byteswap.h /usr/include/sys/select.h \
 /usr/include/bits/select.h /usr/include/bits/time.h \
 /usr/include/sys/sysmacros.h /usr/include/sys/time.h \
 /usr/include/sys/socket.h /usr/include/sys/uio.h /usr/include/bits/uio.h \
 /usr/include/bits/socket.h /usr/include/bits/sockaddr.h \
 /usr/include/asm/socket.h /usr/include/asm-generic/socket.h \
 /usr/include/asm/sockios.h /usr/include/asm-generic/sockios.h \
 /usr/include/sys/file.h /usr/include/fcntl.h /usr/include/bits/fcntl.h \
 /usr/include/sys/stat.h /usr/include/bits/stat.h /usr/include/sys/un.h \
 /usr/include/sys/mman.h /usr/include/bits/mman.h /usr/include/unistd.h \
 /usr/include/bits/posix_opt.h /usr/include/bits/environments.h \
 /usr/include/bits/confname.h /usr/include/getopt.h /usr/include/errno.h \
 /usr/include/bits/errno.h /usr/include/linux/errno.h \
 /usr/include/asm/errno.h /usr/include/asm-generic/errno.h \
 /usr/include/asm-generic/errno-base.h ../machine/interrupt.h \
 ../threads/list.h ../threads/copyright.h ../threads/utility.h \
 ../threads/bool.h ../machine/sysdep.h ../threads/system.h \
 ../threads/thread.h ../threads/scheduler.h ../threads/list.h \
 ../machine/interrupt.h ../machine/stats.h ../machine/timer.h \
 ../threads/utility.h ../threads/threadmanager.h ../threads/synch.h \
 ../threads/messagequeue.h /usr/include/c++/4.4/map \
 /usr/include/c++/4.4/bits/stl_tree.h \
 /usr/include/c++/4.4/bits/stl_algobase.h \
 /usr/include/c++/4.4/i486-linux-gnu/bits/c++config.h \
 /usr/include/c++/4.4/i486-linux-gnu/bits/os_defines.h \
 /usr/include/c++/4.4/i486-linux-gnu/bits/cpu_defines.h \
 /usr/include/c++/4.4/cstddef /usr/include/c++/4.4/bits/functexcept.h \
 /usr/include/c++/4.4/exception_defines.h \
 /usr/include/c++/4.4/bits/cpp_type_traits.h \
 /usr/include/c++/4.4/ext/type_traits.h \
 /usr/include/c++/4.4/ext/numeric_traits.h \
 /usr/include/c++/4.4/bits/stl_pair.h /usr/include/c++/4.4/bits/move.h \
 /usr/include/c++/4.4/bits/concept_check.h \
 /usr/include/c++/4.4/bits/stl_iterator_base_types.h \
 /usr/include/c++/4.4/bits/stl_iterator_base_funcs.h \
 /usr/include/c++/4.4/bits/stl_iterator.h \
 /usr/include/c++/4.4/debug/debug.h /usr/include/c++/4.4/bits/allocator.h \
 /usr/include/c++/4.4/i486-linux-gnu/bits/c++allocator.h \
 /usr/include/c++/4.4/ext/new_allocator.h /usr/include/c++/4.4/new \
 /usr/include/c++/4.4/exception /usr/include/c++/4.4/bits/stl_function.h \
 /usr/include/c++/4.4/backward/binders.h \
 /usr/include/c++/4.4/bits/stl_map.h \
 /usr/include/c++/4.4/initializer_list \
 /usr/include/c++/4.4/bits/stl_multimap.h ../threads/synchlist.h
stats.o: ../machine/stats.cc ../threads/copyright.h ../threads/utility.h \
 ../threads/copyright.h ../threads/bool.h ../machine/sysdep.h \
 /usr/include/stdio.h /usr/include/features.h /usr/include/bits/predefs.h \
 /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
 /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
 /usr/lib/gcc/i486-linux-gnu/4.4.3/include/stddef.h \
 /usr/include/bits/types.h /usr/include/bits/typesizes.h \
 /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
 ../threads/stdarg.h /usr/include/bits/stdio_lim.h \
 /usr/include/bits/sys_errlist.h /usr/include/string.h \
 /usr/include/xlocale.h ../machine/stats.h
timer.o: ../machine/timer.cc ../threads/copyright.h ../machine/timer.h \
 ../threads/utility.h ../threads/copyright.h ../threads/bool.h \
 ../machine/sysdep.h /usr/include/stdio.h /usr/include/features.h \
 /usr/include/bits/predefs.h /usr/include/sys/cdefs.h \
 /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h \
 /usr/include/gnu/stubs-32.h \
 /usr/lib/gcc/i486-linux-gnu/4.4.3/include/stddef.h \
 /usr/include/bits/types.h /usr/include/bits/typesizes.h \
 /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
 ../threads/stdarg.h /usr/include/bits/stdio_lim.h \
 /usr/include/bits/sys_errlist.h /usr/include/string.h \
 /usr/include/xlocale.h ../threads/system.h ../threads/utility.h \
 ../threads/thread.h ../threads/scheduler.h ../threads/list.h \
 ../machine/interrupt.h ../threads/list.h ../machine/stats.h \
 ../machine/timer.h ../threads/threadmanager.h ../threads/synch.h \
 ../threads/messagequeue.h /usr/include/c++/4.4/map \
 /usr/include/c++/4.4/bits/stl_tree.h \
 /usr/include/c++/4.4/bits/stl_algobase.h \
 /usr/include/c++/4.4/i486-linux-gnu/bits/c++config.h \
 /usr/include/c++/4.4/i486-linux-gnu/bits/os_defines.h \
 /usr/include/c++/4.4/i486-linux-gnu/bits/cpu_defines.h \
 /usr/include/c++/4.4/cstddef /usr/include/c++/4.4/bits/functexcept.h \
 /usr/include/c++/4.4/exception_defines.h \
 /usr/include/c++/4.4/bits/cpp_type_traits.h \
 /usr/include/c++/4.4/ext/type_traits.h \
 /usr/include/c++/4.4/ext/numeric_traits.h \
 /usr/include/c++/4.4/bits/stl_pair.h /usr/include/c++/4.4/bits/move.h \
 /usr/include/c++/4.4/bits/concept_check.h \
 /usr/include/c++/4.4/bits/stl_iterator_base_types.h \
 /usr/include/c++/4.4/bits/stl_iterator_base_funcs.h \
 /usr/include/c++/4.4/bits/stl_iterator.h \
 /usr/include/c++/4.4/debug/debug.h /usr/include/c++/4.4/bits/allocator.h \
 /usr/include/c++/4.4/i486-linux-gnu/bits/c++allocator.h \
 /usr/include/c++/4.4/ext/new_allocator.h /usr/include/c++/4.4/new \
 /usr/include/c++/4.4/exception /usr/include/c++/4.4/bits/stl_function.h \
 /usr/include/c++/4.4/backward/binders.h \
 /usr/include/c++/4.4/bits/stl_map.h \
 /usr/include/c++/4.4/initializer_list \
 /usr/include/c++/4.4/bits/stl_multimap.h ../threads/synchlist.h
threadmanager.o: ../threads/threadmanager.cc ../threads/threadmanager.h \
 ../threads/list.h ../threads/copyright.h ../threads/utility.h \
 ../threads/bool.h ../machine/sysdep.h ../threads/copyright.h \
 /usr/include/stdio.h /usr/include/features.h /usr/include/bits/predefs.h \
 /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
 /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
 /usr/lib/gcc/i486-linux-gnu/4.4.3/include/stddef.h \
 /usr/include/bits/types.h /usr/include/bits/typesizes.h \
 /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h \
 ../threads/stdarg.h /usr/include/bits/stdio_lim.h \
 /usr/include/bits/sys_errlist.h /usr/include/string.h \
 /usr/include/xlocale.h ../threads/thread.h ../threads/synch.h
messagequeue.o: ../threads/messagequeue.cc ../threads/messagequeue.h \
 /usr/include/c++/4.4/map /usr/include/c++/4.4/bits/stl_tree.h \
 /usr/include/c++/4.4/bits/stl_algobase.h \
 /usr/include/c++/4.4/i486-linux-gnu/bits/c++config.h \
 /usr/include/c++/4.4/i486-linux-gnu/bits/os_defines.h \
 /usr/include/features.h /usr/include/bits/predefs.h \
 /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h \
 /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h \
 /usr/include/c++/4.4/i486-linux-gnu/bits/cpu_defines.h \
 /usr/include/c++/4.4/cstddef \
 /usr/lib/gcc/i486-linux-gnu/4.4.3/include/stddef.h \
 /usr/include/c++/4.4/bits/functexcept.h \
 /usr/include/c++/4.4/exception_defines.h \
 /usr/include/c++/4.4/bits/cpp_type_traits.h \
 /usr/include/c++/4.4/ext/type_traits.h \
 /usr/include/c++/4.4/ext/numeric_traits.h \
 /usr/include/c++/4.4/bits/stl_pair.h /usr/include/c++/4.4/bits/move.h \
 /usr/include/c++/4.4/bits/concept_check.h \
 /usr/include/c++/4.4/bits/stl_iterator_base_types.h \
 /usr/include/c++/4.4/bits/stl_iterator_base_funcs.h \
 /usr/include/c++/4.4/bits/stl_iterator.h \
 /usr/include/c++/4.4/debug/debug.h /usr/include/c++/4.4/bits/allocator.h \
 /usr/include/c++/4.4/i486-linux-gnu/bits/c++allocator.h \
 /usr/include/c++/4.4/ext/new_allocator.h /usr/include/c++/4.4/new \
 /usr/include/c++/4.4/exception /usr/include/c++/4.4/bits/stl_function.h \
 /usr/include/c++/4.4/backward/binders.h \
 /usr/include/c++/4.4/bits/stl_map.h \
 /usr/include/c++/4.4/initializer_list \
 /usr/include/c++/4.4/bits/stl_multimap.h ../threads/synchlist.h \
 ../threads/copyright.h ../threads/list.h ../threads/utility.h \
 ../threads/bool.h ../machine/sysdep.h ../threads/copyright.h \
 /usr/include/stdio.h /usr/include/bits/types.h \
 /usr/include/bits/typesizes.h /usr/include/libio.h \
 /usr/include/_G_config.h /usr/include/wchar.h ../threads/stdarg.h \
 /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h \
 /usr/include/string.h /usr/include/xlocale.h ../threads/synch.h \
 ../threads/thread.h ../threads/system.h ../threads/scheduler.h \
 ../machine/interrupt.h ../threads/list.h ../machine/stats.h \
 ../machine/timer.h ../threads/utility.h ../threads/threadmanager.h
# DEPENDENCIES MUST END AT END OF FILE
# IF YOU PUT STUFF HERE IT WILL GO AWAY
# see make depend above
