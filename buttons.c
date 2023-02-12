#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

#include <linux/pinctrl/consumer.h>

#define DRIVER_NAME "yohda-button"

irqreturn_t yohda_irq_handler(int irq, void *dev_id)
{
	pr_info("yohda button interrupt");	
	return IRQ_HANDLED;
}

static int button_pinctrl(struct platform_device *pdev)
{
	struct pinctrl *p;
	struct pinctrl_state *s;
	int ret = 0;

	p = pinctrl_get(&pdev->dev);
	if(IS_ERR(p))
		return p;

	s = pinctrl_lookup_state(p, "default");
	if(IS_ERR(s)) {
		devm_pinctrl_put(p);
		return ERR_PTR(ret);
	}

	ret = pinctrl_select_state(p ,s);
	if(ret < 0) {
		devm_pinctrl_put(p);
		return ERR_PTR(ret);
	}
			
	return 0;
}

static int button_probe(struct platform_device *pdev) 
{ 
	int err, irq_number;
	printk("===[%s][L:%d]", __func__, __LINE__);

	button_pinctrl(pdev);
	
/*
	if(irq_number <= 0)
	{
		pr_err("err: %d, Failed to get irq number from gpio number", irq_number);
		goto gpio_err;
	}

	err = request_irq(irq_number, (void *)yohda_irq_handler, IRQF_TRIGGER_FALLING, "yohda_irq_handler", NULL);
	if(err)
	{
		pr_err("Failed to register the irq handler on irq number: %d", irq_number);
		goto gpio_err;
	}

	pr_info("yohda device driver probe was successful\n");
*/

	return 0;
} 

static struct of_device_id button_ids[] = {
	{ .compatible = "yohda,button" },
	{}
};
MODULE_DEVICE_TABLE(of, button_ids);

static struct platform_driver button_driver = { 
	.probe = button_probe, 
	.driver = { 
		.owner = THIS_MODULE,
		.name = DRIVER_NAME,
		.of_match_table = button_ids,
	},
}; 
module_platform_driver(button_driver);

MODULE_LICENSE("GPL");
