#!/bin/sh

INSTALL_PATH=$(pwd)
TARGET_ROOTFS="${INSTALL_PATH}/raspbian-rootfs"
TOOLCHAIN_SYMLINK_DIR="${INSTALL_PATH}/gcc-linaro-arm-linux-gnueabihf"
ARCH="armv8"

for i in "$@"
do
case $i in
    -a=*|--arch=*)
        ARCH="${i#*=}"
    ;;
    *)
        # unknown option
    ;;
esac
done

# Install arm-linux-gnueabihf-ldd
install_crosstool_ng_ldd() {
	local CT_VERSION="origin/master"
	local CT_TARGET="arm-linux-gnueabihf"
	local CT_ARCH_BITNESS="32"
	local bash="/bin/bash"

	# Install 'arm-linux-gnueabihf-ldd' script
	wget -nv -c -O /tmp/xldd.in https://raw.githubusercontent.com/crosstool-ng/crosstool-ng/master/scripts/xldd.in
	sed -r -e 's|@@CT_VERSION@@|'"${CT_VERSION}"'|g;'   \
		   -e 's|@@CT_TARGET@@|'"${CT_TARGET}"'|g;'     \
		   -e 's|@@CT_BITS@@|'"${CT_ARCH_BITNESS}"'|g;' \
		   -e 's|@@CT_install@@|'"install"'|g;'         \
		   -e 's|@@CT_bash@@|'"${bash}"'|g;'            \
		   -e 's|@@CT_grep@@|'"grep"'|g;'               \
		   -e 's|@@CT_make@@|'"make"'|g;'               \
		   -e 's|@@CT_sed@@|'"sed"'|g;'                 \
		   "/tmp/xldd.in"                               \
		   > "/tmp/${CT_TARGET}-ldd"
	chmod 755 "/tmp/${CT_TARGET}-ldd"
	# Uncomment if you want to hard code the --root path in 'arm-linux-gnueabihf-ldd' script
	#sed -i '/^root="\${CT_XLDD_ROOT}"/i CT_XLDD_ROOT="'"${TARGET_ROOTFS}"'"' "/tmp/${CT_TARGET}-ldd"
	mv -f "/tmp/${CT_TARGET}-ldd" "${TOOLCHAIN_SYMLINK_DIR}/bin/${CT_TARGET}-ldd"

	# fix arm-linux-gnueabihf-ldd dependencies /etc/ld.so.conf, if libc-bin is not installed
	if [ ! -f "${TARGET_ROOTFS}/etc/ld.so.conf" ]; then
		mkdir -p ${TARGET_ROOTFS}/etc/
		sh -c "echo 'include /etc/ld.so.conf.d/*.conf' > ${TARGET_ROOTFS}/etc/ld.so.conf"
	fi
	if [ ! -f "${TARGET_ROOTFS}/etc/ld.so.conf.d/arm-linux-gnueabihf.conf" ]; then
		mkdir -p ${TARGET_ROOTFS}/etc/ld.so.conf.d
		sh -c "echo '# Multiarch support' > ${TARGET_ROOTFS}/etc/ld.so.conf.d/arm-linux-gnueabihf.conf"
		sh -c "echo '/lib/arm-linux-gnueabihf' >> ${TARGET_ROOTFS}/etc/ld.so.conf.d/arm-linux-gnueabihf.conf"
		sh -c "echo '/usr/lib/arm-linux-gnueabihf' >> ${TARGET_ROOTFS}/etc/ld.so.conf.d/arm-linux-gnueabihf.conf"
	fi
}

# Install ARM cross compiler from linaro.org
install_linaro_cross_compiler() {
	local TOOLCHAIN
	local TOOLCHAIN_DOWNLOAD_URL
	local LINARO_PACKAGES_DIR
	local RETVAL

	if [ "${ARCH}" = "armv8" ]; then
		# gcc build with: --with-arch=armv8-a --with-fpu=neon-fp-armv8 -with-float=hard --with-mode=thumb --with-tls=gnu
		TOOLCHAIN="gcc-linaro-6.4.1-2017.11-x86_64_armv8l-linux-gnueabihf"
		TOOLCHAIN_DOWNLOAD_URL="https://releases.linaro.org/components/toolchain/binaries/6.4-2017.11/armv8l-linux-gnueabihf/${TOOLCHAIN}.tar.xz"
	else
		# gcc build with: --with-tune=cortex-a9 --with-arch=armv7-a --with-fpu=vfpv3-d16 --with-float=hard --with-mode=thumb
		TOOLCHAIN="gcc-linaro-6.4.1-2017.11-x86_64_arm-linux-gnueabihf"
		TOOLCHAIN_DOWNLOAD_URL="https://releases.linaro.org/components/toolchain/binaries/6.4-2017.11/arm-linux-gnueabihf/${TOOLCHAIN}.tar.xz"
	fi

	LINARO_PACKAGES_DIR="${INSTALL_PATH}/linaro_packages"
	rm -fr ${INSTALL_PATH}/${TOOLCHAIN}
	mkdir -p "${LINARO_PACKAGES_DIR}"
	if [ ! -f "${LINARO_PACKAGES_DIR}/${TOOLCHAIN}.tar.xz" ]; then
		echo "Download, cache and extract: ${TOOLCHAIN}.tar.xz"
		wget -nv -c -O "${LINARO_PACKAGES_DIR}/${TOOLCHAIN}.tar.xz.download" "${TOOLCHAIN_DOWNLOAD_URL}"
		RETVAL=$?
		if [ "x${RETVAL}" = "x0" ]; then
			mv -f "${LINARO_PACKAGES_DIR}/${TOOLCHAIN}.tar.xz.download" "${LINARO_PACKAGES_DIR}/${TOOLCHAIN}.tar.xz"
			tar -C ${INSTALL_PATH} --xz -xf "${LINARO_PACKAGES_DIR}/${TOOLCHAIN}.tar.xz"
		fi
	else
		echo "Extract from cache: ${TOOLCHAIN}.tar.xz"
		tar -C ${INSTALL_PATH} --xz -xf "${LINARO_PACKAGES_DIR}/${TOOLCHAIN}.tar.xz"
	fi

	chown -R root:root ${INSTALL_PATH}/${TOOLCHAIN}
	unlink ${TOOLCHAIN_SYMLINK_DIR} > /dev/null 2>&1
	ln -sf ${TOOLCHAIN} ${TOOLCHAIN_SYMLINK_DIR}
	echo "Symlink: ln -sf ${TOOLCHAIN} ${TOOLCHAIN_SYMLINK_DIR}"

	install_crosstool_ng_ldd
}

install_linaro_cross_compiler