#!
# Author: pmeckiff
# Group: CMG

import re, mechanize, types, cookielib, datetime



class savannahConnect:
    # Creates browser object and calls login method
    def __init__(self, username, password):
        self.user = username
        self.br = mechanize.Browser()
        self.br.open("https://savannah.cern.ch/projects/cmgsample/")
        self.valid = False
        self.login(username, password)

    # From DbsProcessedDataset object prints to a string which can be posted on Savannah
    def datasetString(self, details, files, tags):
        detailArray = []
        # Print name and primary set at top
        detailArray.append( "*%s* " % details['PathList'][0])
        detailArray.append("\t*Status*: " + details['Status'])
        castor = "/store/cmst3/user/" + details['CreatedBy'] + "/" + details['PhysicsGroup'] + details['PathList'][0] 
        detailArray.append("\t*Castor Directory*:\n\t" + castor)
        if details['ParentList']: detailArray.append("\t*Parent Dataset*: \n\t" + details['ParentList'][0])
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
    def getParentURL(self, name):
        # If logged in
        if self.valid==True and name != None:
            self.br.select_form(nr=0)
            self.br.form['words']=name
            self.br.form['type_of_search']=['task',]
            self.br.submit()
            links=self.br.links(text_regex=name+"$")
            for i in links:
                link = i.absolute_url
                self.br.back()
                htmlLink = "[" +link+" "+name+"]"
                return htmlLink
            self.br.back()
            return name

    def submitItem(self, dataset, files, tags, username):
        # If logged in
        if self.valid == True:
            dataset['ParentList'][0] = self.getParentURL(dataset['ParentList'][0])

            response1=self.br.follow_link(text_regex='Submit a new item', url_regex="task")


            # Select form for item data submission
            self.br.select_form(name='trackers_form')
            dayMonthYear = datetime.date.fromtimestamp(int(dataset['DateCreated'])).strftime('%d-%B-%Y').split("-")
            
            
            # Input item data
            self.br.form['planned_starting_date_dayfd']=[dayMonthYear[0].lstrip("0")]

            self.br.form.set_value_by_label([dayMonthYear[1]],'planned_starting_date_monthfd')

            self.br.form['planned_starting_date_yearfd']=dayMonthYear[2]
            self.br.form['summary'] = dataset['PathList'][0]
            self.br.form['priority']= ['5']
            self.br.form['details']= self.datasetString(dataset, files, tags)
            self.br.form.set_value_by_label(["Done"],'resolution_id')

            # If user does not exist in group, savannah entry cannot be assigned, so do not submit
            try:
                self.br.form.set_value_by_label([username], "assigned_to")
                
                self.br.submit()
                
                print "Dataset has been posted to Savannah"
            except:   
                # If user wasnt found print error message
                
                print "User \"%s\" is not a CMG group member on Savannah" % username
                print "-No entry made on Savannah-"
                
            
            # Return the url that users would use to access the savannah page
            return "https://savannah.cern.ch" +self.getItemURL(self.br.response()).split("=")[-1]
          
        else:
            print "User not logged in to Savannah"
            return None

        

