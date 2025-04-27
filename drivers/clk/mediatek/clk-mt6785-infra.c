// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2025 Dinolek <git@dinolek.me>
 */

#include <dt-bindings/clock/mediatek,mt6785-clk.h>
#include <dt-bindings/reset/mediatek,mt6785-resets.h>
#include <linux/platform_device.h>

#include "clk-gate.h"
#include "clk-mtk.h"

static const struct mtk_gate_regs infra0_cg_regs = {
	.sta_ofs = 0x90,
	.clr_ofs = 0x84,
	.set_ofs = 0x80,
};

static const struct mtk_gate_regs infra1_cg_regs = {
	.sta_ofs = 0x94,
	.clr_ofs = 0x8c,
	.set_ofs = 0x88,
};

static const struct mtk_gate_regs infra2_cg_regs = {
	.sta_ofs = 0xac,
	.clr_ofs = 0xa8,
	.set_ofs = 0xa4,
};

static const struct mtk_gate_regs infra3_cg_regs = {
	.sta_ofs = 0xc8,
	.clr_ofs = 0xc4,
	.set_ofs = 0xc0,
};

#define GATE_INFRA0(_id, _name, _parent, _shift)               \
	GATE_MTK(_id, _name, _parent, &infra0_cg_regs, _shift, \
		 &mtk_clk_gate_ops_setclr)

#define GATE_INFRA1(_id, _name, _parent, _shift)               \
	GATE_MTK(_id, _name, _parent, &infra1_cg_regs, _shift, \
		 &mtk_clk_gate_ops_setclr)

#define GATE_INFRA2(_id, _name, _parent, _shift)               \
	GATE_MTK(_id, _name, _parent, &infra2_cg_regs, _shift, \
		 &mtk_clk_gate_ops_setclr)

#define GATE_INFRA2_FLAGS(_id, _name, _parent, _shift, _flag)        \
	GATE_MTK_FLAGS(_id, _name, _parent, &infra2_cg_regs, _shift, \
		       &mtk_clk_gate_ops_setclr, _flag)

#define GATE_INFRA3(_id, _name, _parent, _shift)               \
	GATE_MTK(_id, _name, _parent, &infra3_cg_regs, _shift, \
		 &mtk_clk_gate_ops_setclr)

#define GATE_INFRA3_FLAGS(_id, _name, _parent, _shift, _flag)        \
	GATE_MTK_FLAGS(_id, _name, _parent, &infra3_cg_regs, _shift, \
		       &mtk_clk_gate_ops_setclr, _flag)

