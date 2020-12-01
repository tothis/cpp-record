#ifndef TEST_LIB

#define TEST_LIB

/**
 * extern "C" 使用C函数命名规则
 *  __declspec(dllexport) 导出函数
 * Linux不需要声明导出函数
 *
 * Windows编译为DLL g++ test-lib.cpp -fPIC -shared -o test-lib.dll
 * Linux编译为SO g++ test-lib.cpp -fPIC -shared -o libtest-lib.so
 */
#ifdef _WIN32 // https://docs.microsoft.com/cpp/preprocessor/predefined-macros
extern "C" __declspec(dllexport) int test_add(int a, int b);
#else
extern "C" int test_add(int a, int b);
#endif

#endif