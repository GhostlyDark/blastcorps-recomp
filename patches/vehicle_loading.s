.include "macro.inc"
.include "regsave.inc"

.set noat
.set noreorder

.extern patchVehicle

.section .recomp_patch, "ax"

glabel LoadLevelVehicles
    /* 85A06C 802A350C 27BDFFF0 */  addiu      $sp, $sp, -0x10
    /* 85A070 802A3510 FFBF0000 */  sd         $ra, 0x0($sp)
    /* 85A074 802A3514 0C0A8E09 */  jal        func_hd_code_802A3824
    /* 85A078 802A3518 FFA80008 */   sd        $t0, 0x8($sp)
    /* 85A07C 802A351C 3C09803F */  lui        $t1, %hi(D_hd_code_803ED3F5)
    /* 85A080 802A3520 2529D3F5 */  addiu      $t1, $t1, %lo(D_hd_code_803ED3F5)
    /* 85A084 802A3524 A1200000 */  sb         $zero, 0x0($t1)
    /* 85A088 802A3528 3C01803F */  lui        $at, %hi(D_hd_code_803ED40F)
    /* 85A08C 802A352C A020D40F */  sb         $zero, %lo(D_hd_code_803ED40F)($at)
    /* 85A090 802A3530 8D090050 */  lw         $t1, 0x50($t0)
    /* 85A094 802A3534 8D0A0054 */  lw         $t2, 0x54($t0)
    /* 85A098 802A3538 01284820 */  add        $t1, $t1, $t0 /* handwritten instruction */
    /* 85A09C 802A353C 01485020 */  add        $t2, $t2, $t0 /* handwritten instruction */
  .Lhd_code_802A3540:
    /* 85A0A0 802A3540 112A00B3 */  beq        $t1, $t2, .Lhd_code_802A3810
    /* 85A0A4 802A3544 00000000 */   nop
    /* 85A0A8 802A3548 3C0F8036 */  lui        $t7, %hi(g_nextGameState)
    /* 85A0AC 802A354C DDEF4A98 */  ld         $t7, %lo(g_nextGameState)($t7)
    /* 85A0B0 802A3550 24010002 */  addiu      $at, $zero, 0x2
    /* 85A0B4 802A3554 912B0000 */  lbu        $t3, 0x0($t1)
    /* 85A0B8 802A3558 15E1000A */  bne        $t7, $at, .Lhd_code_802A3584
    /* 85A0BC 802A355C 3C018031 */   lui       $at, %hi(D_hd_code_8030D888)
    /* 85A0C0 802A3560 DC21D888 */  ld         $at, %lo(D_hd_code_8030D888)($at)
    /* 85A0C4 802A3564 15E10007 */  bne        $t7, $at, .Lhd_code_802A3584
    /* 85A0C8 802A3568 00000000 */   nop
    /* 85A0CC 802A356C 3C0F8036 */  lui        $t7, %hi(D_hd_code_8036698C)
    /* 85A0D0 802A3570 91EF698C */  lbu        $t7, %lo(D_hd_code_8036698C)($t7)
    /* 85A0D4 802A3574 11EB0003 */  beq        $t7, $t3, .Lhd_code_802A3584
    /* 85A0D8 802A3578 00000000 */   nop
    /* 85A0DC 802A357C 1000FFF0 */  b          .Lhd_code_802A3540
    /* 85A0E0 802A3580 21290009 */   addi      $t1, $t1, 0x9 /* handwritten instruction */
  .Lhd_code_802A3584:
    /* 85A0E4 802A3584 24010001 */  addiu      $at, $zero, 0x1
    /* 85A0E8 802A3588 1561000A */  bne        $t3, $at, .Lhd_code_802A35B4
    /* 85A0EC 802A358C 00000000 */   nop
    /* 85A0F0 802A3590 3C0F8036 */  lui        $t7, %hi(D_hd_code_80364AA8)
    /* 85A0F4 802A3594 8DEF4AA8 */  lw         $t7, %lo(D_hd_code_80364AA8)($t7)
    /* 85A0F8 802A3598 24010001 */  addiu      $at, $zero, 0x1
    /* 85A0FC 802A359C 11E10005 */  beq        $t7, $at, .Lhd_code_802A35B4
    /* 85A100 802A35A0 24010080 */   addiu     $at, $zero, 0x80
    /* 85A104 802A35A4 11E10003 */  beq        $t7, $at, .Lhd_code_802A35B4
    /* 85A108 802A35A8 00000000 */   nop
    /* 85A10C 802A35AC 3C0B8036 */  lui        $t3, %hi(D_hd_code_803643D4)
    /* 85A110 802A35B0 916B43D4 */  lbu        $t3, %lo(D_hd_code_803643D4)($t3)
  .Lhd_code_802A35B4:
    /* 85A114 802A35B4 24010006 */  addiu      $at, $zero, 0x6
    /* 85A118 802A35B8 11610009 */  beq        $t3, $at, .Lhd_code_802A35E0
    /* 85A11C 802A35BC 24010007 */   addiu     $at, $zero, 0x7
    /* 85A120 802A35C0 11610007 */  beq        $t3, $at, .Lhd_code_802A35E0
    /* 85A124 802A35C4 2401000B */   addiu     $at, $zero, 0xB
    /* 85A128 802A35C8 11610005 */  beq        $t3, $at, .Lhd_code_802A35E0
    /* 85A12C 802A35CC 24010011 */   addiu     $at, $zero, 0x11
    /* 85A130 802A35D0 11610003 */  beq        $t3, $at, .Lhd_code_802A35E0
    /* 85A134 802A35D4 24010012 */   addiu     $at, $zero, 0x12
    /* 85A138 802A35D8 15610004 */  bne        $t3, $at, .Lhd_code_802A35EC
    /* 85A13C 802A35DC 00000000 */   nop
  .Lhd_code_802A35E0:
    /* 85A140 802A35E0 240F0001 */  addiu      $t7, $zero, 0x1
    /* 85A144 802A35E4 3C01803F */  lui        $at, %hi(D_hd_code_803ED40F)
    /* 85A148 802A35E8 A02FD40F */  sb         $t7, %lo(D_hd_code_803ED40F)($at)
  .Lhd_code_802A35EC:
    /* 85A14C 802A35EC 81210001 */  lb         $at, 0x1($t1)
    /* 85A150 802A35F0 912F0002 */  lbu        $t7, 0x2($t1)
    /* 85A154 802A35F4 91330004 */  lbu        $s3, 0x4($t1)
    /* 85A158 802A35F8 00010A00 */  sll        $at, $at, 8
    /* 85A15C 802A35FC 01E17825 */  or         $t7, $t7, $at
    /* 85A160 802A3600 81210003 */  lb         $at, 0x3($t1)
    /* 85A164 802A3604 91300006 */  lbu        $s0, 0x6($t1)
    /* 85A168 802A3608 91310008 */  lbu        $s1, 0x8($t1)
    /* 85A16C 802A360C 00010A00 */  sll        $at, $at, 8
    /* 85A170 802A3610 02619825 */  or         $s3, $s3, $at
    /* 85A174 802A3614 81210005 */  lb         $at, 0x5($t1)
    /* 85A178 802A3618 000F7940 */  sll        $t7, $t7, 5
    /* 85A17C 802A361C 00139940 */  sll        $s3, $s3, 5
    /* 85A180 802A3620 00010A00 */  sll        $at, $at, 8
    /* 85A184 802A3624 02018025 */  or         $s0, $s0, $at
    /* 85A188 802A3628 81210007 */  lb         $at, 0x7($t1)
    /* 85A18C 802A362C 00108140 */  sll        $s0, $s0, 5
    /* 85A190 802A3630 21290009 */  addi       $t1, $t1, 0x9 /* handwritten instruction */
    /* 85A194 802A3634 00010A00 */  sll        $at, $at, 8
    /* 85A198 802A3638 0C0A8E5B */  jal        func_hd_code_802A396C
    /* 85A19C 802A363C 02218825 */   or        $s1, $s1, $at

    save_all_regs
    or $a0, $t3, $zero /* vehicle id */
    jal patchVehicle
    or $a1, $s2, $zero /* loaded vehicle data */
    restore_all_regs

    /* 85A1A0 802A3640 15600005 */  bnez       $t3, .Lhd_code_802A3658
    /* 85A1A4 802A3644 00000000 */   nop
    /* 85A1A8 802A3648 0C0AB8DC */  jal        func_hd_code_802AE370
    /* 85A1AC 802A364C 00000000 */   nop
    /* 85A1B0 802A3650 1000FFBB */  b          .Lhd_code_802A3540
    /* 85A1B4 802A3654 00000000 */   nop
  .Lhd_code_802A3658:
    /* 85A1B8 802A3658 24010001 */  addiu      $at, $zero, 0x1
    /* 85A1BC 802A365C 15610005 */  bne        $t3, $at, .Lhd_code_802A3674
    /* 85A1C0 802A3660 00000000 */   nop
    /* 85A1C4 802A3664 0C0ABF18 */  jal        func_hd_code_802AFC60
    /* 85A1C8 802A3668 00000000 */   nop
    /* 85A1CC 802A366C 1000FFB4 */  b          .Lhd_code_802A3540
    /* 85A1D0 802A3670 00000000 */   nop
  .Lhd_code_802A3674:
    /* 85A1D4 802A3674 24010002 */  addiu      $at, $zero, 0x2
    /* 85A1D8 802A3678 15610005 */  bne        $t3, $at, .Lhd_code_802A3690
    /* 85A1DC 802A367C 00000000 */   nop
    /* 85A1E0 802A3680 0C0AC368 */  jal        func_hd_code_802B0DA0
    /* 85A1E4 802A3684 00000000 */   nop
    /* 85A1E8 802A3688 1000FFAD */  b          .Lhd_code_802A3540
    /* 85A1EC 802A368C 00000000 */   nop
  .Lhd_code_802A3690:
    /* 85A1F0 802A3690 24010003 */  addiu      $at, $zero, 0x3
    /* 85A1F4 802A3694 15610005 */  bne        $t3, $at, .Lhd_code_802A36AC
    /* 85A1F8 802A3698 00000000 */   nop
    /* 85A1FC 802A369C 0C0ACA70 */  jal        func_hd_code_802B29C0
    /* 85A200 802A36A0 00000000 */   nop
    /* 85A204 802A36A4 1000FFA6 */  b          .Lhd_code_802A3540
    /* 85A208 802A36A8 00000000 */   nop
  .Lhd_code_802A36AC:
    /* 85A20C 802A36AC 24010004 */  addiu      $at, $zero, 0x4
    /* 85A210 802A36B0 15610005 */  bne        $t3, $at, .Lhd_code_802A36C8
    /* 85A214 802A36B4 00000000 */   nop
    /* 85A218 802A36B8 0C0AD040 */  jal        func_hd_code_802B4100
    /* 85A21C 802A36BC 00000000 */   nop
    /* 85A220 802A36C0 1000FF9F */  b          .Lhd_code_802A3540
    /* 85A224 802A36C4 00000000 */   nop
  .Lhd_code_802A36C8:
    /* 85A228 802A36C8 24010005 */  addiu      $at, $zero, 0x5
    /* 85A22C 802A36CC 15610005 */  bne        $t3, $at, .Lhd_code_802A36E4
    /* 85A230 802A36D0 00000000 */   nop
    /* 85A234 802A36D4 0C0AD640 */  jal        func_hd_code_802B5900
    /* 85A238 802A36D8 00000000 */   nop
    /* 85A23C 802A36DC 1000FF98 */  b          .Lhd_code_802A3540
    /* 85A240 802A36E0 00000000 */   nop
  .Lhd_code_802A36E4:
    /* 85A244 802A36E4 24010006 */  addiu      $at, $zero, 0x6
    /* 85A248 802A36E8 15610005 */  bne        $t3, $at, .Lhd_code_802A3700
    /* 85A24C 802A36EC 00000000 */   nop
    /* 85A250 802A36F0 0C0AEB60 */  jal        func_hd_code_802BAD80
    /* 85A254 802A36F4 00000000 */   nop
    /* 85A258 802A36F8 1000FF91 */  b          .Lhd_code_802A3540
    /* 85A25C 802A36FC 00000000 */   nop
  .Lhd_code_802A3700:
    /* 85A260 802A3700 24010007 */  addiu      $at, $zero, 0x7
    /* 85A264 802A3704 15610005 */  bne        $t3, $at, .Lhd_code_802A371C
    /* 85A268 802A3708 00000000 */   nop
    /* 85A26C 802A370C 0C0AEE98 */  jal        func_hd_code_802BBA60
    /* 85A270 802A3710 00000000 */   nop
    /* 85A274 802A3714 1000FF8A */  b          .Lhd_code_802A3540
    /* 85A278 802A3718 00000000 */   nop
  .Lhd_code_802A371C:
    /* 85A27C 802A371C 24010008 */  addiu      $at, $zero, 0x8
    /* 85A280 802A3720 15610005 */  bne        $t3, $at, .Lhd_code_802A3738
    /* 85A284 802A3724 00000000 */   nop
    /* 85A288 802A3728 0C0ADCD0 */  jal        func_hd_code_802B7340
    /* 85A28C 802A372C 00000000 */   nop
    /* 85A290 802A3730 1000FF83 */  b          .Lhd_code_802A3540
    /* 85A294 802A3734 00000000 */   nop
  .Lhd_code_802A3738:
    /* 85A298 802A3738 24010009 */  addiu      $at, $zero, 0x9
    /* 85A29C 802A373C 15610005 */  bne        $t3, $at, .Lhd_code_802A3754
    /* 85A2A0 802A3740 00000000 */   nop
    /* 85A2A4 802A3744 0C0B1448 */  jal        func_hd_code_802C5120
    /* 85A2A8 802A3748 00000000 */   nop
    /* 85A2AC 802A374C 1000FF7C */  b          .Lhd_code_802A3540
    /* 85A2B0 802A3750 00000000 */   nop
  .Lhd_code_802A3754:
    /* 85A2B4 802A3754 2401000A */  addiu      $at, $zero, 0xA
    /* 85A2B8 802A3758 15610005 */  bne        $t3, $at, .Lhd_code_802A3770
    /* 85A2BC 802A375C 00000000 */   nop
    /* 85A2C0 802A3760 0C0B26E4 */  jal        func_hd_code_802C9B90
    /* 85A2C4 802A3764 00000000 */   nop
    /* 85A2C8 802A3768 1000FF75 */  b          .Lhd_code_802A3540
    /* 85A2CC 802A376C 00000000 */   nop
  .Lhd_code_802A3770:
    /* 85A2D0 802A3770 2401000B */  addiu      $at, $zero, 0xB
    /* 85A2D4 802A3774 11610005 */  beq        $t3, $at, .Lhd_code_802A378C
    /* 85A2D8 802A3778 24010011 */   addiu     $at, $zero, 0x11
    /* 85A2DC 802A377C 11610003 */  beq        $t3, $at, .Lhd_code_802A378C
    /* 85A2E0 802A3780 24010012 */   addiu     $at, $zero, 0x12
    /* 85A2E4 802A3784 15610005 */  bne        $t3, $at, .Lhd_code_802A379C
    /* 85A2E8 802A3788 00000000 */   nop
  .Lhd_code_802A378C:
    /* 85A2EC 802A378C 0C0B2034 */  jal        func_hd_code_802C80D0
    /* 85A2F0 802A3790 00000000 */   nop
    /* 85A2F4 802A3794 1000FF6A */  b          .Lhd_code_802A3540
    /* 85A2F8 802A3798 00000000 */   nop
  .Lhd_code_802A379C:
    /* 85A2FC 802A379C 2401000D */  addiu      $at, $zero, 0xD
    /* 85A300 802A37A0 15610005 */  bne        $t3, $at, .Lhd_code_802A37B8
    /* 85A304 802A37A4 00000000 */   nop
    /* 85A308 802A37A8 0C0B2DC8 */  jal        func_hd_code_802CB720
    /* 85A30C 802A37AC 00000000 */   nop
    /* 85A310 802A37B0 1000FF63 */  b          .Lhd_code_802A3540
    /* 85A314 802A37B4 00000000 */   nop
  .Lhd_code_802A37B8:
    /* 85A318 802A37B8 2401000E */  addiu      $at, $zero, 0xE
    /* 85A31C 802A37BC 15610005 */  bne        $t3, $at, .Lhd_code_802A37D4
    /* 85A320 802A37C0 00000000 */   nop
    /* 85A324 802A37C4 0C0B3248 */  jal        func_hd_code_802CC920
    /* 85A328 802A37C8 00000000 */   nop
    /* 85A32C 802A37CC 1000FF5C */  b          .Lhd_code_802A3540
    /* 85A330 802A37D0 00000000 */   nop
  .Lhd_code_802A37D4:
    /* 85A334 802A37D4 2401000F */  addiu      $at, $zero, 0xF
    /* 85A338 802A37D8 15610005 */  bne        $t3, $at, .Lhd_code_802A37F0
    /* 85A33C 802A37DC 00000000 */   nop
    /* 85A340 802A37E0 0C0B3DA8 */  jal        func_hd_code_802CF6A0
    /* 85A344 802A37E4 00000000 */   nop
    /* 85A348 802A37E8 1000FF55 */  b          .Lhd_code_802A3540
    /* 85A34C 802A37EC 00000000 */   nop
  .Lhd_code_802A37F0:
    /* 85A350 802A37F0 24010010 */  addiu      $at, $zero, 0x10
    /* 85A354 802A37F4 15610005 */  bne        $t3, $at, .Lhd_code_802A380C
    /* 85A358 802A37F8 00000000 */   nop
    /* 85A35C 802A37FC 0C0B41F8 */  jal        func_hd_code_802D07E0
    /* 85A360 802A3800 00000000 */   nop
    /* 85A364 802A3804 1000FF4E */  b          .Lhd_code_802A3540
    /* 85A368 802A3808 00000000 */   nop
  .Lhd_code_802A380C:
    /* 85A36C 802A380C 0000000C */  syscall    0 /* handwritten instruction */
  .Lhd_code_802A3810:
    /* 85A370 802A3810 DFBF0000 */  ld         $ra, 0x0($sp)
    /* 85A374 802A3814 DFA80008 */  ld         $t0, 0x8($sp)
    /* 85A378 802A3818 27BD0010 */  addiu      $sp, $sp, 0x10
    /* 85A37C 802A381C 03E00008 */  jr         $ra
    /* 85A380 802A3820 00000000 */   nop
endlabel LoadLevelVehicles
