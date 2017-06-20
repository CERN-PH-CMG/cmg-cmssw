#!/usr/bin/env python

import os, subprocess
import CMGTools.Production.eostools as eostools
from optparse import OptionParser

if __name__ == "__main__":

	usage="""%prog [options] localdir remoteEOSdir

# This script can be used to archive the flat trees produced by a module to EOS.
#
# If the trees are in mybasedir/TREE_PRODUCTION_XYZ/myTreeProducer/tree.root,
# you should navigate to mybasedir and execute from there:
# archiveTreesOnEOS.py -t myTreeProducer TREE_PRODUCTION_XYZ /eos/cms/store/user/myusername/mytreesdirectory
#
# The friend tree files of the format mybasedir/TREE_PRODUCTION_XYZ/somename/*evVarFriend* are also archived.
# See the options for customization of the file name format.
#
# This script will not erase the local trees, but you will be provided with the commands to do so.
# Be sure that the copy was successful before doing so! The checksums of local and remote files are compared to help you.

"""

	parser = OptionParser(usage=usage)
	parser.add_option("-t", dest="treeproducername", type='string', default="treeProducerDarkMatterMonoJet", help='Name of the tree producer module')
	parser.add_option("-f", dest="friendtreedir", type='string', default="friends", help='String identifying friend trees directory')
	parser.add_option("-T", dest="treename", type='string', default="tree.root", help='Name of the tree file')
	parser.add_option("--dset", dest="dset", type='string', default=None, help='Name of the dataset to process')
	(options, args) = parser.parse_args()
	if len(args)<2: raise RuntimeError, 'Expecting at least two arguments'
	
	locdir = args[0]
	remdir = args[1]
	
	
	if not eostools.isEOS(remdir): raise RuntimeError, 'Remote directory should be on EOS.'
	if (not eostools.fileExists(locdir)) or eostools.isFile(locdir): 
	    raise RuntimeError, 'The local directory that should contain the trees does not exist.'

# check removed to allow for top-up of tree productions
#	if eostools.fileExists('%s/%s' % (remdir,locdir)):
#	    raise RuntimeError, 'The remote EOS directory where the trees should be archived already exists.'
	
	alldsets = eostools.ls(locdir)
	dsets = [d.split("/")[-1] for d in alldsets if options.friendtreedir not in d ]
	if options.dset: dsets = [d for d in dsets if options.dset in d]
	friends = [d.split("/")[-1] for d in alldsets if options.friendtreedir in d ]
	if options.dset: friends = [d for d in friends if options.dset in d]
	
	tocopy = []
	for d in dsets:
	    if eostools.isFile(d): raise RuntimeError, 'File found in local directory.'
	    if '%s/%s/%s'%(locdir,d,options.treeproducername) not in eostools.ls(locdir+'/'+d): raise RuntimeError, 'Tree producer sub-directory not found.'
	    fname = locdir+'/'+d+'/'+options.treeproducername+'/'+options.treename
	    if (fname not in eostools.ls('%s/%s/%s'%(locdir,d,options.treeproducername))) or (not eostools.isFile(fname)): raise RuntimeError, 'Tree file not found.'
	    tocopy.append( (fname,'%s/%s_%s_%s'%(remdir,d,options.treeproducername,options.treename)) )
	for d in friends:
            print "friend",d
	    allfriends = eostools.ls(locdir+'/'+d)
	    for f in allfriends:
	        if not eostools.isFile(f):
	            raise RuntimeError, 'Unknown file in friend directory.'
	        tocopy.append( (f,'%s/%s_%s'%(remdir,d,f.split('/')[-1])) )
	for task in tocopy:
	    if eostools.fileExists(task[0]+".url"): raise RuntimeError, '.url file already exists.'
	
	newdir='%s'%(remdir)
	print 'Will create EOS directory %s and copy the following files:\n'%newdir
	for task in tocopy: print '%s -> %s' % task
	
	print '\nDo you agree? [y/N]\n'
	if raw_input()!='y':
	    print 'Aborting'
	    exit()
	
	eostools.mkdir(newdir)
        print newdir," created on EOS"
	if not eostools.fileExists(newdir): raise RuntimeError, 'Impossible to create remote directory.'
        print "DONE"
	for task in tocopy:
            print "starting ",task[0]," ",task[1]
	    #eostools.xrdcp(task[0],task[1])
            subprocess.call(['xrdcp '+task[0]+' root://eoscms.cern.ch/'+task[1]], stdout=subprocess.PIPE, shell=True);
            print "boh"
	    fcmd = open(task[0]+".url","w")
            print "task 1 = ",task[1]
	    fcmd.write("root://eoscms.cern.ch/%s\n" % task[1])
	    fcmd.close()
	print 'Copied %.2f GB to EOS\n' % eostools.eosDirSize(newdir)

	print 'Verifying checksums:\n'
	problem = False
	for task in tocopy:
		lcheck = eostools.fileChecksum(task[0])
		rcheck = eostools.fileChecksum(task[1])
		ok = (lcheck==rcheck)
		print task[0],lcheck,rcheck,('OK' if ok else 'ERROR')
		if not ok: problem = True
	if problem:
		raise RuntimeError, 'CHECKSUM ERROR DETECTED !!!'
	else:
		print '\nALL CHECKSUMS OK\n'
		print '\nIf you want, you can rename the local root files to force usage of the remote EOS copy for testing:\n'
		for task in tocopy: print 'mv -n %s %s.transferred'%(task[0],task[0])
		print '\nIf the testing is successful, you can delete the local root files:\n'
		for task in tocopy: print 'rm -v %s.transferred'%task[0]
