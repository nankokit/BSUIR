.model small
.stack 100h

.data
Endl db 0Dh,0Ah,"$" 
Error_Write db "Write error!",0Dh,0Ah,'$'
Error_Read db  "Read error!",0Dh,0Ah,'$'

mas db 0 
           
.code

jmp start

Input proc
    mov ah, 01h
    int 21h
    mov mas, al
    
    mov ah, 09h
    lea dx, Endl
    int 21h
    ret
Input endp

Init_COM1 proc
   xor ax,ax
   mov al,10100011b ; 101 - скорость com порта 2400, 00 - проверка паритета: отсутствует, 11 - длина стопового бита: 2, 11 - кол-во бит в символе: 8
   mov dx,0h ; в dx помещаем номер нашего com порта, то есть первый com порт
   int 14h ; BIOS прерывание
   ret            
Init_COM1 endp

IsWrite_COM1 proc
   mov al, mas ; в младщий байт помещаем наш символ 
   mov ah,1h ; старший - помещаем нашу команду, то 01h - команда записи в порт 
   mov dx,0h
   int 14h
   test al,80h ; в случае ошибки в старший бит старшего байта помещается 1, что равняется 128 10000000b, что соответствует ошибке записи
   jnz NoWRite ; сравнивается по флагу CF ( по умолчанию 0, если ошибка, то 1 ) 
   ret 
IsWrite_COM1 endp

NoWRite proc
   mov ah,9h
   lea dx, Error_Write
   add dx,2h
   int 21h
   ret 
NoWRite endp

IsRead_COM2 proc
    mov ah,2h ; 02р - команда чтения из com порта
    mov dx,1h ; берем уже второй com порт 
    int 14h
    test al,80h
    jnz NoRead
    ret
IsRead_COM2 endp

NoRead proc
   mov ah,9h
   lea dx, Error_Read
   add dx,2h
   int 21h
   ret 
NoRead endp

Output proc
   mov ah,02h
   mov dl,al
   int 21h
   ret
Output endp

Exit proc
    mov ax,4C00h
    int 21h
    ret
Exit endp

error:

    call Exit

start:
   mov ax, @data
   mov ds, ax
   
   call Input
  
   call Init_COM1
   call IsWrite_COM1
   call IsRead_COM2
       
   call Output
   call Exit

end start
