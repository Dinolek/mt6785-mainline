// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2025 Dinolek <git@dinolek.me>
 */

#include <dt-bindings/clock/mediatek,mt6785-clk.h>
#include <linux/platform_device.h>

#include "clk-gate.h"
#include "clk-mtk.h"
#include "clk-mux.h"

#define CLK_TOP_CFG_UPDATE0 0x4
#define CLK_TOP_CFG_ENTRIES 32

#define CLK_TOP_CFG_0 0x20

#define TOP_MUX_GATE_FLAGS(_mux, _id, _name, _parents, _shift, _width, _gate, \
			   _upd, _flags)                                      \
	MUX_GATE_CLR_SET_UPD_FLAGS(                                           \
		_id, _name, _parents,                                         \
		(CLK_TOP_CFG_0 + 0x10 * (_mux)),                              \
		(CLK_TOP_CFG_0 + 0x10 * (_mux) + 0x4),                        \
		(CLK_TOP_CFG_0 + 0x10 * (_mux) + 0x8),                        \
		_shift, _width, _gate,                                        \
		(CLK_TOP_CFG_UPDATE0 + 0x4 * ((_upd) / CLK_TOP_CFG_ENTRIES)), \
		((_upd) % CLK_TOP_CFG_ENTRIES), _flags                        \
	)

#define TOP_MUX_GATE(_mux, _id, _name, _parents, _shift, _width, _gate, _upd) \
	TOP_MUX_GATE_FLAGS(_mux, _id, _name, _parents, _shift, _width, _gate, \
			   _upd, CLK_SET_RATE_PARENT)

#define TOP_MUX_GATE_CRITICAL(_mux, _id, _name, _parents, _shift, _width,     \
			      _gate, _upd)                                    \
	TOP_MUX_GATE_FLAGS(_mux, _id, _name, _parents, _shift, _width, _gate, \
			   _upd, CLK_IS_CRITICAL | CLK_SET_RATE_PARENT)

static DEFINE_SPINLOCK(mt6785_clk_lock);

static const struct mtk_fixed_clk top_fixed_clks[] = {
	FIXED_CLK(CLK_TOP_ULPOSC, "ulposc", NULL, 260000000),
};

