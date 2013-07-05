from CMGTools.RootTools.analyzers.TreeAnalyzerNumpy import TreeAnalyzerNumpy
from CMGTools.GenStudies.analyzers.ntuple import *
import CMGTools.GenStudies.GenStudyParameter as parameter

def var( tree, varName, type=float ):
    tree.var(varName, type)

def fill( tree, varName, value ):
    tree.fill( varName, value )


class MultiTreeProducer( TreeAnalyzerNumpy ):
    
    def declareVariables(self):

        tr = self.tree
        
        self.counter = 0
        
        var(tr, 'x1')
        var(tr, 'x2')
        var(tr, 'id1')
        var(tr, 'id2')
        var(tr, 'scalePdf')

        var(tr, 'decaymode')
        var(tr, 'categoryid')

        for decayid, dict in sorted(parameter.DecayMode.items()):

            channel = dict[0]
            category = dict[1]

            for id, icategory in sorted(category.items()):

                hname = 'h_' + channel + '_' + icategory
                var(tr, hname)


    def process(self, iEvent, event):

        tr = self.tree
        tr.reset()


        self.counter += 1

        fill(tr, 'x1', event.generator.pdf().x.first)
        fill(tr, 'x2', event.generator.pdf().x.second)
        fill(tr, 'id1', event.generator.pdf().id.first)
        fill(tr, 'id2', event.generator.pdf().id.second)
        fill(tr, 'scalePdf', event.generator.pdf().scalePDF)
        fill(tr, 'decaymode', event.channel)
        fill(tr, 'categoryid', event.categoryid)        

        if(event.category=='None'):

            for id, dict in sorted(parameter.DecayMode[event.channel][1].items()):
                hname = 'h_' + parameter.DecayMode[event.channel][0] + '_' + dict
                fill(tr, hname, 0)

        else:

            for id, dict in sorted(parameter.DecayMode[event.channel][1].items()):
                hname = 'h_' + parameter.DecayMode[event.channel][0] + '_' + dict

                if(dict==event.category):
                    fill(tr, hname, 1)
                else:
                    fill(tr, hname, 0)


        for decayid, dict in sorted(parameter.DecayMode.items()):

            channel = dict[0]
            category = dict[1]

            if(decayid == event.channel):
                continue

            for id, icategory in sorted(category.items()):

                hname = 'h_' + channel + '_' + icategory
                fill(tr, hname, -1)




        self.tree.tree.Fill()
                
       
