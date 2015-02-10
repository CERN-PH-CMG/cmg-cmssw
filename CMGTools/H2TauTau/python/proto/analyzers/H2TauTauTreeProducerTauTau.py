# from CMGTools.RootTools.analyzers.TreeAnalyzer import TreeAnalyzer
# from PhysicsTools.HeppyCore.utils.deltar       import bestMatch
from PhysicsTools.Heppy.analyzers.core.TreeAnalyzerNumpy import TreeAnalyzerNumpy
from CMGTools.H2TauTau.proto.analyzers.ntuple            import *

from math import *

def deltaPhi(l1, l2) :
  p = abs(l1.phi()-l2.phi())
  if p < pi : return p
  else      : return 2*pi-p

def deltaR(l1, l2) :
  deta = l1.eta()-l2.eta()
  dphi = deltaPhi(l1,l2)
  return sqrt(deta*deta + dphi*dphi)

def pth(met, l1, l2) :  
  return sqrt( pow( met.px() +          
                    l1 .px() +          
                    l2 .px() , 2 ) +    
               pow( met.py() +          
                    l1 .py() +          
                    l2 .py() , 2 ) )    

class H2TauTauTreeProducerTauTau( TreeAnalyzerNumpy ):
  '''Tree producer for the H->tau tau analysis'''
  
  def declareVariables(self, setup):

    ###################################################
    ###                  EVENT ID                   ###
    ###################################################
    var(self.tree, 'run'  ,int)
    var(self.tree, 'lumi' ,int)
    var(self.tree, 'event',int)

    ###################################################
    ###          TAUS KINEMATICS AND ID             ###
    ###################################################
    bookTau(self.tree, 'l1')
    bookTau(self.tree, 'l2')
    
    bookDiLepton(self.tree)
            
    ###################################################
    ###              DI-TAU VARIABLES               ###
    ###################################################
    var(self.tree, 'visMass'  )
    var(self.tree, 'svfitMass')
    var(self.tree, 'pThiggs'  )
    var(self.tree, 'dRtt'     )
    var(self.tree, 'dEtatt'   )
    var(self.tree, 'dPhitt'   )
   
    ###################################################
    ###                  MISSING ET                 ###
    ###################################################
    var(self.tree, 'mvacov00')
    var(self.tree, 'mvacov01')
    var(self.tree, 'mvacov10')
    var(self.tree, 'mvacov11')
    var(self.tree, 'metPhi'  )
    var(self.tree, 'mex'     )
    var(self.tree, 'mey'     )
    var(self.tree, 'met'     )

    ###################################################
    ###                VBF VARIABLES                ###
    ###################################################
    var(self.tree, 'mjj'              )
    var(self.tree, 'dEtajj'           )
    var(self.tree, 'dPhijj'           )
    var(self.tree, 'nCentralJets', int)

    ###################################################
    ###           JET AND B-JET VARIABLES           ###
    ###################################################
    var    (self.tree, 'nJets'   , int)
    bookJet(self.tree, 'jet1'         )
    bookJet(self.tree, 'jet2'         )
    var    (self.tree, 'jet1Btag'     )
    var    (self.tree, 'jet2Btag'     )
    var    (self.tree, 'nbJets'  , int)
    bookJet(self.tree, 'bjet1'        )
    bookJet(self.tree, 'bjet2'        )

    ###################################################
    ###            WEIGHTS AND VERTICES             ###
    ###################################################
    var(self.tree, 'nVert'       ,int)
    var(self.tree, 'weight'          )
    var(self.tree, 'vertexWeight'    )
    
    ###################################################
    ###            GENERATOR VARIABLES              ###
    ###################################################
    #varInt('genMatched')
    var(self.tree, 'isZtt'  , int)
    var(self.tree, 'isZee'  , int)
    var(self.tree, 'isZmm'  , int)
    var(self.tree, 'isZj'   , int)
    var(self.tree, 'isZttj' , int)
    var(self.tree, 'isZttll', int)
    var(self.tree, 'hasW'   , int)
    var(self.tree, 'hasZ'   , int)
    var(self.tree, 'NUP'    , int)
    var(self.tree, 'genMass'     )
  
    #self.tree.book()

  def process(self, event):
     
    fill(self.tree, 'run'  , event.run    )
    fill(self.tree, 'lumi' , event.lumi   )
    fill(self.tree, 'event', event.eventId)
      
    fill(self.tree, 'visMass'  , event.diLepton.mass()     )
    fill(self.tree, 'svfitMass', event.diLepton.svfitMass())
    fill(self.tree, 'pThiggs'  , pth(event.diLepton.met() ,
                                     event.leg1           ,
                                     event.leg2           ))
                          
    fill(self.tree, 'dRtt'    , deltaR  (event.leg1, event.leg2)      )
    fill(self.tree, 'dPhitt'  , deltaPhi(event.leg1, event.leg2)      )
    fill(self.tree, 'dEtatt'  , abs(event.leg1.eta()-event.leg2.eta()))

