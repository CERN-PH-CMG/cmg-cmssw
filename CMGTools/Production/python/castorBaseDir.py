#!/usr/bin/env python
import os
import CMGTools.Production.eostools as castortools

def castorBaseDir( user=os.environ['USER'], area = None):
    """Gets the top level directory to use for writing for 'user'"""
    
    if area is None:
        #magic string for group area
        tokens = user.split('_')
        if tokens and len(tokens) > 1:
            user = tokens[0]
            area = tokens[1]
        else:
            area = 'user'
    
    d = '/store/cmst3/%s/%s/CMG' % (area,user)
    exists = castortools.fileExists( castortools.lfnToCastor(d) )
    if exists:
        return d
    else:
        print 'directory', d, 'does not exist. Are you sure about the username?'
        raise NameError(d)

def myCastorBaseDir():
    """Gets the top level directory to use for writing for the current user"""
    return castorBaseDir()
