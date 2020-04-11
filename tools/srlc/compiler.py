# Subterranean Rose Project
# Python module: srlc.compiler
# Author: HenryAWE
#
# This file is licensed under GPLv3


import os, sys
from enum import IntEnum
import struct

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
        for idx in range(0, len(sepid)):
            id = sepid[idx]
            if id not in items:
                if idx == len(sepid)-1:
                    items[id] = [{}, tr]
                    break
                else:
                    items[id] = [{}, None]
            else:
                if idx == len(sepid)-1:
                    items[id][1] = tr
                    break
            items = items[id][0]

    @staticmethod
    def translate_str(string):
        string = string.replace('\\\\', '\\')
        string = string.replace('\\n', '\n')
        string = string.replace('\\t', '\t')
        string = string.replace('\\f', '\f')
        string = string.replace('\\v', '\v')
        string = string.replace('\\"', '"')

        return string

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
        if(tr[0] != '"' or tr[-1] != '"' or tr[-2:] == '\\"'):
            self.emit_warning('''Missing '"' at the begin/end of the translation string ''', file=file, line=line, fatal=self.stop_on_err)
            return
        # Remove the '"' characters around the string
        tr = tr[1:-1]
        tr = self.translate_str(tr)
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

    @staticmethod
    def compile_I32(value):
        return struct.Struct("<I").pack(value)
    @staticmethod
    def compile_cxxstr(value):
        if not value:
            return compiler.compile_I32(0)
        bstr = value.encode()
        return compiler.compile_I32(len(bstr))+bstr
    @staticmethod
    def compile_srstrtree(subid, value):
        bstr = compiler.compile_cxxstr(subid)
        bstr += compiler.compile_cxxstr(value[1])
        count = len(value[0])
        bstr += compiler.compile_I32(count)
        for it in value[0].items():
            bstr += compiler.compile_srstrtree(it[0], it[1])
        return bstr

    def compile(self, file):
        output = open(file, "w+b")
        if self.verbose>=2:
            print("[compiler] Write to file \"%s\""%file)
        # Header
        output.write(b"SRLC")
        # Version number
        output.write(struct.Struct("<III").pack(0, 1, 0))
        # Translation texts
        output.write(b"@txt")
        for it in self.data.items():
            output.write(compiler.compile_srstrtree(it[0], it[1]))
