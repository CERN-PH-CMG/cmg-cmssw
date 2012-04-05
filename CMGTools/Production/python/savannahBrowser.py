#!/usr/bin/env python
## Author: Peter Meckiffe
## @ CERN, Meyrin
## September 27th 2011

import CMGTools.Production.mechanize as mechanize
import re, os

class SavannahBrowser(object):
    # Creates browser object and calls login method
    def __init__(self, username, password, taskID):
        self.newDS = True
        self.taskID = taskID
        if self.taskID is not None: self.newDS=False
        self.user = username
        self._br = mechanize.Browser()
        self._valid = False
        if self.newDS:
            self._br.open("https://savannah.cern.ch/projects/cmgsample/")
            if self._login(username, password):
            	self._br.follow_link(text_regex='Submit a new item', url_regex="task")
            	self._br.select_form(name='trackers_form')
        else:
            self._br.open("https://savannah.cern.ch/task/?"+str(taskID))
            if self._login(username, password):
            	self._br.follow_link(url_regex="#postcomment")
            	self._br.select_form(name='item_form')
        
    # Log the browser object into the users Savannah account
    def _login(self, username, password):
        # Check if client is logged in already, to avoid re-calling this method unneccesarily
        if self._valid == True:
            print "Client is already logged in"
        else:
            # Find Login link and follow it
            if self._br.links(text_regex="Login"):
            	try:
                    self._br.follow_link(text_regex="Login")
                    # Check response is HTML
                    assert self._br.viewing_html()

                    # Select password form (second item in forms list)
                    self._br.select_form(nr=1)

                    self._br.form['form_loginname']=username
                    self._br.form['form_pw']=password
                    self._br.submit()
                    

                    #for i in self.br.forms(): print i
                    page = self._br.response().read()
                    a = re.search("Not Logged In",page )
                    if a != None:
                             self._valid = False
                             return False
                             print "Client was unable to login, please check login details."
                    else: 
                    	self._valid = True
                    	return True
                except:
                	self._valid = False
                	return False

    def getOpts(self):
    	if self._valid: return self._br.form
    def getValues(self,name):
    	if self._valid:
        	control = self._br.form.find_control(name)
        	if control is not None:
        		return control	
    def addOption(self,name, value):
    	num = re.compile("^\d+$")
    	if self._valid and name is not 'assigned_to':
            control = self._br.form.find_control(name)
            if control is not None:
                if control.type == "select":
                    if num.match(value) is None:
                    	if isinstance(control.get_value_by_label(), list):
                    		try:
                    			self._br.form.set_value_by_label([value],name)
                    		except:
                    			print name + " is an invalid option"
                            
                    elif num.match(value):
                    	if isinstance(control.get_value_by_label(), str) is None:
                    		value = int(value)
                    	try:
                    		self._br.form[name]=[value]
                    	except:
                    		print name + " is an invalid option"

                elif control.type == "text" or control.type == "textarea":
                    self._br.form[name] = value
    def post(self, assigned):
    	if self._valid:
    		if re.search("_group", assigned): assigned = assigned.rstrip("_group")
    		try:
    			self._br.form.set_value_by_label([assigned], "custom_sb1")
    		except:
    			print "User %s is not a valid Files Owner, field will be blank" % assigned
        	try:
        		self._br.form.set_value_by_label([assigned], "assigned_to")
        	except:
        		self._br.form.set_value_by_label([os.environ['USER']], "assigned_to")
        		print "User \"%s\" is not a CMG group member on Savannah, task will be asigned to self" % assigned
        	finally:
        		try:
        			self._br.submit()
        		except:   
        			# If submit error occurred
        			print "An error occured whilst submitting to Savannah"
        			print "-No entry made on Savannah-"
        			return None
            
            # Return the task no that users would use to access the savannah page
        	if self.newDS:
        		for i in self._br.response().readlines():
        			if re.search("New item posted", i)>0:
        				task = i.split("New item posted")[1]
        				task = task.split("\"")[1]
        				self.taskID = task.split("=")[-1].lstrip("/task/?")
        	if self.taskID is not None:
        		print "Task URL: https://savannah.cern.ch/task/?" + str(self.taskID)
        	return self.taskID
    def closeTask(self):
    	if self.newDS:
    		print "Cannot close un-open task"
    		return None
    	else:
    		
    		self._br.form["status_id"] = ['3']
    		self._br.submit()
        
