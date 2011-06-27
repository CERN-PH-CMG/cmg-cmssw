#!/usr/bin/env python

import pprint
import re

class AliasSetter:
    def __init__(self, tree, aliases, process):
        print 'setting aliases for tree'
        pprint.pprint( aliases )
        self.tree = tree
        self.aliases = aliases
        self.process = process
        self.setAliases()

    def setAliases(self, aliases=None, process=None):

       if aliases==None:
           aliases = self.aliases
       if process==None:
           process = self.process

       # adding the process name, if it's necessary to do so:
       pattern = re.compile('.*__$')        
       for alias,aliased in sorted(aliases.iteritems()):
           # print alias, aliased
           
           match = pattern.match( aliased )
           fullAliased = aliased
           if match != None:
               fullAliased += process
               
           print alias, fullAliased 
           self.tree.SetAlias( alias, fullAliased )
            

if __name__ == '__main__':
                
    from CMGTools.RootTools.RootTools import *
    import sys

    events = Chain('Events', sys.argv[1])  

    aliasSetter = AliasSetter(events, cmgBasic, 'ANA')

    # aliasSetter = AliasSetter(tree, ra2)

