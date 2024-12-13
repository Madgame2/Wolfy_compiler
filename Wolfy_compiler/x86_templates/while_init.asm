while_start<num>:
<expresion>
pop eax
pop ebx
cmp eax, ebx
<flag> while_end<num>

<block>

jmp while_start<num>

while_end<num>: