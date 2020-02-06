# Subterranean Rose Project
# Python module: srlc.compiler
# Author: HenryAWE
#
# This file is licensed under GPLv3


import os, sys
from enum import IntEnum

class option(IntEnum):
    ERROR_CHECKING = 1
    DISPLAY = 2
    COMPILE = 3

class compiler:
    @staticmethod
    def emit_warning(info, file = "<string>", line = 0, fatal = False):
        print("[compiler] [%s - %d]: "%(file, line), info)
        if fatal:
            sys.exit(1)

    stop_on_err = False
    mode = option.ERROR_CHECKING
    verbose = 0
    data = {}

    def push_var(self, strid, tr):
        items = self.data
        sepid = strid.split('.')
        for id in sepid:
            if id not in items:
                if id == sepid[-1]:
                    items[id] = [{}, tr]
                    break
                else:
                    items[id] = [{}, None]
            items = items[id][0]

    def parse_string(self, string, file, line):
        string.strip()
        if(not string or string[0] == '\n'):
            return
        if(string[0] == '#'):
            # This line is a comment
            return
        if(string[0] != '@'):
            # Syntax error
            self.emit_warning("Syntax error - %s"%string, file=file, line=line, fatal=self.stop_on_err)
            return
        eqidx = string.find('=')
        if(eqidx == -1):
            self.emit_warning("Missing '='", file=file, line=line, fatal=self.stop_on_err)
            return
        strid = string[1:eqidx].strip()
        tr = string[eqidx+1:].strip()
        if(tr[0] != '"' or tr[-1] != '"'):
            self.emit_warning('''Missing '"' at the begin/end of the translation string ''', file=file, line=line, fatal=self.stop_on_err)
            return
        # Remove the '"' characters around the string
        tr = tr[1:-1]
        if self.verbose>=3 or self.mode==option.DISPLAY:
            print("[compiler]", strid.split('.'), "= \"%s\""%tr)

        if self.mode!=option.COMPILE:
            return
        self.push_var(strid, tr)


    def load(self, filename):
        f = open(filename, encoding="utf-8")
        if self.verbose>=1:
            print("[compiler] Loading file \"%s\""%os.path.basename(filename))
        line_number = 0

        for string in f.read().splitlines():
            line_number += 1
            self.parse_string(string, os.path.basename(filename), line_number)
