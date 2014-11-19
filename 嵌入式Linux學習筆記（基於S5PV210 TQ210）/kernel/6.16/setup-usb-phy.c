/*
 * Copyright (C) 2012 Samsung Electronics Co.Ltd
 * Author: Joonyoung Shim <jy0922.shim@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundationr
 */

#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/platform_device.h>

#include <mach/map.h>

#include <plat/cpu.h>
#include <plat/regs-usb-hsotg-phy.h>
#include <plat/usb-phy.h>

#define S5PV210_USB_PHY_CON	(S3C_VA_SYS + 0xE80C)
#define S5PV210_USB_PHY0_EN	(1 << 0)
#define S5PV210_USB_PHY1_EN	(1 << 1)

static int s5pv210_usb_otgphy_init(struct platform_device *pdev)
{
	struct clk *xusbxti;
	u32 phyclk;

	writel(readl(S5PV210_USB_PHY_CON) | S5PV210_USB_PHY0_EN,
			S5PV210_USB_PHY_CON);

	/* set clock frequency for PLL */
	phyclk = readl(S3C_PHYCLK) & ~S3C_PHYCLK_CLKSEL_MASK;

	xusbxti = clk_get(&pdev->dev, "xusbxti");
	if (xusbxti && !IS_ERR(xusbxti)) {
		switch (clk_get_rate(xusbxti)) {
		case 12 * MHZ:
			phyclk |= S3C_PHYCLK_CLKSEL_12M;
			break;
		case 24 * MHZ:
			phyclk |= S3C_PHYCLK_CLKSEL_24M;
			break;
		default:
		case 48 * MHZ:
			/* default reference clock */
			break;
		}
		clk_put(xusbxti);
	}

	/* TODO: select external clock/oscillator */
	writel(phyclk | S3C_PHYCLK_CLK_FORCE, S3C_PHYCLK);

	/* set to normal OTG PHY */
	writel((readl(S3C_PHYPWR) & ~S3C_PHYPWR_NORMAL_MASK), S3C_PHYPWR);
	mdelay(1);

	/* reset OTG PHY and Link */
	writel(S3C_RSTCON_PHY | S3C_RSTCON_HCLK | S3C_RSTCON_PHYCLK,
			S3C_RSTCON);
	udelay(20);	/* at-least 10uS */
	writel(0, S3C_RSTCON);

	return 0;
}

static int s5pv210_usb_otgphy_exit(struct platform_device *pdev)
{
	writel((readl(S3C_PHYPWR) | S3C_PHYPWR_ANALOG_POWERDOWN |
				S3C_PHYPWR_OTG_DISABLE), S3C_PHYPWR);

	writel(readl(S5PV210_USB_PHY_CON) & ~S5PV210_USB_PHY0_EN,
			S5PV210_USB_PHY_CON);

	return 0;
}

/* add by zjh */
static int s5pv210_usb_phy1_init(struct platform_device *pdev)
{
	int err;  
    struct clk *otg_clk;  
  
    otg_clk = clk_get(&pdev->dev, "otg");  
    if (IS_ERR(otg_clk)) {  
        dev_err(&pdev->dev, "Failed to get otg clock\n");  
        return PTR_ERR(otg_clk);  
    }     
  
    err = clk_enable(otg_clk);  
    if (err) {  
        clk_put(otg_clk);  
        return err;  
    }
  
    writel(readl(S5PV210_USB_PHY_CON) | S5PV210_USB_PHY1_EN, S5PV210_USB_PHY_CON);
	writel((readl(S3C_PHYPWR) & ~(0x1<<7) & ~(0x1<<6)), S3C_PHYPWR);
    writel((readl(S3C_PHYCLK) & ~(0x1<<7)) | (0x3<<0), S3C_PHYCLK);
    writel((readl(S3C_RSTCON)) | (0x1<<4) | (0x1<<3), S3C_RSTCON);  
    writel(readl(S3C_RSTCON) & ~(0x1<<4) & ~(0x1<<3), S3C_RSTCON);
    /* "at least 10uS" for PHY reset elsewhere, 20 not enough here... */  
    udelay(50);  
  
    clk_disable(otg_clk);  
    clk_put(otg_clk);  
  
    return 0;  
}

/* add by zjh */
static int s5pv210_usb_phy1_exit(struct platform_device *pdev)
{
	writel((readl(S3C_PHYPWR) | (1 << 6) |(1 << 7)), S3C_PHYPWR);

	writel(readl(S5PV210_USB_PHY_CON) & ~S5PV210_USB_PHY1_EN,
			S5PV210_USB_PHY_CON);

	return 0;
}

/* modied by zjh */
int s5p_usb_phy_init(struct platform_device *pdev, int type)
{
	if (type == USB_PHY_TYPE_DEVICE)
		return s5pv210_usb_otgphy_init(pdev);
	else if (type == USB_PHY_TYPE_HOST)
		return s5pv210_usb_phy1_init(pdev);

	return -EINVAL;
}

/* modied by zjh */
int s5p_usb_phy_exit(struct platform_device *pdev, int type)
{
	if (type == USB_PHY_TYPE_DEVICE)
		return s5pv210_usb_otgphy_exit(pdev);
	else if (type == USB_PHY_TYPE_HOST)	
		return s5pv210_usb_phy1_exit(pdev);

	return -EINVAL;
}
