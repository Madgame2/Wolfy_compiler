.386					
.MODEL FLAT, STDCALL	
includelib kernel32.lib
includelib msvcrt.lib
includelib ucrt.lib
includelib vcruntime.lib
includelib msvcprt.lib
includelib WolfyConsoleLib.lib
includelib Wolfy_standart_lib.lib
<include>				

<function_proto>

is_equal PROTO: DWORD, : DWORD
save_is_equal PROTO: DWORD, :DWORD, :DWORD
ExitProcess PROTO :DWORD
print_string PROTO: DWORD
print_int PROTO: DWORD
print_newline PROTO

.stack 4096				
<stack>			

.CONST
<const>

.DATA
<data>					

.CODE					

<code>					

<main>