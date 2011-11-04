#!
# Author: pmeckiff
# Group: CMG

import re, types, cookielib, datetime

import CMGTools.Production.eostools as castortools
import CMGTools.Production.mechanize as mechanize


class savannahConnect:
    # Creates browser object and calls login method
    def __init__(self, username, password):
        self.user = username
        self.br = mechanize.Browser()
        self.br.open("https://savannah.cern.ch/projects/cmgsample/")
        self.valid = False
        self.login(username, password)

    # From DbsProcessedDataset object prints to a string which can be posted on Savannah
    def datasetString(self, details, files, tags, castorDir, comment):
        detailArray = []
        # Print name and primary set at top
        detailArray.append( "*%s* " % details['PathList'][0])
        detailArray.append("\t*Status*: " + details['Status'])
        castor = "/store/cmst3/user/" + details['CreatedBy'] + "/" + details['PhysicsGroup'] + details['PathList'][0] 
        detailArray.append("\t*LFN Castor Directory*:\n\t" + castortools.castorToLFN(castorDir))
        detailArray.append("\t*Castor Directory*:\n\t" + castorDir)
        
        if details['ParentList']: detailArray.append("\t*Parent Dataset*: \n\t" + details['ParentList'][0])
        else: detailArray.append("\t*Parent Dataset*: \n\t No Parent")
        # Print all other data
        for element in details:
            if element != "DateCreated" and element != "PhysicsGroup"  and element != 'CreatedBy':
                pass
            # If no entry found:
            elif not details[element]:
                detailArray.append("\t*%s*: No entry found" % element)
            # If item is a list
            elif type(details[element]) == list:
                # Create human readable name
                name =  "\t*%s*:" % re.sub( r"([A-Z])", r" \1", element).lstrip(" ")
                # If list is empty:
                if not details[element]: detailArray.append(name + ": No entry found")
                else: 
                    detailArray.append(name)
                    for i in details[element]: detailArray.append("\t\t%s"% i)
            # If item is a date attribute
            elif element == 'LastModificationDate' or element == 'DateCreated':
                # Takes timestamp in dataset object and converts to a human readable date
                detailArray.append("\t*%s*:" % re.sub( r"([A-Z])", r" \1", element).lstrip(" ") + " " + datetime.date.fromtimestamp(int(details[element])).strftime('%d-%m-%Y'))
            # Otherwise just print the value
            else:
                detailArray.append("\t*%s*: " % re.sub( r"([A-Z])", r" \1", element).lstrip(" ") + details[element])
        
        # Cycle through array of details and add them to a string
        detailString = ""
        for i in detailArray:
            detailString += i + "\n"

        detailString+= "\n"
        if tags!=None:
            detailString+="*Tags*: \n"
            for i in tags:
                tag = i.split(" ")[0]
                package = i.split(" ")[1]
                detailString +="_"+tag+"_ -- "+package +"\n"
            
        if files!=None:
            detailString+="\n*Root files*:\n"
            for i in files:
                detailString +="\t"+ i + "\n\n"

        if comment!= None:
            detailString+="\n*Additional Comments*\n"+ comment
        return detailString

    # Used to determine if login was successful
    def isValid(self):
        return self.valid

    # Log the browser object into the users Savannah account
    def login(self, username, password):
        # Check if client is logged in already, to avoid re-calling this method unneccesarily
        if self.valid == True:
            print "Client is already logged in"
        else:
            # Find Login link and follow it
            if self.br.links(text_regex="Login"):
                response1 = self.br.follow_link(text_regex="Login")
                # Check response is HTML
                assert self.br.viewing_html()

                # Select password form (second item in forms list)
                self.br.select_form(nr=1)

                self.br.form['form_loginname']=username
                self.br.form['form_pw']=password
                self.br.submit()
  
                #for i in self.br.forms(): print i
                page = self.br.response().read()
                a = re.search("Not Logged In",page )
                #for i in dir(self.br.response()): print i
                if a != None:
                     self.valid = False
                     print "Client was unable to login, please check login details."
                else: self.valid = True
    def getItemURL(self, response):
        URL = None
        for i in response.readlines():
            if re.search("New item posted", i)>0:
                URL = i.split("New item posted")[1]
                URL = URL.split("\"")[1]
                
        return URL
    def getParentURL(self, name, user, category):
        # If logged in
        if self.valid==True and name != None:
            self.br.follow_link(url_regex='/task/\?func\=search')
            self.br.select_form(nr=1)
            self.br.form['words']=name
            self.br.submit()
            # Declare link variable
            link = name
            posLink = []
            posUser = []
            flag = False
            # Try to access form (if you can, there was only 1 result)
            try:
                # This line will throw the exception if it needs to be thrown
                self.br.select_form(name='item_form')
                # Check if task is 100% match
                if re.match(self.br.form['summary'], name):
                    # Check task is "Open"
                    if self.br.form['status_id']==['1'] and self.br.form['category_id']==[category]:
                        # Check the task is not assigned to someone else
                        control = self.br.find_control('assigned_to')
                        if control.get_value_by_label() == [user]:
                            link = "["+self.br.response().geturl() +" "+ name+ "]"
                        else:
                            posLink.append( "["+self.br.response().geturl() +" "+ name+ "]")
                            posUser.append(control.get_value_by_label())
                else: print "Success"
                self.br.back()
                self.br.back()
            # If exception is thrown, a list of results was returned and we must navigate to the correct one
            except:
                # Retrieve a list of links
                links=self.br.links(text_regex=name+"$")
                for i in links:
                    #For every link follow it and examine::
                    url = i.absolute_url
                    self.br.follow_link(i)
                    self.br.select_form(name='item_form')
                    # .. That status is "Open"
                    
                    if self.br.form['status_id']==['1'] and self.br.form['category_id']==[category]:
                        
                        # .. And that the task is assigned to the correct user
                        self.br.find_control('assigned_to')
                        control = self.br.find_control('assigned_to')
               
                        if control.get_value_by_label() == [user]:
                            link = "["+self.br.response().geturl()+" " + name+ "]"
                            # Go back to list of tasks, as break will cause algorithm to skip over inline back() instruction
                            self.br.back()
                            break
                        else:
                            posLink.append( "["+self.br.response().geturl() +" "+ name+ "]")
                            posUser.append(control.get_value_by_label())
                    # Go back to list of tasks
                    self.br.back()
                # Go back to main page
                self.br.back()
                self.br.back()
            if link == name:
                if len(posLink)>0:
                    link = posLink[self._getOptionalParent(posUser, name)]
                
            return link
    def _getOptionalParent(self, posUser, name):
        parentString = ""
        count = 1
        for i in posUser:
            parentString += i[0] + ": [" + str(count)+"]\n"
            
            count+=1
        while True: 
            parent = raw_input("A parent was found but belonging to a different user please select one of the following users,\nor push any other key to exit:\n" + parentString)
            try:
                if int(parent)<=len(posUser):
                    if int(parent)>0:
                        return int(parent)-1
            except: pass
            return name
        
    def _getSelfURL(self, name, user, category):
        # If logged in
        if self.valid==True and name != None:
            self.br.follow_link(url_regex='/task/\?func\=search')
            self.br.select_form(nr=1)
            self.br.form['words']=name
            self.br.submit()
            
            # Try to access form (if you can, there was only 1 result)
            try:
                # This line will throw the exception if it needs to be thrown
                self.br.select_form(name='item_form')
                # Check if task is 100% match
                if re.search(name,self.br.form['summary']):
                    # Check task is "Open"
                    if self.br.form['status_id']==['1'] and self.br.form['category_id']==[category]:
                        # Check the task is not assigned to someone else
                        control = self.br.find_control('assigned_to')
                        if control.get_value_by_label() == [user]:
                            # Follow link to enable comment posting
                            self.br.follow_link(url_regex="#postcomment")
                            return True
                
            # If exception is thrown, a list of results was returned and we must navigate to the correct one
            except:
                # Retrieve a list of links
                links=self.br.links(text_regex=name+"$")
                
                for i in links:
                    #For every link follow it and examine::
                    url = i.absolute_url
                    self.br.follow_link(i)
                    self.br.select_form(name='item_form')
                    # .. That status is "Open"
                    if self.br.form['status_id']==['1'] and self.br.form['category_id']==[category]:

                        # .. And that the task is assigned to the correct user
                        self.br.find_control('assigned_to')
                        control = self.br.find_control('assigned_to')
               
                        if control.get_value_by_label() == [user]:
                            # If all is true, follow comment posting link
                            self.br.follow_link(url_regex="#postcomment")
                            return True
                    # If link was not correct go back
                    self.br.back()
            # If no link was found at all go back to original page
            self.br.back()
            self.br.back()
            return False


    def submitItem(self, dataset, files, tags,castorDir, username, test, comment):
        # If logged in
        if self.valid == True:
            category = '100'
            if test: category = '101'
            # Find if item has parent or not
            if dataset['ParentList']:
                dataset['ParentList'][0] = self.getParentURL(dataset['ParentList'][0], username, category)
            # navigate to submit new task page
            self.br.follow_link(text_regex='Submit a new item', url_regex="task")
            #if item already exists, navigate to its page and 
            previousEntry= self._getSelfURL(dataset['PathList'][0], username, category)
            
            
            
            option = "y"
            if previousEntry:
                option = raw_input("Task for this dataset already exists on Savannah.\nAdd new information as comment?(y/n)")
            
            while True:
                
                
                if option == 'y':
                    # Input item data
                    # If dataset already exists
                    if previousEntry:
                        self.br.select_form(name='item_form')
                        self.br.form['comment']= self.datasetString(dataset, files, tags, castorDir, comment)

                    # If dataset is new
                    else:
                        self.br.select_form(name='trackers_form')
                        self.br.form['details']= self.datasetString(dataset, files, tags, castorDir, comment)
                        dayMonthYear = datetime.date.fromtimestamp(int(dataset['DateCreated'])).strftime('%d-%B-%Y').split("-")
                        self.br.form['planned_starting_date_dayfd']=[dayMonthYear[0].lstrip("0")]
                        self.br.form.set_value_by_label([dayMonthYear[1]],'planned_starting_date_monthfd')
                        self.br.form['planned_starting_date_yearfd']=dayMonthYear[2]
                        self.br.form['summary'] = dataset['PathList'][0]

                    if test:self.br.form['category_id']= ['101']
                    self.br.form['priority']= ['5']
                    if dataset['Status']=="INVALID":
                        self.br.form.set_value_by_label(["Invalid"],'resolution_id')
                    else:
                        self.br.form.set_value_by_label(["Done"],'resolution_id')

                    # If user does not exist in group, savannah entry cannot be assigned, so do not submit
                    try:
                        self.br.form.set_value_by_label([username], "assigned_to")
                        if previousEntry:
                            self.br.submit(id="submitreturn")
                        else:
                            self.br.submit()
                        if previousEntry: print "Dataset has been updated on Savannah"
                        else: print "Dataset has been posted to Savannah"
                    except:   
                        # If user wasnt found print error message

                        print "User \"%s\" is not a CMG group member on Savannah" % username
                        print "-No entry made on Savannah-"


                    # Return the url that users would use to access the savannah page
                    if previousEntry:
                        URL = self.br.response().read().split("title>")[1].split("#")[1].split(",")[0]
                    else:
                        URL = self.getItemURL(self.br.response()).split("=")[-1].lstrip("/task/?")
                    print "https://savannah.cern.ch/task/?" + URL
                    return URL
                elif option == 'n':
                    # If item is already on Savannah and user doesn't want to add a comment
                    print "Dataset was not added to savannah"
                    return None
                else: option = raw_input("Please enter y or n:")
        else:
            print "User not logged in to Savannah"
            return None



