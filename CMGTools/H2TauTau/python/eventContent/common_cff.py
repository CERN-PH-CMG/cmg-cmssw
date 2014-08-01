import FWCore.ParameterSet.Config as cms
import copy



common = [
    'keep *',
    ]


commonDebug = copy.deepcopy( common ) 
commonDebug.extend( [
    ])

