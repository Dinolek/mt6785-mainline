// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2025 Dinolek <git@dinolek.me>
 */

#include <dt-bindings/clock/mediatek,mt6785-clk.h>
#include <linux/platform_device.h>

#include "clk-gate.h"
#include "clk-mtk.h"

static const struct mtk_gate_regs mm0_cg_regs = {
	.sta_ofs = 0x100,
	.clr_ofs = 0x108,
	.set_ofs = 0x104,
};

static const struct mtk_gate_regs mm1_cg_regs = {
	.sta_ofs = 0x110,
	.clr_ofs = 0x118,
	.set_ofs = 0x114,
};

#define GATE_MM0(_id, _name, _parent, _shift)               \
	GATE_MTK(_id, _name, _parent, &mm0_cg_regs, _shift, \
		 &mtk_clk_gate_ops_setclr)

#define GATE_MM1(_id, _name, _parent, _shift)               \
	GATE_MTK(_id, _name, _parent, &mm1_cg_regs, _shift, \
		 &mtk_clk_gate_ops_setclr)

static const struct mtk_gate mm_clks[] = {
	GATE_MM0(CLK_MM_SMI_COMMON, "mm_smi_common", "top_mm", 0),
	GATE_MM0(CLK_MM_SMI_LARB0, "mm_smi_larb0", "top_mm", 1),
	GATE_MM0(CLK_MM_SMI_LARB1, "mm_smi_larb1", "top_mm", 2),
	GATE_MM0(CLK_MM_GALS_COMM0, "mm_gals_comm0", "top_mm", 3),
	GATE_MM0(CLK_MM_GALS_COMM1, "mm_gals_comm1", "top_mm", 4),
	GATE_MM0(CLK_MM_GALS_CCU2MM, "mm_gals_ccu2mm", "top_mm", 5),
	GATE_MM0(CLK_MM_GALS_IPU12MM, "mm_gals_ipu12mm", "top_mm", 6),
	GATE_MM0(CLK_MM_GALS_IMG2MM, "mm_gals_img2mm", "top_mm", 7),
	GATE_MM0(CLK_MM_GALS_CAM2MM, "mm_gals_cam2mm", "top_mm", 8),
	GATE_MM0(CLK_MM_GALS_IPU2MM, "mm_gals_ipu2mm", "top_mm", 9),
	GATE_MM0(CLK_MM_MDP_DL_TX, "mm_mdp_dl_tx", "top_mm", 10),
	GATE_MM0(CLK_MM_IPU_DL_TX, "mm_ipu_dl_tx", "top_mm", 11),
	GATE_MM0(CLK_MM_MDP_RDMA0, "mm_mdp_rdma0", "top_mm", 12),
	GATE_MM0(CLK_MM_MDP_RDMA1, "mm_mdp_rdma1", "top_mm", 13),
	GATE_MM0(CLK_MM_MDP_RSZ0, "mm_mdp_rsz0", "top_mm", 14),
	GATE_MM0(CLK_MM_MDP_RSZ1, "mm_mdp_rsz1", "top_mm", 15),
	GATE_MM0(CLK_MM_MDP_TDSHP, "mm_mdp_tdshp", "top_mm", 16),
	GATE_MM0(CLK_MM_MDP_WROT0, "mm_mdp_wrot0", "top_mm", 17),
	GATE_MM0(CLK_MM_MDP_WROT1, "mm_mdp_wrot1", "top_mm", 18),
	GATE_MM0(CLK_MM_FAKE_ENG, "mm_fake_eng", "top_mm", 19),
	GATE_MM0(CLK_MM_DISP_OVL0, "mm_disp_ovl0", "top_mm", 20),
	GATE_MM0(CLK_MM_DISP_OVL0_2L, "mm_disp_ovl0_2l", "top_mm", 21),
	GATE_MM0(CLK_MM_DISP_OVL1_2L, "mm_disp_ovl1_2l", "top_mm", 22),
	GATE_MM0(CLK_MM_DISP_RDMA0, "mm_disp_rdma0", "top_mm", 23),
	GATE_MM0(CLK_MM_DISP_RDMA1, "mm_disp_rdma1", "top_mm", 24),
	GATE_MM0(CLK_MM_DISP_WDMA0, "mm_disp_wdma0", "top_mm", 25),
	GATE_MM0(CLK_MM_DISP_COLOR0, "mm_disp_color0", "top_mm", 26),
	GATE_MM0(CLK_MM_DISP_CCORR0, "mm_disp_ccorr0", "top_mm", 27),
	GATE_MM0(CLK_MM_DISP_AAL0, "mm_disp_aal0", "top_mm", 28),
	GATE_MM0(CLK_MM_DISP_GAMMA0, "mm_disp_gamma0", "top_mm", 29),
	GATE_MM0(CLK_MM_DISP_DITHER0, "mm_disp_dither0", "top_mm", 30),
	GATE_MM0(CLK_MM_DISP_SPLIT, "mm_disp_split", "top_mm", 31),

	GATE_MM1(CLK_MM_DSI_MM, "mm_dsi_mm", "top_mm", 0),
	GATE_MM1(CLK_MM_DSI_IF, "mm_dsi_if", "top_mm", 1),
	GATE_MM1(CLK_MM_DPI_MM, "mm_dpi_mm", "top_mm", 2),
	GATE_MM1(CLK_MM_DPI_IF, "mm_dpi_if", "top_dpi", 3),
	GATE_MM1(CLK_MM_FAKE_ENG2, "mm_fake_eng2", "top_mm", 4),
	GATE_MM1(CLK_MM_MDP_DL_RX, "mm_mdp_dl_rx", "top_mm", 5),
	GATE_MM1(CLK_MM_IPU_DL_RX, "mm_ipu_dl_rx", "top_mm", 6),
	GATE_MM1(CLK_MM_26M, "mm_26m", "clk26m", 7),
	GATE_MM1(CLK_MM_R2Y, "mm_r2y", "top_mm", 8),
	GATE_MM1(CLK_MM_DISP_RSZ, "mm_disp_rsz", "top_mm", 9),
	GATE_MM1(CLK_MM_MDP_AAL, "mm_mdp_aal", "top_mm", 10),
	GATE_MM1(CLK_MM_MDP_HDR, "mm_mdp_hdr", "top_mm", 11),
	GATE_MM1(CLK_MM_DBI_MM, "mm_dbi_mm", "top_mm", 12),
	GATE_MM1(CLK_MM_DBI_IF, "mm_dbi_if", "top_dpi", 13),
	GATE_MM1(CLK_MM_DISP_POSTMASK0, "mm_disp_postmask0", "top_mm", 14),
	GATE_MM1(CLK_MM_DISP_HRT_BW, "mm_disp_hrt_bw", "top_mm", 15),
	GATE_MM1(CLK_MM_DISP_OVL_FBDC, "mm_disp_ovl_fbdc", "top_mm", 16),
};

static const struct mtk_clk_desc mm_desc = {
	.clks = mm_clks,
	.num_clks = ARRAY_SIZE(mm_clks),
};

static const struct platform_device_id clk_mt6785_mm_id_table[] = {
	{ .name = "clk-mt6785-mm", .driver_data = (kernel_ulong_t)&mm_desc },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(platform, clk_mt6785_mm_id_table);

static struct platform_driver clk_mt6785_mm_drv = {
	.probe = mtk_clk_pdev_probe,
	.remove = mtk_clk_pdev_remove,
	.driver = {
		.name = "clk-mt6785-mm",
	},
	.id_table = clk_mt6785_mm_id_table,
};
module_platform_driver(clk_mt6785_mm_drv);

MODULE_DESCRIPTION("MediaTek MT6785 MultiMedia clocks driver");
MODULE_LICENSE("GPL");
