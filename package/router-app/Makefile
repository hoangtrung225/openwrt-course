include	$(TOPDIR)/rules.mk	
PKG_NAME:=router-app
  
PKG_RELEASE:=1
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
include $(INCLUDE_DIR)/package.mk

define Package/router-app	
	SECTION:=utils	
	CATEGORY:=Customize	
	TITLE:=Application for router
	DEPENDS:=+libzmq +czmq 
endef

define Package/router-app/description  
	Application for smart router control
endef	

define  Build/Prepare	
	mkdir -p $(PKG_BUILD_DIR)	
	$(CP) ./src/* $(PKG_BUILD_DIR)/	
endef

define Package/router-app/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/server $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/client $(1)/usr/bin/
endef

$(eval $(call BuildPackage,router-app))	
