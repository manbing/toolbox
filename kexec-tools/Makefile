include $(TOPDIR)/rules.mk

PKG_NAME:=kexec-tools
PKG_VERSION:=2.0.24
PKG_RELEASE:=2

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_DATA_FILENAME:=$(PKG_NAME)-$(PKG_VERSION)
PKG_DATA_ARCHIVE:=$(PKG_DATA_FILENAME).tar.gz
PKG_CONFIG_DEPENDS :=
include $(INCLUDE_DIR)/package.mk

TARGET_CFLAGS += -ggdb3

TARGET_LDFLAGS +=

## Package
define Package/kexec-tools
  SECTION:=devel
  CATEGORY:=Development
  TITLE:=directly boot into a new kernel
  DEPENDS:=+zlib
  URL:=https://git.kernel.org/pub/scm/utils/kernel/kexec/kexec-tools.git/
endef

define Package/kexec-tools/description
  It's kexec-tools description
endef

define Package/kexec-tools/install
	@echo "kexec-tools isntall"
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/sbin/kexec $(1)/usr/sbin
endef

## Build
define Build/Prepare
	@echo "kexec-tools build prepare"
	tar zxvf ./src/$(PKG_DATA_ARCHIVE) -C $(PKG_BUILD_DIR) --strip-components 1
	cd $(PKG_BUILD_DIR);./bootstrap
endef

CONFIGURE_VARS += \
	BUILD_CC="$(HOSTCC)" \
	BUILD_CFLAGS="" \
	TARGET_CC="$(TARGET_CC)" \
	TARGET_CCFLAGS="" \
	TARGET_LD="$(TARGET_CROSS)ld"

CONFIGURE_ARGS = \
	--prefix=/usr \
        --host=$(REAL_GNU_TARGET_NAME)

define Build/Compile
	@echo "kexec-tools compile"
	$(MAKE) -C $(PKG_BUILD_DIR) DESTDIR=$(PKG_INSTALL_DIR) all install
endef

$(eval $(call BuildPackage,kexec-tools))
