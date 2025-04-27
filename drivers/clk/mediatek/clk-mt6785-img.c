// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2025 Dinolek <git@dinolek.me>
 */

#include <dt-bindings/clock/mediatek,mt6785-clk.h>
#include <linux/platform_device.h>

#include "clk-gate.h"
#include "clk-mtk.h"

static const struct mtk_gate_regs img_cg_regs = {
	.sta_ofs = 0x0,
	.clr_ofs = 0x8,
	.set_ofs = 0x4,
};

#define GATE_IMG(_id, _name, _parent, _shift)               \
	GATE_MTK(_id, _name, _parent, &img_cg_regs, _shift, \
		 &mtk_clk_gate_ops_setclr)

static const struct mtk_gate img_clks[] = {
	GATE_IMG(CLK_IMG_LARB5, "img_larb5", "top_img", 0),
	GATE_IMG(CLK_IMG_LARB4, "img_larb4", "top_img", 1),
	GATE_IMG(CLK_IMG_DIP, "img_dip", "top_img", 2),
	GATE_IMG(CLK_IMG_FDVT, "img_fdvt", "top_img", 3),
	GATE_IMG(CLK_IMG_DPE, "img_dpe", "top_img", 4),
	GATE_IMG(CLK_IMG_RSC, "img_rsc", "top_img", 5),
	GATE_IMG(CLK_IMG_MFB, "img_mfb", "top_img", 6),
	GATE_IMG(CLK_IMG_WPE_A, "img_wpe_a", "top_img", 7),
	GATE_IMG(CLK_IMG_WPE_B, "img_wpe_b", "top_img", 8),
	GATE_IMG(CLK_IMG_OWE, "img_owe", "top_img", 9),
};

static const struct mtk_clk_desc img_desc = {
	.clks = img_clks,
	.num_clks = ARRAY_SIZE(img_clks),
};

static const struct of_device_id of_match_clk_mt6785_img[] = {
	{ .compatible = "mediatek,mt6785-imgsys", .data = &img_desc },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, of_match_clk_mt6785_img);

static struct platform_driver clk_mt6785_img_drv = {
	.probe = mtk_clk_simple_probe,
	.remove = mtk_clk_simple_remove,
	.driver = {
		.name = "clk-mt6785-img",
		.of_match_table = of_match_clk_mt6785_img,
	},
};
module_platform_driver(clk_mt6785_img_drv);

MODULE_DESCRIPTION("MediaTek MT6785 Image clocks driver");
MODULE_LICENSE("GPL");
