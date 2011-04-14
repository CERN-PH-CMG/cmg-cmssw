#!/usr/bin/env python


class relvalDefinition:
    def __init__( self, cmssw, relval, tag):
        self.cmssw = cmssw
        self.relval = relval
        self.tag = tag
    def __str__( self ):
        outstr =  str(self.relval) + '/' + str( self.cmssw ) + '/' + str( self.tag ) 
        return outstr
    
class relvalList:
    def __init__( self ):
        self.list = []
    
    def add(self, cmssw, relval, tag):
        self.list.append( relvalDefinition(cmssw, relval, tag) )


    def __str__(self):
        
        str = 'relvals: \n'
        for relval in self.list:
            str += relval.__str__() + '\n'
        return str
