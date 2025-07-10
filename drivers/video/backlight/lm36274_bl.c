// SPDX-License-Identifier: GPL-2.0
// TI LM36274 Backlight chip family driver
// Copyright (C) 2019 Texas Instruments Incorporated - https://www.ti.com/

#include <linux/backlight.h>
#include <linux/bitops.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/leds-ti-lmu-common.h>
#include <linux/mod_devicetable.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/property.h>

#include <linux/mfd/ti-lmu.h>
#include <linux/mfd/ti-lmu-register.h>

#define LM36274_MAX_STRINGS	4
#define LM36274_BL_EN		BIT(4)

#define LM36274_OVP_SHIFT	5
#define LM36274_OVP_MASK	GENMASK(7, LM36274_OVP_SHIFT)

/**
 * enum @lm36274_ovp_threshold - Over Voltage Protection (OVP) threshold
 */
enum lm36274_ovp_threshold {
	LM36274_OVP_17V = 0,
	LM36274_OVP_21V = 1,
	LM36274_OVP_25V = 2,
	LM36274_OVP_29V = 3,
};

#define LM36274_RAMP_TIME_SHIFT	3
#define LM36274_RAMP_TIME_MASK	GENMASK(6, LM36274_RAMP_TIME_SHIFT)

/**
 * enum @lm36274_ramp_time - Ramp time for brightness change
 */
enum lm36274_ramp_time {
	LM36274_RAMP_TIME_0US = 0,
	LM36274_RAMP_TIME_500US = 1,
	LM36274_RAMP_TIME_750US = 2,
	LM36274_RAMP_TIME_1MS = 3,
	LM36274_RAMP_TIME_2MS = 4,
	LM36274_RAMP_TIME_5MS = 5,
	LM36274_RAMP_TIME_10MS = 6,
	LM36274_RAMP_TIME_20MS = 7,
	LM36274_RAMP_TIME_50MS = 8,
	LM36274_RAMP_TIME_100MS = 9,
	LM36274_RAMP_TIME_250MS = 10,
	LM36274_RAMP_TIME_800MS = 11,
	LM36274_RAMP_TIME_1S = 12,
	LM36274_RAMP_TIME_2S = 13,
	LM36274_RAMP_TIME_4S = 14,
	LM36274_RAMP_TIME_8S = 15,
};

#define LM36274_BLED_MAP_SHIFT	3
#define LM36274_BLED_MAP_MASK	GENMASK(3, LM36274_BLED_MAP_SHIFT)

/**
 * struct lm36274
 * @pdev: platform device
 * @bl_dev: backlight device
 * @lmu_data: Register and setting values for common code
 * @regmap: Devices register map
 * @dev: Pointer to the devices device struct
 * @led_sources: The LED strings supported in this array
 * @num_leds: Number of LED strings are supported in this array
 * @ramp_time: Ramp time for brightness change
 * @ovp: Over Voltage Protection (OVP) threshold
 * @exp_mode: Exponential mode for brightness control
 */
struct lm36274 {
	struct platform_device *pdev;
	struct backlight_device *bl_dev;
	struct ti_lmu_bank lmu_data;
	struct regmap *regmap;
	struct device *dev;

	u32 led_sources[LM36274_MAX_STRINGS];
	int num_leds;

	enum lm36274_ramp_time ramp_time;
	enum lm36274_ovp_threshold ovp;
	bool exp_mode;
};

static int lm36274_bl_update_status(struct backlight_device *bl_dev)
{
	struct lm36274 *chip = bl_get_data(bl_dev);
	int brt_val = backlight_get_brightness(bl_dev);

	return ti_lmu_common_set_brightness(&chip->lmu_data, brt_val);
}

static const struct backlight_ops lm36274_bl_ops = {
	.options = BL_CORE_SUSPENDRESUME,
	.update_status = lm36274_bl_update_status,
};

