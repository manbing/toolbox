include $(TOPDIR)/rules.mk
 
PKG_NAME:=waninfo
PKG_VERSION:=0.0.1
PKG_RELEASE:=1
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

MAKE_PATH:=src
include $(INCLUDE_DIR)/package.mk

## Package
define Package/waninfo
  CATEGORY:=Utilities
  SUBMENU:=toolbox
  DEPENDS:=+libuci +libubus +ubus +ubusd +libubox +procd +cJSON
  TITLE:=get WAN info
endef
 
define Package/waninfo/description
  get WAN inforatmion. like, IP, DNS, gateway and so on
endef
 
define Package/waninfo/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/waninfo $(1)/usr/sbin/
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
 
$(eval $(call BuildPackage,waninfo))
