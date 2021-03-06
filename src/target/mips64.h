/***************************************************************************
 *   Copyright (C) 2008 by Spencer Oliver                                  *
 *   spen@spen-soft.co.uk                                                  *
 *                                                                         *
 *   Copyright (C) 2008 by David T.L. Wong                                 *
 *                                                                         *
 *   Copyright (C) 2011 by Drasko DRASKOVIC                                *
 *   drasko.draskovic@gmail.com                                            *
 *                                                                         *
 *   Copyright (C) 2013 by Donxue Zhang                                    *
 *   elta.era@gmail.com                                                    *
 *                                                                         *
 *   Copyright (C) 2013 by FengGao                                         *
 *   gf91597@gmail.com                                                     *
 *                                                                         *
 *   Copyright (C) 2013 by Jia Liu                                         *
 *   proljc@gmail.com                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef MIPS64_H
#define MIPS64_H

#include "target.h"
#include "mips64_pracc.h"

#define MIPS64_COMMON_MAGIC		0xD320D320

/**
 * Memory segments (32bit kernel mode addresses)
 * These are the traditional names used in the 32-bit universe.
 */
#define MIPS64_KUSEG			0xFFFFFFFF00000000
#define MIPS64_KSEG0			0xFFFFFFFF80000000
#define MIPS64_KSEG1			0xFFFFFFFFa0000000
#define MIPS64_KSEG2			0xFFFFFFFFc0000000
#define MIPS64_KSEG3			0xFFFFFFFFe0000000

/** Returns the kernel segment base of a given address */
#define MIPS64_KSEGX(a)		((a) & 0xFFFFFFFFe0000000)

/**
 * Memory segments (64bit kernel mode addresses)
 * These are the traditional names used in the 64-bit universe.
 */

#define XUSEG                   0x0000000000000000
#define XSSEG                   0x4000000000000000
#define XKPHYS                  0x8000000000000000
#define XKSEG                   0xc000000000000000

/** CP0 CONFIG regites fields */
#define MIPS64_CONFIG0_KU_SHIFT 25
#define MIPS64_CONFIG0_KU_MASK (0x7 << MIPS64_CONFIG0_KU_SHIFT)

#define MIPS64_CONFIG0_K0_SHIFT 0
#define MIPS64_CONFIG0_K0_MASK (0x7 << MIPS64_CONFIG0_K0_SHIFT)

#define MIPS64_CONFIG0_K23_SHIFT 28
#define MIPS64_CONFIG0_K23_MASK (0x7 << MIPS64_CONFIG0_K23_SHIFT)

#define MIPS64_CONFIG0_AR_SHIFT 10
#define MIPS64_CONFIG0_AR_MASK (0x7 << MIPS64_CONFIG0_AR_SHIFT)

#define MIPS64_CONFIG1_DL_SHIFT 10
#define MIPS64_CONFIG1_DL_MASK (0x7 << MIPS64_CONFIG1_DL_SHIFT)

#define MIPS64_ARCH_REL1 0x0
#define MIPS64_ARCH_REL2 0x1

/* offsets into mips32 core register cache */
enum {
	MIPS64_PC = 37,
	MIPS64NUMCOREREGS
};

enum mips64_isa_mode {
	MIPS64_ISA_MIPS64 = 0,
	MIPS64_ISA_MIPS32 = 1,
};

struct mips64_comparator {
	int used;
	uint64_t bp_value;
	uint64_t reg_address;
};

struct mips64_common {
	uint32_t common_magic;
	void *arch_info;
	struct reg_cache *core_cache;
	struct mips_ejtag ejtag_info;
	uint64_t core_regs[MIPS64NUMCOREREGS];
	enum mips64_isa_mode isa_mode;

	/* working area for fastdata access */
	struct working_area *fast_data_area;

	int bp_scanned;
	int num_inst_bpoints;
	int num_data_bpoints;
	int num_inst_bpoints_avail;
	int num_data_bpoints_avail;
	struct mips64_comparator *inst_break_list;
	struct mips64_comparator *data_break_list;

	/* register cache to processor synchronization */
	int (*read_core_reg)(struct target *target, int num);
	int (*write_core_reg)(struct target *target, int num);
};

static inline struct mips64_common *
target_to_mips64(struct target *target)
{
	return target->arch_info;
}

struct mips64_core_reg {
	uint32_t num;
	struct target *target;
	struct mips64_common *mips64_common;
};

struct mips64_algorithm {
	int common_magic;
	enum mips64_isa_mode isa_mode;
};

