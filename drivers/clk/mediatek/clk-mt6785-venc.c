// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2025 Dinolek <git@dinolek.me>
 */

#include <dt-bindings/clock/mediatek,mt6785-clk.h>
#include <linux/platform_device.h>

#include "clk-gate.h"
#include "clk-mtk.h"

static const struct mtk_gate_regs venc_cg_regs = {
	.sta_ofs = 0x0,
	.clr_ofs = 0x8,
	.set_ofs = 0x4,
};

#define GATE_VENC(_id, _name, _parent, _shift)               \
	GATE_MTK(_id, _name, _parent, &venc_cg_regs, _shift, \
		 &mtk_clk_gate_ops_setclr_inv)

static const struct mtk_gate venc_clks[] = {
	GATE_VENC(CLK_VENC_LARB, "venc_larb", "top_venc", 0),
	GATE_VENC(CLK_VENC_MAIN, "venc_main", "top_venc", 4),
	GATE_VENC(CLK_VENC_JPGENC, "venc_jpgenc", "top_venc", 8),
	GATE_VENC(CLK_VENC_GALS, "venc_gals", "top_venc", 28),
};

static const struct mtk_clk_desc venc_desc = {
	.clks = venc_clks,
	.num_clks = ARRAY_SIZE(venc_clks),
};

static const struct of_device_id of_match_clk_mt6785_venc[] = {
	{ .compatible = "mediatek,mt6785-vencsys", .data = &venc_desc },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, of_match_clk_mt6785_venc);

static struct platform_driver clk_mt6785_venc_drv = {
	.probe = mtk_clk_simple_probe,
	.remove = mtk_clk_simple_remove,
	.driver = {
		.name = "clk-mt6785-venc",
		.of_match_table = of_match_clk_mt6785_venc,
	},
};
module_platform_driver(clk_mt6785_venc_drv);

MODULE_DESCRIPTION("MediaTek MT6785 Video Encoders clocks driver");
MODULE_LICENSE("GPL");
