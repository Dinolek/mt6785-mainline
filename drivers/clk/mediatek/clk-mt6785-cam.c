// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2025 Dinolek <git@dinolek.me>
 */

#include <dt-bindings/clock/mediatek,mt6785-clk.h>
#include <linux/platform_device.h>

#include "clk-gate.h"
#include "clk-mtk.h"

static const struct mtk_gate_regs cam_cg_regs = {
	.sta_ofs = 0x0,
	.clr_ofs = 0x8,
	.set_ofs = 0x4,
};

#define GATE_CAM(_id, _name, _parent, _shift)               \
	GATE_MTK(_id, _name, _parent, &cam_cg_regs, _shift, \
		 &mtk_clk_gate_ops_setclr)

static const struct mtk_gate cam_clks[] = {
	GATE_CAM(CLK_CAM_LARB6, "cam_larb6", "top_cam", 0),
	GATE_CAM(CLK_CAM_LARB7, "cam_larb7", "top_cam", 2),
	GATE_CAM(CLK_CAM_GALS, "cam_gals", "top_cam", 4),
	GATE_CAM(CLK_CAM_MAIN, "cam_main", "top_cam", 6),
	GATE_CAM(CLK_CAM_TG, "cam_tg", "top_cam", 7),
	GATE_CAM(CLK_CAM_SENINF, "cam_seninf", "top_cam", 8),
	GATE_CAM(CLK_CAM_SV0, "cam_sv0", "top_cam", 9),
	GATE_CAM(CLK_CAM_SV1, "cam_sv1", "top_cam", 10),
	GATE_CAM(CLK_CAM_CCU, "cam_ccu", "top_cam", 12),
	GATE_CAM(CLK_CAM_FAKE_ENG, "cam_fake_eng", "top_cam", 13),
};

static const struct mtk_clk_desc cam_desc = {
	.clks = cam_clks,
	.num_clks = ARRAY_SIZE(cam_clks),
};

static const struct of_device_id of_match_clk_mt6785_cam[] = {
	{ .compatible = "mediatek,mt6785-camsys", .data = &cam_desc },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, of_match_clk_mt6785_cam);

static struct platform_driver clk_mt6785_cam_drv = {
	.probe = mtk_clk_simple_probe,
	.remove = mtk_clk_simple_remove,
	.driver = {
		.name = "clk-mt6785-cam",
		.of_match_table = of_match_clk_mt6785_cam,
	},
};
module_platform_driver(clk_mt6785_cam_drv);

MODULE_DESCRIPTION("MediaTek MT6785 Camera clocks driver");
MODULE_LICENSE("GPL");
