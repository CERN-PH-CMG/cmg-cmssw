import FWCore.ParameterSet.Config as cms
import copy



common = [
    'drop *',
    ]


commonDebug = copy.deepcopy( common ) 
commonDebug.extend( [
    ])

