from CMGTools.RootTools.physicsobjects.Lepton import *

class Electron( Lepton ):

    def __init__(self, *args, **kwargs):
        '''Initializing tightIdResult to None. The user is responsible
        for setting this attribute externally if he wants to use the tightId
        function.'''
        super(Electron, self).__init__(*args, **kwargs)
        self.tightIdResult = None

    def absEffAreaIso(self,rho,effectiveAreas):
        '''MIKE, missing doc.
        Should have the same name as the function in the mother class.
        Can call the mother class function with super.
        '''
        return self.absIsoFromEA(rho,self.sourcePtr().superCluster().eta(),effectiveAreas.eGamma)

    def mvaIso( self ):
        return self.sourcePtr().userFloat('mvaIsoRings')

    def mvaId( self ):
        return self.mvaNonTrigV0()
        #return self.sourcePtr().userFloat('mvaNonTrigV0')
        
    def tightId( self ):
        return self.tightIdResult
        
    def mvaIDZZ(self):
        '''missing doc, who is using this function?.'''
        mvaRegions = [{'ptMin':0,'ptMax':10, 'etaMin':0.0, 'etaMax':0.8,'mva':0.47},\
                      {'ptMin':0,'ptMax':10, 'etaMin':0.8 ,'etaMax':1.479,'mva':0.004},\
                      {'ptMin':0,'ptMax':10, 'etaMin':1.479, 'etaMax':3.0,'mva':0.295},\
                      {'ptMin':10,'ptMax':99999999, 'etaMin':0.0, 'etaMax':0.8,'mva':0.5},\
                      {'ptMin':10,'ptMax':99999999, 'etaMin':0.8, 'etaMax':1.479,'mva':0.12},\
                      {'ptMin':10,'ptMax':99999999, 'etaMin':1.479, 'etaMax':3.0,'mva':0.6}]
        ID=False 
        for element in mvaRegions:
            if self.pt()>= element['ptMin'] and \
               self.pt()< element['ptMax'] and \
               abs(self.sourcePtr().superCluster().eta())>=element['etaMin'] and \
               abs(self.sourcePtr().superCluster().eta())<element['etaMax'] and \
               self.mvaNonTrigV0()> element['mva']: 
                ID=True

        return ID and (self.numberOfHits()<=1)

    def chargedAllIso(self):
        '''This function is used in the isolation, see Lepton class.
        Here, we replace the all charged isolation by the all charged isolation with cone veto'''
        return self.chargedAllIsoWithConeVeto()


    def dxy(self, vertex=None):
        '''Returns dxy.
        Computed using vertex (or self.associatedVertex if vertex not specified),
        and the gsf track.
        '''
        if vertex is None:
            vertex = self.associatedVertex
        return self.sourcePtr().gsfTrack().dxy( vertex.position() )
 

    def dz(self, vertex=None):
        '''Returns dz.
        Computed using vertex (or self.associatedVertex if vertex not specified),
        and the gsf track.
        '''
        if vertex is None:
            vertex = self.associatedVertex
        return self.sourcePtr().gsfTrack().dz( vertex.position() )


