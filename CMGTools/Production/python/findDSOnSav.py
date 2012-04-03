#!/usr/bin/env python
## Author: Peter Meckiffe
## @ CERN, Meyrin
## October 12th 2011

import CMGTools.Production.mechanize as mechanize
import re, os
def getNameWithID(taskID):
    try:
        br = mechanize.Browser()
        br.open("https://savannah.cern.ch/task/?"+str(taskID))
        # Check response is HTML
        assert br.viewing_html()
        for i in br.response().readlines():
            if re.search("task #"+str(taskID)+"</a></em>: ", i):
                line = i.split("task #"+str(taskID)+"</a></em>: ")[1]
                return line.split("</h2>")[0]
                
    except:
        print "Dataset not found on Savannah"
        return False
def getTaskID(name, category, username, password, isParent):
    
    skip = False
    try:
        br = mechanize.Browser()
        br.open("https://savannah.cern.ch/projects/cmgsample/")
        
        br.follow_link(text_regex="Login")
        # Check response is HTML
        assert br.viewing_html()

        # Select password form (second item in forms list)
        br.select_form(nr=1)
        
        br.form['form_loginname']=username
        br.form['form_pw']=password
        br.submit()
        br.follow_link(url_regex='/task/\?func\=search')
        br.select_form(nr=1)
        newName = name
        if re.search("---\*", name):
            newName = re.sub("---\*/.*", "---", name)
            name = re.sub("---\*","---.*", name)
        br.form['words']=newName
        br.submit()
    except KeyboardInterrupt:
        raise
    except:
        skip =True
        
    if skip is False:
        # Try to access form (if you can, there was only 1 result)
        try:
            
            # This line will throw the exception if it needs to be thrown
            br.select_form(name='item_form')
            # Check if task is 100% match
            if re.search(name,br.form['summary']):
                # Check task is "Open"
                if br.form['category_id']==['101'] and category == '101':
                    print "Test dataset"
                
                if br.form['status_id']==['1'] and ((br.form['category_id']==['101'] and category == '101') or (br.form['category_id']!=['101'] and category != '101')):
                    for i in br.response().readlines():
                        if re.search("<title>CMG", i)>0:
                    	    task = i.split("#")[1]
                    	    if isParent: return [task.split(",")[0],]
                    	    else: return task.split(",")[0]
        except KeyboardInterrupt:
            raise
        # If exception is thrown, a list of results was returned and we must navigate to the correct one
        except:
            links = None
            # Retrieve a list of links
            if isParent: 
                links=br.links(text_regex=name + "$")
            else: links=br.links(text_regex=name+"$")
            checkedLinks = []
            for i in links:
                
                #For every link follow it and examine::                
                br.follow_link(i)
                br.select_form(name='item_form')
                # .. That status is "Open"
                if br.form['status_id']==['1'] and ((br.form['category_id']==['101'] and category == '101') or (br.form['category_id']!=['101'] and category != '101')):
                    for i in br.response().readlines():
                        if re.search("<title>CMG", i):
                    	    task = i.split("#")[1]
                    	    checkedLinks.append(task.split(",")[0])
            
            try:
                if isParent: return checkedLinks
                else: return checkedLinks[0]
            except:
                return None
    else: 
        print "Login failed"
        raise
    return None
    
def validLogin(username, password):
            br = mechanize.Browser()
            br.open("https://savannah.cern.ch/projects/cmgsample/")
            # Find Login link and follow it
            if br.links(text_regex="Login"):
            	try:
                    br.follow_link(text_regex="Login")
                    # Check response is HTML
                    assert br.viewing_html()

                    # Select password form (second item in forms list)
                    br.select_form(nr=1)

                    br.form['form_loginname']=username
                    br.form['form_pw']=password
                    br.submit()
                    

                    page = br.response().read()
                    a = re.search("Not Logged In",page )
                    if a != None:
                             return False
                             print "Client was unable to login, please check login details."
                    else: 
                    	return True
                except KeyboardInterrupt:
                    raise
                except:
                	return False