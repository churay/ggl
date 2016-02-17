#!/usr/bin/env python

import string, os, optparse

parser = optparse.OptionParser()
parser.add_option( "-o", type="string", action="store", dest="filename" )

( options, args ) = parser.parse_args()

suite_path = os.path.dirname( os.path.realpath(__file__) )
template_filename = os.path.join( suite_path, "suite.txt" )
suite_filename = options.filename

with open( template_filename, "r" ) as template_file, open( suite_filename, "w" ) as suite_file:
    test_includes = [ "#include \"test/%s.cpp\"" % test_name for test_name in args ]

    suite_template = string.Template( template_file.read() )
    suite_file.write( suite_template.substitute(test_includes="\n".join(test_includes)) )
