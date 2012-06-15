from CMGTools.Production.ProductionTasks import Task
from CMGTools.Production.publish import publish
import os, sys

class PublishTask(Task):
    """Publish the dataset in DBS, Savannah"""
    def __init__(self, dataset, user, options):
        Task.__init__(self,'PublishTask', dataset, user, options)
        self.password = None

    @staticmethod
    def addOptionStatic(parser):
        # This option will be used to find dataset on castor, and assign dataset on savannah
        parser.add_option("-F", "--fileown", 
                         dest="fileown",
                         help="User who is the files owner on EOS." ,
                         default=os.environ['USER'] )
        # If the purpose is to test the software use this parameter, it will not be recognised by the
        # non-testing algorithm
        parser.add_option("-T", "--test",
                         action = "store_true",
                         dest="test",
                         help="Flag task as a test",
                         default=False )
        parser.add_option("--ns", "--nosavannah",
                         action = "store_false",
                         dest="savannah",
                         help="Do not publish to savannah",
                         default=True )
        # If user wants to add their own comments
        parser.add_option("-C", "--comment",
                         action = "store",
                         dest="commented",
                         help="Take comment as an argument",
                         default = None)

        # If user wants to add their own comments
        parser.add_option("-f", "--force",
                         action = "store_true",
                         dest="force",
                         help="force publish without logger",
                         default = False)
        # If user wants to add their own comments
        parser.add_option("-G", "--groups",
                         action = "store_true",
                         dest="checkGroups",
                         help="check the related group accounts on EOS",
                         default = False)

    def addOption(self,parser):
        self.addOptionStatic(parser)

    @staticmethod
    def getPassword(username, retries = 3):
        import getpass
        from CMGTools.Production.findDSOnSav import validLogin
        
        password = None
        for i in xrange(retries):
            pw = getpass.getpass("Enter NICE Password [%d/%d] for user '%s': " % (i+1,retries,username) )
            if validLogin(username, pw):
                password = pw
                break
            else:
                print >> sys.stderr, 'Password incorrect. %d retries left...' % (retries-(i+1))
        return password

    def run(self, input):
        username = os.getlogin()
        if self.password is None:
            self.password = self.getPassword()
        return publish(self.dataset,self.options.fileown,\
                    self.options.commented,self.options.test,\
                    username,self.password,self.options.force,\
                    self.options.checkGroups, self.options.savannah,\
                    (self.options.min_run, self.options.max_run) )

