// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2025 Dinolek <git@dinolek.me>
 */

#include <dt-bindings/clock/mediatek,mt6785-clk.h>
#include <linux/platform_device.h>

#include "clk-gate.h"
#include "clk-mtk.h"

static const struct mtk_gate_regs mfg_cg_regs = {
	.sta_ofs = 0x0,
	.clr_ofs = 0x8,
	.set_ofs = 0x4,
};

#define GATE_MFG(_id, _name, _parent, _shift)                     \
	GATE_MTK_FLAGS(_id, _name, _parent, &mfg_cg_regs, _shift, \
		       &mtk_clk_gate_ops_setclr, CLK_SET_RATE_PARENT)

static const struct mtk_gate mfg_clks[] = {
	GATE_MFG(CLK_MFG_BG3D, "mfg_bg3d", "top_mfg", 0),
};

static const struct mtk_clk_desc mfg_desc = {
	.clks = mfg_clks,
	.num_clks = ARRAY_SIZE(mfg_clks),
	.need_runtime_pm = true,
};

static const struct of_device_id of_match_clk_mt6785_mfg[] = {
	{ .compatible = "mediatek,mt6785-mfgcfg", .data = &mfg_desc },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, of_match_clk_mt6785_mfg);

static struct platform_driver clk_mt6785_mfg_drv = {
	.probe = mtk_clk_simple_probe,
	.remove = mtk_clk_simple_remove,
	.driver = {
		.name = "clk-mt6785-mfg",
		.of_match_table = of_match_clk_mt6785_mfg,
	},
};
module_platform_driver(clk_mt6785_mfg_drv);

MODULE_DESCRIPTION("MediaTek MT6785 GPU MFlexGraphics driver");
MODULE_LICENSE("GPL");