static const struct mtk_gate infra_clks[] = {
	GATE_INFRA0(CLK_INFRA_PMIC_TMR, "infra_pmic_tmr", "top_pwrap_ulposc", 0),
	GATE_INFRA0(CLK_INFRA_PMIC_AP, "infra_pmic_ap", "top_pwrap_ulposc", 1),
	GATE_INFRA0(CLK_INFRA_PMIC_MD, "infra_pmic_md", "top_pwrap_ulposc", 2),
	GATE_INFRA0(CLK_INFRA_PMIC_CONN, "infra_pmic_conn", "top_pwrap_ulposc", 3),
	GATE_INFRA0(CLK_INFRA_SCP, "infra_scp", "top_scp", 4),
	GATE_INFRA0(CLK_INFRA_SEJ, "infra_sej", "clk26m", 5),
	GATE_INFRA0(CLK_INFRA_APXGPT, "infra_apxgpt", "top_axi", 6),
	GATE_INFRA0(CLK_INFRA_ICUSB, "infra_icusb", "top_axi", 8),
	GATE_INFRA0(CLK_INFRA_GCE, "infra_gce", "top_axi", 9),
	GATE_INFRA0(CLK_INFRA_THERM, "infra_therm", "top_axi", 10),
	GATE_INFRA0(CLK_INFRA_I2C_AP, "infra_i2c_ap", "top_i2c", 11),
	GATE_INFRA0(CLK_INFRA_I2C_CCU, "infra_i2c_ccu", "top_i2c", 12),
	GATE_INFRA0(CLK_INFRA_I2C_SSPM, "infra_i2c_sspm", "top_i2c", 13),
	GATE_INFRA0(CLK_INFRA_I2C_RSV, "infra_i2c_rsv", "top_i2c", 14),
	GATE_INFRA0(CLK_INFRA_PWM_HCLK, "infra_pwm_hclk", "top_pwm", 15),
	GATE_INFRA0(CLK_INFRA_PWM1, "infra_pwm1", "top_pwm", 16),
	GATE_INFRA0(CLK_INFRA_PWM2, "infra_pwm2", "top_pwm", 17),
	GATE_INFRA0(CLK_INFRA_PWM3, "infra_pwm3", "top_pwm", 18),
	GATE_INFRA0(CLK_INFRA_PWM4, "infra_pwm4", "top_pwm", 19),
	GATE_INFRA0(CLK_INFRA_PWM, "infra_pwm", "top_pwm", 21),
	GATE_INFRA0(CLK_INFRA_UART0, "infra_uart0", "top_uart", 22),
	GATE_INFRA0(CLK_INFRA_UART1, "infra_uart1", "top_uart", 23),
	GATE_INFRA0(CLK_INFRA_UART2, "infra_uart2", "top_uart", 24),
	GATE_INFRA0(CLK_INFRA_UART3, "infra_uart3", "top_uart", 25),
	GATE_INFRA0(CLK_INFRA_GCE_26M, "infra_gce_26m", "top_axi", 27),
	GATE_INFRA0(CLK_INFRA_CQ_DMA_FPC, "infra_cq_dma_fpc", "top_axi", 28),
	GATE_INFRA0(CLK_INFRA_BTIF, "infra_btif", "top_axi", 31),

	GATE_INFRA1(CLK_INFRA_SPI0, "infra_spi0", "top_spi", 1),
	GATE_INFRA1(CLK_INFRA_MSDC0, "infra_msdc0", "top_msdc50_0_hclk", 2),
	GATE_INFRA1(CLK_INFRA_MSDC1, "infra_msdc1", "top_axi", 4),
	GATE_INFRA1(CLK_INFRA_MSDC0_SRC, "infra_msdc0_src", "top_msdc50_0", 6),
	GATE_INFRA1(CLK_INFRA_DVFSRC, "infra_dvfsrc", "clk26m", 7),
	GATE_INFRA1(CLK_INFRA_GCPU, "infra_gcpu", "top_axi", 8),
	GATE_INFRA1(CLK_INFRA_TRNG, "infra_trng", "top_axi", 9),
	GATE_INFRA1(CLK_INFRA_AUXADC, "infra_auxadc", "clk26m", 10),
	GATE_INFRA1(CLK_INFRA_CPUM, "infra_cpum", "top_axi", 11),
	GATE_INFRA1(CLK_INFRA_CCIF1_AP, "infra_ccif1_ap", "top_axi", 12),
	GATE_INFRA1(CLK_INFRA_CCIF1_MD, "infra_ccif1_md", "top_axi", 13),
	GATE_INFRA1(CLK_INFRA_AUXADC_MD, "infra_auxadc_md", "clk26m", 14),
	GATE_INFRA1(CLK_INFRA_MSDC1_SRC, "infra_msdc1_src", "top_msdc30_1", 16),
	GATE_INFRA1(CLK_INFRA_AP_DMA, "infra_ap_dma", "top_axi", 18),
	GATE_INFRA1(CLK_INFRA_XIU, "infra_xiu", "top_axi", 19),
	GATE_INFRA1(CLK_INFRA_DEVICE_APC, "infra_device_apc", "top_axi", 20),
	GATE_INFRA1(CLK_INFRA_CCIF_AP, "infra_ccif_ap", "top_axi", 23),
	GATE_INFRA1(CLK_INFRA_DEBUGSYS, "infra_debugsys", "top_axi", 24),
	GATE_INFRA1(CLK_INFRA_AUDIO, "infra_audio", "top_axi", 25),
	GATE_INFRA1(CLK_INFRA_CCIF_MD, "infra_ccif_md", "top_axi", 26),
	GATE_INFRA1(CLK_INFRA_DXCC_SEC_CORE, "infra_dxcc_sec_core", "top_dxcc", 27),
	GATE_INFRA1(CLK_INFRA_DXCC, "infra_dxcc", "top_dxcc", 28),
	GATE_INFRA1(CLK_INFRA_DEVMPU, "infra_devmpu", "top_axi", 30),
	GATE_INFRA1(CLK_INFRA_DRAMC_26M, "infra_dramc_26m", "clk26m", 31),

	GATE_INFRA2(CLK_INFRA_IRTX, "infra_irtx", "clk26m", 0),
	GATE_INFRA2(CLK_INFRA_SSUSB, "infra_ssusb", "top_ssusb", 1),
	GATE_INFRA2(CLK_INFRA_DISP_PWM, "infra_disppwm", "top_axi", 2),
	GATE_INFRA2(CLK_INFRA_AUDIO_26M, "infra_audio_26m", "clk26m", 4),
	GATE_INFRA2(CLK_INFRA_SPI1, "infra_spi1", "top_spi", 6),
	GATE_INFRA2(CLK_INFRA_I2C4, "infra_i2c4", "top_i2c", 7),
	GATE_INFRA2(CLK_INFRA_MODEM_TEMP_SHARE, "infra_md_tmp_share", "clk26m", 8),
	GATE_INFRA2(CLK_INFRA_SPI2, "infra_spi2", "top_spi", 9),
	GATE_INFRA2(CLK_INFRA_SPI3, "infra_spi3", "top_spi", 10),
	GATE_INFRA2(CLK_INFRA_UNIPRO_SYS, "infra_unipro_sys", "top_ufs", 11),
	GATE_INFRA2(CLK_INFRA_UNIPRO_TICK, "infra_unipro_tick", "top_ufs", 12),
	GATE_INFRA2(CLK_INFRA_UFS_MP_SAP, "infra_ufs_mp_sap", "top_ufs", 13),
	GATE_INFRA2(CLK_INFRA_MD32, "infra_md32", "top_axi", 14),
	GATE_INFRA2_FLAGS(CLK_INFRA_SSPM, "infra_sspm", "top_sspm", 15, CLK_IS_CRITICAL),
	GATE_INFRA2(CLK_INFRA_UNIPRO_MBIST, "infra_unipro_mbist", "top_axi", 16),
	GATE_INFRA2_FLAGS(CLK_INFRA_SSPM_BUS_HCLK, "infra_sspm_bus_hclk",
			  "top_axi", 17, CLK_IS_CRITICAL),
	GATE_INFRA2(CLK_INFRA_I2C5, "infra_i2c5", "top_i2c", 18),
	GATE_INFRA2(CLK_INFRA_I2C5_ARBITER, "infra_i2c5_arbiter", "top_i2c", 19),
	GATE_INFRA2(CLK_INFRA_I2C5_IMM, "infra_i2c5_imm", "top_i2c", 20),
	GATE_INFRA2(CLK_INFRA_I2C1_ARBITER, "infra_i2c1_arbiter", "top_i2c", 21),
	GATE_INFRA2(CLK_INFRA_I2C1_IMM, "infra_i2c1_imm", "top_i2c", 22),
	GATE_INFRA2(CLK_INFRA_I2C2_ARBITER, "infra_i2c2_arbiter", "top_i2c", 23),
	GATE_INFRA2(CLK_INFRA_I2C2_IMM, "infra_i2c2_imm", "top_i2c", 24),
	GATE_INFRA2(CLK_INFRA_SPI4, "infra_spi4", "top_spi", 25),
	GATE_INFRA2(CLK_INFRA_SPI5, "infra_spi5", "top_spi", 26),
	GATE_INFRA2(CLK_INFRA_CQ_DMA, "infra_cq_dma", "top_axi", 27),
	GATE_INFRA2(CLK_INFRA_UFS, "infra_ufs", "top_ufs", 28),
	GATE_INFRA2(CLK_INFRA_AES_UFSFDE, "infra_aes_ufsfde", "top_aes_ufsfde", 29),
	GATE_INFRA2(CLK_INFRA_UFS_TICK, "infra_ufs_tick", "top_ufs", 30),
	GATE_INFRA2(CLK_INFRA_SSUSB_XHCI, "infra_ssusb_xhci", "top_ssusb_xhci", 31),

	GATE_INFRA3(CLK_INFRA_MSDC0_SELF, "infra_msdc0_self", "top_msdc50_0", 0),
	GATE_INFRA3(CLK_INFRA_MSDC1_SELF, "infra_msdc1_self", "top_msdc50_0", 1),
	GATE_INFRA3_FLAGS(CLK_INFRA_SSPM_26M_SELF, "infra_sspm_26m_self",
			  "clk26m", 3, CLK_IS_CRITICAL),
	GATE_INFRA3_FLAGS(CLK_INFRA_SSPM_32K_SELF, "infra_sspm_32k_self",
			  "clk32k", 4, CLK_IS_CRITICAL),
	GATE_INFRA3(CLK_INFRA_UFS_AXI, "infra_ufs_axi", "top_axi", 5),
	GATE_INFRA3(CLK_INFRA_I2C6, "infra_i2c6", "top_i2c", 6),
	GATE_INFRA3(CLK_INFRA_AP_MSDC0, "infra_ap_msdc0", "top_msdc50_0_hclk", 7),
	GATE_INFRA3(CLK_INFRA_MD_MSDC0, "infra_md_msdc0", "top_msdc50_0_hclk", 8),
	GATE_INFRA3(CLK_INFRA_ADSP_26M, "infra_adsp_26m", "top_axi", 12),
	GATE_INFRA3(CLK_INFRA_ADSP_32K, "infra_adsp_32k", "top_axi", 13),
	GATE_INFRA3(CLK_INFRA_CCIF2_AP, "infra_ccif2_ap", "top_axi", 16),
	GATE_INFRA3(CLK_INFRA_CCIF2_MD, "infra_ccif2_md", "top_axi", 17),
	GATE_INFRA3(CLK_INFRA_CCIF3_AP, "infra_ccif3_ap", "top_axi", 18),
	GATE_INFRA3(CLK_INFRA_CCIF3_MD, "infra_ccif3_md", "top_axi", 19),
	GATE_INFRA3(CLK_INFRA_SEJ_13M, "infra_sej_13m", "clk13m", 20),
	GATE_INFRA3(CLK_INFRA_AES, "infra_aes", "top_axi", 21),
	GATE_INFRA3(CLK_INFRA_I2C7, "infra_i2c7", "top_i2c", 22),
	GATE_INFRA3(CLK_INFRA_I2C8, "infra_i2c8", "top_i2c", 23),
	GATE_INFRA3(CLK_INFRA_BIST2FPC, "infra_bist2fpc", "top_msdc50_0", 24),
	GATE_INFRA3_FLAGS(CLK_INFRA_DEVICE_APC_SYNC, "infra_device_apc_sync",
			  "top_axi", 25, CLK_IS_CRITICAL),
	GATE_INFRA3(CLK_INFRA_DPMAIF, "infra_dpmaif", "top_dpmaif", 26),
	GATE_INFRA3(CLK_INFRA_ADSP, "infra_adsp", "top_adsp", 27),
	GATE_INFRA3(CLK_INFRA_CCIF4_AP, "infra_ccif4_ap", "top_axi", 28),
	GATE_INFRA3(CLK_INFRA_CCIF4_MD, "infra_ccif4_md", "top_axi", 29),
	GATE_INFRA3(CLK_INFRA_SPI6, "infra_spi6", "top_spi", 30),
	GATE_INFRA3(CLK_INFRA_SPI7, "infra_spi7", "top_spi", 31),
};

