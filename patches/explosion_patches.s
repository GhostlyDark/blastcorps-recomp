.include "macro.inc"
.include "regsave.inc"

.set noat
.set noreorder

.extern explosion_begin
.extern explosion_end

.section .recomp_patch, "ax"

glabel func_hd_code_802A68D4
    /* 85D434 802A68D4 27BDFF08 */  addiu      $sp, $sp, -0xF8
    /* 85D438 802A68D8 FFBF0000 */  sd         $ra, 0x0($sp)
    /* 85D43C 802A68DC FFA10008 */  sd         $at, 0x8($sp)
    /* 85D440 802A68E0 FFA20010 */  sd         $v0, 0x10($sp)
    /* 85D444 802A68E4 FFA30018 */  sd         $v1, 0x18($sp)
    /* 85D448 802A68E8 FFA40020 */  sd         $a0, 0x20($sp)
    /* 85D44C 802A68EC FFA50028 */  sd         $a1, 0x28($sp)
    /* 85D450 802A68F0 FFA60030 */  sd         $a2, 0x30($sp)
    /* 85D454 802A68F4 FFA70038 */  sd         $a3, 0x38($sp)
    /* 85D458 802A68F8 FFA80040 */  sd         $t0, 0x40($sp)
    /* 85D45C 802A68FC FFA90048 */  sd         $t1, 0x48($sp)
    /* 85D460 802A6900 FFAA0050 */  sd         $t2, 0x50($sp)
    /* 85D464 802A6904 FFAB0058 */  sd         $t3, 0x58($sp)
    /* 85D468 802A6908 FFAC0060 */  sd         $t4, 0x60($sp)
    /* 85D46C 802A690C FFAD0068 */  sd         $t5, 0x68($sp)
    /* 85D470 802A6910 FFAE0070 */  sd         $t6, 0x70($sp)
    /* 85D474 802A6914 FFAF0078 */  sd         $t7, 0x78($sp)
    /* 85D478 802A6918 FFB00080 */  sd         $s0, 0x80($sp)
    /* 85D47C 802A691C FFB10088 */  sd         $s1, 0x88($sp)
    /* 85D480 802A6920 FFB20090 */  sd         $s2, 0x90($sp)
    /* 85D484 802A6924 FFB30098 */  sd         $s3, 0x98($sp)
    /* 85D488 802A6928 FFB400A0 */  sd         $s4, 0xA0($sp)
    /* 85D48C 802A692C FFB500A8 */  sd         $s5, 0xA8($sp)
    /* 85D490 802A6930 FFB600B0 */  sd         $s6, 0xB0($sp)
    /* 85D494 802A6934 FFB700B8 */  sd         $s7, 0xB8($sp)
    /* 85D498 802A6938 FFB800C0 */  sd         $t8, 0xC0($sp)
    /* 85D49C 802A693C FFB900C8 */  sd         $t9, 0xC8($sp)
    /* 85D4A0 802A6940 FFBA00D0 */  sd         $k0, 0xD0($sp) /* handwritten instruction */
    /* 85D4A4 802A6944 FFBB00D8 */  sd         $k1, 0xD8($sp) /* handwritten instruction */
    /* 85D4A8 802A6948 FFBC00E0 */  sd         $gp, 0xE0($sp)
    /* 85D4AC 802A694C FFBD00E8 */  sd         $sp, 0xE8($sp)
    /* 85D4B0 802A6950 0C0A9BAE */  jal        func_hd_code_802A6EB8
    /* 85D4B4 802A6954 FFBE00F0 */   sd        $fp, 0xF0($sp)
    /* 85D4B8 802A6958 01401025 */  or         $v0, $t2, $zero
    /* 85D4BC 802A695C 8C4A0000 */  lw         $t2, 0x0($v0)

    save_all_regs
    or $a1, $t5, 0 /* slot index of the explosion, counted backwards 0 - 15 */
    jal explosion_begin
    or $a0, $t2, 0 /* current Gfx pointer */
    sd $v0, 0x50($sp) /* store return of explosion_begin into the restoring stack place of $t2 */
    restore_all_regs

    /* 85D4C0 802A6960 3C121FFF */  lui        $s2, (0x1FFFFFFF >> 16)
    /* 85D4C4 802A6964 3652FFFF */  ori        $s2, $s2, (0x1FFFFFFF & 0xFFFF)
    /* 85D4C8 802A6968 02489024 */  and        $s2, $s2, $t0
    /* 85D4CC 802A696C 3C010104 */  lui        $at, (0x1040040 >> 16)
    /* 85D4D0 802A6970 34210040 */  ori        $at, $at, (0x1040040 & 0xFFFF)
    /* 85D4D4 802A6974 AD520004 */  sw         $s2, 0x4($t2)
    /* 85D4D8 802A6978 AD410000 */  sw         $at, 0x0($t2)
    /* 85D4DC 802A697C 214A0008 */  addi       $t2, $t2, 0x8 /* handwritten instruction */
    /* 85D4E0 802A6980 AC4A0000 */  sw         $t2, 0x0($v0)
    /* 85D4E4 802A6984 01009025 */  or         $s2, $t0, $zero
    /* 85D4E8 802A6988 8D880004 */  lw         $t0, 0x4($t4)
    /* 85D4EC 802A698C 02405825 */  or         $t3, $s2, $zero
    /* 85D4F0 802A6990 01004825 */  or         $t1, $t0, $zero
    /* 85D4F4 802A6994 0C0AB31A */  jal        func_hd_code_802ACC68
    /* 85D4F8 802A6998 01005025 */   or        $t2, $t0, $zero
    /* 85D4FC 802A699C 91820034 */  lbu        $v0, 0x34($t4)
    /* 85D500 802A69A0 24010001 */  addiu      $at, $zero, 0x1
    /* 85D504 802A69A4 10410030 */  beq        $v0, $at, .Lhd_code_802A6A68
    /* 85D508 802A69A8 00000000 */   nop
    /* 85D50C 802A69AC 8D8F000C */  lw         $t7, 0xC($t4)
    /* 85D510 802A69B0 8D8E002C */  lw         $t6, 0x2C($t4)
    /* 85D514 802A69B4 91820036 */  lbu        $v0, 0x36($t4)
    /* 85D518 802A69B8 01CF082A */  slt        $at, $t6, $t7
    /* 85D51C 802A69BC 1420000D */  bnez       $at, .Lhd_code_802A69F4
    /* 85D520 802A69C0 00000000 */   nop
    /* 85D524 802A69C4 8D8B0018 */  lw         $t3, 0x18($t4)
    /* 85D528 802A69C8 01620019 */  multu      $t3, $v0
    /* 85D52C 802A69CC 8D820024 */  lw         $v0, 0x24($t4)
    /* 85D530 802A69D0 00005812 */  mflo       $t3
    /* 85D534 802A69D4 004B1021 */  addu       $v0, $v0, $t3
    /* 85D538 802A69D8 04410002 */  bgez       $v0, .Lhd_code_802A69E4
    /* 85D53C 802A69DC 00000000 */   nop
    /* 85D540 802A69E0 00021022 */  neg        $v0, $v0 /* handwritten instruction */
  .Lhd_code_802A69E4:
    /* 85D544 802A69E4 00027083 */  sra        $t6, $v0, 2
    /* 85D548 802A69E8 004E1023 */  subu       $v0, $v0, $t6
    /* 85D54C 802A69EC AD820024 */  sw         $v0, 0x24($t4)
    /* 85D550 802A69F0 24020000 */  addiu      $v0, $zero, 0x0
  .Lhd_code_802A69F4:
    /* 85D554 802A69F4 204B0001 */  addi       $t3, $v0, 0x1 /* handwritten instruction */
    /* 85D558 802A69F8 A18B0036 */  sb         $t3, 0x36($t4)
    /* 85D55C 802A69FC 8D8B0014 */  lw         $t3, 0x14($t4)
    /* 85D560 802A6A00 8D8E0008 */  lw         $t6, 0x8($t4)
    /* 85D564 802A6A04 8D900010 */  lw         $s0, 0x10($t4)
    /* 85D568 802A6A08 01620019 */  multu      $t3, $v0
    /* 85D56C 802A6A0C 00005812 */  mflo       $t3
    /* 85D570 802A6A10 01CB7021 */  addu       $t6, $t6, $t3
    /* 85D574 802A6A14 8D8B0020 */  lw         $t3, 0x20($t4)
    /* 85D578 802A6A18 01CB7021 */  addu       $t6, $t6, $t3
    /* 85D57C 802A6A1C 8D8B0018 */  lw         $t3, 0x18($t4)
    /* 85D580 802A6A20 AD8E0008 */  sw         $t6, 0x8($t4)
    /* 85D584 802A6A24 000E72C3 */  sra        $t6, $t6, 11
    /* 85D588 802A6A28 01620019 */  multu      $t3, $v0
    /* 85D58C 802A6A2C 00005812 */  mflo       $t3
    /* 85D590 802A6A30 01EB7821 */  addu       $t7, $t7, $t3
    /* 85D594 802A6A34 8D8B0024 */  lw         $t3, 0x24($t4)
    /* 85D598 802A6A38 01EB7821 */  addu       $t7, $t7, $t3
    /* 85D59C 802A6A3C 8D8B001C */  lw         $t3, 0x1C($t4)
    /* 85D5A0 802A6A40 AD8F000C */  sw         $t7, 0xC($t4)
    /* 85D5A4 802A6A44 000F7AC3 */  sra        $t7, $t7, 11
    /* 85D5A8 802A6A48 01620019 */  multu      $t3, $v0
    /* 85D5AC 802A6A4C 00005812 */  mflo       $t3
    /* 85D5B0 802A6A50 020B8021 */  addu       $s0, $s0, $t3
    /* 85D5B4 802A6A54 8D8B0028 */  lw         $t3, 0x28($t4)
    /* 85D5B8 802A6A58 020B8021 */  addu       $s0, $s0, $t3
    /* 85D5BC 802A6A5C AD900010 */  sw         $s0, 0x10($t4)
    /* 85D5C0 802A6A60 10000007 */  b          .Lhd_code_802A6A80
    /* 85D5C4 802A6A64 001082C3 */   sra       $s0, $s0, 11
  .Lhd_code_802A6A68:
    /* 85D5C8 802A6A68 91820030 */  lbu        $v0, 0x30($t4)
    /* 85D5CC 802A6A6C 0C0AAF22 */  jal        func_hd_code_802ABC88
    /* 85D5D0 802A6A70 91830031 */   lbu       $v1, 0x31($t4)
    /* 85D5D4 802A6A74 8C8E0000 */  lw         $t6, 0x0($a0)
    /* 85D5D8 802A6A78 8C8F0004 */  lw         $t7, 0x4($a0)
    /* 85D5DC 802A6A7C 8C900008 */  lw         $s0, 0x8($a0)
  .Lhd_code_802A6A80:
    /* 85D5E0 802A6A80 3C0B8036 */  lui        $t3, %hi(D_hd_code_803643F8)
    /* 85D5E4 802A6A84 3C0C8036 */  lui        $t4, %hi(D_hd_code_803643FC)
    /* 85D5E8 802A6A88 3C0D8036 */  lui        $t5, %hi(D_hd_code_80364400)
    /* 85D5EC 802A6A8C 256B43F8 */  addiu      $t3, $t3, %lo(D_hd_code_803643F8)
    /* 85D5F0 802A6A90 258C43FC */  addiu      $t4, $t4, %lo(D_hd_code_803643FC)
    /* 85D5F4 802A6A94 25AD4400 */  addiu      $t5, $t5, %lo(D_hd_code_80364400)
    /* 85D5F8 802A6A98 8D6B0000 */  lw         $t3, 0x0($t3)
    /* 85D5FC 802A6A9C 8D8C0000 */  lw         $t4, 0x0($t4)
    /* 85D600 802A6AA0 8DAD0000 */  lw         $t5, 0x0($t5)
    /* 85D604 802A6AA4 000B5AC2 */  srl        $t3, $t3, 11
    /* 85D608 802A6AA8 000C62C2 */  srl        $t4, $t4, 11
    /* 85D60C 802A6AAC 0C0AAF37 */  jal        func_hd_code_802ABCDC
    /* 85D610 802A6AB0 000D6AC2 */   srl       $t5, $t5, 11
    /* 85D614 802A6AB4 16200002 */  bnez       $s1, .Lhd_code_802A6AC0
    /* 85D618 802A6AB8 00000000 */   nop
    /* 85D61C 802A6ABC 24110001 */  addiu      $s1, $zero, 0x1
  .Lhd_code_802A6AC0:
    /* 85D620 802A6AC0 018F6023 */  subu       $t4, $t4, $t7
    /* 85D624 802A6AC4 00118AC0 */  sll        $s1, $s1, 11
    /* 85D628 802A6AC8 000C66F8 */  dsll       $t4, $t4, 27
    /* 85D62C 802A6ACC 0191001E */  ddiv       $zero, $t4, $s1
    /* 85D630 802A6AD0 00000000 */  nop
    /* 85D634 802A6AD4 16200002 */  bnez       $s1, .Lhd_code_802A6AE0
    /* 85D638 802A6AD8 00000000 */   nop
    /* 85D63C 802A6ADC 0007000D */  break      7
  .Lhd_code_802A6AE0:
    /* 85D640 802A6AE0 6401FFFF */  daddiu     $at, $zero, -0x1
    /* 85D644 802A6AE4 16210005 */  bne        $s1, $at, .Lhd_code_802A6AFC
    /* 85D648 802A6AE8 64010001 */   daddiu    $at, $zero, 0x1
    /* 85D64C 802A6AEC 00010FFC */  dsll32     $at, $at, 31
    /* 85D650 802A6AF0 15810002 */  bne        $t4, $at, .Lhd_code_802A6AFC
    /* 85D654 802A6AF4 00000000 */   nop
    /* 85D658 802A6AF8 0006000D */  break      6
  .Lhd_code_802A6AFC:
    /* 85D65C 802A6AFC 00005812 */  mflo       $t3
    /* 85D660 802A6B00 05610002 */  bgez       $t3, .Lhd_code_802A6B0C
    /* 85D664 802A6B04 01601821 */   addu      $v1, $t3, $zero
    /* 85D668 802A6B08 000B1822 */  neg        $v1, $t3 /* handwritten instruction */
  .Lhd_code_802A6B0C:
    /* 85D66C 802A6B0C 0C0AB5FF */  jal        func_hd_code_802AD7FC
    /* 85D670 802A6B10 00000000 */   nop
    /* 85D674 802A6B14 05600003 */  bltz       $t3, .Lhd_code_802A6B24
    /* 85D678 802A6B18 001E4103 */   sra       $t0, $fp, 4
    /* 85D67C 802A6B1C 240B0FFF */  addiu      $t3, $zero, 0xFFF
    /* 85D680 802A6B20 01684023 */  subu       $t0, $t3, $t0
  .Lhd_code_802A6B24:
    /* 85D684 802A6B24 3C0B803C */  lui        $t3, %hi(D_hd_code_803C4F30)
    /* 85D688 802A6B28 0C0AB2F7 */  jal        func_hd_code_802ACBDC
    /* 85D68C 802A6B2C 256B4F30 */   addiu     $t3, $t3, %lo(D_hd_code_803C4F30)
    /* 85D690 802A6B30 3C04803C */  lui        $a0, %hi(D_hd_code_803C4F30)
    /* 85D694 802A6B34 0C0AB333 */  jal        func_hd_code_802ACCCC
    /* 85D698 802A6B38 24844F30 */   addiu     $a0, $a0, %lo(D_hd_code_803C4F30)
    /* 85D69C 802A6B3C 3C088036 */  lui        $t0, %hi(D_hd_code_80364452)
    /* 85D6A0 802A6B40 25084452 */  addiu      $t0, $t0, %lo(D_hd_code_80364452)
    /* 85D6A4 802A6B44 3C0B803C */  lui        $t3, %hi(D_hd_code_803C4F30)
    /* 85D6A8 802A6B48 95080000 */  lhu        $t0, 0x0($t0)
    /* 85D6AC 802A6B4C 0C0AB2B1 */  jal        func_hd_code_802ACAC4
    /* 85D6B0 802A6B50 256B4F30 */   addiu     $t3, $t3, %lo(D_hd_code_803C4F30)
    /* 85D6B4 802A6B54 3C04803C */  lui        $a0, %hi(D_hd_code_803C4F30)
    /* 85D6B8 802A6B58 0C0AB333 */  jal        func_hd_code_802ACCCC
    /* 85D6BC 802A6B5C 24844F30 */   addiu     $a0, $a0, %lo(D_hd_code_803C4F30)
    /* 85D6C0 802A6B60 3C0B803C */  lui        $t3, %hi(D_hd_code_803C4F30)
    /* 85D6C4 802A6B64 256B4F30 */  addiu      $t3, $t3, %lo(D_hd_code_803C4F30)
    /* 85D6C8 802A6B68 000E42C0 */  sll        $t0, $t6, 11
    /* 85D6CC 802A6B6C 000F4AC0 */  sll        $t1, $t7, 11
    /* 85D6D0 802A6B70 0C0AB298 */  jal        func_hd_code_802ACA60
    /* 85D6D4 802A6B74 001052C0 */   sll       $t2, $s0, 11
    /* 85D6D8 802A6B78 3C04803C */  lui        $a0, %hi(D_hd_code_803C4F30)
    /* 85D6DC 802A6B7C 0C0AB333 */  jal        func_hd_code_802ACCCC
    /* 85D6E0 802A6B80 24844F30 */   addiu     $a0, $a0, %lo(D_hd_code_803C4F30)
    /* 85D6E4 802A6B84 0C0AB233 */  jal        func_hd_code_802AC8CC
    /* 85D6E8 802A6B88 00000000 */   nop
    /* 85D6EC 802A6B8C DFBF0000 */  ld         $ra, 0x0($sp)
    /* 85D6F0 802A6B90 DFA10008 */  ld         $at, 0x8($sp)
    /* 85D6F4 802A6B94 DFA20010 */  ld         $v0, 0x10($sp)
    /* 85D6F8 802A6B98 DFA30018 */  ld         $v1, 0x18($sp)
    /* 85D6FC 802A6B9C DFA40020 */  ld         $a0, 0x20($sp)
    /* 85D700 802A6BA0 DFA50028 */  ld         $a1, 0x28($sp)
    /* 85D704 802A6BA4 DFA60030 */  ld         $a2, 0x30($sp)
    /* 85D708 802A6BA8 DFA70038 */  ld         $a3, 0x38($sp)
    /* 85D70C 802A6BAC DFA80040 */  ld         $t0, 0x40($sp)
    /* 85D710 802A6BB0 DFA90048 */  ld         $t1, 0x48($sp)
    /* 85D714 802A6BB4 DFAA0050 */  ld         $t2, 0x50($sp)
    /* 85D718 802A6BB8 DFAB0058 */  ld         $t3, 0x58($sp)
    /* 85D71C 802A6BBC DFAC0060 */  ld         $t4, 0x60($sp)
    /* 85D720 802A6BC0 DFAD0068 */  ld         $t5, 0x68($sp)
    /* 85D724 802A6BC4 DFAE0070 */  ld         $t6, 0x70($sp)
    /* 85D728 802A6BC8 DFAF0078 */  ld         $t7, 0x78($sp)
    /* 85D72C 802A6BCC DFB00080 */  ld         $s0, 0x80($sp)
    /* 85D730 802A6BD0 DFB10088 */  ld         $s1, 0x88($sp)
    /* 85D734 802A6BD4 DFB20090 */  ld         $s2, 0x90($sp)
    /* 85D738 802A6BD8 DFB30098 */  ld         $s3, 0x98($sp)
    /* 85D73C 802A6BDC DFB400A0 */  ld         $s4, 0xA0($sp)
    /* 85D740 802A6BE0 DFB500A8 */  ld         $s5, 0xA8($sp)
    /* 85D744 802A6BE4 DFB600B0 */  ld         $s6, 0xB0($sp)
    /* 85D748 802A6BE8 DFB700B8 */  ld         $s7, 0xB8($sp)
    /* 85D74C 802A6BEC DFB800C0 */  ld         $t8, 0xC0($sp)
    /* 85D750 802A6BF0 DFB900C8 */  ld         $t9, 0xC8($sp)
    /* 85D754 802A6BF4 DFBA00D0 */  ld         $k0, 0xD0($sp) /* handwritten instruction */
    /* 85D758 802A6BF8 DFBB00D8 */  ld         $k1, 0xD8($sp) /* handwritten instruction */
    /* 85D75C 802A6BFC DFBC00E0 */  ld         $gp, 0xE0($sp)
    /* 85D760 802A6C00 DFBD00E8 */  ld         $sp, 0xE8($sp)
    /* 85D764 802A6C04 DFBE00F0 */  ld         $fp, 0xF0($sp)
    /* 85D768 802A6C08 03E00008 */  jr         $ra
    /* 85D76C 802A6C0C 27BD00F8 */   addiu     $sp, $sp, 0xF8
