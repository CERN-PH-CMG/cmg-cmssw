class Example(object):
    counter = 0
    def __init__(self):
        self.__class__.counter += 1
        print 'instance', self.__class__.counter


e1 = Example()
e2 = Example()
