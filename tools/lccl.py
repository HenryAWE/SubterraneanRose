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
args = parser.parse_args()

cl = srlc.compiler.compiler()
for file in args.input:
    cl.load(file)
