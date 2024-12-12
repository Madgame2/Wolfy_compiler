.386					
.MODEL FLAT, STDCALL	
includelib kernel32.lib
includelib msvcrt.lib
includelib ucrt.lib
includelib vcruntime.lib
includelib msvcprt.lib
includelib WolfyConsoleLib.lib
				

foo0 PROTO 
foo1 PROTO 

ExitProcess PROTO :DWORD
print_string PROTO: DWORD
print_int PROTO: DWORD
print_newline PROTO

.stack 4096				
			

.CONST
L0 db "hello", 0


.DATA
fooa DWORD 0
					

.CODE					

foo0 PROC uses eax ebx ecx edi esi

    push ebp
    mov ebp, esp

    

    mov eax, 1 


mov esp, ebp
pop ebp
ret 0



foo0 ENDP
foo1 PROC uses eax ebx ecx edi esi

    push ebp
    mov ebp, esp

    mov eax, [ebp+4]
mov fooa, eax


    mov eax, 2 
mov fooa, 0


mov esp, ebp
pop ebp
ret 4



foo1 ENDP
					

main PROC

call foo0

push 1 

call foo1

push offset L0

call print_string


	push 0
	call ExitProcess

	ret
main ENDP

END main

