Complex::Complex(double, double):
        push    rbp
        mov     rbp, rsp
        mov     QWORD PTR [rbp-8], rdi
        movsd   QWORD PTR [rbp-16], xmm0
        movsd   QWORD PTR [rbp-24], xmm1
        mov     rax, QWORD PTR [rbp-8]
        movsd   xmm0, QWORD PTR [rbp-16]
        movsd   QWORD PTR [rax], xmm0
        mov     rax, QWORD PTR [rbp-8]
        movsd   xmm0, QWORD PTR [rbp-24]
        movsd   QWORD PTR [rax+8], xmm0
        nop
        pop     rbp
        ret
Complex::Complex(double*):
        push    rbp
        mov     rbp, rsp
        mov     QWORD PTR [rbp-8], rdi
        mov     QWORD PTR [rbp-16], rsi
        mov     rax, QWORD PTR [rbp-16]
        movsd   xmm0, QWORD PTR [rax]
        mov     rax, QWORD PTR [rbp-8]
        movsd   QWORD PTR [rax], xmm0
        mov     rax, QWORD PTR [rbp-16]
        movsd   xmm0, QWORD PTR [rax+8]
        mov     rax, QWORD PTR [rbp-8]
        movsd   QWORD PTR [rax+8], xmm0
        nop
        pop     rbp
        ret
operator*(Complex const&, Complex const&):
        push    rbp
        mov     rbp, rsp
        push    rbx
        sub     rsp, 40
        mov     QWORD PTR [rbp-40], rdi
        mov     QWORD PTR [rbp-48], rsi
        mov     rax, QWORD PTR [rbp-40]
        movsd   xmm1, QWORD PTR [rax]
        mov     rax, QWORD PTR [rbp-48]
        movsd   xmm0, QWORD PTR [rax]
        mulsd   xmm0, xmm1
        mov     rax, QWORD PTR [rbp-40]
        movsd   xmm2, QWORD PTR [rax+8]
        mov     rax, QWORD PTR [rbp-48]
        movsd   xmm1, QWORD PTR [rax+8]
        mulsd   xmm1, xmm2
        lea     rax, [rbp-32]
        mov     rdi, rax
        call    Complex::Complex(double, double)
        mov     rax, QWORD PTR [rbp-32]
        mov     rdx, QWORD PTR [rbp-24]
        mov     rcx, rax
        mov     rbx, rdx
        movq    xmm0, rcx
        movq    xmm1, rdx
        add     rsp, 40
        pop     rbx
        pop     rbp
        ret
cubeme(Complex const&):
        push    rbp
        mov     rbp, rsp
        push    rbx
        sub     rsp, 40
        mov     QWORD PTR [rbp-40], rdi
        mov     rdx, QWORD PTR [rbp-40]
        mov     rax, QWORD PTR [rbp-40]
        mov     rsi, rdx
        mov     rdi, rax
        call    operator*(Complex const&, Complex const&)
        movq    rax, xmm0
        movapd  xmm0, xmm1
        mov     QWORD PTR [rbp-32], rax
        movsd   QWORD PTR [rbp-24], xmm0
        mov     rdx, QWORD PTR [rbp-40]
        lea     rax, [rbp-32]
        mov     rsi, rdx
        mov     rdi, rax
        call    operator*(Complex const&, Complex const&)
        mov     eax, 0
        mov     edx, 0
        movq    rax, xmm0
        movq    rdx, xmm1
        mov     rcx, rax
        mov     rbx, rdx
        movq    xmm0, rcx
        movq    xmm1, rdx
        add     rsp, 40
        pop     rbx
        pop     rbp
        ret
sqrt(Complex const&):
        push    rbp
        mov     rbp, rsp
        push    rbx
        sub     rsp, 88
        mov     QWORD PTR [rbp-88], rdi
        pxor    xmm0, xmm0
        movsd   QWORD PTR [rbp-80], xmm0
        pxor    xmm0, xmm0
        movsd   QWORD PTR [rbp-72], xmm0
        mov     rax, QWORD PTR [rbp-88]
        movsd   xmm0, QWORD PTR [rax+8]
        pxor    xmm1, xmm1
        ucomisd xmm0, xmm1
        jp      .L8
        pxor    xmm1, xmm1
        ucomisd xmm0, xmm1
        jne     .L8
        mov     rax, QWORD PTR [rbp-88]
        movsd   xmm1, QWORD PTR [rax]
        pxor    xmm0, xmm0
        comisd  xmm0, xmm1
        jbe     .L8
        lea     rdx, [rbp-80]
        lea     rax, [rbp-64]
        mov     rsi, rdx
        mov     rdi, rax
        call    Complex::Complex(double*)
        mov     rax, QWORD PTR [rbp-64]
        mov     rdx, QWORD PTR [rbp-56]
        jmp     .L14
.L8:
        mov     rax, QWORD PTR [rbp-88]
        movsd   xmm1, QWORD PTR [rax]
        mov     rax, QWORD PTR [rbp-88]
        movsd   xmm0, QWORD PTR [rax]
        mulsd   xmm1, xmm0
        mov     rax, QWORD PTR [rbp-88]
        movsd   xmm2, QWORD PTR [rax+8]
        mov     rax, QWORD PTR [rbp-88]
        movsd   xmm0, QWORD PTR [rax+8]
        mulsd   xmm0, xmm2
        addsd   xmm0, xmm1
        call    sqrt
        movq    rax, xmm0
        mov     QWORD PTR [rbp-24], rax
        pxor    xmm0, xmm0
        ucomisd xmm0, QWORD PTR [rbp-24]
        jp      .L17
        pxor    xmm0, xmm0
        ucomisd xmm0, QWORD PTR [rbp-24]
        je      .L12
