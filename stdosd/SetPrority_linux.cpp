//Copyright (C) 2017 Ambiesoft All rights reserved.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions
//are met:
//1. Redistributions of source code must retain the above copyright
//notice, this list of conditions and the following disclaimer.
//2. Redistributions in binary form must reproduce the above copyright
//notice, this list of conditions and the following disclaimer in the
//documentation and/or other materials provided with the distribution.
//
//THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
//ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
//FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
//OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
//LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
//OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
//SUCH DAMAGE.

#include <unistd.h>
#include <sys/syscall.h>   /* For SYS_xxx definitions */

#include <sys/time.h>
#include <sys/resource.h>



#include "SetPrority.h"


namespace Ambiesoft {
namespace stdosd {
namespace Process {



static inline int ioprio_set(int which, int who, int ioprio)
{
    return syscall(SYS_ioprio_set, which, who, ioprio);
}

enum {
    IOPRIO_CLASS_NONE,
    IOPRIO_CLASS_RT,
    IOPRIO_CLASS_BE,
    IOPRIO_CLASS_IDLE,
};

enum {
    IOPRIO_WHO_PROCESS = 1,
    IOPRIO_WHO_PGRP,
    IOPRIO_WHO_USER,
};

#define IOPRIO_CLASS_SHIFT	(13)
#define IOPRIO_PRIO_MASK	((1UL << IOPRIO_CLASS_SHIFT) - 1)

#define IOPRIO_PRIO_CLASS(mask)	((mask) >> IOPRIO_CLASS_SHIFT)
#define IOPRIO_PRIO_DATA(mask)	((mask) & IOPRIO_PRIO_MASK)
#define IOPRIO_PRIO_VALUE(class, data)	(((class) << IOPRIO_CLASS_SHIFT) | data)


static int setpriorityStuff(id_t pid,
                            CPUPRIORITY cpuPriority,
                            IOPRIORITY ioPriority)
{
    int prio = -1;
    switch(cpuPriority)
    {
    case CPU_HIGH:
        prio = -19;
        break;
    case CPU_ABOVENORMAL:
        prio = -5;
        break;
    case CPU_NORMAL:
        prio=0;
        break;
    case CPU_BELOWNORMAL:
        prio=5;
        break;
    case CPU_IDLE:
        prio=19;
        break;
    default:
        break;
    }



    int firstError = 0;
    int err = setpriority(PRIO_PROCESS, pid, prio);
    if(err != 0)
    {
        //        ss <<
        //              "setpriority(" <<
        //              prio <<
        //              ") failed with " <<
        //              err <<
        //              "." <<
        //              std::endl;

        if(firstError==0)
            firstError=err;
    }

    int ioprioclass = IOPRIO_CLASS_NONE;
    switch(ioPriority)
    {
    case IO_HIGH:
    case IO_ABOVENORMAL:
        ioprioclass = IOPRIO_CLASS_RT;
        break;
    case IO_BELOWNORMAL:
        ioprioclass = IOPRIO_CLASS_BE;
        break;
    case IO_IDLE:
        ioprioclass = IOPRIO_CLASS_IDLE;
        break;
    default:
        break;
    }

    if(ioprioclass != IOPRIO_CLASS_NONE)
    {
        // 7 is from https://github.com/karelzak/util-linux/blob/master/schedutils/ionice.c
        int iopriovalue = IOPRIO_PRIO_VALUE(IOPRIO_CLASS_IDLE,7);
        err = ioprio_set(IOPRIO_WHO_PROCESS, pid, iopriovalue);
        if(err != 0)
        {
            //            failed = true;
            //            ss <<
            //                      "ioprio_set(" <<
            //                      ioprioclass <<
            //                      ") failed with " <<
            //                      err <<
            //                      "." <<
            //                      std::endl;

            if(firstError==0)
                firstError=err;
        }
    }
    return firstError;
}
int SetProirity(uint64_t  pid,
                CPUPRIORITY cpuPriority,
                IOPRIORITY ioPriority,
                MEMORYPRIORITY)
{
    id_t pidid = (long)pid;
    return setpriorityStuff(pidid,
                            cpuPriority,
                            ioPriority);
}



}  } } // namespace Ambiesoft
