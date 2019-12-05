# Subterranean Rose Project
# Python module: srlc.compiler
# Author: HenryAWE
#
# This file is licensed under GPLv3


import os

class compiler:
    @staticmethod
    def emit_warning(info, file = "<string>", line = 0):
        print("[%s - %d]: "%(file, line), info);

    def parse_string(self, string, file, line):
        string.strip()
        if(not string or string[0] == '\n'):
            return
        if(string[0] == '#'):
            # This line is a comment
            return
        if(string[0] != '@'):
            # Syntax error
            self.emit_warning("Syntax error - %s"%string, file=file, line=line)
        


    def load(self, filename):
        f = open(filename, encoding="utf-8")
        line_number = 1

        for string in f.read().splitlines():
            self.parse_string(string, os.path.basename(filename), line_number)
