import FWCore.ParameterSet.Config as cms
from CMGTools.Common.miscProducers.cmgPatElectronProducer_cfi import cmgPatElectronProducer

def getattrGenerator( process, postfix ):
    def fun(name):
        return getattr(process, name+postfix)
    return fun


def addMITElectronID( process, patModuleName, seq, postfix ):
    '''Adds MIT electron ID to the PAT electrons

    patModuleName: e.g. selectedPatElectrons
    postfix: e.g. AK5
    if you want to add the MIT ID to selectedPatElectronsAK5.

    seq is the sequence in which the manipulation is going to take place
    '''
    ga = getattrGenerator( process, postfix )
    
    newSeq = cms.Sequence()
    setattr(process, patModuleName+postfix+"WithMITID", newSeq)
    ga( seq ).replace(ga(patModuleName), newSeq)

    # clones patModuleNamepostfix into prepatModuleNamepostfix
    # this module is the new pat electron selector
    premodname = ''.join(['pre', patModuleName, postfix])
    setattr( process, premodname,
             ga(patModuleName).clone() )
    
    # adds a pat electron producer module just after the new pat electron selector
    # this pat electron producer:
    #  - has the same name as the old pat electron selector
    #  - takes a collection of pat electrons. for each of them, create an identical pat
    #    electron, except that this new electron contains the MIT ID. 
    setattr( process, ''.join([patModuleName, postfix]),
             cmgPatElectronProducer.clone(src=premodname) )
	     
    newSeq += ga('pre'+patModuleName) + ga(patModuleName)
