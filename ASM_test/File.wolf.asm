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

extern is_equal0:proc
extern factorial0: proc
extern save_is_equal0:proc
ExitProcess PROTO :DWORD
print_string PROTO: DWORD
print_int PROTO: DWORD
print_newline PROTO

.stack 4096				
			

.CONST


.DATA
foo0result DWORD 0
					

.CODE					

foo0 PROC

    push ebp
    mov ebp, esp

    

    
push 10 

call factorial0

mov foo0result, eax 

push foo0result

call print_int
mov eax, 25 
mov foo0result, 0


mov esp, ebp
pop ebp
ret 0



foo0 ENDP
					


END