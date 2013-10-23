import ConfigParser
from pprint import pprint

from CMGTools.H2TauTau.macros.Weight import Weight

class Component(object):
    def __init__(self, config, componentName):
        if not config.has_section( componentName):
            raise ValueError( ': '.join(['no such section', componentName, configName]))
        self.name = componentName
        self.items = dict( config.items(componentName) )
        self.files = self.items['files']
        self.triggers = self.items['trigger']# .split('%')
        try:
            self.isMC = config.getboolean(componentName, 'mc')
        except ConfigParser.NoOptionError:
            self.isMC = False
        self.lumi = self.items.setdefault('lumi', None)
        if self.lumi is not None:
            self.lumi = float(self.lumi)
        self.xsection = self.items.setdefault('xsection', None)
        if self.xsection is not None:
            self.xsection = float(self.xsection)
        self.eff_corrfactor = self.items.setdefault('eff_corrfactor', '1')
        self.eff_corrfactor = eval(self.eff_corrfactor)
        try:
            self.useTurnOn = config.getboolean(componentName, 'use_turn_on')
        except ConfigParser.NoOptionError:
            self.useTurnOn = False
        self.vertexWeight = self.items.setdefault('vertexweight', None)
        if self.vertexWeight is not None and self.vertexWeight.strip().lower() == 'none':
            self.vertexWeight = None
        #COLIN: warning, n_gen_events is a string here...
        self.n_gen_events = self.items.setdefault('n_gen_events', -1)
        self.n_gen_events = int( self.n_gen_events)
        self.gen_eff = 1
        try:
            self.recoil = config.getboolean(componentName, 'recoil')
        except ConfigParser.NoOptionError:
            self.recoil = False
        if self.isMC is True:
            if self.xsection is None:
                raise ValueError('\n'.join(['xsection must be specified for MC samples.',str(self)]))
            if self.n_gen_events < 0:
                raise ValueError('\n'.join(['n_gen_events must be specified for MC samples.',str(self)]))                 
        elif self.lumi is None:
            print self.isMC
            raise ValueError('\n'.join(['lumi must be specified for data samples.',str(self)]))

    def GetWeight(self):
        '''Returns the weight, for a MC sample

        COLIN: Hum what happens when called on a data sample?'''
        return Weight( self.n_gen_events,
                       self.xsection,
                       self.gen_eff,
                       self.lumi,
                       self.eff_corrfactor ) 

    def __str__(self):
        restr = None
        if self.isMC:
            return '''%s :
            files     = %s
            trigger   = %s
            eff_corr_factor = %3.4f
            use_turn_on = %s
            vertexweight = %s 
            xsection  = %s
            n_gen_events = %s
            recoil    = %s 
            MC?       = %s 
            ''' % (self.name, self.files, self.triggers, self.eff_corrfactor,
                   self.useTurnOn,
                   str(self.vertexWeight),
                   self.xsection, self.n_gen_events, self.recoil, self.isMC)
        else:
            return '''%s :
            files     = %s
            trigger   = %s
            eff_corr_factor = %3.4f
            lumi      = %s 
            MC?       = %s 
            ''' % (self.name, self.files, self.triggers, self.eff_corrfactor,
                   self.lumi, self.isMC)


class Defaults(object):
    def __init__(self, config):
        self.items = config.defaults()
        self.components = self.items['components'].split(',')
        # remove empty strings from component list
        # happens in case of a trailing of leading coma..
        self.components = filter( lambda x : x is not '', self.components)
        self.mc_eff_corrfactor = self.items.setdefault('mc_eff_corrfactor', 1)
        self.mc_eff_corrfactor = eval(self.mc_eff_corrfactor)
        self.mc_vertexWeight = self.items.setdefault('mc_vertexweight', None)
        self.tauPtCut = float( self.items.get('tauptcut', -1.) )
        self.leptonPtCut = float( self.items.get('leptonptcut', -1.) )
        self.leptonEtaCut = float( self.items.get('leptonetacut', -1.) )
        
    def __str__(self):
        header = 'GLOBAL:'
        components = '\tComponents: '
        components += ', '.join(self.components)
        mc_eff_corrfactor = '\tmc_eff_corrfactor = %3.4f' % self.mc_eff_corrfactor
        mc_vertexWeight = '\tmc_vertexWeight = %s' % str(self.mc_vertexWeight)
        return '\n'.join( [header, components, mc_eff_corrfactor, mc_vertexWeight] )

    
class AnalysisConfig(object):

    def __init__(self, configName):
        self.config = ConfigParser.SafeConfigParser()
        self.config.read( configName )
        self.defaults = Defaults(self.config)
        self.ParseAllComponents()

    def ParseAllComponents(self):
        self.components = {}
        for section in self.config.sections():
            self.components[section] = Component( self.config,
                                                  section )

    def SelectedComponents(self):
        sel = dict([(akey,self.components[akey]) for akey in self.defaults.components])
        return sel
        
    def __str__(self):
        retstr = str(self.defaults)
        secs = '\n'.join( [str(section) for key, section in sorted(self.components.iteritems())] )
        retstr = '\n'.join([retstr, secs])
        return retstr


if __name__ == '__main__':
    import sys
    anacfg = AnalysisConfig( sys.argv[1] )
    print anacfg

    print 'Selected Components:'
    for sel in anacfg.SelectedComponents().values():
        print sel
        
