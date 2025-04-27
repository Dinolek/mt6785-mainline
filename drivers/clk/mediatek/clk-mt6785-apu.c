// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2025 Dinolek <git@dinolek.me>
 */

#include <dt-bindings/clock/mediatek,mt6785-clk.h>
#include <linux/platform_device.h>

#include "clk-gate.h"
#include "clk-mtk.h"

static const struct mtk_gate_regs apu_cg_regs = {
	.sta_ofs = 0x0,
	.clr_ofs = 0x8,
	.set_ofs = 0x4,
};

#define GATE_APU(_id, _name, _parent, _shift)               \
	GATE_MTK(_id, _name, _parent, &apu_cg_regs, _shift, \
		 &mtk_clk_gate_ops_setclr)

static const struct mtk_gate apu_core0_clks[] = {
	GATE_APU(CLK_APU_MAIN, "apu0_main", "top_dsp1", 0),
	GATE_APU(CLK_APU_AXI, "apu0_axi", "top_dsp1", 1),
	GATE_APU(CLK_APU_JTAG, "apu0_jtag", "top_dsp1", 2),
};

static const struct mtk_gate apu_core1_clks[] = {
	GATE_APU(CLK_APU_MAIN, "apu1_main", "top_dsp2", 0),
	GATE_APU(CLK_APU_AXI, "apu1_axi", "top_dsp2", 1),
	GATE_APU(CLK_APU_JTAG, "apu1_jtag", "top_dsp2", 2),
};

static const struct mtk_gate apu_conn_clks[] = {
	GATE_APU(CLK_APU_CONN_MAIN, "apu_conn_main", "top_dsp1", 0),
	GATE_APU(CLK_APU_CONN_AHB, "apu_conn_ahb", "top_dsp0", 1),
	GATE_APU(CLK_APU_CONN_AXI, "apu_conn_axi", "top_dsp0", 2),
	GATE_APU(CLK_APU_CONN_ISP, "apu_conn_isp", "top_dsp0", 3),
	GATE_APU(CLK_APU_CONN_CAM_ADL, "apu_conn_cam_adl", "top_dsp0", 4),
	GATE_APU(CLK_APU_CONN_IMG_ADL, "apu_conn_img_adl", "top_dsp0", 5),
	GATE_APU(CLK_APU_CONN_EMI_26M, "apu_conn_emi_26m", "top_dsp0", 6),
	GATE_APU(CLK_APU_CONN_VPU_UDI, "apu_conn_vpu_udi", "top_dsp0", 7),
};

static const struct mtk_gate apu_vcore_clks[] = {
	GATE_APU(CLK_APU_VCORE_AHB, "apu_vcore_ahb", "top_ipu_if", 0),
	GATE_APU(CLK_APU_VCORE_AXI, "apu_vcore_axi", "top_ipu_if", 1),
	GATE_APU(CLK_APU_VCORE_ADL, "apu_vcore_adl", "top_ipu_if", 2),
	GATE_APU(CLK_APU_VCORE_QOS, "apu_vcore_qos", "top_ipu_if", 3),
};

static const struct mtk_clk_desc apu_core0_desc = {
	.clks = apu_core0_clks,
	.num_clks = ARRAY_SIZE(apu_core0_clks),
};

static const struct mtk_clk_desc apu_core1_desc = {
	.clks = apu_core1_clks,
	.num_clks = ARRAY_SIZE(apu_core1_clks),
};

static const struct mtk_clk_desc apu_conn_desc = {
	.clks = apu_conn_clks,
	.num_clks = ARRAY_SIZE(apu_conn_clks),
};

static const struct mtk_clk_desc apu_vcore_desc = {
	.clks = apu_vcore_clks,
	.num_clks = ARRAY_SIZE(apu_vcore_clks),
};

static const struct of_device_id of_match_clk_mt6785_apu[] = {
	{ .compatible = "mediatek,mt6785-apu_core0", .data = &apu_core0_desc },
	{ .compatible = "mediatek,mt6785-apu_core1", .data = &apu_core1_desc },
	{ .compatible = "mediatek,mt6785-apu_conn", .data = &apu_conn_desc },
	{ .compatible = "mediatek,mt6785-apu_vcore", .data = &apu_vcore_desc },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, of_match_clk_mt6785_apu);

static struct platform_driver clk_mt6785_apu_drv = {
	.probe = mtk_clk_simple_probe,
	.remove = mtk_clk_simple_remove,
	.driver = {
		.name = "clk-mt6785-apu",
		.of_match_table = of_match_clk_mt6785_apu,
	},
};
module_platform_driver(clk_mt6785_apu_drv);

MODULE_DESCRIPTION("MediaTek MT6785 AI Processing Unit clocks driver");
MODULE_LICENSE("GPL");