static const struct mtk_fixed_factor top_factor_clks[] = {
	FACTOR_FLAGS(CLK_TOP_MAINPLL_D2, "mainpll_d2", "mainpll", 1, 2, 0),
	FACTOR_FLAGS(CLK_TOP_MAINPLL_D2_D2, "mainpll_d2_d2", "mainpll_d2", 1, 2, 0),
	FACTOR_FLAGS(CLK_TOP_MAINPLL_D2_D4, "mainpll_d2_d4", "mainpll_d2", 1, 4, 0),
	FACTOR_FLAGS(CLK_TOP_MAINPLL_D2_D8, "mainpll_d2_d8", "mainpll_d2", 1, 8, 0),
	FACTOR_FLAGS(CLK_TOP_MAINPLL_D2_D16, "mainpll_d2_d16", "mainpll_d2", 1, 16, 0),

	FACTOR_FLAGS(CLK_TOP_MAINPLL_D3, "mainpll_d3", "mainpll", 1, 3, 0),
	FACTOR_FLAGS(CLK_TOP_MAINPLL_D3_D2, "mainpll_d3_d2", "mainpll_d3", 1, 2, 0),
	FACTOR_FLAGS(CLK_TOP_MAINPLL_D3_D4, "mainpll_d3_d4", "mainpll_d3", 1, 4, 0),

	FACTOR_FLAGS(CLK_TOP_MAINPLL_D5, "mainpll_d5", "mainpll", 1, 5, 0),
	FACTOR_FLAGS(CLK_TOP_MAINPLL_D5_D2, "mainpll_d5_d2", "mainpll_d5", 1, 2, 0),
	FACTOR_FLAGS(CLK_TOP_MAINPLL_D5_D4, "mainpll_d5_d4", "mainpll_d5", 1, 4, 0),

	FACTOR_FLAGS(CLK_TOP_MAINPLL_D7, "mainpll_d7", "mainpll", 1, 7, 0),
	FACTOR_FLAGS(CLK_TOP_MAINPLL_D7_D2, "mainpll_d7_d2", "mainpll_d7", 1, 2, 0),
	FACTOR_FLAGS(CLK_TOP_MAINPLL_D7_D4, "mainpll_d7_d4", "mainpll_d7", 1, 4, 0),

	FACTOR_FLAGS(CLK_TOP_UNIVPLL, "univpll", "univ2pll", 1, 2, 0),

	FACTOR_FLAGS(CLK_TOP_UNIVPLL_D2, "univpll_d2", "univpll", 1, 2, 0),
	FACTOR_FLAGS(CLK_TOP_UNIVPLL_D2_D2, "univpll_d2_d2", "univpll_d2", 1, 2, 0),
	FACTOR_FLAGS(CLK_TOP_UNIVPLL_D2_D4, "univpll_d2_d4", "univpll_d2", 1, 4, 0),
	FACTOR_FLAGS(CLK_TOP_UNIVPLL_D2_D8, "univpll_d2_d8", "univpll_d2", 1, 8, 0),

	FACTOR_FLAGS(CLK_TOP_UNIVPLL_D3, "univpll_d3", "univpll", 1, 3, 0),
	FACTOR_FLAGS(CLK_TOP_UNIVPLL_D3_D2, "univpll_d3_d2", "univpll_d3", 1, 2, 0),
	FACTOR_FLAGS(CLK_TOP_UNIVPLL_D3_D4, "univpll_d3_d4", "univpll_d3", 1, 4, 0),
	FACTOR_FLAGS(CLK_TOP_UNIVPLL_D3_D8, "univpll_d3_d8", "univpll_d3", 1, 8, 0),
	FACTOR_FLAGS(CLK_TOP_UNIVPLL_D3_D16, "univpll_d3_d16", "univpll_d3", 1, 16, 0),

	FACTOR_FLAGS(CLK_TOP_UNIVPLL_D5, "univpll_d5", "univpll", 1, 5, 0),
	FACTOR_FLAGS(CLK_TOP_UNIVPLL_D5_D2, "univpll_d5_d2", "univpll_d5", 1, 2, 0),
	FACTOR_FLAGS(CLK_TOP_UNIVPLL_D5_D4, "univpll_d5_d4", "univpll_d5", 1, 4, 0),

	FACTOR_FLAGS(CLK_TOP_UNIVPLL_D7, "univpll_d7", "univpll", 1, 7, 0),

	FACTOR_FLAGS(CLK_TOP_UNIVPLL_192M, "univpll_192m", "univ2pll", 1, 13, 0),
	FACTOR_FLAGS(CLK_TOP_UNIVPLL_192M_D2, "univpll_192m_d2", "univpll_192m", 1, 2, 0),
	FACTOR_FLAGS(CLK_TOP_UNIVPLL_192M_D4, "univpll_192m_d4", "univpll_192m", 1, 4, 0),
	FACTOR_FLAGS(CLK_TOP_UNIVPLL_192M_D8, "univpll_192m_d8", "univpll_192m", 1, 8, 0),
	FACTOR_FLAGS(CLK_TOP_UNIVPLL_192M_D16, "univpll_192m_d16", "univpll_192m", 1, 16, 0),
	FACTOR_FLAGS(CLK_TOP_UNIVPLL_192M_D32, "univpll_192m_d32", "univpll_192m", 1, 32, 0),

	FACTOR(CLK_TOP_APLL1_D2, "apll1_d2", "apll1", 1, 2),
	FACTOR(CLK_TOP_APLL1_D4, "apll1_d4", "apll1", 1, 4),
	FACTOR(CLK_TOP_APLL1_D8, "apll1_d8", "apll1", 1, 8),

	FACTOR(CLK_TOP_APLL2_D2, "apll2_d2", "apll2", 1, 2),
	FACTOR(CLK_TOP_APLL2_D4, "apll2_d4", "apll2", 1, 4),
	FACTOR(CLK_TOP_APLL2_D8, "apll2_d8", "apll2", 1, 8),

	FACTOR(CLK_TOP_TVDPLL_D2, "tvdpll_d2", "tvdpll", 1, 2),
	FACTOR(CLK_TOP_TVDPLL_D4, "tvdpll_d4", "tvdpll", 1, 4),
	FACTOR(CLK_TOP_TVDPLL_D8, "tvdpll_d8", "tvdpll", 1, 8),
	FACTOR(CLK_TOP_TVDPLL_D16, "tvdpll_d16", "tvdpll", 1, 16),

	FACTOR(CLK_TOP_MMPLL_D4, "mmpll_d4", "mmpll", 1, 4),
	FACTOR(CLK_TOP_MMPLL_D4_D2, "mmpll_d4_d2", "mmpll_d4", 1, 2),
	FACTOR(CLK_TOP_MMPLL_D5, "mmpll_d5", "mmpll", 1, 5),
	FACTOR(CLK_TOP_MMPLL_D5_D2, "mmpll_d5_d2", "mmpll_d5", 1, 2),
	FACTOR(CLK_TOP_MMPLL_D6, "mmpll_d6", "mmpll", 1, 6),
	FACTOR(CLK_TOP_MMPLL_D7, "mmpll_d7", "mmpll", 1, 7),

	FACTOR(CLK_TOP_ADSPPLL_D4, "adsppll_d4", "adsppll", 1, 4),
	FACTOR(CLK_TOP_ADSPPLL_D5, "adsppll_d5", "adsppll", 1, 5),
	FACTOR(CLK_TOP_ADSPPLL_D6, "adsppll_d6", "adsppll", 1, 6),

	FACTOR(CLK_TOP_MSDCPLL_D2, "msdcpll_d2", "msdcpll", 1, 2),
	FACTOR(CLK_TOP_MSDCPLL_D4, "msdcpll_d4", "msdcpll", 1, 4),

	FACTOR(CLK_TOP_OSC_D2, "osc_d2", "ulposc", 1, 2),
	FACTOR(CLK_TOP_OSC_D4, "osc_d4", "ulposc", 1, 4),
	FACTOR(CLK_TOP_OSC_D8, "osc_d8", "ulposc", 1, 8),
	FACTOR(CLK_TOP_OSC_D10, "osc_d10", "ulposc", 1, 10),
	FACTOR(CLK_TOP_OSC_D16, "osc_d16", "ulposc", 1, 16),

	FACTOR(CLK_TOP_TVDPLL_MAINPLL_D2, "tvdpll_mainpll_d2", "tvdpll", 1, 1),
};

