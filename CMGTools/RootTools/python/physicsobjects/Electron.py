from CMGTools.RootTools.physicsobjects.Lepton import Lepton
from CMGTools.RootTools.physicsobjects.ElectronMVAID import ElectronMVAID_Trig, ElectronMVAID_NonTrig, ElectronMVAID_TrigNoIP


class Electron( Lepton ):

    def __init__(self, *args, **kwargs):
        '''Initializing tightIdResult to None. The user is responsible
        for setting this attribute externally if he wants to use the tightId
        function.'''
        super(Electron, self).__init__(*args, **kwargs)
        self.tightIdResult = None
        self.associatedVertex = None
        self.rho              = None
        self._mvaNonTrigV0  = None
        self._mvaTrigV0     = None
        self._mvaTrigNoIPV0 = None

    #def electronID( self, id, vertex=None, rho=None ):
    #    if vertex == None and hasattr(self,'associatedVertex') and self.associatedVertex != None: vertex = self.associatedVertex
    #    if rho == None and hasattr(self,'rho') and self.rho != None: rho = self.rho
    #    return self.electronID_cpp_(id,vertex,rho) if rho != None else self.electronID_cpp_(id,vertex);

    def absEffAreaIso(self,rho,effectiveAreas):
        '''MIKE, missing doc.
        Should have the same name as the function in the mother class.
        Can call the mother class function with super.
        '''
        return self.absIsoFromEA(rho,self.superCluster().eta(),effectiveAreas.eGamma)

    def mvaIso( self ):
        return self.userFloat('mvaIsoRings')

    def mvaId( self ):
        return self.mvaNonTrigV0()
        #return self.userFloat('mvaNonTrigV0')
        
    def tightId( self ):
        return self.tightIdResult
    
    def mvaNonTrigV0( self, debug = False ):
        if self._mvaNonTrigV0 == None:
            if self.associatedVertex == None: raise RuntimeError, "You need to set electron.associatedVertex before calling any MVA"
            if self.rho              == None: raise RuntimeError, "You need to set electron.rho before calling any MVA"
            self._mvaNonTrigV0 = ElectronMVAID_NonTrig(self.physObj, self.associatedVertex, self.rho, debug)
        return self._mvaNonTrigV0 

    def mvaTrigV0( self, debug = False ):
        if self._mvaTrigV0 == None:
            if self.associatedVertex == None: raise RuntimeError, "You need to set electron.associatedVertex before calling any MVA"
            if self.rho              == None: raise RuntimeError, "You need to set electron.rho before calling any MVA"
            self._mvaTrigV0 = ElectronMVAID_Trig(self.physObj, self.associatedVertex, self.rho, debug)
        return self._mvaTrigV0 

    def mvaTrigNoIPV0( self, debug = False ):
        if self._mvaTrigNoIPV0 == None:
            if self.associatedVertex == None: raise RuntimeError, "You need to set electron.associatedVertex before calling any MVA"
            if self.rho              == None: raise RuntimeError, "You need to set electron.rho before calling any MVA"
            self._mvaTrigNoIPV0 = ElectronMVAID_TrigNoIP(self.physObj, self.associatedVertex, self.rho, debug)
        return self._mvaTrigNoIPV0 



    def mvaIDZZ(self):
        '''missing doc, who is using this function?.'''
        mvaRegions = [{'ptMin':0,'ptMax':10, 'etaMin':0.0, 'etaMax':0.8,'mva':0.47},\
                      {'ptMin':0,'ptMax':10, 'etaMin':0.8 ,'etaMax':1.479,'mva':0.004},\
                      {'ptMin':0,'ptMax':10, 'etaMin':1.479, 'etaMax':3.0,'mva':0.295},\
                      {'ptMin':10,'ptMax':99999999, 'etaMin':0.0, 'etaMax':0.8,'mva':-0.34},\
                      {'ptMin':10,'ptMax':99999999, 'etaMin':0.8, 'etaMax':1.479,'mva':-0.65},\
                      {'ptMin':10,'ptMax':99999999, 'etaMin':1.479, 'etaMax':3.0,'mva':0.6}]
        ID=False 
        for element in mvaRegions:
            if self.pt()>= element['ptMin'] and \
               self.pt()< element['ptMax'] and \
               abs(self.superCluster().eta())>=element['etaMin'] and \
               abs(self.superCluster().eta())<element['etaMax'] and \
               self.mvaNonTrigV0()> element['mva']: 
                ID=True

        return ID and (self.gsfTrack().trackerExpectedHitsInner().numberOfLostHits()<=1)

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
        return self.gsfTrack().dxy( vertex.position() )
 

    def p4(self,kind=None):
        return self.physObj.p4(self.physObj.candidateP4Kind() if kind == None else kind)

    def dz(self, vertex=None):
        '''Returns dz.
        Computed using vertex (or self.associatedVertex if vertex not specified),
        and the gsf track.
        '''
        if vertex is None:
            vertex = self.associatedVertex
        return self.gsfTrack().dz( vertex.position() )


