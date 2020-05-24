

class OpenClose:
    """Inherit from this class for context management"""

    def __init__(self):
        pass

    def open(self):
        return self

    def close(self):
        pass

    def __enter__(self):
        return self.open()

    def __exit__(self, exc_type, exc_val, exc_tb):
        return self.close()
