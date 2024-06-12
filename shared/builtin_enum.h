#ifndef BUILTIN_ENUM_H
#define BUILTIN_ENUM_H

enum Builtin {
    GETPID,
    CONS_PUTBYTES,
    START,
    EXIT,
    KILL,

    MON_NOM,
    GETPRIO,
    CHPRIO,

    PCOUNT,
    PCREATE,
    PDELETE,
    PRECEIVE,
    PRESET,
    PSEND,

    CLOCK_SETTINGS,
    CURRENT_CLOCK,

    WAIT_CLOCK,
    WAITPID,

    CONS_ECHO,
    CONS_READ,
    CONS_WRITE
};

#endif