#!/usr/bin/env python
from ROOT import TTree, TFile, AddressOf, gROOT

class Tree(object):
    def __init__(self, name, title):
        self.vars = {}
        self.ttree = TTree(name, title)

    def fill(self):
        self.ttree.Fill()
        self.reinit()

    def reinit(self):
        for var in self.vars:
            setattr( self.s, var, -99)

    def shortType(self, type):
        ltype = type.lower()
        if ltype.startswith('int'): return 'I'
        elif ltype.startswith('float'): return 'F'
        elif ltype.startswith('double'): return 'D'
        else: return None 
            
    def addVar(self, type, name):
        self.vars[name] = type

    def book(self):
        structcode = ['struct', 'struct_name', '{']
        for var,type in self.vars.iteritems():
            structcode.append('{type} {var};'.format(type=type, var=var))
        structcode.append('};')
        print structcode
        gROOT.ProcessLine( ' '.join(structcode) )
        from ROOT import struct_name
        self.s = struct_name()
        for var, type in self.vars.iteritems():
            self.ttree.Branch(var,
                              AddressOf(self.s, var),
                              '/'.join([var, self.shortType(type)]) )
        self.reinit()

def main():
  # Make a tree
  f = TFile('myTest.root','RECREATE')
  t = TTree('MyTree','My test tree')
  t2 = Tree('Colin', 'Another test tree')
  t2.addVar('double', 'a')
  t2.addVar('double', 'b')
  t2.book()
  t2.s.a = 2
  t2.s.b = 50
  t2.fill()
  

  # Create a struct
  gROOT.ProcessLine(\
    "struct MyStruct{\
      Int_t someInt;\
      Double_t someDouble;\
    };")
  from ROOT import MyStruct

  # Create branches in the tree
  s = MyStruct()
  t.Branch('rootInt',AddressOf(s,'someInt'),'someInt/I')
  t.Branch('rootDouble',AddressOf(s,'someDouble'),'someDouble/D')

  # Fill tree
  for i in range(100000):
    s.someInt = i
    s.someDouble = i
    t.Fill()

  f.Write()
  f.Close()

if __name__=='__main__':
    main()