static int lm36274_init(struct lm36274 *chip)
{
	int enable_val = 0;
	int ret;
	int i;

	for (i = 0; i < chip->num_leds; i++)
		enable_val |= (1 << chip->led_sources[i]);

	if (!enable_val) {
		dev_err(chip->dev, "No LEDs were enabled\n");
		return -EINVAL;
	}

	enable_val |= LM36274_BL_EN;

	ret = regmap_write_bits(chip->regmap, LM36274_REG_BL_CFG_1,
				LM36274_BLED_MAP_MASK,
				!chip->exp_mode << LM36274_BLED_MAP_SHIFT);
	if (ret)
		return ret;

	ret = regmap_write_bits(chip->regmap, LM36274_REG_BL_CFG_1,
				LM36274_OVP_MASK,
				chip->ovp << LM36274_OVP_SHIFT);
	if (ret)
		return ret;

	ret = regmap_write_bits(chip->regmap, LM36274_REG_BL_CFG_2,
				LM36274_RAMP_TIME_MASK,
				chip->ramp_time << LM36274_RAMP_TIME_SHIFT);
	if (ret)
		return ret;

	return regmap_write(chip->regmap, LM36274_REG_BL_EN, enable_val);
}

static int lm36274_parse_dt(struct lm36274 *chip,
			    struct led_init_data *init_data,
			    struct backlight_properties *props)
{
	struct device *dev = chip->dev;
	struct fwnode_handle *child;
	u32 brightness;
	u32 ramp_time;
	u32 ovp;
	int ret;

	/* There should only be 1 node */
	if (device_get_child_node_count(dev) != 1)
		return -EINVAL;

	child = device_get_next_child_node(dev, NULL);

	init_data->fwnode = child;
	init_data->devicename = chip->pdev->name;
	/* for backwards compatibility when `label` property is not present */
	init_data->default_label = ":";

	chip->num_leds = fwnode_property_count_u32(child, "led-sources");
	if (chip->num_leds <= 0) {
		ret = -ENODEV;
		goto err;
	}

	ret = fwnode_property_read_u32_array(child, "led-sources",
					     chip->led_sources, chip->num_leds);
	if (ret) {
		dev_err(dev, "led-sources property missing\n");
		goto err;
	}

	ret = fwnode_property_read_u32(child, "max-brightness", &brightness);
	if (ret)
		brightness = MAX_BRIGHTNESS_11BIT;

	props->max_brightness = min_t(u32, brightness, MAX_BRIGHTNESS_11BIT);

	ret = fwnode_property_read_u32(child, "default-brightness",
				       &brightness);
	if (ret)
		brightness = props->max_brightness;

	props->brightness = min_t(u32, brightness, props->max_brightness);

	chip->exp_mode =
		fwnode_property_read_bool(child, "ti,exponential-mode");
	props->scale = chip->exp_mode ? BACKLIGHT_SCALE_NON_LINEAR :
					BACKLIGHT_SCALE_LINEAR;

	ret = fwnode_property_read_u32(child, "ti,ovp-microvolt", &ovp);
	if (ret) {
		chip->ovp = LM36274_OVP_21V;
	} else {
		switch (ovp) {
		case 17000000:
			chip->ovp = LM36274_OVP_17V;
			break;
		case 21000000:
			chip->ovp = LM36274_OVP_21V;
			break;
		case 25000000:
			chip->ovp = LM36274_OVP_25V;
			break;
		case 29000000:
			chip->ovp = LM36274_OVP_29V;
			break;
		default:
			dev_err(dev, "Invalid OVP %d\n", ovp);
			return -EINVAL;
		}
	}

