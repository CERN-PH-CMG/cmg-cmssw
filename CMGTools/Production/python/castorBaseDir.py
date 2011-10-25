#!/usr/bin/env python
import os
import CMGTools.Production.castortools as castortools

def castorBaseDir( user=os.environ['USER']):
    dir = '/store/cmst3/user/'+user+'/CMG'
    exists = castortools.fileExists( castortools.lfnToCastor(dir) )
    if exists:
        return dir
    else:
        print 'directory', dir, 'does not exist. Are you sure about the username?'
        raise NameError(dir)

def myCastorBaseDir():
    return castorBaseDir()
