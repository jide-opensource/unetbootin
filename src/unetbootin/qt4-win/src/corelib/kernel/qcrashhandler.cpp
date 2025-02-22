/****************************************************************************
**
** Copyright (C) 1992-2008 Trolltech ASA. All rights reserved.
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License versions 2.0 or 3.0 as published by the Free Software
** Foundation and appearing in the files LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file.  Alternatively you may (at
** your option) use any later version of the GNU General Public
** License if such license has been publicly approved by Trolltech ASA
** (or its successors, if any) and the KDE Free Qt Foundation. In
** addition, as a special exception, Trolltech gives you certain
** additional rights. These rights are described in the Trolltech GPL
** Exception version 1.2, which can be found at
** http://www.trolltech.com/products/qt/gplexception/ and in the file
** GPL_EXCEPTION.txt in this package.
**
** Please review the following information to ensure GNU General
** Public Licensing requirements will be met:
** http://trolltech.com/products/qt/licenses/licensing/opensource/. If
** you are unsure which license is appropriate for your use, please
** review the following information:
** http://trolltech.com/products/qt/licenses/licensing/licensingoverview
** or contact the sales department at sales@trolltech.com.
**
** In addition, as a special exception, Trolltech, as the sole
** copyright holder for Qt Designer, grants users of the Qt/Eclipse
** Integration plug-in the right for the Qt/Eclipse Integration to
** link to functionality provided by Qt Designer and its related
** libraries.
**
** This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
** INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE. Trolltech reserves all rights not expressly
** granted herein.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

/*************************************************************************
 *
 * stacktrace.c 1.2 1998/12/21
 *
 * Copyright (c) 1998 by Bjorn Reese <breese@imada.ou.dk>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
 * MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE AUTHORS AND
 * CONTRIBUTORS ACCEPT NO RESPONSIBILITY IN ANY CONCEIVABLE MANNER.
 *
 ************************************************************************/

#include "qplatformdefs.h"
#include "private/qcrashhandler_p.h"
#include "qbytearray.h" // for qvsnprintf()

#ifndef QT_NO_CRASHHANDLER

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

QT_BEGIN_NAMESPACE

QtCrashHandler QSegfaultHandler::callback = 0;

#if defined(__GLIBC__) && (__GLIBC__ >= 2) && !defined(__UCLIBC__) && !defined(QT_LSB)
QT_BEGIN_INCLUDE_NAMESPACE
# include "qstring.h"
# include <execinfo.h>
QT_END_INCLUDE_NAMESPACE

static void print_backtrace(FILE *outb)
{
    void *stack[128];
    int stack_size = backtrace(stack, sizeof(stack) / sizeof(void *));
    char **stack_symbols = backtrace_symbols(stack, stack_size);
    fprintf(outb, "Stack [%d]:\n", stack_size);
    if(FILE *cppfilt = popen("c++filt", "rw")) {
        dup2(fileno(outb), fileno(cppfilt));
        for(int i = stack_size-1; i>=0; --i)
            fwrite(stack_symbols[i], 1, strlen(stack_symbols[i]), cppfilt);
        pclose(cppfilt);
    } else {
        for(int i = stack_size-1; i>=0; --i)
            fprintf(outb, "#%d  %p [%s]\n", i, stack[i], stack_symbols[i]);
    }
}
static void init_backtrace(char **, int)
{
}

#else /* Don't use the GLIBC callback */
/* Code sourced from: */
QT_BEGIN_INCLUDE_NAMESPACE
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#if defined(Q_OS_IRIX) && defined(USE_LIBEXC)
# include <libexc.h>
#endif
QT_END_INCLUDE_NAMESPACE


static char *globalProgName = NULL;
static bool backtrace_command(FILE *outb, const char *format, ...)
{

    bool ret = false;
    char buffer[50];

    /*
     * Please note that vsnprintf() is not ASync safe (ie. cannot safely
     * be used from a signal handler.) If this proves to be a problem
     * then the cmd string can be built by more basic functions such as
     * strcpy, strcat, and a home-made integer-to-ascii function.
     */
    va_list args;
    char cmd[512];
    va_start(args, format);
    qvsnprintf(cmd, 512, format, args);
    va_end(args);

    char *foo = cmd;
#if 0
    foo = "echo hi";
#endif
    if(FILE *inb = popen(foo, "r")) {
        while(!feof(inb)) {
            int len = fread(buffer, 1, sizeof(buffer), inb);
            if(!len)
                break;
            if(!ret) {
                fwrite("Output from ", 1, strlen("Output from "), outb);
                strtok(cmd, " ");
                fwrite(cmd, 1, strlen(cmd), outb);
                fwrite("\n", 1, 1, outb);
                ret = true;
            }
            fwrite(buffer, 1, len, outb);
        }
        fclose(inb);
    }
    return ret;
}

