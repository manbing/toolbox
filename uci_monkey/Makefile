include $(TOPDIR)/rules.mk

PKG_NAME:= uci-monkey
PKG_VERSION:=0.0.1
PKG_RELEASE:=1
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

MAKE_PATH:=src
include $(INCLUDE_DIR)/package.mk

## Package
define Package/uci-monkey
  CATEGORY:=Utilities
  SUBMENU:=toolbox
  DEPENDS:=+libuci +libubus +ubus +ubusd +libubox
  TITLE:=UCI monkey test
endef

define Package/uci-monkey/description
  UCI monkey test
endef

define Package/uci-monkey/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ucimonkey $(1)/usr/sbin/
	$(CP) ./files/* $(1)/
endef


## Build
TARGET_CFLAGS +=
TARGET_LDFLAGS += -lubox -lubus -luci

define Build/Prepare
	$(CP) -rf ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	echo "cindy"
	CC="$(TARGET_CC)" CFLAGS="$(TARGET_CFLAGS)" LDFLAGS="$(TARGET_LDFLAGS)" \
	$(MAKE) -C $(PKG_BUILD_DIR)
endef

$(eval $(call BuildPackage,uci-monkey))
