// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2025 Dinolek <git@dinolek.me>
 */

#include <dt-bindings/clock/mediatek,mt6785-clk.h>
#include <linux/platform_device.h>

#include "clk-gate.h"
#include "clk-mtk.h"

static const struct mtk_gate_regs vdec_cg_regs = {
	.sta_ofs = 0x0,
	.clr_ofs = 0x4,
	.set_ofs = 0x0,
};

static const struct mtk_gate_regs larb_cg_regs = {
	.sta_ofs = 0x8,
	.clr_ofs = 0xc,
	.set_ofs = 0x8,
};

#define GATE_VDEC(_id, _name, _parent, _shift)               \
	GATE_MTK(_id, _name, _parent, &vdec_cg_regs, _shift, \
		 &mtk_clk_gate_ops_setclr_inv)

#define GATE_LARB(_id, _name, _parent, _shift)               \
	GATE_MTK(_id, _name, _parent, &larb_cg_regs, _shift, \
		 &mtk_clk_gate_ops_setclr_inv)

static const struct mtk_gate vdec_clks[] = {
	GATE_VDEC(CLK_VDEC_MAIN, "vdec_main", "top_vdec", 0),
	GATE_LARB(CLK_VDEC_LARB1, "vdec_larb1", "top_vdec", 0),
};

static const struct mtk_clk_desc vdec_desc = {
	.clks = vdec_clks,
	.num_clks = ARRAY_SIZE(vdec_clks),
};

static const struct of_device_id of_match_clk_mt6785_vdec[] = {
	{ .compatible = "mediatek,mt6785-vdecsys", .data = &vdec_desc },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, of_match_clk_mt6785_vdec);

static struct platform_driver clk_mt6785_vdec_drv = {
	.probe = mtk_clk_simple_probe,
	.remove = mtk_clk_simple_remove,
	.driver = {
		.name = "clk-mt6785-vdec",
		.of_match_table = of_match_clk_mt6785_vdec,
	},
};
module_platform_driver(clk_mt6785_vdec_drv);

MODULE_DESCRIPTION("MediaTek MT6785 Video Decoders clocks driver");
MODULE_LICENSE("GPL");