static const char *const axi_parents[] = {
	"clk26m",
	"mainpll_d2_d4",
	"mainpll_d7",
	"osc_d4"
};

static const char *const mm_parents[] = {
	"clk26m",
	"adsppll_d5",
	"mmpll_d7",
	"mmpll_d5_d2",
	"mainpll_d2_d2",
	"tvdpll_mainpll_d2"
};

static const char *const scp_parents[] = {
	"clk26m",
	"mainpll_d2_d2",
	"mainpll_d2_d4",
	"mainpll_d3",
	"univpll_d3",
	"mainpll_d5",
	"mainpll_d3_d2",
};

static const char *const isp_parents[] = {
	"clk26m",
	"mainpll_d2",
	"mmpll_d6",
	"adsppll_d5",
	"mainpll_d3",
	"mmpll_d5_d2",
	"tvdpll_mainpll_d2",
	"mainpll_d2_d2"
};

static const char *const cam_parents[] = {
	"clk26m",
	"mainpll_d2",
	"mmpll_d6",
	"mainpll_d3",
	"mmpll_d7",
	"univpll_d3",
	"mmpll_d5_d2",
	"adsppll_d5",
	"tvdpll_mainpll_d2",
	"univpll_d3_d2"
};

static const char *const ccu_parents[] = {
	"clk26m",
	"mainpll_d2",
	"mmpll_d6",
	"mainpll_d3",
	"mmpll_d7",
	"univpll_d3",
	"mmpll_d5_d2",
	"mainpll_d2_d2",
	"adsppll_d5",
	"univpll_d3_d2"
};