#define MIPS64_OP_ADDIU 0x21
#define MIPS64_OP_DADDIU 0x2D
#define MIPS64_OP_ANDI	0x0C
#define MIPS64_OP_BEQ	0x04
#define MIPS64_OP_BGTZ	0x07
#define MIPS64_OP_BNE	0x05
#define MIPS64_OP_ADDI	0x08
#define MIPS64_OP_DADDI	0x18
#define MIPS64_OP_AND	0x24
#define MIPS64_OP_CACHE	0x2F
#define MIPS64_OP_COP0	0x10
#define MIPS64_OP_JR	0x08
#define MIPS64_OP_LUI	0x0F
#define MIPS64_OP_LW	0x23
#define MIPS64_OP_LD	0x37
#define MIPS64_OP_LBU	0x24
#define MIPS64_OP_LHU	0x25
#define MIPS64_OP_MFHI	0x10
#define MIPS64_OP_MTHI	0x11
#define MIPS64_OP_MFLO	0x12
#define MIPS64_OP_MTLO	0x13
#define MIPS64_OP_RDHWR 0x3B
#define MIPS64_OP_SB	0x28
#define MIPS64_OP_SH	0x29
#define MIPS64_OP_SW	0x2B
#define MIPS64_OP_SD	0x3F
#define MIPS64_OP_ORI	0x0D
#define MIPS64_OP_XORI	0x0E
#define MIPS64_OP_XOR	0x26
#define MIPS64_OP_SLTU  0x2B
#define MIPS64_OP_SRL	0x03
#define MIPS64_OP_DSLL	0x38
#define MIPS64_OP_DSRL	0x3a
#define MIPS64_OP_SYNCI	0x1F

#define MIPS64_OP_REGIMM	0x01
#define MIPS64_OP_SDBBP		0x3F
#define MIPS64_OP_SPECIAL	0x00
#define MIPS64_OP_SPECIAL2	0x07
#define MIPS64_OP_SPECIAL3	0x1F

#define MIPS64_COP0_MF	0x00
#define MIPS64_COP0_MT	0x04
#define MIPS64_COP0_DMF	0x01
#define MIPS64_COP0_DMT	0x05

#define MIPS64_R_INST(opcode, rs, rt, rd, shamt, funct) \
	(((opcode) << 26) | ((rs) << 21) | ((rt) << 16) | ((rd) << 11) | ((shamt) << 6) | (funct))
#define MIPS64_I_INST(opcode, rs, rt, immd) \
	(((opcode) << 26) | ((rs) << 21) | ((rt) << 16) | (immd))
#define MIPS64_J_INST(opcode, addr)	(((opcode) << 26) | (addr))

