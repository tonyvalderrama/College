#define ADD 0                                              /* Aritm‚ticas */
#define SUB 2
#define MUL 4
#define DIV 6
#define ADDX 8
#define ADDY 10
#define SUBX 12
#define SUBY 14
#define MULX 16
#define MULY 18
#define DIVX 20
#define DIVY 22
#define INCX 24
#define INCY 26
#define DECX 28
#define DECY 30
#define INCA 32
#define DECA 34

#define AND 64                                                 /* L¢gicas */
#define ANDX 66
#define ANDY 68
#define OR 70
#define ORX 74
#define ORY 76
#define SHL 78
#define SHR 80
#define SHLC 84
#define SHRC 86
#define XOR 88
#define XORX 90
#define XORY 94
#define CS 96
#define CC 98
#define ZS 100
#define ZC 104
#define IS 106
#define IC 108
#define SS 110
#define SC 114
#define CMPA 116
#define CMPX 118
#define CMPY 120

#define J 128                                                     /*SALTOS*/
#define JZ 130
#define JL 132
#define JE 134
#define JG 136
#define JNL 138
#define JNE 140
#define JNG 142
#define JS 144
#define LOOP 146
#define INT 148
#define RET 150
#define RTI 152
#define PUSH 154
#define PUSHA 156
#define PUSHX 158
#define PUSHY 160
#define POP 162
#define POPA 164
#define POPX 166
#define POPY 168

#define LDA 194                                          /* TRANSFERENCIA */
#define LDX 196
#define LDY 198
#define LDPC 200
#define LDBP 202
#define LDSP 204
#define LDSW 206
#define STA 208
#define STX 210
#define STY 212
#define STPC 214
#define STBP 216
#define STSP 218
#define STSW 220
#define LDSI 222
#define STSI 224
#define LDAL 226
#define STAL 228
#define LDXL 230
#define STXL 232
#define LDYL 234
#define STYL 236

#define END 255

struct head
   {
   char H;
   unsigned int reg_t,size_text,size_sic,dir_in;
   char nombre_arch[13];
   };
struct code
   {
   char T;
   unsigned int dir,longi;
   };
struct fin
   {
   char E;
   unsigned int firstinst;
   };