static const char *const dsp_ipu_if_parents[] = {
	"clk26m",
	"univpll_d3_d8",
	"univpll_d3_d4",
	"mainpll_d2_d4",
	"univpll_d3_d2",
	"mainpll_d2_d2",
	"univpll_d2_d2",
	"mainpll_d3",
	"univpll_d3",
	"mmpll_d7",
	"mmpll_d6",
	"adsppll_d5",
	"tvdpll",
	"univpll_d2",
	"adsppll_d4",
	"apupll"
};

static const char *const mfg_parents[] = {
	"clk26m",
	"mfgpll",
	"univpll_d3",
	"mainpll_d5"
};

static const char *const mfg_52m_parents[] = {
	"clk26m",
	"univpll_d3_d2",
	"univpll_d3_d4",
	"univpll_d3_d8"
};

static const char *const camtg_parents[] = {
	"clk26m",
	"univpll_192m_d8",
	"univpll_d3_d8",
	"univpll_192m_d4",
	"univpll_d3_d16",
	"clk13m",
	"univpll_192m_d16",
	"univpll_192m_d32"
};

static const char *const uart_parents[] = {
	"clk26m",
	"univpll_d3_d8"
};

static const char *const spi_parents[] = {
	"clk26m",
	"mainpll_d5_d2",
	"mainpll_d3_d4",
	"msdcpll_d4"
};

static const char *const msdc50_0_hclk_parents[] = {
	"clk26m",
	"mainpll_d2_d2",
	"mainpll_d3_d2"
};

static const char *const msdc50_0_parents[] = {
	"clk26m",
	"msdcpll",
	"msdcpll_d2",
	"univpll_d2_d4",
	"mainpll_d3_d2",
	"univpll_d2_d2"
};

static const char *const msdc30_1_parents[] = {
	"clk26m",
	"univpll_d3_d2",
	"mainpll_d3_d2",
	"mainpll_d7",
	"msdcpll_d2"
};

static const char *const audio_parents[] = {
	"clk26m",
	"mainpll_d5_d4",
	"mainpll_d7_d4",
	"mainpll_d2_d16"
};

static const char *const audio_intbus_parents[] = {
	"clk26m",
	"mainpll_d2_d4",
	"mainpll_d7_d2"
};

static const char *const pwrap_ulposc_parents[] = {
	"osc_d10",
	"clk26m",
	"osc_d4",
	"osc_d8",
	"osc_d16"
};

static const char *const atb_parents[] = {
	"clk26m",
	"mainpll_d2_d2",
	"mainpll_d5"
};

static const char *const sspm_parents[] = {
	"clk26m",
	"univpll_d2_d4",
	"mainpll_d2_d2",
	"univpll_d2_d2",
	"mainpll_d3"
};

static const char *const dpi_parents[] = {
	"clk26m",
	"tvdpll_d2",
	"tvdpll_d4",
	"tvdpll_d8",
	"tvdpll_d16"
};

static const char *const scam_parents[] = {
	"clk26m",
	"mainpll_d5_d2"
};

static const char *const disp_pwm_parents[] = {
	"clk26m",
	"univpll_d3_d4",
	"osc_d2",
	"osc_d4",
	"osc_d16"
};

static const char *const ssusb_parents[] = {
	"clk26m",
	"univpll_d5_d4",
	"univpll_d3_d4",
	"univpll_d5_d2"
};

static const char *const spm_parents[] = {
	"clk26m",
	"osc_d8",
	"mainpll_d2_d8"
};

static const char *const i2c_parents[] = {
	"clk26m",
	"mainpll_d2_d8",
	"univpll_d5_d2"
};

static const char *const seninf_parents[] = {
	"clk26m",
	"univpll_d7",
	"univpll_d3_d2",
	"univpll_d2_d2"
};

