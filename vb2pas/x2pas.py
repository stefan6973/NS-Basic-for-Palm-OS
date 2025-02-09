import os

class VBConverter:
    def __init__(self, filePath: str):
        self.src = filePath
        self.name = filePath.split('\\')[-1].split('.')[-2]
        self.ext = filePath.split('.')[-1]        
        self.lines = []
        self.__test()

    def load(self, dest_folder: str):
        self.dest = f'{dest_folder}\\{self.name}.pas'
        if os.path.exists(self.dest):
            os.remove(self.dest)
            #raise Exception('file already converted: %s' % self.dest)  
        
        with open(self.src, 'r') as f:
            self.lines = f.readlines()

    basKinds = {
        'attribute': 'unit', 
        'option': 'unit', 
        'const': 'const',
        #'type': '',
        'dim': 'var',
        'sub': 'procedure', 
        'function': 'function',
        'property': 'function',
        'class': 'class', 
    }
    
    basAttrs = (
        'public', 
        'private',
        'declare',
        )
    
    basVerbs = (
        'as',
        'get',
        'let',
        'set',
        'end',
        'if',
        'then',
    )

    basReserved = [] 
    basReserved += [b for b in basKinds]
    basReserved += [b for b in basAttrs]
    basReserved += [b for b in basVerbs]

    pasTargets = (
        'interface', 
        'implementation', 
        'initialization', 
        'finalization'
        )
    
    basTypes = {
        'boolean': 'boolean',
        'string': 'string',
        'long' : 'longword',        
    }

    def pasType(self, basType):
        if basType in self.basTypes:
            res= self.basTypes[basType]
            return res
        
        return 'unknown'
    
    def convert(self, dst_folder: str):
        raise Exception('must be implemented in a derrived class')

    def __test(self):
        try:
            if self.pasType('boolean') != 'boolean':
                raise Exception('pasType: boolean failed')
            if self.pasType('string') != 'string':
                raise Exception('pasType: string failed')
            if self.pasType('long') != 'longword':
                raise Exception('pasType: long failed')
            
        except Exception as e:
            print(e)
        
    def determineTargetType(self, line: str):
        line = line.strip()
        res = {
            'scope': 'unknown',
            'kind': 'unknown', 
            'type': '',
            'name': '',
            'value': '',
            'line': line
        }
        if line.startswith("'"): 
            res['kind'] = 'comment'
            return res

        orgWords = [x for x in line.split(' ') if x.strip() != '']
        words = [x for x in line.lower().split(' ') if x.strip() != '']
        if len(words) <= 1: return None
        
        w0 = words[0] if len(words) > 0 else ''
        w1 = words[1] if len(words) > 1 else ''
        w2 = words[2] if len(words) > 2 else '' 
        w3 = words[3] if len(words) > 3 else ''
        w4 = words[4] if len(words) > 4 else ''

        # Attribute VB_Name = "dbgUtils"
        if w0 == 'attribute' and w2 == '=':
            res['scope'] = 'unit'
            res['kind'] = w0
            res['name'] = words[1]
            res['value'] = w3.strip('\"')
            return res
        
        # Option Explicit
        if w0 == 'option':
            res['scope'] = 'unit'
            res['kind'] = w0
            res['name'] = orgWords[1]
            return res

        # Private msg_active As Boolean
        if w0 == 'private' and w2 == 'as':
            res['scope'] = 'implementation'
            res['kind'] = 'var'
            res['name'] = orgWords[1]
            res['type'] = w3
            return res
        
        # Public  Property Get isTracingEnabled() As Boolean
        # Private Property Let Prefix(value As String)
        if w1 == 'property':
            if w0 == 'public':
                res['scope'] = 'interface'
            elif w0 == 'private':
                res['scope'] = 'implementation'
            else:   
                return None
            
            if w2 == 'get' and words[-2] == 'as':
                res['kind'] = 'function'
                res['name'] = orgWords[3]
                res['type'] = words[-1]
            elif w2 == 'let' and words[4] == '(' and words[-1] == ')':
                res['kind'] = 'procedure'
                res['name'] = orgWords[3]
            else: 
                return None

            return res
        
        # Public Const CF_BITMAP = 2
        if w1 == 'const' and w3 == '=':
            if w0 == 'public':
                res['scope'] = 'interface'
            elif w0 == 'private':
                res['scope'] = 'implementation'
            elif w0 == 'global':
                res['scope'] = 'interface'
            else:
                raise Exception('unknown scope: %s' % w0)
            
            res['kind'] = 'const'
            res['name'] = orgWords[2]
            res['value'] = w4
            return res
        
        # Declare Function EmptyClipboard Lib "user32" () As Long
        if w0 == 'declare':
            if w1 == 'function':
                res['scope'] = 'interface'
                res['kind'] = 'function'
                res['name'] = orgWords[2]
                res['type'] = words[-1] if words[-2] == 'as' else ''
                return res
            if w1 == 'sub':
                res['scope'] = 'interface'
                res['kind'] = 'procedure'
                res['name'] = orgWords[2]
                return res
            
        return res
        