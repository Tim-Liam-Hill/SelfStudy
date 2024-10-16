

print_string:
pusha 
mov ah , 0x0e 
start:
mov al , [bx]
cmp al, 0
je end
int 0x10 
add bx, 0x1
jmp start
end:
popa 
ret 