#     fill(self.tree, 'mvacov00', event.diLepton.metSig().significance()(0,0))
#     fill(self.tree, 'mvacov01', event.diLepton.metSig().significance()(0,1))
#     fill(self.tree, 'mvacov10', event.diLepton.metSig().significance()(1,0))
#     fill(self.tree, 'mvacov11', event.diLepton.metSig().significance()(1,1))
    fill(self.tree, 'metPhi'  , event.diLepton.met().phi())
    fill(self.tree, 'mex'     , event.diLepton.met().px() )
    fill(self.tree, 'mey'     , event.diLepton.met().py() )
    fill(self.tree, 'met'     , event.diLepton.met().pt() )
    
    fillTau(self.tree, 'l1', event.leg1 )
    fillTau(self.tree, 'l2', event.leg2 )

    if hasattr(event,'genMass'):
      fill(self.tree, 'genMass', event.genMass )
    else:
      fill(self.tree, 'genMass'         , -1 )

    nJets = len(event.cleanJets)
    fill(self.tree, 'nJets', nJets )
    if nJets >= 1 :
      fillJet(self.tree, 'jet1'         , event.cleanJets[0]                                         )
      fill   (self.tree, 'jet1Btag'     , event.cleanJets[0].btag('combinedSecondaryVertexBJetTags') )
#       fill   (self.tree, 'jet1Bmatch'   , event.cleanJets[0].matchGenParton                          )
    if nJets>=2:
      fillJet(self.tree, 'jet2'         , event.cleanJets[1]                                         )
      fill   (self.tree, 'jet2Btag'     , event.cleanJets[1].btag('combinedSecondaryVertexBJetTags') )
#       fill   (self.tree, 'jet2Bmatch'   , event.cleanJets[1].matchGenParton                          )
    
    if hasattr(event, 'vbf') :
      fill(self.tree, 'mjj'          , event.vbf.mjj                                   )
      fill(self.tree, 'dEtajj'       , event.vbf.deta                                  )
      fill(self.tree, 'dPhijj'       , deltaPhi(event.cleanJets[0], event.cleanJets[1]))
      fill(self.tree, 'nCentralJets' , len(event.vbf.centralJets)                      )

    nbJets = len(event.cleanBJets)
    fill(self.tree, 'nbJets', nbJets )
    if nbJets>=1:
      fillJet(self.tree, 'bjet1', event.cleanBJets[0] )
    if nbJets>=2:
      fillJet(self.tree, 'bjet2', event.cleanBJets[1] )

    fill(self.tree, 'weight', event.eventWeight  )
    fill(self.tree, 'nVert' , len(event.vertices)) 
#     fill(self.tree, 'NUP'   , event.NUP          )
    
    if hasattr( event, 'isZtt'  ) and \
       hasattr( event, 'isZee'  ) and \
       hasattr( event, 'isZmm'  ) and \
       hasattr( event, 'isZj'   ) and \
       hasattr( event, 'isZttll') and \
       hasattr( event, 'isZttj' ) : 
      fill(self.tree, 'isZtt'  , event.isZtt   )
      fill(self.tree, 'isZee'  , event.isZee   )
      fill(self.tree, 'isZmm'  , event.isZmm   )
      fill(self.tree, 'isZj'   , event.isZj    )
      fill(self.tree, 'isZttll', event.isZttll )
      fill(self.tree, 'isZttj' , event.isZttj  )
#     else :
#       fill(self.tree, 'isZtt'  , -1 )
#       fill(self.tree, 'isZee'  , -1 )
#       fill(self.tree, 'isZmm'  , -1 )
#       fill(self.tree, 'isZj'   , -1 )
#       fill(self.tree, 'isZttll', -1 )
#       fill(self.tree, 'isZttj' , -1 )
    
    hasW = 0
    if hasattr(event,'hasW') : hasW = event.hasW
    fill(self.tree, 'hasW', hasW)

    hasZ = 0
    if hasattr(event,'hasZ') : hasZ = event.hasZ
    fill(self.tree, 'hasZ', hasZ)

    self.tree.tree.Fill() # funny
    #self.tree.fill()
