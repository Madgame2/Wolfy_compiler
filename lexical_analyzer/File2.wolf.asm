.386					
.MODEL FLAT, STDCALL	
includelib kernel32.lib
includelib msvcrt.lib
includelib ucrt.lib
includelib vcruntime.lib
includelib msvcprt.lib
includelib WolfyConsoleLib.lib
				

extern  min0
min0 PROTO 

ExitProcess PROTO :DWORD
print_string PROTO: DWORD
print_int PROTO: DWORD
print_newline PROTO

.stack 4096				
			

.CONST


.DATA
min0a DWORD 0
min0b DWORD 0
					

.CODE					

min0 PROC

    push ebp
    mov ebp, esp

    mov eax, [ebp+8]
mov min0a, eax
mov eax, [ebp+12]
mov min0b, eax


    mov eax, min0a
mov mina, 0
mov minb, 0


mov esp, ebp
pop ebp
ret 8



min0 ENDP
					


