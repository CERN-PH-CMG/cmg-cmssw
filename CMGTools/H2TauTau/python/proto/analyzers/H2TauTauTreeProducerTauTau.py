from math import *

from CMGTools.RootTools.analyzers.TreeAnalyzer import TreeAnalyzer
from PhysicsTools.HeppyCore.utils.deltar       import bestMatch

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

class H2TauTauTreeProducerTauTau( TreeAnalyzer ):
  '''Tree producer for the H->tau tau analysis'''
  
  def declareVariables(self):

    def var( varName ):
      self.tree.addVar('float', varName)

    def varInt( varName ):
      self.tree.addVar('int', varName)

    ###################################################
    ###                  EVENT ID                   ###
    ###################################################
    varInt('run'  )
    varInt('lumi' )
    varInt('event')

    ###################################################
    ###        PARTICLE KINEMATICS AND ID           ###
    ###################################################
    def particleVars( pName ):
      var('{pName}Px'    .format(pName=pName))
      var('{pName}Py'    .format(pName=pName))
      var('{pName}Pz'    .format(pName=pName))
      var('{pName}E'     .format(pName=pName))
      var('{pName}Pt'    .format(pName=pName))
      var('{pName}Eta'   .format(pName=pName))
      var('{pName}Phi'   .format(pName=pName))
      var('{pName}Charge'.format(pName=pName))
      var('{pName}MT'    .format(pName=pName))            
      var('{pName}Mass'  .format(pName=pName))            
      if not ('diTau' in pName or 'jet' in pName or 'L1' in pName):
        var('{pName}DecayMode' .format(pName=pName))
        var('{pName}RawDB3HIso'.format(pName=pName))
        var('{pName}LooseEle'  .format(pName=pName))
        var('{pName}LooseMu'   .format(pName=pName))
        var('{pName}GenPdgId'  .format(pName=pName))
        var('{pName}GenPt'     .format(pName=pName))
        var('{pName}GenEta'    .format(pName=pName))
        var('{pName}GenPhi'    .format(pName=pName))
        var('{pName}GenMass'   .format(pName=pName))
        
    ###################################################
    ###              DI-TAU VARIABLES               ###
    ###################################################
    var('visMass'  )
    var('svfitMass')
    var('pThiggs'  )
    var('dRtt'     )
    var('dEtatt'   )
    var('dPhitt'   )
   
    ###################################################
    ###                  MISSING ET                 ###
    ###################################################
    var('mvacov00')
    var('mvacov01')
    var('mvacov10')
    var('mvacov11')
    var('metPhi'  )
    var('mex'     )
    var('mey'     )
    var('met'     )

    ###################################################
    ###                VBF VARIABLES                ###
    ###################################################
    var   ('mjj'         )
    var   ('dEtajj'      )
    var   ('dPhijj'      )
    varInt('nCentralJets')

    ###################################################
    ###           TAU KINEMATIC VARIABLES           ###
    ###################################################
    particleVars('l1'   )
    particleVars('l2'   )
    particleVars('diTau')

    ###################################################
    ###           JET AND B-JET VARIABLES           ###
    ###################################################
    varInt      ('nJets'   )
    particleVars('jet1'    )
    particleVars('jet2'    )
    var         ('jet1Btag')
    var         ('jet2Btag')
    varInt      ('nbJets'  )
    particleVars('bjet1'   )
    particleVars('bjet2'   )

    ###################################################
    ###            WEIGHTS AND VERTICES             ###
    ###################################################
    varInt      ('nVert'       )
    var         ('weight'      )
    var         ('vertexWeight')
    
    ###################################################
    ###            GENERATOR VARIABLES              ###
    ###################################################
    #varInt('genMatched')
    varInt('isZtt'     )
    varInt('isZee'     )
    varInt('isZmm'     )
    varInt('isZj'      )
    varInt('isZttj'    )
    varInt('isZttll'   )
    varInt('hasW'      )
    varInt('hasZ'      )
    var   ('genMass'   )
    var   ('NUP'       )
  
    self.tree.book()

  def process(self, event):

    def fill( varName, value ):
      setattr( self.tree.s, varName, value )

    def fParticleVars( pName, particle ):
      fill('{pName}Px'    .format(pName=pName), particle.px()    )
      fill('{pName}Py'    .format(pName=pName), particle.py()    )
      fill('{pName}Pz'    .format(pName=pName), particle.pz()    )
      fill('{pName}E'     .format(pName=pName), particle.energy())
      fill('{pName}Pt'    .format(pName=pName), particle.pt()    )
      fill('{pName}Eta'   .format(pName=pName), particle.eta()   )
      fill('{pName}Phi'   .format(pName=pName), particle.phi()   )
      fill('{pName}Charge'.format(pName=pName), particle.charge())
      fill('{pName}MT'    .format(pName=pName), particle.charge()) ## FIXME!
      fill('{pName}Mass'  .format(pName=pName), particle.mass()  )
      if not ('diTau' in pName or 'jet' in pName or 'L1' in pName):
        fill('{pName}DecayMode' .format(pName=pName), particle.decayMode()                                      )
        fill('{pName}RawDB3HIso'.format(pName=pName), particle.tauID('byCombinedIsolationDeltaBetaCorrRaw3Hits'))
        fill('{pName}LooseEle'  .format(pName=pName), particle.tauID('againstElectronLoose')                    )
        fill('{pName}LooseMu'   .format(pName=pName), particle.tauID('againstMuonLoose')                        )
        if hasattr(particle,'genl') :
          fill('{pName}GenPdgId', particle.genl.pdgId())
          fill('{pName}GenPt'   , particle.genl.pt()   )
          fill('{pName}GenEta'  , particle.genl.eta()  )
          fill('{pName}GenPhi'  , particle.genl.phi()  )
          fill('{pName}GenMass' , particle.genl.mass() )
        else :
          fill('{pName}GenPdgId',   0 )
          fill('{pName}GenPt'   , -99.)
          fill('{pName}GenEta'  , -99.)
          fill('{pName}GenPhi'  , -99.)
          fill('{pName}GenMass' , -99.)
 
    fill('run'  , event.run    )
    fill('lumi' , event.lumi   )
    fill('event', event.eventId)
      
    fill('visMass'  , event.diLepton.mass()     )
    fill('svfitMass', event.diLepton.massSVFit())
    fill('pThiggs'  , pth(event.diLepton.met() ,
                          event.leg1           ,
                          event.leg2           ))
                          
    fill('dRtt'    , deltaR  (event.leg1, event.leg2)      )
    fill('dPhitt'  , deltaPhi(event.leg1, event.leg2)      )
    fill('dEtatt'  , abs(event.leg1.eta()-event.leg2.eta()))

    fill('mvacov00', event.diLepton.metSig().significance()(0,0))
    fill('mvacov01', event.diLepton.metSig().significance()(0,1))
    fill('mvacov10', event.diLepton.metSig().significance()(1,0))
    fill('mvacov11', event.diLepton.metSig().significance()(1,1))
    fill('metPhi'  , event.diLepton.met().phi())
    fill('mex'     , event.diLepton.met().px() )
    fill('mey'     , event.diLepton.met().py() )
    fill('met'     , event.diLepton.met().pt() )
    
    fParticleVars('l1', leg1 )
    fParticleVars('l2', leg2 )

    if hasattr(event,'genMass'):
      fill('genMass', event.genMass )
    else:
      fill('genMass'         , -1 )

    nJets = len(event.cleanJets)
    fill('nJets', nJets )
    if nJets >= 1 :
      fParticleVars('jet1', event.cleanJets[0] )
      fill('jet1Btag'     , event.cleanJets[0].btag('combinedSecondaryVertexBJetTags') )
      fill('jet1Bmatch'   , event.cleanJets[0].matchGenParton )
    if nJets>=2:
      fParticleVars('jet2', event.cleanJets[1] )
      fill('jet2Btag'     , event.cleanJets[1].btag('combinedSecondaryVertexBJetTags') )
      fill('jet2Bmatch'   , event.cleanJets[1].matchGenParton )
    
    fill('mjj'          , event.vbf.mjj                                   )
    fill('dEtajj'       , event.vbf.deta                                  )
    fill('dPhijj'       , deltaPhi(event.cleanJets[0], event.cleanJets[1]))
    fill('nCentralJets' , len(event.vbf.centralJets)                      )

    nbJets = len(event.cleanBJets)
    fill('nbJets', nbJets )
    if nbJets>=1:
      fParticleVars('bjet1', event.cleanBJets[0] )
    if nbJets>=2:
      fParticleVars('bjet2', event.cleanBJets[1] )

    fill('weight', event.eventWeight  )
    fill('nVert' , len(event.vertices)) 
    fill('NUP'   , event.NUP          )
    
    if hasattr( event, 'isZtt'  ) and \
       hasattr( event, 'isZee'  ) and \
       hasattr( event, 'isZmm'  ) and \
       hasattr( event, 'isZj'   ) and \
       hasattr( event, 'isZttll') and \
       hasattr( event, 'isZttj' ) : 
      fill('isZtt'  , event.isZtt   )
      fill('isZee'  , event.isZee   )
      fill('isZmm'  , event.isZmm   )
      fill('isZj'   , event.isZj    )
      fill('isZttll', event.isZttll )
      fill('isZttj' , event.isZttj  )
    else :
      fill('isZtt'  , -1 )
      fill('isZee'  , -1 )
      fill('isZmm'  , -1 )
      fill('isZj'   , -1 )
      fill('isZttll', -1 )
      fill('isZttj' , -1 )
    
    hasW = 0
    if hasattr(event,'hasW') : hasW = event.hasW
    fill('hasW', hasW)

    hasZ = 0
    if hasattr(event,'hasZ') : hasZ = event.hasZ
    fill('hasZ', hasZ)

    self.tree.fill()
