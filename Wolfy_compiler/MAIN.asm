.386					
.MODEL FLAT, STDCALL	
includelib kernel32.lib
includelib msvcrt.lib
includelib ucrt.lib
includelib vcruntime.lib
includelib msvcprt.lib
includelib WolfyConsoleLib.lib
includelib Wolfy_standart_lib.lib
				

foo0 PROTO 


;extern factorial0:proc
;extern is_equal0:proc
;extern save_is_equal0:proc
is_equal0 PROTO: DWORD, :DWORD
save_is_equal0 PROTO: DWORD, :DWORD, :DWORD
factorial0 PROTO: DWORD
ExitProcess PROTO :DWORD
print_string PROTO: DWORD
print_int PROTO: DWORD
print_short PROTO: WORD
print_newline PROTO

.stack 4096				
			

.CONST
L0 db "Hello wolrd", 0
L1 db "how working if block :", 0
L2 db "hello wolrd", 0
L3 db "hello", 0
L4 db "hello", 0
L5 db "hi", 0


.DATA
MAIN0a DWORD 0
print_massage0str DWORD 0
MAINb DWORD 0
MAINnew WORD 0
					

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

push offset L1

call print_string

call print_newline
while_start0:

push 10 

push MAIN0a

pop eax
pop ebx
cmp eax, ebx
jg while_end0


push 0 

push 2 

push MAIN0a

pop eax
pop ebx
mov ecx, ebx
cdq 
idiv ecx
mov eax, edx
 
push eax

push 0

pop eax
pop ebx
cmp eax, ebx
jne skip0


push MAIN0a

call print_int

call print_newline


skip0:



push 1 

push MAIN0a

pop eax
pop ebx
add eax, ebx
 
push eax

pop MAIN0a


jmp while_start0

while_end0:

push offset L2

call print_massage0

push offset L4

push offset L3

call is_equal0

mov MAINb, eax 

push MAINb

call print_int

push 2 

push 2 

pop eax
pop ebx
add eax, ebx
 
push eax

push 0

pop eax
pop ebx
cmp eax, ebx
je skip2


push offset L5

call print_string

call print_newline


skip2:



call foo0

mov MAINnew, ax 

push MAINnew

call print_short


	push 0
	call ExitProcess

	ret
main ENDP

END main



END
