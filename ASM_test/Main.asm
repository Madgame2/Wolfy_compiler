.386					
.MODEL FLAT, STDCALL	
includelib kernel32.lib
includelib msvcrt.lib
includelib ucrt.lib
includelib vcruntime.lib
includelib msvcprt.lib
includelib WolfyConsoleLib.lib
				

foo0 PROTO 

ExitProcess PROTO :DWORD
print_string PROTO: DWORD
print_int PROTO: DWORD
print_newline PROTO

.stack 4096				
			

.CONST


.DATA
fooa DWORD 0
					

.CODE					

foo0 PROC

    push ebp
    mov ebp, esp

    mov eax, [ebp+8]
mov fooa, eax


    
push fooa

call print_int
mov eax, 0
mov fooa, 0


mov esp, ebp
pop ebp
ret 4



foo0 ENDP
					

main PROC

push 25 

call foo0


	push 0
	call ExitProcess

	ret
main ENDP

END main

