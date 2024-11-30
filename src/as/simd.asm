section .text
	global __asm_avx_addf
	global __asm_avx_subf

; void __asm_avx_addf(const float *__a, const float *__b, float *__r)
__asm_avx_addf:
	; __a -> rdi
	; __b -> rsi
	; __r -> rdx
	vmovaps ymm0, [rdi]				; Load 32 bytes from a.
	vmovaps ymm1, [rsi]				; Load 32 bytes from b.
	vaddps ymm2, ymm0, ymm1		; Add ymm0 and ymm1, store the result in ymm2.
	vmovaps [rdx], ymm2				; Store the result in the allocated stack space.
	ret												; Return.

; void __asm_avx_subf(const float *__a, const float *__b, float *__r)
__asm_avx_subf:
	; __a -> rdi
	; __b -> rsi
	; __r -> rdx
	vmovaps ymm0, [rdi]				; Load __a into ymm0
	vmovaps ymm1, [rsi]				; Load __b into ymm1
	vsubps ymm2, ymm0, ymm1		; Store result in ymm2
	vmovaps [rdx], ymm2				; Load ymm2 into __r
	ret												; Return
	
