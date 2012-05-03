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
    
    tauCurves2 = dict( tau20 = (triggerEfficiency.effIsoTau20,
                                 TGraph(npoints)),
                      tau25 = (triggerEfficiency.effIsoTau25,
                                 TGraph(npoints)),
                      tau35 = (triggerEfficiency.effIsoTau35,
                                 TGraph(npoints)),
                      tau45 = (triggerEfficiency.effIsoTau45,
                                 TGraph(npoints)),
                      )
    
    muCurves = dict( p2011A = (triggerEfficiency.effMu2011A,
                               TGraph(npoints)),
                     p2011B = (triggerEfficiency.effMu2011B,
                               TGraph(npoints)),
                     p2011AB = (triggerEfficiency.effMu2011AB,
                                TGraph(npoints))
                     )

    eleCurves = dict( p2011A = (triggerEfficiency.effEle2011A,
                               TGraph(npoints)),
                     p2011B = (triggerEfficiency.effEle2011B,
                               TGraph(npoints)),
                     p2011AB = (triggerEfficiency.effEle2011AB,
                                TGraph(npoints))
                     )

    def fillGraphs( curves, region=None):
        for np in range(0, npoints):
            pt = np / 10.
            for period, struct in curves.iteritems():
                (fun, gr) = struct
                if region is None:
                    gr.SetPoint( np, pt, fun( pt, 0) )
                elif region == 'Barrel':
                    gr.SetPoint( np, pt, fun( pt, 0 ) )
                elif region == 'Endcaps':
                    gr.SetPoint( np, pt, fun( pt, 2 ) )
                    

    fillGraphs( tauCurves )
    fillGraphs( tauCurves2 )
    fillGraphs( muCurves, region='Barrel' )
    fillGraphs( eleCurves, region='Endcaps' )


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
    can1b = drawCurves( tauCurves2, 'tau2')
    can2 = drawCurves( muCurves, 'mu')
    can3 = drawCurves( eleCurves, 'ele')


    def graphEta( curves, pt = 20):

        
        etaMax = 3.
        etaBin = 2*etaMax / npoints
        
        for np in range(0, npoints):
            eta = -etaMax + np * etaBin  
            for period, struct in curves.iteritems():
                (fun, gr) = struct
                gr.SetPoint( np, eta, fun( pt, eta) )

