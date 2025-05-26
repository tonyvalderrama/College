; Programa que ordena el teclado de la computadora de forma alfab‚tica.
; Para computadoras 8088, 80286 y 80386.
; Jos‚ Antonio Valderrama Ramos   -   90120509-9

.model small

.stack 100h

.data
   tecla  db ?
   menext db ' ** Puls¢ tecla extendida ** ','$'
   menent db 'Presione cualquier tecla, o ESC para salir.','$'

imptec macro tec
   mov ah,02h
   mov dl,tec   
   int 21h                ;Interrupci¢n que escribe una tecla a la pantalla.
   jmp ciclo                                         ;Haz el ciclo de nuevo.
   endm

.code

begin:                                           ;Inicio mi programa con ...
   mov ah,15                                    ;... un borrado de pantalla.
   int 10h
   mov ah,0
   int 10h

   mov ah,09h                                  ;Escribo un mensaje en la ...
   mov dx,seg menent                                          ;... pantalla.
   mov ds,dx
   mov dx,offset menent
   int 21h

ciclo:                ;Me meto a un ciclo que sigue hasta que apachurro ESC.
   mov ah,00h
   int 16h                           ;Interrupci¢n que lee tecla presionada.
   cmp al,27                             ;Si la tecla presionada es ESC, ...
   jne versiext
   mov ah,4Ch                                      ;... termina el programa.
   mov al,1
   int 21h
versiext:   
   mov tecla,al                            ;Meto lo que le¡ a las variables.
   cmp tecla,0                             ;Ver si es un caracter extendido.
   jne noext
   mov ah,09h                             ;Si es, imprimo el mensaje en ...
   mov dx,seg menext                                      ;... la pantalla.
   mov ds,dx
   mov dx,offset menext
   int 21h
noext:
   cmp tecla,116                                               ;'t' por 'e'.
   jne sig1
   imptec 101
sig1:
   cmp tecla,122                                               ;'z' por 't'.
   jne sig2
   imptec 116
sig2:
   cmp tecla,113                                               ;'q' por 'a'.
   jne sig3
   imptec 97
sig3:
   cmp tecla,103                                               ;'g' por 'o'.
   jne sig4
   imptec 111
sig4:
   cmp tecla,'f'                                               ;'f' por 'n'.
   jne sig5
   imptec 110
sig5:
   cmp tecla,107                                               ;'k' por 'r'.
   jne sig6
   imptec 114
sig6:
   cmp tecla,'o'                                               ;'o' por 'i'.
   jne sig6a
   imptec 'i'
sig6a:
   cmp tecla,108                                               ;'l' por 's'.
   jne sig7
   imptec 115
sig7:
   cmp tecla,105                                               ;'i' por 'h'.
   jne sig8
   imptec 104
sig8:
   cmp tecla,114                                               ;'r' por 'd'.
   jne sig9
   imptec 100
sig9:
   cmp tecla,115                                               ;'s' por 'l'.
   jne sig10
   imptec 108
sig10:
   cmp tecla,121                                               ;'y' por 'f'.
   jne sig11
   imptec 102
sig11:
   cmp tecla,101                                               ;'e' por 'c'.
   jne sig12
   imptec 99
sig12:
   cmp tecla,100                                               ;'d' por 'm'.
   jne sig13
   imptec 109
sig13:
   cmp tecla,120                                               ;'x' por 'u'.
   jne sig14
   imptec 117
sig14:
   cmp tecla,117                                               ;'u' por 'g'.
   jne sig15
   imptec 103
sig15:
   cmp tecla,110                                               ;'n' por 'y'.
   jne sig16
   imptec 121
sig16:
   cmp tecla,104                                               ;'h' por 'p'.
   jne sig17
   imptec 112
sig17:
   cmp tecla,118                                               ;'v' por 'w'.
   jne sig18
   imptec 119
sig18:
   cmp tecla,119                                               ;'w' por 'b'.
   jne sig19
   imptec 98
sig19:
   cmp tecla,99                                                ;'c' por 'v'.
   jne sig20
   imptec 118
sig20:
   cmp tecla,97                                                ;'a' por 'k'.
   jne sig21
   imptec 107
sig21:
   cmp tecla,98                                                ;'b' por 'x'.
   jne sig22
   imptec 120
