                section         .text

                global          _start
_start:
                sub             rsp, 5 * 129 * 8
                lea             rdi, [rsp + 4 * 129 * 8]
                mov             rcx, 128
                call            read_long
                lea             rdi, [rsp + 3 * 129 * 8]
                call            read_long
                lea             rsi, [rsp + 4 * 129 * 8]
                lea             rbx, [rsp + 3 * 129 * 8]
                lea             r8, [rsp + 2 * 129 * 8]
                mov             rdi, rsp
                mov				rcx, 129
                call            mul_long_long
                mov             rcx, 2 * 129
                call            write_long

                mov             al, 0x0a
                call            write_char

                jmp             exit

; adds two long number
;    rdi -- address of summand #1 (long number)
;    r8 -- address of summand #2 (long number)
;    rcx -- length of long numbers in qwords
; result:
;    sum is written to rdi
my_add_long_long:
                push            rdi
                push            r8
                push            rcx

                clc
.loop:
                mov             rax, [r8]
                lea             r8, [r8 + 8]
                adc             [rdi], rax
                lea             rdi, [rdi + 8]
                dec             rcx
                jnz             .loop

                pop             rcx
                pop             r8
                pop             rdi
                ret

; multiplies long number by a short
;    rsi -- address of multiplier #1 (long number)
;    [rbx] -- multiplier #2 (64-bit unsigned)
;    rcx -- length of long number in qwords
; result:
;    product is written to r8
my_mul_long_short:
                push            rax
                push            rsi
                push            rcx
                push            r8

                xor             r9, r9
                mov             r10, [rbx]
.loop:
                mov             rax, [rsi]
                mul             r10
                add             rax, r9
                adc             rdx, 0
                mov             [r8], rax
                add             rsi, 8
                add             r8, 8
                mov             r9, rdx
                dec             rcx
                jnz             .loop

                pop             r8
                pop             rcx
                pop             rsi
                pop             rax
                ret

; muls two long number
;    rbx -- address of summand #1 (long number)
;    rsi -- address of summand #2 (long number)
;    rcx -- length of long numbers in qwords
;    r8 --  store intermediate value
; result:
;    multiplication is written to rdi

mul_long_long:

                push            rbx
                push            rcx
                push            rdi

                mov             r11, rcx          
.loop:
                push            rcx
                mov             rcx, r11
                call            my_mul_long_short
                call            my_add_long_long
                add             rbx, 8
                add             rdi, 8
                pop             rcx
                dec             rcx
                jnz             .loop

                pop             rdi
                pop             rcx
                pop             rbx

                ret



; divides long number by a short
;    rdi -- address of dividend (long number)
;    rbx -- divisor (64-bit unsigned)
;    rcx -- length of long number in qwords
; result:
;    quotient is written to rdi
;    rdx -- remainder
div_long_short:
                push            rdi
                push            rax
                push            rcx

                lea             rdi, [rdi + 8 * rcx - 8]
                xor             rdx, rdx

.loop:
                mov             rax, [rdi]
                div             rbx
                mov             [rdi], rax
                sub             rdi, 8
                dec             rcx
                jnz             .loop

                pop             rcx
                pop             rax
                pop             rdi
                ret

; assigns a zero to long number
;    rdi -- argument (long number)
;    rcx -- length of long number in qwords
set_zero:
                push            rax
                push            rdi
                push            rcx

                xor             rax, rax
                rep stosq

                pop             rcx
                pop             rdi
                pop             rax
                ret

; checks if a long number is a zero
;    rdi -- argument (long number)
;    rcx -- length of long number in qwords
; result:
;    ZF=1 if zero
is_zero:
                push            rax
                push            rdi
                push            rcx

                xor             rax, rax
                rep scasq

                pop             rcx
                pop             rdi
                pop             rax
                ret


add_digit:
                push            rax
                push            rdi
                push            rcx
                push            rsi

                xor             rsi, rsi
.loop:
                mov             rax, [rdi]
                mul             rbx
                add             rax, rsi
                adc             rdx, 0
                mov             [rdi], rax
                add             rdi, 8
                mov             rsi, rdx
                dec             rcx
                jnz             .loop

                pop             rsi
                pop             rcx
                pop             rdi
                pop             rax

                push            rdi
                push            rcx
                push            rdx

                xor             rdx,rdx
.loop2:
                add             [rdi], rax
                adc             rdx, 0
                mov             rax, rdx
                xor             rdx, rdx
                add             rdi, 8
                dec             rcx
                jnz             .loop2

                pop             rdx
                pop             rcx
                pop             rdi
                ret


; read long number from stdin
;    rdi -- location for output (long number)
;    rcx -- length of long number in qwords
read_long:
                push            rcx
                push            rdi

                call            set_zero
.loop:
                call            read_char
                or              rax, rax
                js              exit
                cmp             rax, 0x0a
                je              .done
                cmp             rax, '0'
                jb              .invalid_char
                cmp             rax, '9'
                ja              .invalid_char

                sub             rax, '0'
                mov             rbx, 10
                call            add_digit
                jmp             .loop

.done:
                pop             rdi
                pop             rcx
                ret

.invalid_char:
                mov             rsi, invalid_char_msg
                mov             rdx, invalid_char_msg_size
                call            print_string
                call            write_char
                mov             al, 0x0a
                call            write_char

.skip_loop:
                call            read_char
                or              rax, rax
                js              exit
                cmp             rax, 0x0a
                je              exit
                jmp             .skip_loop

; write long number to stdout
;    rdi -- argument (long number)
;    rcx -- length of long number in qwords
write_long:
                push            rax
                push            rcx

                mov             rax, 20
                mul             rcx
                mov             rbp, rsp
                sub             rsp, rax

                mov             rsi, rbp

.loop:
                mov             rbx, 10
                call            div_long_short
                add             rdx, '0'
                dec             rsi
                mov             [rsi], dl
                call            is_zero
                jnz             .loop

                mov             rdx, rbp
                sub             rdx, rsi
                call            print_string

                mov             rsp, rbp
                pop             rcx
                pop             rax
                ret

; read one char from stdin
; result:
;    rax == -1 if error occurs
;    rax \in [0; 255] if OK
read_char:
                push            rcx
                push            rdi

                sub             rsp, 1
                xor             rax, rax
                xor             rdi, rdi
                mov             rsi, rsp
                mov             rdx, 1
                syscall

                cmp             rax, 1
                jne             .error
                xor             rax, rax
                mov             al, [rsp]
                add             rsp, 1

                pop             rdi
                pop             rcx
                ret
.error:
                mov             rax, -1
                add             rsp, 1
                pop             rdi
                pop             rcx
                ret

; write one char to stdout, errors are ignored
;    al -- char
write_char:
                sub             rsp, 1
                mov             [rsp], al

                mov             rax, 1
                mov             rdi, 1
                mov             rsi, rsp
                mov             rdx, 1
                syscall
                add             rsp, 1
                ret

exit:
                mov             rax, 60
                xor             rdi, rdi
                syscall

; print string to stdout
;    rsi -- string
;    rdx -- size
print_string:
                push            rax

                mov             rax, 1
                mov             rdi, 1
                syscall

                pop             rax
                ret


                section         .rodata
invalid_char_msg:
                db              "Invalid character: "
invalid_char_msg_size: equ             $ - invalid_char_msg
