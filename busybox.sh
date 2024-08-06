#!/bin/bash

set -e

BUSYBOX_VERSION="1.36.0"
BUSYBOX_TAR="busybox-${BUSYBOX_VERSION}.tar.bz2"
BUSYBOX_URL="https://busybox.net/downloads/${BUSYBOX_TAR}"
INSTALL_DIR="rootfs"

mkdir -p /tmp/busybox
mkdir -p ${INSTALL_DIR}

cd /tmp/busybox
wget ${BUSYBOX_URL}
tar xjf ${BUSYBOX_TAR}

cd busybox-${BUSYBOX_VERSION}

make menuconfig

make

make CONFIG_PREFIX=${INSTALL_DIR} install

cat << 'EOF' > ${INSTALL_DIR}/init
#!/bin/sh
mount -t proc proc /proc
mount -t sysfs sysfs /sys
exec /bin/sh
EOF

chmod +x ${INSTALL_DIR}/init

cd /tmp
rm -rf /tmp/busybox

echo "BusyBox has been installed to ${INSTALL_DIR}."
