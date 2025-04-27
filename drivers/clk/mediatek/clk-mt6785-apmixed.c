// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2025 Dinolek <git@dinolek.me>
 */

#include <dt-bindings/clock/mediatek,mt6785-clk.h>
#include <linux/platform_device.h>

#include "clk-gate.h"
#include "clk-mtk.h"
#include "clk-pll.h"

static const struct mtk_gate_regs apmixed_cg_regs = {
	.sta_ofs = 0x20,
	.clr_ofs = 0x20,
	.set_ofs = 0x20,
};

#define GATE_APMIXED_FLAGS(_id, _name, _parent, _shift, _flags)       \
	GATE_MTK_FLAGS(_id, _name, _parent, &apmixed_cg_regs, _shift, \
		       &mtk_clk_gate_ops_no_setclr_inv, _flags)

#define GATE_APMIXED(_id, _name, _parent, _shift) \
	GATE_APMIXED_FLAGS(_id, _name, _parent, _shift, 0)

static const struct mtk_gate apmixed_clks[] = {
	GATE_APMIXED(CLK_APMIXED_SSUSB, "apmixed_ssusb", "clk26m", 4),
	GATE_APMIXED_FLAGS(CLK_APMIXED_APPLL, "apmixed_appll", "clk26m", 5,
			   CLK_IS_CRITICAL),
	GATE_APMIXED(CLK_APMIXED_MIPIC0, "apmixed_mipic0", "clk26m", 6),
	GATE_APMIXED(CLK_APMIXED_MDPLL, "apmixed_mdpll", "clk26m", 7),
	GATE_APMIXED(CLK_APMIXED_MMSYS, "apmixed_mmsys", "clk26m", 8),
	GATE_APMIXED(CLK_APMIXED_UFS, "apmixed_ufs", "clk26m", 9),
	GATE_APMIXED(CLK_APMIXED_MIPIC1, "apmixed_mipic1", "clk26m", 11),
	GATE_APMIXED(CLK_APMIXED_MEMPLL, "apmixed_mempll", "clk26m", 13),
	GATE_APMIXED(CLK_APMIXED_LVPLL, "apmixed_lvpll", "clk26m", 14),
	GATE_APMIXED(CLK_APMIXED_MIPID0, "apmixed_mipid0", "clk26m", 16),
	GATE_APMIXED(CLK_APMIXED_MIPID1, "apmixed_mipid1", "clk26m", 17),
};

#define MT6785_PLL_FMAX (3800UL * MHZ)
#define MT6785_PLL_FMIN (1500UL * MHZ)
#define MT6785_INTEGER_BITS 8

#define PLL_B(_id, _name, _reg, _pwr_reg, _flags, _rst_bar_mask, _pcwbits,  \
	      _pd_reg, _pd_shift, _tuner_reg, _tuner_en_reg, _tuner_en_bit, \
	      _pcw_reg, _div_table)                                         \
	{                                                                   \
		.id = _id,                                                  \
		.name = _name,                                              \
		.reg = _reg,                                                \
		.pwr_reg = _pwr_reg,                                        \
		.pd_reg = _pd_reg,                                          \
		.tuner_reg = _tuner_reg,                                    \
		.tuner_en_reg = _tuner_en_reg,                              \
		.tuner_en_bit = _tuner_en_bit,                              \
		.pd_shift = _pd_shift,                                      \
		.flags = _flags,                                            \
		.rst_bar_mask = _rst_bar_mask,                              \
		.fmin = MT6785_PLL_FMIN,                                    \
		.fmax = MT6785_PLL_FMAX,                                    \
		.pcwbits = _pcwbits,                                        \
		.pcwibits = MT6785_INTEGER_BITS,                            \
		.pcw_reg = _pcw_reg,                                        \
		.div_table = _div_table,                                    \
	}

#define PLL(_id, _name, _reg, _pwr_reg, _flags, _rst_bar_mask, _pcwbits,    \
	    _pd_reg, _pd_shift, _tuner_reg, _tuner_en_reg, _tuner_en_bit,   \
	    _pcw_reg)                                                       \
	PLL_B(_id, _name, _reg, _pwr_reg, _flags, _rst_bar_mask, _pcwbits,  \
	      _pd_reg, _pd_shift, _tuner_reg, _tuner_en_reg, _tuner_en_bit, \
	      _pcw_reg, NULL)

static const struct mtk_pll_div_table armpll_ll_div_table[] = {
	{ .div = 0, .freq = MT6785_PLL_FMAX },
	{ .div = 1, .freq = 1700 * MHZ },
	{ .div = 2, .freq = 850 * MHZ },
	{ .div = 3, .freq = 425 * MHZ },
	{ .div = 4, .freq = 212500000 },
	{ /* sentinel */ }
};

static const struct mtk_pll_div_table armpll_bl_div_table[] = {
	{ .div = 0, .freq = MT6785_PLL_FMAX },
	{ .div = 1, .freq = 1600 * MHZ },
	{ .div = 2, .freq = 800 * MHZ },
	{ .div = 3, .freq = 400 * MHZ },
	{ .div = 4, .freq = 200 * MHZ },
	{ /* sentinel */ }
};

static const struct mtk_pll_div_table mfgpll_div_table[] = {
	{ .div = 0, .freq = MT6785_PLL_FMAX },
	{ .div = 1, .freq = 1900 * MHZ },
	{ .div = 2, .freq = 950 * MHZ },
	{ .div = 3, .freq = 375 * MHZ },
	{ .div = 4, .freq = 187500000 },
	{ /* sentinel */ }
};

