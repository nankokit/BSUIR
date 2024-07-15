.model small
.stack 100h

.data

ErrorWr db "Error of writing!",0Dh,0Ah,'$'
ErrorRd db "Error of reading!",0Dh,0Ah,'$'
Endl db 0Dh,0Ah,"$" 
mas db 0 
mas2 db ?           
.code

Input proc
    mov ah, 01h
    int 21h
    mov mas, al
    
    mov ah, 09h
    lea dx, Endl
    int 21h
    ret
Input endp
           
InitCOM1 proc
    mov al,80h
    mov dx,3FBh  
    out dx,al  
    mov dx,3F8h  
    mov al,00h
    out dx,al
    mov al,0Ch
    mov dx,3F9h 
    out dx,al
    
    mov dx,3FCh 
    mov al,00001011b
    out dx,al
    
    mov dx,3F9h 
    mov al,0
    out dx,al
    ret
InitCOM1 endp

IsWriteCOM1 proc
    xor al,al
    mov dx,3FDh 
    in al,dx
    test al,10h
    jnz NoWRite
    ret
IsWriteCOM1 endp

NoWRite proc
   mov ah,9
   lea dx, ErrorWr
   int 21h
   ret 
NoWRite endp

IsReadCOM2 proc
    xor al,al
    mov dx,3FDh 
    in al,dx
    test al,10b
    jnz NoRead
    ret
IsReadCOM2 endp

NoRead proc
   mov ah,9
   lea dx,ErrorRd
   int 21h
   ret 
NoRead endp

SendByte proc
    mov dx,3F8h
    mov al,mas
    out dx,al
    ret
SendByte endp

ReadByte proc
    mov dx,3F8h
    in al,dx
    mov mas2,al
    ret
ReadByte endp

Exit proc
    mov ax,4C00h
    int 21h
    ret
Exit endp

start:
    mov ax,@data
    mov ds,ax
    call Input
    call InitCOM1
    call IsWriteCOM1
    call SendByte
    mov al,2 
    call IsReadCOM2
    call ReadByte
    mov ah,02h
    mov dl,mas2
    int 21h 
    call exit
   
end start
