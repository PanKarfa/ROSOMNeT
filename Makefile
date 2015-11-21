INET_DIR ?= ${OMNETPP_ROOT}/../inet-3.1.1

CONFIGFILE = $(shell opp_configfilepath)
include $(CONFIGFILE)

all: checkmakefiles
	cd src && $(MAKE)

clean: checkmakefiles
	cd src && $(MAKE) clean

cleanall: checkmakefiles
	cd src && $(MAKE) clean
	rm -f src/Makefile
	rm -rf out/
	
# Create makefiles
makefiles:
	cd src && opp_makemake -f --deep -o rosomnet -O out \
	-I${OMNETPP_SRC_DIR}/common -I${OMNETPP_INCL_DIR}/platdep -I${OMNETPP_SRC_DIR}/envir \
	-I${INET_DIR}/src/linklayer/ieee80211/radio \
	-I${INET_DIR}/src/linklayer/ethernet \
	-I${INET_DIR}/src/networklayer/common \
	-I${INET_DIR}/src \
	-I${INET_DIR}/src/networklayer/icmpv6 \
	-I${INET_DIR}/src/world/obstacles \
	-I${INET_DIR}/src/networklayer/xmipv6 \
	-I${INET_DIR}/src/networklayer/manetrouting/base \
	-I${INET_DIR}/src/networklayer/contract \
	-I${INET_DIR}/src/mobility/models \
	-I${INET_DIR}/src/mobility \
	-I${INET_DIR}/src/networklayer/autorouting/ipv4 \
	-I${INET_DIR}/src/linklayer/ieee80211/mgmt \
	-I${INET_DIR}/src/util \
	-I${INET_DIR}/src/linklayer/common \
	-I${INET_DIR}/src/transport/contract \
	-I${INET_DIR}/src/status \
	-I${INET_DIR}/src/linklayer/radio/propagation \
	-I${INET_DIR}/src/linklayer/ieee80211/radio/errormodel \
	-I${INET_DIR}/src/linklayer/radio \
	-I${INET_DIR}/src/util/headerserializers/tcp \
	-I${INET_DIR}/src/networklayer/ipv4 \
	-I${INET_DIR}/src/util/headerserializers/ipv4 \
	-I${INET_DIR}/src/base \
	-I${INET_DIR}/src/util/headerserializers \
	-I${INET_DIR}/src/world/radio \
	-I${INET_DIR}/src/linklayer/ieee80211/mac \
	-I${INET_DIR}/src/networklayer/ipv6 \
	-I${INET_DIR}/src/transport/sctp \
	-I${INET_DIR}/src/util/headerserializers/udp \
	-I${INET_DIR}/src/networklayer/ipv6tunneling \
	-I${INET_DIR}/src/applications/pingapp \
	-I${INET_DIR}/src/battery/models \
	-I${INET_DIR}/src/util/headerserializers/sctp \
	-I${INET_DIR}/src/linklayer/contract \
	-I${INET_DIR}/src/networklayer/arp \
	-I${INET_DIR}/src/transport/tcp_common \
	-I${INET_DIR}/src/transport/udp \
	-I${INET_DIR}/src/mobility/static \
	-I${INET_DIR}/src/mobility/common \
	-I${INET_DIR}/src/mobility/contract \
	-L${INET_DIR}/out/$(CONFIGNAME)/src \
	-lINET -KINET_PROJ=${INET_DIR} -KV=1 \
	-L/opt/ros/indigo/lib \
	-lroscpp \
	-lrostime \
	-lrosconsole \
	-lroscpp_serialization


checkmakefiles: makefiles
	@if [ ! -f src/Makefile ]; then \
	echo; \
	echo '=========================================================='; \
	echo 'src/Makefile does not exist. Please use "make makefiles"'; \
	echo 'to generate it!'; \
	echo '=========================================================='; \
	echo; \
	exit 1; \
	fi

