from CMGTools.TTHAnalysis.treeReAnalyzer import *

def conept(ptlep,minireliso,ptratio,ptrel,pdgid,wp):
    assert (wp==2)
    if (abs(pdgid)!=11 and abs(pdgid)!=13):
        return ptlep
    A = 0.10 if (abs(pdgid)==11) else 0.14
    B = 0.70 if (abs(pdgid)==11) else 0.68
    C = 7.0 if (abs(pdgid)==11) else 6.7
    if (ptrel>C):
        return ptlep*(1+max(minireliso-A,0))
    else:
        return max(ptlep,ptlep/ptratio*B)
