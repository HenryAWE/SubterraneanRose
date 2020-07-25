#! /usr/bin/env python
# Subterranean Rose Project
# Locale script compiler
# Author: HenryAWE
#
# This file is licensed under GPLv3


import sys, argparse, os
import srlc.compiler
from enum import IntEnum

class option(IntEnum):
    CHECK = 1
    COMPILE = 2

parser = argparse.ArgumentParser(description="Compiler Options")
parser.add_argument("--verbosity", "-v", action="count", default=0)
parser.add_argument("--input", "-i", help="Input file(s)", nargs='+', required=True)
parser.add_argument("--config", "-c", help="Configuration", required=False)
parser.add_argument("--output", "-o", help="Output")
parser.add_argument("--action", choices=["check", "compile"], default="compile")
if len(sys.argv)<=1:
    parser.print_help()
    sys.exit(0)
args = parser.parse_args()

mode = option.COMPILE
if args.action == "check":
    if args.verbosity >= 1:
        print("[lccl] Check")
    mode = option.CHECK
if args.action == "compile":
    if args.verbosity >=1:
        print("[lccl] Compile")
    mode = option.COMPILE

cl = srlc.compiler.srlc_compiler(args.verbosity)
compile = mode == option.COMPILE
display = mode == args.verbosity >= 2
if args.config:
    cl.load_cfg(args.config)
cl.load_txt(args.input, compile, display, mode == option.CHECK)
if compile:
    cl.output(args.output)