sig22:
   cmp tecla,112                                               ;'p' por 'j'.
   jne sig23
   imptec 106
sig23:
   cmp tecla,106                                               ;'j' por 'q'.
   jne sig24
   imptec 113
sig24:
   cmp tecla,109                                               ;'m' por 'z'.
   jne sig25
   imptec 122
sig25:
   cmp tecla,84                                                ;'T' por 'E'.
   jne sig26
   imptec 69
sig26:
   cmp tecla,90                                                ;'Z' por 'T'.
   jne sig27
   imptec 84
sig27:
   cmp tecla,81                                                ;'Q' por 'A'.
   jne sig28
   imptec 65
sig28:
   cmp tecla,71                                                ;'G' por 'O'.
   jne sig29
   imptec 79
sig29:
   cmp tecla,'F'                                               ;'F' por 'N'.
   jne sig30
   imptec 78
sig30:
   cmp tecla,75                                                ;'K' por 'R'.
   jne sig31
   imptec 82
sig31:
   cmp tecla,'O'                                               ;'O' por 'I'.
   jne sig32a
   imptec 'I'
sig32a:
   cmp tecla,76                                                ;'L' por 'S'.
   jne sig32
   imptec 83
sig32:
   cmp tecla,73                                                ;'I' por 'H'.
   jne sig33
   imptec 72
sig33:
   cmp tecla,82                                                ;'R' por 'D'.
   jne sig34
   imptec 68
sig34:
   cmp tecla,83                                                ;'S' por 'L'.
   jne sig35
   imptec 76
sig35:
   cmp tecla,89                                                ;'Y' por 'F'.
   jne sig36
   imptec 70
sig36:
   cmp tecla,69                                                ;'E' por 'C'.
   jne sig37
   imptec 67
sig37:
   cmp tecla,68                                                ;'D' por 'M'.
   jne sig38
   imptec 77
sig38:
   cmp tecla,88                                                ;'X' por 'U'.
   jne sig39
   imptec 85
sig39:
   cmp tecla,85                                                ;'U' por 'G'.
   jne sig40
   imptec 71
sig40:
   cmp tecla,78                                                ;'N' por 'Y'.
   jne sig41
   imptec 89
sig41:
   cmp tecla,72                                                ;'H' por 'P'.
   jne sig42
   imptec 80
sig42:
   cmp tecla,86                                                ;'V' por 'W'.
   jne sig43
   imptec 87
sig43:
   cmp tecla,'W'                                               ;'W' por 'B'.
   jne sig44
   imptec 'B'
sig44:
   cmp tecla,67                                                ;'C' por 'V'.
   jne sig45
   imptec 86
sig45:
   cmp tecla,65                                                ;'A' por 'K'.
   jne sig46
   imptec 75
sig46:
   cmp tecla,66                                                ;'B' por 'X'.
   jne sig47
   imptec 88
sig47:
   cmp tecla,'P'                                               ;'P' por 'J'.
   jne sig48
   imptec 'J'
sig48:
   cmp tecla,74                                                ;'J' por 'Q'.
   jne sig49
   imptec 81
sig49:
   cmp tecla,77                                                ;'M' por 'Z'.
   jne sig50
   imptec 90
sig50:                                           ;No es caracter alfab‚tico.
   mov ah,02h
   mov dl,tecla
   int 21h
   jmp ciclo

   end

; NOTAS REFERENTES DEL PROGRAMA:
; * El orden en que se comparan las letras es debido a un estudio de la
;   lengua inglesa, donde las letras, ordenadas en funci¢n de la frecuencia
;   con que se usan, es as¡:  ETAONRISHDLFCMUGYPWBVKXJQZ.
; * Se dice que con las primeras 4 letras (ETAON) se forman el 45% de las
;   palabras en el ingl‚s, y con las primeras 9 (ETAONRISH) se forman el 70% 
;   de ellas.
; * Se quiso usar este orden para comparar las letras presionadas, para hacer
;   menos comparaciones para la mayor parte de las letras.
; * Se podr  preguntar, porque se us¢ la tabla de frecuencia de las letras
;   utilizadas en el ingl‚s, pero fue por dos motivos.  La primera es que no
;   cuento con la informaci¢n correspondiente para el lenguaje castellano; y
;   porque todo el software de hoy en d¡a es para los gringos.


;   Atte: El programador.