.L17:
        mov     rax, QWORD PTR [rbp-88]
        movsd   xmm0, QWORD PTR [rax+8]
        divsd   xmm0, QWORD PTR [rbp-24]
        call    asin
        movq    rax, xmm0
        mov     QWORD PTR [rbp-32], rax
        mov     rax, QWORD PTR [rbp-24]
        movq    xmm0, rax
        call    sqrt
        movsd   QWORD PTR [rbp-96], xmm0
        movsd   xmm0, QWORD PTR [rbp-32]
        movsd   xmm1, QWORD PTR .LC1[rip]
        divsd   xmm0, xmm1
        call    cos
        mulsd   xmm0, QWORD PTR [rbp-96]
        movsd   QWORD PTR [rbp-80], xmm0
        mov     rax, QWORD PTR [rbp-24]
        movq    xmm0, rax
        call    sqrt
        movsd   QWORD PTR [rbp-96], xmm0
        movsd   xmm0, QWORD PTR [rbp-32]
        movsd   xmm1, QWORD PTR .LC1[rip]
        divsd   xmm0, xmm1
        call    sin
        mulsd   xmm0, QWORD PTR [rbp-96]
        movsd   QWORD PTR [rbp-72], xmm0
.L12:
        lea     rdx, [rbp-80]
        lea     rax, [rbp-48]
        mov     rsi, rdx
        mov     rdi, rax
        call    Complex::Complex(double*)
        mov     rax, QWORD PTR [rbp-48]
        mov     rdx, QWORD PTR [rbp-40]
.L14:
        mov     rcx, rax
        mov     rbx, rdx
        movq    xmm0, rcx
        movq    xmm1, rdx
        add     rsp, 88
        pop     rbx
        pop     rbp
        ret
stupidmacro():
        push    rbp
        mov     rbp, rsp
        sub     rsp, 80
        movsd   xmm0, QWORD PTR .LC2[rip]
        movsd   QWORD PTR [rbp-80], xmm0
        movsd   xmm0, QWORD PTR .LC3[rip]
        movsd   QWORD PTR [rbp-72], xmm0
        lea     rax, [rbp-80]
        mov     rdi, rax
        call    sqrt(Complex const&)
        movq    rax, xmm0
        movapd  xmm0, xmm1
        mov     QWORD PTR [rbp-64], rax
        movsd   QWORD PTR [rbp-56], xmm0
        lea     rax, [rbp-80]
        mov     rdi, rax
        call    sqrt(Complex const&)
        movq    rax, xmm0
        movapd  xmm0, xmm1
        mov     QWORD PTR [rbp-32], rax
        movsd   QWORD PTR [rbp-24], xmm0
        lea     rax, [rbp-80]
        mov     rdi, rax
        call    sqrt(Complex const&)
        movq    rax, xmm0
        movapd  xmm0, xmm1
        mov     QWORD PTR [rbp-16], rax
        movsd   QWORD PTR [rbp-8], xmm0
        lea     rdx, [rbp-32]
        lea     rax, [rbp-16]
        mov     rsi, rdx
        mov     rdi, rax
        call    operator*(Complex const&, Complex const&)
        movq    rax, xmm0
        movapd  xmm0, xmm1
        mov     QWORD PTR [rbp-48], rax
        movsd   QWORD PTR [rbp-40], xmm0
        lea     rdx, [rbp-64]
        lea     rax, [rbp-48]
        mov     rsi, rdx
        mov     rdi, rax
        call    operator*(Complex const&, Complex const&)
        mov     eax, 0
        mov     edx, 0
        movq    rax, xmm0
        movq    rdx, xmm1
        nop
        leave
        ret
constexprversion():
        push    rbp
        mov     rbp, rsp
        sub     rsp, 32
        movsd   xmm0, QWORD PTR .LC2[rip]
        movsd   QWORD PTR [rbp-32], xmm0
        movsd   xmm0, QWORD PTR .LC3[rip]
        movsd   QWORD PTR [rbp-24], xmm0
        lea     rax, [rbp-32]
        mov     rdi, rax
        call    sqrt(Complex const&)
        movq    rax, xmm0
        movapd  xmm0, xmm1
        mov     QWORD PTR [rbp-16], rax
        movsd   QWORD PTR [rbp-8], xmm0
        lea     rax, [rbp-16]
        mov     rdi, rax
        call    cubeme(Complex const&)
        mov     eax, 0
        mov     edx, 0
        movq    rax, xmm0
        movq    rdx, xmm1
        nop
        leave
        ret
__static_initialization_and_destruction_0(int, int):
        push    rbp
        mov     rbp, rsp
        sub     rsp, 16
        mov     DWORD PTR [rbp-4], edi
        mov     DWORD PTR [rbp-8], esi
        cmp     DWORD PTR [rbp-4], 1
        jne     .L22
        cmp     DWORD PTR [rbp-8], 65535
        jne     .L22
        mov     edi, OFFSET FLAT:_ZStL8__ioinit
        call    std::ios_base::Init::Init() [complete object constructor]
        mov     edx, OFFSET FLAT:__dso_handle
        mov     esi, OFFSET FLAT:_ZStL8__ioinit
        mov     edi, OFFSET FLAT:_ZNSt8ios_base4InitD1Ev
        call    __cxa_atexit
.L22:
        nop
        leave
        ret
_GLOBAL__sub_I_sqrt(Complex const&):
        push    rbp
        mov     rbp, rsp
        mov     esi, 65535
        mov     edi, 1
        call    __static_initialization_and_destruction_0(int, int)
        pop     rbp
        ret
.LC1:
        .long   0
        .long   1073741824
.LC2:
        .long   0
        .long   1077608448
.LC3:
        .long   0
        .long   1076559872
