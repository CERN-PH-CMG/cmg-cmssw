from PhysicsTools.Heppy.analyzers.core.AutoFillTreeProducer  import * 
from CMGTools.VVResonances.analyzers.vvTypes  import * 
from CMGTools.VVResonances.analyzers.Skimmer  import * 
import PhysicsTools.HeppyCore.framework.config as cfg

vvSkimmer = cfg.Analyzer(
    Skimmer,
    name='vvSkimmer',
    required = ['LNuJJ','JJ','LLJJ','JJNuNu','TopCR']
)

leptonSkimmer = cfg.Analyzer(
    Skimmer,
    name='leptonSkimmer',
    required = ['inclusiveLeptons']
)


vvTreeProducer = cfg.Analyzer(
     AutoFillTreeProducer, name='vvTreeProducer',
     vectorTree = True,
     saveTLorentzVectors = False,  # can set to True to get also the TLorentzVectors, but trees will be bigger
     defaultFloatType = 'F', # use Float_t for floating point
#     PDFWeights = PDFWeights,
     globalVariables = [
        NTupleVariable("nAllLNu",lambda ev: len(ev.allLNu) , int),       
        NTupleVariable("nAllLL",lambda ev: len(ev.allLL) , int),       
        NTupleVariable("nLL",lambda ev: len(ev.LL) , int),       
        NTupleVariable("nLNu",lambda ev: len(ev.LNu) , int),       
        NTupleVariable("rho",  lambda ev: ev.rho, float, help="kt6PFJets rho"),
        NTupleVariable("rhoCN",  lambda ev: ev.rhoCN, float, help="fixed grid rho central neutral"),
        NTupleVariable("nVert",  lambda ev: len(ev.goodVertices), int, help="Number of good vertices"), 
     ],
     globalObjects =  {
            "met" : NTupleObject("met", metType, help="PF E_{T}^{miss}, after type 1 corrections"),
     },

     collections = {
#            "genleps"          : NTupleCollection("gen",     genParticleWithLinksType, 10, help="Generated leptons (e/mu) from W/Z decays"),                                                                                                
#            "inclusiveLeptons" : NTupleCollection("l",    leptonTypeExtra, 10, help="Inclusive Leptons"),                                                                                                
        "LNuJJ" : NTupleCollection("lnujj",LNuJJType ,5, help="VV candidate with a lepton neutrino and a fat jet"),                                                   
        "TopCR" : NTupleCollection("topCR",LNuJJType ,5, help="Top control region candidate with a lepton neutrino and a fat jet"),                                                   
        "JJ" : NTupleCollection("jj",JJType ,5, help="VV candidate with two fat jets"),                                       
        "LLJJ" : NTupleCollection("lljj",LLJJType ,5, help="VV candidate with two leptons and a fat jet"),                                       
        "JJNuNu" : NTupleCollection("nunujj",NuNuJJType ,5, help="VV candidate with  fat jet and MET")                                      
#        "leadJetConstituents" : NTupleCollection("jetConstituents",     particleType, 500, help="Constituents"),                                                                                                

#            "genVBosons" : NTupleCollection("genV",     genParticleWithLinksType, 10, help="Generated V bosons"),                                                                                                
     }
)



leptonTreeProducer = cfg.Analyzer(
     AutoFillTreeProducer, name='leptonTreeProducer',
     vectorTree = True,
     saveTLorentzVectors = False,  # can set to True to get also the TLorentzVectors, but trees will be bigger
     defaultFloatType = 'F', # use Float_t for floating point
#     PDFWeights = PDFWeights,
#     globalVariables = susyMultilepton_globalVariables,
#     globalObjects = susyMultilepton_globalObjects,
     collections = {
            "genleps"          : NTupleCollection("gen",     genParticleWithLinksType, 10, help="Generated leptons (e/mu) from W/Z decays"),                                                                                                
            "inclusiveLeptons" : NTupleCollection("l",    leptonTypeExtra, 10, help="Inclusive Leptons"),                                                                                                
     }
)
