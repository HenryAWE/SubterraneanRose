#! /usr/bin/env python
# Subterranean Rose Project
# Locale script compiler
# Author: HenryAWE
#
# This file is licensed under GPLv3


import sys, argparse, os
import srlc.compiler

parser = argparse.ArgumentParser(description="Compiler Options")
parser.add_argument("--verbose", "-v", action="count", default=0)
parser.add_argument("--input", "-i", help="Input file(s)", nargs='+', required=True)
parser.add_argument("--output", "-o", help="Output")
parser.add_argument("--stop-on-error", help="Stop on error", action="store_true")
parser.add_argument("--action", choices=["error-checking", "display","compile"], default="error-checking")
if len(sys.argv)<=1:
    parser.print_help()
    sys.exit(0)
args = parser.parse_args()

cl = srlc.compiler.compiler()
cl.stop_on_err = args.stop_on_error
cl.verbose = args.verbose
if args.action == "error-checking":
    if cl.verbose >= 1:
        print("[lccl] Error-checking")
    cl.stop_on_err = True
    cl.mode = srlc.compiler.option.ERROR_CHECKING
    for file in args.input:
        cl.load(file)
elif args.action == "display":
    if cl.verbose >= 1:
        print("[lccl] Display")
    cl.mode = srlc.compiler.option.DISPLAY
    for file in args.input:
        cl.load(file)
elif args.action == "compile":
    if cl.verbose >=1:
        print("[lccl] Compile")
    cl.mode = srlc.compiler.option.COMPILE
    for file in args.input:
        cl.load(file)
    cl.compile(args.output)