static const char *const dxcc_parents[] = {
	"clk26m",
	"mainpll_d2_d2",
	"mainpll_d2_d4",
	"mainpll_d2_d8"
};

static const char *const audio_eng1_parents[] = {
	"clk26m",
	"apll1_d2",
	"apll1_d4",
	"apll1_d8"
};

static const char *const audio_eng2_parents[] = {
	"clk26m",
	"apll2_d2",
	"apll2_d4",
	"apll2_d8"
};

static const char *const aes_ufsfde_parents[] = {
	"clk26m",
	"mainpll_d2",
	"mainpll_d2_d2",
	"mainpll_d3",
	"mainpll_d2_d4",
	"univpll_d3"
};

static const char *const ufs_parents[] = {
	"clk26m",
	"mainpll_d2_d4",
	"mainpll_d2_d8",
	"mainpll_d2_d16"
};

static const char *const audio1_parents[] = {
	"clk26m",
	"apll1"
};

static const char *const audio2_parents[] = {
	"clk26m",
	"apll2"
};

static const char *const adsp_parents[] = {
	"clk26m",
	"mainpll_d3",
	"univpll_d2_d4",
	"univpll_d2",
	"mmpll_d4",
	"adsppll_d4",
	"adsppll_d6"
};

static const char *const dpmaif_parents[] = {
	"clk26m",
	"univpll_d2_d4",
	"mainpll_d3",
	"mainpll_d2_d2",
	"univpll_d2_d2",
	"univpll_d3"
};

static const char *const venc_parents[] = {
	"clk26m",
	"mmpll_d7",
	"mainpll_d3",
	"univpll_d2_d2",
	"mainpll_d2_d2",
	"univpll_d3",
	"mmpll_d6",
	"mainpll_d5",
	"mainpll_d3_d2",
	"mmpll_d4_d2",
	"univpll_d2_d4",
	"mmpll_d5",
	"univpll_192m_d2"
};

static const char *const vdec_parents[] = {
	"clk26m",
	"univpll_d2_d4",
	"mainpll_d3",
	"univpll_d2_d2",
	"mainpll_d2_d2",
	"univpll_d3",
	"univpll_d5",
	"univpll_d5_d2",
	"mainpll_d2",
	"univpll_d2",
	"univpll_192m_d2"
};

static const char *const camtm_parents[] = {
	"clk26m",
	"univpll_d7",
	"univpll_d3_d2",
	"univpll_d2_d2"
};

static const char *const pwm_parents[] = {
	"clk26m",
	"univpll_d2_d8"
};

static const char *const audio_hires_parents[] = {
	"clk26m",
	"univpll_d7",
	"apll1",
	"apll2"
};

static const char *const apll_i2s_parents[] = {
	"top_audio1",
	"top_audio2"
};

