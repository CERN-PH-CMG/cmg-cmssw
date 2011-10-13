import FWCore.ParameterSet.Config as cms

from CMGTools.Common.eventContent.everything_cff import *
from CMGTools.Susy.RA1.RA1EventContent_cff import *
from CMGTools.Susy.RA2.RA2EventContent_cff import *
from CMGTools.Susy.Razor.razorEventContent_cff import *
from CMGTools.Susy.LeptonicStop.leptonicStopEventContent_cff import *
from CMGTools.Susy.LP.LPEventContent_cff import *
from CMGTools.Susy.common.eventContent_cff import eventContent as commonEventContent

susyEventContent = everything
susyEventContent += RA1EventContent
susyEventContent += RA2EventContent
susyEventContent += razorEventContent
susyEventContent += leptonicStopEventContent
susyEventContent += LPEventContent
susyEventContent += commonEventContent
 
