import logging


class LogConfig():
    #def _init_(self, logName = 'logOutput.log'):
    #create a logger
    logger = logging.getLogger('statisticNew')
    logger.setLevel(logging.DEBUG)
    #create a handler, it's used to write the log
    fh = logging.FileHandler('logOutput.log', 'w')
    fh.setLevel(logging.DEBUG)
    #create a handler, it's used to output to the console
    ch = logging.StreamHandler()
    ch.setLevel(logging.DEBUG)
    #define the format of output
    formatSet = logging.Formatter('%(asctime)s [%(filename)s:%(lineno)d]-%(message)s')
    fh.setFormatter(formatSet)
    ch.setFormatter(formatSet)
    #add logger for handler
    logger.addHandler(fh)
    logger.addHandler(ch)
    def getLog(self):
        return self.logger
