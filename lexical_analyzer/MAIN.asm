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
print_newline PROTO

.stack 4096				
			

.CONST


.DATA
MAINb WORD 0
					

.CODE					

					

main PROC

push 2 

push 2 

pop eax
pop ebx
sub eax, edx
 
push ax

pop MAINb


	push 0
	call ExitProcess

	ret
main ENDP

END main

