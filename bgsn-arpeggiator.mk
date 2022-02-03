######################################
#
# bgsn-arpeggiator
#
######################################

# where to find the source code - locally in this case
BGSN_ARPEGGIATOR_SITE_METHOD = local
BGSN_ARPEGGIATOR_SITE = $($(PKG)_PKGDIR)/

# even though this is a local build, we still need a version number
# bump this number if you need to force a rebuild
BGSN_ARPEGGIATOR_VERSION = 1

# dependencies (list of other buildroot packages, separated by space)
BGSN_ARPEGGIATOR_DEPENDENCIES =

# LV2 bundles that this package generates (space separated list)
BGSN_ARPEGGIATOR_BUNDLES = bgsn-arpeggiator.lv2

# call make with the current arguments and path. "$(@D)" is the build directory.
BGSN_ARPEGGIATOR_TARGET_MAKE = $(TARGET_MAKE_ENV) $(TARGET_CONFIGURE_OPTS) $(MAKE) NOOPT=true HAVE_CAIRO=false HAVE_GL=false -C $(@D)

# build command
define BGSN_ARPEGGIATOR_BUILD_CMDS
	$(BGSN_ARPEGGIATOR_TARGET_MAKE)
endef

# install command
define BGSN_ARPEGGIATOR_INSTALL_TARGET_CMDS
	install -d $(TARGET_DIR)/usr/lib/lv2/
	cp -r $(@D)/bin/bgsn-arpeggiator.lv2           $(TARGET_DIR)/usr/lib/lv2/
endef

# import everything else from the buildroot generic package
$(eval $(generic-package))
