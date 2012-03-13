#!/usr/bin/env python
import os
import CMGTools.Production.eostools as castortools

def getUserAndArea(user):
    """Factor out the magic user hack for use in other classes"""
    
    area = 'user'
    
    tokens = user.split('_')
    if tokens and len(tokens) > 1:
        user = tokens[0]
        area = tokens[1]
    return user, area

def castorBaseDir( user=os.environ['USER'], area = None):
    """Gets the top level directory to use for writing for 'user'"""
    
    if area is None:
        user, area = getUserAndArea(user)
    
    d = '/store/cmst3/%s/%s/CMG' % (area,user)
    exists = castortools.isDirectory( castortools.lfnToCastor(d) )
    if exists:
        return d
    else:
        print 'directory', d, 'does not exist. Are you sure about the username?'
        raise NameError(d)

def myCastorBaseDir():
    """Gets the top level directory to use for writing for the current user"""
    return castorBaseDir()
