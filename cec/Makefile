include $(TOPDIR)/rules.mk

PKG_NAME:=cec
PKG_RELEASE:=1

PKG_MAINTAINER:=Manbing <manbing3@gamil.com>
PKG_CHECK_FORMAT_SECURITY:=0

include $(INCLUDE_DIR)/kernel.mk
include $(INCLUDE_DIR)/package.mk

## Package
define KernelPackage/current-execution-context
  SECTION:=sys
  CATEGORY:=Kernel hacking
  SUBMENU:=toolbox
  DEPENDS:=
  TITLE:=Check context
  URL:=
  VARIANT:=
  DEPENDS:=
  FILES:=$(PKG_BUILD_DIR)/cec.ko
  KCONFIG:=
  #AUTOLOAD:=$(call AutoLoad,50,check_context)
endef

define KernelPackage/current-execution-context/description
  Check current context
endef

## Build
TARGET_CFLAGS +=
TARGET_LDFLAGS +=

define Build/Configure
endef

define Build/Compile
	+$(KERNEL_MAKE) $(PKG_JOBS) \
		M="$(PKG_BUILD_DIR)" \
		modules
endef

$(eval $(call KernelPackage,current-execution-context))
