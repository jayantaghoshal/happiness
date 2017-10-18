import abc

class BaseCommand(metaclass=abc.ABCMeta):

    def __init__(self, options):
        self._options = options
    
    @abc.abstractmethod
    def run(self):
        """Execute command"""
