import FWCore.ParameterSet.Config as cms

from CMGTools.Common.eventContent.everything_cff import *
from CMGTools.Susy.RA2.RA2EventContent_cff import *
from CMGTools.Susy.RA1.RA1EventContent_cff import *
from CMGTools.Susy.Razor.razorEventContent_cff import *

fullyHadronicEventContent = everything
fullyHadronicEventContent += RA2EventContent
fullyHadronicEventContent += razorEventContent