	ret = fwnode_property_read_u32(child, "ti,ramp-time-us", &ramp_time);
	if (ret) {
		chip->ramp_time = LM36274_RAMP_TIME_500US;
	} else {
		switch (ramp_time) {
		case 0:
			chip->ramp_time = LM36274_RAMP_TIME_0US;
			break;
		case 500:
			chip->ramp_time = LM36274_RAMP_TIME_500US;
			break;
		case 750:
			chip->ramp_time = LM36274_RAMP_TIME_750US;
			break;
		case 1000:
			chip->ramp_time = LM36274_RAMP_TIME_1MS;
			break;
		case 2000:
			chip->ramp_time = LM36274_RAMP_TIME_2MS;
			break;
		case 5000:
			chip->ramp_time = LM36274_RAMP_TIME_5MS;
			break;
		case 10000:
			chip->ramp_time = LM36274_RAMP_TIME_10MS;
			break;
		case 20000:
			chip->ramp_time = LM36274_RAMP_TIME_20MS;
			break;
		case 50000:
			chip->ramp_time = LM36274_RAMP_TIME_50MS;
			break;
		case 100000:
			chip->ramp_time = LM36274_RAMP_TIME_100MS;
			break;
		case 250000:
			chip->ramp_time = LM36274_RAMP_TIME_250MS;
			break;
		case 800000:
			chip->ramp_time = LM36274_RAMP_TIME_800MS;
			break;
		case 1000000:
			chip->ramp_time = LM36274_RAMP_TIME_1S;
			break;
		case 2000000:
			chip->ramp_time = LM36274_RAMP_TIME_2S;
			break;
		case 4000000:
			chip->ramp_time = LM36274_RAMP_TIME_4S;
			break;
		case 8000000:
			chip->ramp_time = LM36274_RAMP_TIME_8S;
			break;
		default:
			dev_err(dev, "Invalid ramp time %d\n", ramp_time);
			return -EINVAL;
		}
	}

	return 0;
err:
	fwnode_handle_put(child);
	return ret;
}

static int lm36274_probe(struct platform_device *pdev)
{
	struct ti_lmu *lmu = dev_get_drvdata(pdev->dev.parent);
	struct led_init_data init_data = {};
	struct lm36274 *chip;
	int ret;

	struct backlight_properties props = {
		.type = BACKLIGHT_RAW,
	};

	chip = devm_kzalloc(&pdev->dev, sizeof(*chip), GFP_KERNEL);
	if (!chip)
		return -ENOMEM;

	chip->pdev = pdev;
	chip->dev = &pdev->dev;
	chip->regmap = lmu->regmap;
	platform_set_drvdata(pdev, chip);

	ret = lm36274_parse_dt(chip, &init_data, &props);
	if (ret) {
		dev_err(chip->dev, "Failed to parse DT node\n");
		return ret;
	}

	ret = lm36274_init(chip);
	if (ret) {
		fwnode_handle_put(init_data.fwnode);
		dev_err(chip->dev, "Failed to init the device\n");
		return ret;
	}

	chip->lmu_data.regmap = chip->regmap;
	chip->lmu_data.max_brightness = MAX_BRIGHTNESS_11BIT;
	chip->lmu_data.msb_brightness_reg = LM36274_REG_BRT_MSB;
	chip->lmu_data.lsb_brightness_reg = LM36274_REG_BRT_LSB;

	chip->bl_dev = devm_backlight_device_register(&pdev->dev, pdev->name,
						      &pdev->dev, chip,
						      &lm36274_bl_ops, &props);
	if (IS_ERR(chip->bl_dev))
		dev_err(chip->dev,
			"Failed to register backlight for node %pfw\n",
			init_data.fwnode);

	fwnode_handle_put(init_data.fwnode);
	backlight_update_status(chip->bl_dev);

	return ret;
}

static const struct of_device_id of_lm36274_leds_match[] = {
	{ .compatible = "ti,lm36274-backlight", },
	{},
};
MODULE_DEVICE_TABLE(of, of_lm36274_leds_match);

static struct platform_driver lm36274_driver = {
	.probe  = lm36274_probe,
	.driver = {
		.name = "lm36274-backlight",
		.of_match_table = of_lm36274_leds_match,
	},
};
module_platform_driver(lm36274_driver);

MODULE_DESCRIPTION("Texas Instruments LM36274 Backlight driver");
MODULE_AUTHOR("Dan Murphy <dmurphy@ti.com>");
MODULE_LICENSE("GPL v2");
