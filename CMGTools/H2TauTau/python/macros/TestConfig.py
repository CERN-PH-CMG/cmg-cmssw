import sys
import imp

if len( sys.argv )!=2:
    print 'TestConfig.py <config.py>'
    print 'Specify cfg file!'
    sys.exit(1)

cfgFileName = sys.argv[1]

file = open( cfgFileName, 'r' )
anacfg = imp.load_source( 'cfg', cfgFileName, file)

print anacfg.config