static const struct mtk_mux top_mux_clks[] = {
	TOP_MUX_GATE_CRITICAL(0, CLK_TOP_AXI, "top_axi", axi_parents, 0, 2, 7, 0),
	TOP_MUX_GATE(0, CLK_TOP_MM, "top_mm", mm_parents, 8, 3, 15, 1),
	TOP_MUX_GATE(0, CLK_TOP_SCP, "top_scp", scp_parents, 16, 3, 23, 2),
	/* cksys_fmem */

	TOP_MUX_GATE(1, CLK_TOP_IMG, "top_img", isp_parents, 0, 3, 7, 4),
	TOP_MUX_GATE(1, CLK_TOP_IPE, "top_ipe", isp_parents, 8, 3, 15, 5),
	TOP_MUX_GATE(1, CLK_TOP_DPE, "top_dpe", isp_parents, 16, 3, 23, 6),
	TOP_MUX_GATE(1, CLK_TOP_CAM, "top_cam", cam_parents, 24, 4, 31, 7),

	TOP_MUX_GATE(2, CLK_TOP_CCU, "top_ccu", ccu_parents, 0, 4, 7, 8),
	TOP_MUX_GATE(2, CLK_TOP_DSP0, "top_dsp0", dsp_ipu_if_parents, 8, 4, 15, 9),
	TOP_MUX_GATE(2, CLK_TOP_DSP1, "top_dsp1", dsp_ipu_if_parents, 16, 4, 23, 10),
	TOP_MUX_GATE(2, CLK_TOP_DSP2, "top_dsp2", dsp_ipu_if_parents, 24, 4, 31, 11),

	/* dsp3 */
	TOP_MUX_GATE(3, CLK_TOP_IPU_IF, "top_ipu_if", dsp_ipu_if_parents, 8, 4, 15, 13),
	TOP_MUX_GATE(3, CLK_TOP_MFG, "top_mfg", mfg_parents, 16, 2, 23, 14),
	TOP_MUX_GATE(3, CLK_TOP_MFG_52M, "top_mfg_52m", mfg_52m_parents, 24, 2, 31, 15),

	TOP_MUX_GATE(4, CLK_TOP_CAMTG1, "top_camtg1", camtg_parents, 0, 3, 7, 16),
	TOP_MUX_GATE(4, CLK_TOP_CAMTG2, "top_camtg2", camtg_parents, 8, 3, 15, 17),
	TOP_MUX_GATE(4, CLK_TOP_CAMTG3, "top_camtg3", camtg_parents, 16, 3, 23, 18),
	TOP_MUX_GATE(4, CLK_TOP_CAMTG4, "top_camtg4", camtg_parents, 24, 3, 31, 19),

	TOP_MUX_GATE(5, CLK_TOP_UART, "top_uart", uart_parents, 0, 1, 7, 20),
	TOP_MUX_GATE(5, CLK_TOP_SPI, "top_spi", spi_parents, 8, 2, 15, 21),
	TOP_MUX_GATE_FLAGS(5, CLK_TOP_MSDC50_0_HCLK, "top_msdc50_0_hclk",
			   msdc50_0_hclk_parents, 16, 2, 23, 22, 0),
	TOP_MUX_GATE_FLAGS(5, CLK_TOP_MSDC50_0, "top_msdc50_0",
			   msdc50_0_parents, 24, 3, 31, 23, 0),

	TOP_MUX_GATE_FLAGS(6, CLK_TOP_MSDC30_1, "top_msdc30_1",
			   msdc30_1_parents, 0, 3, 7, 24, 0),
	TOP_MUX_GATE(6, CLK_TOP_AUDIO, "top_audio", audio_parents, 8, 2, 15, 25),
	TOP_MUX_GATE(6, CLK_TOP_AUDIO_INTBUS, "top_audio_intbus",
		     audio_intbus_parents, 16, 2, 23, 26),
	TOP_MUX_GATE(6, CLK_TOP_PWRAP_ULPOSC, "top_pwrap_ulposc",
		     pwrap_ulposc_parents, 24, 3, 31, 27),

	TOP_MUX_GATE(7, CLK_TOP_ATB, "top_atb", atb_parents, 0, 2, 7, 28),
	TOP_MUX_GATE_CRITICAL(7, CLK_TOP_SSPM, "top_sspm", sspm_parents, 8, 3, 15, 29),
	TOP_MUX_GATE(7, CLK_TOP_DPI, "top_dpi", dpi_parents, 16, 3, 23, 30),
	TOP_MUX_GATE(7, CLK_TOP_SCAM, "top_scam", scam_parents, 24, 1, 31, 32),

	TOP_MUX_GATE(8, CLK_TOP_DISP_PWM, "top_disp_pwm", disp_pwm_parents, 0, 3, 7, 33),
	TOP_MUX_GATE(8, CLK_TOP_SSUSB, "top_ssusb", ssusb_parents, 8, 2, 15, 34),
	TOP_MUX_GATE(8, CLK_TOP_SSUSB_XHCI, "top_ssusb_xhci", ssusb_parents, 16, 2, 23, 35),
	TOP_MUX_GATE_CRITICAL(8, CLK_TOP_SPM, "top_spm", spm_parents, 24, 2, 31, 36),

	TOP_MUX_GATE(9, CLK_TOP_I2C, "top_i2c", i2c_parents, 0, 2, 7, 37),
	TOP_MUX_GATE(9, CLK_TOP_SENINF0, "top_seninf0", seninf_parents, 8, 2, 15, 38),
	TOP_MUX_GATE(9, CLK_TOP_SENINF1, "top_seninf1", seninf_parents, 16, 2, 23, 39),
	TOP_MUX_GATE(9, CLK_TOP_SENINF2, "top_seninf2", seninf_parents, 24, 2, 31, 40),

	TOP_MUX_GATE(10, CLK_TOP_DXCC, "top_dxcc", dxcc_parents, 0, 2, 7, 41),
	TOP_MUX_GATE(10, CLK_TOP_AUDIO_ENG1, "top_audio_eng1",
		     audio_eng1_parents, 8, 2, 15, 42),
	TOP_MUX_GATE(10, CLK_TOP_AUDIO_ENG2, "top_audio_eng2",
		     audio_eng2_parents, 16, 2, 23, 43),
	TOP_MUX_GATE(10, CLK_TOP_AES_UFSFDE, "top_aes_ufsfde",
		     aes_ufsfde_parents, 24, 3, 31, 44),

	TOP_MUX_GATE(11, CLK_TOP_UFS, "top_ufs", ufs_parents, 0, 2, 7, 45),
	TOP_MUX_GATE(11, CLK_TOP_AUDIO1, "top_audio1", audio1_parents, 8, 1, 15, 46),
	TOP_MUX_GATE(11, CLK_TOP_AUDIO2, "top_audio2", audio2_parents, 16, 1, 23, 47),
	TOP_MUX_GATE(11, CLK_TOP_ADSP, "top_adsp", adsp_parents, 24, 3, 31, 48),

	TOP_MUX_GATE(12, CLK_TOP_DPMAIF, "top_dpmaif", dpmaif_parents, 0, 3, 7, 49),
	TOP_MUX_GATE(12, CLK_TOP_VENC, "top_venc", venc_parents, 8, 4, 15, 50),
	TOP_MUX_GATE(12, CLK_TOP_VDEC, "top_vdec", vdec_parents, 16, 4, 23, 51),
	TOP_MUX_GATE(12, CLK_TOP_CAMTM, "top_camtm", camtm_parents, 24, 2, 31, 52),

	TOP_MUX_GATE(13, CLK_TOP_PWM, "top_pwm", pwm_parents, 0, 1, 7, 53),
	TOP_MUX_GATE(13, CLK_TOP_AUDIO_HIRES, "top_audio_hires",
		     audio_hires_parents, 8, 2, 15, 54),
	/* bus_aximem */
	TOP_MUX_GATE(13, CLK_TOP_CAMTG5, "top_camtg5", camtg_parents, 24, 3, 31, 56),
};

