cmd_Release/obd_cougar.node := c++ -bundle -undefined dynamic_lookup -Wl,-no_pie -Wl,-search_paths_first -mmacosx-version-min=10.7 -arch x86_64 -L./Release -stdlib=libc++  -o Release/obd_cougar.node Release/obj.target/obd_cougar/obd_cougar_lib.o Release/obj.target/obd_cougar/serial/macOS/InterfacesOSX.o -framework IOKit -framework CoreFoundation
