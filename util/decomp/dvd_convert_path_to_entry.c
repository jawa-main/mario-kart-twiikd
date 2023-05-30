int path2entry(char* path)
{
   char* path_copy = path;      // mr	r26, r3
   unsigned int r31 = 0x8027;     // lis	r31, 0x8027
        // lwz	r29, -0x65D8 (r13)
        // mr	r28, r26
        // subi	r31, r31, 12792
        // lis	r30, 0x8028
        // lbz	r0, 0 (r26)
        // extsb.	r0, r0
        // bne-	 ->0x8015DEEC
        // mr	r3, r29
        // b	->0x8015E19C
        // cmpwi	r0, 47
        // bne-	 ->0x8015DF00
        // li	r29, 0
        // addi	r26, r26, 1
        // b	->0x8015DED8
        // cmpwi	r0, 46
        // bne-	 ->0x8015DF7C
        // lbz	r0, 0x0001 (r26)
        // extsb	r0, r0
        // cmpwi	r0, 46
        // bne-	 ->0x8015DF5C
        // lbz	r0, 0x0002 (r26)
        // extsb	r0, r0
        // cmpwi	r0, 47
        // bne-	 ->0x8015DF40
        // mulli	r0, r29, 12
        // lwz	r3, -0x65C0 (r13)
        // addi	r26, r26, 3
        // add	r3, r3, r0
        // lwz	r29, 0x0004 (r3)
        // b	->0x8015DED8
        // cmpwi	r0, 0
        // bne-	 ->0x8015DF7C
        // mulli	r0, r29, 12
        // lwz	r3, -0x65C0 (r13)
        // add	r3, r3, r0
        // lwz	r3, 0x0004 (r3)
        // b	->0x8015E19C
        // cmpwi	r0, 47
        // bne-	 ->0x8015DF6C
        // addi	r26, r26, 2
        // b	->0x8015DED8
        // cmpwi	r0, 0
        // bne-	 ->0x8015DF7C
        // mr	r3, r29
        // b	->0x8015E19C
        // lwz	r0, -0x7348 (r13)
        // cmpwi	r0, 0
        // bne-	 ->0x8015E02C
        // mr	r24, r26
        // li	r5, 0
        // li	r4, 0
        // b	->0x8015DFDC
        // extsb	r0, r3
        // cmpwi	r0, 46
        // bne-	 ->0x8015DFCC
        // sub	r0, r24, r26
        // cmpwi	r0, 8
        // bgt-	 ->0x8015DFB8
        // cmpwi	r5, 1
        // bne-	 ->0x8015DFC0
        // li	r4, 1
        // b	->0x8015DFF0
        // addi	r27, r24, 1
        // li	r5, 1
        // b	->0x8015DFD8
        // cmpwi	r0, 32
        // bne-	 ->0x8015DFD8
        // li	r4, 1
        // addi	r24, r24, 1
        // lbz	r3, 0 (r24)
        // extsb.	r0, r3
        // beq-	 ->0x8015DFF0
        // cmpwi	r0, 47
        // bne+	 ->0x8015DF98
        // cmpwi	r5, 1
        // bne-	 ->0x8015E008
        // sub	r0, r24, r27
        // cmpwi	r0, 3
        // ble-	 ->0x8015E008
        // li	r4, 1
        // cmpwi	r4, 0
        // beq-	 ->0x8015E04C
        // mr	r6, r28
        // addi	r5, r30, 16096
        // subi	r3, r13, 29504
        // li	r4, 443
        // crclr	6, 6
        // bl	->0x801A25C0
        // b	->0x8015E04C
        // mr	r24, r26
        // b	->0x8015E038
        // addi	r24, r24, 1
        // lbz	r0, 0 (r24)
        // extsb.	r0, r0
        // beq-	 ->0x8015E04C
        // cmpwi	r0, 47
        // bne+	 ->0x8015E034
        // lbz	r3, 0 (r24)
        // mulli	r0, r29, 12
        // lwz	r8, -0x65C0 (r13)
        // sub	r9, r24, r26
        // extsb	r6, r3
        // lwz	r4, -0x65C4 (r13)
        // neg	r5, r6
        // add	r3, r8, r0
        // lwz	r12, 0x0038 (r31)
        // or	r0, r5, r6
        // lwz	r5, 0x0008 (r3)
        // rlwinm	r0, r0, 1, 31, 31 (80000000)
        // addi	r3, r29, 1
        // b	->0x8015E170
        // mulli	r6, r3, 12
        // lwzux	r10, r6, r8
        // rlwinm.	r7, r10, 0, 0, 7 (ff000000)
        // bne-	 ->0x8015E09C
        // cmpwi	r0, 1
        // beq-	 ->0x8015E15C
        // rlwinm	r10, r10, 0, 8, 31 (00ffffff)
        // mr	r29, r26
        // add	r25, r4, r10
        // b	->0x8015E124
        // lbz	r11, 0 (r25)
        // li	r10, 0
        // addi	r25, r25, 1
        // extsb.	r24, r11
        // blt-	 ->0x8015E0C8
        // cmpwi	r24, 256
        // blt-	 ->0x8015E0CC
        // li	r10, 1
        // cmpwi	r10, 0
        // beq-	 ->0x8015E0D8
        // b	->0x8015E0E0
        // lwz	r10, 0x0010 (r12)
        // lbzx	r24, r10, r24
        // lbz	r11, 0 (r29)
        // li	r10, 0
        // addi	r29, r29, 1
        // extsb.	r11, r11
        // blt-	 ->0x8015E0FC
        // cmpwi	r11, 256
        // blt-	 ->0x8015E100
        // li	r10, 1
        // cmpwi	r10, 0
        // beq-	 ->0x8015E10C
        // b	->0x8015E114
        // lwz	r10, 0x0010 (r12)
        // lbzx	r11, r10, r11
        // cmpw	r11, r24
        // beq-	 ->0x8015E124
        // li	r10, 0
        // b	->0x8015E154
        // lbz	r10, 0 (r25)
        // extsb.	r10, r10
        // bne+	 ->0x8015E0AC
        // lbz	r10, 0 (r29)
        // extsb	r10, r10
        // cmpwi	r10, 47
        // beq-	 ->0x8015E148
        // cmpwi	r10, 0
        // bne-	 ->0x8015E150
        // li	r10, 1
        // b	->0x8015E154
        // li	r10, 0
        // cmpwi	r10, 1
        // beq-	 ->0x8015E180
        // cmpwi	r7, 0
        // beq-	 ->0x8015E16C
        // lwz	r3, 0x0008 (r6)
        // b	->0x8015E170
        // addi	r3, r3, 1
        // cmplw	r3, r5
        // blt+	 ->0x8015E084
        // li	r3, -1
        // b	->0x8015E19C
        // cmpwi	r0, 0
        // bne-	 ->0x8015E18C
        // b	->0x8015E19C
        // add	r4, r9, r26
        // mr	r29, r3
        // addi	r26, r4, 1
        // b	->0x8015DED8
        // addi	r11, sp, 48
        // bl	->0x80020A80
        // lwz	r0, 0x0034 (sp)
        // mtlr	r0
        // addi	sp, sp, 48
blr	

}