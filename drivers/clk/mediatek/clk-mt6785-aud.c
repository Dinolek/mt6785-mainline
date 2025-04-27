// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2025 Dinolek <git@dinolek.me>
 */

#include <dt-bindings/clock/mediatek,mt6785-clk.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>

#include "clk-gate.h"
#include "clk-mtk.h"

static const struct mtk_gate_regs aud0_cg_regs = {
	.sta_ofs = 0x0,
	.clr_ofs = 0x0,
	.set_ofs = 0x0,
};

static const struct mtk_gate_regs aud1_cg_regs = {
	.sta_ofs = 0x4,
	.clr_ofs = 0x4,
	.set_ofs = 0x4,
};

#define GATE_AUD0(_id, _name, _parent, _shift)               \
	GATE_MTK(_id, _name, _parent, &aud0_cg_regs, _shift, \
		 &mtk_clk_gate_ops_no_setclr)

#define GATE_AUD1(_id, _name, _parent, _shift)               \
	GATE_MTK(_id, _name, _parent, &aud1_cg_regs, _shift, \
		 &mtk_clk_gate_ops_no_setclr)

static const struct mtk_gate aud_clks[] = {
	GATE_AUD0(CLK_AUD_AFE, "aud_afe", "top_audio", 2),
	GATE_AUD0(CLK_AUD_22M, "aud_22m", "top_audio_eng1", 8),
	GATE_AUD0(CLK_AUD_24M, "aud_24m", "top_audio_eng2", 9),
	GATE_AUD0(CLK_AUD_APLL2_TUNER, "aud_apll2_tuner", "top_audio_eng2", 18),
	GATE_AUD0(CLK_AUD_APLL_TUNER, "aud_apll_tuner", "top_audio_eng1", 19),
	GATE_AUD0(CLK_AUD_TDM, "aud_tdm", "top_audio_eng1", 20),
	GATE_AUD0(CLK_AUD_ADC, "aud_adc", "top_audio", 24),
	GATE_AUD0(CLK_AUD_DAC, "aud_dac", "top_audio", 25),
	GATE_AUD0(CLK_AUD_DAC_PREDIS, "aud_dac_predis", "top_audio", 26),
	GATE_AUD0(CLK_AUD_TML, "aud_tml", "top_audio", 27),
	GATE_AUD0(CLK_AUD_NLE, "aud_nle", "top_audio", 28),

	GATE_AUD1(CLK_AUD_I2S1, "aud_i2s1", "top_audio", 4),
	GATE_AUD1(CLK_AUD_I2S2, "aud_i2s2", "top_audio", 5),
	GATE_AUD1(CLK_AUD_I2S3, "aud_i2s3", "top_audio", 6),
	GATE_AUD1(CLK_AUD_I2S4, "aud_i2s4", "top_audio", 7),
	GATE_AUD1(CLK_AUD_I2S5, "aud_i2s5", "top_audio", 8),
	GATE_AUD1(CLK_AUD_CONN_I2S, "aud_conn_i2s", "top_audio", 12),
	GATE_AUD1(CLK_AUD_GENERAL1, "aud_general1", "top_audio", 13),
	GATE_AUD1(CLK_AUD_GENERAL2, "aud_general2", "top_audio", 14),
	GATE_AUD1(CLK_AUD_DAC_HIRES, "aud_dac_hires", "top_audio_hires", 15),
	GATE_AUD1(CLK_AUD_ADC_HIRES, "aud_adc_hires", "top_audio_hires", 16),
	GATE_AUD1(CLK_AUD_ADC_HIRES_TML, "aud_adc_hires_tml", "top_audio_hires", 17),
	GATE_AUD1(CLK_AUD_ADDA6_ADC, "aud_adda6_adc", "top_audio", 20),
	GATE_AUD1(CLK_AUD_ADDA6_ADC_HIRES, "aud_adda6_adc_hires", "top_audio_hires", 21),
	GATE_AUD1(CLK_AUD_3RD_DAC, "aud_3rd_dac", "top_audio", 28),
	GATE_AUD1(CLK_AUD_3RD_DAC_PREDIS, "aud_3rd_dac_predis", "top_audio", 29),
	GATE_AUD1(CLK_AUD_3RD_DAC_TML, "aud_3rd_dac_tml", "top_audio", 30),
	GATE_AUD1(CLK_AUD_3RD_DAC_HIRES, "aud_3rd_dac_hires", "top_audio_hires", 31),
};

static const struct mtk_clk_desc aud_desc = {
	.clks = aud_clks,
	.num_clks = ARRAY_SIZE(aud_clks),
};

static int clk_mt6785_aud_probe(struct platform_device *pdev)
{
	int r;

	r = mtk_clk_simple_probe(pdev);
	if (r)
		return r;

	r = devm_of_platform_populate(&pdev->dev);
	if (r)
		mtk_clk_simple_remove(pdev);

	return r;
}

static void clk_mt6785_aud_remove(struct platform_device *pdev)
{
	of_platform_depopulate(&pdev->dev);
	mtk_clk_simple_remove(pdev);
}

static const struct of_device_id of_match_clk_mt6785_aud[] = {
	{ .compatible = "mediatek,mt6785-audsys", .data = &aud_desc },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, of_match_clk_mt6785_aud);

static struct platform_driver clk_mt6785_aud_drv = {
	.probe = clk_mt6785_aud_probe,
	.remove = clk_mt6785_aud_remove,
	.driver = {
		.name = "clk-mt6785-aud",
		.of_match_table = of_match_clk_mt6785_aud,
	},
};
module_platform_driver(clk_mt6785_aud_drv);

MODULE_DESCRIPTION("MediaTek MT6785 Audio clocks driver");
MODULE_LICENSE("GPL");
