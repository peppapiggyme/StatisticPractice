#!/usr/bin/python

"""
A tool that adds a new excercise
Will add .cpp and .h file

Currently it doesn't modify qmake file 
or ExUtils class

Usage:

    tool/AddNewEx.py <N>

Warning: will not add if file exists!

@author: Bowen Zhang
@date: 2020/10/05

"""

import sys
import os.path

nEx = sys.argv[1]

cpp_str = "#include \"Exercise_{N}.h\"\n\n#include <iostream>\n\nusing namespace std;\n\nvoid Exercise_{N}::test() const\n".format(N=nEx) + "{\n\n}\n"

header_str = "#ifndef EXERCISE_{N}_H\n#define EXERCISE_{N}_H\n\n#include \"ExerciseBase.h\"\n\nDEFINENEWEXERCISE({N})\n\n#endif // EXERCISE_{N}_H\n".format(N=nEx)

cpp_fn = "Exercise_{N}.cpp".format(N=nEx)
header_fn = "Exercise_{N}.h".format(N=nEx)

def CreateIfNotExist(fn, content):
    if os.path.isfile(fn):
        print "Exercise file " + fn + " already exists, will not create!"
    else:
        with open(fn, "w") as f:
            f.write(content)

CreateIfNotExist(cpp_fn, cpp_str)
CreateIfNotExist(header_fn, header_str)