endlabel func_hd_code_802A68D4

glabel func_hd_code_802A64A4
    /* 85D004 802A64A4 27BDFFA8 */  addiu      $sp, $sp, -0x58
    /* 85D008 802A64A8 3C088036 */  lui        $t0, %hi(D_hd_code_8035805C)
    /* 85D00C 802A64AC FFBF0000 */  sd         $ra, 0x0($sp)
    /* 85D010 802A64B0 FFB00008 */  sd         $s0, 0x8($sp)
    /* 85D014 802A64B4 FFB10010 */  sd         $s1, 0x10($sp)
    /* 85D018 802A64B8 FFB20018 */  sd         $s2, 0x18($sp)
    /* 85D01C 802A64BC FFB30020 */  sd         $s3, 0x20($sp)
    /* 85D020 802A64C0 FFB40028 */  sd         $s4, 0x28($sp)
    /* 85D024 802A64C4 FFB50030 */  sd         $s5, 0x30($sp)
    /* 85D028 802A64C8 FFB60038 */  sd         $s6, 0x38($sp)
    /* 85D02C 802A64CC FFB70040 */  sd         $s7, 0x40($sp)
    /* 85D030 802A64D0 FFBC0048 */  sd         $gp, 0x48($sp)
    /* 85D034 802A64D4 FFBE0050 */  sd         $fp, 0x50($sp)
    /* 85D038 802A64D8 2508805C */  addiu      $t0, $t0, %lo(D_hd_code_8035805C)
    /* 85D03C 802A64DC 91080000 */  lbu        $t0, 0x0($t0)
    /* 85D040 802A64E0 11000010 */  beqz       $t0, .Lhd_code_802A6524
    /* 85D044 802A64E4 00000000 */   nop
    /* 85D048 802A64E8 3C0A803C */  lui        $t2, %hi(D_hd_code_803C6370)
    /* 85D04C 802A64EC 254A6370 */  addiu      $t2, $t2, %lo(D_hd_code_803C6370)
    /* 85D050 802A64F0 3C01803F */  lui        $at, %hi(D_hd_code_803EB780)
    /* 85D054 802A64F4 AC2AB780 */  sw         $t2, %lo(D_hd_code_803EB780)($at)
    /* 85D058 802A64F8 3C0A803C */  lui        $t2, %hi(D_hd_code_803C7B70)
    /* 85D05C 802A64FC 3C08803C */  lui        $t0, %hi(D_hd_code_803C5370)
    /* 85D060 802A6500 3C09803D */  lui        $t1, %hi(D_hd_code_803C9770)
    /* 85D064 802A6504 254A7B70 */  addiu      $t2, $t2, %lo(D_hd_code_803C7B70)
    /* 85D068 802A6508 3C01803F */  lui        $at, %hi(D_hd_code_803EB784)
    /* 85D06C 802A650C 3C0B803E */  lui        $t3, %hi(D_hd_code_803DA770)
    /* 85D070 802A6510 25085370 */  addiu      $t0, $t0, %lo(D_hd_code_803C5370)
    /* 85D074 802A6514 25299770 */  addiu      $t1, $t1, %lo(D_hd_code_803C9770)
    /* 85D078 802A6518 AC2AB784 */  sw         $t2, %lo(D_hd_code_803EB784)($at)
    /* 85D07C 802A651C 1000000F */  b          .Lhd_code_802A655C
    /* 85D080 802A6520 256BA770 */   addiu     $t3, $t3, %lo(D_hd_code_803DA770)
  .Lhd_code_802A6524:
    /* 85D084 802A6524 3C0A803C */  lui        $t2, %hi(D_hd_code_803C5770)
    /* 85D088 802A6528 254A5770 */  addiu      $t2, $t2, %lo(D_hd_code_803C5770)
    /* 85D08C 802A652C 3C01803F */  lui        $at, %hi(D_hd_code_803EB780)
    /* 85D090 802A6530 AC2AB780 */  sw         $t2, %lo(D_hd_code_803EB780)($at)
    /* 85D094 802A6534 3C0A803C */  lui        $t2, %hi(D_hd_code_803C6F70)
    /* 85D098 802A6538 3C08803C */  lui        $t0, %hi(D_hd_code_803C4F70)
    /* 85D09C 802A653C 3C09803D */  lui        $t1, %hi(D_hd_code_803C8770)
    /* 85D0A0 802A6540 254A6F70 */  addiu      $t2, $t2, %lo(D_hd_code_803C6F70)
    /* 85D0A4 802A6544 3C01803F */  lui        $at, %hi(D_hd_code_803EB784)
    /* 85D0A8 802A6548 3C0B803D */  lui        $t3, %hi(D_hd_code_803CA770)
    /* 85D0AC 802A654C 25084F70 */  addiu      $t0, $t0, %lo(D_hd_code_803C4F70)
    /* 85D0B0 802A6550 25298770 */  addiu      $t1, $t1, %lo(D_hd_code_803C8770)
    /* 85D0B4 802A6554 AC2AB784 */  sw         $t2, %lo(D_hd_code_803EB784)($at)
    /* 85D0B8 802A6558 256BA770 */  addiu      $t3, $t3, %lo(D_hd_code_803CA770)
  .Lhd_code_802A655C:
    /* 85D0BC 802A655C 3C0C803C */  lui        $t4, %hi(D_hd_code_803C4B70)
    /* 85D0C0 802A6560 3C1E803F */  lui        $fp, %hi(D_hd_code_803EA770)
    /* 85D0C4 802A6564 258C4B70 */  addiu      $t4, $t4, %lo(D_hd_code_803C4B70)
    /* 85D0C8 802A6568 27DEA770 */  addiu      $fp, $fp, %lo(D_hd_code_803EA770)
    /* 85D0CC 802A656C 0C0A9B4D */  jal        func_hd_code_802A6D34
    /* 85D0D0 802A6570 240D0010 */   addiu     $t5, $zero, 0x10
  .Lhd_code_802A6574:
    /* 85D0D4 802A6574 11A0005B */  beqz       $t5, .Lhd_code_802A66E4
    /* 85D0D8 802A6578 00000000 */   nop
    /* 85D0DC 802A657C 918E0033 */  lbu        $t6, 0x33($t4)
    /* 85D0E0 802A6580 21ADFFFF */  addi       $t5, $t5, -0x1 /* handwritten instruction */
    /* 85D0E4 802A6584 11C00054 */  beqz       $t6, .Lhd_code_802A66D8
    /* 85D0E8 802A6588 00000000 */   nop
    /* 85D0EC 802A658C 0C0A9A35 */  jal        func_hd_code_802A68D4
    /* 85D0F0 802A6590 00000000 */   nop
    /* 85D0F4 802A6594 8D8F0000 */  lw         $t7, 0x0($t4)
    /* 85D0F8 802A6598 919C0035 */  lbu        $gp, 0x35($t4)
    /* 85D0FC 802A659C 25840037 */  addiu      $a0, $t4, 0x37
    /* 85D100 802A65A0 91E20002 */  lbu        $v0, 0x2($t7)
    /* 85D104 802A65A4 91E30003 */  lbu        $v1, 0x3($t7)
    /* 85D108 802A65A8 91F40003 */  lbu        $s4, 0x3($t7)
    /* 85D10C 802A65AC 95F5000C */  lhu        $s5, 0xC($t7)
    /* 85D110 802A65B0 00430019 */  multu      $v0, $v1
    /* 85D114 802A65B4 91830032 */  lbu        $v1, 0x32($t4)
    /* 85D118 802A65B8 95F80004 */  lhu        $t8, 0x4($t7)
    /* 85D11C 802A65BC 00001012 */  mflo       $v0
    /* 85D120 802A65C0 00000000 */  nop
    /* 85D124 802A65C4 00000000 */  nop
    /* 85D128 802A65C8 00430019 */  multu      $v0, $v1
    /* 85D12C 802A65CC 95E3000A */  lhu        $v1, 0xA($t7)
    /* 85D130 802A65D0 00001012 */  mflo       $v0
    /* 85D134 802A65D4 00021040 */  sll        $v0, $v0, 1
    /* 85D138 802A65D8 004F1021 */  addu       $v0, $v0, $t7
    /* 85D13C 802A65DC 24590010 */  addiu      $t9, $v0, 0x10
    /* 85D140 802A65E0 91E20002 */  lbu        $v0, 0x2($t7)
    /* 85D144 802A65E4 00430019 */  multu      $v0, $v1
    /* 85D148 802A65E8 00009012 */  mflo       $s2
    /* 85D14C 802A65EC 00129043 */  sra        $s2, $s2, 1
    /* 85D150 802A65F0 0240B025 */  or         $s6, $s2, $zero
    /* 85D154 802A65F4 02950019 */  multu      $s4, $s5
    /* 85D158 802A65F8 00009812 */  mflo       $s3
    /* 85D15C 802A65FC 00139843 */  sra        $s3, $s3, 1
    /* 85D160 802A6600 00139822 */  neg        $s3, $s3 /* handwritten instruction */
  .Lhd_code_802A6604:
    /* 85D164 802A6604 1280000F */  beqz       $s4, .Lhd_code_802A6644
    /* 85D168 802A6608 00000000 */   nop
    /* 85D16C 802A660C 2294FFFF */  addi       $s4, $s4, -0x1 /* handwritten instruction */
    /* 85D170 802A6610 02C09025 */  or         $s2, $s6, $zero
    /* 85D174 802A6614 0040B825 */  or         $s7, $v0, $zero
  .Lhd_code_802A6618:
    /* 85D178 802A6618 52E0FFFA */  beql       $s7, $zero, .Lhd_code_802A6604
    /* 85D17C 802A661C 02759821 */   addu      $s3, $s3, $s5
    /* 85D180 802A6620 0C0A9B04 */  jal        func_hd_code_802A6C10
    /* 85D184 802A6624 22F7FFFF */   addi      $s7, $s7, -0x1 /* handwritten instruction */
    /* 85D188 802A6628 0C0A99F1 */  jal        func_hd_code_802A67C4
    /* 85D18C 802A662C 00000000 */   nop
    /* 85D190 802A6630 0C0A9B7A */  jal        func_hd_code_802A6DE8
    /* 85D194 802A6634 20840001 */   addi      $a0, $a0, 0x1 /* handwritten instruction */
    /* 85D198 802A6638 21290040 */  addi       $t1, $t1, 0x40 /* handwritten instruction */
    /* 85D19C 802A663C 1000FFF6 */  b          .Lhd_code_802A6618
    /* 85D1A0 802A6640 02439022 */   sub       $s2, $s2, $v1 /* handwritten instruction */
  .Lhd_code_802A6644:
    /* 85D1A4 802A6644 0C0A9BAE */  jal        func_hd_code_802A6EB8
    /* 85D1A8 802A6648 21080040 */   addi      $t0, $t0, 0x40 /* handwritten instruction */
    /* 85D1AC 802A664C 01407025 */  or         $t6, $t2, $zero
    /* 85D1B0 802A6650 8DCA0000 */  lw         $t2, 0x0($t6)

    save_all_regs
    jal explosion_end
    or $a0, $t2, 0 /* current Gfx pointer */
    sd $v0, 0x50($sp) /* store return of explosion_end into the restoring stack place of $t2 */
    restore_all_regs

    /* 85D1B4 802A6654 3C01BD00 */  lui        $at, (0xBD000000 >> 16)
    /* 85D1B8 802A6658 AD410000 */  sw         $at, 0x0($t2)
    /* 85D1BC 802A665C 24010000 */  addiu      $at, $zero, 0x0
    /* 85D1C0 802A6660 AD410004 */  sw         $at, 0x4($t2)
    /* 85D1C4 802A6664 214A0008 */  addi       $t2, $t2, 0x8 /* handwritten instruction */
    /* 85D1C8 802A6668 ADCA0000 */  sw         $t2, 0x0($t6)
    /* 85D1CC 802A666C 8D8F0000 */  lw         $t7, 0x0($t4)
    /* 85D1D0 802A6670 918E0032 */  lbu        $t6, 0x32($t4)
    /* 85D1D4 802A6674 95E2000E */  lhu        $v0, 0xE($t7)
    /* 85D1D8 802A6678 21CE0001 */  addi       $t6, $t6, 0x1 /* handwritten instruction */
    /* 85D1DC 802A667C 11C20003 */  beq        $t6, $v0, .Lhd_code_802A668C
    /* 85D1E0 802A6680 00000000 */   nop
    /* 85D1E4 802A6684 10000014 */  b          .Lhd_code_802A66D8
    /* 85D1E8 802A6688 A18E0032 */   sb        $t6, 0x32($t4)
  .Lhd_code_802A668C:
    /* 85D1EC 802A668C 8D8F0000 */  lw         $t7, 0x0($t4)
    /* 85D1F0 802A6690 A1800033 */  sb         $zero, 0x33($t4)
    /* 85D1F4 802A6694 91E20002 */  lbu        $v0, 0x2($t7)
    /* 85D1F8 802A6698 91E30003 */  lbu        $v1, 0x3($t7)
    /* 85D1FC 802A669C 258F0037 */  addiu      $t7, $t4, 0x37
    /* 85D200 802A66A0 00430019 */  multu      $v0, $v1
    /* 85D204 802A66A4 3C03803F */  lui        $v1, %hi(D_hd_code_803EB770)
    /* 85D208 802A66A8 2463B770 */  addiu      $v1, $v1, %lo(D_hd_code_803EB770)
    /* 85D20C 802A66AC 00001012 */  mflo       $v0
    /* 85D210 802A66B0 00000000 */  nop
    /* 85D214 802A66B4 00000000 */  nop
  .Lhd_code_802A66B8:
    /* 85D218 802A66B8 10400007 */  beqz       $v0, .Lhd_code_802A66D8
    /* 85D21C 802A66BC 00000000 */   nop
    /* 85D220 802A66C0 91EE0000 */  lbu        $t6, 0x0($t7)
    /* 85D224 802A66C4 2442FFFF */  addiu      $v0, $v0, -0x1
    /* 85D228 802A66C8 21EF0001 */  addi       $t7, $t7, 0x1 /* handwritten instruction */
    /* 85D22C 802A66CC 01C37021 */  addu       $t6, $t6, $v1
    /* 85D230 802A66D0 1000FFF9 */  b          .Lhd_code_802A66B8
    /* 85D234 802A66D4 A1C00000 */   sb        $zero, 0x0($t6)
  .Lhd_code_802A66D8:
    /* 85D238 802A66D8 218C003C */  addi       $t4, $t4, 0x3C /* handwritten instruction */
    /* 85D23C 802A66DC 1000FFA5 */  b          .Lhd_code_802A6574
    /* 85D240 802A66E0 23DE0100 */   addi      $fp, $fp, 0x100 /* handwritten instruction */
  .Lhd_code_802A66E4:
    /* 85D244 802A66E4 3C0A803F */  lui        $t2, %hi(D_hd_code_803EB780)
    /* 85D248 802A66E8 8D4AB780 */  lw         $t2, %lo(D_hd_code_803EB780)($t2)
    /* 85D24C 802A66EC 3C01B800 */  lui        $at, (0xB8000000 >> 16)
    /* 85D250 802A66F0 AD410000 */  sw         $at, 0x0($t2)
    /* 85D254 802A66F4 AD400004 */  sw         $zero, 0x4($t2)
    /* 85D258 802A66F8 214A0008 */  addi       $t2, $t2, 0x8 /* handwritten instruction */
    /* 85D25C 802A66FC 3C0A803F */  lui        $t2, %hi(D_hd_code_803EB784)
    /* 85D260 802A6700 8D4AB784 */  lw         $t2, %lo(D_hd_code_803EB784)($t2)
    /* 85D264 802A6704 AD410000 */  sw         $at, 0x0($t2)
    /* 85D268 802A6708 AD400004 */  sw         $zero, 0x4($t2)
    /* 85D26C 802A670C 0C0A99D2 */  jal        func_hd_code_802A6748
    /* 85D270 802A6710 214A0008 */   addi      $t2, $t2, 0x8 /* handwritten instruction */
    /* 85D274 802A6714 DFBF0000 */  ld         $ra, 0x0($sp)
    /* 85D278 802A6718 DFB00008 */  ld         $s0, 0x8($sp)
    /* 85D27C 802A671C DFB10010 */  ld         $s1, 0x10($sp)
    /* 85D280 802A6720 DFB20018 */  ld         $s2, 0x18($sp)
    /* 85D284 802A6724 DFB30020 */  ld         $s3, 0x20($sp)
    /* 85D288 802A6728 DFB40028 */  ld         $s4, 0x28($sp)
    /* 85D28C 802A672C DFB50030 */  ld         $s5, 0x30($sp)
    /* 85D290 802A6730 DFB60038 */  ld         $s6, 0x38($sp)
    /* 85D294 802A6734 DFB70040 */  ld         $s7, 0x40($sp)
    /* 85D298 802A6738 DFBC0048 */  ld         $gp, 0x48($sp)
    /* 85D29C 802A673C DFBE0050 */  ld         $fp, 0x50($sp)
    /* 85D2A0 802A6740 03E00008 */  jr         $ra
    /* 85D2A4 802A6744 27BD0058 */   addiu     $sp, $sp, 0x58
endlabel func_hd_code_802A64A4