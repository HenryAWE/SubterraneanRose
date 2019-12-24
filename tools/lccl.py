#! /usr/bin/env python
# Subterranean Rose Project
# Locale script compiler
# Author: HenryAWE
#
# This file is licensed under GPLv3


import sys, argparse, os
import srlc.compiler

parser = argparse.ArgumentParser(description="Compiler Options")
parser.add_argument("--input", "-i", help="Input file(s)", nargs='+', required=True)
parser.add_argument("--stop-on-error", help="Stop on error", action="store_true")
if len(sys.argv)<=1:
    parser.print_help()
    sys.exit(0)
args = parser.parse_args()

cl = srlc.compiler.compiler()
cl.stop_on_err = args.stop_on_error
for file in args.input:
    cl.load(file)
