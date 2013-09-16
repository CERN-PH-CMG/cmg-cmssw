#!/usr/bin/env python
## Author: Peter Meckiffe
## @ CERN, Meyrin
## September 27th 2011

from CMGTools.Production.savannahBrowser import SavannahBrowser
from datetime import date
import types

class SavannahFormatter(object):
    """Class to format information into human readable format, and interact with SavannahBrowser class"""
	
    def __init__(self, username, password, taskID, opts):
    	"""Creates SavannahBrowser object and initialises dictionary as well as several important fields, also passes unformatted metadata to SavannahBrowser
    	
    	'username' takes users NICE username
    	'password' takes users NICE password
    	'opts' takes the list of metadata options to be passed to SavannahBrowser first, these do not require formatting"""
        self._savannahBrowser = SavannahBrowser(username,password, taskID)
        self._taskID=taskID
        self._opts = opts
        savOpts = self._savannahBrowser.getOpts()
        self.mainField = ""
        for i in self._opts:
            if savOpts is not None:
            	if savOpts[i] is not None:
                	self._savannahBrowser.addOption(i, self._opts[i])
    
    def appendField(self, field): 
    	"""Appends extra required information onto the end of the main section on Savannah
    	
    	'Extra' takes the required data as a string int float dict or list and passes it to the recursive read function before appending to an the userFields variable as a string
    	"""
    	self.mainField+=field+"\n\n"
        
    def publish(self):
    	"""Fill in the details section with the compiled data"""
        if self._taskID is None:
            self._savannahBrowser.addOption("details", self.mainField)
        else: self._savannahBrowser.addOption("comment", self.mainField)
        
        return self._savannahBrowser.post(self._opts['assigned_to'])
            
