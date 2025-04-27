// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2025 Dinolek <git@dinolek.me>
 */

#include <dt-bindings/clock/mediatek,mt6785-clk.h>
#include <linux/platform_device.h>

#include "clk-mtk.h"

static const char *const mcu_armpll_ll_parents[] = {
	"clk26m",
	"armpll_ll",
	"armpll_div_pll1",
	"armpll_div_pll2"
};

static const char *const mcu_armpll_bl_parents[] = {
	"clk26m",
	"armpll_bl",
	"armpll_div_pll1",
	"armpll_div_pll2"
};

static const char *const mcu_armpll_bus_parents[] = {
	"clk26m",
	"ccipll",
	"armpll_div_pll1",
	"armpll_div_pll2"
};

static struct mtk_composite mcu_muxes[] = {
	MUX(CLK_MCU_ARMPLL_LL, "mcu_armpll_ll", mcu_armpll_ll_parents, 0x2a0, 9, 2),
	MUX(CLK_MCU_ARMPLL_BL, "mcu_armpll_bl", mcu_armpll_bl_parents, 0x2a4, 9, 2),
	MUX(CLK_MCU_ARMPLL_BUS, "mcu_armpll_bus", mcu_armpll_bus_parents, 0x2e0, 9, 2),
};

static const struct mtk_clk_desc mcu_desc = {
	.composite_clks = mcu_muxes,
	.num_composite_clks = ARRAY_SIZE(mcu_muxes),
};

static const struct of_device_id of_match_clk_mt6785_mcu[] = {
	{ .compatible = "mediatek,mt6785-mcusys", .data = &mcu_desc },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, of_match_clk_mt6785_mcu);

static struct platform_driver clk_mt6785_mcu_drv = {
	.probe = mtk_clk_simple_probe,
	.remove = mtk_clk_simple_remove,
	.driver = {
		.name = "clk-mt6785-mcu",
		.of_match_table = of_match_clk_mt6785_mcu,
	},
};
module_platform_driver(clk_mt6785_mcu_drv);

MODULE_DESCRIPTION("MediaTek MT6785 MicroController Unit clocks driver");
MODULE_LICENSE("GPL");
