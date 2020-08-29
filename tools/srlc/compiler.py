# Subterranean Rose Project
# Python module: srlc.compiler
# Author: HenryAWE
#
# This file is licensed under GPLv3


import os, sys
import struct
import xml.dom.minidom as xmldoc
import re



class data_compiler:
    @staticmethod
    def compile_I32(value:int):
        if value >= 4294967295:
            raise ValueError("Value too larege")
        return struct.Struct("<I").pack(value)
    @staticmethod
    def compile_cxxstr(value:str):
        # Length
        if not value:
            return data_compiler.compile_I32(0)
        bstr = value.encode()
        # Data
        return data_compiler.compile_I32(len(bstr))+bstr
    @staticmethod
    def compile_srstrtree(subid, value):
        # Identifier
        bstr = data_compiler.compile_cxxstr(subid)
        # Data
        bstr += data_compiler.compile_cxxstr(value[1])
        # Children count
        count = len(value[0])
        bstr += data_compiler.compile_I32(count)
        # Children
        for it in value[0].items():
            bstr += data_compiler.compile_srstrtree(it[0], it[1])
        return bstr



class config_compiler:
    def __init__(self, verbosity):
        self.verbosity = verbosity

    def parse_file(self, file):
        if self.verbosity >= 1:
            print("[config_compiler] Loading file \"%s\"" % file)
        doc = xmldoc.parse(file)
        root = doc.getElementsByTagName("locale")[0]
        info = root.getElementsByTagName("info")
        self.parse_info(info[0])

    def parse_info(self, doc):
        self.id = doc.getElementsByTagName("id")[0].firstChild.data
        if self.verbosity >=2:
            print("[config_compiler]: id = ", self.id)
        self.name = doc.getElementsByTagName("name")[0].firstChild.data
        if self.verbosity >=2:
            print("[config_compiler]: name = ", self.name)
        version_string = doc.getElementsByTagName("version")[0].firstChild.data
        semver_pattern = "(\\d+)\\.(\\d+)\\.(\\d+)"
        if not re.match(semver_pattern, version_string):
            raise ValueError("Invalid version string : " + version_string)
        result = re.match(semver_pattern, version_string)
        self.version = (int(result.group(1)), int(result.group(2)), int(result.group(3)))
        author = doc.getElementsByTagName("author")
        if len(author) > 0:
            self.author = author[0].firstChild.data
        else:
            self.author = "Unknown"
        comment = doc.getElementsByTagName("comment")
        if len(comment) > 0:
            if comment[0].firstChild:
                self.comment = comment[0].firstChild.data
            else:
                self.comment = ""
        else:
            self.comment = ""

    def output(self, stream):
        stream.write(b"@inf")
        stream.write(data_compiler.compile_cxxstr(self.id))
        stream.write(data_compiler.compile_cxxstr(self.name))
        for i in range(3):
            stream.write(data_compiler.compile_I32(self.version[i]))
        stream.write(data_compiler.compile_cxxstr(self.author))
        stream.write(data_compiler.compile_cxxstr(self.comment))



class action_compiler:
    def __init__(self, verbosity):
        self.verbosity = verbosity

    def parse_file(self, file):
        if self.verbosity >= 1:
            print("[action_compiler] Loading file \"%s\"" % file)
        doc = xmldoc.parse(file)
        root = doc.getElementsByTagName("locale")[0]
        action = root.getElementsByTagName("action")
        if len(action) > 0:
            self.parse_action(action[0])
        else:
            self.text_error = 0

    def parse_action(self, doc):
        text = doc.getElementsByTagName("text")
        if len(text) > 0:
            self.parse_text_action(text[0])
        else:
            self.text_error = 0 # RETURN_EMPTY_STRING

    def parse_text_action(self, doc):
        text_error_action_str = doc.getElementsByTagName("error")[0].firstChild.data
        if text_error_action_str == "RETURN_EMPTY_STRING":
            self.text_error = 0
        elif text_error_action_str == "RETURN_REQUEST":
            self.text_error = 1
        elif text_error_action_str == "RETURN_ERROR_STRING":
            self.text_error = 2
            error_string = doc.getElementsByTagName("error-string")
            if len(error_string) > 0:
                self.error_string = error_string[0].firstChild.data
            else:
                self.error_string = ""
        elif text_error_action_str == "USE_FALLBACK":
            self.text_error = 3
            self.fallback = doc.getElementsByTagName("fallback")[0].firstChild.data
        elif text_error_action_str == "THROW_EXCEPTION":
            self.text_error = 4
        else:
            raise ValueError("Unknown text error action: ", text_error_action_str)
        if self.verbosity >= 2:
            print("[action_compiler] %s (%d)" % (text_error_action_str, self.text_error))
            if self.text_error is 2:
                print("[action_compiler] Error string: ", self.error_string)
            elif self.text_error is 3:
                print("[action_compiler] Fallback: ", self.fallback)

    def output(self, stream):
        stream.write(b"@act")
        stream.write(data_compiler.compile_I32(self.text_error))
        if self.text_error is 2: # RETURN_ERROR_STRING
            stream.write(data_compiler.compile_cxxstr(self.error_string))
        elif self.text_error is 3: # USE_FALLBACK
            stream.write(data_compiler.compile_cxxstr(self.fallback))



class script_compiler:
    @staticmethod
    def emit_warning(info, file = "<string>", line = 0, fatal = False):
        print("[script_compiler] [%s - %d]: " % (file, line), info)
        if fatal:
            sys.exit(1)

    def __init__(self, verbosity):
        self.verbosity = verbosity
        self.text = {}

    def push_data(self, strid, translation):
        items = self.text
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

    def output(self, stream):
        stream.write(b'@txt')
        top_level_id_count = len(self.text)
        stream.write(data_compiler.compile_I32(top_level_id_count))
        for it in self.text.items():
            stream.write(data_compiler.compile_srstrtree(it[0], it[1]))



class srlc_compiler:
    @staticmethod
    def emit_error(info:str):
        print("[srlc_compiler] ERROR: " + info)
        sys.exit()

    def __init__(self, incremental = False, verbosity = 0):
        self.incremental = incremental
        self.verbosity = verbosity
        self.mtime = 0.0
        self.config = config_compiler(verbosity)
        self.action = action_compiler(verbosity)
        self.script = script_compiler(verbosity)

    @staticmethod
    def write_header(stream):
        stream.write(b'SRLC')
        # Backend version number
        stream.write(struct.Struct("<I").pack(1))

    def setmtime(self, file):
        mt = os.path.getmtime(file)
        if(mt > self.mtime):
            self.mtime = mt

    def load_cfg(self, file):
        self.config.parse_file(file)
        self.action.parse_file(file)
        self.setmtime(file)

    def load_txt(self, files, compile = True, display = False, check = False):
        for file in files:
            result = self.script.load(file, compile, display)
            self.setmtime(file)
            if not result and check:
                sys.exit(1)

    def output(self, file):
        ofile_mt = os.path.getmtime(file)
        if(self.incremental and ofile_mt > self.mtime):
            return
        stream = open(file, "w+b")
        if not stream.writable:
            self.emit_error("Open file \"%s\" failed" % file)

        if self.verbosity >=2:
            print("[srlc_compiler] Write to file \"%s\"" % file)

        self.write_header(stream)
        self.config.output(stream)
        self.action.output(stream)
        self.script.output(stream)