static const struct mtk_pll_data plls[] = {
	PLL_B(CLK_APMIXED_ARMPLL_LL, "armpll_ll", 0x200, 0x20c, PLL_AO, 0, 22,
	      0x204, 24, 0, 0, 0, 0x204, armpll_ll_div_table),
	PLL_B(CLK_APMIXED_ARMPLL_BL, "armpll_bl", 0x210, 0x21c, PLL_AO, 0, 22,
	      0x214, 24, 0, 0, 0, 0x214, armpll_bl_div_table),
	PLL(CLK_APMIXED_MAINPLL, "mainpll", 0x230, 0x23c, HAVE_RST_BAR | PLL_AO,
	    BIT(24), 22, 0x234, 24, 0, 0, 0, 0x234),
	PLL(CLK_APMIXED_UNIV2PLL, "univ2pll", 0x240, 0x24c, HAVE_RST_BAR,
	    BIT(24), 22, 0x244, 24, 0, 0, 0, 0x244),
	PLL_B(CLK_APMIXED_MFGPLL, "mfgpll", 0x250, 0x25c, 0, 0, 22, 0x254, 24,
	      0, 0, 0, 0x254, mfgpll_div_table),
	PLL(CLK_APMIXED_MSDCPLL, "msdcpll", 0x260, 0x26c, 0, 0, 22, 0x264, 24,
	    0, 0, 0, 0x264),
	PLL(CLK_APMIXED_TVDPLL, "tvdpll", 0x270, 0x27c, 0, 0, 22, 0x274, 24, 0,
	    0, 0, 0x274),
	PLL(CLK_APMIXED_MMPLL, "mmpll", 0x280, 0x28c, HAVE_RST_BAR, BIT(23), 22,
	    0x284, 24, 0, 0, 0, 0x284),
	PLL(CLK_APMIXED_CCIPLL, "ccipll", 0x2a0, 0x2ac, PLL_AO, 0, 22, 0x2a4,
	    24, 0, 0, 0, 0x2a4),
	PLL(CLK_APMIXED_ADSPPLL, "adsppll", 0x2b0, 0x2bc, HAVE_RST_BAR, BIT(23),
	    22, 0x2b4, 24, 0, 0, 0, 0x2b4),
	PLL(CLK_APMIXED_APLL1, "apll1", 0x2c0, 0x2d0, 0, 0, 32, 0x2c0, 1, 0,
	    0x14, 0, 0x2c4),
	PLL(CLK_APMIXED_APLL2, "apll2", 0x2d4, 0x2e4, 0, 0, 32, 0x2d4, 1, 0,
	    0x14, 1, 0x2d8),
	PLL(CLK_APMIXED_APUPLL, "apupll", 0x2ec, 0x2f8, 0, 0, 22, 0x2f0, 24, 0,
	    0, 0, 0x2f0),
};

static int clk_mt6785_apmixed_probe(struct platform_device *pdev)
{
	struct clk_hw_onecell_data *clk_data;
	struct device_node *node = pdev->dev.of_node;
	int r;

	clk_data = mtk_alloc_clk_data(CLK_APMIXED_NR_CLK);
	if (!clk_data)
		return -ENOMEM;

	r = mtk_clk_register_plls(&pdev->dev, plls, ARRAY_SIZE(plls), clk_data);
	if (r)
		goto free_apmixed_data;

	r = mtk_clk_register_gates(&pdev->dev, node, apmixed_clks,
				   ARRAY_SIZE(apmixed_clks), clk_data);
	if (r)
		goto unregister_plls;

	r = of_clk_add_hw_provider(node, of_clk_hw_onecell_get, clk_data);
	if (r)
		goto unregister_gates;

	platform_set_drvdata(pdev, clk_data);

	return 0;

unregister_gates:
	mtk_clk_unregister_gates(apmixed_clks, ARRAY_SIZE(apmixed_clks),
				 clk_data);
unregister_plls:
	mtk_clk_unregister_plls(plls, ARRAY_SIZE(plls), clk_data);
free_apmixed_data:
	mtk_free_clk_data(clk_data);
	return r;
}

static void clk_mt6785_apmixed_remove(struct platform_device *pdev)
{
	struct device_node *node = pdev->dev.of_node;
	struct clk_hw_onecell_data *clk_data = platform_get_drvdata(pdev);

	of_clk_del_provider(node);
	mtk_clk_unregister_gates(apmixed_clks, ARRAY_SIZE(apmixed_clks),
				 clk_data);
	mtk_clk_unregister_plls(plls, ARRAY_SIZE(plls), clk_data);
	mtk_free_clk_data(clk_data);
}

static const struct of_device_id of_match_clk_mt6785_apmixed[] = {
	{ .compatible = "mediatek,mt6785-apmixedsys" },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, of_match_clk_mt6785_apmixed);

static struct platform_driver clk_mt6785_apmixed_drv = {
	.probe = clk_mt6785_apmixed_probe,
	.remove = clk_mt6785_apmixed_remove,
	.driver = {
		.name = "clk-mt6785-apmixed",
		.of_match_table = of_match_clk_mt6785_apmixed,
	},
};
module_platform_driver(clk_mt6785_apmixed_drv);

MODULE_DESCRIPTION("MediaTek MT6785 apmixed clocks driver");
MODULE_LICENSE("GPL");
