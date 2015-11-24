#ifndef _Util_h__
#define _Util_h__

#include <wtypes.h>
#include <psapi.h>
#include <Shlwapi.h>
#include <io.h>
#include <cctype>

#include "uft8.h"

#  define ATTR_NORETURN
#  define ATTR_PRINTF(F, V)
#  define ATTR_DEPRECATED

#define ASSERT_BEGIN __pragma(warning(push)) __pragma(warning(disable: 4127))
#define ASSERT_END __pragma(warning(pop))

#define WOW_MOP_16135 16135

// hook entry structure
// stores the offsets which are will be hooked
// every different client version should has different offsets
typedef struct {
    // offset of NetClient::Send2 to sniff client packets
    DWORD send_2;
    // offset of NetClient::ProcessMessage to sniff server packets
    DWORD receive;
    // offset of client locale "xxXX"
    DWORD locale;
} HookEntry;


bool Utf8toWStr(char const* utf8str, size_t csize, wchar_t* wstr, size_t& wsize);
inline bool Utf8toWStr(const std::string& utf8str, wchar_t* wstr, size_t& wsize)
{
    return Utf8toWStr(utf8str.c_str(), utf8str.size(), wstr, wsize);
}

inline bool char_isspace(char c)
{
    return std::isspace(static_cast<unsigned char>(c)) != 0;
}

inline void ctolower(char* s)
{
    int len = strlen(s);
    for (int i = 0; i < len; ++i)
        s[i] = tolower(s[i]);
}

inline void stolower(std::string& s)
{
    for (size_t i = 0; i < s.size(); ++i)
        s[i] = tolower(s[i]);
}

namespace Program
{
    static bool Is64Bit()
    {
        #if defined(_WIN64)
            return true;
        #endif

        return false;
    }

    static void Splash()
    {
        printf("https://github.com/Zedron/Whiff\n");
        printf("____    __    ____  __    __   __   _______  _______ \n");
        printf("\\   \\  /  \\  /   / |  |  |  | |  | |   ____||   ____|\n");
        printf(" \\   \\/    \\/   /  |  |__|  | |  | |  |__   |  |__   \n");
        printf("  \\            /   |   __   | |  | |   __|  |   __|  \n");
        printf("   \\    /\\    /    |  |  |  | |  | |  |     |  |     \n");
        printf("    \\__/  \\__/     |__|  |__| |__| |__|     |__|     \n");
        printf("                                                     \n");
        printf("Type quit or CTRL-C to detach the sniffer\n\n");
    }

    DECLSPEC_NORETURN void Assert(char const* file, int line, char const* function, char const* message) ATTR_NORETURN;
    DECLSPEC_NORETURN void Assert(char const* file, int line, char const* function, char const* message, char const* format, ...) ATTR_NORETURN ATTR_PRINTF(5, 6);
}

#define WPAssert(cond, ...) ASSERT_BEGIN do { if (!(cond)) Program::Assert(__FILE__, __LINE__, __FUNCTION__, #cond, ##__VA_ARGS__); } while(0) ASSERT_END
#define ASSERT WPAssert

enum Expansions
{
    EXPANSION_NONE,
    EXPANSION_VANILLA,
    EXPANSION_TBC,
    EXPANSION_WOTLK,
    EXPANSION_CATA,
    EXPANSION_MOP,
    EXPANSION_WOD,
    EXPANSION_LEGION,
    MAX_EXPANSION
};


static Expansions GetExpansion(DWORD build)
{
    // 7.0.1
    if (build >= 20740)
        return EXPANSION_LEGION;

    // 6.0.2
    if (build >= 19033)
        return EXPANSION_WOD;

    // 5.0.4
    if (build >= 16016)
        return EXPANSION_MOP;

    // 4.0.1
    if (build >= 13164)
        return EXPANSION_CATA;

    // 3.0.2
    if (build >= 9056)
        return EXPANSION_WOTLK;

    // 2.0.1
    if (build >= 6180)
        return EXPANSION_TBC;

    // 1.1.0
    if (build >= 4044)
        return EXPANSION_VANILLA;

    return EXPANSION_NONE;
}

bool WStrToUtf8(std::wstring const& wstr, std::string& utf8str);
bool consoleToUtf8(const std::string& conStr, std::string& utf8str);

WORD GetBuildNumberFromProcess(HANDLE hProcess = NULL);

#endif


