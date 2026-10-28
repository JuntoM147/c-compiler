#!/usr/bin/python3

import sys
import subprocess
import os
import argparse

parser = argparse.ArgumentParser(prog="compiler_driver", 
                                 description="A program to call the preprocessor, compiler, assembler and linker")

parser.add_argument('filename')

# Optional flags
group = parser.add_mutually_exclusive_group(required=False)
group.add_argument('-S', 
                    help="Option that directs the compiler to emit assembly but not assemble or link it",
                    action='store_true')
group.add_argument('--lexer', 
                   help="Directs compiler to run the lexer but stop before parsing",
                   action='store_true')
group.add_argument('--parse', 
                   help="Directs compiler to run the lexer and parser but stop before assembly generation",
                   action='store_true')
group.add_argument('--codegen', 
                   help="Directs compiler to perform lexing, parsing, and assembly generation but stop before code emission",
                   action='store_true')

args = parser.parse_args()

if not args.filename.endswith(".c"):
    parser.error("source file must be a C file")

output_file = args.filename.removesuffix(".c")

#TODO: remove
sys.exit(0)

try:
    # Preprocess the source file
        # -E to only run the preprocessor
        # -P to not emit line markers
    preprocessed_file = output_file + ".i"
    subprocess.run(["gcc", str(args.filename), "-E", "-P", "-o", preprocessed_file], check=True)

    # TODO Compile the preprocessed source file
    if args.lexer:
        pass
        if args.parser:
            pass
            
            if args.codegen:
                pass

    assembly_file = output_file + ".s"
    #TODO delete the preprocessed file when you are done with it

    if not args.S:
        # Assemble and link the assembly file to produce a executable
        subprocess.run(["gcc", assembly_file, "-o", output_file], check=True)
        os.remove(assembly_file)

except subprocess.CalledProcessError as e:
    sys.exit(e.returncode)

