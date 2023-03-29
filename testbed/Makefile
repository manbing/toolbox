include $(TOPDIR)/rules.mk
 
PKG_NAME:=testbed
PKG_VERSION:=0.0.1
PKG_RELEASE:=1
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

MAKE_PATH:=src
include $(INCLUDE_DIR)/package.mk

## Package
define Package/testbed
  CATEGORY:=Utilities
  SUBMENU:=toolbox
  DEPENDS:=+libuci +libubus +ubus +ubusd +libubox +procd +cJSON
  TITLE:=for test
endef
 
define Package/testbed/description
  cindy irene
endef
 
define Package/testbed/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/testbed $(1)/usr/sbin/
	$(CP) ./files/* $(1)/
endef

## Build
TARGET_CFLAGS +=
TARGET_LDFLAGS += -lubox -lubus -luci -lblobmsg_json -lcjson

define Build/Prepare
	$(CP) -rf ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	CC="$(TARGET_CC)" CFLAGS="$(TARGET_CFLAGS)" LDFLAGS="$(TARGET_LDFLAGS)" \
        $(MAKE) -C $(PKG_BUILD_DIR)
endef
 
$(eval $(call BuildPackage,testbed))
