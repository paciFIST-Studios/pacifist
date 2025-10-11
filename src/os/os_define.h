// paciFIST Studios. 2025. MIT License

#ifndef PF_OS_DEFINE_H
#define PF_OS_DEFINE_H


#define MAX_PATH_LINUX 4096
#define MAX_PATH_WINDOWS 255


#ifdef _WIN32
#define OS_WINDOWS
#define OS_X86
#endif

#ifdef _WIN64
#define OS_WINDOWS
#define OS_X64
#endif

#ifdef __CYGWIN__
#define OS_WINDOWS
#endif

#ifdef __linux__
#define OS_LINUX
#endif


// finds PATH_MAX for the current OS and uses that
#define PF_PATH_MAX MAX_PATH_LINUX



#endif //PF_OS_DEFINE_H
