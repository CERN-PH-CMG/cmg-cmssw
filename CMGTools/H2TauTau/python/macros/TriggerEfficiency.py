from CMGTools.RootTools.RootTools import loadLibs
from ROOT import gSystem

loadLibs()

gSystem.Load("libCMGToolsH2TauTau")

from ROOT import TriggerEfficiency
    

if __name__ == '__main__':

    from ROOT import TGraph, TCanvas
    
    triggerEfficiency = TriggerEfficiency()

    npoints = 1000
    
    tauCurves = dict( p2011A = (triggerEfficiency.effTau2011A,
                                TGraph(npoints)),
                      p2011B = (triggerEfficiency.effTau2011B,
                                TGraph(npoints)),
                      p2011AB = (triggerEfficiency.effTau2011AB,
                                 TGraph(npoints))
                      )
    
    muCurves = dict( p2011A = (triggerEfficiency.effMu2011A,
                               TGraph(npoints)),
                     p2011B = (triggerEfficiency.effMu2011B,
                               TGraph(npoints)),
                     p2011AB = (triggerEfficiency.effMu2011AB,
                                TGraph(npoints))
                     )

    def fillGraphs( curves, mu=None):
        for np in range(0, npoints):
            pt = np / 10.
            for period, struct in curves.iteritems():
                (fun, gr) = struct
                if mu is None:
                    gr.SetPoint( np, pt, fun( pt ) )
                elif mu == 'Barrel':
                    gr.SetPoint( np, pt, fun( pt, 0 ) )
                elif mu == 'Endcaps':
                    gr.SetPoint( np, pt, fun( pt, 2 ) )
                    

    fillGraphs( tauCurves )
    fillGraphs( muCurves, mu='Barrel' )

    def drawCurves( curves, name):
        first = True

        tauC = TCanvas(name, name)
        for dummy, gr in curves.values(): 
            if first is True: 
                gr.Draw('AL')
                first = False
            else:
                gr.Draw('Lsame')
        return tauC

    
    can1 = drawCurves( tauCurves, 'tau')
    can2 = drawCurves( muCurves, 'mu')


    def graphEta( curves, pt = 20):

        
        etaMax = 3.
        etaBin = 2*etaMax / npoints
        
        for np in range(0, npoints):
            eta = -etaMax + np * etaBin  
            for period, struct in curves.iteritems():
                (fun, gr) = struct
                gr.SetPoint( np, eta, fun( pt, eta) )

