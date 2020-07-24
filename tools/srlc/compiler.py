# Subterranean Rose Project
# Python module: srlc.compiler
# Author: HenryAWE
#
# This file is licensed under GPLv3


import os, sys
import struct



class script_compiler:
    @staticmethod
    def emit_warning(info, file = "<string>", line = 0, fatal = False):
        print("[script_compiler] [%s - %d]: " % (file, line), info)
        if fatal:
            sys.exit(1)

    def __init__(self, verbosity):
        self.verbosity = verbosity
        self.data = {}

    def push_data(self, strid, translation):
        items = self.data
        ids = strid.split('.')
        for idx in range(0, len(ids)):
            id = ids[idx]
            if id not in items:
                if idx == len(ids)-1:
                    items[id] = [{}, translation]
                    break
                else:
                    items[id] = [{}, None]
            else:
                if idx == len(ids)-1:
                    items[id][1] = translation
                    break
            items = items[id][0]

    @staticmethod
    def translate_data(string:str):
        string = string.replace('\\\\', '\\')
        string = string.replace('\\n', '\n')
        string = string.replace('\\t', '\t')
        string = string.replace('\\f', '\f')
        string = string.replace('\\v', '\v')
        string = string.replace('\\"', '"')

        return string

    def parse_string(self, string:str, file:str, line:int, compile = False, display = False):
        string.strip()
        # Empty string
        if(not string or string[0] == '\n'):
            return True
        if(string[0] == '#'):
            # This line is a comment
            return True
        if(string[0] != '@'):
            # Syntax error
            self.emit_warning("Syntax error - %s" % string, file=file, line=line)
            return False
        eqidx = string.find('=')
        if(eqidx == -1):
            self.emit_warning("Missing '='", file=file, line=line)
            return False
        strid = string[1:eqidx].strip()
        translation = string[eqidx+1:].strip()
        if(translation[0] != '"' or translation[-1] != '"' or translation[-2:] == '\\"'):
            self.emit_warning('''Missing '"' at the begin/end of the translation string ''', file, line)
            return False
        # Remove the '"' characters around the string
        translation = translation[1:-1]
        translation = self.translate_data(translation)

        if display:
            print("[script_compiler]", strid.split('.'), "= \"%s\"" % translation)
        if compile:
            self.push_data(strid, translation)
        return True

    def load(self, file, compile = True, display = False):
        basename = os.path.basename(file)
        stream = open(file, encoding="utf-8", mode="r")
        if not stream.readable:
            self.emit_warning("Open file failed", basename, fatal=False) 
        if self.verbosity >= 1:
            print("[script_compiler] Loading file \"%s\"" % basename)

        line_number = 0
        result = True
        for string in stream.read().splitlines():
            line_number += 1
            result &= self.parse_string(string, basename, line_number, compile, display)
        return result

    @staticmethod
    def compile_I32(value:int):
        return struct.Struct("<I").pack(value)
    @staticmethod
    def compile_cxxstr(value:str):
        if not value:
            return script_compiler.compile_I32(0)
        bstr = value.encode()
        return script_compiler.compile_I32(len(bstr))+bstr
    @staticmethod
    def compile_srstrtree(subid, value):
        # Identifier
        bstr = script_compiler.compile_cxxstr(subid)
        bstr += script_compiler.compile_cxxstr(value[1])
        # Children
        count = len(value[0])
        bstr += script_compiler.compile_I32(count)
        for it in value[0].items():
            bstr += script_compiler.compile_srstrtree(it[0], it[1])
        return bstr

    def output(self, stream):
        stream.write(b'@txt')
        for it in self.data.items():
            stream.write(script_compiler.compile_srstrtree(it[0], it[1]))



class srlc_compiler:
    @staticmethod
    def emit_error(info:str):
        print("[srlc_compiler] ERROR: " + info)
        sys.exit()

    def __init__(self, verbosity = 0):
        self.verbosity = verbosity
        self.script = script_compiler(verbosity)

    @staticmethod
    def write_header(stream):
        stream.write(b'SRLC')
        # Backend version number
        stream.write(struct.Struct("<I").pack(1))

    def load_txt(self, files, compile = True, display = False, check = False):
        for file in files:
            result = self.script.load(file, compile, display)
            if not result and check:
                sys.exit(1)

    def output(self, file):
        stream = open(file, "w+b")
        if not stream.writable:
            self.emit_error("Open file \"%s\" failed" % file)

        if self.verbosity >=2:
            print("[srlc_compiler] Write to file \"%s\"" % file)

        self.write_header(stream)
        self.script.output(stream)