static const struct mtk_composite top_composite_clks[] = {
	MUX(CLK_TOP_APLL_I2S0, "top_apll_i2s0", apll_i2s_parents, 0x320, 8, 1),
	MUX(CLK_TOP_APLL_I2S1, "top_apll_i2s1", apll_i2s_parents, 0x320, 9, 1),
	MUX(CLK_TOP_APLL_I2S2, "top_apll_i2s2", apll_i2s_parents, 0x320, 10, 1),
	MUX(CLK_TOP_APLL_I2S3, "top_apll_i2s3", apll_i2s_parents, 0x320, 11, 1),
	MUX(CLK_TOP_APLL_I2S4, "top_apll_i2s4", apll_i2s_parents, 0x320, 12, 1),
	MUX(CLK_TOP_APLL_I2S5, "top_apll_i2s5", apll_i2s_parents, 0x328, 20, 1),

	DIV_GATE(CLK_TOP_APLL12_DIV0, "top_apll12_div0", "top_apll_i2s0", 0x320, 2, 0x324, 8, 0),
	DIV_GATE(CLK_TOP_APLL12_DIV1, "top_apll12_div1", "top_apll_i2s1", 0x320, 3, 0x324, 8, 8),
	DIV_GATE(CLK_TOP_APLL12_DIV2, "top_apll12_div2", "top_apll_i2s2", 0x320, 4, 0x324, 8, 16),
	DIV_GATE(CLK_TOP_APLL12_DIV3, "top_apll12_div3", "top_apll_i2s3", 0x320, 5, 0x324, 8, 24),
	DIV_GATE(CLK_TOP_APLL12_DIV4, "top_apll12_div4", "top_apll_i2s4", 0x320, 6, 0x328, 8, 0),
	DIV_GATE(CLK_TOP_APLL12_DIVB, "top_apll12_divb", "top_apll12_div4", 0x320, 7, 0x328, 8, 8),
	DIV_GATE(CLK_TOP_APLL12_DIV5, "top_apll12_div5", "top_apll_i2s5", 0x328, 16, 0x328, 4, 28),
};

