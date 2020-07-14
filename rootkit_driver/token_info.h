#pragma once
#include <Ntifs.h>
typedef struct _EX_FAST_REF {
    union {
        PVOID Object;
        ULONG RefCnt : 3;
        ULONG Value;
    };
} EX_FAST_REF, * PEX_FAST_REF;


constexpr int TOKEN_OFFSET = 0xf8;
constexpr int TOKEN_OFFSET_SIDCOUNT = 0x7c;
constexpr int TOKEN_OFFSET_PRIV = 0x040;
constexpr int TOKEN_OFFSET_ENABLED = 0x048;
constexpr int TOKEN_OFFSET_DEFAULT = 0x050;

