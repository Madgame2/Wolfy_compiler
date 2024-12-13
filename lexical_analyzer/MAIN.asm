.386					
.MODEL FLAT, STDCALL	
includelib kernel32.lib
includelib msvcrt.lib
includelib ucrt.lib
includelib vcruntime.lib
includelib msvcprt.lib
includelib WolfyConsoleLib.lib
includelib Wolfy_standart_lib.lib
				

print_massage0 PROTO 


extern is_equal0:proc
extern save_is_equal0:proc
ExitProcess PROTO :DWORD
print_string PROTO: DWORD
print_int PROTO: DWORD
print_newline PROTO

.stack 4096				
			

.CONST
L0 db "Hello wolrd", 0
L1 db "how working if block :", 0
L2 db "hello wolrd", 0
L3 db "hello", 0
L4 db "hello", 0


.DATA
MAINa DWORD 0
print_massage0str DWORD 0
MAINb DWORD 0
					

.CODE					

print_massage0 PROC

    push ebp
    mov ebp, esp

    mov eax, [ebp+8]
mov print_massage0str, eax


    
push print_massage0str

call print_string

call print_newline
mov eax, 0
mov print_massage0str, 0


mov esp, ebp
pop ebp
ret 4



print_massage0 ENDP
					

main PROC

push offset L0

call print_string

call print_newline

push 12 

call print_int

call print_newline
while_start0:

push 10 

push MAINa

pop eax
pop ebx
cmp eax, ebx
jg while_end0


push MAINa

call print_int

call print_newline

push 1 

push MAINa

pop eax
pop ebx
add eax, ebx
 
push eax

pop MAINa


jmp while_start0

while_end0:

mov MAINa, 0  

push offset L1

call print_string

call print_newline
while_start1:

push 10 

push MAINa

pop eax
pop ebx
cmp eax, ebx
jg while_end1


push 0 

push 2 

push MAINa

pop eax
pop ebx
mov ecx, ebx
cdq 
idiv ecx
mov eax, edx
 
push eax

pop eax
pop ebx
cmp eax, ebx
jne skip0


push MAINa

call print_int

call print_newline


skip0:



push 1 

push MAINa

pop eax
pop ebx
add eax, ebx
 
push eax

pop MAINa


jmp while_start1

while_end1:

push offset L2

call print_massage0

push offset L4

push offset L3

call is_equal0

mov MAINb, eax 

push MAINb

call print_int


	push 0
	call ExitProcess

	ret
main ENDP

END main

