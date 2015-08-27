#!/usr/bin/env python

import os
import CMGTools.Production.eostools as eostools
from optparse import OptionParser

if __name__ == "__main__":
	
	parser = OptionParser(usage="%prog localdir remoteEOSdir")
	parser.add_option("-t", dest="treeproducername", type='string', default="myTreeProducer", help='Name of the tree producer module')
	parser.add_option("-f", dest="friendtreestring", type='string', default="evVarFriend", help='String identifying friend trees (must be contained in the root file name)')
	parser.add_option("-T", dest="treename", type='string', default="tree.root", help='Name of the tree file')
	(options, args) = parser.parse_args()
	if len(args)<2: raise RuntimeError, 'Expecting at least two arguments'
	
	locdir = args[0]
	remdir = args[1]
	
	
	if not eostools.isEOS(remdir): raise RuntimeError, 'Remote directory should be on EOS.'
	if (not eostools.fileExists(locdir)) or eostools.isFile(locdir): 
	    raise RuntimeError, 'The local directory that should contain the trees does not exist.'
	if eostools.fileExists('%s/%s' % (remdir,locdir)):
	    raise RuntimeError, 'The remote EOS directory where the trees should be archived already exists.'
	
	alldsets = eostools.ls(locdir)
	dsets = [d for d in alldsets if [ fname for fname in eostools.ls(d) if options.friendtreestring in fname]==[] ]
	friends = [d for d in alldsets if d not in dsets]
	
	tocopy = []
	for d in dsets:
	    if eostools.isFile(d): raise RuntimeError, 'File found in local directory.'
	    if '%s/%s'%(d,options.treeproducername) not in eostools.ls(d): raise RuntimeError, 'Tree producer sub-directory not found.'
	    fname = d+'/'+options.treeproducername+'/'+options.treename
	    if (fname not in eostools.ls('%s/%s'%(d,options.treeproducername))) or (not eostools.isFile(fname)): raise RuntimeError, 'Tree file not found.'
	    tocopy.append( (fname,'%s/%s_%s_%s'%(remdir,d,options.treeproducername,options.treename)) )
	for d in friends:
	    allfriends = eostools.ls(d)
	    for f in allfriends:
	        if (options.friendtreestring not in f) or (not eostools.isFile(f)):
	            raise RuntimeError, 'Unknown file in friend directory.'
	        tocopy.append( (f,'%s/%s_%s'%(remdir,d,f.split('/')[-1])) )
	for task in tocopy:
	    if eostools.fileExists(task[0]+".url"): raise RuntimeError, '.url file already exists.'
	
	newdir='%s/%s'%(remdir,locdir)
	print 'Will create EOS directory %s and copy the following files:\n'%newdir
	for task in tocopy: print '%s -> %s' % task
	
	print '\nDo you agree? [y/N]\n'
	if raw_input()!='y':
	    print 'Aborting'
	    exit()
	
	eostools.mkdir(newdir)
	if not eostools.fileExists(newdir): raise RuntimeError, 'Impossible to create remote directory.'
	for task in tocopy:
	    eostools.xrdcp(task[0],task[1])
	    fcmd = open(task[0]+".url","w")
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
		print '\nALL CHECKSUMS OK\n\nIf you want, you can delete the local root files:\n'
		for task in tocopy: print 'rm %s'%task[0]
