/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef __SOC_MEDIATEK_MT6785_PM_DOMAINS_H
#define __SOC_MEDIATEK_MT6785_PM_DOMAINS_H

#include "mtk-pm-domains.h"
#include <dt-bindings/power/mediatek,mt6785-power.h>

/*
 * MT6785 power domain support
 */

#define MT6785_PWR_STATUS 0x160
#define MT6785_PWR_STATUS_2ND 0x164

#define MT6785_INFRA_TOPAXI_PROTECTEN_STA1 0x228
#define MT6785_INFRA_TOPAXI_PROTECTEN_SET 0x2a0
#define MT6785_INFRA_TOPAXI_PROTECTEN_CLR 0x2a4

#define MT6785_INFRA_TOPAXI_PROTECTEN_1_STA1 0x258
#define MT6785_INFRA_TOPAXI_PROTECTEN_1_SET 0x2a8
#define MT6785_INFRA_TOPAXI_PROTECTEN_1_CLR 0x2ac

#define MT6785_INFRA_TOPAXI_PROTECTEN_MM_SET 0x2d4
#define MT6785_INFRA_TOPAXI_PROTECTEN_MM_CLR 0x2d8
#define MT6785_INFRA_TOPAXI_PROTECTEN_MM_STA1 0x2ec

#define MT6785_SMI_COMMON_CLAMP_EN 0x3c0
#define MT6785_SMI_COMMON_CLAMP_EN_SET 0x3c4
#define MT6785_SMI_COMMON_CLAMP_EN_CLR 0x3c8

#define MT6785_BUS_PROT_INFRA_WR_TOPAXI(_mask)				\
	BUS_PROT_WR_IGN(INFRA, _mask,					\
			MT6785_INFRA_TOPAXI_PROTECTEN_SET,		\
			MT6785_INFRA_TOPAXI_PROTECTEN_CLR,		\
			MT6785_INFRA_TOPAXI_PROTECTEN_STA1)

#define MT6785_BUS_PROT_INFRA_WR_TOPAXI_1(_mask)				\
	BUS_PROT_WR_IGN(INFRA, _mask,					\
			MT6785_INFRA_TOPAXI_PROTECTEN_1_SET,		\
			MT6785_INFRA_TOPAXI_PROTECTEN_1_CLR,		\
			MT6785_INFRA_TOPAXI_PROTECTEN_1_STA1)

#define MT6785_BUS_PROT_INFRA_WR_TOPAXI_MM(_mask)			\
	BUS_PROT_WR_IGN(INFRA, _mask,					\
			MT6785_INFRA_TOPAXI_PROTECTEN_MM_SET,		\
			MT6785_INFRA_TOPAXI_PROTECTEN_MM_CLR,		\
			MT6785_INFRA_TOPAXI_PROTECTEN_MM_STA1)

#define MT6785_BUS_PROT_SMI_WR_CLAMP_EN(_mask)				\
	BUS_PROT_WR_IGN(SMI, _mask,					\
			MT6785_SMI_COMMON_CLAMP_EN_SET,			\
			MT6785_SMI_COMMON_CLAMP_EN_CLR,			\
			MT6785_SMI_COMMON_CLAMP_EN)

#define MT6785_BUS_PROT_WAY_EN(_set_mask, _set, _sta_mask, _sta)	\
	_BUS_PROT_STA(INFRA, INFRA_NAO, _set_mask, _set, _set,		\
		      _sta_mask, _sta,					\
		      BUS_PROT_INVERTED | BUS_PROT_REG_UPDATE)

