.386					
.MODEL FLAT, STDCALL	
includelib kernel32.lib
includelib msvcrt.lib
includelib ucrt.lib
includelib vcruntime.lib
includelib msvcprt.lib
includelib WolfyConsoleLib.lib
				


ExitProcess PROTO :DWORD
print_string PROTO: DWORD
print_int PROTO: DWORD
endl PROTO

.stack 4096				
			


.DATA
MAINa DWORD 0
					




.CODE					

					

main PROC

xor eax, eax
mov eax, 1

while_start0:

push 10
push MAINa

pop eax
pop ebx
cmp eax, ebx
jl while_end0


push MAINa

call print_int

call endl

push 1

pop eax
pop ebx
add eax, ebx
 
push eax

pop MAINa


jmp while_start0

while_end0:


	push 0
	call ExitProcess

	ret
main ENDP

END main

