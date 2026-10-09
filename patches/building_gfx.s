.include "macro.inc"
.include "regsave.inc"

.set noat
.set noreorder

.extern building_part_begin
.extern building_part_end

.section .recomp_patch, "ax"

glabel func_hd_code_802BD1F8
    /* 873D58 802BD1F8 27BDFFA8 */  addiu      $sp, $sp, -0x58
    /* 873D5C 802BD1FC 8FA80068 */  lw         $t0, 0x68($sp)
    /* 873D60 802BD200 3C01803F */  lui        $at, %hi(D_hd_code_803F7658)
    /* 873D64 802BD204 FFBF0000 */  sd         $ra, 0x0($sp)
    /* 873D68 802BD208 AC287658 */  sw         $t0, %lo(D_hd_code_803F7658)($at)
    /* 873D6C 802BD20C 8FA8006C */  lw         $t0, 0x6C($sp)
    /* 873D70 802BD210 3C01803F */  lui        $at, %hi(D_hd_code_803F765C)
    /* 873D74 802BD214 FFB00008 */  sd         $s0, 0x8($sp)
    /* 873D78 802BD218 FFB10010 */  sd         $s1, 0x10($sp)
    /* 873D7C 802BD21C FFB20018 */  sd         $s2, 0x18($sp)
    /* 873D80 802BD220 FFB30020 */  sd         $s3, 0x20($sp)
    /* 873D84 802BD224 FFB40028 */  sd         $s4, 0x28($sp)
    /* 873D88 802BD228 FFB50030 */  sd         $s5, 0x30($sp)
    /* 873D8C 802BD22C FFB60038 */  sd         $s6, 0x38($sp)
    /* 873D90 802BD230 FFB70040 */  sd         $s7, 0x40($sp)
    /* 873D94 802BD234 FFBC0048 */  sd         $gp, 0x48($sp)
    /* 873D98 802BD238 FFBE0050 */  sd         $fp, 0x50($sp)
    /* 873D9C 802BD23C 8FA20070 */  lw         $v0, 0x70($sp)
    /* 873DA0 802BD240 0C0B0231 */  jal        func_hd_code_802C08C4
    /* 873DA4 802BD244 AC28765C */   sw        $t0, %lo(D_hd_code_803F765C)($at)
    /* 873DA8 802BD248 0C0B032F */  jal        func_hd_code_802C0CBC
    /* 873DAC 802BD24C 8FA20074 */   lw        $v0, 0x74($sp)
    /* 873DB0 802BD250 3C028036 */  lui        $v0, %hi(D_hd_code_8035805C)
    /* 873DB4 802BD254 2442805C */  addiu      $v0, $v0, %lo(D_hd_code_8035805C)
    /* 873DB8 802BD258 90420000 */  lbu        $v0, 0x0($v0)
    /* 873DBC 802BD25C 00E04025 */  or         $t0, $a3, $zero
    /* 873DC0 802BD260 10400004 */  beqz       $v0, .Lhd_code_802BD274
    /* 873DC4 802BD264 00000000 */   nop
    /* 873DC8 802BD268 3C02803F */  lui        $v0, %hi(D_hd_code_803F24D0)
    /* 873DCC 802BD26C 10000003 */  b          .Lhd_code_802BD27C
    /* 873DD0 802BD270 244224D0 */   addiu     $v0, $v0, %lo(D_hd_code_803F24D0)
  .Lhd_code_802BD274:
    /* 873DD4 802BD274 3C02803F */  lui        $v0, %hi(D_hd_code_803F2ED0)
    /* 873DD8 802BD278 24422ED0 */  addiu      $v0, $v0, %lo(D_hd_code_803F2ED0)
  .Lhd_code_802BD27C:
    /* 873DDC 802BD27C 3C03803F */  lui        $v1, %hi(D_hd_code_803F3964)
    /* 873DE0 802BD280 24633964 */  addiu      $v1, $v1, %lo(D_hd_code_803F3964)
    /* 873DE4 802BD284 AC620000 */  sw         $v0, 0x0($v1)
    /* 873DE8 802BD288 3C03803F */  lui        $v1, %hi(D_hd_code_803F7654)
    /* 873DEC 802BD28C 3C02803F */  lui        $v0, %hi(D_hd_code_803F4030)
    /* 873DF0 802BD290 24637654 */  addiu      $v1, $v1, %lo(D_hd_code_803F7654)
    /* 873DF4 802BD294 24424030 */  addiu      $v0, $v0, %lo(D_hd_code_803F4030)
    /* 873DF8 802BD298 8C630000 */  lw         $v1, 0x0($v1)
  .Lhd_code_802BD29C:
    /* 873DFC 802BD29C 10430159 */  beq        $v0, $v1, .Lhd_code_802BD804
    /* 873E00 802BD2A0 00000000 */   nop
    /* 873E04 802BD2A4 3C09803C */  lui        $t1, %hi(D_hd_code_803C30A8)
    /* 873E08 802BD2A8 904700E8 */  lbu        $a3, 0xE8($v0)
    /* 873E0C 802BD2AC 252930A8 */  addiu      $t1, $t1, %lo(D_hd_code_803C30A8)
  .Lhd_code_802BD2B0:
    /* 873E10 802BD2B0 852A0000 */  lh         $t2, 0x0($t1)
    /* 873E14 802BD2B4 2401FFFF */  addiu      $at, $zero, -0x1
    /* 873E18 802BD2B8 21290002 */  addi       $t1, $t1, 0x2 /* handwritten instruction */
    /* 873E1C 802BD2BC 1141013B */  beq        $t2, $at, .Lhd_code_802BD7AC
    /* 873E20 802BD2C0 00000000 */   nop
    /* 873E24 802BD2C4 14EAFFFA */  bne        $a3, $t2, .Lhd_code_802BD2B0
    /* 873E28 802BD2C8 00000000 */   nop
    /* 873E2C 802BD2CC 8C490030 */  lw         $t1, 0x30($v0)
    /* 873E30 802BD2D0 24010038 */  addiu      $at, $zero, 0x38
    /* 873E34 802BD2D4 15210005 */  bne        $t1, $at, .Lhd_code_802BD2EC
    /* 873E38 802BD2D8 00000000 */   nop
    /* 873E3C 802BD2DC 0C0AF632 */  jal        func_hd_code_802BD8C8
    /* 873E40 802BD2E0 00000000 */   nop
    /* 873E44 802BD2E4 15200131 */  bnez       $t1, .Lhd_code_802BD7AC
    /* 873E48 802BD2E8 00000000 */   nop
  .Lhd_code_802BD2EC:
    /* 873E4C 802BD2EC 8C4B0000 */  lw         $t3, 0x0($v0)
    /* 873E50 802BD2F0 956B000E */  lhu        $t3, 0xE($t3)
    /* 873E54 802BD2F4 11600074 */  beqz       $t3, .Lhd_code_802BD4C8
    /* 873E58 802BD2F8 00000000 */   nop
    /* 873E5C 802BD2FC 27BDFF78 */  addiu      $sp, $sp, -0x88
    /* 873E60 802BD300 FFBF0000 */  sd         $ra, 0x0($sp)
    /* 873E64 802BD304 FFA20008 */  sd         $v0, 0x8($sp)
    /* 873E68 802BD308 FFA30010 */  sd         $v1, 0x10($sp)
    /* 873E6C 802BD30C FFA40018 */  sd         $a0, 0x18($sp)
    /* 873E70 802BD310 FFA50020 */  sd         $a1, 0x20($sp)
    /* 873E74 802BD314 FFA60028 */  sd         $a2, 0x28($sp)
    /* 873E78 802BD318 FFA70030 */  sd         $a3, 0x30($sp)
    /* 873E7C 802BD31C FFA80038 */  sd         $t0, 0x38($sp)
    /* 873E80 802BD320 FFA90040 */  sd         $t1, 0x40($sp)
    /* 873E84 802BD324 FFAA0048 */  sd         $t2, 0x48($sp)
    /* 873E88 802BD328 FFAB0050 */  sd         $t3, 0x50($sp)
    /* 873E8C 802BD32C FFAC0058 */  sd         $t4, 0x58($sp)
    /* 873E90 802BD330 FFAD0060 */  sd         $t5, 0x60($sp)
    /* 873E94 802BD334 FFAE0068 */  sd         $t6, 0x68($sp)
    /* 873E98 802BD338 FFAF0070 */  sd         $t7, 0x70($sp)
    /* 873E9C 802BD33C FFB80078 */  sd         $t8, 0x78($sp)
    /* 873EA0 802BD340 FFB90080 */  sd         $t9, 0x80($sp)
    /* 873EA4 802BD344 23BDFFC0 */  addi       $sp, $sp, -0x40 /* handwritten instruction */
    /* 873EA8 802BD348 AFA40018 */  sw         $a0, 0x18($sp)
    /* 873EAC 802BD34C AFA5001C */  sw         $a1, 0x1C($sp)
    /* 873EB0 802BD350 8C440010 */  lw         $a0, 0x10($v0)
    /* 873EB4 802BD354 3C07803F */  lui        $a3, %hi(D_hd_code_803F765C)
    /* 873EB8 802BD358 8CE7765C */  lw         $a3, %lo(D_hd_code_803F765C)($a3)
    /* 873EBC 802BD35C AFA40024 */  sw         $a0, 0x24($sp)
    /* 873EC0 802BD360 8C440014 */  lw         $a0, 0x14($v0)
    /* 873EC4 802BD364 3C01803F */  lui        $at, %hi(D_hd_code_803F765C)
    /* 873EC8 802BD368 AFA40028 */  sw         $a0, 0x28($sp)
    /* 873ECC 802BD36C 8C440018 */  lw         $a0, 0x18($v0)
    /* 873ED0 802BD370 AFA4002C */  sw         $a0, 0x2C($sp)
    /* 873ED4 802BD374 8C44001C */  lw         $a0, 0x1C($v0)
    /* 873ED8 802BD378 8C450020 */  lw         $a1, 0x20($v0)
    /* 873EDC 802BD37C 8C460024 */  lw         $a2, 0x24($v0)
    /* 873EE0 802BD380 AFA70010 */  sw         $a3, 0x10($sp)
    /* 873EE4 802BD384 20E70040 */  addi       $a3, $a3, 0x40 /* handwritten instruction */
    /* 873EE8 802BD388 AC27765C */  sw         $a3, %lo(D_hd_code_803F765C)($at)
    /* 873EEC 802BD38C 24470038 */  addiu      $a3, $v0, 0x38
    /* 873EF0 802BD390 AFA70014 */  sw         $a3, 0x14($sp)
    /* 873EF4 802BD394 8C470034 */  lw         $a3, 0x34($v0)
    /* 873EF8 802BD398 AFA70020 */  sw         $a3, 0x20($sp)
    /* 873EFC 802BD39C 0C0A4CE8 */  jal        func_hd_code_802933A0
    /* 873F00 802BD3A0 01603825 */   or        $a3, $t3, $zero
    /* 873F04 802BD3A4 23BD0040 */  addi       $sp, $sp, 0x40 /* handwritten instruction */
    /* 873F08 802BD3A8 DFBF0000 */  ld         $ra, 0x0($sp)
    /* 873F0C 802BD3AC DFA20008 */  ld         $v0, 0x8($sp)
    /* 873F10 802BD3B0 DFA30010 */  ld         $v1, 0x10($sp)
    /* 873F14 802BD3B4 DFA40018 */  ld         $a0, 0x18($sp)
    /* 873F18 802BD3B8 DFA50020 */  ld         $a1, 0x20($sp)
    /* 873F1C 802BD3BC DFA60028 */  ld         $a2, 0x28($sp)
    /* 873F20 802BD3C0 DFA70030 */  ld         $a3, 0x30($sp)
    /* 873F24 802BD3C4 DFA80038 */  ld         $t0, 0x38($sp)
    /* 873F28 802BD3C8 DFA90040 */  ld         $t1, 0x40($sp)
    /* 873F2C 802BD3CC DFAA0048 */  ld         $t2, 0x48($sp)
    /* 873F30 802BD3D0 DFAB0050 */  ld         $t3, 0x50($sp)
    /* 873F34 802BD3D4 DFAC0058 */  ld         $t4, 0x58($sp)
    /* 873F38 802BD3D8 DFAD0060 */  ld         $t5, 0x60($sp)
    /* 873F3C 802BD3DC DFAE0068 */  ld         $t6, 0x68($sp)
    /* 873F40 802BD3E0 DFAF0070 */  ld         $t7, 0x70($sp)
    /* 873F44 802BD3E4 DFB80078 */  ld         $t8, 0x78($sp)
    /* 873F48 802BD3E8 DFB90080 */  ld         $t9, 0x80($sp)
    /* 873F4C 802BD3EC 27BD0088 */  addiu      $sp, $sp, 0x88
    /* 873F50 802BD3F0 27BDFF78 */  addiu      $sp, $sp, -0x88
    /* 873F54 802BD3F4 FFA60028 */  sd         $a2, 0x28($sp)
    /* 873F58 802BD3F8 3C06803F */  lui        $a2, %hi(D_hd_code_803F7664)
    /* 873F5C 802BD3FC 8CC67664 */  lw         $a2, %lo(D_hd_code_803F7664)($a2)
    /* 873F60 802BD400 20840008 */  addi       $a0, $a0, 0x8 /* handwritten instruction */
    /* 873F64 802BD404 20A50008 */  addi       $a1, $a1, 0x8 /* handwritten instruction */
    /* 873F68 802BD408 FFA70030 */  sd         $a3, 0x30($sp)
    /* 873F6C 802BD40C FFA50020 */  sd         $a1, 0x20($sp)
    /* 873F70 802BD410 FFA40018 */  sd         $a0, 0x18($sp)
    /* 873F74 802BD414 FFBF0000 */  sd         $ra, 0x0($sp)
    /* 873F78 802BD418 FFA20008 */  sd         $v0, 0x8($sp)
    /* 873F7C 802BD41C FFA30010 */  sd         $v1, 0x10($sp)
    /* 873F80 802BD420 FFA80038 */  sd         $t0, 0x38($sp)
    /* 873F84 802BD424 FFA90040 */  sd         $t1, 0x40($sp)
    /* 873F88 802BD428 FFAA0048 */  sd         $t2, 0x48($sp)
    /* 873F8C 802BD42C FFAB0050 */  sd         $t3, 0x50($sp)
    /* 873F90 802BD430 FFAC0058 */  sd         $t4, 0x58($sp)
    /* 873F94 802BD434 FFAD0060 */  sd         $t5, 0x60($sp)
    /* 873F98 802BD438 FFAE0068 */  sd         $t6, 0x68($sp)
    /* 873F9C 802BD43C FFAF0070 */  sd         $t7, 0x70($sp)
    /* 873FA0 802BD440 FFB80078 */  sd         $t8, 0x78($sp)
    /* 873FA4 802BD444 FFB90080 */  sd         $t9, 0x80($sp)
    /* 873FA8 802BD448 8C430010 */  lw         $v1, 0x10($v0)
    /* 873FAC 802BD44C 3C07803F */  lui        $a3, %hi(D_hd_code_803F7668)
    /* 873FB0 802BD450 AC460010 */  sw         $a2, 0x10($v0)
    /* 873FB4 802BD454 8CE77668 */  lw         $a3, %lo(D_hd_code_803F7668)($a3)
    /* 873FB8 802BD458 8C440014 */  lw         $a0, 0x14($v0)
    /* 873FBC 802BD45C 3C08803F */  lui        $t0, %hi(D_hd_code_803F766C)
    /* 873FC0 802BD460 AC470014 */  sw         $a3, 0x14($v0)
    /* 873FC4 802BD464 8D08766C */  lw         $t0, %lo(D_hd_code_803F766C)($t0)
    /* 873FC8 802BD468 8C450018 */  lw         $a1, 0x18($v0)
    /* 873FCC 802BD46C 00C33023 */  subu       $a2, $a2, $v1
    /* 873FD0 802BD470 AC480018 */  sw         $t0, 0x18($v0)
    /* 873FD4 802BD474 00E43823 */  subu       $a3, $a3, $a0
    /* 873FD8 802BD478 0C0AF667 */  jal        func_hd_code_802BD99C
    /* 873FDC 802BD47C 01054023 */   subu      $t0, $t0, $a1
    /* 873FE0 802BD480 DFBF0000 */  ld         $ra, 0x0($sp)
    /* 873FE4 802BD484 DFA20008 */  ld         $v0, 0x8($sp)
    /* 873FE8 802BD488 DFA30010 */  ld         $v1, 0x10($sp)
    /* 873FEC 802BD48C DFA40018 */  ld         $a0, 0x18($sp)
    /* 873FF0 802BD490 DFA50020 */  ld         $a1, 0x20($sp)
    /* 873FF4 802BD494 DFA60028 */  ld         $a2, 0x28($sp)
    /* 873FF8 802BD498 DFA70030 */  ld         $a3, 0x30($sp)
    /* 873FFC 802BD49C DFA80038 */  ld         $t0, 0x38($sp)
    /* 874000 802BD4A0 DFA90040 */  ld         $t1, 0x40($sp)
    /* 874004 802BD4A4 DFAA0048 */  ld         $t2, 0x48($sp)
    /* 874008 802BD4A8 DFAB0050 */  ld         $t3, 0x50($sp)
    /* 87400C 802BD4AC DFAC0058 */  ld         $t4, 0x58($sp)
    /* 874010 802BD4B0 DFAD0060 */  ld         $t5, 0x60($sp)
    /* 874014 802BD4B4 DFAE0068 */  ld         $t6, 0x68($sp)
    /* 874018 802BD4B8 DFAF0070 */  ld         $t7, 0x70($sp)
    /* 87401C 802BD4BC DFB80078 */  ld         $t8, 0x78($sp)
    /* 874020 802BD4C0 DFB90080 */  ld         $t9, 0x80($sp)
    /* 874024 802BD4C4 27BD0088 */  addiu      $sp, $sp, 0x88
  .Lhd_code_802BD4C8:
    /* 874028 802BD4C8 8C4B0000 */  lw         $t3, 0x0($v0)
    /* 87402C 802BD4CC 8D690028 */  lw         $t1, 0x28($t3)
    /* 874030 802BD4D0 8D6A002C */  lw         $t2, 0x2C($t3)
    /* 874034 802BD4D4 8D6C0038 */  lw         $t4, 0x38($t3)
    /* 874038 802BD4D8 8D6D003C */  lw         $t5, 0x3C($t3)
    /* 87403C 802BD4DC 012B4821 */  addu       $t1, $t1, $t3
    /* 874040 802BD4E0 014B5021 */  addu       $t2, $t2, $t3
    /* 874044 802BD4E4 018B6021 */  addu       $t4, $t4, $t3
    /* 874048 802BD4E8 0C0AF76D */  jal        func_hd_code_802BDDB4
    /* 87404C 802BD4EC 01AB6821 */   addu      $t5, $t5, $t3
    /* 874050 802BD4F0 3C01FFEF */  lui        $at, (0xFFEF0009 >> 16)
    /* 874054 802BD4F4 34210009 */  ori        $at, $at, (0xFFEF0009 & 0xFFFF)
    /* 874058 802BD4F8 00010A80 */  sll        $at, $at, 10
    /* 87405C 802BD4FC 34210006 */  ori        $at, $at, 0x6
    /* 874060 802BD500 AC810000 */  sw         $at, 0x0($a0)
    /* 874064 802BD504 3C0E8000 */  lui        $t6, (0x80000000 >> 16)
    /* 874068 802BD508 216F0050 */  addi       $t7, $t3, 0x50 /* handwritten instruction */
    /* 87406C 802BD50C 3C01FFEF */  lui        $at, (0xFFEF0009 >> 16)
    /* 874070 802BD510 01EE7823 */  subu       $t7, $t7, $t6
    /* 874074 802BD514 34210009 */  ori        $at, $at, (0xFFEF0009 & 0xFFFF)
    /* 874078 802BD518 AC8F0004 */  sw         $t7, 0x4($a0)
    /* 87407C 802BD51C 00010A80 */  sll        $at, $at, 10
    /* 874080 802BD520 34210006 */  ori        $at, $at, 0x6
    /* 874084 802BD524 ACA10000 */  sw         $at, 0x0($a1)
    /* 874088 802BD528 ACAF0004 */  sw         $t7, 0x4($a1)
    /* 87408C 802BD52C 00CE7823 */  subu       $t7, $a2, $t6
    /* 874090 802BD530 AC8F000C */  sw         $t7, 0xC($a0)
    /* 874094 802BD534 3C010600 */  lui        $at, (0x6000000 >> 16)
    /* 874098 802BD538 AC810008 */  sw         $at, 0x8($a0)
    /* 87409C 802BD53C 010E7823 */  subu       $t7, $t0, $t6
    /* 8740A0 802BD540 ACAF000C */  sw         $t7, 0xC($a1)
    /* 8740A4 802BD544 ACA10008 */  sw         $at, 0x8($a1)
    /* 8740A8 802BD548 8D670010 */  lw         $a3, 0x10($t3)
    /* 8740AC 802BD54C 8D6F0018 */  lw         $t7, 0x18($t3)
    /* 8740B0 802BD550 20840008 */  addi       $a0, $a0, 0x8 /* handwritten instruction */
    /* 8740B4 802BD554 20A50008 */  addi       $a1, $a1, 0x8 /* handwritten instruction */
    /* 8740B8 802BD558 20840008 */  addi       $a0, $a0, 0x8 /* handwritten instruction */
    /* 8740BC 802BD55C 20A50008 */  addi       $a1, $a1, 0x8 /* handwritten instruction */
    /* 8740C0 802BD560 00EB3821 */  addu       $a3, $a3, $t3
    /* 8740C4 802BD564 01EB7821 */  addu       $t7, $t7, $t3
  .Lhd_code_802BD568:
    /* 8740C8 802BD568 10EF0008 */  beq        $a3, $t7, .Lhd_code_802BD58C
    /* 8740CC 802BD56C 00000000 */   nop
    /* 8740D0 802BD570 DCF00000 */  ld         $s0, 0x0($a3)
    /* 8740D4 802BD574 20E70008 */  addi       $a3, $a3, 0x8 /* handwritten instruction */
    /* 8740D8 802BD578 20C60008 */  addi       $a2, $a2, 0x8 /* handwritten instruction */
    /* 8740DC 802BD57C FCD0FFF8 */  sd         $s0, -0x8($a2)
    /* 8740E0 802BD580 21080008 */  addi       $t0, $t0, 0x8 /* handwritten instruction */
    /* 8740E4 802BD584 1000FFF8 */  b          .Lhd_code_802BD568
    /* 8740E8 802BD588 FD10FFF8 */   sd        $s0, -0x8($t0)
  .Lhd_code_802BD58C:
    /* 8740EC 802BD58C 240E0001 */  addiu      $t6, $zero, 0x1
  .Lhd_code_802BD590:
    /* 8740F0 802BD590 118D0086 */  beq        $t4, $t5, .Lhd_code_802BD7AC
    /* 8740F4 802BD594 00000000 */   nop
    /* 8740F8 802BD598 95870004 */  lhu        $a3, 0x4($t4)
    /* 8740FC 802BD59C 8D8F0000 */  lw         $t7, 0x0($t4)
    /* 874100 802BD5A0 218C0004 */  addi       $t4, $t4, 0x4 /* handwritten instruction */
    /* 874104 802BD5A4 20E7FFFF */  addi       $a3, $a3, -0x1 /* handwritten instruction */
  .Lhd_code_802BD5A8:
    /* 874108 802BD5A8 11E00017 */  beqz       $t7, .Lhd_code_802BD608
    /* 87410C 802BD5AC 00000000 */   nop
    /* 874110 802BD5B0 95900000 */  lhu        $s0, 0x0($t4)
    /* 874114 802BD5B4 91910002 */  lbu        $s1, 0x2($t4)
    /* 874118 802BD5B8 205300EC */  addi       $s3, $v0, 0xEC /* handwritten instruction */
    /* 87411C 802BD5BC 2210FFFF */  addi       $s0, $s0, -0x1 /* handwritten instruction */
    /* 874120 802BD5C0 02709821 */  addu       $s3, $s3, $s0
    /* 874124 802BD5C4 25EFFFFF */  addiu      $t7, $t7, -0x1
    /* 874128 802BD5C8 91920003 */  lbu        $s2, 0x3($t4)
    /* 87412C 802BD5CC 218C0004 */  addi       $t4, $t4, 0x4 /* handwritten instruction */
    /* 874130 802BD5D0 12200006 */  beqz       $s1, .Lhd_code_802BD5EC
    /* 874134 802BD5D4 92730000 */   lbu       $s3, 0x0($s3)
    /* 874138 802BD5D8 0272082A */  slt        $at, $s3, $s2
    /* 87413C 802BD5DC 1020FFF2 */  beqz       $at, .Lhd_code_802BD5A8
    /* 874140 802BD5E0 00000000 */   nop
    /* 874144 802BD5E4 10000004 */  b          .Lhd_code_802BD5F8
    /* 874148 802BD5E8 00000000 */   nop
  .Lhd_code_802BD5EC:
    /* 87414C 802BD5EC 0253082A */  slt        $at, $s2, $s3
    /* 874150 802BD5F0 1020FFED */  beqz       $at, .Lhd_code_802BD5A8
    /* 874154 802BD5F4 00000000 */   nop
  .Lhd_code_802BD5F8:
    /* 874158 802BD5F8 000F7880 */  sll        $t7, $t7, 2
    /* 87415C 802BD5FC 018F6021 */  addu       $t4, $t4, $t7
    /* 874160 802BD600 10000068 */  b          .Lhd_code_802BD7A4
    /* 874164 802BD604 218C0010 */   addi      $t4, $t4, 0x10 /* handwritten instruction */
  .Lhd_code_802BD608:
    /* 874168 802BD608 20530048 */  addi       $s3, $v0, 0x48 /* handwritten instruction */
    /* 87416C 802BD60C 00073840 */  sll        $a3, $a3, 1
    /* 874170 802BD610 02679821 */  addu       $s3, $s3, $a3
    /* 874174 802BD614 96740000 */  lhu        $s4, 0x0($s3)
    /* 874178 802BD618 1280002F */  beqz       $s4, .Lhd_code_802BD6D8
    /* 87417C 802BD61C 00000000 */   nop
    /* 874180 802BD620 20550068 */  addi       $s5, $v0, 0x68 /* handwritten instruction */
    /* 874184 802BD624 02A7A821 */  addu       $s5, $s5, $a3
    /* 874188 802BD628 96B60000 */  lhu        $s6, 0x0($s5)
    /* 87418C 802BD62C 24178AD0 */  addiu      $s7, $zero, -0x7530
    /* 874190 802BD630 22D60001 */  addi       $s6, $s6, 0x1 /* handwritten instruction */
    /* 874194 802BD634 02D60019 */  multu      $s6, $s6
    /* 874198 802BD638 A6B60000 */  sh         $s6, 0x0($s5)
    /* 87419C 802BD63C 0000B012 */  mflo       $s6
    /* 8741A0 802BD640 00000000 */  nop
    /* 8741A4 802BD644 00000000 */  nop
    /* 8741A8 802BD648 02D70019 */  multu      $s6, $s7
    /* 8741AC 802BD64C 8C570000 */  lw         $s7, 0x0($v0)
    /* 8741B0 802BD650 8EF8003C */  lw         $t8, 0x3C($s7)
    /* 8741B4 802BD654 02F8B821 */  addu       $s7, $s7, $t8
    /* 8741B8 802BD658 02E7B821 */  addu       $s7, $s7, $a3
    /* 8741BC 802BD65C 96F70000 */  lhu        $s7, 0x0($s7)
    /* 8741C0 802BD660 0000B012 */  mflo       $s6
    /* 8741C4 802BD664 0017BC00 */  sll        $s7, $s7, 16
    /* 8741C8 802BD668 0017B822 */  neg        $s7, $s7 /* handwritten instruction */
    /* 8741CC 802BD66C 02F6082A */  slt        $at, $s7, $s6
    /* 8741D0 802BD670 14200012 */  bnez       $at, .Lhd_code_802BD6BC
    /* 8741D4 802BD674 00000000 */   nop
    /* 8741D8 802BD678 A6600000 */  sh         $zero, 0x0($s3)
    /* 8741DC 802BD67C 0C0AFF47 */  jal        func_hd_code_802BFD1C
    /* 8741E0 802BD680 00073842 */   srl       $a3, $a3, 1
    /* 8741E4 802BD684 3C1C802F */  lui        $gp, %hi(D_hd_code_802E8BE4)
    /* 8741E8 802BD688 279C8BE4 */  addiu      $gp, $gp, %lo(D_hd_code_802E8BE4)
    /* 8741EC 802BD68C 2417000A */  addiu      $s7, $zero, 0xA
    /* 8741F0 802BD690 A3970000 */  sb         $s7, 0x0($gp)
    /* 8741F4 802BD694 3C1C802F */  lui        $gp, %hi(D_hd_code_802E8BE8)
    /* 8741F8 802BD698 279C8BE8 */  addiu      $gp, $gp, %lo(D_hd_code_802E8BE8)
    /* 8741FC 802BD69C 2417012C */  addiu      $s7, $zero, 0x12C
    /* 874200 802BD6A0 AF970000 */  sw         $s7, 0x0($gp)
    /* 874204 802BD6A4 00E2E021 */  addu       $gp, $a3, $v0
    /* 874208 802BD6A8 24170064 */  addiu      $s7, $zero, 0x64
    /* 87420C 802BD6AC 239C00EC */  addi       $gp, $gp, 0xEC /* handwritten instruction */
    /* 874210 802BD6B0 0C0AF617 */  jal        func_hd_code_802BD85C
    /* 874214 802BD6B4 A3970000 */   sb        $s7, 0x0($gp)
    /* 874218 802BD6B8 00073840 */  sll        $a3, $a3, 1
  .Lhd_code_802BD6BC:
    /* 87421C 802BD6BC 0C0AF95D */  jal        func_hd_code_802BE574
    /* 874220 802BD6C0 00000000 */   nop

    /* @recomp: building part with animation / toppling effect part 1/2 with no reflection */
    save_all_regs
    or $a0, $a2, 0 /* current Gfx pointer */
    or $a1, $a3, 0 /* building index */
    jal building_part_begin
    li $a2, 0 /* no reflection */
    sd $v0, 0x30($sp) /* store return of building_part_begin into the restoring stack place of $a2 */
    restore_all_regs

    /* 874224 802BD6C4 3C010104 */  lui        $at, (0x1040040 >> 16)
    /* 874228 802BD6C8 34210040 */  ori        $at, $at, (0x1040040 & 0xFFFF)
    /* 87422C 802BD6CC ACC10000 */  sw         $at, 0x0($a2)
    /* 874230 802BD6D0 ACD50004 */  sw         $s5, 0x4($a2)
    /* 874234 802BD6D4 20C60008 */  addi       $a2, $a2, 0x8 /* handwritten instruction */
  .Lhd_code_802BD6D8:
    /* 874238 802BD6D8 8D8F0000 */  lw         $t7, 0x0($t4)
    /* 87423C 802BD6DC 8D900004 */  lw         $s0, 0x4($t4)
    /* 874240 802BD6E0 01EB7821 */  addu       $t7, $t7, $t3
    /* 874244 802BD6E4 112A0005 */  beq        $t1, $t2, .Lhd_code_802BD6FC
    /* 874248 802BD6E8 020B8021 */   addu      $s0, $s0, $t3
    /* 87424C 802BD6EC 0C0AF88A */  jal        func_hd_code_802BE228
    /* 874250 802BD6F0 00000000 */   nop
    /* 874254 802BD6F4 10000008 */  b          .Lhd_code_802BD718
    /* 874258 802BD6F8 00000000 */   nop
  .Lhd_code_802BD6FC:
    /* 87425C 802BD6FC 11F00006 */  beq        $t7, $s0, .Lhd_code_802BD718
    /* 874260 802BD700 00000000 */   nop
    /* 874264 802BD704 DDF10000 */  ld         $s1, 0x0($t7)
    /* 874268 802BD708 21EF0008 */  addi       $t7, $t7, 0x8 /* handwritten instruction */
    /* 87426C 802BD70C 20C60008 */  addi       $a2, $a2, 0x8 /* handwritten instruction */
    /* 874270 802BD710 1000FFFA */  b          .Lhd_code_802BD6FC
    /* 874274 802BD714 FCD1FFF8 */   sd        $s1, -0x8($a2)
  .Lhd_code_802BD718:
      /* @recomp: building part with animation / toppling effect part 2/2 with reflection */
      /* normal rendering is in $a2, reflection rendering is in $t0 */
      save_all_regs
      jal building_part_end
      or $a0, $a2, 0 /* current Gfx pointer */
      sd $v0, 0x30($sp) /* store return of building_part_begin into the restoring stack place of $a2 */
      restore_all_regs
    /* 874278 802BD718 1280000A */  beqz       $s4, .Lhd_code_802BD744
    /* 87427C 802BD71C 3C01BD00 */   lui       $at, (0xBD000000 >> 16)
    /* 874280 802BD720 ACC10000 */  sw         $at, 0x0($a2)
    /* 874284 802BD724 24010000 */  addiu      $at, $zero, 0x0
    /* 874288 802BD728 ACC10004 */  sw         $at, 0x4($a2)


    save_all_regs
    or $a0, $t0, 0 /* current Gfx pointer */
    or $a1, $a3, 0 /* building index */
    jal building_part_begin
    li $a2, 1 /* reflection */
    sd $v0, 0x40($sp) /* store return of building_part_begin into the restoring stack place of $t0 */
    restore_all_regs

    /* 87428C 802BD72C 3C010104 */  lui        $at, (0x1040040 >> 16)
    /* 874290 802BD730 34210040 */  ori        $at, $at, (0x1040040 & 0xFFFF)
    /* 874294 802BD734 20C60008 */  addi       $a2, $a2, 0x8 /* handwritten instruction */
    /* 874298 802BD738 AD010000 */  sw         $at, 0x0($t0)
    /* 87429C 802BD73C AD150004 */  sw         $s5, 0x4($t0)
    /* 8742A0 802BD740 21080008 */  addi       $t0, $t0, 0x8 /* handwritten instruction */
  .Lhd_code_802BD744:
    /* 8742A4 802BD744 8D8F0008 */  lw         $t7, 0x8($t4)
    /* 8742A8 802BD748 8D90000C */  lw         $s0, 0xC($t4)
    /* 8742AC 802BD74C 01EB7821 */  addu       $t7, $t7, $t3
    /* 8742B0 802BD750 112A0005 */  beq        $t1, $t2, .Lhd_code_802BD768
    /* 8742B4 802BD754 020B8021 */   addu      $s0, $s0, $t3
    /* 8742B8 802BD758 0C0AF8F2 */  jal        func_hd_code_802BE3C8
    /* 8742BC 802BD75C 00000000 */   nop
    /* 8742C0 802BD760 10000008 */  b          .Lhd_code_802BD784
    /* 8742C4 802BD764 00000000 */   nop
  .Lhd_code_802BD768:
    /* 8742C8 802BD768 11F00006 */  beq        $t7, $s0, .Lhd_code_802BD784
    /* 8742CC 802BD76C 00000000 */   nop
    /* 8742D0 802BD770 DDF10000 */  ld         $s1, 0x0($t7)
    /* 8742D4 802BD774 21EF0008 */  addi       $t7, $t7, 0x8 /* handwritten instruction */
    /* 8742D8 802BD778 21080008 */  addi       $t0, $t0, 0x8 /* handwritten instruction */
    /* 8742DC 802BD77C 1000FFFA */  b          .Lhd_code_802BD768
    /* 8742E0 802BD780 FD11FFF8 */   sd        $s1, -0x8($t0)
  .Lhd_code_802BD784:
    /* 8742E4 802BD784 12800005 */  beqz       $s4, .Lhd_code_802BD79C
    /* 8742E8 802BD788 3C01BD00 */   lui       $at, (0xBD000000 >> 16)
    /* 8742EC 802BD78C AD010000 */  sw         $at, 0x0($t0)
    /* 8742F0 802BD790 24010000 */  addiu      $at, $zero, 0x0
    /* 8742F4 802BD794 AD010004 */  sw         $at, 0x4($t0)
    /* 8742F8 802BD798 21080008 */  addi       $t0, $t0, 0x8 /* handwritten instruction */

    // @recomp: Pop reflect matrix here
    save_all_regs
    jal building_part_end
    or $a0, $t0, 0 /* current Gfx pointer */
    sd $v0, 0x40($sp) /* store return of building_part_begin into the restoring stack place of $t0 */
    restore_all_regs

  .Lhd_code_802BD79C:
    /* 8742FC 802BD79C 15C00003 */  bnez       $t6, .Lhd_code_802BD7AC
    /* 874300 802BD7A0 218C0010 */   addi      $t4, $t4, 0x10 /* handwritten instruction */
  .Lhd_code_802BD7A4:
    /* 874304 802BD7A4 1000FF7A */  b          .Lhd_code_802BD590
    /* 874308 802BD7A8 240E0000 */   addiu     $t6, $zero, 0x0
  .Lhd_code_802BD7AC:
    /* 87430C 802BD7AC 3C01B800 */  lui        $at, (0xB8000000 >> 16)
    /* 874310 802BD7B0 ACC10000 */  sw         $at, 0x0($a2)
    /* 874314 802BD7B4 ACC00004 */  sw         $zero, 0x4($a2)
    /* 874318 802BD7B8 AD000004 */  sw         $zero, 0x4($t0)
    /* 87431C 802BD7BC AD010000 */  sw         $at, 0x0($t0)
    /* 874320 802BD7C0 8C470000 */  lw         $a3, 0x0($v0)
    /* 874324 802BD7C4 20C60008 */  addi       $a2, $a2, 0x8 /* handwritten instruction */
    /* 874328 802BD7C8 21080008 */  addi       $t0, $t0, 0x8 /* handwritten instruction */
    /* 87432C 802BD7CC 94E9000E */  lhu        $t1, 0xE($a3)
    /* 874330 802BD7D0 1120000A */  beqz       $t1, .Lhd_code_802BD7FC
    /* 874334 802BD7D4 3C01BD00 */   lui       $at, (0xBD000000 >> 16)
    /* 874338 802BD7D8 AC810000 */  sw         $at, 0x0($a0)
    /* 87433C 802BD7DC 24010000 */  addiu      $at, $zero, 0x0
    /* 874340 802BD7E0 AC810004 */  sw         $at, 0x4($a0)
    /* 874344 802BD7E4 3C01BD00 */  lui        $at, (0xBD000000 >> 16)
    /* 874348 802BD7E8 ACA10000 */  sw         $at, 0x0($a1)
    /* 87434C 802BD7EC 24010000 */  addiu      $at, $zero, 0x0
    /* 874350 802BD7F0 20840008 */  addi       $a0, $a0, 0x8 /* handwritten instruction */
    /* 874354 802BD7F4 ACA10004 */  sw         $at, 0x4($a1)
    /* 874358 802BD7F8 20A50008 */  addi       $a1, $a1, 0x8 /* handwritten instruction */
  .Lhd_code_802BD7FC:
    /* 87435C 802BD7FC 1000FEA7 */  b          .Lhd_code_802BD29C
    /* 874360 802BD800 204200FC */   addi      $v0, $v0, 0xFC /* handwritten instruction */
  .Lhd_code_802BD804:
    /* 874364 802BD804 0C0B04B8 */  jal        func_hd_code_802C12E0
    /* 874368 802BD808 00000000 */   nop
    /* 87436C 802BD80C 3C01B800 */  lui        $at, (0xB8000000 >> 16)
    /* 874370 802BD810 AC810000 */  sw         $at, 0x0($a0)
    /* 874374 802BD814 AC800004 */  sw         $zero, 0x4($a0)
    /* 874378 802BD818 ACA00004 */  sw         $zero, 0x4($a1)
    /* 87437C 802BD81C ACA10000 */  sw         $at, 0x0($a1)
    /* 874380 802BD820 DFBF0000 */  ld         $ra, 0x0($sp)
    /* 874384 802BD824 DFBE0050 */  ld         $fp, 0x50($sp)
    /* 874388 802BD828 DFBC0048 */  ld         $gp, 0x48($sp)
    /* 87438C 802BD82C DFB70040 */  ld         $s7, 0x40($sp)
    /* 874390 802BD830 DFB60038 */  ld         $s6, 0x38($sp)
    /* 874394 802BD834 DFB50030 */  ld         $s5, 0x30($sp)
    /* 874398 802BD838 DFB40028 */  ld         $s4, 0x28($sp)
    /* 87439C 802BD83C DFB30020 */  ld         $s3, 0x20($sp)
    /* 8743A0 802BD840 DFB20018 */  ld         $s2, 0x18($sp)
    /* 8743A4 802BD844 DFB10010 */  ld         $s1, 0x10($sp)
    /* 8743A8 802BD848 DFB00008 */  ld         $s0, 0x8($sp)
    /* 8743AC 802BD84C 20840008 */  addi       $a0, $a0, 0x8 /* handwritten instruction */
    /* 8743B0 802BD850 20A50008 */  addi       $a1, $a1, 0x8 /* handwritten instruction */
    /* 8743B4 802BD854 03E00008 */  jr         $ra
    /* 8743B8 802BD858 27BD0058 */   addiu     $sp, $sp, 0x58
endlabel func_hd_code_802BD1F8