static const struct mtk_gate_regs top_cg_regs = {
	.sta_ofs = 0x110,
	.clr_ofs = 0x110,
	.set_ofs = 0x110,
};

#define GATE_TOP(_id, _name, _parent, _shift)               \
	GATE_MTK(_id, _name, _parent, &top_cg_regs, _shift, \
		 &mtk_clk_gate_ops_no_setclr_inv)

static const struct mtk_gate top_clks[] = {
	GATE_TOP(CLK_TOP_ARMPLL_DIV_PLL1, "armpll_div_pll1", "mainpll", 4),
	GATE_TOP(CLK_TOP_ARMPLL_DIV_PLL2, "armpll_div_pll2", "univpll", 5),
};

/* Register mux notifier for MFG mux */
static int clk_mt6785_reg_mfg_mux_notifier(struct device *dev, struct clk *clk)
{
	struct mtk_mux_nb *mfg_mux_nb;
	int i;

	mfg_mux_nb = devm_kzalloc(dev, sizeof(*mfg_mux_nb), GFP_KERNEL);
	if (!mfg_mux_nb)
		return -ENOMEM;

	for (i = 0; i < ARRAY_SIZE(top_mux_clks); i++)
		if (top_mux_clks[i].id == CLK_TOP_MFG)
			break;
	if (i == ARRAY_SIZE(top_mux_clks))
		return -EINVAL;

	mfg_mux_nb->ops = top_mux_clks[i].ops;
	mfg_mux_nb->bypass_index = 3; /* Bypass to mainpll_d5 */

	return devm_mtk_clk_mux_notifier_register(dev, clk, mfg_mux_nb);
}

static const struct mtk_clk_desc top_desc = {
	.clks = top_clks,
	.num_clks = ARRAY_SIZE(top_clks),
	.fixed_clks = top_fixed_clks,
	.num_fixed_clks = ARRAY_SIZE(top_fixed_clks),
	.factor_clks = top_factor_clks,
	.num_factor_clks = ARRAY_SIZE(top_factor_clks),
	.mux_clks = top_mux_clks,
	.num_mux_clks = ARRAY_SIZE(top_mux_clks),
	.composite_clks = top_composite_clks,
	.num_composite_clks = ARRAY_SIZE(top_composite_clks),
	.clk_lock = &mt6785_clk_lock,
	.clk_notifier_func = clk_mt6785_reg_mfg_mux_notifier,
	.mfg_clk_idx = CLK_TOP_MFG,
};

static const struct of_device_id of_match_clk_mt6785_top[] = {
	{ .compatible = "mediatek,mt6785-topckgen", .data = &top_desc },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, of_match_clk_mt6785_top);

static struct platform_driver clk_mt6785_top_drv = {
	.probe = mtk_clk_simple_probe,
	.remove = mtk_clk_simple_remove,
	.driver = {
		.name = "clk-mt6785-top",
		.of_match_table = of_match_clk_mt6785_top,
	},
};
module_platform_driver(clk_mt6785_top_drv);

MODULE_DESCRIPTION("MediaTek MT6785 top clock generators driver");
MODULE_LICENSE("GPL");
