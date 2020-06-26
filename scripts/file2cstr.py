#! /usr/bin/env python
# Subterranean Rose Project
# Script for converting file into C-style string
# Author: HenryAWE
#
# This file is licensed under GPLv3

import sys, argparse, os

def GenerateString(input):
    output = '"'
    for i in input:
        output += "\\x{:02x}".format(i)
    output += '"'
    return output

def GenerateArray(input:str):
    output = "{ "
    for i in input:
        output += "0x{:02x},".format(i)
    output = output[:-1]
    output += " }"
    return output

parser = argparse.ArgumentParser(description="File2cstr Options")
parser.add_argument("--input", "-i", help="Input file(s)", required=True)
parser.add_argument("--output", "-o", help="Output", required=True)
parser.add_argument("--cached", help="Cache result", action="store_true")
parser.add_argument("--format", help="Output format", choices=["string", "array"], default="string")

args = parser.parse_args()

if args.cached and os.path.exists(args.output):
    input_file_time = os.path.getmtime(args.input)
    output_file_time = os.path.getmtime(args.output)
    if output_file_time >= input_file_time:
        sys.exit()

infile = open(args.input, "rb")
data = infile.read()
outfile = open(args.output, "w+")

written = 0
if args.format == "string":
    written = outfile.write(GenerateString(data))
elif args.format == "array":
    written = outfile.write(GenerateArray(data))

print("Bytes written: {}".format(written))
