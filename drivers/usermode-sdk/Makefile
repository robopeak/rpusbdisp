PREFIX:=/usr/lib/

CPPFLAGS:=--std c++11
INFRA_SOURCE=$(wildcard infra/src/util/*.cc)
INFRA_OBJECT=$(patsubst %.cc,%.o,$(INFRA_SOURCE))

LIBUSBX_WRAP_SOURCE=$(wildcard deps-wraps/libusbx_wrap/src/*.cc)
LIBUSBX_WRAP_OBJ=$(patsubst %.cc,%.o,${LIBUSBX_WRAP_SOURCE})

LIBRPUSBDISP_DRV_SOURCE=$(wildcard rpusbdisp-drv/src/*.cc)
LIBRPUSBDISP_DRV_OBJ=$(patsubst %.cc,%.o,${LIBRPUSBDISP_DRV_SOURCE})

RPUSBDISPDEMO_SRC=$(wildcard demo/src/*.cc)
RPUSBDISPDEMO_OBJ=$(patsubst %.cc,%.o,${RPUSBDISPDEMO_SRC})

.phony: all clean distclean document

all: build/rpusbdispdemo

clean:
	rm -rf build

distclean:
	rm -rf build
	find . -name "*.o" | xargs rm -rf

build/libinfra.a: $(INFRA_OBJECT)
	mkdir -p build
	ar rc $@ $(INFRA_OBJECT)

$(INFRA_OBJECT): $(INFRA_SOURCE)
	g++ -c -o $@ $(CPPFLAGS) -Iinfra/include $(patsubst %.o,%.cc,$@)

build/libdeps_wraps.a: build/deps_wraps/libusbxwrap.a
	cp $^ $@

build/deps_wraps/libusbxwrap.a: $(LIBUSBX_WRAP_OBJ)
	mkdir -p build/deps_wraps
	ar rc $@ $(LIBUSBX_WRAP_OBJ)

$(LIBUSBX_WRAP_OBJ):$(LIBUSBX_WRAP_SOURCE)
	g++ -c -o $@ $(CPPFLAGS) -Iinfra/include -Ideps-wraps/libusbx_wrap/include -I/usr/local/include/libusb-1.0 -I/usr/include/libusb-1.0 $(patsubst %.o,%.cc,$@)

build/librpusbdisp-drv.a: $(LIBRPUSBDISP_DRV_OBJ)
	mkdir -p build
	ar rc $@ $(LIBRPUSBDISP_DRV_OBJ)

$(LIBRPUSBDISP_DRV_OBJ): $(LIBRPUSBDISP_DRV_SOURCE)
	g++ -c -o $@ $(CPPFLAGS) -Iinfra/include -Ideps-wraps/libusbx_wrap/include -Irpusbdisp-drv/include $(patsubst %.o,%.cc,$@)

build/rpusbdispdemo: ${RPUSBDISPDEMO_OBJ} build/libinfra.a build/libdeps_wraps.a build/librpusbdisp-drv.a
	g++ -o $@ ${RPUSBDISPDEMO_OBJ} $(CPPFLAGS) -Lbuild -L/usr/local/lib -L/usr/lib -lrpusbdisp-drv -ldeps_wraps -linfra -lusb-1.0

${RPUSBDISPDEMO_OBJ}: ${RPUSBDISPDEMO_SRC}
	g++ -c -o $@ $(CPPFLAGS) -Iinfra/include -Ideps-wraps/libusbx_wrap/include -Irpusbdisp-drv/include $(patsubst %.o,%.cc,$@)

document:
	doxygen Doxygen
