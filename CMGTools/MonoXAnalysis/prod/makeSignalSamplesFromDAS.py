#! /usr/bin/env python

import os
import sys
import re
import optparse

def main():
    usage = '''usage: %prog --prefix DMS scalar.txt'''
    parser = optparse.OptionParser(usage=usage)
    
    parser.add_option('-p', '--prefix',   action='store',     dest='prefix',    help='the prefix to be added to the component name'  , default='DMS')
    parser.add_option('-n', '--compname', action='store',     dest='compname',  help='the name of the group of signals'  , default='SM_Scalars')
    (opt, args) = parser.parse_args()
    
    if len(args) != 1:
        print usage
        sys.exit(1)
    inputlist = args[0]
        
    inputListfile=open(inputlist)
    inputdatasets = inputListfile.readlines()

    components = []
    for line in inputdatasets:
        dataset = line.rstrip('\n')
        matchObj = re.match( r'.*_Mphi(.*)_13TeV.*/(.*)/MINIAODSIM', dataset )
        pdname = (opt.prefix+"_Mphi"+matchObj.group(1)).replace("-","_")

        compKreatorLine = pdname + " = kreator.makeMCComponent(\"" + pdname + "\",\"" + dataset + "\", \"CMS\", \".*root\", 1.0, useAAA=True)"
        print compKreatorLine

        components.append(pdname)
        
    print "\n"
    print opt.compname + " = [ " + ", ".join(components) + " ]\n"


if __name__ == "__main__":
        main()

