import inspect
import os

def getTemplate(name):
    #get the location of *this* function
    topDir = os.path.abspath(os.path.dirname(inspect.getsourcefile(getTemplate)))
    templateName = os.path.join(topDir,name)
    
    #read the template contents from the file
    result = None
    if os.path.exists(templateName):
        f = None
        try:
            f = file(templateName)
            result = f.read()
        finally:
            if f is not None: f.close()
    return result