static const struct scpsys_domain_data scpsys_domain_data_mt6785[] = {
	[MT6785_POWER_DOMAIN_AUDIO] = {
		.name = "audio",
		.sta_mask = BIT(24),
		.ctl_offs = 0x31c,
		.pwr_sta_offs = MT6785_PWR_STATUS,
		.pwr_sta2nd_offs = MT6785_PWR_STATUS_2ND,
		.sram_pdn_bits = GENMASK(8, 8),
		.sram_pdn_ack_bits = GENMASK(12, 12),
		.bp_cfg = {
			MT6785_BUS_PROT_INFRA_WR_TOPAXI(BIT(31)),
		},
	},
	[MT6785_POWER_DOMAIN_CAM] = {
		.name = "cam",
		.sta_mask = BIT(25),
		.ctl_offs = 0x324,
		.pwr_sta_offs = MT6785_PWR_STATUS,
		.pwr_sta2nd_offs = MT6785_PWR_STATUS_2ND,
		.sram_pdn_bits = GENMASK(9, 8),
		.sram_pdn_ack_bits = GENMASK(13, 12),
		.bp_cfg = {
			MT6785_BUS_PROT_INFRA_WR_TOPAXI_MM(BIT(4) | BIT(5) | BIT(9) | BIT(13)),
			MT6785_BUS_PROT_INFRA_WR_TOPAXI(BIT(28)),
			MT6785_BUS_PROT_INFRA_WR_TOPAXI_MM(BIT(11)),
			MT6785_BUS_PROT_SMI_WR_CLAMP_EN(GENMASK(7, 6)),
		},
	},
	[MT6785_POWER_DOMAIN_CONN] = {
		.name = "conn",
		.sta_mask = BIT(1),
		.ctl_offs = 0x320,
		.pwr_sta_offs = MT6785_PWR_STATUS,
		.pwr_sta2nd_offs = MT6785_PWR_STATUS_2ND,
		.bp_cfg = {
			MT6785_BUS_PROT_INFRA_WR_TOPAXI(BIT(13) | BIT(18)),
			MT6785_BUS_PROT_INFRA_WR_TOPAXI_1(BIT(10)),
			MT6785_BUS_PROT_INFRA_WR_TOPAXI(BIT(14)),
		},
	},
	[MT6785_POWER_DOMAIN_ISP] = {
		.name = "isp",
		.sta_mask = BIT(5),
		.ctl_offs = 0x308,
		.pwr_sta_offs = MT6785_PWR_STATUS,
		.pwr_sta2nd_offs = MT6785_PWR_STATUS_2ND,
		.sram_pdn_bits = GENMASK(8, 8),
		.sram_pdn_ack_bits = GENMASK(12, 12),
		.bp_cfg = {
			MT6785_BUS_PROT_INFRA_WR_TOPAXI_MM(BIT(3) | BIT(8)),
			MT6785_BUS_PROT_SMI_WR_CLAMP_EN(BIT(4)),
			MT6785_BUS_PROT_INFRA_WR_TOPAXI_MM(BIT(10)),
		},
	},
	[MT6785_POWER_DOMAIN_MD] = {
		.name = "md",
		.sta_mask = BIT(0),
		.ctl_offs = 0x318,
		.pwr_sta_offs = MT6785_PWR_STATUS,
		.pwr_sta2nd_offs = MT6785_PWR_STATUS_2ND,
		.ext_buck_iso_offs = 0x3b0,
		.ext_buck_iso_mask = GENMASK(1, 0),
		.bp_cfg = {
			MT6785_BUS_PROT_INFRA_WR_TOPAXI(BIT(7)),
			MT6785_BUS_PROT_INFRA_WR_TOPAXI_1(BIT(6)),
			MT6785_BUS_PROT_INFRA_WR_TOPAXI(BIT(3) | BIT(4)),
		},
		.caps = MTK_SCPD_EXT_BUCK_ISO | MTK_SCPD_MODEM_PWRSEQ,
	},
	[MT6785_POWER_DOMAIN_MFG] = {
		.name = "mfg",
		.sta_mask = BIT(7),
		.ctl_offs = 0x32c,
		.pwr_sta_offs = MT6785_PWR_STATUS,
		.pwr_sta2nd_offs = MT6785_PWR_STATUS_2ND,
		.sram_pdn_bits = GENMASK(9, 8),
		.sram_pdn_ack_bits = GENMASK(13, 12),
		.bp_cfg = {
			MT6785_BUS_PROT_INFRA_WR_TOPAXI_1(GENMASK(21, 19)),
			MT6785_BUS_PROT_INFRA_WR_TOPAXI(GENMASK(22, 21)),
		},
		.caps = MTK_SCPD_DOMAIN_SUPPLY,
	},
	[MT6785_POWER_DOMAIN_MFG_ASYNC] = {
		.name = "mfg_async",
		.sta_mask = BIT(4),
		.ctl_offs = 0x328,
		.pwr_sta_offs = MT6785_PWR_STATUS,
		.pwr_sta2nd_offs = MT6785_PWR_STATUS_2ND,
		.sram_pdn_bits = 0,
		.sram_pdn_ack_bits = 0,
		.caps = MTK_SCPD_DOMAIN_SUPPLY,
	},
	[MT6785_POWER_DOMAIN_MFG_CORE0] = {
		.name = "mfg_core0",
		.sta_mask = BIT(20),
		.ctl_offs = 0x330,
		.pwr_sta_offs = MT6785_PWR_STATUS,
		.pwr_sta2nd_offs = MT6785_PWR_STATUS_2ND,
		.sram_pdn_bits = GENMASK(8, 8),
		.sram_pdn_ack_bits = GENMASK(12, 12),
	},
	[MT6785_POWER_DOMAIN_MFG_CORE1] = {
		.name = "mfg_core1",
		.sta_mask = BIT(22),
		.ctl_offs = 0x334,
		.pwr_sta_offs = MT6785_PWR_STATUS,
		.pwr_sta2nd_offs = MT6785_PWR_STATUS_2ND,
		.sram_pdn_bits = GENMASK(8, 8),
		.sram_pdn_ack_bits = GENMASK(12, 12),
	},
	[MT6785_POWER_DOMAIN_MFG_CORE2] = {
		.name = "mfg_core2",
		.sta_mask = BIT(23),
		.ctl_offs = 0x338,
		.pwr_sta_offs = MT6785_PWR_STATUS,
		.pwr_sta2nd_offs = MT6785_PWR_STATUS_2ND,
		.sram_pdn_bits = GENMASK(8, 8),
		.sram_pdn_ack_bits = GENMASK(12, 12),
	},
	[MT6785_POWER_DOMAIN_MFG_CORE3] = {
		.name = "mfg_core3",
		.sta_mask = BIT(13),
		.ctl_offs = 0x350,
		.pwr_sta_offs = MT6785_PWR_STATUS,
		.pwr_sta2nd_offs = MT6785_PWR_STATUS_2ND,
		.sram_pdn_bits = GENMASK(8, 8),
		.sram_pdn_ack_bits = GENMASK(12, 12),
	},
	[MT6785_POWER_DOMAIN_MM] = {
		.name = "mm",
		.sta_mask = BIT(3),
		.ctl_offs = 0x30c,
		.pwr_sta_offs = MT6785_PWR_STATUS,
		.pwr_sta2nd_offs = MT6785_PWR_STATUS_2ND,
		.sram_pdn_bits = GENMASK(8, 8),
		.sram_pdn_ack_bits = GENMASK(12, 12),
		.bp_cfg = {
			MT6785_BUS_PROT_INFRA_WR_TOPAXI_MM(GENMASK(5, 0) & ~BIT(2)),
			MT6785_BUS_PROT_SMI_WR_CLAMP_EN(GENMASK(7, 0) & ~BIT(5)),
			MT6785_BUS_PROT_INFRA_WR_TOPAXI_1(GENMASK(17, 16)),
			MT6785_BUS_PROT_INFRA_WR_TOPAXI(GENMASK(11, 10)),
			MT6785_BUS_PROT_WAY_EN(
				MT8365_INFRA_TOPAXI_SI0_WAY_EN_MMAPB_S,
				MT8365_INFRA_TOPAXI_SI0_CTL,
				MT8365_INFRA_NAO_TOPAXI_SI0_CTRL_UPDATED,
				MT8365_INFRA_NAO_TOPAXI_SI0_STA),
			MT6785_BUS_PROT_WAY_EN(
				MT8365_INFRA_TOPAXI_SI2_WAY_EN_PERI_M1,
				MT8365_INFRA_TOPAXI_SI2_CTL,
				MT8365_INFRA_NAO_TOPAXI_SI2_CTRL_UPDATED,
				MT8365_INFRA_NAO_TOPAXI_SI2_STA),
			MT6785_BUS_PROT_INFRA_WR_TOPAXI(BIT(6)),
		},
		.caps =
			MTK_SCPD_STRICT_BUS_PROTECTION | MTK_SCPD_HAS_INFRA_NAO,
	},
	[MT6785_POWER_DOMAIN_VDEC] = {
		.name = "vdec",
		.sta_mask = BIT(31),
		.ctl_offs = 0x300,
		.pwr_sta_offs = MT6785_PWR_STATUS,
		.pwr_sta2nd_offs = MT6785_PWR_STATUS_2ND,
		.sram_pdn_bits = GENMASK(8, 8),
		.sram_pdn_ack_bits = GENMASK(12, 12),
		.bp_cfg = {
			MT6785_BUS_PROT_INFRA_WR_TOPAXI_MM(BIT(1)),
			MT6785_BUS_PROT_SMI_WR_CLAMP_EN(BIT(2)),
		},
	},
	[MT6785_POWER_DOMAIN_VENC] = {
		.name = "venc",
		.sta_mask = BIT(21),
		.ctl_offs = 0x304,
		.pwr_sta_offs = MT6785_PWR_STATUS,
		.pwr_sta2nd_offs = MT6785_PWR_STATUS_2ND,
		.sram_pdn_bits = GENMASK(11, 8),
		.sram_pdn_ack_bits = GENMASK(15, 12),
		.bp_cfg = {
			MT6785_BUS_PROT_INFRA_WR_TOPAXI_MM(BIT(0)),
			MT6785_BUS_PROT_SMI_WR_CLAMP_EN(BIT(3)),
		},
	},
};

static const struct scpsys_soc_data mt6785_scpsys_data = {
	.domains_data = scpsys_domain_data_mt6785,
	.num_domains = ARRAY_SIZE(scpsys_domain_data_mt6785),
};

#endif /* __SOC_MEDIATEK_MT6785_PM_DOMAINS_H */