static void init_backtrace(char **argv, int argc)
{
    if(argc >= 1)
        globalProgName = argv[0];
}

static void print_backtrace(FILE *outb)
{
    /*
     * In general dbx seems to do a better job than gdb.
     *
     * Different dbx implementations require different flags/commands.
     */
#if defined(Q_OS_AIX)
    if(backtrace_command(outb, "dbx -a %d 2>/dev/null <<EOF\n"
                         "where\n"
                         "detach\n"
                         "EOF\n",
                         (int)getpid()))
        return;
    if(backtrace_command(outb, "gdb -q %s %d 2>/dev/null <<EOF\n"
                         "set prompt\n"
                         "where\n"
                         "detach\n"
                         "quit\n"
                         "EOF\n",
                         globalProgName, (int)getpid()))
        return;
#elif defined(Q_OS_FREEBSD)
    /*
     * FreeBSD insists on sending a SIGSTOP to the process we
     * attach to, so we let the debugger send a SIGCONT to that
     * process after we have detached.
     */
    if(backtrace_command(outb, "gdb -q %s %d 2>/dev/null <<EOF\n"
                         "set prompt\n"
                         "where\n"
                         "detach\n"
                         "shell kill -CONT %d\n"
                         "quit\n"
                         "EOF\n",
                         globalProgName, (int)getpid(), (int)getpid()))
        return;
#elif defined(Q_OS_HPUX)
    /*
     * HP decided to call their debugger xdb.
     *
     * This does not seem to work properly yet. The debugger says
     * "Note: Stack traces may not be possible until you are
     *  stopped in user code." on HP-UX 09.01
     *
     * -L = line-oriented interface.
     * "T [depth]" gives a stacktrace with local variables.
     * The final "y" is confirmation to the quit command.
     */
    if(backtrace_command(outb, "xdb -P %d -L %s 2>&1 <<EOF\n"
                         "T 50\n"
                         "q\ny\n"
                         "EOF\n",
                         (int)getpid(), globalProgName))
        return;
    if(backtrace_command(outb, "gdb -q %s %d 2>/dev/null <<EOF\n"
                         "set prompt\n"
                         "where\n"
                         "detach\n"
                         "quit\n"
                         "EOF\n",
                         globalProgName, (int)getpid()))
        return;
#elif defined(Q_OS_IRIX)
    /*
     * "set $page=0" drops hold mode
     * "dump ." displays the contents of the variables
     */
    if(backtrace_command(outb, "dbx -p %d 2>/dev/null <<EOF\n"
                         "set \\$page=0\n"
                         "where\n"
# if !defined(__GNUC__)
                         /* gcc does not generate this information */
                         "dump .\n"
# endif
                         "detach\n"
                         "EOF\n",
                         (int)getpid()))
        return;

# if defined(USE_LIBEXC)
    if(trace_back_stack_and_print())
        return;
# endif
    if(backtrace_command(outb, "gdb -q %s %d 2>/dev/null <<EOF\n"
                         "set prompt\n"
                         "where\n"
                         "echo ---\\n\n"
                         "frame 5\n"      /* Skip signal handler frames */
                         "set \\$x = 50\n"
                         "while (\\$x)\n" /* Print local variables for each frame */
                         "info locals\n"
                         "up\n"
                         "set \\$x--\n"
                         "end\n"
                         "echo ---\\n\n"
                         "detach\n"
                         "quit\n"
                         "EOF\n",
                         globalProgName, (int)getpid()))
        return;
#elif defined(Q_OS_OSF)
    if(backtrace_command(outb, "dbx -pid %d %s 2>/dev/null <<EOF\n"
                         "where\n"
                         "detach\n"
                         "quit\n"
                         "EOF\n",
                         (int)getpid(), globalProgName))
        return;
    if(backtrace_command(outb, "gdb -q %s %d 2>/dev/null <<EOF\n"
                         "set prompt\n"
                         "where\n"
                         "detach\n"
                         "quit\n"
                         "EOF\n",
                         globalProgName, (int)getpid()))
        return;
#elif defined(Q_OS_SCO)
    /*
     * SCO OpenServer dbx is like a catch-22. The 'detach' command
     * depends on whether ptrace(S) support detaching or not. If it
     * is supported then 'detach' must be used, otherwise the process
     * will be killed upon dbx exit. If it isn't supported then 'detach'
     * will cause the process to be killed. We do not want it to be
     * killed.
     *
     * Out of two evils, the omission of 'detach' was chosen because
     * it worked on our system.
     */
    if(backtrace_command(outb, "dbx %s %d 2>/dev/null <<EOF\n"
                         "where\n"
                         "quit\nEOF\n",
                         globalProgName, (int)getpid()))
        return;
    if(backtrace_command(outb, "gdb -q %s %d 2>/dev/null <<EOF\n"
                         "set prompt\n"
                         "where\n"
                         "detach\n"
                         "quit\n"
                         "EOF\n",
                         globalProgName, (int)getpid()))
        return;
#elif defined(Q_OS_SOLARIS)
    if(backtrace_command(outb, "dbx %s %d 2>/dev/null <<EOF\n"
                         "where\n"
                         "detach\n"
                         "EOF\n",
                         globalProgName, (int)getpid()))
        return;
    if(backtrace_command(outb, "gdb -q %s %d 2>/dev/null <<EOF\n"
                         "set prompt\n"
                         "where\n"
                         "echo ---\\n\n"
                         "frame 5\n"      /* Skip signal handler frames */
                         "set \\$x = 50\n"
                         "while (\\$x)\n" /* Print local variables for each frame */
                         "info locals\n"
                         "up\n"
                         "set \\$x--\n"
                         "end\n"
                         "echo ---\\n\n"
                         "detach\n"
                         "quit\n"
                         "EOF\n",
                         globalProgName, (int)getpid()))
        return;
    if(backtrace_command(outb, "/usr/proc/bin/pstack %d",
                         (int)getpid()))
        return;
    /*
     * Other Unices (AIX, HPUX, SCO) also have adb, but
     * they seem unable to attach to a running process.)
     */
    if(backtrace_command(outb, "adb %s 2>&1 <<EOF\n"
                         "0t%d:A\n" /* Attach to pid */
                         "\\$c\n"   /* print stacktrace */
                         ":R\n"     /* Detach */
                         "\\$q\n"   /* Quit */
                         "EOF\n",
                         globalProgName, (int)getpid()))
        return;
#else /* All other platforms */
    /*
     * TODO: SCO/UnixWare 7 must be something like (not tested)
     *  debug -i c <pid> <<EOF\nstack -f 4\nquit\nEOF\n
     */
# if !defined(__GNUC__)
    if(backtrace_command(outb, "dbx %s %d 2>/dev/null <<EOF\n"
                         "where\n"
                         "detach\n"
                         "EOF\n",
                         globalProgName, (int)getpid()))
        return;
# endif
    if(backtrace_command(outb, "gdb -q %s %d 2>/dev/null <<EOF\n"
                         "set prompt\n"
                         "where\n"
#if 0
                         "echo ---\\n\n"
                         "frame 4\n"
                         "set \\$x = 50\n"
                         "while (\\$x)\n"
                         "info locals\n"
                         "up\n"
                         "set \\$x--\n"
                         "end\n"
                         "echo ---\\n\n"
#endif
                         "detach\n"
                         "quit\n"
                         "EOF\n",
                         globalProgName, (int)getpid()))
        return;
#endif
    const char debug_err[] = "No debugger found\n";
    fwrite(debug_err, strlen(debug_err), 1, outb);
}
/* end of copied code */
#endif


void qt_signal_handler(int sig)
{
    signal(sig, SIG_DFL);
    if(QSegfaultHandler::callback) {
        (*QSegfaultHandler::callback)();
        _exit(1);
    }
    FILE *outb = stderr;
    if(char *crash_loc = ::getenv("QT_CRASH_OUTPUT")) {
        if(FILE *new_outb = fopen(crash_loc, "w")) {
            fprintf(stderr, "Crash (backtrace written to %s)!!!\n", crash_loc);
            outb = new_outb;
        }
    } else {
        fprintf(outb, "Crash!!!\n");
    }
    print_backtrace(outb);
    if(outb != stderr)
        fclose(outb);
    _exit(1);
}


void
QSegfaultHandler::initialize(char **argv, int argc)
{
    init_backtrace(argv, argc);

    struct sigaction SignalAction;
    SignalAction.sa_flags = 0;
    SignalAction.sa_handler = qt_signal_handler;
    sigemptyset(&SignalAction.sa_mask);
    sigaction(SIGSEGV, &SignalAction, NULL);
    sigaction(SIGBUS, &SignalAction, NULL);
}

QT_END_NAMESPACE

#endif // QT_NO_CRASHHANDLER