static u16 infra_rst_ofs[] = {
	INFRA_RST0_SET_OFFSET,
	INFRA_RST1_SET_OFFSET,
	INFRA_RST2_SET_OFFSET,
	INFRA_RST3_SET_OFFSET,
	INFRA_RST4_SET_OFFSET,
};

static u16 infra_idx_map[] = {
	[MT6785_INFRA_THERM_CTRL_RST] = 0 * RST_NR_PER_BANK + 0,
	[MT6785_INFRA_UFS_HCI_RST] = 1 * RST_NR_PER_BANK + 15,
	[MT6785_INFRA_PMIC_WRAP_RST] = 2 * RST_NR_PER_BANK + 0,
	[MT6785_INFRA_UFS_UNIPRO_RST] = 2 * RST_NR_PER_BANK + 7,
	[MT6785_INFRA_SVS_RST] = 3 * RST_NR_PER_BANK + 5,
	[MT6785_INFRA_UFS_AES_RST] = 3 * RST_NR_PER_BANK + 21,
};

static const struct mtk_clk_rst_desc clk_rst_desc = {
	.version = MTK_RST_SET_CLR,
	.rst_bank_ofs = infra_rst_ofs,
	.rst_bank_nr = ARRAY_SIZE(infra_rst_ofs),
	.rst_idx_map = infra_idx_map,
	.rst_idx_map_nr = ARRAY_SIZE(infra_idx_map),
};

static const struct mtk_clk_desc infra_desc = {
	.clks = infra_clks,
	.num_clks = ARRAY_SIZE(infra_clks),
	.rst_desc = &clk_rst_desc,
};

static const struct of_device_id of_match_clk_mt6785_infra[] = {
	{ .compatible = "mediatek,mt6785-infracfg", .data = &infra_desc },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, of_match_clk_mt6785_infra);

static struct platform_driver clk_mt6785_infra_drv = {
	.probe = mtk_clk_simple_probe,
	.remove = mtk_clk_simple_remove,
	.driver = {
		.name = "clk-mt6785-infra",
		.of_match_table = of_match_clk_mt6785_infra,
	},
};
module_platform_driver(clk_mt6785_infra_drv);

MODULE_DESCRIPTION("MediaTek MT6785 Infrastructure clocks driver");
MODULE_LICENSE("GPL");
