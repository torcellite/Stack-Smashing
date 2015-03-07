import re
import sys

class GetShellcode(object):

    def __init__(self, dump):
        self.dump = dump

        self.entire_pattern = r'([a-f0-9]+\:\s*[a-f0-9\s]+\s*.+)'
        self.hex_code_pattern = r'(?:[a-f0-9]+\:\s*)([a-f0-9\s]+\s{1})(?:\s*)(.+)'

    def dump_shellcode(self):
        primary_matches = re.findall(self.entire_pattern, self.dump)
        for primary_match in primary_matches:
            m = re.match(self.hex_code_pattern, ''.join(primary_match), 0);
            if m:
                line = '"'
                hex_code = m.group(1)
                hex_code = hex_code[hex_code.find(':') + 1:]
                hex_code = hex_code.strip()
                hex_code = hex_code.replace(' ', '\\x')
                hex_code = '\\x' + hex_code
                line = line + hex_code + '"'
                line = line + '\t// '
                line = line + m.group(2)
                print line


def main():
    if len(sys.argv) < 2:
        print 'No input given.\n'
    else:
        getshellcode = GetShellcode(dump=sys.argv[1])
        getshellcode.dump_shellcode()

if __name__ == '__main__':
    main()