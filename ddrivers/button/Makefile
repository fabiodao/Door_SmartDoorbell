obj-m := button.o
button-objs:= button_driver.o utils.o
KDIR := /home/vitor/buildroots/Fabio/buildroot-2021.08/output/build/linux-custom
ARCH ?= arm

CROSS_COMPILE ?= /home/vitor/buildroots/Fabio/buildroot-2021.08/output/host/bin/arm-buildroot-linux-uclibcgnueabihf-

  

export ARCH CROSS_COMPILE


KBUILD_TARGETS	:= clean help modules modules_install

.PHONY: all $(KBUILD_TARGETS)

all: modules
$(KBUILD_TARGETS):
	#$(MAKE) -C $(KDIR) M=$(shell pwd) $@ EXTRA_CFLAGS="$(MY_CFLAGS)"
	$(MAKE) -C $(KDIR) M=$(shell pwd) $@