#define MIPS64_NOP						0
#define MIPS64_DADDI(tar, src, val)		MIPS64_I_INST(MIPS64_OP_ADDI, src, tar, val)
#define MIPS64_DADDU(dst, src, tar)		MIPS64_R_INST(MIPS64_OP_SPECIAL, src, tar, dst, 0, MIPS64_OP_DADDIU)
#define MIPS64_ADDI(tar, src, val)		MIPS64_I_INST(MIPS64_OP_ADDI, src, tar, val)
#define MIPS64_ADDU(dst, src, tar)		MIPS64_R_INST(MIPS64_OP_SPECIAL, src, tar, dst, 0, MIPS64_OP_ADDIU)
#define MIPS64_AND(reg, off, val)		MIPS64_R_INST(0, off, val, reg, 0, MIPS64_OP_AND)
#define MIPS64_ANDI(tar, src, val)		MIPS64_I_INST(MIPS64_OP_ANDI, src, tar, val)
#define MIPS64_B(off)				MIPS64_BEQ(0, 0, off)
#define MIPS64_BEQ(src, tar, off)		MIPS64_I_INST(MIPS64_OP_BEQ, src, tar, off)
#define MIPS64_BGTZ(reg, off)			MIPS64_I_INST(MIPS64_OP_BGTZ, reg, 0, off)
#define MIPS64_BNE(src, tar, off)		MIPS64_I_INST(MIPS64_OP_BNE, src, tar, off)
#define MIPS64_CACHE(op, off, base)		MIPS64_I_INST(MIPS64_OP_CACHE, base, op, off)
#define MIPS64_JR(reg)				MIPS64_R_INST(0, reg, 0, 0, 0, MIPS64_OP_JR)
#define MIPS64_MFC0(gpr, cpr, sel)		MIPS64_R_INST(MIPS64_OP_COP0, MIPS64_COP0_MF, gpr, cpr, 0, sel)
#define MIPS64_MTC0(gpr, cpr, sel)		MIPS64_R_INST(MIPS64_OP_COP0, MIPS64_COP0_MT, gpr, cpr, 0, sel)
#define MIPS64_DMFC0(gpr, cpr, sel)		MIPS64_R_INST(MIPS64_OP_COP0, MIPS64_COP0_DMF, gpr, cpr, 0, sel)
#define MIPS64_DMTC0(gpr, cpr, sel)		MIPS64_R_INST(MIPS64_OP_COP0, MIPS64_COP0_DMT, gpr, cpr, 0, sel)
#define MIPS64_LBU(reg, off, base)		MIPS64_I_INST(MIPS64_OP_LBU, base, reg, off)
#define MIPS64_LHU(reg, off, base)		MIPS64_I_INST(MIPS64_OP_LHU, base, reg, off)
#define MIPS64_LWU(reg, off, base)		MIPS64_I_INST(MIPS64_OP_LWU, base, reg, off)
#define MIPS64_LUI(reg, val)			MIPS64_I_INST(MIPS64_OP_LUI, 0, reg, val)
#define MIPS64_LW(reg, off, base)		MIPS64_I_INST(MIPS64_OP_LW, base, reg, off)
#define MIPS64_LD(reg, off, base)		MIPS64_I_INST(MIPS64_OP_LD, base, reg, off)
#define MIPS64_DMFLO(reg)			MIPS64_R_INST(0, 0, 0, reg, 0, MIPS64_OP_MFLO)
#define MIPS64_DMFHI(reg)			MIPS64_R_INST(0, 0, 0, reg, 0, MIPS64_OP_MFHI)
#define MIPS64_DMTLO(reg)			MIPS64_R_INST(0, reg, 0, 0, 0, MIPS64_OP_MTLO)
#define MIPS64_DMTHI(reg)			MIPS64_R_INST(0, reg, 0, 0, 0, MIPS64_OP_MTHI)
#define MIPS64_ORI(tar, src, val)		MIPS64_I_INST(MIPS64_OP_ORI, src, tar, val)
#define MIPS64_XORI(tar, src, val)		MIPS64_I_INST(MIPS64_OP_XORI, src, tar, val)
#define MIPS64_RDHWR(tar, dst)			MIPS64_R_INST(MIPS64_OP_SPECIAL3, 0, tar, dst, 0, MIPS64_OP_RDHWR)
#define MIPS64_SB(reg, off, base)		MIPS64_I_INST(MIPS64_OP_SB, base, reg, off)
#define MIPS64_SH(reg, off, base)		MIPS64_I_INST(MIPS64_OP_SH, base, reg, off)
#define MIPS64_SW(reg, off, base)		MIPS64_I_INST(MIPS64_OP_SW, base, reg, off)
#define MIPS64_SD(reg, off, base)		MIPS64_I_INST(MIPS64_OP_SD, base, reg, off)
#define MIPS64_XOR(reg, val1, val2)		MIPS64_R_INST(0, val1, val2, reg, 0, MIPS64_OP_XOR)
#define MIPS64_SRL(reg, src, off)		MIPS64_R_INST(0, 0, src, reg, off, MIPS64_OP_SRL)
#define MIPS64_DSLL(dst, src, sa)		MIPS64_R_INST(0, 0, src, dst, sa, MIPS64_OP_DSLL)
#define MIPS64_DSRL(dst, src, sa)		MIPS64_R_INST(0, 0, src, dst, sa, MIPS64_OP_DSRL)
#define MIPS64_SLTU(dst, src, tar)		MIPS64_R_INST(MIPS64_OP_SPECIAL, src, tar, dst, 0, MIPS64_OP_SLTU)
#define MIPS64_SYNCI(off, base)			MIPS64_I_INST(MIPS64_OP_REGIMM, base, MIPS64_OP_SYNCI, off)

#define MIPS64_SYNC			0xF
#define MIPS64_SYNCI_STEP	0x1	/* reg num od address step size to be used with synci instruction */

/**
 * Cache operations definietions
 * Operation field is 5 bits long :
 * 1) bits 1..0 hold cache type
 * 2) bits 4..2 hold operation code
 */
#define MIPS64_CACHE_D_HIT_WRITEBACK ((0x1 << 0) | (0x6 << 2))
#define MIPS64_CACHE_I_HIT_INVALIDATE ((0x0 << 0) | (0x4 << 2))

/* ejtag specific instructions */
#define MIPS64_DRET					0x4200001F
#define MIPS64_SDBBP				0x7000003F	/* MIPS64_J_INST(MIPS64_OP_SPECIAL2, MIPS64_OP_SDBBP) */
#define MIPS32_SDBBP				0x7000003F
#define MIPS16_SDBBP				0xE801

extern const struct command_registration mips64_command_handlers[];

int mips64_arch_state(struct target *target);

int mips64_init_arch_info(struct target *target,
		struct mips64_common *mips64, struct jtag_tap *tap);

int mips64_restore_context(struct target *target);
int mips64_save_context(struct target *target);

struct reg_cache *mips64_build_reg_cache(struct target *target);

int mips64_run_algorithm(struct target *target,
		int num_mem_params, struct mem_param *mem_params,
		int num_reg_params, struct reg_param *reg_params,
		target_ulong entry_point, target_ulong exit_point,
		int timeout_ms, void *arch_info);

int mips64_configure_break_unit(struct target *target);

int mips64_enable_interrupts(struct target *target, int enable);

int mips64_examine(struct target *target);

int mips64_register_commands(struct command_context *cmd_ctx);

int mips64_get_gdb_reg_list(struct target *target, struct reg **reg_list[],
		int *reg_list_size, enum target_register_class reg_class);
int mips64_checksum_memory(struct target *target, target_ulong address,
		uint32_t count, uint32_t *checksum);
int mips64_blank_check_memory(struct target *target,
		target_ulong address, uint32_t count, uint32_t *blank);

#endif	/*MIPS64_H*/