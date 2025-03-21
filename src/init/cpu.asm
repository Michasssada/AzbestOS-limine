section .text
    global setup_sse
    global setup_avx

; --------------------------------------------------------------------
; Function: setup_sse
; Checks if SSE is available and enables it by:
;   - Calling CPUID (rax = 1) and testing rdx bit 25.
;   - Clearing the EM bit (bit 2) in CR0.
;   - Setting OSFXSR (bit 9) and OSXMMEXCPT (bit 10) in CR4.
; If SSE is not supported, the function simply returns.
; --------------------------------------------------------------------
setup_sse:
    push    rbx              ; preserve rbx (modified by CPUID)
    mov     rax, 1
    cpuid
    bt      rdx, 25          ; test for SSE support (rdx bit 25)
    jc      .sse_ok          ; jump if SSE supported
    pop     rbx
    ret                      ; return if not supported

.sse_ok:
    ; Clear EM bit (bit 2) in CR0
    mov     rax, cr0
    and     rax, 0xFFFFFFFFFFFFFFFB  ; clear bit 2 (EM = 0)
    mov     cr0, rax

    ; Set OSFXSR (bit 9) and OSXMMEXCPT (bit 10) in CR4
    mov     rax, cr4
    or      rax, 0x600       ; 0x200 (bit 9) | 0x400 (bit 10)
    mov     cr4, rax

    pop     rbx
    ret

; --------------------------------------------------------------------
; Function: setup_avx
; Checks if AVX is available and enables it by:
;   - Calling CPUID (rax = 1) and testing:
;         rcx bit 27 for OSXSAVE support and
;         rcx bit 28 for AVX support.
;   - Using XGETBV to check that XCR0 has bits 1 (SSE) and 2 (AVX)
;     enabled. If not, it sets these bits using XSETBV.
; If the required features are not available, the function returns.
; --------------------------------------------------------------------
setup_avx:
    push    rbx              ; preserve rbx
    mov     rax, 1
    cpuid
    bt      rcx, 27          ; test for OSXSAVE (rcx bit 27)
    jc      .osx_ok          ; if set, continue
    pop     rbx
    ret                      ; otherwise, not supported

.osx_ok:
    bt      rcx, 28          ; test for AVX support (rcx bit 28)
    jc      .avx_ok          ; if set, continue
    pop     rbx
    ret                      ; otherwise, not supported

.avx_ok:
    ; Read XCR0 to check if OS has enabled saving SSE/AVX state.
    mov     rcx, 0           ; XCR register 0
    xgetbv                   ; result: rax = low 32 bits of XCR0, rdx = high 32 bits
    ; Check if bits 1 and 2 are set (mask 0x6)
    mov     rdx, rax
    and     rdx, 6
    cmp     rdx, 6
    jne     .enable_xsave    ; if not, enable them
    jmp     .avx_enabled

.enable_xsave:
    ; Enable XCR0 bits for SSE and AVX (set bits 1 and 2)
    mov     rax, 6           ; 0x6: bits 1 (SSE) and 2 (AVX)
    xor     rdx, rdx         ; upper 32 bits = 0
    xsetbv

.avx_enabled:
    pop     rbx
    ret