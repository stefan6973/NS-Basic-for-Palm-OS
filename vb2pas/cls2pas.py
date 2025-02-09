import os
from x2pas import VBConverter

class VBClass(VBConverter):
    def __init__(self, path):
        super().__init__(path)

    def convert(self, dst_folder: str):
        self.load
        pass