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

LO db "hello wrold", 0


.DATA
MAINstr DWORD offset LO
MAINstr1 DWORD offset MAINstr
					




.CODE					

					




main PROC

push MAINstr1

call print_string


	push 0
	call ExitProcess

	ret
main ENDP

END main

