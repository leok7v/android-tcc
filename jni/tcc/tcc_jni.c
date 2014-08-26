#include "manifest.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <android/log.h>

#include "libtcc.h"

/* this function is called by the generated code */
int add(int a, int b)
{
    trace("a=%d b=%d", a, b);
    return a + b;
}

void bar(const char* fmt, ...) {
    va_list vl;
    va_start(vl, fmt);
    __android_log_vprint(ANDROID_LOG_VERBOSE, "tcc-exec", fmt, vl);
    va_end(vl);
}

// "#define printf(const char* fmt, ...) __android_log_print(1, \"tcc-exec\", fmt, __VA_ARGS__)\n"

char my_program[] =
"int fib(int n)\n"
"{\n"
"    if (n <= 2)\n"
"        return 1;\n"
"    else\n"
"        return fib(n-1) + fib(n-2);\n"
"}\n"
"\n"
"int main(int argc, const char** argv)\n"
"{\n"
"    printf(\"main!\\n\");\n"
"    printf(\"argc=%d\", argc);\n"
"    printf(\"argv[0]=%s\", argv[0]);\n"
"    printf(\"argv[1]=%s\", argv[1]);\n"
/*
"    char* bad = 0x0;\n"
"    *bad = 1;\n"
*/
"    return 0;\n"
"}\n"
"\n"
"int foo(int n)\n"
"{\n"
"    printf(\"Hello World!\\n\");\n"
"    printf(\"fib(%d) = %d\\n\", n, fib(n));\n"
"    printf(\"add(%d, %d) = %d\\n\", n, 2 * n, add(n, 2 * n));\n"
"    char* bad = 0x0;\n"
"    *bad = 1;\n"
"    return 0;\n"
"}\n";

static void error_func(void *opaque, const char *msg) {
    trace("%s", msg);
}

int test()
{
    TCCState *s;
    int (*func)(int);

    s = tcc_new();
    if (!s) {
        trace("Could not create tcc state");
        return -1;
    }

    tcc_set_error_func(s, null, error_func);

    /* if tcclib.h and libtcc1.a are not installed, where can we find them */

    tcc_set_lib_path(s, "/system/lib"); // libc.so

    /* MUST BE CALLED before any compilation */
    tcc_set_output_type(s, TCC_OUTPUT_MEMORY);

/*
http://bellard.org/tcc/tcc-doc.html
`-g'
Generate run time debug information so that you get clear run time error messages: test.c:68: in function 'test5()': dereferencing invalid pointer instead of the laconic Segmentation fault.
`-b'
Generate additional support code to check memory allocations and array/pointer bounds. `-g' is implied. Note that the generated code is slower and bigger in this case.
`-bt N'
Display N callers in stack traces. This is useful with `-g' or `-b'.
*/
    tcc_set_options(s, "g");
    tcc_set_options(s, "b");
    tcc_set_options(s, "bt 30");

    if (tcc_compile_string(s, my_program) == -1) {
        trace("Could not tcc_compile_string");
        return -1;
    }

    /* as a test, we add a symbol that the compiled program can use.
       You may also open a dll with tcc_add_dll() and use symbols from that */
    tcc_add_symbol(s, "add", add);

//  tcc_add_file(s, "/system/lib/liblog.so");
    tcc_add_symbol(s, "printf", bar);

    if (0) {
        /* relocate the code */
        if (tcc_relocate(s, TCC_RELOCATE_AUTO) < 0) {
            trace("Could not tcc_relocate");
            return -1;
        }
        /* get entry symbol */
        func = tcc_get_symbol(s, "foo");
        if (!func) {
            trace("Could not tcc_get_symbol");
            return -1;
        }
        /* run the code */
        func(32);
    } else {
        char* args[] = {"arg1", "arg2"};
        tcc_run(s, countof(args), args);
    }
    /* delete the state */
    tcc_delete(s);

    trace("done");
    return 0;
}

#include <jni.h>
#include <android/log.h>

void Java_net_kuznetsovs_tc_TCC_test(JNIEnv* env, jobject that) {
    test();
}
