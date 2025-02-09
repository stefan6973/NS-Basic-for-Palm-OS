import os
from x2pas import VBConverter



class VBModule(VBConverter):    
    def __init__(self, path):
        super().__init__(path)

    def convert(self, dst_folder: str):
        self.load(dst_folder)
        
        pasFile = bas_template
        pUnit = {
            'interface': {
                'uses': [],
                'type': [],
                'const': [],
                'var': [],
                'code': [],
                'unknown': [],
                },
            'implementation': {
                'uses': [],
                'type': [],
                'const': [],
                'var': [],
                'code': [],
                'unknown': [],
                },
            'initialization': [],
            'finalization': []
        }

        pComments = []
        lastTarget = None
        nextTarget = None

        def appendComments(dest: str = ''):
            if len(pComments) > 0:
                s = '// ' + '\n// '.join(pComments)
                appendContent(s, dest)
                pComments.clear()
        
        def appendBegin(dest: str = ''):
            appendContent('begin //#' + nextTarget['line'], dest)
        
        def appendEnd(dest: str = ''):
            appendContent('end; //#' + nextTarget['line'], dest)

        def appendContent(s, dest: str = ''):
            if s != '':
                if dest == '':
                    dest = nextTarget['kind']
                pUnit[nextTarget['scope']][dest].append(s)

        try:
            for currentLine in self.lines:
                # get line
                currentLine = currentLine.strip()
                
                # save last target
                lastTarget = nextTarget
                
                # determine next target
                nextTarget = self.determineTargetType(currentLine)

                # skip unknown targets
                if nextTarget is None:
                    continue
                
                # skip comments
                if nextTarget['kind'] == 'comment':
                    pComments.append(currentLine)
                    continue

                # continue with last target?
                if nextTarget['scope'] == 'unknown':
                    if nextTarget['kind'] != 'unknown':
                        nextTarget['scope'] = lastTarget['scope']
                    else: # continue with last target
                        nextTarget['scope'] = lastTarget['scope']
                        nextTarget['kind'] = lastTarget['kind']

                
                # skip unknown targets
                if nextTarget['kind'] == 'unknown':
                    continue

                # tokenize
                words = [x for x in currentLine.lower().split(' ') if x.strip() != '']

                # handle targets
                #--------------------------------------------------------------
                if nextTarget['scope'] == 'unit':
                #--------------------------------------------------------------
                    if nextTarget['kind'] == 'attribute' and nextTarget['name'] == 'vb_name':
                        pasFile = pasFile.replace('@@NAME@@', nextTarget['value'])
                    continue

                #--------------------------------------------------------------
                if nextTarget['scope'] in ('interface', 'implementation') :
                #--------------------------------------------------------------
                    match nextTarget['kind']:
                        case 'type':
                            appendComments()
                            continue

                        case 'const':
                            appendComments()
                            appendContent(f'{nextTarget['name']} = {nextTarget['value']};')
                            continue

                        case 'var':
                            appendComments()
                            if nextTarget['value'] == '':
                                appendContent(f'{nextTarget['name']}: {nextTarget['type']};')
                            else:
                                appendContent(f'{nextTarget['name']}: {nextTarget['type']} = {nextTarget['value']};')
                            continue

                        case 'function':
                            if nextTarget['name'] != '':
                                appendComments('code')
                                s = f'function {nextTarget['name']}(): {nextTarget['type']};'
                                appendContent('//# ' + currentLine, 'code')
                                appendContent(s, 'code')
                                appendBegin('code')
                            elif currentLine.endswith('end function'): appendEnd('code')
                            elif currentLine.endswith('end property'): appendEnd('code')
                            elif nextTarget['value'] == '':
                                s = ':='.join(currentLine.split('='))
                                appendContent('//# ' + currentLine, 'code')
                                appendContent(s, 'code')
                                appendContent('end;', 'code')
                            continue
                        
                        case _:
                            pass

                else:
                    print('unknown scope')
        except:
            pass
        finally:
            try:
                s = ''
                if len(pUnit['interface']['uses']) > 0:
                    pUnit['interface']['uses'].insert(0, 'uses')
                    s += '\n'.join(pUnit['interface']['uses'])
                if len(pUnit['interface']['type']) > 0:
                    pUnit['interface']['type'].insert(0, 'type')
                    s += '\n'.join(pUnit['interface']['type'])
                if len(pUnit['interface']['const']) > 0:
                    pUnit['interface']['const'].insert(0, 'const')
                    s += '\n'.join(pUnit['interface']['const'])
                if len(pUnit['interface']['var']) > 0:  
                    pUnit['interface']['var'].insert(0, 'var')
                    s += '\n'.join(pUnit['interface']['var'])
                if len(pUnit['interface']['code']) > 0:
                    pUnit['interface']['code'].insert(0, '// code')
                    s += '\n'.join(pUnit['interface']['code'])
                pasFile = pasFile.replace('@@INTERFACE@@', s)

                s = ''
                if len(pUnit['implementation']['uses']) > 0:
                    pUnit['implementation']['uses'].insert(0, 'uses')
                    s += '\n'.join(pUnit['implementation']['uses'])
                if len(pUnit['implementation']['type']) > 0:
                    pUnit['implementation']['type'].insert(0, 'type')
                    s += '\n'.join(pUnit['implementation']['type'])
                if len(pUnit['implementation']['const']) > 0:
                    pUnit['implementation']['const'].insert(0, 'const')
                    s += '\n'.join(pUnit['implementation']['const'])
                if len(pUnit['implementation']['var']) > 0:
                    pUnit['implementation']['var'].insert(0, 'var')
                    s += '\n'.join(pUnit['implementation']['var'])
                if len(pUnit['implementation']['code']) > 0:
                    pUnit['implementation']['code'].insert(0, '// code')
                    s += '\n'.join(pUnit['implementation']['code'])
                pasFile = pasFile.replace('@@IMPLEMENTATION@@', s)

                s = ''
                if len(pUnit['initialization']) > 0:
                    pUnit['initialization'].insert(0, 'initialization')
                    s += '\n'.join(pUnit['initialization'])
                pasFile = pasFile.replace('@@INITIALIZATION@@', s)

                s = ''
                if len(pUnit['finalization']) > 0:
                    pUnit['finalization'].insert(0, 'finalization')
                    s += '\n'.join(pUnit['finalization'])
                pasFile = pasFile.replace('@@FINALIZATION@@', s)

                with open(self.dest, 'w') as f:
                    f.write(pasFile)
            except Exception as ex:
                print(f'error writing file> {ex}')
        pass

bas_template = '''
unit @@NAME@@;

{$mode ObjFPC}{$H+}

interface
@@INTERFACE@@

uses
   Classes, SysUtils;

implementation
@@IMPLEMENTATION@@

@@INITIALIZATION@@

@@FINALIZATION@@

end.
'''