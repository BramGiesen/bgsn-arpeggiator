######################################
#
# mod-arpeggiator
#
######################################

# where to find the source code - locally in this case
MOD_ARPEGGIATOR_DEV_SITE_METHOD = local
MOD_ARPEGGIATOR_DEV_SITE = $($(PKG)_PKGDIR)/

# even though this is a local build, we still need a version number
# bump this number if you need to force a rebuild
MOD_ARPEGGIATOR_DEV_VERSION = 1

# dependencies (list of other buildroot packages, separated by space)
MOD_ARPEGGIATOR_DEV_DEPENDENCIES =

# LV2 bundles that this package generates (space separated list)
MOD_ARPEGGIATOR_DEV_BUNDLES = mod-arpeggiator.lv2

# call make with the current arguments and path. "$(@D)" is the build directory.
MOD_ARPEGGIATOR_DEV_TARGET_MAKE = $(TARGET_MAKE_ENV) $(TARGET_CONFIGURE_OPTS) $(MAKE) NOOPT=true HAVE_CAIRO=false HAVE_GL=false -C $(@D)

# build command
define MOD_ARPEGGIATOR_DEV_BUILD_CMDS
	$(MOD_ARPEGGIATOR_DEV_TARGET_MAKE)
endef

# install command
define MOD_ARPEGGIATOR_DEV_INSTALL_TARGET_CMDS
	install -d $(TARGET_DIR)/usr/lib/lv2/
	cp -r $(@D)/bin/mod-arpeggiator.lv2           $(TARGET_DIR)/usr/lib/lv2/
	cp -rL $($(PKG)_PKGDIR)/mod-arpeggiator.lv2/* $(TARGET_DIR)/usr/lib/lv2/mod-arpeggiator.lv2/
endef

# import everything else from the buildroot generic package
$(eval $(generic-package))
