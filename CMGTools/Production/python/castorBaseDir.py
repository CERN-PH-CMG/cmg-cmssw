#!/usr/bin/env python
import os
import CMGTools.Production.eostools as castortools

def castorBaseDir( user=os.environ['USER']):
    """Gets the top level directory to use for writing for 'user'"""
    d = '/store/cmst3/user/'+user+'/CMG'
    exists = castortools.fileExists( castortools.lfnToCastor(d) )
    if exists:
        return d
    else:
        print 'directory', d, 'does not exist. Are you sure about the username?'
        raise NameError(d)

def myCastorBaseDir():
    """Gets the top level directory to use for writing for the current user"""
    return castorBaseDir()
