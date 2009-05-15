TARGET_ROOTFS=/home/guohr/workdir/tmlinux/export/
THIS_DIRECTORY= $(shell pwd)

install: clean
	cp -fr fs/etc/ $(TARGET_ROOTFS)/
	cd $(TARGET_ROOTFS); sudo $(THIS_DIRECTORY)/fs-install 

clean:
	sudo rm -fr $(TARGET_ROOTFS)/etc $(TARGET_ROOTFS)/dev/


