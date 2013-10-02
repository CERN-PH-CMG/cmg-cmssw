import cmd
import os

from CMGTools.Common.templates.templates import getTemplate

class Factory(cmd.Cmd):
    
    def __init__(self):
        cmd.Cmd.__init__(self)
        
        self.intro = u"""
****************************************************************************************************        
* \u00b5PAT - A framework for really easy analyses in the full framework by Colin Bernet and Will Reece *
****************************************************************************************************

Creates a cmg::AbstractPhysicsObject object, factory, and python config. See "help" and "showconfig"!


        """.encode('utf-8')
        
        #this is a bit cool!
        self.prompt = u'\u00b5PAT> '.encode('utf-8')
        
        #the config to be used
        self.config = {}
        
        self.config['inputcollection'] = 'selectedPatMuons'
        self.config['_inputtype'] = 'reco::Candidate'
        self.config['objectname'] = 'Foo'
        self.config['namespace'] = 'cmg'
        self.config['formatpackage'] = 'AnalysisDataFormats/CMGTools'
        self.config['package'] = 'CMGTools/Common'
        self.config['outputdir'] = 'FACTORY'

        self.config['_baseclass'] = 'cmg::AbstractPhysicsObject'
        self.config['_baseclassconstructor'] = 'cmg::AbstractPhysicsObject::AbstractPhysicsObject(o.p4())'
        
        #filenames
        self.config['_classname'] = '%s::%s'
        self.config['_factoryname'] = '%sFactory'
        self.config['_producername'] = '%s%sPOProducer'
        self.config['_histogramname'] = '%s%sLorentzVector'
        self.config['_pyfactoryname'] = '%s%sFactory'
        self.config['_pysequencename'] = '%s%sSequence'
        self.config['_pluginname'] = 'Plugins'
        
        self.config['ffobject'] = None
        self.config['_formatclassfile'] = None
        self.config['_factoryclassfile'] = None
        self.config['_histogramconfigfile'] = None
        self.config['_factoryconfigfile'] = None
        self.config['_pluginlibname'] = None
        self.config['_factorylibname'] = None
        self.config['_formatlibname'] = None
        
        self.configured = False
        
        self.write = True
        self.files = []
    
    
    #### utility functions    
    def do_EOF(self,line):
        """EOF or Ctrl+D
        Exit this prompt without writing any files."""
        print ''
        return True
    def do_quit(self, line):
        """quit
         Exit this prompt without writing any files."""
        return self.do_EOF(line)
    def do_shell(self, line):
        """! [cmd] or shell [cmd]
        Execute a command in the shell."""
        if line and line != 'EOF':
            os.system(line)
    def do_less(self,line):
        """less [file]
        Runs the command 'less' on a file.
        """
        if line and line != 'EOF':
            os.system('less %s' % line)
   
    def setconfig(self,name,line):
        if line is not None and line and line != 'EOF':
            self.config[name] = line
            
    def getfilepath(self,outputFile):
        '''Removes the topdir'''
        elements = outputFile.split(os.sep)
        if len(elements) > 1 and elements[0] == self.config['outputdir']:
            elements = elements[1:]
        return os.sep.join(elements)
    
    def getpythonfilepath(self, outputFile):
        '''removes the topDir and change to a python import'''
        elements = outputFile.split(os.sep)
        elements_output = []
        for e in elements:
            if e not in ['python',self.config['outputdir']]:
                fileName, ext = os.path.splitext(e)
                if ext.lower() == '.py': 
                    elements_output.append(fileName)
                else:
                    elements_output.append(e)
        return '.'.join(elements_output)
    
    def writeTemplate(self,outputDir, outputFile, template):
        
        #store the files we actually write
        self.files.append(outputFile)
        if not self.write:
            return
        
        #rename hidden variables!
        config = {}
        for k,c in self.config.iteritems():
            if k.startswith('_'):
                config[k[1:]] = c
            else:
                config[k] = c
        
        if not os.path.exists(outputDir):
            os.makedirs(outputDir)
        output = file(outputFile,'w')
        output.write(template % config)
        output.close()
        print "Written: '%s'" % outputFile
        
    def getClassesDef(self):
        
        template = '''<class name="%s"/>'''
        lines = []
        lines.append(template % self.config['_classname'])
        lines.append(template % ('std::vector< %s >' % self.config['_classname']))
        lines.append(template % ('edm::Wrapper< %s >' % self.config['_classname']))
        lines.append(template % ('edm::Ptr< %s >' % self.config['_classname']))
        lines.append(template % ('edm::Wrapper<std::vector< %s > >' % self.config['_classname']))
        
        if self.config['ffobject'] is not None:
            lines.append(template % ('cmg::PhysicsObjectWithPtr< edm::Ptr< %s > >' % self.config['ffobject']))
        return '\n'.join(lines)
        
            
    def configure(self):
        
        if self.configured:
            return

        self.config['_classname'] = self.config['_classname'] % (self.config['namespace'],self.config['objectname'])
        self.config['_factoryname'] = self.config['_factoryname'] % (self.config['objectname'])
        self.config['_producername'] = self.config['_producername'] % (self.config['namespace'],self.config['objectname'])
        self.config['_histogramname'] = self.config['_histogramname'] % (self.config['namespace'],self.config['objectname'])
        self.config['_pyfactoryname'] = self.config['_pyfactoryname'] % (self.config['namespace'],self.config['objectname'])
        self.config['_pysequencename'] = self.config['_pysequencename'] % (self.config['namespace'],self.config['objectname'])
        
        def makeGuard(self,package,objectname):
            fileName = os.path.join(self.config[package],'%s.h' % self.config[objectname])
            fileName = fileName.upper()
            fileName = fileName.replace(os.sep,'_')
            fileName = fileName.replace('.','_')
            return '%s_' % fileName
        self.config['_formatguard'] = makeGuard(self,'formatpackage','objectname')
        self.config['_factoryguard'] = makeGuard(self,'package','_factoryname')
        
        def makePluginName(self, package,classname, name):
            name = os.path.join(package,classname,name)
            return name.replace(os.sep,'')
        self.config['_pluginlibname'] = makePluginName(self,self.config['package'],self.config['_factoryname'],self.config['_pluginname'])
        self.config['_factorylibname'] = makePluginName(self,self.config['package'],self.config['_factoryname'],'')
        self.config['_formatlibname'] = makePluginName(self,self.config['formatpackage'],self.config['_factoryname'],'')
        
        if self.config['ffobject'] is not None:
            self.config['_baseclass'] = 'cmg::PhysicsObjectWithPtr< edm::Ptr< %s > >' % self.config['ffobject']
            self.config['_baseclassconstructor'] = 'cmg::PhysicsObjectWithPtr<value>::PhysicsObjectWithPtr(o)'
            self.config['_inputtype'] = 'value'

        self.configured = True
        

    def do_showconfig(self, line):
        """showconfig [all]
        Shows the current config that will be used.
        """
        all = False
        if line == 'all':
            all = True
        keys = self.config.keys()
        if keys:
            print 'Factory config:'
        for k in keys:
            if not k.startswith('_') or all:
                print "\t %s: '%s'" % (k,self.config[k])
                
    def do_showfiles(self,line):
        """showfiles
        Show the files to be written with the current config.
        """
        
        self.write = False
        self.files = []
        self.do_writeall(line)
        
        self.files.sort()
        if self.files:
            print 'Files to write:'
            for f in self.files:
                print "\t File: '%s'" % f
        
        self.write = True
        self.files = []
        
            
    def do_writeobject(self, line):
        """writeobject
        Writes the generated PhysicsObject
        """
        self.configure()
        
        template = getTemplate('PhysicsObject_h.txt')
        outputDir = os.path.join(self.config['outputdir'],self.config['formatpackage'],'interface')
        outputFile = os.path.join(outputDir,'%s.h' % self.config['objectname'])
        self.writeTemplate(outputDir, outputFile, template)
        # cache the name of the file to write the factory
        self.setconfig('_formatclassfile',self.getfilepath(outputFile))
        
        #write the formats buildfile
        template = getTemplate('BuildFile_formats.txt')
        outputDir = os.path.join(self.config['outputdir'],self.config['formatpackage'])
        outputFile = os.path.join(outputDir,'BuildFile')
        self.writeTemplate(outputDir, outputFile, template)
        
        #write classes.h
        template = getTemplate('classes_h.txt')
        outputDir = os.path.join(self.config['outputdir'],self.config['formatpackage'],'src')
        outputFile = os.path.join(outputDir,'classes.h')
        self.writeTemplate(outputDir, outputFile, template)
        
        #write classesdef.xml
        template = getTemplate('classes_def_xml.txt')
        outputDir = os.path.join(self.config['outputdir'],self.config['formatpackage'],'src')
        outputFile = os.path.join(outputDir,'classes_def.xml')
        
        self.config['lcgdict'] = self.getClassesDef()
        self.writeTemplate(outputDir, outputFile, template)
        del self.config['lcgdict']
        

    def do_writefactory(self, line):
        """writefactory
        Writes the generated PhysicsObject factory
        """
        self.configure()
          
        #now write the factory BuildFile
        outputDir = os.path.join(self.config['outputdir'],self.config['package'])
        outputFile = os.path.join(outputDir,'BuildFile')
        template = getTemplate('BuildFile_factory.txt')
        self.writeTemplate(outputDir, outputFile, template)
        
        # now write the factory header
        template = getTemplate('PhysicsObjectFactory_h.txt')
        outputDir = os.path.join(self.config['outputdir'],self.config['package'],'interface')
        outputFile = os.path.join(outputDir,'%s.h' % self.config['_factoryname'])
        self.writeTemplate(outputDir, outputFile, template)
        # cache the name of the file to write the factory
        self.setconfig('_factoryclassfile',self.getfilepath(outputFile))
        
        ## now the src file
        outputDir = os.path.join(self.config['outputdir'],self.config['package'],'src')
        outputFile = os.path.join(outputDir,'%s.cc' % self.config['_factoryname'])
        template = getTemplate('PhysicsObjectFactory_cc.txt')
        self.writeTemplate(outputDir, outputFile, template)
        
        #now write the plugins file
        outputDir = os.path.join(self.config['outputdir'],self.config['package'],'plugins')
        outputFile = os.path.join(outputDir,'%s.cc' % self.config['_pluginname'])
        template = getTemplate('PhysicsObjectPlugins_cc.txt')
        self.writeTemplate(outputDir, outputFile, template)
       
        #now write the plugins BuildFile
        outputDir = os.path.join(self.config['outputdir'],self.config['package'],'plugins')
        outputFile = os.path.join(outputDir,'BuildFile')
        template = getTemplate('BuildFile_plugins.txt')
        self.writeTemplate(outputDir, outputFile, template)
       
        #now write the python factory file
        outputDir = os.path.join(self.config['outputdir'],self.config['package'],'python','factories')
        outputFile = os.path.join(outputDir,'%s_cfi.py' % self.config['_pyfactoryname'])
        self.setconfig('_factoryconfigfile',self.getpythonfilepath(outputFile))
        template = getTemplate('PhysicsObjectFactory_py.txt')
        self.writeTemplate(outputDir, outputFile, template)
        
        #now write the python histogram file
        outputDir = os.path.join(self.config['outputdir'],self.config['package'],'python','histograms')
        outputFile = os.path.join(outputDir,'%s_cfi.py' % self.config['_histogramname'])
        self.setconfig('_histogramconfigfile',self.getpythonfilepath(outputFile))
        template = getTemplate('Histograms_py.txt')
        self.writeTemplate(outputDir, outputFile, template)
     
        #now write the python histogram file
        outputDir = os.path.join(self.config['outputdir'],self.config['package'],'python')
        outputFile = os.path.join(outputDir,'%s_cff.py' % self.config['_pysequencename'])
        template = getTemplate('Sequences_py.txt')
        self.writeTemplate(outputDir, outputFile, template)
      
    def do_writeall(self,line):
        """writeall
        Write all the files according to the current config.
        """
        self.do_writeobject(line)
        self.do_writefactory(line)
            
    #### functions for setting parameters
    def do_setinputcollection(self,line):
        """setinputcollection [src]
        Set the name of the collection to run the factory on
        """
        self.setconfig('inputcollection', line)
    
    def do_setobjectname(self, line):
        """setobjectname [name]
        Set the name of the object that this factory will produce.
        """
        self.setconfig('objectname', line)
            
    def do_setnamespace(self, line):
        """setnamespace [namespace]
        Set the C++ namespace that this class will be created in.
        """
        self.setconfig('namespace', line)
            
    def do_setpackage(self, line):
        """setpackage [packagename]
        Set the C++ package for the main code
        """
        self.setconfig('package', line)
        
    def do_setformatpackage(self, line):
        """setformatpackage [packagename]
        Set the C++ package for the PhysicsObject format class
        """
        self.setconfig('package', line)
        
    def do_setffobject(self, line):
        """setffobject [ffobject]
        Sets the full framework (e.g. pat) object that this PhysicsObject is created from.
        """
        self.setconfig('ffobject', line)
        
    def do_setoutputdir(self,line):
        """setoutputdir [dir]
        Set the name of the top level dir to write into.
        """
        self.setconfig('outputdir', line)

if __name__ == '__main__':
    

    f = Factory()
    f.cmdloop()