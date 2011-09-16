#!/usr/bin/env python
# Colin
# creates new source file for a dataset on castor
# compiles the python module
# prints the line to be added to the cfg. 

import os, sys,  imp, re, pprint, string
from optparse import OptionParser

import castortools

parser = OptionParser()
parser.usage = """
importNewSource.py <sampleName>
Create the source file corresponding to a given sample on castor. Run it from the package where you want to put the source cff, for example CMGTools/Common.

For example, the source file for /HT/Run2011A-May10ReReco-v1/AOD
would be placed in
python/sources/HT/Run2011A_May10ReReco_v1/AOD/source_cff.py
and can easily be loaded in any cfg.

Note that the script makes sure to change all '-' into '_' when creating the destanation directory from the dataset name, so that the source module can then be loaded in a python cfg. 
"""

parser.add_option("-n", "--negate", action="store_true",
                  dest="negate",
                  help="do not proceed",
                  default=False)


#parser.add_option("-c", "--castorBaseDir", 
#                  dest="castorBaseDir",
#                  help="Base castor directory. Subdirectories will be created automatically for each prod",
#                  default=castorBaseDir.defaultCastorBaseDir)

parser.add_option("-u", "--user", 
                  dest="user",
                  help="User who is the owner of the castor base directory, where the sample is located.",
                  default=os.environ['USER'] )
parser.add_option("-p", "--pattern", 
                  dest="pattern",
                  help="Regexp pattern for root files in castor dir.",
                  default=".*root")
parser.add_option("-o", "--output", 
                  dest="output",
                  help="Output file name.",
                  default="source_cff.py")

(options,args) = parser.parse_args()

if len(args)!=1:
    parser.print_help()
    sys.exit(1)

sampleName = args[0].rstrip('/')

# preparing castor dir -----------------

import castorBaseDir

cdir = castortools.lfnToCastor( castorBaseDir.castorBaseDir( options.user ) )
cdir += sampleName

if not castortools.fileExists(cdir):
    print 'importNewSource: castor directory does not exist. Exit!'
    sys.exit(1)

# making local source directory ---------

tmp = './python/sources' + sampleName
ldir = re.sub( '-', '_', tmp)
mkdir = 'mkdir -p ' + ldir
print mkdir
os.system( mkdir )

# creating source file ------------------

sourceFile = ldir + '/' + options.output

if os.path.isfile( sourceFile ):
    print sourceFile, 'already exists. use the -o option to define another file name'
    sys.exit(1)

sourceFileList = 'sourceFileList.py -c %s "%s" > %s' % (cdir, options.pattern, sourceFile)
print sourceFileList
os.system(sourceFileList)
# compile new source file
os.system( 'scram b python')

# printing module load command ----------
base = os.environ['CMSSW_BASE']
cwd = os.getcwd()
cwd = re.sub('%s/src/' % base, '', cwd)
# cwd now equal to package name

# replace ./python by package name 
module = re.sub( './python', cwd, sourceFile)
# replace / by .
module = re.sub( '/', '.', module)
# remove trailing .py
module = re.sub( '\.py$', '', module)

os.system( 'cat %s' % sourceFile )

print 'new source file ready to be used:'
print sourceFile
print 'process.load("%s")' % module
